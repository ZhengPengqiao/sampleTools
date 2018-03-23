#include <stdio.h>

int NEND = 0;
int MEND = 0;
int KEND = 0;

int NSTART = 0;
int MSTART = 0;
int KSTART = 0;

int inputfreq = 0, outfreq = 0;

int N = 0;
int M = 0;
int K = 0;


int main()
{
	int ret;
	int tmp = 0;
	printf("input N Start, N END: \n");
	ret = scanf("%d%d",&NSTART, &NEND);
	if( ret != 2 )
	{
		perror("input err:");
	}
	printf("N Start = %d, N END = %d \n", NSTART, NEND);

	printf("input M Start, M END: \n");
	ret = scanf("%d%d",&MSTART, &MEND);
	if( ret != 2 )
	{
		perror("input err:");
	}
	printf("M Start = %d, M END = %d \n", MSTART, MEND);

	printf("input K Start, K END: \n");
	ret = scanf("%d%d",&KSTART, &KEND);
	if( ret != 2 )
	{
		perror("input err:");
	}
	printf("K Start = %d, K END = %d \n", KSTART, KEND);

	printf("input freq, out freq: \n");
	ret = scanf("%d%d",&inputfreq, &outfreq);
	if( ret != 2 )
	{
		perror("input err:");
	}
	printf("inputfreq = %d, outfreq = %d \n", inputfreq, outfreq);

	for( N = NSTART; N <= NEND; N++)
	{
		for( M = MSTART; M <= MEND; M++)
		{
			for( K = KSTART; K <= KEND; K++)
			{
				tmp = outfreq - ( (inputfreq * (N+2)) / ((M+2) * (K+2)) );
				if( -2 <= tmp && tmp <= 2 )
				{
					printf("N=%d M=%d K=%d tmp=%d\n", N,M,K, tmp);
				}
			}
		}

	}

	printf("end ---- end\n");
	return 0;
}