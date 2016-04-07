#ifndef DESCRIPTORS_H
#define DESCRIPTOR_H
#define PIC
#include <common.h>
#include <stdint.h>
/*struct gdt_entry_struct
 {
    uint16_t limit_low;           // The lower 16 bits of the limit.
    uint16_t base_low;            // The lower 16 bits of the base.
    uint8_t  base_middle;         // The next 8 bits of the base.
    uint8_t  access;              // Access flags, determine what ring this segment can be used in.
    uint8_t  granularity;
    uint8_t  base_high;           // The last 8 bits of the base.
    } __attribute__((packed));*/

typedef uint64_t gdt_entry_t;

/*struct gdt_ptr_struct
 {
    uint16_t limit;               // The upper 16 bits of all selector limits.
    uint32_t base;                // The address of the first gdt_entry_t struct.
 }
 __attribute__((packed));*/

typedef uint16_t gdt_ptr_t[3];

void init_descriptor_tables();
/*struct idt_entry_struct
 {
    uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t sel;                 // Kernel segment selector.
    uint8_t  always0;             // This must always be zero.
    uint8_t  flags;               // More flags. See documentation.
    uint16_t base_hi;             // The upper 16 bits of the address to jump to.
    } __attribute__((packed));*/
typedef uint64_t idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
/*struct idt_ptr_struct
 {
    uint16_t limit;
    uint32_t base;                // The address of the first element in our idt_entry_t array.
    } __attribute__((packed));*/
typedef uint16_t idt_ptr_t[3];

//Interrupt handlers
extern void divByZero_handler();
extern void debug_handler();
extern void NMI_handler();
extern void breakpoint_handler();
extern void overflow_handler();
extern void outOfBounds_handler();
extern void invalidInstr_handler();
extern void noCoprocessor_handler();
extern void doubleFault_handler();
extern void coprocessor_handler();
extern void badTSS_handler();
extern void segmentNotPresent_handler();
extern void stackFault_handler();
extern void generalProtectionFault_handler();
extern void pageFault_handler();
extern void unknownInterrupt_handler();
extern void coprocessorFault_handler();
extern void alignmentCheck_handler();
extern void machineCheck_handler();
extern void reserved_handler();

#ifdef PIC
extern void PIT_handler();
extern void keyboard_interrupt();
extern void cascade_handler();//This particular interrupt is never raised
extern void COM2_handler();
extern void COM1_handler();
extern void LPT2_handler();
extern void FD_handler();
extern void RTC_handler();
extern void perpiph1_handler();
extern void perpiph2_handler();
extern void perpiph3_handler();
extern void mouse_handler();
extern void FPU_handler();
extern void PHDD_handler();//P -> Primary
extern void SHDD_handler();//S -> Secondary
#endif

#endif
