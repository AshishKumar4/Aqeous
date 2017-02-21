#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "common.h"
#include "mathex.h"
#include "timer.h"
#include "hpet.h"
#include "APIC/LocalAPIC/lapic.h"

void Randomizer()
{
  seed1 = ReadFromCMOS() + 9;
  seed2 = (*HPET_main_counter + 1)*3;
  seed3 = Lapic->CurrentCount_Reg[0] + 999;
}

uint32_t random()
{
  uint32_t random = (seed2/seed3) + 1023*seed2;
  random *= (uint32_t)(seed1*seed1*524287);
  random /= 1023;
  random += ((seed2*(random%3))*(seed1*(random%2)))*(random%5);
  seed1 = (seed1 + seed2*seed3 + 1)%127 + (seed2%100) + 1;
  seed2 = (uint32_t)(seed2 + random*178481);
  seed3 = ((seed3 + seed2*seed1)/((seed1/57)+3))%8192 + seed1;  /*/
  seed1 = (seed2 % 100) + 1;
  seed2 = (uint32_t)(seed2*178481);
  seed3 = ((seed3 + random/seed1)%524287) + 1; //*/
  //printf("S1: %x S2: %x S3: %x\n", seed1, seed2, seed3);
  return random;
}
