/*
 * 文件名称 : bmp.h
 * 创建时间 : 2017-08-21
 * 文件介绍 : 用于读写BMP文件
 */

#ifndef __BMP_H_
#define __BMP_H_


class BmpInfo
{
public:
	BmpInfo(char *fileName);
	~BmpInfo();
	int MirrorByV();
	char *fileName;
	int imagewidth;
    int imageheight;
    int pixellength;
    char* pixeldata;
};    

#endif    