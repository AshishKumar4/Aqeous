#ifndef STDLIB_h
#define STDLIB_h
#include <stdlib.h>
#include <stdio.h>
__attribute__((__noreturn__))
void abort(void)
{
	// TODO: Add proper kernel panic.
	printf("Kernel Panic: abort()\n");
	while ( 1 ) { }
	__builtin_unreachable();
}

#endif
