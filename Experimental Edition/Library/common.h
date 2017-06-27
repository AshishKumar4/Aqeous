#ifndef COMMON_H
#define COMMON_H

#include <system.h>
#include <stdint-gcc.h>
#include <stdbool.h>

typedef void (*func_t)();	//void function pointer
typedef uint32_t (*intfunc2_t)(uint32_t, uint32_t);	//int function pointer with 2 arguments

#ifndef NULL
#define NULL ((void*) 0)
#endif

#define KERNEL_BASE 0xC0000000

func_t Screen_BuffSync;

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
	uint32_t __a = (uint32_t) (a);				\
	(typeof(a)) (__a - __a % (n));				\
})
// Round up to the nearest multiple of n
#define ROUNDUP(a, n)						\
({								\
	uint32_t __n = (uint32_t) (n);				\
	(typeof(a)) (ROUNDDOWN((uint32_t) (a) + __n - 1, __n));	\
})

#define sgn(x) ((x<0)?-1:((x>0)?1:0)) /* macro to return the sign of a
                                         number */
#define abs(x) ((x<0)?-x:x) /* macro to return the absolute magnitude of a
																				number */


#define DECLARE_LOCK(name) volatile int name ## Locked
#define LOCK(name) \
	while (!__sync_bool_compare_and_swap(& name ## Locked, 0, 1)); \
	__sync_synchronize();
#define UNLOCK(name) \
	__sync_synchronize(); \
	name ## Locked = 0;

DECLARE_LOCK(printlock);

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

extern void memcpy_rep(uint32_t s, uint32_t d, uint32_t sz);
extern void Enable_SSE();
extern void memcpy_sse(uint32_t s, uint32_t d, uint32_t sz);
extern void memset_sse(uint32_t s, int a, uint32_t sz);

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

inline uint32_t Lower16(uint32_t val)
{
	uint32_t a=val&0xffff;
	return a;
}

inline uint32_t Higher16(uint32_t val)
{
	uint32_t b=val>>16;
	return b;
}

#define TRUE    1  //define a few variable
#define ON      1
#define FALSE   0
#define OFF     0

int posix_time()
{
	return 0xffff;
}

#endif // COMMON_H
