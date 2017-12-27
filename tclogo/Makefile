CC 		= gcc
CFLAGS 	= -std=c99 -g -I../libtclogo/include
LIBS	= -L../libtclogo/lib -ltclogo -lm
TARGET 	= tclogo
OBJS 	= $(patsubst %.c, %.o, $(wildcard src/*.c))

all: $(TARGET)

$(TARGET): $(OBJS) ../libtclogo/lib/libtclogo.a
	$(CC) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)
