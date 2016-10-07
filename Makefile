CC=gcc
CCA=cc
CFLAGS=-Wall -Wextra -std=c99 -O3 -pedantic -lfuse -D_FILE_OFFSET_BITS=64 -Wno-unused-parameter -I/usr/local/include
.DEFAULT_GOAL := all

all: ctlfs.c
	$(CC) $(CFLAGS) ctlfs.c -o ctlfs
clean:
	rm ctlfs
