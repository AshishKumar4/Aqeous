SOURCE:=Source
LIBRARY:=Library
CONSOLE:=$(SOURCE)/console
CONSOLEVGA:=$(CONSOLE)/arch/x86
NASM:=nasm/
NASMARCH:=$(NASM)/arch/x86
GRAPHICS:=$(SOURCE)/graphics
MALLOC:=$(SOURCE)/malloc
OBJ:=objs
CC:=i686-elf-gcc
KERNEL:=$(SOURCE)/kernel.o
VESA:=$(SOURCE)/vesa
ARCH:=$(SOURCE)/arch/x86
LIBARCH:=$(LIBRARY)/arch/x86
TIMER:=$(SOURCE)/timer
DESCRIPTORS:=$(SOURCE)/GDT_IDT
INTERRUPTS:=$(SOURCE)/Interrupts
DRIVERS:=$(SOURCE)/Drivers
PAGING:=$(SOURCE)/paging
KHEAP:=$(SOURCE)/kheap
VFS:=$(SOURCE)/vfs
MULTIBOOT:=$(SOURCE)/multiboot
MULTITASK:=$(SOURCE)/multitask
MEMMANAGEMENT:=$(SOURCE)/MemManagement
ATA:=$(SOURCE)/ATA

OBJS:= $(OBJ)/*.o
INCLUDED:=-ILibrary -I$(SOURCE) -I$(LIBARCH) -I$(MALLOC) -I$(VESA) -I$(GRAPHICS) -I$(CONSOLEVGA)
INCLUDED:=$(INCLUDED) -I$(CONSOLE) -I$(TIMER) -I$(KHEAP) -I$(PAGING) -I$(DESCRIPTORS) -I$(INTERRUPTS) -I$(ARCH) -I$(DRIVERS) -I./
INCLUDED:=$(INCLUDED) -I$(VFS) -I$(MULTIBOOT) -I$(MULTITASK) -I$(MEMMANAGEMENT) -I$(ATA)

FLAGS:= -O2 -g -ffreestanding -fbuiltin -Wall -Wextra -std=gnu11 -nostdlib -lgcc -fno-builtin -fno-stack-protector $(INCLUDED)
all: clean build-nasm build-kernel

clean:
	rm -f build-kernel *.o */*.o */*/*.o

build-nasm:
	mkdir -p objs
	nasm -f elf $(ARCH)/process.s -o $(OBJ)/process.o
	nasm -f elf $(NASMARCH)/*.asm -o $(OBJ)/arch.o
	nasm -f elf $(ARCH)/descriptors.asm -o $(OBJ)/descriptors.o
	nasm -f elf $(ARCH)/interrupts.s -o $(OBJ)/interrupts.o
	i686-elf-as $(SOURCE)/arch/x86/boot.S -o $(OBJ)/boot.o

build-kernel: $(KERNEL) linker.ld
	$(CC) -T linker.ld -o $(OBJ)/Aqeous $(FLAGS) $(KERNEL) $(OBJS)
%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(FLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(FLAGS)
