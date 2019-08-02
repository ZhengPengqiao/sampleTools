#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>  
#include <signal.h>  
#include <string.h>  
#include <sys/types.h>

#include <sys/syscall.h>  
#define gettid() syscall(__NR_gettid)


int pid = -1;
int pri_val = -1;
int policy = SCHED_RR;
int is_test = 0;

static void display_sched_attr(int policy, struct sched_param *param)
{
    printf("    policy=%s, priority=%d\n",
           (policy == SCHED_FIFO) ? "SCHED_FIFO" : (policy == SCHED_RR) ? "SCHED_RR" : (policy == SCHED_OTHER) ? "SCHED_OTHER" : "???",
           param->sched_priority);
}

void *consumer1(void *p)
{

    int policy;  
    struct sched_param param;  
    int i;
    int ret;
    int tid = gettid();
    (void)p;
    printf("<<<<<<<(%d) \n", tid);
    sleep(1);    
    for (i = 0;1; i++)
    {        
       //if(i%200 == 10)
        printf("\r <<<<<<<(%d)", tid);
        //获取线程调度参数  
        ret = pthread_getschedparam(pthread_self(), &policy, &param);  
        if(ret!=0)  
        {  
            printf("pthread_getschedparam %s\n", strerror(ret) );  
            exit(1);  
        }
        display_sched_attr(policy, &param);
        fflush(stdout);
    }
}       

void *consumer2(void *p)
{
    struct sched_param param;  
    int i;
    int ret;
    int policy;  
    int tid = gettid();
    (void)p;
    printf("<<<<<<<(%d) \n", tid);
    sleep(1);    
    for (i = 0;1; i++)
    {        
       //if(i%200 == 10)
        printf("\r <<<<<<<(%d)", tid);
        //获取线程调度参数  
        ret = pthread_getschedparam(pthread_self(), &policy, &param);  
        if(ret!=0)  
        {  
            printf("pthread_getschedparam %s\n", strerror(ret) );  
            exit(1);  
        }
        display_sched_attr(policy, &param);
        fflush(stdout);
    }
}  

void *consumer3(void *p)
{
    struct sched_param param;  
    int i;
    int ret;
    int policy;
    int tid = gettid();
    (void)p;
    printf("<<<<<<<(%d) \n", tid);
    sleep(1);    
    for (i = 0;1; i++)
    {        
       //if(i%200 == 10)
        printf("\r <<<<<<<(%d)", tid);
        //获取线程调度参数  
        ret = pthread_getschedparam(pthread_self(), &policy, &param);  
        if(ret!=0)  
        {  
            printf("pthread_getschedparam %s\n", strerror(ret) );  
            exit(1);  
        }
        display_sched_attr(policy, &param);
        fflush(stdout);
    }
}  



void showHelp()
{
    printf("USED:\n");
    printf("-H : HELP\n");
    printf("-p val: will change thread id's priority (default %d) \n", pid);
    printf("-v val: will setting priority val (default %d)\n", pri_val);
    printf("-t val: %d:SCHED_OTHER, %d:SCHED_FIFO, %d:SCHED_RR, (default %d)\n", SCHED_OTHER, SCHED_FIFO, SCHED_RR, policy);
    printf("-u : create three thread, to test this(default False)\n");
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
            case 'u':
              is_test = 1;
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

int main(int argc, char *argv[])
{
    pthread_t  pt1, pt2, pt3;
    struct sched_param param; 
    param.sched_priority = 99;
  
    if ( checkParam(argc, argv) )
    {
        return -1;
    }

    if( is_test )
    {
        pthread_create(&pt1, NULL, consumer1, (void *)4);
        pthread_create(&pt2, NULL, consumer2, (void *)5);
        pthread_create(&pt3, NULL, consumer3, (void *)6);
        printf("pt1=%lu , pt2=%lu , pt3=%lu\n\n", pt1,pt2,pt3);
        sleep(8);
        pthread_setschedparam(pt3, SCHED_RR, &param);
        pthread_join(pt1, NULL);
        pthread_join(pt2, NULL);
        pthread_join(pt3, NULL);
    }
    else
    {
        param.sched_priority = pri_val;
        if (pthread_setschedparam(pid, policy, &param)) //设置优先级
        { 
            perror("pthread_setschedparam() failed"); 
            exit(1); 
        }
        display_sched_attr(policy, &param);

    }
    
      return 0;
}   