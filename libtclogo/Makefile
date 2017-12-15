CC 		= gcc
CFLAGS 	= -std=c99 -g -Iinclude
TARGET 	= lib/libtclogo.a
OBJS 	= $(patsubst %.c, %.o, $(wildcard src/*.c)) \
			src/logo.yy.o \
			src/logo.tab.o

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $^

src/logo.tab.c: src/logo.y
	bison -d $< -o $@
	cp src/logo.tab.h include/tclogo/logo.tab.h

src/logo.yy.c: src/logo.l src/logo.tab.c
	flex -o $@ $<

src/logo.tab.o: src/logo.tab.c
	$(CC) $(CFLAGS) -w $^ -c -o $@

src/logo.yy.o: src/logo.yy.c
	$(CC) $(CFLAGS) -w $^ -c -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET) src/logo.yy.c src/logo.tab.h src/logo.tab.c
	rm -f include/tclogo/logo.tab.h

