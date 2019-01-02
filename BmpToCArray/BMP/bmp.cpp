/*
 * 文件名称 : bmp.c
 * 创建时间 : 2017-08-21
 * 文件介绍 : 用于读写BMP文件
 */
#include "bmp.h"
#include<cstdio>
#include<cstdlib>
#include<cstring>

/*
 * 函数名称 : BmpInfo
 * 函数介绍 : 构造函数
 * 参数介绍 : fileName : 要加载的文件名
 * 返回值   : 无
 */
BmpInfo::BmpInfo(char* inFileName)
{
    int bgrStart = 0;
    int ret;
    
    imagewidth = 0;
    imageheight = 0;
    pixellength = 0;
    pixeldata = NULL;

    fileName = (char *)malloc( strlen(inFileName)+1 );
    strcpy(fileName,inFileName);
    //打开文件
    FILE* pfile=fopen(fileName,"rb");

    if(pfile == 0) 
    {
        perror("fopen Err:");
        return;
    }

    //读取图像大小
    fseek(pfile,0x0012,SEEK_SET);
    ret = fread(&imagewidth,sizeof(imagewidth),1,pfile);
    if ( ret < 0 )
    {
        perror("read imagewidth Err:");
        return;
    }

    ret = fread(&imageheight,sizeof(imageheight),1,pfile);
    if ( ret < 0 )
    {
        perror("read imageheight Err:");
        return;
    }
    //读取数据的开始位置
    fseek(pfile,0x0A,SEEK_SET);
    ret = fread(&bgrStart,sizeof(bgrStart),1,pfile);
    if ( ret < 0 )
    {
        perror("read bgrStart Err:");
        return;
    }

    //计算像素数据长度
    pixellength=imagewidth*3;
    while(pixellength%4 != 0)
    {
        pixellength++;
    }

    pixellength *= imageheight;
    //读取像素数据
    pixeldata = (char *)malloc(pixellength);
    if(pixeldata == NULL)
    {
        return;
    }

    fseek(pfile, bgrStart,SEEK_SET);
    ret = fread(pixeldata,pixellength,1,pfile);
    if ( ret < 0 )
    {
        perror("read pixeldata Err:");
        return;
    }
    //关闭文件
    fclose(pfile);
}


/*
 * 函数名称 : MirrorByV
 * 函数介绍 : 垂直自身镜像
 * 参数介绍 : 无
 * 返回值   : -1:失败，  0：成功, -2:内存分配失败
 */
int BmpInfo::MirrorByV()
{
    if( pixeldata == NULL )
    {
        return -1;
    }

    int offset = 0;
    //读取像素数据
    char* pixeldatab = (char *)malloc(pixellength);
    if(pixeldatab == NULL)
    {
        return -2;
    }
    
    for(int i = imageheight-1; i >= 0; i--)
    {
        for(int j = 0; j < imagewidth; j++)
        {
            pixeldatab[offset++] = pixeldata[i*imagewidth*3+j*3+0];
            pixeldatab[offset++] = pixeldata[i*imagewidth*3+j*3+1];
            pixeldatab[offset++] = pixeldata[i*imagewidth*3+j*3+2];
        }
    }

    free(pixeldata);
    pixeldata = pixeldatab;
}


/*
 * 函数名称 : ~BmpInfo
 * 函数介绍 : 析构函数
 * 参数介绍 : 无
 * 返回值   : 无
 */
BmpInfo::~BmpInfo()
{
    if( fileName != NULL )
    {
        free(fileName);
    }
    
    if( pixeldata != NULL )
    {
        free(pixeldata);
    }
}
