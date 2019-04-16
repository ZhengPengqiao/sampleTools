#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "adpcm.h"

#define BLKSIZE 1024 
#define MAXCHANNELS 2
short sbuf[BLKSIZE * 2 * MAXCHANNELS];
char cbuf[BLKSIZE * MAXCHANNELS];

char *pcm_filename = (char*)"pcm.wav";
char *adpcm_filename = (char*)"adpcm.wav";
char *out_pcm_filename = (char*)"out_pcm.wav";

#define ENCODE 0
#define DECODE 1
#define ENCODE_DECODE 2
int mode = ENCODE_DECODE;

/**
 * @brief wav头格式
 * 偏移地址  	大小  	字节  	数据块  	类型  	内容
 * 00H~03H		4		4字符		资源交换文件标志（RIFF
 * 04H~07H  	4		长整数		从下个地址开始到文件尾的总字节数
 * 08H~0BH		4		4字符		WAV文件标志（WAVE）
 * 0CH~0FH		4		4字符		波形格式标志（fmt ），最后一位空格。
 * 10H~13H		4		整数		过滤字节（一般为00000010H）
 * 14H~15H		2		整数		格式种类（值为1时，表示数据为线性PCM编码）
 * 16H~17H		2		整数		通道数，单声道为1，双声道为2
 * 18H~1BH		4		长整数		采样频率
 * 1CH~1FH		4		长整数		波形数据传输速率（每秒平均字节数）
 * 20H~21H		2		整数		DATA数据块长度，字节。
 * 22H~23H		2		整数		PCM位宽
 * 24H~27H		4		4字符		“fact”,该部分一下是可选部分，即可能有，可能没有,一般到WAV文件由某些软件转换而成时，包含这部分。
 * 28H~2BH		4		长整数		size,数值为4
 */

/**
 * @brief 定义wav文件存储pcm数据时的头信息
 * 
 */
unsigned char pcmHeader[] = {
	'R', 'I', 'F', 'F',
	0x00, 0x00, 0x00, 0x00,//datalen +  sizeof(pcmHeader) - 8;
	'W', 'A', 'V', 'E',
	'f', 'm', 't', ' ',
	0x10, 0x00, 0x00, 0x00,
	0x01, 0x00,
	0x00, 0x00,//channels
	0x00, 0x00, 0x00, 0x00,//sample rate
	0x00, 0x00, 0x00, 0x00,//nAvgBytesperSec
	0x00, 0x00,//blockalign
	0x10, 0x00,//bitspersample
	'd', 'a', 't', 'a',
	0x00, 0x00, 0x00, 0x00//datalen
};


/**
 * @brief 定义wav文件存储adpcm时的头信息
 * 
 */
unsigned char adpcmHeader[] = {
	'R', 'I', 'F', 'F',
	0x00, 0x00, 0x00, 0x00,//datalen +  sizeof(adpcmHeader) - 8;
	'W', 'A', 'V', 'E',
	'f', 'm', 't', ' ',
	0x14, 0x00, 0x00, 0x00,
	0x11, 0x00,
	0x00, 0x00,//channels
	0x00, 0x00, 0x00, 0x00,//sample rate
	0x00, 0x00, 0x00, 0x00,//nAvgBytesperSec
	0x00, 0x00,//blockalign
	0x04, 0x00,//bitspersample
	0x02, 0x00,
	0x00, 0x00,//samples of a block
	'f', 'a', 'c', 't',
	0x04, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,//totalframes
	'd', 'a', 't', 'a',
	0x00, 0x00, 0x00, 0x00//datalen
};


/**
 * @brief 将pcm的音频数据转换成adpcm
 * 
 * @param pcm_filename : 存储pcm原始音频数据的文件
 * @param adpcm_filename : 用来存储转换后的adpcm数据
 * @return int : 0:ok, other:err
 */
int mainencode(char *pcm_filename, char *adpcm_filename)
{
	int ret = 0;
	int channels = 0;
	int samplerate = 0;
	int nframes = 0;
	int block_align = 0;
	int ch = 0;
	int blocks = 0;
	int i = 0;
	int tail = 0;
	int nAvgBytesperSec = 0;
	int totalframes = 0;
	int datasize = 0;
	char *dst = NULL;
	short *samples = NULL;
	struct adpcm_state state[MAXCHANNELS];
	FILE* fpin = NULL;
	FILE* foutadpcm = NULL;

	fpin = fopen(pcm_filename, "rb");
	if (!fpin)
	{
		printf("open file err:%s\n", pcm_filename);
		ret = -1;
		goto err;
	}

	foutadpcm = fopen(adpcm_filename, "wb");
	if (!foutadpcm)
	{
		printf("open file err:%s\n", adpcm_filename);
		ret = -1;
		goto err;
	}
	memset(&state, 0, MAXCHANNELS * sizeof(struct adpcm_state));

	ret = fread(sbuf, 1, 44, fpin);
	if (ret < 44)
	{
		goto err;
	}
	fwrite(adpcmHeader, 1, sizeof(adpcmHeader), foutadpcm);
	channels = sbuf[11];
	samplerate = ((int)sbuf[12] & 0x00ffff) | ((int)sbuf[13] << 16);
	nframes = (BLKSIZE * channels - 4 * channels) * 8 /
		(4 * channels) + 1;
	block_align = BLKSIZE * channels;
	nAvgBytesperSec = samplerate * block_align / nframes;
	do{
		memset(sbuf, 0, 2 * channels * nframes);
		ret = fread(sbuf, 2 * channels, nframes, fpin);
		if (ret == 0)
		{
			break;
		}

#if BLOCKS
		totalframes += nframes;
		blocks = (nframes - 1) / 8;

#else
		totalframes += ret;
		blocks = (ret - 1) / 8;
		tail = (ret - 1) & 0x07;
#endif		
		dst = cbuf;
		samples = sbuf;
		for (ch = 0; ch < channels; ch++) {
			state[ch].valprev = *samples++;
			*dst++ = state[ch].valprev & 0x00ff;
			*dst++ = (state[ch].valprev >> 8) & 0x00ff;
			*dst++ = state[ch].index & 0x00ff;
			*dst++ = 0; /* unknown */
		}

		for (i = 0; i < blocks; i++) {
			for (ch = 0; ch < channels; ch++) {
				adpcm_coder(samples + ch, (signed char *)dst, 8, &state[ch], channels);
				dst += 4;
			}
			samples += 8 * channels;
		}

		fwrite(cbuf, 4 * channels, blocks + 1, foutadpcm);
		datasize += 4 * channels *(blocks + 1);
		//last block?
		if (tail)
		{
			memset(cbuf, 0, 4 * channels);
			for (ch = 0; ch < channels; ch++) {
				adpcm_coder(samples + ch, (signed char *)dst, tail, &state[ch], channels);
				dst += 4;
			}
			fwrite(cbuf, 4 * channels, 1, foutadpcm);
			datasize += 4 * channels;
		}
	} while (ret == nframes);
	adpcmHeader[0x04] = (datasize + 52) & 0x00ff;
	adpcmHeader[0x05] = ((datasize + 52) >> 8) & 0x00ff;
	adpcmHeader[0x06] = ((datasize + 52) >> 16) & 0x00ff;
	adpcmHeader[0x07] = ((datasize + 52) >> 24) & 0x00ff;

	adpcmHeader[0x16] = channels & 0x00ff;
	adpcmHeader[0x17] = (channels >> 8) & 0x00ff;

	adpcmHeader[0x18] = samplerate & 0x00ff;
	adpcmHeader[0x19] = (samplerate >> 8) & 0x00ff;
	adpcmHeader[0x1A] = (samplerate >> 16) & 0x00ff;
	adpcmHeader[0x1B] = (samplerate >> 24) & 0x00ff;

	adpcmHeader[0x1C] = nAvgBytesperSec & 0x00ff;
	adpcmHeader[0x1D] = (nAvgBytesperSec >> 8) & 0x00ff;
	adpcmHeader[0x1E] = (nAvgBytesperSec >> 16) & 0x00ff;
	adpcmHeader[0x1F] = (nAvgBytesperSec >> 24) & 0x00ff;

	adpcmHeader[0x20] = block_align & 0x00ff;
	adpcmHeader[0x21] = (block_align >> 8) & 0x00ff;

	adpcmHeader[0x26] = nframes & 0x00ff;
	adpcmHeader[0x27] = (nframes >> 8) & 0x00ff;

	adpcmHeader[0x30] = totalframes & 0x00ff;
	adpcmHeader[0x31] = (totalframes >> 8) & 0x00ff;
	adpcmHeader[0x32] = (totalframes >> 16) & 0x00ff;
	adpcmHeader[0x33] = (totalframes >> 24) & 0x00ff;

	adpcmHeader[0x38] = datasize & 0x00ff;
	adpcmHeader[0x39] = (datasize >> 8) & 0x00ff;
	adpcmHeader[0x3A] = (datasize >> 16) & 0x00ff;
	adpcmHeader[0x3B] = (datasize >> 24) & 0x00ff;
	fseek(foutadpcm, 0, 0);
	fwrite(adpcmHeader, 1, sizeof(adpcmHeader), foutadpcm);

err:
	if (fpin)
	{
		fclose(fpin);
		fpin = NULL;
	}
	if (foutadpcm)
	{
		fclose(foutadpcm);
		foutadpcm = NULL;
	}

	return ret;
}


/**
 * @brief 将adpcm数据转换成pcm数据
 * 
 * @param adpcm_filename : 存储adpcm数据的文件
 * @param out_pcm_filename : 用于存储转换后的pcm数据的文件
 * @return int 0:ok, other:err
 */
int maindecode(char *adpcm_filename, char *out_pcm_filename)
{
	int ret = 0;
	int channels = 0;
	int samplerate = 0;
	int nframes = 0;
	int block_align = 0;
	int ch = 0;
	int blocks = 0;
	int i = 0;
	int tail = 0;
	int nAvgBytesperSec = 0;
	int totalframes = 0;
	int datasize = 0;
	char *dst = NULL;
	short *samples = NULL;
	struct adpcm_state state[MAXCHANNELS];
	FILE* fpin = NULL;
	FILE* foutpcm = NULL;
	fpin = fopen(adpcm_filename, "rb");
	if (!fpin)
	{
		printf("open file err:%s\n", adpcm_filename);
		ret = -1;
		goto err;
	}
	foutpcm = fopen(out_pcm_filename, "wb");
	if (!foutpcm)
	{
		printf("open file err:%s\n", out_pcm_filename);
		ret = -1;
		goto err;
	}
	memset(&state, 0, MAXCHANNELS * sizeof(struct adpcm_state));

	ret = fread(sbuf, 1, 60, fpin);
	if (ret < 60)
	{
		goto err;
	}
	fwrite(pcmHeader, 1, sizeof(pcmHeader), foutpcm);
	channels = sbuf[11];
	samplerate = ((int)sbuf[12] & 0x00ffff) | ((int)sbuf[13] << 16);
	block_align = sbuf[0x10];
	nframes = sbuf[0x13];
	totalframes = ((int)sbuf[0x18] & 0x00ffff) | ((int)sbuf[0x19] << 16);
	nAvgBytesperSec = samplerate * channels * 2;
	do{
		ret = fread(cbuf, 1, block_align, fpin);
		if (ret == 0)
		{
			break;
		}
		nframes = (ret - 4 * channels) * 8 /
			(4 * channels) + 1;
		blocks = (nframes - 1) / 8;
		tail = (nframes - 1) & 0x07;
		if (tail > 0)
		{
			blocks++;
		}
		dst = cbuf;
		samples = sbuf;
		for (ch = 0; ch < channels; ch++) {
			*samples = ((int)dst[0] & 0x00ff) | (((int)dst[1] & 0x00ff) << 8);
			state[ch].valprev = *samples++;
			state[ch].index = dst[2];
			dst += 4;
		}
		for (i = 0; i < blocks; i++) {
			for (ch = 0; ch < channels; ch++) {
				adpcm_decoder((signed char *)dst, samples + ch, 8, &state[ch], channels);
				dst += 4;
			}
			samples += 8 * channels;
		}
		if (totalframes < nframes)
		{
			nframes = totalframes;
		}
		else
		{
			totalframes -= nframes;
		}
		fwrite(sbuf, 2 * channels, nframes, foutpcm);
		datasize += 2 * channels * nframes;
	} while (ret == block_align);
	pcmHeader[0x04] = (datasize + 36) & 0x00ff;
	pcmHeader[0x05] = ((datasize + 36) >> 8) & 0x00ff;
	pcmHeader[0x06] = ((datasize + 36) >> 16) & 0x00ff;
	pcmHeader[0x07] = ((datasize + 36) >> 24) & 0x00ff;

	pcmHeader[0x16] = channels & 0x00ff;
	pcmHeader[0x17] = (channels >> 8) & 0x00ff;

	pcmHeader[0x18] = samplerate & 0x00ff;
	pcmHeader[0x19] = (samplerate >> 8) & 0x00ff;
	pcmHeader[0x1A] = (samplerate >> 16) & 0x00ff;
	pcmHeader[0x1B] = (samplerate >> 24) & 0x00ff;

	pcmHeader[0x1C] = nAvgBytesperSec & 0x00ff;
	pcmHeader[0x1D] = (nAvgBytesperSec >> 8) & 0x00ff;
	pcmHeader[0x1E] = (nAvgBytesperSec >> 16) & 0x00ff;
	pcmHeader[0x1F] = (nAvgBytesperSec >> 24) & 0x00ff;

	pcmHeader[0x20] = (2 * channels) & 0x00ff;
	pcmHeader[0x21] = ((2 * channels) >> 8) & 0x00ff;

	pcmHeader[0x28] = datasize & 0x00ff;
	pcmHeader[0x29] = (datasize >> 8) & 0x00ff;
	pcmHeader[0x2A] = (datasize >> 16) & 0x00ff;
	pcmHeader[0x2B] = (datasize >> 24) & 0x00ff;
	fseek(foutpcm, 0, 0);
	fwrite(pcmHeader, 1, sizeof(pcmHeader), foutpcm);

err:
	if (fpin)
	{
		fclose(fpin);
		fpin = NULL;
	}
	if (foutpcm)
	{
		fclose(foutpcm);
		foutpcm = NULL;
	}

	return ret;
}



void showHelp()
{
    printf("USED:\n");
    printf("	-H : HELP\n");
	printf("	-pcmfile filename: 获取pcm数据的文件名\n");
	printf("	-adpcmfile filename: 编码后的adpcm数据的文件名\n");
	printf("	-outpcmfile filename: 解码后生成的pcm数据的文件名\n");
	printf("	-mode val: 0:编码, 1:解码 2:先编码在解码(default:%d)\n", mode);
}

int checkParam(int argc, char *argv[])
{
    int i;
    for( i = 1; i < argc; i++ )
    {
        if( strcmp(argv[i], "-H") == 0 )
		{
            showHelp();
		}
		else if( strcmp(argv[i], "-pcmfile") == 0 )
		{
        	pcm_filename = (char *)argv[i+1];
            i++;
			printf("=> pcm_filename:%s\n", pcm_filename);
		}
		else if( strcmp(argv[i], "-adpcmfile") == 0 )
		{
        	adpcm_filename = (char *)argv[i+1];
            i++;
			printf("=> adpcm_filename:%s\n", adpcm_filename);
		}
		else if( strcmp(argv[i], "-outpcmfile") == 0 )
		{
        	out_pcm_filename = (char *)argv[i+1];
            i++;
			printf("=> out_pcm_filename:%s\n", out_pcm_filename);
		}
		else if( strcmp(argv[i], "-mode") == 0 )
		{
        	mode = atoi(argv[i+1]);
            i++;
		}
		else
		{
			printf("param %s is err \n\n", argv[i]);
			showHelp();
		}
    }
    return 0;
}
	

int main(int argc, char* argv[])
{
	int ret = 0;
	if ( checkParam(argc, argv) )
	{
		return -1;
	}

	if( mode == ENCODE )
	{
		ret = mainencode(pcm_filename, adpcm_filename);
	}
	else if( mode == DECODE )
	{
		ret = maindecode(adpcm_filename, out_pcm_filename);
	}
	else if( mode == ENCODE_DECODE )
	{
		ret = mainencode(pcm_filename, adpcm_filename);
		ret = maindecode(adpcm_filename, out_pcm_filename);
	}
	return ret;
}