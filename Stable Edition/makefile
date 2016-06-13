SOURCE:=Source
LIBRARY:=Library
CONSOLE:=$(SOURCE)/console
CONSOLEVGA:=$(CONSOLE)/arch/x86
NASM:=nasm/
NASMARCH:=$(NASM)/arch/x86
GRAPHICS:=$(SOURCE)/graphics
OBJ:=objs
CC:=i686-elf-gcc-4.9.2
KERNEL:=$(SOURCE)/kernel.o
VESA:=$(SOURCE)/vesa
ARCH:=$(SOURCE)/arch/x86
LIBARCH:=$(LIBRARY)/arch/x86
DESCRIPTORS:=$(SOURCE)/GDT_IDT
INTERRUPTS:=$(SOURCE)/Interrupts
DRIVERS:=$(SOURCE)/Drivers
TIMER:=$(DRIVERS)/timer
VFS:=$(SOURCE)/vfs
MULTIBOOT:=$(SOURCE)/multiboot
MULTITASK:=$(SOURCE)/multitasking
MEMMANAGEMENT:=$(SOURCE)/MemManagement
ATA:=$(DRIVERS)/ATA
PCI:=$(DRIVERS)/PCI
AHCI:=$(DRIVERS)/AHCI
ACPI:=$(DRIVERS)/ACPI
MOUSE:=$(DRIVERS)/mouse
KEYBOARD:=$(DRIVERS)/keyboard
CPUID:=$(ARCH)/cpuid
APIC:=$(ARCH)/APIC
PIC:=$(ARCH)/PIC
FILESYSTEM:=$(SOURCE)/FileSystem
HPET:=$(TIMER)/HPET
BASICSHELL:=$(SOURCE)/Shell
IOQUEUE:=$(SOURCE)/IOQueues
KBQUEUE:=$(IOQUEUE)/Keyboard

OBJS:= $(OBJ)/*.o
INCLUDED:=-ILibrary -I$(SOURCE) -I$(LIBARCH) -I$(VESA) -I$(GRAPHICS) -I$(CONSOLEVGA)
INCLUDED:=$(INCLUDED) -I$(CONSOLE) -I$(TIMER) -I$(DESCRIPTORS) -I$(INTERRUPTS) -I$(ARCH) -I./
INCLUDED:=$(INCLUDED) -I$(VFS) -I$(MULTIBOOT) -I$(MULTITASK) -I$(MEMMANAGEMENT) -I$(ATA) -I$(PCI) -I$(AHCI) -I$(ACPI) -I$(MOUSE)
INCLUDED:=$(INCLUDED) -I$(KEYBOARD) -I$(DRIVERS) -I$(CPUID) -I$(APIC) -I$(FILESYSTEM) -I$(HPET) -I$(BASICSHELL) -I$(IOQUEUE) -I$(KBQUEUE)
INCLUDED:=$(INCLUDED) -I$(PIC)

FLAGS:= -O2 -ffreestanding -fbuiltin -Wall -Wextra -std=gnu11 -nostdlib -lgcc -fno-builtin -fno-stack-protector $(INCLUDED)
all: clean build-nasm build-kernel

clean:
	rm -f build-kernel *.o */*.o */*/*.o

build-nasm:
	mkdir -p objs
	nasm -f elf $(NASMARCH)/v86.asm -o $(OBJ)/v86.o
	nasm -f elf $(ARCH)/descriptors.asm -o $(OBJ)/descriptors.o
	nasm -f elf $(ARCH)/tasking.asm -o $(OBJ)/tasking.o
	nasm -f elf $(ARCH)/interrupts.asm -o $(OBJ)/interrupts.o
	i686-elf-as $(SOURCE)/arch/x86/boot.S -o $(OBJ)/boot.o

build-kernel: $(KERNEL) linker.ld
	$(CC) -T linker.ld -o $(OBJ)/Aqeous $(FLAGS) $(KERNEL) $(OBJS)
%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(FLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(FLAGS)
