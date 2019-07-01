
/*********************************************************************************
 *      Copyright:  (C) 2017 zoulei
 *                  All rights reserved.
 *
 *       Filename:  gps_analyse.c
 *    Description:  This file
 *
 *        Version:  1.0.0(2017年06月19日)
 *         Author:  zoulei <zoulei121@gmail.com>
 *      ChangeLog:  1, Release initial version on "2017年06月19日 12时16分39秒"
 *
 ********************************************************************************/
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "gps.h"
 
int gps_analyse (char *buff,GPRMC *gps_data)
{
    char *ptr=NULL;
     if(gps_data==NULL)
      {
         return -1;
      }
      if(strlen(buff)<10)
      {
         return -1;
      }
/* 如果buff字符串中包含字符"$GPRMC"则将$GPRMC的地址赋值给ptr */
      if(NULL==(ptr=strstr(buff,"$GPRMC")))
      {
         return -1;
      }
/* sscanf函数为从字符串输入，意思是将ptr内存单元的值作为输入分别输入到后面的结构体成员 */
      sscanf(ptr,"$GPRMC,%d.000,%c,%f,N,%f,E,%f,%f,%d,,,%c*",&(gps_data->time),&(gps_data->pos_state),&(gps_data->latitude),&(gps_data->longitude),&(gps_data->speed),&(gps_data->direction),&(gps_data->date),&(gps_data->mode));
      return 0;
}
 
int print_gps (GPRMC *gps_data)
{
    printf("                                                           \n");
    printf("                                                           \n");
    printf("===========================================================\n");
    printf("==                   全球GPS定位导航模块                 ==\n");
    printf("===========================================================\n");
    printf("                                                           \n");
    printf("===========================================================\n");
    printf("==   GPS state bit : %c  [A:有效状态 V:无效状态]              \n",gps_data->pos_state);
    printf("==   GPS mode  bit : %c  [A:自主定位 D:差分定位]               \n", gps_data->mode);
    printf("==   Date : 20%02d-%02d-%02d                                  \n",gps_data->date%100,(gps_data->date%10000)/100,gps_data->date/10000);
    printf("==   Time : %02d:%02d:%02d                                   \n",(gps_data->time/10000+8)%24,(gps_data->time%10000)/100,gps_data->time%100);
    printf("==   纬度 : 北纬:%d度%d分%d秒                              \n", ((int)gps_data->latitude) / 100, (int)(gps_data->latitude - ((int)gps_data->latitude / 100 * 100)), (int)(((gps_data->latitude - ((int)gps_data->latitude / 100 * 100)) - ((int)gps_data->latitude - ((int)gps_data->latitude / 100 * 100))) * 60.0));
    printf("==   经度 : 东经:%d度%d分%d秒                              \n", ((int)gps_data->longitude) / 100, (int)(gps_data->longitude - ((int)gps_data->longitude / 100 * 100)), (int)(((gps_data->longitude - ((int)gps_data->longitude / 100 * 100)) - ((int)gps_data->longitude - ((int)gps_data->longitude / 100 * 100))) * 60.0));
    printf("==   速度 : %.3f  m/s                                      \n",gps_data->speed);
    printf("==                                                       \n");
    printf("============================================================\n");
 
    return 0;
}