#include "APIC/IOAPIC/ioapic.h"
#include "APIC/apic.h"
#include <stdio.h>
#include <stdlib.h>

void Init_ioapic()
{
  uint32_t maxintr = (ioapic_read(IOAPIC_REG_VER) >> 16) & 0xFF;
  for(uint32_t i = 0; i <= maxintr; i++)
  {
    ioapic_write(IOAPIC_REG_TABLE+2*i, IOAPIC_INT_DISABLED | (0x20 + i));
    ioapic_write(IOAPIC_REG_TABLE+2*i+1, 0);
  }
/*  for(int i = 0; i < maxintr; i++)
  {
    ioapic_set_irq(i, 0, IRQ_Base + i);
  }
*/
  ioapic_set_irq(0x12, 0, 50);
  ioapic_set_irq(0x13, 0, 51);
  ioapic_set_irq(0x14, 0, 52);
  asm volatile("sti");
}

void ioapic_enableIRQ(int irq, int cpunum)
{
  // Mark interrupt edge-triggered, active high,
  // enabled, and routed to the given cpunum,
  // which happens to be that cpu's APIC ID.
  ioapic_write(IOAPIC_REG_TABLE+2*irq, 0 + irq);
  ioapic_write(IOAPIC_REG_TABLE+2*irq+1, cpunum << 24);
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
