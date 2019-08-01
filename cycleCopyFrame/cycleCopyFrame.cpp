#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <utils/Log.h>
#define FRANESIZE 1440*1152*4

unsigned char frame1[FRANESIZE];
unsigned char frame2[FRANESIZE];

int main()
{
	static unsigned char data = 0;
	while(1)
	{
		for (int i = 0; i < FRANESIZE; ++i)
		{
			frame1[i] = data;
		}

		memcpy(frame2,frame1,FRANESIZE);

		if(data == 255)
		{
   	 		ALOGD("cycleCopyFrame: have 255 Times Ok");
		}

		data++;

		usleep(33000);
	}
}