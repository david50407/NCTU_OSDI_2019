# Makefile for the simple kernel.
CC	=gcc
AS	=as
LD	=ld
OBJCOPY = objcopy
OBJDUMP = objdump
NM = nm

CFLAGS = -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS = -m elf_i386

# Add debug symbol
CFLAGS += -g

CFLAGS += -I.

OBJDIR = .

.PHONY: all clean

all: $(OBJDIR)/kernel.img

$(OBJDIR)/kernel.img: boot/boot kernel/system
	dd if=/dev/zero of=$(OBJDIR)/kernel.img count=10000 2>/dev/null
	dd if=$(OBJDIR)/boot/boot of=$(OBJDIR)/kernel.img conv=notrunc 2>/dev/null
	dd if=$(OBJDIR)/kernel/system of=$(OBJDIR)/kernel.img seek=1 conv=notrunc 2>/dev/null

include boot/Makefile
include kernel/Makefile

clean:
	rm $(OBJDIR)/boot/*.o $(OBJDIR)/boot/boot.out $(OBJDIR)/boot/boot $(OBJDIR)/boot/boot.asm || true
	rm $(OBJDIR)/kernel/*.o $(OBJDIR)/kernel/system* kernel.* || true
	rm $(OBJDIR)/lib/*.o || true

run: 
	qemu-system-i386 -hda $(OBJDIR)/kernel.img -nographic -curses

debug:
	qemu-system-i386 -hda $(OBJDIR)/kernel.img -nographic -curses -s -S
