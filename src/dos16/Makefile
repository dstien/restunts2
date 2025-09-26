include ../common.mk
include platform.mk

OUT_DIR := ../../build/$(PLATFORM)
RESTUNTS_PORT := $(OUT_DIR)/restunts.exe
RESTUNTS_ORIG := $(OUT_DIR)/restunto.exe
RESTUNTS_PORT_LNK := restunts-port.lnk
RESTUNTS_ORIG_LNK := restunts-orig.lnk

C_OBJS := $(patsubst ../c/%.c,../../build/c/$(PLATFORM)/%.o,$(wildcard ../c/*.c))
ASM_PORT_OBJS := $(patsubst ../asm/%.asm,../../build/asm/port/%.o,$(wildcard ../asm/*.asm))
ASM_ORIG_OBJS := $(patsubst ../asm/%.asm,../../build/asm/orig/%.o,$(wildcard ../asm/*.asm))

.PHONY: all clean
all: restunts-port restunts-orig
restunts-port: $(RESTUNTS_PORT)
restunts-orig: $(RESTUNTS_ORIG)

$(OUT_DIR):
	@$(MKDIR) $@

$(ASM_PORT_OBJS):
	@$(MAKE) -C ../asm TARGET=port

$(ASM_ORIG_OBJS):
	@$(MAKE) -C ../asm TARGET=orig

$(C_OBJS):
	@$(MAKE) -C ../c PLATFORM=$(PLATFORM)

$(RESTUNTS_PORT): $(ASM_PORT_OBJS) $(C_OBJS) $(RESTUNTS_PORT_LNK) | $(OUT_DIR)
	$(LD) $(LDFLAGS) @$(RESTUNTS_PORT_LNK)

$(RESTUNTS_ORIG): $(ASM_ORIG_OBJS) $(RESTUNTS_ORIG_LNK) | $(OUT_DIR)
	$(LD) $(LDFLAGS) @$(RESTUNTS_ORIG_LNK)

clean:
	$(RM) $(RESTUNTS_PORT) $(RESTUNTS_PORT:.exe=.map)
	$(RM) $(RESTUNTS_ORIG) $(RESTUNTS_ORIG:.exe=.map)
