#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    char str[100];
    FILE *fp;
    while(1)
    {
        sleep(1);
        sprintf(str, "ps -A | grep \"WizNote\" | wc -l");
        if( (fp = popen(str,"r") ) == NULL )
        {
            printf("Get Device Path Err\n");
            return -1; 
                                               
        }
        //从输出流中读取信息,查看是否有对应的路径
        while ( fscanf(fp,"%s", str) != EOF )
        {
            if( strncmp(str, "1", 1) == 0  )
            {   
                printf("WizNote 正在运行\n");
            }
            else
            {
                printf("WizNote 停止运行\n");
            }
        }
    }
    return 0;
}
