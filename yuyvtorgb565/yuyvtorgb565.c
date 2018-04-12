#include <stdio.h>  
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define YUYV_FILENAME "yuyv.yuyv"
#define RGB565_FILENAME "rgb565.data"


static long U[256], V[256], Y1[256], Y2[256]; 
#ifndef MAX  
#define MAX(a, b) ((a) > (b) ? (a) : (b))  
#endif  
#ifndef MIN  
#define MIN(a, b) ((a) < (b) ? (a) : (b))  
#endif  
#define WIDTH 1280
#define HEIGHT 720
#define DATA_LENGTH  WIDTH*HEIGHT*2
#define RGB888DATA_LENGTH  WIDTH*HEIGHT*3
#define RGBA8888DATA_LENGTH  WIDTH*HEIGHT*4
#define RGB565DATA_LENGTH  WIDTH*HEIGHT*2
#define FMT_YUYV 0
#define FMT_YVYU 1
#define FMT_UYVY 2
#define FMT_VYUY 3
unsigned char yuyvdata[DATA_LENGTH];
unsigned char rgb565data[RGBA8888DATA_LENGTH];


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


int main(int argc,char **argv)
{

	int rgbfb = 0;
	int yuvfb = 0;
	int ret = 0;
	yuvfb = open(YUYV_FILENAME, O_RDWR, 777);
	if( yuvfb <= 0 )
	{
		printf("open %s err\n", YUYV_FILENAME);
		return 0;
	}
	rgbfb = open(RGB565_FILENAME, O_RDWR|O_CREAT, 777);
	if( yuvfb <= 0 )
	{
		printf("open %s err\n", YUYV_FILENAME);
		return 0;
	}

	ret = read(yuvfb, yuyvdata, DATA_LENGTH);
	if( ret <= 0 )
	{
		printf("read err form %s \n", YUYV_FILENAME);
	}
	else
	{
		printf("read %d byte form %s\n", ret, YUYV_FILENAME);
	}
	yuv422packed_to_rgb16(FMT_YUYV, yuyvdata, rgb565data, WIDTH, HEIGHT);
	
	ret = write(rgbfb, rgb565data, RGB565DATA_LENGTH);
	if( ret <= 0 )
	{
		printf("read err form %s \n", RGB565_FILENAME);
	}
	else
	{
		printf("write %d byte to %s\n", ret, RGB565_FILENAME);
	}
	

	close(yuvfb);
	close(rgbfb);
	return 0;
}