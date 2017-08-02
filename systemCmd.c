// 阶段1：创建子进程等准备工作。如果失败，返回-1。
// 阶段2：调用/bin/sh拉起shell脚本，如果拉起失败或者shell未正常执行结束（参见备注1），原因值
//   被写入到status的低8~15比特位中。system的man中只说明了会写了127这个值，但实测发现还会写
//   126等值。
// 阶段3：如果shell脚本正常执行结束，将shell返回值填到status的低8~15比特位中。

#include <stdio.h>  
#include <stdlib.h>  
#include <sys/wait.h>  
#include <sys/types.h>  

// 0      success

// 1      incorrect invocation or permissions

// 2      system error (out of memory, cannot fork, no more loop devices)

// 4      internal mount bug

// 8      user interrupt

// 16     problems writing or locking /etc/mtab

// 32     mount failure

// 64     some mount succeeded


int main()  
{  
    pid_t status;  

  
    status = system("mount /dev/mmcblk1p1 /run/media/mmcblk1p1");  
  
    printf("status=[0x%x] status[8~15]=%d status[0~7]=%d\n", status, (status>>8)&0xFF, status&0xFF);
    if (-1 == status)  
    {  
        printf("system error!");  
    }  
    else  
    {  
        printf("exit status value = [0x%x]\n", status);  
  
        if (WIFEXITED(status))  
        {  
            if (0 == WEXITSTATUS(status))  
            {  
                printf("run shell script successfully.\n");  
            }  
            else  
            {  
                printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));  
            }  
        }  
        else  
        {  
            printf("exit status = [%d]\n", WEXITSTATUS(status));  
        }  
    }  
  
    return 0;  
}  