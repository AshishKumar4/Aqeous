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

uint32_t StrToInt(char *str)
{
		if(!str) return 0;
    uint32_t in=0;
    int ln=strlen(str);
    int arr[999],a=1;
    for(int i=0;i<=ln;i++) a=a*10;
    a=a/100;
    for(int i=0;i<=ln;i++)
    {
        arr[i]=str[i]-48;
        in=in+arr[i]*a;
        a=a/10;
    }
		return in;
}

uint32_t (*kmalloc)(uint32_t);
uint32_t (*malloc)(uint32_t);
void (*free)(uint32_t);
void (*kfree)(uint32_t);

#endif
