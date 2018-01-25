#include "string.h"
#include "stdio.h"
#include "common.h"
#include "stdlib.h"

int ByteSequence_Replace(uint32_t magic, uint32_t nbytes, uint32_t replacement, uint32_t mbytes, uint32_t* start, uint32_t* end)
{
  uint32_t m = magic;
  uint8_t* tmp = (uint8_t*)&m;
  uint32_t r = replacement;
  uint8_t* tmp2 = (uint8_t*)&r;
  uint8_t* i = (uint8_t*)start;
  uint32_t j = 0;
  uint32_t c = 0;
  if(end-nbytes < start) return -1;
  for(; i<=((uint8_t*)end)-nbytes; i++)
  {
    for(; j<nbytes; j++)
    {
      if(!(*(i+j) == tmp[j])) //Sequence dosent match
      {
        goto out;
      }
    }
    memcpy(i,tmp2,mbytes); //Sequence Matched; Replace it.
    ++c;
    out:
    j = 0;
  }
  return c;
}
