include ../common.mk
include ../platform/$(PLATFORM)/platform.mk

CFLAGS += -I../c

# All the new string constants in the unit tests fills up DGRUOP quickly.
# Watcom can put these in the code segments with -zc, but that requires large
# memory model. Since we still use the original medium-model CRT startup, DS
# is set to DGROUP when our main function is called. When we need to pass near
# pointers to medium-model functions, we copy the data to a buffer we are
# borrowing in DGROUP.
ifeq ($(PLATFORM), dos16)
	ADD_CFLAGS := -mcmodel=l -Wc,-zc
	CFLAGS := $(subst -mcmodel=m,$(ADD_CFLAGS),$(CFLAGS))
endif

define BUILD_INFO
PLATFORM $(PLATFORM)
CC       $(CC)
CFLAGS   $(CFLAGS)
DEFINES  $(DEFINES)
LD       $(LD)
LDFLAGS  $(LDFLAGS)
AR       $(AR)
ARFLAGS  $(ARFLAGS)
DEBUG    $(DEBUG)
endef

SRCS := test.c test_math.c
OUT_DIR := ../../build/test/$(PLATFORM)
OBJS := $(addprefix $(OUT_DIR)/, $(SRCS:.c=.o))
LIB := $(OUT_DIR)/libtest.a

.PHONY: all clean
all: $(LIB)

$(OUT_DIR):
	@$(MKDIR) $@

$(OUT_DIR)/%.o: %.c | $(OUT_DIR)
	$(call INFO_BOX, $(BUILD_INFO))
	$(info $(CC) $<)
	@$(CC) $(CFLAGS) $(DEFINES) -o $@ $<

$(LIB): $(OBJS) | $(OUT_DIR)
	$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	$(RM) $(LIB) $(OUT_DIR)/*.o
