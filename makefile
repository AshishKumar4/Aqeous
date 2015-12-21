SOURCE:=source
LIBRARY:=library/
CONSOLE:=$(SOURCE)/console
CONSOLEVGA:=$(CONSOLE)/arch/x86
NASM:=nasm/
NASMARCH:=$(NASM)/arch/x86
GRAPHICS:=$(SOURCE)/graphics
MALLOC:=$(SOURCE)/malloc
OBJ:=objs/
CC:=i686-elf-gcc
KERNEL:=$(SOURCE)/kernel.o
VESA:=$(SOURCE)/vesa
ARCH:=$(SOURCE)/arch/x86

OBJS:= $(OBJ)/*.o
INCLUDED:=-Ilibrary -I$(SOURCE) -I$(MALLOC) -I$(VESA) -I$(GRAPHICS) -I$(CONSOLEVGA) -I$(CONSOLE) -I$(ARCH) -I./

FLAGS:= -O2 -g -ffreestanding -fbuiltin -Wall -Wextra -std=gnu11 -nostdlib -lgcc $(INCLUDED)
all: clean build-nasm build.kernel

clean:
	rm -f build.kernel *.o */*.o */*/*.o

build-nasm:
	nasm -f elf $(NASMARCH)/*.asm -o $(OBJ)/arch.o
	i686-elf-as $(SOURCE)/arch/x86/boot.s -o $(OBJ)/boot.o

build.kernel: $(KERNEL) linker.ld
	$(CC) -T linker.ld -o $(OBJ)/Aqeous.water $(FLAGS) $(KERNEL) $(OBJS)
%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(FLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(FLAGS)
