#include <stdio.h>  
#include <string.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief 全局变量,记录着从参数中解析的信息 
 */
int width = 1280;
int height = 720;
int click = 72000000;
int frame = 30;

typedef unsigned int u32;
#define FB_VMODE_NONINTERLACED 0
#define FB_MODE_IS_DETAILED 0
#define LCDNAME "BT656-720P"

struct fb_videomode {
     const char *name; /*< BT656-720P */
     u32 refresh; /*< 帧数*/
     u32 xres; /*< width有效值*/
     u32 yres; /*< height有效值*/
     u32 pixclock; /*< 每个像素时间(单位:皮秒ps) */
     u32 left_margin; /*< 左消隐*/
     u32 right_margin; /*<  */
     u32 upper_margin; /*<  */
     u32 lower_margin; /*<  */
     u32 hsync_len; /*<  */
     u32 vsync_len; /*<  */
     u32 sync; /*< 0 */
     u32 vmode; /*< FB_VMODE_NONINTERLACED */
     u32 flag; /*< FB_MODE_IS_DETAILED */
};

void showHelp()
{
	printf("./programe options \n");	
	printf("USED:");
	printf("	-help: show help info\n");
	printf("	-w value: set width (default %d)\n", width);
	printf("	-f value: set Frame (default %d)\n", frame);
	printf("	-h value: set height (default %d)\n", height);
	printf("	-clk value: set click (default %d)\n", click);
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
		else if( strcmp("-h", argv[i]) == 0)
		{
			height = atoi(argv[i+1]);
			i++;
		}
		else if( strcmp("-w", argv[i]) == 0)
		{
			width = atoi(argv[i+1]);
			i++;
		}
		else if( strcmp("-f", argv[i]) == 0)
		{
			frame = atoi(argv[i+1]);
			i++;
		}
		else if( strcmp("-clk", argv[i]) == 0)
		{
			click = atoi(argv[i+1]);
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

int display_fb_videomode(struct fb_videomode mode)
{
    printf("{\n");
    printf("\"%s\", %u, %u, %u, %u,\n", mode.name, mode.refresh, mode.xres, mode.yres, mode.pixclock);
    printf("%u, %u,\n", mode.left_margin, mode.right_margin);
    printf("%u, %u,\n",mode.upper_margin, mode.lower_margin);
    printf("%u, %u,\n", mode.hsync_len, mode.vsync_len);
    printf("%u,\n", mode.sync);
    printf("FB_VMODE_NONINTERLACED,\n");
    printf("FB_MODE_IS_DETAILED,},\n");
}

int main(int argc,char **argv)
{
	struct fb_videomode bt656if_modedb;
	int i = 0;
	int twTotal = 0;
	int tw,th;
	if( checkParam(argc, argv) )
	{
		return -1;
	}

	twTotal = click/2/frame;

	bt656if_modedb.name = LCDNAME;
	bt656if_modedb.refresh = frame;
	bt656if_modedb.xres = width;
	bt656if_modedb.yres = height;
	bt656if_modedb.pixclock = pow(10, 12)/click;
	bt656if_modedb.left_margin = 2;
	bt656if_modedb.right_margin = 2;
	bt656if_modedb.upper_margin = 0;
	bt656if_modedb.lower_margin = 0;
	bt656if_modedb.hsync_len = 0;
	bt656if_modedb.vsync_len = 1;
	bt656if_modedb.sync = 0;
	bt656if_modedb.vmode = FB_VMODE_NONINTERLACED;
	bt656if_modedb.flag = FB_MODE_IS_DETAILED;

	for( i = width; i < twTotal/height; i++ )
	{
		tw = i;
		th = twTotal/i;
		
		if( (th*tw) == twTotal )
		{
			printf("\n");
			printf("Tw=%d Th=%d width=%d, height=%d, click=%d, frame=%d twTotal=%d\n", tw, th, width, height, click, frame, twTotal);
			bt656if_modedb.hsync_len = (tw-width)*2;
			display_fb_videomode(bt656if_modedb);
		}
	}

	return 0;
};