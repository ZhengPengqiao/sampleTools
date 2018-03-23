#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


#define ACC_MAIN 1
#define ACC_LOW  2
#define ACC_SSR  3
#define ACC_LTP  4

unsigned int samratearray[] = {
    8000,
    11025,
    16000,
    22050,
    32000,
    44100,
    47250,
    48000,
    50000,
    96000,
};


unsigned int getES(int accObj,  int samRate, int channelNum)
{
    int samVal = 11;
    if (92017 <= samRate)
    {
        samVal = 0;
    }
    else if (75132 <= samRate)
    {
        samVal = 1;
    }
    else if (55426 <= samRate)
    {
        samVal = 2;
    }
    else if (46009 <= samRate)
    {
        samVal = 3;
    }
    else if (37566 <= samRate)
    {
        samVal = 4;
    }
    else if (27713 <= samRate)
    {
        samVal = 5;
    }
    else if (23004 <= samRate)
    {
        samVal = 6;
    }
    else if (18783 <= samRate)
    {
        samVal = 7;
    }
    else if (13856 <= samRate)
    {
        samVal = 8;
    }
    else if (11502 <= samRate)
    {
        samVal = 9;
    }
    else if (9391 <= samRate)
    {
        samVal = 10;
    }
    else 
    {
        samVal = 11;
    }

    return (accObj<<11) | (samVal<<7) | (channelNum<<3);
}



unsigned int es;
int main()
{
    for ( int accObj = 0; accObj < 0x1F; accObj++ )
    {
        for ( int channelNum = 0; channelNum < 0x0F; channelNum++ )
        {
            for ( int samRateIndex = 0; samRateIndex < (sizeof(samratearray)/sizeof(samratearray[0])); samRateIndex++ )
            {
            
                if( channelNum <= 2 && channelNum >= 1 )
                {
                   
                    switch( accObj )
                    {
                        case ACC_MAIN:
                            printf("accObj=Main ");
                            es = getES(accObj, samratearray[samRateIndex], channelNum);
                            printf("samRate=%d channelNum=%d: es=%04x\n", samratearray[samRateIndex], channelNum, es);
                        break;
                        case ACC_LOW:
                            printf("accObj=LOW ");
                            es = getES(accObj, samratearray[samRateIndex], channelNum);
                            printf("samRate=%d channelNum=%d: es=%04x\n", samratearray[samRateIndex], channelNum, es);
                        break;
                        case ACC_SSR:
                            printf("accObj=SSR ");
                            es = getES(accObj, samratearray[samRateIndex], channelNum);
                            printf("samRate=%d channelNum=%d: es=%04x\n", samratearray[samRateIndex], channelNum, es);
                        break;
                        case ACC_LTP:
                            printf("accObj=LTP ");
                            es = getES(accObj, samratearray[samRateIndex], channelNum);
                            printf("samRate=%d channelNum=%d: es=%04x\n", samratearray[samRateIndex], channelNum, es);
                        break;
                        default: continue; break;
                    }
                        
                }
            }
        }
    }


    return 0;
}