#ifndef COMMON_H
#define COMMON_H

#include <system.h>
#include <stdint-gcc.h>
#include <stdbool.h>

typedef void (*func_t)();	//void function pointer

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef 	uint32_t   u32int;
typedef          int   s32int;
typedef 	  uint16_t u16int;
typedef          short s16int;
typedef   	  uint8_t  u8int;
typedef          char  s8int;

#ifndef NULL
#define NULL ((void*) 0)
#endif

#define KERNEL_BASE 0xC0000000

// Efficient min and max operations
#define MIN(_a, _b)						\
({								\
	typeof(_a) __a = (_a);					\
	typeof(_b) __b = (_b);					\
	__a <= __b ? __a : __b;					\
})
#define MAX(_a, _b)						\
({								\
	typeof(_a) __a = (_a);					\
	typeof(_b) __b = (_b);					\
	__a >= __b ? __a : __b;					\
})

// Rounding operations (efficient when n is a power of 2)
// Round down to the nearest multiple of n
#define ROUNDDOWN(a, n)						\
({								\
	uint64_t __a = (uint64_t) (a);				\
	(typeof(a)) (__a - __a % (n));				\
})
// Round up to the nearest multiple of n
#define ROUNDUP(a, n)						\
({								\
	uint64_t __n = (uint64_t) (n);				\
	(typeof(a)) (ROUNDDOWN((uint64_t) (a) + __n - 1, __n));	\
})

typedef struct
{
    int inode_num;//This field will be index in the tarfs table
    char filename[100];
    int perm;
    int size;
    char type;
    int sector_loc[10];
    int offset;
    uint64_t address;
    int is_fs;
} file;

#define SEEK_SET 1
#define SEEK_CUR 2
#define SEEK_END 3


typedef struct registers
 {
    uint32_t ds;                  // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
 } registers_t;

typedef void (*isr_t)(registers_t);

typedef void (*irq_t)();

#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

inline uint32_t Lower32(uint64_t val)
{
	uint32_t a=val&0xffffffff;
	return a;
}

inline uint32_t Higher32(uint64_t val)
{
	uint32_t b=val>>32;
	return b;
}


#endif // COMMON_H
