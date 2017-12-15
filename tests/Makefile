LIBS 	= -Llib/ -ltclogo -lm
CC		= gcc
CFLAGS  = -Iinclude/

all: tests

tests: node_test.c
	$(CC) $(CFLAGS) node_test.c -o tests $(LIBS)
