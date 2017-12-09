#include "library.h"
#include "stdint.h"
#include "syscalls.h"
#include "Syscalls/nativeCalls.c"
#include "Syscalls/posixCalls.c"


uint32_t __attribute__((optimize("O0"))) _syscall_pop()
{
  uint32_t fr = SYSCALL_SPACE[SYSCALL_SPACE[0]];
  --SYSCALL_SPACE[0];
  return fr;
}

void __attribute__((optimize("O0"))) _syscall_push(uint32_t fr)
{
  ++SYSCALL_SPACE[0];
  SYSCALL_SPACE[SYSCALL_SPACE[0]] = fr;
}

uint32_t __attribute__((optimize("O0"))) _syscall_(uint32_t call)
{
  _syscall_push(call);
  _SYSCALL_();            //    Execute The System Call
  return _syscall_pop();
}
