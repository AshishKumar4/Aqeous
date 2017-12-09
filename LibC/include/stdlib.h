#ifndef _STDLIB_H_
#define _STDLIB_H_ 

#include "stdint.h"

#ifdef _AQ_LIBC_Compile_
#include "library.h"

#else 

#endif

#ifndef NULL
#define NULL ((void*) 0)
#endif

void exit(int code);

void* malloc(size_t size);
void free(void* ptr);

#endif 