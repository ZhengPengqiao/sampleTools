/*********************************************************************************
 *      Copyright:  (C) 2017 zoulei
 *                  All rights reserved.
 *
 *       Filename:  main.c
 *    Description:  This file
 *
 *        Version:  1.0.0(2017年06月19日)
 *         Author:  zoulei <zoulei121@gmail.com>
 *      ChangeLog:  1, Release initial version on "2017年06月19日 14时55分30秒"
 *
 ********************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "gps.h"
#define GPS_LEN 1024
int set_serial(int fd,int nSpeed, int nBits, char nEvent, int nStop);
int gps_analyse(char *buff,GPRMC *gps_data);
int print_gps(GPRMC *gps_data);
 
int main (int argc, char **argv)
{
    int fd=0;
    int n=0;
    GPRMC gprmc;
    char buff[GPS_LEN];
    char *dev_name="/dev/ttyUSB0";
 
    if((fd=open(dev_name,O_RDWR|O_NOCTTY|O_NDELAY))<0)
    {
            perror("Can't Open the ttyUSB0 Serial Port");
            return -1;
    }
    set_serial( fd,9600,8,'N',1);
 
    while(1)
    {
       sleep(2);
       if((n=read(fd,buff,sizeof(buff)))<0)
        {
           perror("read error");
           return -1;
        }
        printf("buff:%s\n",buff);
        memset(&gprmc, 0 , sizeof(gprmc));
        gps_analyse(buff,&gprmc);
        print_gps(&gprmc);
 
    }
    close(fd);
    return 0;
}