include platform.mk
include ../../common.mk

OUT_DIR := ../../../build/platform/$(PLATFORM)
TEST := $(OUT_DIR)/test32.exe
TEST_LNK := test.lnk

RESTUNTS_LIB := ../../../build/c/$(PLATFORM)/librestunts.a
TEST_LIB := ../../../build/test/$(PLATFORM)/libtest.a

.PHONY: all clean
all: $(TEST)

$(OUT_DIR):
	@$(MKDIR) $@

$(RESTUNTS_LIB):
	@$(MAKE) -C ../../c PLATFORM=$(PLATFORM)

$(TEST_LIB):
	@$(MAKE) -C ../../test PLATFORM=$(PLATFORM)

$(TEST): $(RESTUNTS_LIB) $(TEST_LIB) $(TEST_LNK) | $(OUT_DIR)
	$(LD) $(LDFLAGS) @$(TEST_LNK)

clean:
	$(RM) $(TEST)  $(TEST:.exe=.map)
