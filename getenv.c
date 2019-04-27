#include <stdlib.h>
#include <stdio.h>
char str[1024];
int main()
{
	int ret = 0;

	char *pathvar = getenv("QT_QPA_GENERIC_PLUGINS"); 
	printf("QT_QPA_GENERIC_PLUGINS is : %s\n", pathvar);

	sprintf(str, "echo QT_QPA_GENERIC_PLUGINS is : \"%s\" > /dev/kmsg\n", pathvar);
	printf("system str: %s\n", str);
	ret = system(str);
	if( ret < 0 )
	{
		perror("system err:");
	}
	return 0;
}
