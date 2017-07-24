#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
 

/*
 * 函数名称 ： copyFile
 * 函数介绍 ： 将formFile文件拷贝到tofile文件中
 * 参数介绍 ： formFile:需要拷贝的文件,   toFile:要拷贝的位置
 * 返回值   ： 0: 正确,   -1:失败
 */
int copyFile(const char * formFile, const char * toFile)
{
    #define BUFFER_SIZE 1024
    int from_fd,to_fd;
    int bytes_read,bytes_write;
    char buffer[BUFFER_SIZE];
    char *ptr;
    /* 打开源文件 */
    if((from_fd=open( formFile, O_RDONLY))==-1)   /*open file readonly,返回-1表示出错，否则返回文件描述符*/
    {
        fprintf(stderr,"Open %s Error:%s\n", formFile,strerror(errno));
        return -1;
    }
 
    /* 创建目的文件 */
    /* 使用了O_CREAT选项-创建文件,open()函数需要第3个参数,
      mode=S_IRUSR|S_IWUSR表示S_IRUSR 用户可以读 S_IWUSR 用户可以写*/
    if((to_fd=open( toFile, O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1) 
    {
        fprintf(stderr,"Open %s Error:%s\n", toFile,strerror(errno));
        return -1;
    }

    /* 以下代码是一个经典的拷贝文件的代码 */
    while( (bytes_read=read( from_fd, buffer,BUFFER_SIZE)) )
    {
        /* 一个致命的错误发生了 */
        if((bytes_read==-1)&&(errno!=EINTR))
        { 
            return -1;
        }
        else if(bytes_read>0)
        {
            ptr=buffer;
            while( (bytes_write=write( to_fd, ptr, bytes_read)) )
            {
                /* 一个致命错误发生了 */
                if((bytes_write==-1)&&(errno!=EINTR))
                {
                    return -1;
                }
                /* 写完了所有读的字节 */
                else if(bytes_write==bytes_read) 
                {
                    break;
                }
                /* 只写了一部分,继续写 */
                else if(bytes_write>0)
                {
                    ptr += bytes_write;
                    bytes_read -= bytes_write;
                }
            }
            /* 写的时候发生的致命错误 */
            if( bytes_write == -1 )
            {
                return -1;
            }
        }
    }
    close(from_fd);
    close(to_fd);
    return 0;
}

int main(int argc,char **argv)
{
    if(argc!=3)
    {
        fprintf(stderr,"Usage:%s fromfile tofile\n\a",argv[0]);
        return -1;
    }

    if ( copyFile(argv[1], argv[2]) == 0 )
    {
        printf("copy file ok\n");
    }
    else
    {
        printf("copy file failure\n");
    }
    return 0;
} 