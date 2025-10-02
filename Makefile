.DEFAULT_GOAL := all
%::
	@$(MAKE) -C src $@

check-fmt:
	@for f in `find src -iname '*.h' -o -iname '*.c'`; do \
	    diff -u --label="$$f" "$$f" --label="$$f (formatted)" <(clang-format --style=file "$$f"); \
	done | `git config --get core.pager || echo $$PAGER`
