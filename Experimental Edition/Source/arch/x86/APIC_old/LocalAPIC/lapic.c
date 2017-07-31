#include "lapic.h"

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
