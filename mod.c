#include <stdio.h>
int main()
{
    int n = 100;
    int p = -100;
    int mod = 27;
    printf("正数取模 %d  %d = %d\n", n, mod, n%mod);
    printf("负数取模 %d  %d = %d\n", p, mod, p%mod);
    return 0;
}
