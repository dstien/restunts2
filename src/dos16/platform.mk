PLATFORM := dos16

CC := owcc
CFLAGS := -march=i86 -mcmodel=m -mabi=cdecl -fnostdlib -O2 -c -std=c99 -Wall

LD := wlink
LDFLAGS :=

ifeq ($(DEBUG),1)
	CFLAGS += -g2
	LDFLAGS += debug watcom all
endif
