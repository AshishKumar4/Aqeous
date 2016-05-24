#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47


#ifndef SYSTEM_H
#define SYSTEM_H

typedef struct regs
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
