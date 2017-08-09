#include "library.c"

extern void _main();

void _start()
{
  // FOR Time being, Lets just give the user program full kernel access;
  // Would work on it later. Lets just for now allow these things just for
  // performance reasons.
  printf = (func_t)LIBSYMTABLE[0];
  malloc = (func_t)LIBSYMTABLE[1];
  free = (func_t)LIBSYMTABLE[2];
  memset = (func_t)LIBSYMTABLE[3];
  memcpy = (func_t)LIBSYMTABLE[4];
  SYSCALL_SPACE = (uint32_t*)(SYSCALL_SPACE_bottom);
  SYSCALL_SPACE[0] = 0;
  printf("\nLoading Main Program...");
  _main();
  _syscall_(5, 0);
}
