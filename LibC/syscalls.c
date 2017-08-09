#include "library.h"
#include "stdint.h"
#include "syscalls.h"

uint32_t _syscall_pop()
{
  uint32_t fr = SYSCALL_SPACE[SYSCALL_SPACE[0]];
  --SYSCALL_SPACE[0];
  return fr;
}

void _syscall_push(uint32_t fr)
{
  ++SYSCALL_SPACE[0];
  SYSCALL_SPACE[SYSCALL_SPACE[0]] = fr;
}

uint32_t _syscall_(uint32_t call, uint32_t n_params)
{
//  memset(SYSCALL_SPACE - 1024, 0x4284, 1024);
  _syscall_push(n_params);
  _syscall_push(call);
  asm volatile("int $54;");
  uint32_t returned_value = _syscall_pop();
  return returned_value;
}
