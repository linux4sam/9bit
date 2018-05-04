#
# Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
# Joshua Henderson <joshua.henderson@microchip.com>
#

all: senda_example p9bit_example

CFLAGS:=-Wall -g
LDFLAGS:=-static

senda_example_objs = \
	senda_example.o \
	custom_baud.o

p9bit_example_objs = \
	p9bit_example.o \
	custom_baud.o

senda_example: $(senda_example_objs)
	$(CC) $(senda_example_objs) $(CFLAGS) -o $@ $(LDFLAGS)

p9bit_example: $(p9bit_example_objs)
	$(CC) $(p9bit_example_objs) $(CFLAGS) -o $@ $(LDFLAGS)

clean:
	rm -f senda_example $(senda_example_objs) p9bit_example $(p9bit_example_objs)
