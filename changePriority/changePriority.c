#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>

int pid = -1;

void showHelp()
{
    printf("USED:\n");
    printf("-H : HELP\n");
    printf("-p : will change pid's priority\n");
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
    int maxpri;
	struct sched_param param; 
	
	if ( checkParam(argc, argv) )
	{
		return -1;
	}
	
	printf("try to set pro as maxpri\n"); 
	maxpri = sched_get_priority_max(SCHED_FIFO);
	if(maxpri == -1)
	{
	    perror("sched_get_priority_max() failed"); 
	    exit(1);
	}
	else
	{
		printf("maxpri = %d\n", maxpri);
	}
	param.sched_priority = maxpri; 
	if (sched_setscheduler(pid, SCHED_FIFO, &param) == -1) //设置优先级
	{ 
	    perror("sched_setscheduler() failed"); 
	    exit(1); 
	}
    return 0;
}
