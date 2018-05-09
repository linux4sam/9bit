/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 * Joshua Henderson <joshua.henderson@microchip.com>
 */
#ifndef CUSTOM_TERMIOS2_H
#define CUSTOM_TERMIOS2_H

/*
 * struct termios2 provides 2 new members that allow us to set any baud rate we
 * want (as long as the hardware supports it) outside of the predefined ones.
 *
 * If you just use struct termios2 to set the baud to something custom, and then
 * turn around and use the standard struct termios and tcsetattr(), you'll lose
 * the baud setting.  So, it makes sense to just use struct termios2 everywhere.
 *
 * Going straight for <asm/termios.h> causes conflicts with other necessary
 * headers (like <termios.h>), so we are stuck trying to get to struct termios2
 * with that header.
 *
 * The result is this header provides:
 *  1. Bring struct termios2 definition here.
 *  2. Define some helper functions tcgetattr2() and tcsetattr2() which are
 *     equal to tcgetattr() and tcsetattr(), but for struct termios2.
 */

#include <termios.h>

#if !defined(__arm__)
#warning "Only tested on ARM arch.  This may or may not work."
#endif

/*
 * Definition from linux/include/uapi/asm-generic/termbits.h
 */

#define _NCCS 19
struct termios2 {
	tcflag_t c_iflag;               /* input mode flags */
	tcflag_t c_oflag;               /* output mode flags */
	tcflag_t c_cflag;               /* control mode flags */
	tcflag_t c_lflag;               /* local mode flags */
	cc_t c_line;                    /* line discipline */
	cc_t c_cc[_NCCS];               /* control characters */
	speed_t c_ispeed;               /* input speed */
	speed_t c_ospeed;               /* output speed */
};

#ifndef BOTHER
#define BOTHER CBAUDEX
#endif

int tcgetattr2(int fd, struct termios2 *termios);

int tcsetattr2(int fd, int optional_actions, const struct termios2 *termios);

#endif
