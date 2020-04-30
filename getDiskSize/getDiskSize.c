/**
 * @file getDiskSize.c
 * @author ZhengPengqiao (157510312@qq.com)
 * @brief 
 * @version 0.1
 * @date 2020-03-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/vfs.h>

char *folderName = (char*)"/dev/sdb1";

//获取磁盘空间大小（KB）
long TSH_Get_DiskSize(const char* path)
{
	DIR *dir;
    if(!(dir = opendir(path)))
    {
        return 0;
    }
	closedir(dir);
	struct statfs diskInfo;
    statfs(path,&diskInfo);
	unsigned long long totalSize = (diskInfo.f_bsize>>10)*diskInfo.f_blocks;
	long totalSizeK = totalSize;
	printf("DiskSize %ld K\n", totalSizeK);
	return totalSizeK;
}

void showHelp()
{
	printf("./programe options \n");	
	printf("USED:");
	printf("	-help: show help info\n");
	printf("	-f folderName: folderName (default %s)\n", folderName);
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
		else if( strcmp("-f", argv[i]) ==0 )
		{
			folderName = (char*)argv[i+1];
			i++;
		}
		else
		{
			showHelp();
			return -1;
		}
		
	}
	return 0;
}

int main(int argc,char **argv)
{
	if( checkParam(argc, argv) )
	{
		return -1;
	}

	printf("folderName=%s\n", folderName);
	printf("folder Size = %ld M \n", TSH_Get_DiskSize(folderName)/1000 );

	return 0;
}