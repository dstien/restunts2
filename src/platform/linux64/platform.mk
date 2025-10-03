PLATFORM  := linux64
DEBUG     ?= 1

CC        := clang
CFLAGS    := -march=x86-64 -O2 -c -std=c99 -Wall
LD        := $(CC)
LDFLAGS   :=
AR        := ar
ARFLAGS   := rcs
