.PHONY: all asm c dos16 linux64 clean check-format
all: dos16

asm:
	@$(MAKE) -C asm TARGET=port
	@$(MAKE) -C asm TARGET=orig

c:
	@$(MAKE) -C c

dos16:
	@$(MAKE) -C platform/dos16

linux64:
	@$(MAKE) -C platform/linux64

clean:
	@$(MAKE) -C asm clean TARGET=port
	@$(MAKE) -C asm clean TARGET=orig
	@$(MAKE) -C c clean PLATFORM=dos16
	@$(MAKE) -C c clean PLATFORM=linux64
	@$(MAKE) -C platform/dos16 clean
	@$(MAKE) -C platform/linux64 clean
	@$(MAKE) -C test clean PLATFORM=dos16
	@$(MAKE) -C test clean PLATFORM=linux64

check-format:
	@for f in `find . -iname '*.h' -o -iname '*.c'`; do \
	    diff -u --label="$$f" "$$f" --label="$$f (formatted)" <(clang-format --style=file "$$f"); \
	done | `git config --get core.pager || echo $$PAGER`
