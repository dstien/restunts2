.PHONY: all asm dos clean
all: asm dos

asm:
	@$(MAKE) -C asm

dos: asm
	$(MAKE) -C dos

clean:
	$(MAKE) -C dos clean
	$(MAKE) -C asm clean
