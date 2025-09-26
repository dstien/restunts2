include ../common.mk

PLATFORM ?= dos16
include ../$(PLATFORM)/platform.mk

define BUILD_INFO
PLATFORM $(PLATFORM)
CC       $(CC)
CFLAGS   $(CFLAGS)
LD       $(LD)
LDFLAGS  $(LDFLAGS)
DEBUG    $(DEBUG)
endef

SRCS := math.c
OBJ_DIR := ../../build/c/$(PLATFORM)
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

.PHONY: all clean
all: $(OBJS)

$(OBJ_DIR):
	@$(MKDIR) $@

$(OBJ_DIR)/%.o: %.c $(INCLUDES) | $(OBJ_DIR)
	$(call INFO_BOX, $(BUILD_INFO))
	$(info $(CC) $<)
	@$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(OBJ_DIR)/*.o
