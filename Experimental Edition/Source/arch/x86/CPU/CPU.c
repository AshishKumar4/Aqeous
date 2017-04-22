AP_stack_start#include "cpuid_c.h"
#include "apic.h"
#include "cpu/cpu.h"
#include "LocalAPIC/lapic.h"

#include "timer.h"
#include "Intel_MP/mp.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "system.h"
#include "common.h"
#include "sys.h"
#include "descriptors.h"

bool cpuHasMSR()
{
   uint32_t a, b, c, d; // eax, edx
   cpuid(1, a, b, c, d);
   return d & CPUID_FLAG_MSR;
}

void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
   asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi)
{
   asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

static inline uint64_t rdtsc()
{
    uint64_t ret;
    asm volatile ( "rdtsc" : "=A"(ret) );
    return ret;
}

static inline void invlpg(void* m)
{
    /* Clobber memory to avoid optimizer re-ordering access before invlpg, which may cause nasty bugs. */
    asm volatile ( "invlpg (%0)" : : "b"(m) : "memory" );
}

uint32_t AP_stacks = 8192;

void BasicCPU_Init()
{
  enable_pic();
  disable_pic();
  APIC_LOCAL_BASE = (uint32_t)cpu_get_apic_base();
  BSP_init_LAPIC(APIC_LOCAL_BASE);        // BSP LAPIC Initialize
	MPtables_parse();
	MADTapic_parse();
  BSP_id = Lapic->lapicID_Reg[0]&&0xff;
  asm volatile("sti");
  if(!total_CPU_Cores)
    total_CPU_Cores = 1;
  BootAPs();
}

extern void test_entry();

void __attribute__((optimize("O0"))) BootAPs()
{
  memset((void*)0x1F000, 0, 4096);
  memcpy((void*)0x1F000, APIC_Error_vector, APIC_Error_vector_end - APIC_Error_vector);
  ByteSequence_Replace(0x6679, 2, 0x0500, 2, (uint32_t*)APIC_Error_vector, (uint32_t*)APIC_Error_vector_end);

  // Boot the AP's
  *(char*)(0xf) = 0xa;

  uint32_t vector = 0;
  AP_startup_Code_sz = AP_startup_Code_end - AP_startup_Code;
  AP_startup_Code_sz = ((AP_startup_Code_sz/4) + 1)*4;

  uint32_t pmode_code_addr;
  pmode_code_size = pmode_AP_code_end - pmode_AP_code;
  pmode_code_size = ((pmode_code_size/4) + 1)*4;

//  uint32_t APIC_error_vector = 0x1F000;

  uint32_t* test_counter = (uint32_t*)0x00000500;
  *test_counter = 0;

  uint32_t* semaphore = (uint32_t*)0x00000504;
  *semaphore = 0;
  vector = 1;
  uint32_t vector_addr = 4096;
  for(uint32_t i = AP_stacks,j = 0; j<(total_CPU_Cores - 1)%CORES_BOOT_SINGLE_SQ; i+=8192, j++)
  {
    if(BSP_id == j) continue;
    vector_addr = (i - 4096);
    memset((void*)vector_addr, 0, 4096);
    memcpy((void*)vector_addr, AP_startup_Code, AP_startup_Code_sz);   // Copy the code of switching to protected mode into the bottom of stacks for each AP

    ByteSequence_Replace(0x4284, 2, (i+4092), 2, (uint32_t*)vector_addr, (uint32_t*)(vector_addr+AP_startup_Code_sz));

    uint32_t* gdt_new = (uint32_t*)(vector_addr + AP_startup_Code_sz + pmode_code_size + 16);
    AP_gdt_Setup((uint32_t*)(vector_addr + AP_startup_Code_sz + pmode_code_size + 8 + 16), gdt_new);

    ByteSequence_Replace(0x3240, 2, (uint32_t)gdt_new, 2, (uint32_t*)vector_addr, (uint32_t*)(vector_addr+AP_startup_Code_sz));

    uint32_t* idt_new = (uint32_t*)(vector_addr + AP_startup_Code_sz + pmode_code_size + 16 + 48);
    AP_idt_Setup((uint32_t*)(vector_addr + AP_startup_Code_sz + pmode_code_size + 8 + 16 + 48), idt_new);

    ByteSequence_Replace(0x3250, 2, (uint32_t)idt_new, 2, (uint32_t*)vector_addr, (uint32_t*)(vector_addr+AP_startup_Code_sz));

    pmode_code_addr = vector_addr + AP_startup_Code_sz + 16;
    memcpy((void*)pmode_code_addr , pmode_AP_code, pmode_code_size);   // Copy the code of switching to protected mode into the bottom of stacks for each AP

    *(uint32_t*)(vector_addr + AP_startup_Code_sz + 8) = 0;
    ByteSequence_Replace(0x5599, 2, vector_addr + AP_startup_Code_sz + 8, 2, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));
    ByteSequence_Replace(0x4959, 2, pmode_code_addr, 2, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));

    vector = ((vector_addr)/4096);
    //memset_faster(APIC_LOCAL_BASE, 0xffffffff, 4096);

    localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (j<<24));        // INIT IPI
    localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004500);        // INIT IPI

  //  Lapic->ICR[0][0] = 0x000C4500;
    delay1(1);
    localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (j<<24));        // INIT IPI
    localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004600 | vector);     // SIPI IP
  //  Lapic->ICR[0][0] = 0x000C4600 + vector;
    delay1(4);
    if((*test_counter)*2 == i)
    {
    //  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (j<<24));        // INIT IPI
      printf("\nReatempting INIT SIPI");
      localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004600 | vector);     // 2nd SIPI IP
    }
  //  Lapic->ICR[0][0] = 0x000C4600 + vector;
  //  func_t f = vector*4096;
  //  f();

    printf("\n\t\t%gThe AP #%x has been Booted Successfully %x Error %x Vector %x %x %g",10,j,*test_counter,Lapic->ICR[0][0] & (1<<12), *(uint32_t*)(0x50001000), (vector_addr + AP_startup_Code_sz + 8), 0);
  }
  delay1(4);
//  while(1);
}

void __attribute__((optimize("O0"))) BootAP(uint32_t ap_ID)
{
  // Boot the AP's
  *(char*)(0xf) = 0xa;

  uint32_t AP_start_s = ap_ID*4096;
  AP_start_s += 4096;

  uint32_t vector = 0;
  AP_startup_Code_sz = AP_startup_Code_end - AP_startup_Code;
  AP_startup_Code_sz = ((AP_startup_Code_sz/4) + 1)*4;

  uint32_t pmode_code_addr;
  pmode_code_size = pmode_AP_code_end - pmode_AP_code;
  pmode_code_size = ((pmode_code_size/4) + 1)*4;

  uint32_t* test_counter = (uint32_t*)0x00000500;
  *test_counter = 0;

  uint32_t* semaphore = (uint32_t*)0x00000504;
  *semaphore = 0;
  vector = 1;
  uint32_t vector_addr = 4096;

  if(BSP_id == ap_ID) return;
  vector_addr = (AP_stack_s - 4096);
  memset((void*)vector_addr, 0, 4096);
  memcpy((void*)vector_addr, AP_startup_Code, AP_startup_Code_sz);   // Copy the code of switching to protected mode into the bottom of stacks for each AP

  ByteSequence_Replace(0x4284, 2, (AP_stack_s+4092), 2, (uint32_t*)vector_addr, (uint32_t*)(vector_addr+AP_startup_Code_sz));

  uint32_t* gdt_new = (uint32_t*)(vector_addr + AP_startup_Code_sz + pmode_code_size + 16);
  AP_gdt_Setup((uint32_t*)(vector_addr + AP_startup_Code_sz + pmode_code_size + 8 + 16), gdt_new);

  ByteSequence_Replace(0x3240, 2, (uint32_t)gdt_new, 2, (uint32_t*)vector_addr, (uint32_t*)(vector_addr+AP_startup_Code_sz));

  uint32_t* idt_new = (uint32_t*)(vector_addr + AP_startup_Code_sz + pmode_code_size + 16 + 48);
  AP_idt_Setup((uint32_t*)(vector_addr + AP_startup_Code_sz + pmode_code_size + 8 + 16 + 48), idt_new);

  ByteSequence_Replace(0x3250, 2, (uint32_t)idt_new, 2, (uint32_t*)vector_addr, (uint32_t*)(vector_addr+AP_startup_Code_sz));

  pmode_code_addr = vector_addr + AP_startup_Code_sz + 16;
  memcpy((void*)pmode_code_addr , pmode_AP_code, pmode_code_size);   // Copy the code of switching to protected mode into the bottom of stacks for each AP

  *(uint32_t*)(vector_addr + AP_startup_Code_sz + 8) = 0;
  ByteSequence_Replace(0x5599, 2, vector_addr + AP_startup_Code_sz + 8, 2, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));
  ByteSequence_Replace(0x4959, 2, pmode_code_addr, 2, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));

  vector = ((vector_addr)/4096);
  //memset_faster(APIC_LOCAL_BASE, 0xffffffff, 4096);

  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (ap_ID<<24));        // INIT IPI
  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004500);        // INIT IPI

//  Lapic->ICR[0][0] = 0x000C4500;
  delay1(1);
  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (ap_ID<<24));        // INIT IPI
  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004600 | vector);     // SIPI IP
//  Lapic->ICR[0][0] = 0x000C4600 + vector;
  delay1(4);
  if((*test_counter)*2 == AP_stack_start)
  {
  //  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (j<<24));        // INIT IPI
    printf("\nReatempting INIT SIPI");
    localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004600 | vector);     // 2nd SIPI IP
  }
//  Lapic->ICR[0][0] = 0x000C4600 + vector;
//  func_t f = vector*4096;
//  f();

  printf("\n\t\t%gThe AP #%x has been Booted Successfully %x Error %x Vector %x %x %g",10,ap_ID,*test_counter,Lapic->ICR[0][0] & (1<<12), *(uint32_t*)(0x50001000), (vector_addr + AP_startup_Code_sz + 8), 0);

  delay1(4);
//  while(1);
}
