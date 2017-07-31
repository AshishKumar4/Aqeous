#include <stdint.h>
#include <descriptors.h>
#include <sys.h>
#include <string.h>
#include <cpu/cpu.h>
#include "hpet.h"
#include "cmos.h"
#include "stdio.h"
#include "stdlib.h"


//TODO: More optimizations
// Internal function prototypes.
static void init_gdt();
static void init_idt();
static uint8_t makeFlagByte();

//Defines
#define KERNEL_MODE 0b00
#define USER_MODE 0b11

// Initialisation routine - zeroes all the interrupt service routines,
// initialises the GDT and IDT.
void init_descriptor_tables()
 {
    // Initialise the global descriptor table.
    init_gdt();
    init_idt();
 }

extern uint32_t initial_esp;

static void init_gdt()  // For BSP
 {
    uint32_t* gdt_new = (uint32_t*)(0x2000 + AP_startup_Code_sz + 16);
    AP_gdt_Setup((uint32_t*)(0x2000 + AP_startup_Code_sz + 8 + 16), gdt_new);
    lgdt((void *)gdt_new);
 }

 uintptr_t pmode_GDT_init(uint32_t APIC_id)
 {
   uintptr_t gdt_new = (uintptr_t)kmalloc(64);//(vector_addr + AP_startup_Code_sz + pmode_code_size + 16);
   AP_gdt_Setup((uint32_t*)(gdt_new + 8), (uint32_t*)gdt_new);//(vector_addr + AP_startup_Code_sz + pmode_code_size + 8 + 16), gdt_new);

   if(APIC_id == 0)
   {
     lgdt((void *)gdt_new);
   }
   else
   {
     uint32_t pmode_code_addr = (0x1000 + (APIC_id*0x2000)) + AP_startup_Code_sz + 16;
     ByteSequence_Replace(0x32409798, 4, (uint32_t)gdt_new, 4, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));
   }
   return gdt_new;
 }

 uintptr_t pmode_IDT_init(uint32_t APIC_id)
 {
   uintptr_t idt_new = (uintptr_t)kmalloc(960);//(vector_addr + AP_startup_Code_sz + pmode_code_size + 16);
   AP_idt_Setup((uint32_t*)(idt_new + 8), (uint32_t*)idt_new);//(vector_addr + AP_startup_Code_sz + pmode_code_size + 8 + 16), gdt_new);

   if(APIC_id == 0)
   {
      lidt((void *)idt_new);
   }
   else
   {
     uint32_t pmode_code_addr = (0x1000 + (APIC_id*0x2000)) + AP_startup_Code_sz + 16;
     ByteSequence_Replace(0x32409799, 4, (uint32_t)idt_new, 4, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));
   }
   return idt_new;
 }

 uintptr_t pmode_IDT_initP(uint32_t APIC_id)
 {
   uintptr_t idt_new = (uintptr_t)kmalloc(960);//(vector_addr + AP_startup_Code_sz + pmode_code_size + 16);
   AP_idt_Setup((uint32_t*)(idt_new + 8), (uint32_t*)idt_new);//(vector_addr + AP_startup_Code_sz + pmode_code_size + 8 + 16), gdt_new);

   return idt_new;
 }

 void pmode_GDT_lgdt(uint32_t APIC_id, uintptr_t gdt_new)
 {
   if(APIC_id == 0)
   {
     lgdt((void *)gdt_new);
   }
   else
   {
     uint32_t pmode_code_addr = (0x1000 + (APIC_id*0x2000)) + AP_startup_Code_sz + 16;
     ByteSequence_Replace(0x32409798, 4, (uint32_t)gdt_new, 4, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));
   }
 }

 void pmode_IDT_lidt(uint32_t APIC_id, uintptr_t idt_new)
 {
   if(APIC_id == 0)
   {
      lidt((void *)idt_new);
   }
   else
   {
     uint32_t pmode_code_addr = (0x1000 + (APIC_id*0x2000)) + AP_startup_Code_sz + 16;
     ByteSequence_Replace(0x32409799, 4, (uint32_t)idt_new, 4, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));
   }
 }

 void pmode_GDT_lgdt_Vec(uint32_t vector_addr, uintptr_t gdt_new)
 {
   uint32_t pmode_code_addr = vector_addr + AP_startup_Code_sz + 16;
   ByteSequence_Replace(0x32409798, 4, (uint32_t)gdt_new, 4, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));
 }

 void pmode_IDT_lidt_Vec(uint32_t vector_addr, uintptr_t idt_new)
 {
   uint32_t pmode_code_addr = vector_addr + AP_startup_Code_sz + 16;
   ByteSequence_Replace(0x32409799, 4, (uint32_t)idt_new, 4, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));
 }

 void AP_gdt_Setup(uint32_t* gdt, uint32_t* gdtr)
 {
   uint32_t* gdt_new = gdtr;
   uint16_t* gdt_new_ptr = (uint16_t*)gdtr;
   uint64_t* gdt_new_entries = (uint64_t*)gdt;
   memset(gdt_new, 0, 8);
   memset(gdt_new_entries, 0, 40);

   *gdt_new_ptr = ((sizeof(gdt_entry_t) * 5) - 1);
   ++gdt_new_ptr;
   uint32_t* gdt_new_ptr2=(uint32_t*)gdt_new_ptr;
   *gdt_new_ptr2 = ((uint32_t)gdt_new_entries);
   //while(1);
   gdt_set_gate(0, 0, 0, 0, 0, gdt_new_entries);                // Null segment
   gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF, (uint64_t*)gdt_new_entries); // Code segment
   gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF, (uint64_t*)gdt_new_entries); // Data segment
   gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF, (uint64_t*)gdt_new_entries); // User mode code segment
   gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF, (uint64_t*)gdt_new_entries); // User mode data segment
   tss_struct_t tss;
   gdt_set_gate(5,(uint32_t)&tss,sizeof(tss_struct_t),0x89,0x40, (uint64_t*)gdt_new_entries);
   tss.ss0=0x10;
   tss.esp0=initial_esp;
   tss.iomap=sizeof(tss_struct_t);
 }

 extern uint32_t gdt16_base;

 void AP_rmode_GDT_setup(uint32_t* gdt, uint32_t* gdtr)
 {
   uint32_t* gdt_new = gdtr;
   uint16_t* gdt_new_ptr = (uint16_t*)gdtr;
   uint64_t* gdt_new_entries = (uint64_t*)gdt;
   memset(gdt_new, 0, 8);
   memset(gdt_new_entries, 0, 40);

   *gdt_new_ptr = ((sizeof(gdt_entry_t) * 5) - 1);
   ++gdt_new_ptr;
   uint32_t* gdt_new_ptr2=(uint32_t*)gdt_new_ptr;
   *gdt_new_ptr2 = ((uint32_t)gdt_new_entries);

  memcpy(gdt_new_entries, &gdt16_base, 64);
  /*gdt_set_gate(0, 0, 0, 0, 0, gdt_new_entries);                // Null segment
  gdt_set_gate(1, 0, 0xFFFF0000, 0x9A, 0x0F, (uint64_t*)gdt_new_entries); // Code segment
  gdt_set_gate(2, 0, 0xFFFF0000, 0x92, 0x0F, (uint64_t*)gdt_new_entries); // Data segment
  gdt_set_gate(3, 0, 0xFFFF0000, 0x9A, 0x0F, (uint64_t*)gdt_new_entries); // User mode code segment
  gdt_set_gate(4, 0, 0xFFFF0000, 0x92, 0x0F, (uint64_t*)gdt_new_entries); // User mode data segment
*/
 }

// Set the value of one GDT entry.
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint16_t gran, uint64_t* gdt_base)
{
  uint32_t* entry = (uint32_t*)&gdt_base[num];
  *entry = (limit & 0xffff) | ((base & 0xffff)<<16); //lower 32 bit
  *(entry+1) = (((base >> 16) & 0xFF)) | ((access & 0xff)<<8) | (((limit >> 16) & 0x0F)<<16) | ((gran & 0xf0)<<16) | (((base >> 24) & 0xFF)<<24);
}

void test_handler()
{
  asm volatile("hlt");
//  *(uint32_t*)(0x50001000) = 12345;
  while(1);
}

extern void test_ttt();
extern uint32_t t_c;

uint32_t ddd = 0;
extern uint32_t dbuff_run;

void ttt_t()
{
  ++t_c;
  if(t_c == 6)
  {
  //  printf(" A%d %d", (uint32_t)*HPET_main_counter - ddd, ReadFromCMOS());
  //  ddd = (uint32_t)*HPET_main_counter;
    dbuff_run = 1;
    t_c = 0;
  }
}

void PIT_ab()
{
  asm volatile("iret");
}
void PIT_ab2()
{
  asm volatile("iret");
}

void AP_idt_Setup(uint32_t* idt, uint32_t* idtr)
{
  //The limit is 1 less than our table size because this is the end address
  uint16_t* ptr = (uint16_t*)idtr;
  *ptr = ((sizeof(idt_entry_t) * 256) - 1);
  ++ptr;
  uint32_t* ptr2=(uint32_t*)ptr;
  *ptr2 = ((uint32_t)idt);
  //We need to make sure the interrupt values are nulled out, else BAD things may happen
  memset(idt, 0, sizeof(idt_entry_t)*256);
/*
  for(int i = 0; i < 32; i++)
  {
    idtSetEntry(i, (uint32_t*)&INTbasedPmodeTrampoline, 0x08, makeFlagByte(1, KERNEL_MODE), idt);
  }*/
  int num = 0;
  idtSetEntry(num++, (uint32_t)&divByZero_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&debug_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&NMI_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&breakpoint_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&overflow_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&outOfBounds_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&invalidInstr_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&noCoprocessor_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&doubleFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&coprocessor_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&badTSS_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&segmentNotPresent_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&stackFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&INTbasedPmodeTrampoline, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&pageFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&unknownInterrupt_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&coprocessorFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&alignmentCheck_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&machineCheck_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&PIT_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&kb_handle, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&cascade_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&COM2_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&COM1_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&LPT2_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&floppyDisk_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&LPT1_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&RTC_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&periph1_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&periph2_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&periph3_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&mouse_handle, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&FPU_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&primaryHDD_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);
  idtSetEntry(num++, (uint32_t)&secondaryHDD_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)idt);

}
extern void abcdt1();

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

static void init_idt()
{
  //The limit is 1 less than our table size because this is the end address
  uint16_t* ptr = idt_ptr;
  *ptr = ((sizeof(idt_entry_t) * 256) - 1);
  ++ptr;
  uint32_t* ptr2=(uint32_t*)ptr;
  *ptr2 = ((uint32_t)&idt_entries);
  //We need to make sure the interrupt values are nulled out, else BAD things may happen
  memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

  int num = 0;
  idtSetEntry(num++, (uint32_t)&divByZero_handler, 0x08, makeFlagByte(1, KERNEL_MODE),(uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&debug_handler, 0x08, makeFlagByte(1, KERNEL_MODE),(uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&NMI_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&breakpoint_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&overflow_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&outOfBounds_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&invalidInstr_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&noCoprocessor_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&doubleFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&coprocessor_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&badTSS_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&segmentNotPresent_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&stackFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&generalProtectionFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&pageFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&unknownInterrupt_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&coprocessorFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&alignmentCheck_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&machineCheck_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&PIT_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&kb_handle, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&cascade_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&COM2_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&COM1_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&LPT2_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&floppyDisk_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&LPT1_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&RTC_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&periph1_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&periph2_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&periph3_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&mouse_handle, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&FPU_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&primaryHDD_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(num++, (uint32_t)&secondaryHDD_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);


  idtSetEntry(51, (uint32_t)&test_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  idtSetEntry(50, (uint32_t)&test_handler, 0x08, makeFlagByte(1, KERNEL_MODE), (uint64_t*)&idt_entries);
  num = 0;

  lidt((void *)&idt_ptr);
}

static uint8_t makeFlagByte(int isPresent, int ring)
{
   return (((isPresent & 0x01) << 7) | ((ring & 0x3) << 5) | (0b1110)) & (~(1));
}

static void idtSetEntry(int num, uint32_t base, uint32_t sel, uint32_t flags, uint64_t* idt_base)
{
  uint32_t* entry = (uint32_t*)&idt_base[num];
  *entry = (base & 0xFFFF) | (sel << 16);
  *(entry+1) = 0x0000 | (flags << 8) | (((base >> 16) & 0xFFFF) << 16);
}
