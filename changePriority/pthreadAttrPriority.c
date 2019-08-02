/**
 * @file pthreadAttrPriority.c
 * @author ZhengPengqiao (157510312@qq.com)
 * @brief 配置线程属性, 然后使用这个属性来创建线程
 * @version 0.1
 * @date 2019-08-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include    <stdio.h>  
#include    <stdlib.h>  
#include    <unistd.h>  
#include    <pthread.h>  
#include    <signal.h>  
#include    <string.h>  
#include <sys/syscall.h>

static void display_sched_attr(int policy, struct sched_param *param)
{
    printf("    policy=%s, priority=%d\n",
           (policy == SCHED_FIFO) ? "SCHED_FIFO" : (policy == SCHED_RR) ? "SCHED_RR" : (policy == SCHED_OTHER) ? "SCHED_OTHER" : "???",
           param->sched_priority);
}


void * thread_fun(void *arg)  
{  
    int policy, ret;  
    struct sched_param param;  

    while (1) {  
        printf("thread_fun pid=%d thread_id=%ld\n", getpid(), syscall(SYS_gettid));

        //获取线程调度参数  
        ret = pthread_getschedparam(pthread_self(), &policy, &param);  
        if(ret!=0)  
        {  
            printf("pthread_getschedparam %s\n", strerror(ret) );  
            exit(1);  
        }
        display_sched_attr(policy, &param);

        sleep(1);
    }
    pthread_exit(NULL);  
}  

int main(int argc,char *argv[])  
{  
    int ret;  
    pthread_t tid;  
    pthread_attr_t attr;  
    int policy, inher;  
    struct sched_param param;  
  
    //初始化线程属性  
    pthread_attr_init(&attr);  
    //获取继承的调度策略  
    ret = pthread_attr_getinheritsched(&attr, &inher);  
    if (ret!=0)  
    {  
        printf("pthread_attr_getinheritsched %s\n", strerror(ret));  
        exit(1);  
    }  
  
    if (inher == PTHREAD_EXPLICIT_SCHED)  
    {  
        printf("PTHREAD_EXPLICIT_SCHED\n");  
    }  
    else if (inher == PTHREAD_INHERIT_SCHED)  
    {  
        printf("PTHREAD_INHERIT_SCHED\n");  
        inher = PTHREAD_EXPLICIT_SCHED;  
    }  

    //设置继承的调度策略  
    //必需设置inher的属性为 PTHREAD_EXPLICIT_SCHED，否则设置线程的优先级会被忽略  
    ret = pthread_attr_setinheritsched(&attr, inher);  
    if (ret!=0)  
    {  
        printf("pthread_attr_setinheritsched %s\n", strerror(ret));  
        exit(1);  
    }  

    sleep(1);  
    policy = SCHED_RR;
    //设置线程调度策略  
    ret = pthread_attr_setschedpolicy(&attr, policy);  
    if (ret!=0)  
    {  
        printf(" pthread_attr_setschedpolicy%s\n", strerror(ret));  
        exit(1);  
    }  
    param.sched_priority = 3;  
    //设置调度参数  
    ret = pthread_attr_setschedparam(&attr, &param);  
    if (ret!=0)  
    {  
        printf(" pthread_attr_setschedparam %s\n", strerror(ret));  
        exit(1);  
    }
    
    //创建线程  
    ret = pthread_create(&tid, &attr, thread_fun, NULL);  
    if (ret!=0)  
    {  
        printf("pthread_create %s\n", strerror(ret));  
        exit(1);  
    }  

    while (1) {  
        printf("thread_fun pid=%d thread_id=%ld\n", getpid(), syscall(SYS_gettid));
        //获取线程调度参数  
        ret = pthread_getschedparam(pthread_self(), &policy, &param);  
        if(ret!=0)  
        {  
            printf("pthread_getschedparam %s\n", strerror(ret) );  
            exit(1);  
        }
        display_sched_attr(policy, &param);
        sleep(1);
    }  
    pthread_join(tid, NULL);  
    pthread_exit(NULL);  
}
