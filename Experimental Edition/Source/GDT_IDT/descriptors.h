#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H

#include <common.h>
#include <stdint.h>

extern void switcher();
extern void switcher_manual();
extern void lgdt(void *);
extern void lidt(void *);

typedef uint64_t gdt_entry_t;

typedef uint16_t gdt_ptr_t[3];

void init_descriptor_tables();

typedef uint64_t idt_entry_t;
/*
struct idt_entry_struct
 {
    uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t sel;                 // Kernel segment selector.
    uint8_t  always0;             // This must always be zero.
    uint8_t  flags;               // More flags. See documentation.
    uint16_t base_hi;             // The upper 16 bits of the address to jump to.
 } __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;
*/

typedef uint16_t idt_ptr_t[3];

//extern void Test_Timer();

//Interrupt handlers
void divByZero_handler();
void debug_handler();
void NMI_handler();
void breakpoint_handler();
void overflow_handler();
void outOfBounds_handler();
void invalidInstr_handler();
void noCoprocessor_handler();
void doubleFault_handler();
void coprocessor_handler();
void badTSS_handler();
void segmentNotPresent_handler();
void stackFault_handler();
void generalProtectionFault_handler();
void pageFault_handler();
void unknownInterrupt_handler();
void coprocessorFault_handler();
void alignmentCheck_handler();
void machineCheck_handler();
void reserved_handler();

//#ifdef PIC
void PIT_handler();
extern void kb_handle();
//extern void Scheduler_Promoter_assistance();
extern void switcher_ksp();
extern void switcher_ksp2();
void keyboardInterrupt_handler();
void cascade_handler();//This particular interrupt is never raised
void COM2_handler();
void COM1_handler();
void LPT2_handler();
void floppyDisk_handler();
void LPT1_handler();
void RTC_handler();
void periph1_handler();
void periph2_handler();
void periph3_handler();
void mouse_handle();
void FPU_handler();
void primaryHDD_handler();//P -> Primary
void secondaryHDD_handler();//S -> Secondary
//#endif

typedef volatile struct __tss_struct
{
    unsigned short   link;
    unsigned short   link_h;

    unsigned long   esp0;
    unsigned short   ss0;
    unsigned short   ss0_h;

    unsigned long   esp1;
    unsigned short   ss1;
    unsigned short   ss1_h;

    unsigned long   esp2;
    unsigned short   ss2;
    unsigned short   ss2_h;

    unsigned long   cr3;
    unsigned long   eip;
    unsigned long   eflags;

    unsigned long   eax;
    unsigned long   ecx;
    unsigned long   edx;
    unsigned long    ebx;

    unsigned long   esp;
    unsigned long   ebp;

    unsigned long   esi;
    unsigned long   edi;

    unsigned short   es;
    unsigned short   es_h;

    unsigned short   cs;
    unsigned short   cs_h;

    unsigned short   ss;
    unsigned short   ss_h;

    unsigned short   ds;
    unsigned short   ds_h;

    unsigned short   fs;
    unsigned short   fs_h;

    unsigned short   gs;
    unsigned short   gs_h;

    unsigned short   ldt;
    unsigned short   ldt_h;

    unsigned short   trap;
    unsigned short   iomap;

} tss_struct_t;

void gdt_set_gate(int num,uint32_t base,uint32_t limit,uint8_t access,uint16_t gran, uint64_t* gdt_base);

static void idtSetEntry(int num, uint32_t base, uint32_t sel, uint32_t flags, uint64_t* idt_base);

void AP_gdt_Setup(uint32_t* gdt, uint32_t* gdtr);
void AP_rmode_GDT_setup(uint32_t* gdt, uint32_t* gdtr);

void AP_idt_Setup(uint32_t* idt, uint32_t* idtr);

uintptr_t  pmode_GDT_init(uint32_t APIC_id);
uintptr_t  pmode_IDT_init(uint32_t APIC_id);
uintptr_t  pmode_IDT_initP(uint32_t APIC_id);
void pmode_GDT_lgdt(uint32_t APIC_id, uintptr_t gdt_new);
void pmode_IDT_lidt(uint32_t APIC_id, uintptr_t idt_new);
void pmode_GDT_lgdt_Vec(uint32_t vector_addr, uintptr_t gdt_new);
void pmode_IDT_lidt_Vec(uint32_t vector_addr, uintptr_t idt_new);

extern void pmode_AP_code();
extern void pmode_AP_code_end();
extern void INTbasedPmodeTrampoline();
void test_handler();

#endif
