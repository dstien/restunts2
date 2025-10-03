PLATFORM  := dos16
DEBUG     ?= 1

CC        := owcc
CFLAGS    := -march=i86 -mcmodel=m -mabi=cdecl -fnostdlib -O2 -c -std=c99 -Wall
LD        := wlink
LDFLAGS   :=
AR        := wlib
ARFLAGS   := -fa -n -q

ifeq ($(DEBUG),1)
	CFLAGS += -g2
	LDFLAGS += debug watcom all
endif
