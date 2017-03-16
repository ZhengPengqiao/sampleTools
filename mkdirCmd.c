#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
	mkdir("./aa",0775);
	return 0;
}
