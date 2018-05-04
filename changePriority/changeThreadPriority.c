#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
void *consumer(void *p)
{
    int i;
    for (i = 0;1; i++)
    {        
       //if(i%200 == 10)
        printf("\r <<<<<<<(%d)", (int)p);
        fflush(stdout);
             
    }
}       
int main(int argc, char *argv[])
{
    pthread_t  t1, t2, t3;
    struct sched_param sched3;
    sched3.sched_priority = 99;

    pthread_create(&t1, NULL, consumer, (void *)4);
    pthread_create(&t2, NULL, consumer, (void *)5);
    pthread_create(&t3, NULL, (consumer), (void *)6);
    printf("t1=%lu , t2=%lu , t3=%lu\n\n", t1,t2,t3);
    printf("t1=%lu , t2=%lu , t3=%lu\n\n", t1,t2,t3);
    sleep(8);
    pthread_setschedparam(t3, SCHED_FIFO, &sched3);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
      return 0;
}   