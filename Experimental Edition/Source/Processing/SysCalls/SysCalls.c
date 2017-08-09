#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "tasking.h"
#include "common.h"
#include "SysCalls.h"

inline uint32_t _syscall_pop()
{
  uint32_t fr = SYSCALL_SPACE[SYSCALL_SPACE[0]];
  --SYSCALL_SPACE[0];
  return fr;
}

inline void _syscall_push(uint32_t fr)
{
  ++SYSCALL_SPACE[0];
  SYSCALL_SPACE[SYSCALL_SPACE[0]] = fr;
}

extern uint32_t tmp_esp;

void syscall_init()
{
  SYSCALL_SPACE_bottom = (uint32_t)pmem_4k(1);
  SYSCALL_SPACE = (uint32_t*)(SYSCALL_SPACE_bottom);
  SYSCALL_SPACE_top = SYSCALL_SPACE;
  tmp_esp = pmem_4k(1);
  tmp_esp += 1023;
}

void syscall_MapPdir(PageDirectory_t* pdir)
{
  Map_non_identity(SYSCALL_SPACE_bottom, 0xC0004000, 4096, pdir);
  printf("\n{%x %x}", SYSCALL_SPACE, SYSCALL_SPACE_bottom);
}

void syscall_handler()
{
  uint32_t call = _syscall_pop();
//  printf("[%d]", call);
  func_t handle = LIBSYMTABLE[call];
  handle();
}
