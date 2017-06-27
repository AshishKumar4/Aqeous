SOURCE:=Source
LIBRARY:=Library
CONSOLE:=$(SOURCE)/console
CONSOLEVGA:=$(CONSOLE)/arch/x86
GRAPHICS:=$(SOURCE)/graphics
OBJ:=objs
CC:=i686-elf-gcc
CPP:=i686-elf-g++
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
#VFS:=$(SOURCE)/vfs
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
IOHANDLING:=$(SOURCE)/IO_Handling
IO_DEVICES:=$(IOHANDLING)/IO_Devices
KBHANDLE:=$(IO_DEVICES)/Keyboard
FSHANDLE:=$(IO_DEVICES)/FileSystem
PARALLEL_IO:=$(IOHANDLING)/Parallel_Processing
SERIAL_IO:=$(IOHANDLING)/Serial_Processing
FONT_ENGINE:=$(GRAPHICS)/Font_Engine
GUI_SHELL:=$(BASICSHELL)/GUI
AQFS:=$(FILESYSTEM)/AqFS

OBJS:= $(OBJ)/*.o
INCLUDED:= -ILibrary -I$(SOURCE) -I$(LIBARCH) -I$(VESA) -I$(GRAPHICS) -I$(FONT_ENGINE) -I$(CONSOLEVGA) -I$(FONT_ENGINE)
INCLUDED:=$(INCLUDED) -I$(CONSOLE) -I$(TIMER) -I$(DESCRIPTORS) -I$(INTERRUPTS) -I$(ARCH) -I./
INCLUDED:=$(INCLUDED) -I$(MULTIBOOT) -I$(MULTITASK) -I$(MEMMANAGEMENT) -I$(ATA) -I$(PCI) -I$(AHCI) -I$(ACPI) -I$(MOUSE)
INCLUDED:=$(INCLUDED) -I$(KEYBOARD) -I$(DRIVERS) -I$(CPUID) -I$(APIC) -I$(FILESYSTEM) -I$(AQFS) -I$(HPET) -I$(BASICSHELL) -I$(IOHANDLING) -I$(KBHANDLE) -I$(FSHANDLE)
INCLUDED:=$(INCLUDED) -I$(PIC) -I$(PARALLEL_IO) -I$(SERIAL_IO) -I$(GUI_SHELL)

FLAGS:= -O2 -ffreestanding -fbuiltin -Wall -Wextra -nostdlib -lgcc -fno-builtin -fno-stack-protector $(INCLUDED)
all: clean build-nasm build-kernel

clean:
	rm -f build-kernel *.o */*.o */*/*.o
	rm -rfv objs
	rm -rfv isodir

build-nasm:
	mkdir -p objs
	$(NASM) -f elf $(ARCH)/v86.asm -o $(OBJ)/v86.o
	$(NASM) -f elf $(ARCH)/descriptors.asm -o $(OBJ)/descriptors.o
	$(NASM) -f elf $(ARCH)/tasking.asm -o $(OBJ)/tasking.o
	$(NASM) -f elf $(ARCH)/interrupts.asm -o $(OBJ)/interrupts.o
	$(NASM) -f elf $(ARCH)/FaultHandlers.asm -o $(OBJ)/FaultHandlers.o
	$(NASM) -f elf $(ARCH)/asmlibrary.asm -o $(OBJ)/asmlibrary.o
	$(NASM) -f elf $(ARCH)/a.asm -o $(OBJ)/a.o
	$(NASM) -f elf $(ARCH)/scheduler.asm -o $(OBJ)/scheduler.o
	$(NASM) -f elf $(ARCH)/AP_Bootup.asm -o $(OBJ)/AP_Bootup.o
	$(AS) $(SOURCE)/arch/x86/boot.S -o $(OBJ)/boot.o

build-kernel: $(KERNEL) linker.ld
	$(LD) -T linker.ld -o $(OBJ)/Aqeous $(OBJS) $(KERNEL)
	cp $(OBJ)/Aqeous Aqeous.bin
	make clean
	echo -e "\n\n\n\n\tKernel Built Successfully! Run it using Qemu_aqeous.bat. \n\tBe sure to modify it accordingly!!!"

build-iso:
	sh grub_config.sh
	grub-mkrescue -o aqeous.iso --modules="multiboot normal ext2 part_msdos" isodir

build-installer:
	cd installer
	make
	cd ..

build-assembly:
	$(CC) -T linker.ld $(FLAGS) -S -m32 $(SOURCE)/kernel.c

build-objdump:
	objdump -D -M intel aqeous.bin > aqeous.txt

%.o: %.c
	$(CC) -c $< -o $@ $(FLAGS)

%.o: %.cpp
	$(CPP) -c $< -o $@ $(FLAGS)

%.o: %.asm
	$(NASM)	-f elf $c -o $@

%.o: %.S
	$(AS) $< -o $@
