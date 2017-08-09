#ifndef STDLIB_h
#define STDLIB_h
#include <stdlib.h>
#include <stdio.h>
__attribute__((__noreturn__))
void abort(void);
int ByteSequence_Replace(uint32_t magic, uint32_t nbytes, uint32_t replacement, uint32_t mbytes, uint32_t* start, uint32_t* end);
uint32_t StrToInt(char *str);
void* (*kmalloc)(uint32_t);
uint32_t (*malloc)(uint32_t);
void (*free)(void*);
void (*kfree)(void*);
void* (*krealloc)(void*, size_t);

void hexdump_8(uintptr_t addr, uint32_t n);

#endif
