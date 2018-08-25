#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <signal.h>       /* for signal */  
#include <execinfo.h>     /* for backtrace() */  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

char *filename = NULL;
int fd;
char str[1024];
int mat[3][5][5];
int filter[6][3][3];
int matIndex = -1;
int matLine = 0;
int filterIndex = -1;
int filterLine = -1;
int outmat[3][3][3];
int bias[2];

#define FLAG_X 0
#define FLAG_W 1
int xwFlag  = FLAG_X;




void showHelp()
{
    printf("USED:\n");
    printf("-H : HELP\n");
    printf("-if filename: in param form file\n");
}

int checkParam(int argc, char *argv[])
{
    int i;
    for( i = 1; i < argc; i++ )
    {
		if( strcmp(argv[i],"-H") == 0 )
		{
            showHelp();
		}
		else if( strcmp(argv[i],"-if") == 0 )
		{
            filename = argv[i+1];
			i++;
			printf("param file = %s\n", filename);
		}
		else
		{
			printf("param %s is err \n\n", argv[i]);
			showHelp();	
		}
		
    }
    return 0;
}

void dealLine(char * line)
{
	int a,b,c,d,e;

	bias[0] = 1;
	bias[1] = 0;

	if( strncmp(line, "x[", 2) == 0 )
	{
		matIndex++;
		matLine = 0;
		xwFlag = FLAG_X;
		printf("mat[%d]\n", matIndex);
	}
	else if( strncmp(line, "w[", 2) == 0 )
	{
		filterIndex++;
		filterLine = 0;
		xwFlag = FLAG_W;
		printf("filter[%d]\n", filterIndex);
	}
	else if( strlen(line) <= 1 )
	{
		printf("%s", line);
	}
	else if( xwFlag == FLAG_X )
	{
		sscanf(str, "%d%d%d%d%d", &a,&b,&c,&d,&e);
		mat[matIndex][matLine][0] = a;
		mat[matIndex][matLine][1] = b;
		mat[matIndex][matLine][2] = c;
		mat[matIndex][matLine][3] = d;
		mat[matIndex][matLine][4] = e;
		printf("mat len=%lu \t:\t %d %d %d %d %d \n", strlen(line), a,b,c,d,e);
		matLine++;
	}
	else if( xwFlag == FLAG_W )
	{
		sscanf(str, "%d%d%d", &a,&b,&c);
		filter[filterIndex][filterLine][0] = a;
		filter[filterIndex][filterLine][1] = b;
		filter[filterIndex][filterLine][2] = c;
		printf("filter len=%lu \t:\t %d %d %d \n", strlen(line), a,b,c);
		filterLine++;
	}
}


int main(int argc, char *argv[])
{
	FILE *fb = 0;
	char *ret = 0;
	int i,j,m,n,k;
	int tmp = 0;
	if ( checkParam(argc, argv) )
	{
		return -1;
	}

	memset(outmat, 0, sizeof(outmat));

	fb = fopen(filename, "rw");
	if( fb <= 0 )
	{
		printf("open %s err\n", filename);
		return -1;
	}

 	while( !feof(fb) )
    {
        ret = fgets(str, 1024, fb);
		if( ret <= 0 )
		{
			break;
		}

		dealLine(str);
    }

	for( k = 0; k < 3; k++ )
	{
		for( i = 0; i < 3; i++ )
		{
			for( j = 0; j < 3; j++ )
			{
				tmp = 0;
				printf("==== [%d][%d] \n", i, j);
				for( m = 0; m < 3; m++ )
				{
					for( n = 0; n < 3; n++ )
					{
						tmp += 	mat[k][i+m][j+n]*filter[k][m][n];
						printf("tmp[%d][%d] %d,%d=%d num=%d\n", 
							m, n, mat[k][i+m][j+n], filter[k][m][n],
							mat[k][i+m][j+n]*filter[k][m][n], tmp);
					}
				}
				outmat[k][i][j] += tmp + bias[0];
				printf("outmat=%d \n", outmat[k][i][j]);
			}
		}
	}

	for( k = 0; k < 3; k++ )
	{
		printf("outmat :\n");
		for( i = 0; i < 3; i++ )
		{
			for( j = 0; j < 3; j++ )
			{
				printf("= %d ", outmat[k][i][j]);
			}
			printf("\n");
		}
	}

	if( 0 != fclose(fb) )
    {
		printf("fclose  %s err\n", filename);
        return -1;
    }
    return 0;  
}  

