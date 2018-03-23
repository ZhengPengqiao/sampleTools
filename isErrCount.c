#include <stdio.h>
#include <stdlib.h>

int main()
{
	unsigned int vaddrIsNullCount = 0;
	int in;
	while(1)
	{
		if( scanf("%d", &in) == 0 )
		{
			printf("input err:\n");
		}

		if( in != 0 && in != 1 )
		{
			printf("please input 0 or 1\n");
			continue;
		}

		vaddrIsNullCount = (vaddrIsNullCount<<1) | in;
		printf("vaddrIsNullCount = 0x%04x \n", vaddrIsNullCount);

		if( (vaddrIsNullCount & 0xFF) == 0xFF )
		{
			printf("IS ERR: vaddrIsNullCount = 0x%04x \n", vaddrIsNullCount);
			vaddrIsNullCount = 0;
		}
	}

	return 0;
}
