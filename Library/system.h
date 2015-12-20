
#ifndef SYSTEM_H
#define SYSTEM_H

struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
}*MOUSE_HANDLE;

// asm macros
#define INTELSYNTAX ( asm ( ".intel_syntax noprefix" ) )
#define ATTSYNTAX	( asm ( ".att_syntax" ) )


unsigned int read_cr0();
void write_cr0( unsigned int cr0 );


typedef unsigned int uint_t;


#ifndef NULL
#define NULL ((void*)0)
#endif

#endif
