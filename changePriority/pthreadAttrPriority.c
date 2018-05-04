/* pthreads_sched_test.c */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static void display_sched_attr(int policy, struct sched_param *param)
{
   printf("    policy=%s, priority=%d\n",
           (policy == SCHED_FIFO)  ? "SCHED_FIFO" :
           (policy == SCHED_RR)    ? "SCHED_RR" :
           (policy == SCHED_OTHER) ? "SCHED_OTHER" :
           "???",
           param->sched_priority);
}


int main(int argc, char *argv[])
{
   int s,policy;
   pthread_attr_t attr;
   struct sched_param param;

   /* Initialize thread attributes object according to options */


    s = pthread_attr_init(&attr);
    if (s != 0)
       perror("pthread_attr_init");

   policy = SCHED_RR;
   param.sched_priority = 10;

   s = pthread_attr_setschedpolicy(&attr, policy);
   if (s != 0)
       perror("pthread_attr_setschedpolicy");
   s = pthread_attr_setschedparam(&attr, &param);
   if (s != 0)
       perror("pthread_attr_setschedparam");

   /* If we initialized a thread attributes object, display
      the scheduling attributes that were set in the object */

   s = pthread_attr_getschedparam(&attr, &param);
   if (s != 0)
        perror("pthread_attr_getschedparam");
   s = pthread_attr_getschedpolicy(&attr, &policy);
   if (s != 0)
       perror("pthread_attr_getschedpolicy");

   display_sched_attr(policy, &param);


   return 0;
}