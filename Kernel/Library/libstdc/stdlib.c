#include "stdlib.h"
#include "stdio.h"
__attribute__((__noreturn__))
void abort(void)
{
	// TODO: Add proper kernel panic.
	printf("Kernel Panic: abort()\n");
	while ( 1 ) { }
	__builtin_unreachable();
}

int ByteSequence_Replace(uint32_t magic, uint32_t nbytes, uint32_t replacement, uint32_t mbytes, uint32_t* start, uint32_t* end);

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


void hexdump_8(uintptr_t addr, uint32_t n)
{
	uint8_t* ptr = (uint8_t*)addr;
	for(uint32_t i = 0; i < n; i++)
	{
		if(!(i % 4)) printf("\n");
		printf("[%x] ", *ptr);
		++ptr;
	}
}
