#include "cpuid.h"
#include "apic.h"
#include "sys.h"

bool check_apic()
{
   uint32_t eax, ebx, ecx, edx;
   cpuid(1, eax, ebx, ecx, edx);
   return edx & CPUID_FLAG_APIC;
}

void enable_pic()
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void disable_pic()
{
    /* Set OCW1 (interrupt masks) */
    outb(0x21, 0xff);
    outb(0xa1, 0xff);
}

void init_APIC()
{/*
  disable_pic();
  //localapic_write_with_mask(LAPIC_SVR, (1<<8), (1<<8));
  printf("\nTesting APIC! Local APIC revision: %x Max LVT entry: %x\n",localapic_read(LAPIC_VER)&&0xff, ((localapic_read(LAPIC_VER)>>16) && 0xff)+1);
  localapic_write(LAPIC_ERROR, 0x1F); /// 0x1F: temporary vector (all other bits: 0)
  localapic_write(LAPIC_TPR, 0);

  localapic_write(LAPIC_DFR, 0xffffffff);
  localapic_write(LAPIC_LDR, 0x01000000);
  localapic_write(LAPIC_SVR, 0x100|0xff);
//*/
  //ioapic_set_irq(14, 0x0020, 14);
  //while(1);
  enable_pic();
}

uint32_t ioapic_read(uint32_t reg) //IO Apic
{
   uint32_t volatile *ioapic = (uint32_t volatile *)APIC_IO_BASE;
   ioapic[0] = (reg & 0xff);
   return ioapic[4];
}

void ioapic_write(uint32_t reg, uint32_t value) //IO Apic
{
   uint32_t volatile *ioapic = (uint32_t volatile *)APIC_IO_BASE;
   ioapic[0] = (reg & 0xff);
   ioapic[4] = value;
}

uint32_t localapic_read(uint32_t reg)
{
  uint32_t volatile *localapic = (uint32_t volatile *)(APIC_LOCAL_BASE+reg);
  return *localapic;
}

void localapic_write(uint32_t reg, uint32_t value)
{
  uint32_t volatile *localapic = (uint32_t volatile *)(APIC_LOCAL_BASE+reg);
  *localapic = value;
}

void localapic_write_with_mask(uint32_t reg, uint32_t mask, uint32_t value)
{
  uint32_t volatile *localapic = (uint32_t volatile *)(APIC_LOCAL_BASE+reg);
  *localapic &= ~mask;
  *localapic |= value;
}

void ioapic_set_irq(uint8_t irq, uint64_t apic_id, uint8_t vector)
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
