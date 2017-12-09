#include "stdio.h"

void main()
{
   // asm volatile("hlt");
    asm volatile("int $50; hlt");
    while(1);
}