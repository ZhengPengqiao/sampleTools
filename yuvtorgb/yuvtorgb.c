#include <stdio.h>  
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


static long U[256], V[256], Y1[256], Y2[256]; 
#ifndef MAX  
#define MAX(a, b) ((a) > (b) ? (a) : (b))  
#endif  
#ifndef MIN  
#define MIN(a, b) ((a) < (b) ? (a) : (b))  
#endif

#define FMT_YUYV 0
#define FMT_YVYU 1
#define FMT_UYVY 2
#define FMT_VYUY 3


#define FMT_RGB16 0
#define FMT_RGB24 1
#define FMT_RGB32 2

// 最大支持(9920)x(9920)
#define MAX_WIDTH 9920
#define MAX_HEIGHT 9920
#define MAX_DATA_LENGTH  MAX_WIDTH*MAX_HEIGHT*2
#define MAX_RGBABUFF_LENGTH  MAX_WIDTH*MAX_HEIGHT*4
unsigned char yuyvdata[MAX_DATA_LENGTH];
unsigned char rgbadata[MAX_RGBABUFF_LENGTH];


// 默认参数
int width=1280;
int height=720;
int yuyvdata_length = 1280*720*2;
int rgbadata_length = 1280*720*3;
char *yuv_filename=(char*)"yuyv.yuyv";
char *rgb_filename=(char*)"rgb.data";
int ofmt = FMT_YUYV;
int ifmt = FMT_RGB16;

/*
 * 函数名称 : init_yuv422p_table
 * 函数介绍 : 将FMT_YUYV,FMT_YVYU,FMT_UYVY,FMT_VYUY转换成rgb24格式
 * 参数介绍 : 生成查询表格
 */  
void init_yuv422p_table(void)  
{  
    int i;  
    static int init = 0;  
    if (init == 1) return;  
    // Initialize table  
    for (i = 0; i < 256; i++)  
    {   
        V[i]  = 15938 * i - 2221300;  
        U[i]  = 20238 * i - 2771300;  
        Y1[i] = 11644 * i;  
        Y2[i] = 19837 * i - 311710;  
    }  
  
    init = 1;  
} 

/*
 * 函数名称 : yuv422packed_to_rgb32
 * 函数介绍 : 将FMT_YUYV,FMT_YVYU,FMT_UYVY,FMT_VYUY转换成rgb24格式
 * 参数介绍 : typr : YUV422的格式
 *          yuv422p : YUV422的数据字节
 *          rgba : 生成的RGBA数据字节
 *          width,height : 图片的宽高
 */
void yuv422packed_to_rgb32(int type, unsigned char* yuv422p, unsigned char* rgba, int width, int height)  
{  
    int y  = 0;
    int cb = 0;
    int cr = 0;  
    int r, g, b;  
    int i = 0;  
    unsigned char* p;  
    unsigned char* p_rgb;  
  
    p = yuv422p;  
  
    p_rgb = rgba;  
  
    init_yuv422p_table();  
  
    for (i = 0; i < width * height / 2; i++)  
    {  
        switch(type)  
        {  
        case FMT_YUYV:  
            y  = p[0];  
            cb = p[1];  
            cr = p[3];  
            break;  
        case FMT_YVYU:  
            y  = p[0];  
            cr = p[1];  
            cb = p[3];  
            break;  
        case FMT_UYVY:  
            cb = p[0];  
            y  = p[1];  
            cr = p[2];  
            break;  
        case FMT_VYUY:  
            cr = p[0];  
            y  = p[1];  
            cb = p[2];  
            break;  
        default:  
            break;  
        }  
  
        r = MAX (0, MIN (255, (V[cr] + Y1[y])/10000));   //R value  
        b = MAX (0, MIN (255, (U[cb] + Y1[y])/10000));   //B value  
        g = MAX (0, MIN (255, (Y2[y] - 5094*(r) - 1942*(b))/10000)); //G value  
  
        // 此处可调整RGB排序，BMP图片排序为BGR  
        // 默认排序为：RGB  
        p_rgb[0] = r;  
        p_rgb[1] = g;  
        p_rgb[2] = b;  
        p_rgb[3] = 255;  
  
        switch(type)  
        {  
        case FMT_YUYV:  
        case FMT_YVYU:  
            y = p[2];  
            break;  
        case FMT_UYVY:  
        case FMT_VYUY:  
            y = p[3];  
            break;  
        default:  
            break;  
        }  
  
        r = MAX (0, MIN (255, (V[cr] + Y1[y])/10000));   //R value  
        b = MAX (0, MIN (255, (U[cb] + Y1[y])/10000));   //B value  
        g = MAX (0, MIN (255, (Y2[y] - 5094*(r) - 1942*(b))/10000)); //G value  
  
        p_rgb[4] = r;  
        p_rgb[5] = g;  
        p_rgb[6] = b;  
        p_rgb[7] = 255;  
  
        p += 4;  
        p_rgb += 8;  
    }  
}  

/*
 * 函数名称 : yuv422packed_to_rgb24
 * 函数介绍 : 将FMT_YUYV,FMT_YVYU,FMT_UYVY,FMT_VYUY转换成rgb24格式
 * 参数介绍 : typr : YUV422的格式
 *          yuv422p : YUV422的数据字节
 *          rgb : 生成的RGB数据字节
 *          width,height : 图片的宽高
 */
void yuv422packed_to_rgb24(int type, unsigned char* yuv422p, unsigned char* rgb, int width, int height)  
{  
    int y  = 0;
    int cb = 0;
    int cr = 0;  
    int r, g, b;  
    int i = 0;  
    unsigned char* p;  
    unsigned char* p_rgb;  
  
    p = yuv422p;  
  
    p_rgb = rgb;  
  
    init_yuv422p_table();  
  
    for (i = 0; i < width * height / 2; i++)  
    {  
        switch(type)  
        {  
        case FMT_YUYV:  
            y  = p[0];  
            cb = p[1];  
            cr = p[3];  
            break;  
        case FMT_YVYU:  
            y  = p[0];  
            cr = p[1];  
            cb = p[3];  
            break;  
        case FMT_UYVY:  
            cb = p[0];  
            y  = p[1];  
            cr = p[2];  
            break;  
        case FMT_VYUY:  
            cr = p[0];  
            y  = p[1];  
            cb = p[2];  
            break;  
        default:  
            break;  
        }  
  
        r = MAX (0, MIN (255, (V[cr] + Y1[y])/10000));   //R value  
        b = MAX (0, MIN (255, (U[cb] + Y1[y])/10000));   //B value  
        g = MAX (0, MIN (255, (Y2[y] - 5094*(r) - 1942*(b))/10000)); //G value  
  
        // 此处可调整RGB排序，BMP图片排序为BGR  
        // 默认排序为：RGB  
        p_rgb[0] = r;  
        p_rgb[1] = g;  
        p_rgb[2] = b;  
  
        switch(type)  
        {  
        case FMT_YUYV:  
        case FMT_YVYU:  
            y = p[2];  
            break;  
        case FMT_UYVY:  
        case FMT_VYUY:  
            y = p[3];  
            break;  
        default:  
            break;  
        }  
  
        r = MAX (0, MIN (255, (V[cr] + Y1[y])/10000));   //R value  
        b = MAX (0, MIN (255, (U[cb] + Y1[y])/10000));   //B value  
        g = MAX (0, MIN (255, (Y2[y] - 5094*(r) - 1942*(b))/10000)); //G value  
  
        p_rgb[3] = r;  
        p_rgb[4] = g;  
        p_rgb[5] = b;  
  
        p += 4;  
        p_rgb += 6;  
    }  
}  

/*
 * 函数名称 : yuv422packed_to_rgb16
 * 函数介绍 : 将FMT_YUYV,FMT_YVYU,FMT_UYVY,FMT_VYUY转换成rgb24格式
 * 参数介绍 : typr : YUV422的格式
 *          yuv422p : YUV422的数据字节
 *          rgb : 生成的RGB数据字节
 *          width,height : 图片的宽高
 */
void yuv422packed_to_rgb16(int type, unsigned char* yuv422p, unsigned char* rgb, int width, int height)  
{  
    int y  = 0;
    int cb = 0;
    int cr = 0;  
    unsigned int r, g, b;  
    int i = 0;
    unsigned char* p;  
    unsigned char* p_rgb;  
    unsigned int rgbval;
  
    p = yuv422p;  
  
    p_rgb = rgb;  
  
    init_yuv422p_table();  
  
    for (i = 0; i < width * height / 2; i++)  
    {  
        switch(type)  
        {  
        case FMT_YUYV:  
            y  = p[0];  
            cb = p[1];  
            cr = p[3];  
            break;  
        case FMT_YVYU:  
            y  = p[0];  
            cr = p[1];  
            cb = p[3];  
            break;  
        case FMT_UYVY:  
            cb = p[0];  
            y  = p[1];  
            cr = p[2];  
            break;  
        case FMT_VYUY:  
            cr = p[0];  
            y  = p[1];  
            cb = p[2];  
            break;  
        default:  
            break;  
        }  
  
        r = MAX (0, MIN (255, (V[cr] + Y1[y])/10000));   //R value  
        b = MAX (0, MIN (255, (U[cb] + Y1[y])/10000));   //B value  
        g = MAX (0, MIN (255, (Y2[y] - 5094*(r) - 1942*(b))/10000)); //G value  
  
        // 此处可调整RGB排序，BMP图片排序为BGR  
        // 默认排序为：RGB  

        rgbval = ((r << 8) & 0xF800) | ((g << 3) & 0x7E0) | (b >> 3);
        p_rgb[0] = (rgbval)&0xFF;
        p_rgb[1] = (rgbval>>8)&0xFF;
  
        switch(type)  
        {  
        case FMT_YUYV:
        case FMT_YVYU:  
            y = p[2];  
            break;  
        case FMT_UYVY:  
        case FMT_VYUY:  
            y = p[3];
            break;
        default:
            break;  
        }  
  
        r = MAX (0, MIN (255, (V[cr] + Y1[y])/10000));   //R value  
        b = MAX (0, MIN (255, (U[cb] + Y1[y])/10000));   //B value  
        g = MAX (0, MIN (255, (Y2[y] - 5094*(r) - 1942*(b))/10000)); //G value  
        rgbval = ((r << 8) & 0xF800) | ((g << 3) & 0x7E0) | (b >> 3);
        p_rgb[2] = (rgbval)&0xFF;
        p_rgb[3] = (rgbval>>8)&0xFF;
  
        p += 4;  
        p_rgb += 4;  
    }  
}  


void showHelp()
{
	printf("./programe options \n");	
	printf("USED:");
	printf("	-help: show help info\n");
	printf("	-if filename:  set input file (default %s)\n", yuv_filename);
	printf("	-of FileName:  set output file (default %s)\n", rgb_filename);
	printf("	-width value:  set width (default %d)\n", width);
	printf("	-height value: set height (default %d)\n", height);
	printf("	-ifmt value: frame Format(FMT_NV12=0/FMT_YUYV=1/FMT_UYVY=2) (define %d)\n", ifmt);
	printf("	-ofmt value: frame Format(FMT_RGB16=0/FMT_RGB24=1/FMT_RGB32=2) (define %d)\n", ofmt);
}

int checkParam(int argc,char **argv)
{
	int i = 0;
	for(i = 1;i < argc;i++)
	{
		if( strcmp("-help", argv[i]) == 0 )
		{
			showHelp();
			return -1;
		}
		else if( strcmp("-if", argv[i]) == 0 )
		{
			yuv_filename = argv[i+1];
			i++;
		}
		else if( strcmp("-of", argv[i]) ==0 )
		{
			rgb_filename = argv[i+1];
			i++;
		}
		else if( strcmp("-width", argv[i]) == 0)
		{
			width = atoi(argv[i+1]);
			i++;
		}
		else if( strcmp("-height", argv[i]) == 0)
		{
			height = atoi(argv[i+1]);
			i++;
		}
		else if( strcmp("-ifmt", argv[i]) == 0)
		{
			ifmt = atoi(argv[i+1]);
			i++;
		}
		else if( strcmp("-ofmt", argv[i]) == 0)
		{
			ofmt = atoi(argv[i+1]);
			i++;
		}
		else
		{
			printf("param %s is not support \n\n", argv[i]);
			showHelp();
			return -1;
		}
	}
	return 0;
}

int showParam()
{
    printf("width=%d\n", width);
    printf("height=%d\n",height);
    printf("yuyvdata_length=%d\n",yuyvdata_length);
    printf("rgbadata_length=%d\n",rgbadata_length);
    printf("yuv_filename=%s\n",yuv_filename);
    printf("rgb_filename=%s\n",rgb_filename);
    
    switch(ifmt)
    {
        case FMT_UYVY:
        {
            printf("ifmt=%s\n", "FMT_UYVY");
        }
        break;
        case FMT_VYUY:
        {
            printf("ifmt=%s\n", "FMT_VYUY");
        }
        break;
        case FMT_YUYV:
        {
            printf("ifmt=%s\n", "FMT_YUYV");
        }
        break;
        case FMT_YVYU:
        {
            printf("ifmt=%s\n", "FMT_YVYU");
        }
        break;
        default: 
        {
            printf("ifmt not support format");
            return 0;
        }
    }

    switch(ofmt)
    {
        case FMT_RGB16:
        {
            printf("ofmt=%s\n", "FMT_RGB16");
        }
        break;
        case FMT_RGB24:
        {
            printf("ofmt=%s\n", "FMT_RGB24");
        }
        break;
        case FMT_RGB32:
        {
            printf("ofmt=%s\n", "FMT_RGB32");
        }
        break;
        default: 
        {
            printf("ofmt not support format");
            return 0;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{

    int rgbfb = 0;
    int yuvfb = 0;
    int ret = 0;

	if ( checkParam(argc, argv) )
	{
		return -1;
	}

    if ( height > MAX_HEIGHT )
    {
		printf("height is to larger, must lower %d\n", MAX_HEIGHT);
		return 0;
    }

    if ( width > MAX_WIDTH )
    {
		printf("width is to larger, must lower %d\n", MAX_WIDTH);
		return 0;
    }

    switch(ifmt)
    {
        case FMT_UYVY:
        {
            yuyvdata_length = width*height*2;
        }
        break;
        case FMT_VYUY:
        {
            yuyvdata_length = width*height*2;
        }
        break;
        case FMT_YUYV:
        {
            yuyvdata_length = width*height*2;
        }
        break;
        case FMT_YVYU:
        {
            yuyvdata_length = width*height*2;
        }
        break;
        default: 
        {
            printf("ifmt not support format");
            return 0;
        }
    }

    switch(ofmt)
    {
        case FMT_RGB16:
        {
            rgbadata_length = width*height*2;
        }
        break;
        case FMT_RGB24:
        {
            rgbadata_length = width*height*3;
        }
        break;
        case FMT_RGB32:
        {
            rgbadata_length = width*height*4;
        }
        break;
        default: 
        {
            printf("ofmt not support format");
            return 0;
        }
    }

    showParam();

	yuvfb = open(yuv_filename, O_RDWR, 0777);
	if( yuvfb <= 0 )
	{
		printf("open %s err\n", yuv_filename);
		return 0;
	}
    else
    {
        /* code */
    }
    

	rgbfb = open(rgb_filename, O_RDWR|O_CREAT, 0777);
	if( yuvfb <= 0 )
	{
		printf("open %s err\n", rgb_filename);
		return 0;
	}
    else
    {
		printf("open %s ok\n", rgb_filename);
    }
    

	ret = read(yuvfb, yuyvdata, yuyvdata_length);
	if( ret <= 0 )
	{
		printf("read err form %s \n", yuv_filename);
	}
	else
	{
		printf("read %d byte form %s\n", ret, yuv_filename);
	}

    switch(ofmt)
    {
        case FMT_RGB16:
        {
	        yuv422packed_to_rgb16(ifmt, yuyvdata, rgbadata, width, height);
        }
        break;
        case FMT_RGB24:
        {
	        yuv422packed_to_rgb24(ifmt, yuyvdata, rgbadata, width, height);
        }
        break;
        case FMT_RGB32:
        {
	        yuv422packed_to_rgb32(ifmt, yuyvdata, rgbadata, width, height);
        }
        break;
        default: 
        {
            printf("ofmt not support format");
            return 0;
        }
    }
	
	ret = write(rgbfb, rgbadata, rgbadata_length);
	if( ret <= 0 )
	{
		printf("write err to %s , err=%d\n", rgb_filename,ret);
	}
	else
	{
		printf("write %d byte to %s\n", ret, rgb_filename);
	}
	

	close(yuvfb);
	close(rgbfb);
	return 0;
}