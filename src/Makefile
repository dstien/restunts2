.PHONY: all asm c dos16 linux64 clean
all: dos16

asm:
	@$(MAKE) -C asm TARGET=port
	@$(MAKE) -C asm TARGET=orig

c:
	@$(MAKE) -C c

dos16:
	@$(MAKE) -C dos16

linux64:
	@$(MAKE) -C linux64

clean:
	@$(MAKE) -C asm clean TARGET=port
	@$(MAKE) -C asm clean TARGET=orig
	@$(MAKE) -C c clean PLATFORM=dos16
	@$(MAKE) -C c clean PLATFORM=linux64
	@$(MAKE) -C dos16 clean
	@$(MAKE) -C linux64 clean
	@$(MAKE) -C test clean PLATFORM=dos16
	@$(MAKE) -C test clean PLATFORM=linux64
