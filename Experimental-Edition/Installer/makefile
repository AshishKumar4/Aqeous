SOURCE:=Source
LIBRARY:=Library
CONSOLE:=$(SOURCE)/console
CONSOLEVGA:=$(CONSOLE)/arch/x86
GRAPHICS:=$(SOURCE)/graphics
OBJ:=objs
CC:=i686-elf-gcc
LD:=i686-elf-ld
AS:=i686-elf-as
NASM:=nasm

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
INCLUDED:= -ILibrary -I$(SOURCE) -I$(LIBARCH) -I$(VESA) -I$(GRAPHICS) -I$(CONSOLEVGA)
INCLUDED:=$(INCLUDED) -I$(CONSOLE) -I$(TIMER) -I$(DESCRIPTORS) -I$(INTERRUPTS) -I$(ARCH) -I./
INCLUDED:=$(INCLUDED) -I$(VFS) -I$(MULTIBOOT) -I$(MULTITASK) -I$(MEMMANAGEMENT) -I$(ATA) -I$(PCI) -I$(AHCI) -I$(ACPI) -I$(MOUSE)
INCLUDED:=$(INCLUDED) -I$(KEYBOARD) -I$(DRIVERS) -I$(CPUID) -I$(APIC) -I$(FILESYSTEM) -I$(HPET) -I$(BASICSHELL) -I$(IOQUEUE) -I$(KBQUEUE)
INCLUDED:=$(INCLUDED) -I$(PIC)

FLAGS:= -O2 -ffreestanding -fbuiltin -Wall -Wextra -nostdlib -lgcc -fno-builtin -fno-stack-protector $(INCLUDED)
all: clean build-nasm build-kernel

clean:
	rm -f build-kernel *.o */*.o */*/*.o
	rm -rfv objs
	rm -rfv isodir

build-nasm:
	mkdir -p objs
	$(NASM) -f elf $(ARCH)/descriptors.asm -o $(OBJ)/descriptors.o
	$(NASM) -f elf $(ARCH)/interrupts.s -o $(OBJ)/interrupts.o
	$(AS) $(SOURCE)/arch/x86/boot.S -o $(OBJ)/boot.o

build-kernel: $(KERNEL) linker.ld
	$(LD) -T linker.ld -o $(OBJ)/Installer $(OBJS) $(KERNEL) -O0
	cp $(OBJ)/Installer Installer.aqeous

build-iso:
	sh grub_config.sh
	grub-mkrescue -o installer.iso isodir
	make clean

%.o: %.c
	$(CC) -c $< -o $@ $(FLAGS)

%.o: %.asm
	$(NASM)	-f elf $c -o $@

%.o: %.S
	$(AS) $< -o $@
