all: tclogo

tclogo: libtclogo
	cp -r libtclogo/include/* tclogo/include/
	cp -r libtclogo/lib/* tclogo/lib/
	cd tclogo && $(MAKE)

libtclogo:
	cd libtclogo && $(MAKE)
	
tests: libtclogo
	cp -r libtclogo/include/* tests/include/
	cp -r libtclogo/lib/* tests/lib/
	cd tests && $(MAKE)

clean:
	cd tclogo && $(MAKE) clean
	cd libtclogo && $(MAKE) clean

.PHONY: clean libtclogo tclogo tests
