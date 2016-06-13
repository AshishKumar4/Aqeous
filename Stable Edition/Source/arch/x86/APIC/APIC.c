#include "cpuid.h"
#include "apic.h"
#include "sys.h"
#include "pic.h"

bool check_apic()
{
   uint32_t eax, ebx, ecx, edx;
   cpuid(1, eax, ebx, ecx, edx);
   return edx & CPUID_FLAG_APIC;
}

void init_APIC()
{
  enable_pic();
  disable_pic();
//  clearIRQMask(0);
//  clearIRQMask(1);
  //localapic_write_with_mask(LAPIC_SVR, (1<<8), (1<<8));
/*
  printf("\nTesting APIC! Local APIC revision: %x Max LVT entry: %x\n",localapic_read(LAPIC_VER)&&0xff, ((localapic_read(LAPIC_VER)>>16) && 0xff)+1);
  localapic_write(LAPIC_ERROR, 0x1F); /// 0x1F: temporary vector (all other bits: 0)
  localapic_write(LAPIC_TPR, 0);

  localapic_write(LAPIC_DFR, 0xffffffff);
  localapic_write(LAPIC_LDR, 0x01000000);
  localapic_write(LAPIC_SVR, 0x100|0xff);
//*/
//  ioapic_set_irq(33, 0x0020, 33);
//  ioapic_set_irq(1, 0x0020, 1);
  //while(1);
}

void ioapic_init()
{
  printf("\nInitializing IO APIC!!!");
  uint_fast32_t *ptr = acpiGetRSDPtr();
  // check if address is correct  ( if acpi is available on this pc )
  if (ptr != NULL && acpiCheckHeader(ptr, "RSDT") == 0)
  {
    //Get the APIC and HPET tables!!!
    struct RSDT* rsdt = (struct RSDT*)ptr;
    uint_fast32_t entries = rsdt->h.length;
    entries = (entries - sizeof(struct ACPI_SDTHeader))/4;
    ptr = (uint_fast32_t*)&rsdt->PointerToOtherSDT;

    for(uint32_t i=0; i<entries; i++)
    {
      if (acpiCheckHeader(*ptr, "APIC") == 0)
      {
        printf("\nGot the MADT Structure");
        madt = *ptr;
        madt_entry_t* madt_entry = (madt_entry_t*)&madt->rest_fields;
        for(int i=0;!i;)
        {
          switch(madt_entry->type)
          {
            case 0 :
              printf("\n\tLocal APIC Found");
              LAPIC_entry = &madt_entry->rest_field;
              madt_entry = (madt_entry_t*)LAPIC_entry->rest_fields;
              break;
            case 1 :
              printf("\n\tIO APIC Found");
              IOAPIC_entry = &madt_entry->rest_field;
              printf(" id: %x, address: %x, GSIB: %x",IOAPIC_entry->id, IOAPIC_entry->address, IOAPIC_entry->gsib);
              APIC_IO_BASE = IOAPIC_entry->address;
              madt_entry = (madt_entry_t*)IOAPIC_entry->rest_fields;
              break;
            case 2 :
              printf("\n\tInterrupt Source Override Found");
              ISD_entry = &madt_entry->rest_field;
              madt_entry = (madt_entry_t*)ISD_entry->rest_fields;
              break;
            default:
            //  while(1);
            //  printf(" %x ", madt_entry->type);
            //  printf("\n\tUnknown entry type");
              i=1;
              break;
          }
        }
      }
      ptr++;
    }
  }
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
