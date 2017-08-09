#ifndef _SYSCALL_H_
#define _SYSCALL_H_
#include "stdint.h"

uint32_t* LIBSYMTABLE = (uint32_t*)0xC0001000;
uint32_t SYSCALL_SPACE_bottom = 0xC0004000;
uint32_t* SYSCALL_SPACE;

uint32_t _syscall_pop();

void _syscall_push(uint32_t fr);

uint32_t _syscall_(uint32_t call, uint32_t n_params);

#endif
