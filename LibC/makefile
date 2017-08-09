OBJ:=objs
CC:=i686-elf-gcc
CPP:=i686-elf-g++
LD:=i686-elf-ld
AS:=i686-elf-as
NASM:=nasm

SOURCE:=
LIBRARY:=include

CRT0:=crt0.o

OBJS:= *.o
INCLUDED:=	-I$(LIBRARY)

FLAGS:= -m32 -O2 -ffreestanding -fbuiltin -Wall -Wextra -nostdlib -lgcc -fno-builtin -fno-stack-protector $(INCLUDED)
all: clean build-crt0

build-crt0:	$(CRT0)
	echo -e "\n\n\n\n\tC Library for Aqeous Built successfully"

build-prog:	$(CRT0)
	$(LD) -T linker.ld -o t1.o $(OBJS) $(crt0)

clean:
	rm -f build-crt0 *.o */*.o */*/*.o
	rm -rfv objs

%.o: %.c
	$(CC) -c $< -o $@ $(FLAGS)

%.o: %.cpp
	$(CPP) -c $< -o $@ $(FLAGS)

%.o: %.asm
	$(NASM)	-f elf $c -o $@

%.o: %.S
	$(AS) $< -o $@
