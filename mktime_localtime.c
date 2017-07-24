#include <stdio.h>
#include <time.h>

int main ()
{
   time_t ret;
   struct tm info;
   char buffer[80];

   info.tm_year = 2017 - 1900;
   info.tm_mon = 7 - 1;
   info.tm_mday = 4;
   info.tm_hour = 0;
   info.tm_min = 0;
   info.tm_sec = 1;
   info.tm_isdst = -1;

   ret = mktime(&info);
   if( ret == -1 )
   {
       printf("错误：不能使用 mktime 转换时间。\n");
   }
   else
   {
      strftime(buffer, sizeof(buffer), "%c", &info );
      printf("%s   %d\n",buffer,  (int)ret);
   }

   struct tm *tm_info;
   time_t rawtime = ret;
   tm_info = localtime( &rawtime );
   printf("当前的本地时间和日期：%s", asctime(tm_info));


   return(0);
}