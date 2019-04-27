/*
 * @Author: ZhengPengqiao 
 * @Date: 2019-04-27 14:25:43 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
/**
 * @brief 帮助函数
 * 
 */
void help(void)
{
    printf("usage: libusb-test [-v vid] [-p pid] [-h] [-s[val]] \n");
    printf("   -h : display usage(不需跟参数)\n");
    printf("   -v : test -v (-v 必须跟参数)\n");
    printf("   -p : test -p (-p 必须跟参数)\n"); 
    printf("   -s : test -s (-s 不必须跟参数)\n"); 

    return;  
 
}

/**
 * @brief 描述
 * 
 * 1、getopt函数解析命令行参数，argc、argv是调用main函数时传入的参数。传入的'-'开始的字符被解析为选项，
 *  	getopt一次执行解析出一个option，如此循环执行，可以将argv中的全部option解析出来；
 * 2、在getopt的执行中，每次进入都会更新optind变量，该变量指向下一个argv参数；
 * 3、如getopt返回-1，表示argv[]中的所有选项被解析出，optind指向第一个非选项的argument元素；这里要注意，
 * 		在getopt执行过程中会将单独的argument交换到argv数组的后面，option选项提前，如：cmd -a file1 -b 
 * 		file2,如果a/b均为不带参数的选项，这最终argv数组变为：cmd -a -b file1 file2;
 * 4、optstring指定选项合法的选项，一个字符代表一个选项，在字符后面加一个':'表示该选项带一个参数，字符后带
 * 		两个':'表示该选项带可选参数(参数可有可无)，若有参数，optarg指向该该参数，否则optarg为0；
 * 5、前面说了getopt会进行argv顺序的调整，但也可以通过设置optstring改变它的方式，这里有两种：
 *      1) 如果optstring的第一个参数是'+'或者POSIXLY_CORRECT被设置，则getopt在原argv的顺序上遇
 * 			到第一个非选项就返回-1；
 *      2) 如果optstring的第一个参数是'-'，则会将所有的非选项当选项处理，并且返回1，用字符代码1表
 * 			示该选项；
 * 6、如果getopt不能识别一个选项字符，它会打印一个错误消息到stderr上，并将该字符存放到optopt中，返
 * 		回'?'；调用程序可以设置opterr=0设置不打印错误信息；注意：要使能打印错误信息，optstring的第一
 * 		个字符（或者在第一个字符是+/-之后）不能是':'，否则也不会打印错误;
 * 7、如果optstring中指定了option需要参数，但在命令行没有参数，那么getopt将返回'?'，如果在optstring
 * 		的第一个字符（或者在第一个字符是+/-之后）是':'，那么将返回':';
 * 
 */

/**
 * @brief 返回值
 * 
 * 1、返回类型为int，这个在编程的时候要注意，因为返回值类型范围要包含-1，很容易返回值接收定义为char，
 * 		但在一些系统中char是无符号的，将导致程序错误；
 * 2、当传入的argv中的选项全部被解析，getopt()返回-1，这也是getopt进行选项解析的循环截至条件；
 * 3、如果argv中解析出optstring中描述的选项字符，则返回该字符，如果该选项指定了参数，则全局变量optarg
 * 		指向该参数；
 * 4、如果getopt遇到一个非optstring指定的选项字符，这表示该选项是未识别的，返回'?',并且将该选项存放到
 * 		全局变量optopt中；
 * 5、如果optstring指定了选项必须带参数，但传入的相应option丢失了参数，返回值依赖于optstring的第一
 * 		个字符，若第一个字符是':'，返回':',否则返回'?'；由于非法的选项返回也是'?'，所以常常optstring
 * 		的第一个字符指定为':'；同时将该选项存放到全局变量 optopt中；
 */
 
int main(int argc, char **argv)
{
    int opt;

    while((opt = getopt(argc, argv, ":hv:p:s::")) != -1)
    {
        switch(opt)
        {
			case 'h' :
                help();
                return 0;
            case 'v' :
				printf ("v: option: %c argv: %s\n", opt, optarg);
                break;
            case 'p' :
				printf ("p: option: %c argv: %s\n", opt, optarg);
                break;
			case 's' :
				if (optarg)
				{
					printf ("s: option: %c argv: %s\n", opt, optarg);
				}
				else
				{
					printf ("s: option: %c\n", opt);
				}
                break;
			case '?': //未知参数
				printf ("encountered a unrecognized option: %c, argv: %s\n", optopt, argv[optind - 1]);
				break;
			case ':':  //getopt第三个参数以":"开头才会进这里
				printf ("option: %c missing argument\n", optopt);
				break;
            default  :
                printf("unkonw option.\n");
                help();
                return 0;
        }
    }

	printf("test param, the programe end\n");
	return 0;
}