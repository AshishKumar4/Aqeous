void _start()
{
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
    asm volatile("int $50");
   // asm volatile("hlt");
    while(1)
        asm volatile("int $50");
}

// i686-elf-gcc tl.c -o tl.o -m32 -O2 -ffreestanding -fbuiltin -Wall -Wextra -nostdlib -lgcc -fno-builtin -fno-stack-protector