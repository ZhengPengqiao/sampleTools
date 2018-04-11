#include <stdio.h>  
#include <string.h>
#include <stdlib.h>

/**
 * @brief 全局变量,记录着从参数中解析的信息 
 */

int  isSetSave = 0; 		/*!< 记录是否设置某个参数 */
char *fileName = (char*)"./default.bin"; /*!< 记录设置的字符串 */
int  value; 				/*!< 记录设置的数值 */

void showHelp()
{
	printf("./programe options \n");	
	printf("USED:");
	printf("	-help: show help info\n");
	printf("	-save: is want to save image (default %s)\n", isSetSave?"True":"False");
	printf("	-fn FileName: Save file Name (default %s)\n", fileName);
	printf("	-val value: set A Value Will Need (default %d)\n", value);
}

int checkParam(int argc,char **argv)
{
	int i = 0;
	for(i = 1;i < argc;i++)
	{
		if( strcmp("-help", argv[i]) == 0 )
		{
			showHelp();
			return -1;
		}
		else if( strcmp("-save", argv[i]) == 0 )
		{
			isSetSave = 1;
		}
		else if( strcmp("-fn", argv[i]) ==0 )
		{
			fileName = argv[i+1];
			i++;
		}
		else if( strcmp("-val", argv[i]) == 0)
		{
			value = atoi(argv[i+1]);
			i++;
		}
		else
		{
			printf("param %s is not support \n\n", argv[i]);
			showHelp();
			return -1;
		}
	}
	return 0;
}

int main(int argc,char **argv)
{
	if( checkParam(argc, argv) )
	{
		return -1;
	}

	printf("isSetSave = %s\n", isSetSave?"True":"False");
	printf("fileName=%s\n", fileName);
	printf("value=%d\n",value);

	return 0;
}