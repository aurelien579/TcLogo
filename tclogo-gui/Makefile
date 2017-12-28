CC 		= gcc
CFLAGS 	= -std=c11 -g -I../libtclogo/include \
		  `pkg-config --cflags gtk+-3.0` -DCAIRO -Wall -D_POSIX_C_SOURCE=199309L -pg

LIBS	= -L../libtclogo/lib -ltclogo-cairo -lm \
		  `pkg-config --libs gtk+-3.0` -lpthread

TARGET 	= tclogo
OBJS 	= $(patsubst %.c, %.o, $(wildcard src/*.c)) src/resources.o

all: $(TARGET)

$(TARGET): $(OBJS) ../libtclogo/lib/libtclogo-cairo.a
	@$(CC) $^ -o $@ $(LIBS)

src/resources.c: tclogoapp.gresource.xml ui/*.ui
	@echo "Compiling : $<"
	@glib-compile-resources $< --target=$@ --generate-source

%.o: %.c
	@echo "Compiling : $<"
	@$(CC) $(CFLAGS) $^ -c -o $@

.PHONY: clean

clean:
	@rm -f $(OBJS) $(TARGET) src/resources.c
