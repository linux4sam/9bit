/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 * Joshua Henderson <joshua.henderson@microchip.com>
 */
#include "custom_termios2.h"
#include <asm/ioctls.h>
#include <errno.h>
#include <sys/ioctl.h>

int tcgetattr2(int fd, struct termios2 *termios)
{
	return ioctl(fd, TCGETS2, termios);
}

int tcsetattr2(int fd, int optional_actions, const struct termios2 *termios)
{
	unsigned long int cmd;

	switch (optional_actions)
	{
	case TCSANOW:
		cmd = TCSETS2;
		break;
	case TCSADRAIN:
		cmd = TCSETSW2;
		break;
	case TCSAFLUSH:
		cmd = TCSETSF2;
		break;
	default:
		return -EINVAL;
	}

	return ioctl(fd, cmd, termios);
}
