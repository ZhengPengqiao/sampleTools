#include <stdio.h>  
#include <string.h>
#include <stdlib.h>

/**
 * @brief 全局变量,记录着从参数中解析的信息 
 */
int value = 0;


struct fb_videomode {
     const char *name; /*< BT656-720P */
     u32 refresh; /*< 30 : 帧数*/
     u32 xres; /*< 1280 : width有效值*/
     u32 yres; /*< 720 ： height有效值*/
     u32 pixclock; /*< 13889 ： 每个像素时间(单位:皮秒ps) */
     u32 left_margin; /*< 17 ： 左消隐*/
     u32 right_margin; /*< 3 */
     u32 upper_margin; /*< 52 */
     u32 lower_margin; /*< 2 */
     u32 hsync_len; /*< 200 */
     u32 vsync_len; /*< 25 */
     u32 sync; /*< 0 */
     u32 vmode; /*< FB_VMODE_NONINTERLACED */
     u32 flag; /*< FB_MODE_IS_DETAILED */
};

#define modeitem(id,vf,vb,vs,hf,fb,fs) {"BT656-720P0"#id, 30, 1280, 720, 13889, vf, vb, hf, hf, vs, fs, 0, FB_VMODE_NONINTERLACED, FB_MODE_IS_DETAILED,},


void showHelp()
{
	printf("./programe options \n");	
	printf("USED:");
	printf("	-help: show help info\n");
	printf("	-val value: set A Value Will Need (default %d)\n", value);
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
		else if( strcmp("-val", argv[i]) == 0)
		{
			value = atoi(argv[i+1]);
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
    printf("{");
    printf("%s, %u, %u, %u, %u,", mode.name, mode.refresh, mode.xres, mode.yres, mode.pixclock);
    printf("%u, %u,", mode.left_margin, mode.right_margin);
    printf("%u, %u,",mode.upper_margin, mode.lower_margin);
    printf("%u, %u,", mode.hsync_len, mode.vsync_len);
    printf("%u,", mode.sync);
    printf("FB_VMODE_NONINTERLACED,");
    printf("FB_MODE_IS_DETAILED,},");
}

int main(int argc,char **argv)
{
	int len = 0;
	if( checkParam(argc, argv) )
	{
		return -1;
	}
	
	struct fb_videomode bt656if_modedb[] = {
		modeitem(1,2,3,4,5,6,7)
		modeitem(2,3,4,5,6,7,8)
	}

	len = sizeof(bt656if_modedb) / sizeof(bt656if_modedb[0]);
	for (int i = 0; i < len; i++)
	{
		display_fb_videomode(bt656if_modedb[i]);
	}
	return 0;
};