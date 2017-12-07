CC 	= gcc
CFLAGS 	= -std=c99 -g
LIBS 	= -lm
TARGET 	= tclogo
TEST	= node-test
OBJS 	= $(patsubst %.c, %.o, $(wildcard sources/*.c)) \
		sources/logo.yy.o \
		sources/logo.tab.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LIBS) 

$(TEST): sources/node.o tests/node_test.o
	$(CC) $^ -o $@

sources/logo.tab.c: sources/logo.y
	bison -d $< -o $@

sources/logo.yy.c: sources/logo.l sources/logo.tab.c
	flex -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET) sources/logo.yy.c sources/logo.tab.h sources/logo.tab.c

