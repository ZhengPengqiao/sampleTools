#include <stdio.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <string.h> 
#include <unistd.h> 
#include <linux/fb.h>  
#include <sys/mman.h>  
#include <sys/ioctl.h>  
#include <arpa/inet.h>
#include <sys/time.h>  
#include "bmp.h"


char temp[256];

int main ( int argc, char *argv[] )  
{  
    BmpInfo *bmpInfo = NULL;
    int i, j;
    FILE* pfile = NULL;
    if(argc == 3)
    {
        bmpInfo = new BmpInfo(argv[1]);
        /*BMP的上下是翻转的，这里将图像数据翻转过来*/
        bmpInfo->MirrorByV();
        printf("fileName=%s  width=%d height=%d length=%d\n", bmpInfo->fileName, bmpInfo->imagewidth, bmpInfo->imageheight, bmpInfo->pixellength);
        pfile=fopen(argv[2], "ab+");
        if(pfile == NULL)
        {
            printf("open error: %s\n", argv[2]);
            delete bmpInfo;
            return 0;
        }
        //将数据存储文件
        sprintf(temp, "%s = { \n", argv[2]);
        fwrite(temp, 1, strlen(temp), pfile);

        for(int i = 0; i < bmpInfo->imageheight; i++)
        {

            for(int j = 0; j < bmpInfo->imagewidth; j++)
            {
                //将数据存储文件
                sprintf(temp, "0x%02x,0x%02x,0x%02x,",
                        (unsigned char)bmpInfo->pixeldata[i*bmpInfo->imagewidth*3+j*3+0],
                        (unsigned char)bmpInfo->pixeldata[i*bmpInfo->imagewidth*3+j*3+1],
                        (unsigned char)bmpInfo->pixeldata[i*bmpInfo->imagewidth*3+j*3+2]);
                fwrite(temp, 1, strlen(temp), pfile);
            }
            sprintf(temp, "//this %d line\n", i);
            fwrite(temp, 1, strlen(temp), pfile);
        }
        sprintf(temp, "};\n");
        fwrite(temp, 1, strlen(temp), pfile);
        fclose(pfile);
        delete bmpInfo;
    }
    else
    {
        printf("Used: %s fileName outfilename\n", argv[0]);
    }
    return 0;  
}  
