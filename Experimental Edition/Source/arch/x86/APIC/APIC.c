#include "cpuid_c.h"
#include "apic.h"
#include "sys.h"
#include "pic.h"
#include "tasking.h"
#include "cpu/cpu.h"

#include "localapic/lapic.h"

bool check_apic()
{
   uint32_t eax, ebx, ecx, edx;
   cpuid(1, eax, ebx, ecx, edx);
   return edx & CPUID_FLAG_APIC;
}

uint32_t* cpu_get_apic_base()
{
   uint32_t eax, edx;
   cpuGetMSR(IA32_APIC_BASE_MSR, &eax, &edx);
  // cpuSetMSR((IA32_APIC_BASE_MSR)|(1<<11), &eax, &edx);

   return (eax & 0xfffff000);
}

void __attribute__((optimize("O0"))) BSP_init_LAPIC(uint32_t base)
{
//  clearIRQMask(0);
//  clearIRQMask(1);
  //localapic_write_with_mask(LAPIC_SVR, (1<<8), (1<<8));

//  printf("\n\nTesting APIC! Local APIC revision: %x Max LVT entry: %x\n",localapic_read(base, LAPIC_VER)&0xff, ((localapic_read(base, LAPIC_VER)>>16) & 0xff)+1);
  localapic_write(base, LAPIC_ERROR, 0x1F); /// 0x1F: temporary vector (all other bits: 0)
  localapic_write(base, LAPIC_TPR, 0);

  localapic_write(base, LAPIC_DFR, 0xffffffff);
  localapic_write(base, LAPIC_LDR, 0x01000000);
  localapic_write(base, LAPIC_SVR, 0x100|0xff);


//  uint32_t eax, edx;
//  cpuGetMSR(IA32_APIC_BASE_MSR, &eax, &edx);
//  cpuSetMSR((IA32_APIC_BASE_MSR)|(1<<11), &eax, &edx);

  Lapic = base;
  uint32_t version_lapic = Lapic->lapicVER_Reg[0];
  version_lapic &= 0xff;
}

void __attribute__((optimize("O0"))) AP_init_LAPIC()
{
  localapic_write(APIC_LOCAL_BASE, LAPIC_ERROR, 0x1F); /// 0x1F: temporary vector (all other bits: 0)
  localapic_write(APIC_LOCAL_BASE, LAPIC_TPR, 0);

  localapic_write(APIC_LOCAL_BASE, LAPIC_DFR, 0xffffffff);
  localapic_write(APIC_LOCAL_BASE, LAPIC_LDR, 0x01000000);
  localapic_write(APIC_LOCAL_BASE, LAPIC_SVR, 0x100|0xff);
}

void MADTapic_parse()
{
  printf("\nInitializing IO APIC!!!");
  uint_fast32_t *ptr = (uint_fast32_t*)acpiGetRSDPtr();
  // check if address is correct  ( if acpi is available on this pc )
  if (ptr != NULL && acpiCheckHeader((uint32_t*)ptr, "RSDT") == 0)
  {
    //Get the APIC and HPET tables!!!
    struct RSDT* rsdt = (struct RSDT*)ptr;
    uint_fast32_t entries = rsdt->h.length;
    entries = (entries - sizeof(struct ACPI_SDTHeader))/4;
    ptr = (uint_fast32_t*)&rsdt->PointerToOtherSDT;

    for(uint32_t i=0; i<entries; i++)
    {
      if (acpiCheckHeader((uint32_t*)*ptr, "APIC") == 0)
      {
        printf("\nGot the MADT Structure");
        madt = (MADT_t*)*ptr;
        madt_entry_t* madt_entry = (madt_entry_t*)&madt->rest_fields;
        for(int i=0;!i;)
        {
          switch(madt_entry->type)
          {
            case 0 :
              ++total_CPU_Cores;
              printf("\n\tLocal APIC Found");
              LAPIC_entry = (lapic_entry_t*)&madt_entry->rest_field;
              madt_entry = (madt_entry_t*)LAPIC_entry->rest_fields;
              printf("\t\t\t%gLAPIC USABILITY: %x%g", 10, LAPIC_entry->flags & (1<<0),0);
            //  ++total_CPU_Cores;
              break;
            case 1 :
              printf("\n\tIO APIC Found");
              IOAPIC_entry = (ioapic_entry_t*)&madt_entry->rest_field;
              printf(" id: %x, address: %x, GSIB: %x",IOAPIC_entry->id, IOAPIC_entry->address, IOAPIC_entry->gsib);
              APIC_IO_BASE = IOAPIC_entry->address;
              madt_entry = (madt_entry_t*)IOAPIC_entry->rest_fields;
            //  while(1);
              break;
            case 2 :
              printf("\n\tInterrupt Source Override Found");
              ISD_entry = (isd_entry_t*)&madt_entry->rest_field;
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

inline void ioapic_write(uint32_t reg, uint32_t value) //IO Apic
{
   uint32_t volatile *ioapic = (uint32_t volatile *)APIC_IO_BASE;
   ioapic[0] = (reg & 0xff);
   ioapic[4] = value;
}

uint32_t __attribute__((optimize("O0"))) localapic_read(uint32_t base, uint32_t reg)
{
  uint32_t volatile *localapic = (uint32_t volatile *)(base+reg);
  return *localapic;
}

void __attribute__((optimize("O0"))) localapic_write(uint32_t base, uint32_t reg, uint32_t value)
{
  uint32_t volatile *localapic = (uint32_t volatile *)(base+reg);
  *localapic = value;
}

inline void localapic_write_with_mask(uint32_t base, uint32_t reg, uint32_t mask, uint32_t value)
{
  uint32_t volatile *localapic = (uint32_t volatile *)(APIC_LOCAL_BASE+reg);
  *localapic &= ~mask;
  *localapic |= value;
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
