#include <stdio.h>
int main()
{	
	FILE *pp; 
	char line[256];

	pp = popen("/home/zpq/workspaces/sampleTools/ifTest", "r");

	while (fgets(line, sizeof(line), pp)) 
	{ 
		printf("读取的数据为:%s", line);
	} 
	pclose(pp);
    return 0;
}
