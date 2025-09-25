.PHONY: all asm dos16 clean
all: asm dos16

asm:
	@$(MAKE) -C asm

dos16: asm
	$(MAKE) -C dos16

clean:
	$(MAKE) -C dos16 clean
	$(MAKE) -C asm clean
