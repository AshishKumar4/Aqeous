
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
#ifndef COMMON_h
#define COMMON_h
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

#define TRUE    1  //define a few variable
#define ON      1
#define FALSE   0
#define OFF     0

#endif // COMMON_h

