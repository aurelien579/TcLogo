CC 		= gcc
CFLAGS 	= -std=c99 -g -Iinclude
LIBS	= -Llib -ltclogo -lm
TARGET 	= tclogo
OBJS 	= $(patsubst %.c, %.o, $(wildcard src/*.c))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)
