#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	 
	int pid;
	pid_t cpid; 
	pid_t ppid; 
	int ret;
	
   	if ((pid = fork()) == 0)
   	{
		while(1)
	   	{
	   		sleep(1);
	   		cpid = getpid(); 
	   		printf("cpid ===== %d\n", cpid);
	   	}
	}
   else
   {
	   while(1)
	   {
	   		sleep(2);
	   		ppid = getpid(); 
	   		printf("ppid ===== %d\n", ppid);
	   }
   }
           return 0;
}
