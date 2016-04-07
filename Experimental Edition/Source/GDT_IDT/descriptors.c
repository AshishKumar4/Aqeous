#include <stdint.h>
#include <descriptors.h>
#include <sys.h>
#include <string.h>


extern void lgdt(void *);
extern void lidt(void *);


//TODO: More optimizations
// Internal function prototypes.
static void init_gdt();
static void gdt_set_gate(int num,uint32_t base,uint32_t limit,uint8_t access,uint8_t gran);
static void init_idt();
static void idtSetEntry(int num, uint32_t base, uint16_t sel, uint8_t flags);
static void init_pic(uint8_t vector1, uint8_t vector2);
static uint8_t makeFlagByte();



gdt_entry_t gdt_entries[10];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;


//Defines
#define KERNEL_MODE 0b00
#define USER_MODE 0b11
#define MASTER_PIC 0x20
#define MASTER_PIC_DATA 0x21
#define SLAVE_PIC 0xA0
#define SLAVE_PIC_DATA 0xA1
#define INIT_COMMAND 0x11
#define ENVIRONMENT_AUTO 0x02


// Initialisation routine - zeroes all the interrupt service routines,
// initialises the GDT and IDT.
void init_descriptor_tables()
 {
    // Initialise the global descriptor table.
    init_gdt();
    init_idt();
 }



static void init_gdt()
 {
   *(gdt_ptr) = ((sizeof(gdt_entry_t) * 5) - 1);
   *(gdt_ptr + 1)  = ((((uint32_t)&gdt_entries) & (0xFFFF0000)) >> 16);
   *(gdt_ptr + 1) = ((uint32_t)&gdt_entries) & (0x0000FFFF);

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    lgdt((void *)&gdt_ptr);
 }



// Set the value of one GDT entry.
static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
 {
   /*gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;*/
   //Basically a shorter and much more faster version of the above piece of code
   (gdt_entries[num]) = ((limit & 0xFFFF) << 48) | ((base & 0xFFFF) << 32) | ((base & 0xFF0000) << 8) | (access << 16) | (((gran & 0xF0) | ((limit >> 16) & 0x0F)) << 8) | ((base & 0xFF000000) >>24);
   
 }



void initPIC(uint8_t vector1, uint8_t vector2){
    uint8_t slavePIC_interrupt_mask = inb(SLAVE_PIC_DATA);
    uint8_t masterPIC_interrupt_mask = inb(MASTER_PIC_DATA);
    //Send the initialization commands to the Master and Slave Pic
    outb(MASTER_PIC, INIT_COMMAND);
    nop();//nop skips over an instruction cycle, leaving a bit of time for the comparatively slower PIC to update it's registers
    outb(SLAVE_PIC, INIT_COMMAND);
    nop();
    //Send the vectors of the PIC IRQs to the PICs
    outb(MASTER_PIC_DATA, vector1);
    nop();
    outb(SLAVE_PIC_DATA, vector2);
    nop();
    //Telling the master on which line (and therefore bit) the slave PIC is connected
    outb(MASTER_PIC_DATA, (1 << 2));
    nop();
    //Telling the slave the above informatiin
    outb(SLAVE_PIC_DATA, 2);
    nop();
    //A little bit of information about the environment
    outb(MASTER_PIC_DATA, ENVIRONMENT_AUTO);
    nop();
    outb(SLAVE_PIC_DATA, ENVIRONMENT_AUTO);
    nop();
    //Restoring the interrupt mask
    outb(MASTER_PIC_DATA, masterPIC_interrupt_mask);
    nop();
    outb(SLAVE_PIC_DATA, slavePIC_interrupt_mask);
}


static void init_idt()
{
  //The limit is 1 less than our table size because this is the end address
  *(idt_ptr) = sizeof(idt_entry_t) * 256 -1;
  *(idt_ptr + 1)  = (((uint32_t)&idt_entries & 0xFFFF0000) >> 16);
  *(idt_ptr + 2) = ((uint32_t)&idt_entries & 0x0000FFFF);
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
  idtSetEntry(num++, (uint32_t)&keyboardInterrupt_handler, 0x08, makeFlagByte(1, KERNEL_MODE));
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
  num = 0;
  
  lidt((void *)&idt_ptr);
}

static uint8_t makeFlagByte(int isPresent, int ring){
   return (((isPresent & 0x01) << 7) | ((ring & 0x3) << 5) | (0b1110)) & (~(1));
}



static void idtSetEntry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
  /*  idt_entries[num].base_lo = base & 0xFFFF;
  idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
  
  idt_entries[num].sel     = sel;
  idt_entries[num].always0 = 0;
  // We must uncomment the OR below when we get to using user-mode.
  // It sets the interrupt gate's privilege level to 3.
  idt_entries[num].flags   = flags /* | 0x60 */
  idt_entries[num] = (((base & 0x0000FFFF) << 48) | (sel << 32) | (flags << 16) | ((base & 0xFFFF0000) >> 16)) & (~(0xFF << 24));
}
