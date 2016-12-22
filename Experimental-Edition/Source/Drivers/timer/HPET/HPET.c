#include "hpet.h"
#include "stdio.h"
#include "string.h"
#include "apic.h"
#include "sys.h"

inline unsigned int hpet_readl(unsigned int a)
{
	return readl(hpet_virt_address + a);
}

static inline void hpet_writel(unsigned int d, unsigned int a)
{
	writel(d, hpet_virt_address + a);
}


void init_hpet()
{
  printf("\nInitializing HPET (High Precision Event Timer)!!!");
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
      if (acpiCheckHeader((uint32_t*)*ptr, "HPET") == 0)
      {
        //TODO: INITIALIZE HPET
        hpet_sdt = (HPET_descriptor_table_t*)*ptr;
        printf("\nHPET TABLE FOUND :D CONGRATULATIONS :D %s ", hpet_sdt->signature);
        hpet_base = (uint32_t*)hpet_sdt->Base_address[1];
        hpet_virt_address = (uint32_t)hpet_base;
        hpet = (HPET_Table_t*)hpet_base;
        hpet->Main_Counter_Reg = 0;
        hpet->GCReg = 3;
			//	printf("-%x \nMinimum Tick period: %x ",hpet->Main_Counter_Reg,hpet->GCIDReg[1]);
        //hpet->GCIDReg[1] = 10000;
      //  hpet->Timer0CCReg[0] = (1<<1);
      //  hpet->Timer0CVReg = 1000;
				/*for(int i = 0; i < 10000; i++)
				{
					printf("-%x",hpet->Main_Counter_Reg);
				}*/
				HPET_main_counter = &hpet->Main_Counter_Reg;
				break;
      }
      ptr++;
    }
  }
}
