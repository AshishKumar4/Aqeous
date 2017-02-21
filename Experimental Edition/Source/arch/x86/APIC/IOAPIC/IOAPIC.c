#include <APIC/IOAPIC/IOAPIC.h>
#include <APIC.h>
#include <stdio.h>
#include <stdlib.h>

void Init_ioapic()
{

}

uint32_t ioapic_read(uint32_t reg) //IO Apic
{
   uint32_t volatile *ioapic = (uint32_t volatile *)APIC_IO_BASE;
   ioapic[0] = (reg & 0xff);
   return ioapic[4];
}

inline void ioapic_write(uint32_t reg, uint32_t value) //IO Apic
{
   uint32_t volatile *ioapic = (uint32_t volatile *)APIC_IO_BASE;
   ioapic[0] = (reg & 0xff);
   ioapic[4] = value;
}

inline void ioapic_set_irq(uint8_t irq, uint64_t apic_id, uint8_t vector)
{
  const uint32_t low_index = 0x10 + irq*2;
  const uint32_t high_index = 0x10 + irq*2 + 1;

  uint32_t high = ioapic_read(high_index);
  // set APIC ID
  high &= ~0xff000000;
  high |= apic_id << 24;
  ioapic_write(high_index, high);

  uint32_t low = ioapic_read(low_index);

  // unmask the IRQ
  low &= ~(1<<16);

  // set to physical delivery mode
  low &= ~(1<<11);

  // set to fixed delivery mode
  low &= ~0x700;

  // set delivery vector
  low &= ~0xff;
  low |= vector;

  ioapic_write(low_index, low);
}
