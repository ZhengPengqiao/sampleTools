#include <stdio.h>  
#include <string.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief 全局变量,记录着从参数中解析的信息 
 */
int twidth = 1500;
int theight = 820;
int width = 1280;
int height = 720;
int click = 72000000;

typedef unsigned int u32;
#define FB_VMODE_NONINTERLACED 0
#define FB_MODE_IS_DETAILED 0

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

#define modeitem(id, click, left, right, upper, lower, hsync, vsync) {"BT656-720P"#id, 30, 1280, 720, pow(10,12)/click, left, right, upper, lower, hsync, vsync, 0, FB_VMODE_NONINTERLACED, FB_MODE_IS_DETAILED,},
#define	setModeItem(id, click, left, right, upper, lower) modeitem(id, click, left, right, upper, lower, sw-left-right, sh-upper-lower)

void showHelp()
{
	printf("./programe options \n");	
	printf("USED:");
	printf("	-help: show help info\n");
	printf("	-twidth value: set total width (default %d)\n", twidth);
	printf("	-theight value: set total height (default %d)\n", theight);
	printf("	-width value: set width (default %d)\n", width);
	printf("	-height value: set height (default %d)\n", height);
	printf("	-click value: set click (default %d)\n", click);
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
		else if( strcmp("-height", argv[i]) == 0)
		{
			height = atoi(argv[i+1]);
			i++;
		}
		else if( strcmp("-width", argv[i]) == 0)
		{
			width = atoi(argv[i+1]);
			i++;
		}
		else if( strcmp("-twidth", argv[i]) == 0)
		{
			twidth = atoi(argv[i+1]);
			i++;
		}
		else if( strcmp("-theight", argv[i]) == 0)
		{
			theight = atoi(argv[i+1]);
			i++;
		}
		else if( strcmp("-click", argv[i]) == 0)
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
	int len = 0;
	int sh = theight-height;
	int sw = twidth-width;
	if( checkParam(argc, argv) )
	{
		return -1;
	}

	printf("twidth=%d, theight=%d, width=%d, height=%d, click=%d\n", twidth, theight, width, height, click);
	
	struct fb_videomode bt656if_modedb[] = {
		setModeItem(1, click, 10, 10, 15, 10)
		setModeItem(2, click, 15, 20, 20, 20)
		setModeItem(3, click, 20, 30, 25, 30)
		setModeItem(4, click, 25, 20, 30, 40)
		setModeItem(5, click, 30, 30, 35, 40)
		setModeItem(6, click, 45, 40, 40, 40)
		setModeItem(7, click, 55, 50, 45, 40)
		setModeItem(8, click, 60, 60, 50, 40)
		setModeItem(9, click, 65, 50, 55, 40)
		setModeItem(10, click, 70, 70, 60, 40)
		setModeItem(11, click, 75, 80, 25, 40)
		setModeItem(12, click, 80, 20, 30, 40)
		setModeItem(13, click, 90, 20, 40, 40)
		setModeItem(14, click, 100, 30, 45, 40)
		setModeItem(15, click, 110, 40, 50, 35)
		setModeItem(16, click, 120, 50, 60, 20)
		setModeItem(17, click, 130, 60, 70, 10)
		setModeItem(18, click, 140, 70, 80, 10)
	};

	len = sizeof(bt656if_modedb) / sizeof(bt656if_modedb[0]);
	for (int i = 0; i < len; i++)
	{
		display_fb_videomode(bt656if_modedb[i]);
	}

	return 0;
};