DEBUG ?= 1

LD := wlink
LDFLAGS :=
MKDIR := mkdir -p

ifeq ($(DEBUG),1)
	LDFLAGS += debug watcom all
endif

OUT_DIR := ../../build/dos
RESTUNTS_ORIG := $(OUT_DIR)/restunto.exe
RESTUNTS_ORIG_LNK := restunts-orig.lnk

ASM_OBJS := $(patsubst ../asm/%.asm,../../build/asm/%.obj,$(wildcard ../asm/*.asm))

.PHONY: all clean
all: $(RESTUNTS_ORIG)

$(OUT_DIR):
	@$(MKDIR) $@

$(RESTUNTS_ORIG): $(ASM_OBJS) $(RESTUNTS_ORIG_LNK) | $(OUT_DIR)
	$(LD) $(LDFLAGS) @$(RESTUNTS_ORIG_LNK)

clean:
	$(RM) $(RESTUNTS_ORIG) $(RESTUNTS_ORIG:.exe=.map)
