#include <stdint.h>
#include <descriptors.h>
#include <sys.h>
#include <string.h>

extern void switcher();
extern void lgdt(void *);
extern void lidt(void *);


//TODO: More optimizations
// Internal function prototypes.
static void init_gdt();
static void gdt_set_gate(int num,uint32_t base,uint32_t limit,uint8_t access,uint16_t gran);
static void init_idt();
static void idtSetEntry(int num, uint32_t base, uint32_t sel, uint32_t flags);
static uint8_t makeFlagByte();



gdt_entry_t gdt_entries[10];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;


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

static void init_gdt()
 {
    uint16_t* ptr = gdt_ptr;
    *ptr = ((sizeof(gdt_entry_t) * 5) - 1);
    ++ptr;
    uint32_t* ptr2=ptr;
    *ptr2 = ((uint32_t)&gdt_entries);
    //*(gdt_ptr+2)  = ((((uint32_t)&gdt_entries) & (0xFFFF0000)) >> 16);

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

		tss_struct_t tss;
		gdt_set_gate(5,(uint32_t)&tss,sizeof(tss_struct_t),0x89,0x40);
		tss.ss0=0x10;
		tss.esp0=initial_esp;
		tss.iomap=sizeof(tss_struct_t);

    lgdt((void *)&gdt_ptr);
 }

// Set the value of one GDT entry.
static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint16_t gran)
{
  uint32_t* entry = (uint32_t*)&gdt_entries[num];
  *entry = (limit & 0xffff) | ((base & 0xffff)<<16); //lower 32 bit
  *(entry+1) = (((base >> 16) & 0xFF)) | ((access & 0xff)<<8) | (((limit >> 16) & 0x0F)<<16) | ((gran & 0xf0)<<16) | (((base >> 24) & 0xFF)<<24);
}

static void init_idt()
{
  //The limit is 1 less than our table size because this is the end address
  uint16_t* ptr = idt_ptr;
  *ptr = ((sizeof(idt_entry_t) * 256) - 1);
  ++ptr;
  uint32_t* ptr2=ptr;
  *ptr2 = ((uint32_t)&idt_entries);
  //We need to make sure the interrupt values are nulled out, else BAD things may happen
  memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

  int num = 0;
  idtSetEntry(num++, (uint32_t)&divByZero_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&debug_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&NMI_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&breakpoint_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&overflow_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&outOfBounds_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&invalidInstr_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&noCoprocessor_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&doubleFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&coprocessor_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&badTSS_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&segmentNotPresent_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&stackFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&generalProtectionFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&pageFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&unknownInterrupt_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&coprocessorFault_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&alignmentCheck_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&machineCheck_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&reserved_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&PIT_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&PIT_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&cascade_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&COM2_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&COM1_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&LPT2_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&floppyDisk_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&LPT1_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&RTC_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&periph1_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&periph2_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&periph3_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&mouse_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&FPU_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&primaryHDD_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
  idtSetEntry(num++, (uint32_t)&secondaryHDD_handler, 0x08, makeFlagByte(1, KERNEL_MODE));


  idtSetEntry(50, (uint32_t)&switcher, 0x08, makeFlagByte(1, KERNEL_MODE));
  num = 0;

  lidt((void *)&idt_ptr);
}

static uint8_t makeFlagByte(int isPresent, int ring)
{
   return (((isPresent & 0x01) << 7) | ((ring & 0x3) << 5) | (0b1110)) & (~(1));
}



static void idtSetEntry(int num, uint32_t base, uint32_t sel, uint32_t flags)
{
  uint32_t* entry = (uint32_t*)&idt_entries[num];
  *entry = (base & 0xFFFF) | (sel << 16);
  *(entry+1) = 0x0000 | (flags << 8) | (((base >> 16) & 0xFFFF) << 16);
}
