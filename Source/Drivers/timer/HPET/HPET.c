#include "hpet.h"
#include "stdio.h"
#include "string.h"
#include "apic.h"

void init_hpet()
{
  printf("\nInitializing HPET (High Precision Event Timer)!!!");
  unsigned int *ptr = acpiGetRSDPtr();
  // check if address is correct  ( if acpi is available on this pc )
  if (ptr != NULL && acpiCheckHeader(ptr, "RSDT") == 0)
  {
    //TODO: Get the APIC and HPET tables!!!

  }
}
