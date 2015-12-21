SOURCE:=source/
LIBRARY:=library/
CONSOLE:=$(LIBRARY)/console/
NASM:=nasm/
NASMARCH:=$(NASM)/arch/x86/
LIBRARYHEADERS:=$(LIBRARY)/*o
GRAPHICS:=$(SOURCE)/graphics
MALLOC:=$(SOURCE)/malloc/
KERNEL:=$(SOURCE)/kernel.o
OBJ:=objs/
CC:=i686-elf-gcc
FILES:=$(SOURCE)/kernel.o

OBJS:= $(OBJ)/*.o

FLAGS:= -O2 -g -ffreestanding -fbuiltin -Wall -Wextra -std=gnu11 -nostdlib -lgcc -Ilibrary -Isource -Isource/malloc -Isource/vesa -Isource/graphics -Isource/console -Isource/console/arch/x86 -I./


all: clean build-nasm build.kernel

clean:
	rm -f build.kernel *.o */*.o */*/*.o

build-nasm:
	nasm -f elf $(NASMARCH)/*.asm -o $(OBJ)/arch.o
	i686-elf-as $(SOURCE)/arch/x86/boot.s -o $(OBJ)/boot.o

build.kernel: $(FILES) linker.ld
	$(CC) -T linker.ld -o $(OBJ)/Aqeous.water $(FLAGS) $(FILES) $(OBJS)
%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(FLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(FLAGS)
