include ../common.mk
include ../platform/$(PLATFORM)/platform.mk

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

SRCS := math.c
OUT_DIR := ../../build/c/$(PLATFORM)
OBJS := $(addprefix $(OUT_DIR)/, $(SRCS:.c=.o))
LIB := $(OUT_DIR)/librestunts.a

.PHONY: all clean
all: $(LIB)

$(OUT_DIR):
	@$(MKDIR) $@

$(OUT_DIR)/%.o: %.c $(INCLUDES) | $(OUT_DIR)
	$(call INFO_BOX, $(BUILD_INFO))
	$(info $(CC) $<)
	@$(CC) $(CFLAGS) $(DEFINES) -o $@ $<

$(LIB): $(OBJS) | $(OUT_DIR)
	$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	$(RM) $(LIB) $(OUT_DIR)/*.o
