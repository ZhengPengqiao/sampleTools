
/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */
 
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

// spi参数配置
static const char *device = "/dev/spidev2.0";
static uint8_t mode = SPI_CPHA;
static uint8_t bits = 8;
static uint32_t speed = 1000000;
static uint16_t delay;

// 程序运行控制参数
static uint16_t wsize = 20;
static uint16_t test_delay = 10;
static uint32_t times=1;
static int verbose = 0;

#define T_R_BUFF_MAXLEN 4096
uint8_t tx[T_R_BUFF_MAXLEN] = {0};
uint8_t rx[T_R_BUFF_MAXLEN] = {0};

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void print_usage(const char *prog)
{
	printf("Usage: %s [-DsbdlHOLC3]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev1.1)\n"
	     "  -S --speed    max speed (Hz)\n"
	     "  -d --delay    delay (usec)\n"
	     "  -b --bpw      bits per word \n"
	     "  -l --loop     loopback\n"
	     "  -H --cpha     clock phase\n"
	     "  -O --cpol     clock polarity\n"
	     "  -L --lsb      least significant bit first\n"
	     "  -C --cs-high  chip select active high\n"
	     "  -3 --3wire    SI/SO signals shared\n"
	     "  -W --wsize    write size\n"
	     "  -T --test_delay  cycle test delay once\n"
		 "  -v --verbose  Verbose (no show tx buffer)\n"
	     "  -T --times    cycle transfer test times\n");
	exit(1);
}
 
static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "speed",   1, 0, 'S' },
			{ "delay",   1, 0, 'd' },
			{ "bpw",     1, 0, 'b' },
			{ "loop",    0, 0, 'l' },
			{ "cpha",    0, 0, 'H' },
			{ "cpol",    0, 0, 'O' },
			{ "lsb",     0, 0, 'L' },
			{ "cs-high", 0, 0, 'C' },
			{ "3wire",   0, 0, '3' },
			{ "no-cs",   0, 0, 'N' },
			{ "ready",   0, 0, 'R' },
			{ "times",   1, 0, 'T' },
			{ "wsize",   1, 0, 'W' },			
			{ "verbose", 0, 0, 'v' },
			{ "test_delay",   1, 0, 'Y' },
			{ NULL, 0, 0, 0 },
		};
		int c;
 
		c = getopt_long(argc, argv, "D:S:d:b:lHOLC3NRT:W:Y:v", lopts, NULL);
 
		if (c == -1)
			break;
 
		switch (c) {
		case 'D':
			device = optarg;
			break;
		case 'S':
			speed = atoi(optarg);
			break;
		case 'd':
			delay = atoi(optarg);
			break;
		case 'b':
			bits = atoi(optarg);
			break;
		case 'l':
			mode |= SPI_LOOP;
			break;
		case 'H':
			mode |= SPI_CPHA;
			break;
		case 'O':
			mode |= SPI_CPOL;
			break;
		case 'L':
			mode |= SPI_LSB_FIRST;
			break;
		case 'C':
			mode |= SPI_CS_HIGH;
			break;
		case '3':
			mode |= SPI_3WIRE;
			break;
		case 'N':
			mode |= SPI_NO_CS;
			break;
		case 'R':
			mode |= SPI_READY;
			break;
		case 'T':
			times = atoi(optarg);
			break;
		case 'W':
			wsize = atoi(optarg);
			break;
		case 'Y':
			test_delay = atoi(optarg);
			break;
		case 'v':
			verbose = 1;
			break;
		default:
			print_usage(argv[0]);
			break;
		}
	}
}
 
    
static void pabort(const char *s)
{
	perror(s);
	abort();
}


static void hex_dump(const void *src, size_t length, size_t line_size,
		     const char *prefix)
{
	int i = 0;
	const unsigned char *address = (unsigned char*)src;
	const unsigned char *line = address;
	unsigned char c;

	printf("%s | ", prefix);
	while (length-- > 0) {
		printf("%02X ", *address++);
		if (!(++i % line_size) || (length == 0 && i % line_size)) {
			if (length == 0) {
				while (i++ % line_size)
					printf("__ ");
			}
			printf(" | ");  /* right close */
			while (line < address) {
				c = *line++;
				printf("%c", (c < 33 || c == 255) ? 0x2E : c);
			}
			printf("\n");
			if (length > 0)
				printf("%s | ", prefix);
		}
	}
}


int setParam(int spi_fd)
{
    int ret = 0;

	ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);//change spi to 32bits mode
	if (ret == -1)
	{
		printf("can't set spi mode\n");
 	}

	// spi mode
	ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
	{
		pabort("can't set spi mode");
	}

	ret = ioctl(spi_fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
	{
		pabort("can't get spi mode");
	}

	// bits per word
	ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
	{
		pabort("can't set bits per word");
	}

	ret = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
	{
		pabort("can't get bits per word");
	}

	// max speed hz
	ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		pabort("can't set max speed hz");
	}

	ret = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		pabort("can't get max speed hz");
	}

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

	return 0;
}


static void transfer(int fd)
{
	int ret;
	
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = wsize,
		.speed_hz = speed,
		.delay_usecs = delay,
		.bits_per_word = bits,
	};
 
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
	{
		perror("can't send spi message");
	}


	if (verbose)
	{
		hex_dump(tx, wsize, 32, "TX");
	}

	if (verbose)
	{
		hex_dump(rx, wsize, 32, "RX");
	}
}


int main(int argc, char *argv[])
{
	int err = 0;
	int spi_fd = 0;
	int ret = 0;
	int i = 0;

	parse_opts(argc, argv);
 
	spi_fd = open(device,O_RDWR);
	if(spi_fd < 0)
	{	
		printf("open %s  error \n", device);
		return -1;
	}

	if( setParam(spi_fd) < 0 )
	{
		printf("set param %s  error \n", device);
		return -1;
	}

	printf("spi_fd = %x \n", spi_fd);
 
	// 初始化要发送的数据
    for ( i = 0; i < T_R_BUFF_MAXLEN; i++ )
    {
        tx[i] = i % 256;
    }

	printf("times=%d\n", times);
	for( i = 0; i < times; i++ )
	{
		transfer(spi_fd);
		usleep(test_delay*1000);
 	}

	close(spi_fd);
 
	return ret;
}
