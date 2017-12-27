all: tclogo-gui

run: tclogo-gui 
	@cd tclogo-gui && ./tclogo

tclogo: libtclogo
	@cd tclogo && $(MAKE)
	
tclogo-gui: libtclogo-cairo
	@cd tclogo-gui && $(MAKE)

libtclogo:
	@cd libtclogo && $(MAKE) lib

libtclogo-cairo:
	@cd libtclogo && $(MAKE) lib-cairo
	
tests: libtclogo
	@cd tests && $(MAKE)

clean:
	@cd tclogo && $(MAKE) clean
	@cd libtclogo && $(MAKE) clean
	@cd tests && $(MAKE) clean
	@cd tclogo-gui && $(MAKE) clean

.PHONY: clean libtclogo libtclogo-cairo tclogo tclogo-gui tests run
