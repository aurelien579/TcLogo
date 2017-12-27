CC 		= gcc
CFLAGS 	= $(CAIRO_FLAGS) -std=c99 -g -Iinclude -Wall
SOURCES = $(wildcard src/*.c)
SOURCES := $(filter-out src/logo.tab.c src/logo.yy.c, $(SOURCES))
OBJS 	= $(patsubst %.c, %.o, $(wildcard src/*.c)) \
			src/logo.yy.o \
			src/logo.tab.o

lib-cairo: lib/libtclogo-cairo.a

lib: lib/libtclogo.a

lib/libtclogo-cairo.a: $(SOURCES)
	$(MAKE) clean-objects
	$(MAKE) build-lib-cairo CAIRO_FLAGS="-DCAIRO `pkg-config --cflags gtk+-3.0`"

lib/libtclogo.a: $(SOURCES)
	$(MAKE) clean-objects
	$(MAKE) build-lib

build-lib: $(OBJS)
	@ar rcs lib/libtclogo.a $^

build-lib-cairo: $(OBJS)
	@ar rcs lib/libtclogo-cairo.a $^

src/logo.tab.c: src/logo.y
	@echo "Compiling : $<"
	@bison -d $< -o $@
	@cp src/logo.tab.h include/tclogo/logo.tab.h

src/logo.yy.c: src/logo.l src/logo.tab.c
	@echo "Compiling : $<"
	@flex -o $@ $<

src/logo.tab.o: src/logo.tab.c
	@echo "Compiling : $<"
	@$(CC) $(CFLAGS) -w $^ -c -o $@

src/logo.yy.o: src/logo.yy.c
	@echo "Compiling : $<"
	@$(CC) $(CFLAGS) -w $^ -c -o $@

%.o: %.c src/logo.tab.o
	@echo "Compiling : $<"
	$(CC) $(CFLAGS) $< -c -o $@

.PHONY: clean lib lib-cairo build-lib build-lib-cairo clean-objects

clean-objects:
	@rm -f $(OBJS) src/logo.yy.c src/logo.tab.h src/logo.tab.c
	@rm -f include/tclogo/logo.tab.h

clean: clean-objects
	@rm -f lib/*
