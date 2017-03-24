#include <stdio.h>

int main()
{
	int a = 0,b = 0, c = 0;
	a = 0;
	b = 4;
	printf("判断之前:a=%d, b=%d\n", a, b);

	if ( (a = b) == 4 )
	{
		printf("判断之后:a=%d, b=%d\n", a, b);
		//if条件已经计算一次, 这里将数值还原
		a = 0;
		b = 4;
		printf("为真: (a=b) = %d\n", (a = b));
	} 
	else
	{
		printf("判断之后:a=%d, b=%d\n", a, b);
		//if条件已经计算一次, 这里将数值还原
		a = 0;
		b = 4;
		printf("为假: ( c = (a = b) + 1 ) = %d\n", (a = b) );
	}

	b = 3;
	a = 0;
	c = 0;
	printf("判断之前:a=%d, b=%d, c=%d\n", a, b, c);
	if ( ( c = (a = b) + 1 ) == 4 )
	{
		printf("判断之后:a=%d, b=%d, c=%d\n", a, b, c);
		//if条件已经计算一次, 这里将数值还原
		b = 3;
		a = 0;
		c = 0;
		printf("为真:  ( c = (a = b) + 1 ) = %d\n" , ( c = (a = b) + 1 ));
	} 
	else
	{
		printf("判断之后:a=%d, b=%d, c=%d\n", a, b, c);
		//if条件已经计算一次, 这里将数值还原
		b = 3;
		a = 0;
		c = 0;
		printf("为假: ( c = (a = b) + 1 ) = %d\n", ( c = (a = b) + 1 ) );
	}

	return 0;
}
