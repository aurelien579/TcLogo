CC 		= gcc
CFLAGS 	= -std=c99 -g -I../libtclogo/include \
		  `pkg-config --cflags gtk+-3.0`

LIBS	= -L../libtclogo/lib -ltclogo -lm \
		  `pkg-config --libs gtk+-3.0`

TARGET 	= tclogo
OBJS 	= $(patsubst %.c, %.o, $(wildcard src/*.c)) src/resources.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LIBS)

src/resources.c: tclogoapp.gresource.xml ui/*.ui
	glib-compile-resources $< --target=$@ --generate-source

%.o: %.c
	$(CC) $(CFLAGS) $^ -c -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET) src/resources.c