/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 * Joshua Henderson <joshua.henderson@microchip.com>
 */
#include <stropts.h>
#include <asm/termios.h>

/*
 * Going straight for <asm/termios.h> causes conflicts with other necessary
 * headers in most cases, so break this off into another file so we can limit
 * includes to just get struct termios2.
 */

int set_custom_baud(int fd, int baud)
{
	struct termios2 t;
	int ret;

	ret = ioctl(fd, TCGETS2, &t);
	if (ret)
		return ret;

	t.c_cflag &= ~CBAUD;
	t.c_cflag |= BOTHER;
	t.c_ispeed = baud;
	t.c_ospeed = baud;

	return ioctl(fd, TCSETS2, &t);
}
