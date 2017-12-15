all: tclogo

tclogo: libtclogo
	cd tclogo && $(MAKE)

libtclogo:
	cd libtclogo && $(MAKE)
	
tests: libtclogo
	cd tests && $(MAKE)

clean:
	cd tclogo && $(MAKE) clean
	cd libtclogo && $(MAKE) clean

.PHONY: clean libtclogo tclogo tests
