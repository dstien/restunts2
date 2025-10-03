include platform.mk
include ../../common.mk

OUT_DIR := ../../../build/platform/$(PLATFORM)
TEST := $(OUT_DIR)/test-$(PLATFORM)

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

$(TEST): $(RESTUNTS_LIB) $(TEST_LIB) | $(OUT_DIR)
	$(LD) $(LDFLAGS) -o $(TEST) $(TEST_LIB) $(RESTUNTS_LIB)

clean:
	$(RM) $(TEST)
