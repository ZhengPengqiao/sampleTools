#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//PRIO_PROCESS, PRIO_PGRP, or PRIO_USER
int which = PRIO_PROCESS;
int pid = -1;
int nice_val = 0;
void showHelp()
{
    printf("USED:\n");
    printf("-H : HELP\n"); 
    printf("-p val: will change id's priority (default %d) \n", pid);
    printf("-v val: will setting Nice val (default %d)\n", nice_val);
    printf("-t val: %d:PRIO_PROCESS, %d:PRIO_PGRP, %d:PRIO_USER, (default %d)\n", PRIO_PROCESS, PRIO_PGRP, PRIO_USER, which);
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
            case 'p':
                pid = atoi(argv[i+1]);
                i++;
            break;
            case 'v':
                nice_val = atoi(argv[i+1]);
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

int main(int argc, char **argv)
{
    int ret = 0;
    
    if ( checkParam(argc, argv) )
    {
        return -1;
    }
    //返回值:若成功,返回nice值;若失败,返回-1  
    ret = getpriority( which, pid);
    printf("old pid(%d), Nice is %d \n", pid, ret);

    //返回值:0:OK -1:Err  pid可以进程号,也可以是线程号
    ret = setpriority( PRIO_PROCESS, pid,  nice_val);
    if( ret == -1 )
    {
        perror("setpriority Err:");
    }

        
    //返回值:若成功,返回nice值;若失败,返回-1
    ret = getpriority( which, pid); 
    printf("new pid(%d), Nice is %d \n", pid, ret);
    
    return 0;
}

