#ifndef COMMON_H
#define COMMON_H

#include <system.h>
#include <stdint-gcc.h>

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;
typedef unsigned int   uint_t;
typedef          int   sint_t;
typedef unsigned short ushort_t;
typedef          short short_t;
typedef unsigned char  uchar_t;
typedef          char  schar_t;

#ifndef NULL
#define NULL ((void*) 0)
#endif

// Represents true-or-false values
typedef _Bool bool;
enum { false, true };

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

// Return the offset of 'member' relative to the beginning of a struct type
#define offsetof(type, member)  ((size_t) (&((type*)0)->member))
typedef struct {
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
    u32int ds;                  // Data segment selector
    u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    u32int int_no, err_code;    // Interrupt number and error code (if applicable)
    u32int eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
 } registers_t;

typedef void (*isr_t)(registers_t);


void panic(const char *message, const char *file, u32int line)
{

}
void panic_assert(const char *file, u32int line, const char *desc)
{

}
#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))


#endif // COMMON_H
