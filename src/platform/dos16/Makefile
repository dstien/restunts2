include platform.mk
include ../../common.mk

OUT_DIR := ../../../build/platform/$(PLATFORM)
RESTUNTS_PORT := $(OUT_DIR)/restunts.exe
RESTUNTS_ORIG := $(OUT_DIR)/restunto.exe
RESTUNTS_PORT_LNK := restunts-port.lnk
RESTUNTS_ORIG_LNK := restunts-orig.lnk
ALIASES_PORT_LNK := aliases-port.lnk
ALIASES_ORIG_LNK := aliases-orig.lnk

TEST_PORT := $(OUT_DIR)/test.exe
TEST_ORIG := $(OUT_DIR)/testo.exe
TEST_PORT_LNK := test-port.lnk
TEST_ORIG_LNK := test-orig.lnk

REPLDUMP_PORT := $(OUT_DIR)/repldump.exe
REPLDUMP_ORIG := $(OUT_DIR)/repldumo.exe
REPLDUMP_PORT_LNK := repldump-port.lnk
REPLDUMP_ORIG_LNK := repldump-orig.lnk

ASM_PORT_OBJS := $(patsubst ../../asm/%.asm,../../../build/asm/port/%.o,$(wildcard ../../asm/*.asm))
ASM_ORIG_OBJS := $(patsubst ../../asm/%.asm,../../../build/asm/orig/%.o,$(wildcard ../../asm/*.asm))

RESTUNTS_LIB := ../../../build/c/$(PLATFORM)/librestunts.a
TEST_LIB := ../../../build/test/$(PLATFORM)/libtest.a
REPLDUMP_LIB := ../../../build/tools/repldump/$(PLATFORM)/librepldump.a

.PHONY: all clean
all: restunts-port restunts-orig test-port test-orig repldump-port repldump-orig
restunts-port: $(RESTUNTS_PORT)
restunts-orig: $(RESTUNTS_ORIG)
test-port: $(TEST_PORT)
test-orig: $(TEST_ORIG)
repldump-port: $(REPLDUMP_PORT)
repldump-orig: $(REPLDUMP_ORIG)

$(OUT_DIR):
	@$(MKDIR) $@

$(ASM_PORT_OBJS):
	@$(MAKE) -C ../../asm TARGET=port

$(ASM_ORIG_OBJS):
	@$(MAKE) -C ../../asm TARGET=orig

$(RESTUNTS_LIB):
	@$(MAKE) -C ../../c PLATFORM=$(PLATFORM)

$(TEST_LIB):
	@$(MAKE) -C ../../test PLATFORM=$(PLATFORM)

$(REPLDUMP_LIB):
	@$(MAKE) -C ../../tools/repldump PLATFORM=$(PLATFORM)

$(RESTUNTS_PORT): $(ASM_PORT_OBJS) $(RESTUNTS_LIB) $(RESTUNTS_PORT_LNK) $(ALIASES_PORT_LNK) | $(OUT_DIR)
	$(LD) $(LDFLAGS) @$(RESTUNTS_PORT_LNK) @$(ALIASES_PORT_LNK)

$(RESTUNTS_ORIG): $(ASM_ORIG_OBJS) $(RESTUNTS_ORIG_LNK) | $(OUT_DIR)
	$(LD) $(LDFLAGS) @$(RESTUNTS_ORIG_LNK)

$(TEST_PORT): $(ASM_PORT_OBJS) $(RESTUNTS_LIB) $(TEST_LIB) $(TEST_PORT_LNK) $(ALIASES_PORT_LNK) | $(OUT_DIR)
	$(LD) $(LDFLAGS) @$(TEST_PORT_LNK) @$(ALIASES_PORT_LNK)

$(TEST_ORIG): $(ASM_ORIG_OBJS) $(TEST_LIB) $(TEST_ORIG_LNK) $(ALIASES_ORIG_LNK) | $(OUT_DIR)
	$(LD) $(LDFLAGS) @$(TEST_ORIG_LNK) @$(ALIASES_ORIG_LNK)

$(REPLDUMP_PORT): $(ASM_PORT_OBJS) $(RESTUNTS_LIB) $(REPLDUMP_LIB) $(REPLDUMP_PORT_LNK) $(ALIASES_PORT_LNK) | $(OUT_DIR)
	$(LD) $(LDFLAGS) @$(REPLDUMP_PORT_LNK) @$(ALIASES_PORT_LNK)

$(REPLDUMP_ORIG): $(ASM_ORIG_OBJS) $(REPLDUMP_LIB) $(REPLDUMP_ORIG_LNK) $(ALIASES_ORIG_LNK) | $(OUT_DIR)
	$(LD) $(LDFLAGS) @$(REPLDUMP_ORIG_LNK) @$(ALIASES_ORIG_LNK)

clean:
	$(RM) $(RESTUNTS_PORT) $(RESTUNTS_PORT:.exe=.map)
	$(RM) $(RESTUNTS_ORIG) $(RESTUNTS_ORIG:.exe=.map)
	$(RM) $(TEST_PORT) $(TEST_PORT:.exe=.map)
	$(RM) $(TEST_ORIG) $(TEST_ORIG:.exe=.map)
	$(RM) $(REPLDUMP_PORT) $(REPLDUMP_PORT:.exe=.map)
	$(RM) $(REPLDUMP_ORIG) $(REPLDUMP_ORIG:.exe=.map)
