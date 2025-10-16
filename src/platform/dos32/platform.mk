PLATFORM  := dos32
DEBUG     ?= 1

CC        := owcc
CFLAGS    := -march=i386 -O2 -c -std=c99 -Wall
LD        := wlink
LDFLAGS   :=
AR        := wlib
ARFLAGS   := -fa -n -q

ifeq ($(DEBUG),1)
	CFLAGS += -g2
	LDFLAGS += debug watcom all
endif
