#ifndef IOAPIC_h
#define IOAPIC_h

typedef struct __attribute__((packed)) IOAPIC_regs
{
  uint8_t IOregsel;
  uint8_t reserved1[15];
  uint32_t IOwin;
  uint32_t reserved[3];
}IOAPIC_regs_t;
/*
        Memory Mapped Registers for Accessing IOAPIC Registers
          3.1.1. IOREGSEL—I/O REGISTER SELECT REGISTER
          Memory Address: FEC0 xy00h (xy=See APICBASE Register in the PIIX3)
          Default Value: 00h
          Attribute: Read/Write
          This register selects the IOAPIC Register to be read/written. The data is then read from or written to the selected
          register through the IOWIN Register.
          Bit Description
          31:8 Reserved.
          7:0 APIC Register Address—R/W. Bits [7:0] specify the IOAPIC register to be read/written via the
          IOWIN Register.
          3.1.2. IOWIN—I/O WINDOW REGISTER
          Memory Address: FEC0 xy10h (xy=See APICBASE Register in PIIX3)
          Default Value: 00h
          Attribute: Read/Write
          This register is used to write to and read from the register selected by the IOREGSEL Register.
          Readability/writability is determined by the IOAPIC register that is currently selected.
          Bit Description
          31:0 APIC Register Data—R/W. Memory references to this register are mapped to the APIC register
          specified by the contents of the IOREGSEL Register.
*/

/*
        IOAPIC Registers
          Address Offset Mnemonic Register Name Access
          00h IOAPICID IOAPIC ID R/W
          01h IOAPICVER IOAPIC Version RO
          02h IOAPICARB IOAPIC Arbitration ID RO
          10-3Fh IOREDTBL[0:23] Redirection Table (Entries 0-23) (64 bits each) R/W
*/

#define IOAPIC_REG_ID     0x00  // Register index: ID
#define IOAPIC_REG_VER    0x01  // Register index: version
#define IOAPIC_REG_TABLE  0x10  // Redirection table base

#define IOAPIC_INT_DISABLED   0x00010000  // Interrupt disabled
#define IOAPIC_INT_LEVEL      0x00008000  // Level-triggered (vs edge-)
#define IOAPIC_INT_ACTIVELOW  0x00002000  // Active low (vs high)
#define IOAPIC_INT_LOGICAL    0x00000800  // Destination is CPU id (vs APIC ID)

#define IRQ_Base 0x20

void ioapic_set_irq(uint8_t irq, uint64_t apic_id, uint8_t vector);
void ioapic_write(uint32_t reg, uint32_t value);
uint32_t ioapic_read(uint32_t reg);

#endif
