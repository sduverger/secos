#!/usr/bin/make -f

MAKEFLAGS  := --no-print-directory
CC         := $(shell which gcc)
CPP        := $(shell which cpp)
LD         := $(shell which ld)
RM         := $(shell which rm)
FIND       := $(shell which find)
GIT        := $(shell which git)
RELEASE    := $(shell $(GIT) log -n 1 --no-merges --pretty=format:%h-%t)

# Compilation options
CFLG_FP    := -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-ssse3 -mno-sse4.1 \
              -mno-sse4.2 -mno-sse4 -mno-avx -mno-avx2 -mno-aes -mno-pclmul \
              -mno-fsgsbase -mno-rdrnd -mno-f16c -mno-fma -mno-sse4a \
              -mno-fma4 -mno-xop -mno-lwp -mno-3dnow -mno-popcnt \
              -mno-abm -mno-bmi -mno-bmi2 -mno-lzcnt -mno-tbm

CFLG_32    := -m32 -g -fno-pic
CFLG_WRN   := -Wall -W -Werror
CFLG_KRN   := -pipe -nostdlib -nostdinc -ffreestanding -fms-extensions
CFLG_REL   := -DRELEASE=\"secos-$(RELEASE)\"
CFLAGS     := $(CFLG_WRN) $(CFLG_FP) $(CFLG_KRN) $(CFLG_32) $(CFLG_REL)

# elementary kernel parts
INCLUDE    := -I../kernel/include
CORE       := ../kernel/core/

core_obj   :=	entry.o \
		start.o \
		print.o \
		uart.o	\
		pic.o 	\
		intr.o	\
		idt.o	\
		excp.o	\
		stack.o

objects    := $(addprefix $(CORE), $(core_obj))

# Linking options
LDFLG_32   := -melf_i386
LDFLG_WRN  := --warn-common --no-check-sections -n
LDFLAGS    := $(LDFLG_WRN) $(LDFLG_32)

CCLIB      := $(shell $(CC) -m32 -print-libgcc-file-name)
LDSCRIPT   := ../utils/linker.lds
TARGET     := kernel.elf

# Qemu options
QEMU := $(shell which qemu-system-i386)
#QEMU := $(shell which kvm)
QFDA := -drive media=disk,format=raw,if=floppy,file=../utils/grub.floppy
QHDD := -drive media=disk,format=raw,if=ide,index=0,file=fat:rw:.
QSRL := -serial mon:stdio
QDBG := -d int,pcall,cpu_reset,unimp,guest_errors
QOPT := $(QFDA) $(QHDD) $(QSRL) -boot a -nographic

ifneq ($(findstring "kvm",$(QEMU)),)
QOPT += -cpu host
endif
