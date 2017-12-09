#include "stdlib.h"
#include "syscalls.h"

void* malloc(size_t size)
{
    _syscall_push(size);
    int tmp = _syscall_(__SYS_MALLOC__);
    return (void*)tmp;
}

void free(void* ptr)
{
    return;
}