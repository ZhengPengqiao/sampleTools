#include <stdlib.h>
#include <stdio.h>
char str[1024];
int main(){

	char *pathvar = getenv("QT_QPA_GENERIC_PLUGINS"); 
	printf("QT_QPA_GENERIC_PLUGINS is : %s\n", pathvar);

	sprintf(str, "echo QT_QPA_GENERIC_PLUGINS is : %s > /dev/kmsg\n", pathvar);
	system(str);
	return 0;
}
