#include <stdio.h>
#include <unistd.h>

show_progress(unsigned long total, unsigned long cur)
{
    static unsigned long p = 0;
    static const unsigned char w[] = "/-";
    unsigned long val;

    val = (cur * 100) / total;
    if(p++ > 1)  p = 0;
    printf("\r%4d%c%c", val, '%', w[p]);
}
 
int main(void)
{
    int i;
       int j;
       int k;
       for (i=0; i <= 500; i++)
       {
          usleep(200*1000);
          show_progress(100, i/5);
          fflush(stdout);
       }
       printf("/n");
       return 0;
}