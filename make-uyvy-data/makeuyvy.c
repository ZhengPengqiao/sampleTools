#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <signal.h>       /* for signal */  
#include <execinfo.h>     /* for backtrace() */  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FMT_NV12 0
#define FMT_YUYV 1
#define FMT_UYVY 2

int frameSize = 0;
int width = 1280;
int height = 720;
char *filename = (char*)"./tmp.nv12";
int frameCount = 90;
int fmt = FMT_NV12;
char *frameBuff = NULL;
int fb;

static void frame_render(int frameCount)
{
	int colorIndex = 0;
	static int frame = 0;
	int ret;
	char *disY = NULL;
	char *disUV = NULL;
	int tw = 0;
	while(frame < frameCount)
	{
		switch( fmt )
		{
			case FMT_NV12:
				disY = frameBuff;
				disUV = frameBuff+width*height;
				for(int i = 0; i < height; i++)
				{
					for(int j = 0; j < width; j++)
					{
						disY[i*width+j] = (j+colorIndex)%255;
					}
				}

				for(int i = 0; i < height; i++)
				{
					for(int j = 0; j < width/4; j++)
					{
						disUV[(i*width/4+j)*2+0] = 90;
						disUV[(i*width/4+j)*2+1] = 90;
					}
				}
				colorIndex++;
			break;
			case FMT_YUYV:
				tw = width/2;
				for(int i = 0; i < height; i++)
				{
					for(int j = 0; j < width; j++)
					{
						frameBuff[(i*width+j)*2] = (j+colorIndex)%255;
					}
				}

				for(int i = 0; i < height; i++)
				{
					for(int j = 0; j < tw; j++)
					{
						frameBuff[(i*tw+j)*4+1] = 90;
						frameBuff[(i*tw+j)*4+3] = 90;
					}
				}
				colorIndex++;
			break;
			case FMT_UYVY:
				tw = width/2;
				for(int i = 0; i < height; i++)
				{
					for(int j = 0; j < width; j++)
					{
						frameBuff[(i*width+j)*2+1] = (j+colorIndex)%255;
					}
				}
				for(int i = 0; i < height; i++)
				{
					for(int j = 0; j < tw; j++)
					{
						frameBuff[(i*tw+j)*4+0] = 90;
						frameBuff[(i*tw+j)*4+2] = 90;
					}
				}
				colorIndex++;
			break;
		}
		//frameBuff
		ret = write(fb, frameBuff, frameSize);
		printf("ret = %d\n", ret);
		frame++;
	}
}

void showHelp()
{
    printf("USED:\n");
    printf("-H : HELP\n");
    printf("-w : yuv data width (define 1280)\n");
    printf("-h : yuv data height (define 720)\n");
    printf("-o : filename (define ./tmp.nv12)\n");
    printf("-n : frame Count (define 90)\n");
    printf("-f : frame Format(FMT_NV12=0/FMT_YUYV=1/FMT_UYVY=2) (define 0)\n");
}

int checkParam(int argc, char *argv[])
{
    int i;
    for( i = 1; i < argc; i++ )
    {
        switch( argv[i][1] )
        {
            case 'H':
                showHelp();
                return -1;
            break;
            case 'f':
            	fmt = atoi(argv[i+1]);
                i++;
            break;
            case 'w':
            	width = atoi(argv[i+1]);
                i++;
            break;
            case 'h':
            	height = atoi(argv[i+1]);
                i++;
            break;
            case 'o':
            	filename = argv[i+1];
            	i++;
            break;
            case 'n':
            	frameCount = atoi(argv[i+1]);
                i++;            
            break;

            default:
                printf("param %s is err \n\n", argv[i]);
                showHelp();
               return -1;
            break;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
	if ( checkParam(argc, argv) )
	{
		return -1;
	}
	
	printf("width=%d\n", width);
	printf("height=%d\n", height);
	printf("输出文件名：filename=%s\n", filename);
	printf("生成的帧数：frameCount=%d\n", frameCount);
	printf("fmt=%d\n", fmt);

	switch( fmt )
	{
		case FMT_NV12:
			frameSize = width*height*1.5;
		break;
		case FMT_UYVY:
			frameSize = width*height*2;
		break;
		case FMT_YUYV:
			frameSize = width*height*2;
		break;
		default:
			printf("format is not support\n");
			return -1;
		break;
	}

	printf("try to malloc memory:size = %d\n", frameSize);
	frameBuff = (char*)malloc(frameSize);
	if( frameBuff == NULL )
	{
		printf("frameBuff == NULL , malloc Err\n");
	}

	printf("try to open file:%s\n", filename);
	fb = open(filename, O_CREAT|O_RDWR, 0777);
	if( fb < 0 )
	{
		perror("open err:");
	}

	printf("try to render data\n");
    frame_render(frameCount);


	close(fb);
	free(frameBuff);
    printf("run end ok\n");
    return 0;  
}  

