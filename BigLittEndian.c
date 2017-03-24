#include <stdio.h>
int main()
{
    unsigned char Byte0,Byte1,Byte2,Byte3;
    unsigned int a = 0x44332211;
    Byte0 = (unsigned char)a;
    Byte1 = (unsigned char)(a>>8);
    Byte2 = (unsigned char)(a>>16);
    Byte3 = (unsigned char)(a>>24);

    if(Byte0 < Byte1)
    {
	printf("Little-endian\n");
    }
    else
    {
        printf("Big-endian\n");
    }
    printf("B0=%2x, B1=%2x, B2=%2x, B3=%2x", Byte0,Byte1,Byte2,Byte3);
    return 0;

}
