DEBUG ?= 1

AS := wasm
ASFLAGS := -bt=dos -d$(DEBUG) -zq
MKDIR := mkdir -p

INCLUDES := structs.inc

SRCS := \
	seg000.asm seg001.asm seg002.asm seg003.asm seg004.asm seg005.asm \
	seg006.asm seg007.asm seg008.asm seg009.asm seg010.asm seg011.asm \
	seg012.asm seg013.asm seg014.asm seg015.asm seg016.asm seg017.asm \
	seg018.asm seg019.asm seg020.asm seg021.asm seg022.asm seg023.asm \
	seg024.asm seg025.asm seg026.asm seg027.asm seg028.asm seg029.asm \
	seg030.asm seg031.asm seg032.asm seg033.asm seg034.asm seg035.asm \
	seg036.asm seg037.asm seg038.asm seg039.asm dseg.asm dsegu.asm custom.asm

SEG_SIZES := \
	seg000:4712 seg001:56B3 seg002:014D seg003:428E seg004:39D9 seg005:31EB \
	seg006:1E49 seg007:0903 seg008:2E0C seg009:2996 seg010:1DB5 seg011:0022 \
	seg012:7793 seg013:0089 seg014:01CD seg015:039F seg016:00F9 seg017:0248 \
	seg018:0024 seg019:00A7 seg020:002B seg021:0096 seg022:0242 seg023:0069 \
	seg024:0104 seg025:008D seg026:0018 seg027:156A seg028:1495 seg029:01F9 \
	seg030:014A seg031:06A1 seg032:04A3 seg033:007D seg034:0302 seg035:03E3 \
	seg036:0070 seg037:0096 seg038:0030 seg039:0580 dseg:55D0 dsegu:5750 custom:0000

get_seg_size = $(lastword $(subst :, ,$(filter $1:%,$(SEG_SIZES))))

OBJ_DIR := ../../build/asm
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.asm=.obj))

.PHONY: all clean
all: $(OBJS)

$(OBJ_DIR):
	@$(MKDIR) $@

$(OBJ_DIR)/%.obj: %.asm $(INCLUDES) | $(OBJ_DIR)
	@echo $(AS) $<
	@$(AS) $(ASFLAGS) -fr=$(OBJ_DIR)/$*.err -fl=$(OBJ_DIR)/$*.lst -fo=$@ $<
# Disassemble.
#	@wdis $@ > $(OBJ_DIR)/$*.dis
# Verify compiled size for maintaining original alignments.
	@target_size="$(call get_seg_size,$*)"; [ -n "$$target_size" ] || { echo "Missing target size for $*"; exit 1; }; \
	compiled_size=$$(grep -m1 "^$*" $(OBJ_DIR)/$*.lst | cut -c50-53); \
	[ "$$compiled_size" = "$$target_size" ] || { printf "\033[1;31merror:\033[0m %s size %s does not match target size %s\n" "$*" "$$compiled_size" "$$target_size"; exit 1; }; \

clean:
	$(RM) $(OBJ_DIR)/*.obj $(OBJ_DIR)/*.lst $(OBJ_DIR)/*.err $(OBJ_DIR)/*.dis
