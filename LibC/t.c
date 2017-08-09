#include "stdint.h"
#include "include/aqeouslib.h"

// FIRST COMPILE THE LIBRARY, THEN COMPILE THIS
// i686-elf-gcc t.c -o t.o crt0.o -m32 -O2 -ffreestanding -fbuiltin -Wall -Wextra -nostdlib -lgcc -fno-builtin -fno-stack-protector
// TEST PROGRAM

void fibonacci()
{
  printf("\nPrinting Fibonacci Series-->");
  int i, n = 5, t1 = 0, t2 = 1, nextTerm;

    printf("Fibonacci Series: ");

    for (i = 1; i <= n; ++i)
    {
        printf("%d, ", t1);
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }
}

void _main()
{
  printf("\nHello World");
  fibonacci();

  _syscall_push(4096);
  _syscall_(1, 1);
  printf("Testing... IT WORKS");
}
