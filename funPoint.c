#include <stdio.h>
#include <string.h>

typedef struct S_MenuFun
{
    int id;
    void (*fun)();
}S_MenuFun;

void fun1()
{
	printf("fun1\n");
}

void fun2()
{
	printf("fun2\n");
}

void fun3()
{
	printf("fun3\n");
}


 //建立ID <-> 函数映射表
S_MenuFun menuFun[] = {
	{0,fun1},  
	{1,fun2},
	{2,fun3},
};
int main()
{
	printf("%ld\n", sizeof(menuFun)/sizeof(menuFun[0]));  //打印出函数的个数
	
	return 0;
}
