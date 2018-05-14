#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define OPEN_RDWR O_RDWR
#define OPEN_CREAT O_CREAT 
#define OPEN_RDONLY O_RDONLY

int openfile(char *muldir)
{
    int i,len;
    int ret = 0;
    char str[512];
    strncpy(str, muldir, 512);
    len=strlen(str);
    for( i=0; i<len; i++ )
    {  
        if( str[i]=='/' && i != 0)  
        {
            str[i] = '\0';
            if( access(str,0)!=0 )
            {
                printf("str[%d]=%s\n", i, str);
                ret = mkdir( str, 0777 );
                if( ret != 0 )
                {
                    return ret;
                }
            }
            str[i]='/';
        }
    }

    if( len>0 && access(str,0)!=0 )
    {
        printf("File=%s\n", str);
        int fb = open(str, OPEN_RDWR|OPEN_CREAT,0600);
        if( fb <= 0 )
        {
            return -100;
        }
        ret = write(fb, str, strlen(str));
        close(fb);
    }
    return ret;
}

int main(int argc, char const **argv)
{
    int ret = 0;
    ret = openfile("a/b/c/d/");
    if( ret != 0 )
    {
        printf("err openfile a/b/c/d/d");
    }
    openfile("./aa/bb/cc/dd/");
    if( ret != 0 )
    {
        printf("err openfile ./aa/bb/cc/dd/");
    }
    openfile("/home/zpq/WorkSpaces/workspaces/sampleTools/aaa/bbb/ccc/ddd/");
    if( ret != 0 )
    {
        printf("err openfile /home/zpq/WorkSpaces/workspaces/sampleTools/aaa/bbb/ccc/ddd/");
    }
    openfile("/home/zpq/WorkSpaces/workspaces/sampleTools/aaaa/bbbb/cccc/dddd/abcd.txt");
    if( ret != 0 )
    {
        printf("err openfile /home/zpq/WorkSpaces/workspaces/sampleTools/aaaa/bbbb/cccc/dddd/abcd.txt");
    }
    return 0;
}
