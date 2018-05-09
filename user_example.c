/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 * Joshua Henderson <joshua.henderson@microchip.com>
 *
 * Example to exercise PARMD and SENDA 9-bit UART support on SAMA5D3.
 */
#include "custom_termios2.h"
#include <errno.h>
#include <fcntl.h>
#include <linux/serial.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

/**
 * Sends a message, but indicates the first byte should have the 9th bit set.
 */
static int send_9bit_msg(int fd, struct termios2* term, const char* buf, int size)
{
	int ret;

	/* set mark parity */
	term->c_cflag |= PARENB | CMSPAR | PARODD;

	ret = tcsetattr2(fd, TCSADRAIN, term);
	if (ret < 0) {
		fprintf(stderr, "tcsetattr() failed: %s\n",
			strerror(errno));
		return ret;
	}

	ret = write(fd, buf, 1);
	if (ret < 0) {
		fprintf(stderr, "tcsetattr() failed: %s\n",
			strerror(errno));
		return ret;
	}

	if (size > 1) {
		/* set space parity */
		term->c_cflag |= PARENB | CMSPAR;
		term->c_cflag &= ~PARODD;
		ret = tcsetattr2(fd, TCSADRAIN, term);
		if (ret < 0) {
			fprintf(stderr, "tcsetattr() failed: %s\n",
				strerror(errno));
			return ret;

		}

		/* write the message */
		ret = write(fd, buf+1, size-1);
		if (ret < 0) {
			fprintf(stderr, "write() failed: %s\n",
				strerror(errno));
			return ret;
		}
	}

	return ret;
}

static int quit = 0;
static void signal_handler(int dummy)
{
	quit = 1;
}

#define MAX_MSG_SIZE 255

int main(int argc, char** argv)
{
	char buf[MAX_MSG_SIZE];
	int n = 0;
	int fd;
	struct termios2 old_termios;
	struct termios2 new_termios;
	struct serial_rs485 rs485conf;
	const char* dev;
	int baud;

	signal(SIGINT, signal_handler);

	if (argc != 3) {
		printf("usage: %s DEVICE BAUD", argv[0]);
		return -1;
	}

	dev = argv[1];
	baud = atoi(argv[2]);

	fd = open(dev, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		fprintf(stderr, "could not open %s\n", dev);
		return -1;
	}
	if (tcgetattr2(fd, &old_termios)) {
		fprintf(stderr, "tcgetattr() failed: %s\n",
			strerror(errno));
		return -1;
	}
	memcpy(&new_termios, &old_termios, sizeof(new_termios));

	new_termios.c_cflag = CS8 | CLOCAL;

	new_termios.c_cflag &= ~CBAUD;
	new_termios.c_cflag |= BOTHER;
	new_termios.c_ispeed = baud;
	new_termios.c_ospeed = baud;

	new_termios.c_iflag = 0;
	new_termios.c_oflag = 0;
	new_termios.c_lflag = 0;

	tcflush(fd, TCIOFLUSH);

	if (tcsetattr2(fd, TCSANOW, &new_termios)) {
		fprintf(stderr, "tcsetattr() failed: %s\n",
			strerror(errno));
		return -1;
	}

	// https://github.com/torvalds/linux/blob/master/Documentation/serial/serial-rs485.txt

	/* Enable RS485 mode: */
	rs485conf.flags = SER_RS485_ENABLED;

	/* Set logical level for RTS pin equal to 1 when sending: */
	rs485conf.flags |= SER_RS485_RTS_ON_SEND;
	/* or, set logical level for RTS pin equal to 0 when sending: */
	/*rs485conf.flags &= ~(SER_RS485_RTS_ON_SEND);*/

	/* Set logical level for RTS pin equal to 1 after sending: */
	/*rs485conf.flags |= SER_RS485_RTS_AFTER_SEND;*/
	/* or, set logical level for RTS pin equal to 0 after sending: */
	rs485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);

	/* Set rts delay before send, if needed: */
	rs485conf.delay_rts_before_send = 0;

	/* Set rts delay after send, if needed: */
	rs485conf.delay_rts_after_send = 0;

	/* Set this flag if you want to receive data even whilst sending data */
	/*rs485conf.flags |= SER_RS485_RX_DURING_TX;*/

	if (ioctl(fd, TIOCSRS485, &rs485conf)) {
		fprintf(stderr, "ioctl() failed: %s\n", strerror(errno));
		return -1;
	}

	/* build simple message */
	buf[n++] = 0x1;
	buf[n++] = 0x2;
	buf[n++] = 0x3;
	buf[n++] = 0x4;
	buf[n++] = 0x5;
	buf[n++] = 0x6;

	while (!quit)
	{
		send_9bit_msg(fd, &new_termios, buf, n);

		struct timespec ts = {0,0};
		nanosleep(&ts, NULL);
	}

	/* restore termios to original */
	tcsetattr2(fd, TCSANOW, &old_termios);

	return 0;
}
