#include "cpuid.h"
#include "apic.h"

const uint32_t CPUID_FLAG_MSR = 1 << 5;

bool cpuHasMSR()
{
   uint32_t a, b, c, d; // eax, edx
   cpuid(1, a, b, c, d);
   return d & CPUID_FLAG_MSR;
}

inline void cpu_get_msr(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
   asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

inline void cpu_set_msr(uint32_t msr, uint32_t lo, uint32_t hi)
{
   asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}
