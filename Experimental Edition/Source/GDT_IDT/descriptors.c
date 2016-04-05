#include <stdint.h>
#include <descriptors.h>
#include <sys.h>
#include <string.h>
extern void gdt_flush(u32int);
//extern void idt_flush(u32int);
extern void lidt(uint32_t *)
/**Sorry less comments, no time, will explain later**/
// Internal function prototypes.
static void init_gdt();
static void gdt_set_gate(int num,uint32_t base,uint32_t limit,uint8_t access,uint8_t gran);

static void init_idt();
static void idtSetEntry(int num, uint32_t base, uint16_t sel, uint8_t flags);
static void init_pic(uint8_t vector1, uint8_t vector2);
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
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base  = (u32int)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    gdt_flush((u32int)&gdt_ptr);
 }

// Set the value of one GDT entry.
static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
 {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
 }

void initPIC(uint8_t vector1, uint8_t vector2){
    uint8_t slavePIC_interrupt_mask = inb(SLAVE_PIC_DATA);
    uint8_t masterPIC_interrupt_mask = inb(MASTER_PIC_DATA);
    outb(MASTER_PIC, INIT_COMMAND);
    nop();
    outb(SLAVE_PIC, INIT_COMMAND);
    nop();
    outb(MASTER_PIC_DATA, vector1);
    nop();
    outb(SLAVE_PIC_DATA, vector2);
    nop();
    outb(MASTER_PIC_DATA, (1 << 2));
    nop();
    outb(SLAVE_PIC_DATA, 2);
    nop();
    outb(MASTER_PIC_DATA, ENVIRONMENT_AUTO);
    nop();
    outb(SLAVE_PIC_DATA, ENVIRONMENT_AUTO);
    nop();

    outb(MASTER_PIC_DATA, masterPIC_interrupt_mask);
    nop();
    outb(SLAVE_PIC_DATA, slavePIC_interrupt_mask);

static void init_idt()
{
  //The limit is 1 less than our table size because this is the end address
  idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
  idt_ptr.base  = (u32int)&idt_entries;
  //We need to make sure the interrupt values are nulled out, else BAD things may happen
  memset(&idt_entries, 0, sizeof(idt_entry_t)*256);
  int num = 0;
  /*
  idt_set_gate( 0, (u32int)isr0 , 0x08, 0x8E);
  idt_set_gate( 1, (u32int)isr1 , 0x08, 0x8E);
  idt_set_gate( 2, (u32int)isr2 , 0x08, 0x8E);
  idt_set_gate( 3, (u32int)isr3 , 0x08, 0x8E);
  idt_set_gate( 4, (u32int)isr4 , 0x08, 0x8E);
  idt_set_gate( 5, (u32int)isr5 , 0x08, 0x8E);
  idt_set_gate( 6, (u32int)isr6 , 0x08, 0x8E);
  idt_set_gate( 7, (u32int)isr7 , 0x08, 0x8E);
  idt_set_gate( 8, (u32int)isr8 , 0x08, 0x8E);
  idt_set_gate( 9, (u32int)isr9 , 0x08, 0x8E);
  idt_set_gate(10, (u32int)isr10, 0x08, 0x8E);
  idt_set_gate(11, (u32int)isr11, 0x08, 0x8E);
  idt_set_gate(12, (u32int)isr12, 0x08, 0x8E);
  idt_set_gate(13, (u32int)isr13, 0x08, 0x8E);
  idt_set_gate(14, (u32int)isr14, 0x08, 0x8E);
  idt_set_gate(15, (u32int)isr15, 0x08, 0x8E);
  idt_set_gate(16, (u32int)isr16, 0x08, 0x8E);
  idt_set_gate(17, (u32int)isr17, 0x08, 0x8E);
  idt_set_gate(18, (u32int)isr18, 0x08, 0x8E);
  idt_set_gate(19, (u32int)isr19, 0x08, 0x8E);
  idt_set_gate(20, (u32int)isr20, 0x08, 0x8E);
  idt_set_gate(21, (u32int)isr21, 0x08, 0x8E);
  idt_set_gate(22, (u32int)isr22, 0x08, 0x8E);
  idt_set_gate(23, (u32int)isr23, 0x08, 0x8E);
  idt_set_gate(24, (u32int)isr24, 0x08, 0x8E);
  idt_set_gate(25, (u32int)isr25, 0x08, 0x8E);
  idt_set_gate(26, (u32int)isr26, 0x08, 0x8E);
  idt_set_gate(27, (u32int)isr27, 0x08, 0x8E);
  idt_set_gate(28, (u32int)isr28, 0x08, 0x8E);
  idt_set_gate(29, (u32int)isr29, 0x08, 0x8E);
  idt_set_gate(30, (u32int)isr30, 0x08, 0x8E);
  idt_set_gate(31, (u32int)isr31, 0x08, 0x8E);
  idt_set_gate(32, (u32int)irq0, 0x08, 0x8E);
  idt_set_gate(33, (u32int)irq1, 0x08, 0x8E);
  idt_set_gate(34, (u32int)irq2, 0x08, 0x8E);
  idt_set_gate(35, (u32int)irq3, 0x08, 0x8E);
  idt_set_gate(36, (u32int)irq4, 0x08, 0x8E);
  idt_set_gate(37, (u32int)irq5, 0x08, 0x8E);
  idt_set_gate(38, (u32int)irq6, 0x08, 0x8E);
  idt_set_gate(39, (u32int)irq7, 0x08, 0x8E);
  idt_set_gate(40, (u32int)irq8, 0x08, 0x8E);
  idt_set_gate(41, (u32int)irq9, 0x08, 0x8E);
  idt_set_gate(42, (u32int)irq10, 0x08, 0x8E);
  idt_set_gate(43, (u32int)irq11, 0x08, 0x8E);
  idt_set_gate(44, (u32int)irq12, 0x08, 0x8E);
  idt_set_gate(45, (u32int)irq13, 0x08, 0x8E);
  idt_set_gate(46, (u32int)irq14, 0x08, 0x8E);
  idt_set_gate(47, (u32int)irq15, 0x08, 0x8E);
  */ 
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
  
  lidt();
}

static void idtSetEntry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
  idt_entries[num].base_lo = base & 0xFFFF;
  idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
  
  idt_entries[num].sel     = sel;
  idt_entries[num].always0 = 0;
  // We must uncomment the OR below when we get to using user-mode.
  // It sets the interrupt gate's privilege level to 3.
  idt_entries[num].flags   = flags /* | 0x60 */;
}
