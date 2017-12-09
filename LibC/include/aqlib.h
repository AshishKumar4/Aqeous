#ifndef _AQLIB_H_
#define _AQLIB_H_ 

#include "library.h"

#ifdef _AQ_LIBC_Compile_

#define STD_IN_normal 0x1255

uint32_t* LIBSYMTABLE = (uint32_t*)0xC0001000;

uint32_t glb_fd = 3;
uint32_t* glb_fd_tbl = (uint32_t*)0xC0002000;

volatile const uint32_t* argTable = (uint32_t*)0xC0003000;

uint32_t* SYSCALL_SPACE = (uint32_t*)0xC0004000;

const uint32_t* STD_OUT_Buff = (uint32_t*)0xC0007000;
uint32_t* STD_OUT_ptr = (uint32_t*)0xC0007000;

const uint32_t* STD_IN_Buff = (uint32_t*)0xC000a000;
uint32_t* STD_IN_ptr = (uint32_t*)0xC000a000;

uint32_t* std_in = (uint32_t*)0xC000a000;

//uint32_t* KernelMessageSpace = (uint32_t*)0xC0010000;

uint32_t STD_OUT_sz = 0;
uint32_t STD_OUT_sz4 = 0;
uint32_t default_console_color = 15, console_color;

uint32_t StrToInt(char *str);

#endif 

#endif 
