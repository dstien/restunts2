DEBUG ?= 1
MKDIR := mkdir -p

# Instead of filling the screen with compiler arguments, we just print the
# input filenames and show the important details in a dandy info box that is
# only shown once per target.
define \n


endef
define INFO_BOX
$(if $(INFO_BOX_PRINTED),,$(eval INFO_BOX_PRINTED:=1) \
	$(info ╭───────────────────────────────────────────────────────────────────────────────) \
	$(info $(subst $(\n),$(\n)│ ,│$1)) \
	$(info ╰───────────────────────────────────────────────────────────────────────────────) \
)
endef
