#include "stdint.h"
#include "stdio.h"

// FIRST COMPILE THE LIBRARY, THEN COMPILE THIS
// i686-elf-gcc t.c -o t.o crt0.o -m32 -O2 -ffreestanding -fbuiltin -Wall -Wextra -nostdlib -lgcc -fno-builtin -fno-stack-protector -Iinclude
// TEST PROGRAM

void fibonacci()
{
  printf("\nPrinting Fibonacci Series-->");
  int i, n = 100, t1 = 0, t2 = 1, nextTerm;

    printf("Fibonacci Series: ");

    for (i = 1; i <= n; ++i)
    {
        printf("%d, ", t1);
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }
}

void main()
{
  printf("\nHello World");
  asm volatile("int $50");
  asm volatile("int $50");
  fibonacci();
  int i = 0;
  scanf("%d", &i);
  printf("\n{%d} Testing... IT WORKS!!!", i);
}
