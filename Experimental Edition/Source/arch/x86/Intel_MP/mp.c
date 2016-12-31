#include "mp.h"
#include "stdio.h"
#include "common.h"
#include "stdlib.h"
#include "string.h"

// checks for a given header and validates checksum
int MPCheckHeader(uint32_t *ptr, char *sig)
{
   if (memcmp(ptr, sig, 4) == 0)
   {
      char *checkPtr = (char *) ptr;
      uint8_t len = *((uint8_t*)(ptr+2));
      len *= 16;
      char check = 0;
      while (0<len--)
      {
         check += *checkPtr;
         checkPtr++;
      }
      if (check == 0)
         return 0;

   }
   return -1;
}

void MPtables_parse()
{
  printf("\nParsing Multi Processing Specification table by Intel!!!\n");
  uint_fast32_t *ptr;
  int found = 0;
  for(ptr = 0x000E0000; ptr <= 0x00100000; ptr++)
  {
    if(MPCheckHeader((uint32_t*)ptr, "_MP_") == 0)
    {
      found = 1;
      break;
    }
  }
  if(!found)
  {
    // at address 0x40:0x0E is the RM segment of the ebda
    int ebda = *((short *) 0x40E);   // get pointer
    ebda = ebda*0x10 &0x000FFFFF;   // transform segment into linear address

    // search Extended BIOS Data Area for the Root System Description Pointer signature
    for (ptr = (uint32_t *) ebda; (int) ptr<ebda+1024; ptr+= 0x10/4)
    {
      if(MPCheckHeader((uint32_t*)ptr, "_MP_") == 0)
      {
        found = 1;
        break;
      }
    }
  }
  if(!found)
  {
    for(ptr = 0xFFFFEFFF; ptr < 0xFFFFFFFF; ptr++)
    {
      if(MPCheckHeader((uint32_t*)ptr, "_MP_") == 0)
      {
        found = 1;
        break;
      }
    }
  }


  if(!found)
  {
    printf("\n%gMP Tables NOT FOUND!!! %x%g",9,ptr,0);
    return;
  }

  printf("\n%gMP Tables FOUND!!! Address: %x%g",10, ptr, 0);

  mpfp = (MP_FloatingPointer_t*)ptr;
  mpcth = (MP_ConfigTbl_Header_t*)mpfp->ConfigTable_Ptr;
  uint8_t* entries = &mpcth->reserved;
  ++entries;
  printf("\n\t\t%gLAPIC Base address: %x%g",5,mpcth->LAPIC_addr,0);

//  printf("\n%x %x",sizeof(MP_ConfigTbl_Header_t), sizeof(ProcessorEntry_t));

  Processor_Entries = entries;
  uint32_t processors = 0;
  //uint32_t* ptr = Processor_Entries;
  for(int i=0;i<mpcth->entry_count;i++)
  {
    switch(*entries)
    {
      case 0 :
      //  printf("\n\t%gProcessor #%x Found, LAPIC ID #%x, LAPIC Version #%x%g",6,processors,pr->id, pr->version,0);

        printf("\n\t%gProcessor #%x Found, LAPIC ID #%x, LAPIC Version #%x%g",7,processors,Processor_Entries[processors].id, Processor_Entries[processors].version,0);
        entries += 20;
        break;
      case 1 :
        entries += 8;
        break;
      case 2 :
        entries += 8;
        break;
      case 3 :
        entries += 8;
        break;
      case 4 :
        entries += 8;
        break;
      default:
      //  while(1);
      //  printf("\n\tUnknown entry type");
        break;
    }
  }
}
