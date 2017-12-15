LIBS 	= -L../libtclogo/lib/ -ltclogo -lm
CC		= gcc
CFLAGS  = -I../libtclogo/include/

all: tests

tests: node_test.c
	$(CC) $(CFLAGS) node_test.c -o tests $(LIBS)
