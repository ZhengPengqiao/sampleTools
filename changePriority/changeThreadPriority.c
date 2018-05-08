#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>

#include <sys/syscall.h>  
#define gettid() syscall(__NR_gettid)

void *consumer1(void *p)
{
    int i;
    int tid = gettid();
    printf("<<<<<<<(%d) \n", tid);
    sleep(1);    
    for (i = 0;1; i++)
    {        
       //if(i%200 == 10)
        printf("\r <<<<<<<(%d)", tid);
        fflush(stdout);
             
    }
}       

void *consumer2(void *p)
{
    int i;
    int tid = gettid();
    printf("<<<<<<<(%d) \n", tid);
    sleep(1);    
    for (i = 0;1; i++)
    {        
       //if(i%200 == 10)
        printf("\r <<<<<<<(%d)", tid);
        fflush(stdout);
             
    }
}  

void *consumer3(void *p)
{
    int i;
    int tid = gettid();
    printf("<<<<<<<(%d) \n", tid);
    sleep(1);    
    for (i = 0;1; i++)
    {        
       //if(i%200 == 10)
        printf("\r <<<<<<<(%d)", tid);
        fflush(stdout);
             
    }
}  

int main(int argc, char *argv[])
{
    pthread_t  pt1, pt2, pt3;
    struct sched_param sched3;
    sched3.sched_priority = 99;

    pthread_create(&pt1, NULL, consumer1, (void *)4);
    pthread_create(&pt2, NULL, consumer2, (void *)5);
    pthread_create(&pt3, NULL, consumer3, (void *)6);
    printf("pt1=%lu , pt2=%lu , pt3=%lu\n\n", pt1,pt2,pt3);
    sleep(8);
    pthread_setschedparam(pt3, SCHED_FIFO, &sched3);
    pthread_join(pt1, NULL);
    pthread_join(pt2, NULL);
    pthread_join(pt3, NULL);
      return 0;
}   