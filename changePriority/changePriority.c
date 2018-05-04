
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
int pri_val = -1;
int policy = SCHED_RR;


static void  display_sched_attr(int policy, struct sched_param *param)
{
   printf("policy=%s, priority=%d\n",
           (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
           (policy == SCHED_RR)    ? "SCHED_RR" :
           (policy == SCHED_OTHER) ? "SCHED_OTHER" :
           "???",
           param->sched_priority);
}

int show_priority_rang()
{
  int maxpri = -100;
  int minpri = -100;

  maxpri = sched_get_priority_max(policy);
  if(maxpri == -1)
  {
      perror("sched_get_priority_max() failed"); 
      exit(1);
  }


  minpri = sched_get_priority_min(policy);
  if(minpri == -1)
  {
      perror("sched_get_priority_min() failed"); 
      exit(1);
  }

  printf("priority(min~max)=(%d~%d)\n", minpri, maxpri);
  return 0;
}



void showHelp()
{
    printf("USED:\n");
    printf("-H : HELP\n");
    printf("-p val: will change thread id's priority (default %d) \n", pid);
    printf("-v val: will setting priority val (default %d)\n", pri_val);
    printf("-s : show min-max priority val(min~max)\n");
    printf("-t val: %d:SCHED_OTHER, %d:SCHED_FIFO, %d:SCHED_RR, (default %d)\n", SCHED_OTHER, SCHED_FIFO, SCHED_RR, policy);
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
              pri_val = atoi(argv[i+1]);
                  i++;
            break;
            case 's':
              show_priority_rang();
              return -1;
            break;
            case 't':
              switch(atoi(argv[i+1]))
              {
                case SCHED_OTHER: 
                  policy = SCHED_OTHER;
                break;
                case SCHED_FIFO:
                  policy = SCHED_FIFO;
                break;
                case SCHED_RR:
                  policy = SCHED_RR;
                break;
                default: printf("not support type\n");showHelp();break;
              }
              i++;
              break;
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
  int maxpri = -100;
  int minpri = -100;

  struct sched_param param; 
  
  if ( checkParam(argc, argv) )
  {
    return -1;
  }


  if(policy == SCHED_OTHER)
  {
      printf("SCHED_OTHER is not need\n");
      exit(1);
  }


  printf("try to set pid(%d)'s priority is %d \n", pid, pri_val); 
  maxpri = sched_get_priority_max(policy);
  if(maxpri == -1)
  {
      perror("sched_get_priority_max() failed"); 
      exit(1);
  }

  minpri = sched_get_priority_min(policy);
  if(minpri == -1)
  {
      perror("sched_get_priority_max() failed"); 
      showHelp();
      exit(1);
  }

  if( pri_val > maxpri || pri_val < minpri)
  {
  	printf("you setting priority value is err, range(%d~%d)\n", minpri, maxpri);
  	showHelp();
    exit(1);
  }

  param.sched_priority = pri_val; 

  if (sched_setscheduler(pid, policy, &param) == -1) //设置优先级
  { 
      perror("sched_setscheduler() failed"); 
      exit(1); 
  }

  display_sched_attr(policy, &param);

  return 0;
}
