#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int clk = 0;
int P_Rpt = 0;
int X_Res = 0;
int Y_Res = 0;
int H_Total = 0;
int H_Back_Porch = 0;
int H_Front_Porch = 0;
int H_Sync = 0;
int V_Total = 0;
int V_Back_Porch = 0;
int V_Front_Porch = 0;
int V_Sync = 0;
int H_Sync_Polarity = 0;
int V_Sync_Polarity = 0;
int type = 0;
unsigned char str[256];

char *file = (char*)"/home/tsh/log/tshproxy.txt";
char buf[2560];
char *p;
char cmd[1024];
int catchData()
{
    FILE *fp = fopen(file, "r");

    if( fp <= 0 )
    {
        return -1;
    }
    while ( fgets(buf, 2560, fp) != NULL)
    {
        p = strstr(buf, "recv data: Hex[2E FD");
        if( p != NULL )
        {
            p += 15;
            printf("%s\n", p);
            for( int i = 0; i < 30; i++ )
            {
                sscanf(p+i*3, "%x", &str[i]);
            }

            for( int i = 0; i < 30; i++ )
            {
                printf("%02x ", str[i]);
            }
            printf("\n");
        }
    }
    fclose(fp);

    return 0;
}

// BYTE[0]      : 2E
// BYTE[1]      : Data Type	0xFD
// BYTE[2]      : Length	0x1A
// BYTE[3~6]    : Data0~3	P_Clk
// BYTE[7]      : Data4	P_Rpt
// BYTE[8~9]    : Data5~6	X_Res
// BYTE[10~11]  : Data7~8	Y_Res
// BYTE[12~13]       : Data9~10	H_Total
// BYTE[14~15]       : Data11~12	H_Back_Porch
// BYTE[16~17]       : Data13~14	H_Front_Porch
// BYTE[18~19]       : Data15~16	H_Sync
// BYTE[20~21]       : Data17~18	V_Total
// BYTE[22~23]       : Data19~20	V_Back_Porch
// BYTE[24~25]       : Data21~22	V_Front_Porch
// BYTE[26]       : Data23	V_Sync
// BYTE[27]       : Data24	H_Sync_Polarity
// BYTE[28]       : Data25	V_Sync_Polarity
int analyseData(unsigned char* str)
{
    
    clk = (str[3]<<24) + (str[4]<<16) + (str[5]<<8) + str[6];
    P_Rpt = str[7];
    X_Res = (str[8]<<8) + str[9];
    Y_Res = (str[10]<<8) + str[11];
    H_Total = (str[12]<<8) + str[13];
    H_Back_Porch = (str[14]<<8) + str[15];
    H_Front_Porch = (str[16]<<8) + str[17];
    H_Sync = (str[18]<<8) + str[19];
    V_Total = (str[20]<<8) + str[21];
    V_Back_Porch = (str[22]<<8) + str[23];
    V_Front_Porch = (str[24]<<8) + str[25];
    V_Sync = str[26];
    H_Sync_Polarity = str[27];
    V_Sync_Polarity = str[28];
}

int showData()
{
    printf("clk= %d\n", clk);
    printf("P_Rpt= %d\n", P_Rpt);
    printf("X_Res=%d\n", X_Res);
    printf("Y_Res=%d\n", Y_Res);
    printf("H_Total=%d\n", H_Total);
    printf("H_Back_Porch=%d\n", H_Back_Porch);
    printf("H_Front_Porch=%d\n", H_Front_Porch);
    printf("H_Sync=%d\n", H_Sync);
    printf("V_Total=%d\n", V_Total);
    printf("V_Back_Porch=%d\n", V_Back_Porch);
    printf("V_Front_Porch=%d\n", V_Front_Porch);
    printf("V_Sync=%d\n", V_Sync);
    printf("H_Sync_Polarity=%d\n", H_Sync_Polarity);
    printf("V_Sync_Polarity=%d\n", V_Sync_Polarity);
    
    return 0;
}

int setDataType()
{
    if( X_Res == 720 && Y_Res == 480 )
    {
        type = 405;
        printf("    TYPE=405 #define TS_DISP_TV_MOD_480P_YXCX            4\n");
    }
    else if( X_Res == 1920 && Y_Res == 1080 )
    {
        type = 406;
        printf("    TYPE=406 #define TS_DISP_TV_MOD_1080P_30HZ_YXCX      5\n");
    }
    else if( X_Res == 1024 && Y_Res == 480 )
    {
        type = 407;
        printf("    TYPE=407 #define TS_DISP_TV_MOD_1024_480P_60HZ_YXCX  6\n");
    }
    else if( X_Res == 1280 && Y_Res == 480 )
    {
        type = 408;
        printf("    TYPE=408 #define TS_DISP_TV_MOD_1280_480P_60HZ_YXCX  7\n");
    }
    else if( X_Res == 960 && Y_Res == 540 )
    {
        type = 409;
        printf("    TYPE=409 #define TS_DISP_TV_MOD_960_540P_60HZ_YXCX   8\n");
    }
    else if( X_Res == 1280 && Y_Res == 540 )
    {
        type = 410;
        printf("    TYPE=410 #define TS_DISP_TV_MOD_1280_540P_60HZ_YXCX  9\n");
    }
    else if( X_Res == 1440 && Y_Res == 540 )
    {
        type = 411;
        printf("    TYPE=411 #define TS_DISP_TV_MOD_1440_540P_60HZ_YXCX  10\n");
    }
    else if( X_Res == 1540 && Y_Res == 720 )
    {
        type = 412;
        printf("    TYPE=412 #define TS_DISP_TV_MOD_1540_720P_60HZ_YXCX  11\n");
    }
    else if( X_Res == 1920 && Y_Res == 720 && clk < 80000000 )
    {
        type = 413;
        printf("    TYPE=413 #define TS_DISP_TV_MOD_1920_720P_30HZ_YXCX  12\n");
    }
    else if( X_Res == 768 && Y_Res == 1024 )
    {
        type = 414;
        printf("    TYPE=414 #define TS_DISP_TV_MOD_768_1024P_60HZ_YXCX  13\n");
    }
    else if( X_Res == 1280 && Y_Res == 720 )
    {
        type = 415;
        printf("    TYPE=415 #define TS_DISP_TV_MOD_720P_60HZ_YXCX       14\n");
    }
    else if( X_Res == 1920 && Y_Res == 720 && clk < 80000000 )
    {
        type = 413;
        printf("    TYPE=413 #define TS_DISP_TV_MOD_1920_720P_30HZ_YXCX  12\n");
    }
    else if( X_Res == 1920 && Y_Res == 720  && clk > 80000000 )
    {
        type = 416;
        printf("    TYPE=416 #define TS_DISP_TV_MOD_1920_720P_60HZ_YXCX  15\n");
    }
    else if( X_Res == 800 && Y_Res == 480 )
    {
        type = 417;
        printf("    TYPE=417 #define TS_DISP_TV_MOD_800_480P_60HZ_YXCX   16\n");
    }


    sprintf(cmd, "sed -i 's/Output=[0-9]*/Output=%d/' home/tsh/1_tianshuang/cfg_logic.txt", type);
    system(cmd);
}


int main(int argc, char** argv)
{
    int ret = 0;
    if( argc >= 2 )
    {
        file = argv[1];
        printf("file=%s\n", file);
    }
    
    ret = catchData();

    if( ret < 0 )
    {
        printf("No Log To Analyse\n");
        return 0;
    }

    analyseData(str);
    
    showData();

    setDataType();

    return 0;
}


















































