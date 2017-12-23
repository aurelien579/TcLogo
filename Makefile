all: tclogo-gui

run: tclogo-gui 
	cd tclogo-gui && ./tclogo

tclogo: libtclogo
	cd tclogo && $(MAKE)
	
tclogo-gui: libtclogo
	cd tclogo-gui && $(MAKE)

libtclogo:
	cd libtclogo && $(MAKE)
	
tests: libtclogo
	cd tests && $(MAKE)

clean:
	cd tclogo && $(MAKE) clean
	cd libtclogo && $(MAKE) clean
	cd tests && $(MAKE) clean
	cd tclogo-gui && $(MAKE) clean

.PHONY: clean libtclogo tclogo tests run
