#include "cpuid_c.h"
#include "apic.h"
#include "cpu/cpu.h"
#include "LocalAPIC/lapic.h"

#include "timer.h"
#include "HPET/HPET.h"
#include "Intel_MP/mp.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "system.h"
#include "common.h"
#include "sys.h"
#include "descriptors.h"
#include "Scheduler/scheduler.h"

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
	//MPtables_parse();
	MADTapic_parse();
  BSP_id = Lapic->lapicID_Reg[0]&&0xff;
  asm volatile("sti");
  if(!total_CPU_Cores)
    total_CPU_Cores = 1;
  printf("\nTotal CPU Cores: %d", total_CPU_Cores);
  tss_entries = (tss_struct_t**)desc_malloc(sizeof(tss_struct_t*)*(total_CPU_Cores));
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

  uint32_t* test_counter = (uint32_t*)0x00001000;
  *test_counter = 0;

  uint32_t* semaphore = (uint32_t*)0x00000504;
  *semaphore = 0;
  vector = 1;
  uint32_t vector_addr = 4096;

  CPU_BOOT_MODE = 0;

  for(uint32_t i = AP_stacks,j = 0; j<total_CPU_Cores; i+=8192, j++)
  {
    if(BSP_id == j) continue;
    vector_addr = i - 4096;
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
    delay1(2);
    if((*test_counter)*2 == i)
    {
      localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (j<<24));        // INIT IPI
      printf("\nReatempting INIT SIPI");
      localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004600 | vector);     // 2nd SIPI IP
    }
  //  Lapic->ICR[0][0] = 0x000C4600 + vector;
  //  func_t f = vector*4096;
  //  f();
    if((*test_counter)*2 != i)
      printf("\n\t\t%gThe AP #%x has been Booted Successfully %x %g",10,j,*test_counter, 0);
  }
  delay1(2);
//  while(1);
}

extern void Spawner_Task();

void __attribute__((optimize("O0"))) Boot_AP(uint32_t APIC_id)  // Basically Reboots the APs
{
  asm volatile("cli");

  SchedulerKits_t* kit = &KitList[APIC_id];
  // Boot the AP's
  *(char*)(0xf) = 0xa;

  uint32_t vector = 0;

  uint32_t pmode_code_addr;

  uint32_t vector_addr = 0x5000;

  uint32_t* test_counter = (uint32_t*)0x00001000;
  *test_counter = 0;

  uint32_t* semaphore = (uint32_t*)0x00000504;
  *semaphore = 0;

  uint32_t AP_tmp = vector_addr;

  vector_addr = AP_tmp - 4096;

  memset((void*)vector_addr, 0, 4096);
  memcpy((void*)vector_addr, AP_startup_Code, AP_startup_Code_sz);   // Copy the code of switching to protected mode into the bottom of stacks for each AP

  CPU_BOOT_MODE = 0x1111;

  ByteSequence_Replace(0x4284, 2, (AP_tmp+4092), 2, (uint32_t*)vector_addr, (uint32_t*)(vector_addr+AP_startup_Code_sz));

  uint32_t* gdt_new = (uint32_t*)(vector_addr + AP_startup_Code_sz + pmode_code_size + 16);
  AP_gdt_Setup((uint32_t*)(vector_addr + AP_startup_Code_sz + pmode_code_size + 8 + 16), gdt_new);

  ByteSequence_Replace(0x3240, 2, (uint32_t)gdt_new, 2, (uint32_t*)vector_addr, (uint32_t*)(vector_addr+AP_startup_Code_sz));

  uint32_t* idt_new = (uint32_t*)(vector_addr + AP_startup_Code_sz + pmode_code_size + 16 + 48);
  AP_idt_Setup((uint32_t*)(vector_addr + AP_startup_Code_sz + pmode_code_size + 8 + 16 + 48), idt_new);

  ByteSequence_Replace(0x3250, 2, (uint32_t)idt_new, 2, (uint32_t*)vector_addr, (uint32_t*)(vector_addr+AP_startup_Code_sz));

  pmode_code_addr = vector_addr + AP_startup_Code_sz + 16;
  printf("\n{%x}", pmode_code_addr);
  memcpy((void*)pmode_code_addr , pmode_AP_code, pmode_code_size);   // Copy the code of switching to protected mode into the bottom of stacks for each AP

  *(uint32_t*)(vector_addr + AP_startup_Code_sz + 8) = 0;
  ByteSequence_Replace(0x5599, 2, vector_addr + AP_startup_Code_sz + 8, 2, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));
  ByteSequence_Replace(0x4959, 2, pmode_code_addr, 2, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));

  // Setup Permanent GDT, IDT
  ByteSequence_Replace(0x32409798, 4, (uint32_t)kit->gdt, 4, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));
  ByteSequence_Replace(0x32409799, 4, (uint32_t)kit->idt, 4, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));
  ByteSequence_Replace(0x42842222, 4, (uint32_t)&Spawner_Task, 4, (uint32_t*)pmode_code_addr, (uint32_t*)(pmode_code_addr + pmode_code_size));

  vector = ((vector_addr)/4096);

  printf("\nSending INIT IPI...");
  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (APIC_id<<24));
  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004500);        // INIT IPI
  delay_hpet(10);     // 10 ms
  printf("\nSending INIT SIPI...");
  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (APIC_id<<24));
  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004600 | vector);     // SIPI IP]
  delay_hpet(200);    // 200 ms
  if(!(*test_counter))
  {
    localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (APIC_id<<24));
    printf("\nReatempting INIT SIPI");
    localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004600 | vector);     // 2nd SIPI IP
  }

  pmode_GDT_lgdt_Vec(vector_addr, KitList[APIC_id].gdt);
  pmode_IDT_lidt_Vec(vector_addr, KitList[APIC_id].idt);

  *(uint32_t*)(vector_addr + AP_startup_Code_sz + 8) = 0x4284;
  //while(*(uint32_t*)(vector_addr + AP_startup_Code_sz + 8) != 0x33);

  *(uint32_t*)(vector_addr + AP_startup_Code_sz + 8) = 0x3240;

  if(*test_counter)
    printf("\n\t\t%gThe AP #%x has been Booted Successfully %x %g",10,APIC_id,*test_counter, 0);
  
  asm volatile("sti");
}

void __attribute__((optimize("O0"))) AP_RealMode_Setup(uint32_t APIC_id)
{
  asm volatile("cli");
  // Boot the AP's
  *(char*)(0xf) = 0xa;

  uint32_t vector = 0;

 // uint32_t rmode_code_addr;
  rmode_code_size = rmode_AP_code_end - rmode_AP_code;

  uint32_t vector_addr = 0x5000;

  uint32_t* test_counter = (uint32_t*)0x00001000;
  *test_counter = 0;

  uint32_t* semaphore = (uint32_t*)0x00000504;
  *semaphore = 0;

  uint32_t AP_tmp = vector_addr;

  vector_addr = AP_tmp - 4096;

  CPU_BOOT_MODE = 0x1111;

  memset((void*)vector_addr, 0, 4096);
  memcpy((void*)vector_addr, rmode_AP_code, rmode_code_size);   // Copy the code of switching to protected mode into the bottom of stacks for each AP

  ByteSequence_Replace(0x4284, 2, (AP_tmp+4092), 2, (uint32_t*)vector_addr, (uint32_t*)(vector_addr+rmode_code_size));  // Temporary Stack

  uint32_t* gdt_new = (uint32_t*)(vector_addr + rmode_code_size + 16);
  AP_rmode_GDT_setup((uint32_t*)(vector_addr + rmode_code_size + 8 + 16), gdt_new);

  ByteSequence_Replace(0x3240, 2, (uint32_t)gdt_new, 2, (uint32_t*)vector_addr, (uint32_t*)(vector_addr+rmode_code_size));  // Temporary GDT

  uint32_t* idt_new = (uint32_t*)(vector_addr + rmode_code_size + 16 + 48);
  //AP_idt_Setup((uint32_t*)(vector_addr + rmode_code_size + 8 + 16 + 48), idt_new);
  *idt_new = 0;
  *(idt_new+1) = 0;
  *((uint16_t*)idt_new) = 0x03FF;

  ByteSequence_Replace(0x3250, 2, (uint32_t)idt_new, 2, (uint32_t*)vector_addr, (uint32_t*)(vector_addr+rmode_code_size));  // Temporary IDT

  vector = ((vector_addr)/4096);
  memset((void*)0x1000, 0, 0x2000);

  printf("\nSending INIT IPI...");
  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (APIC_id<<24));
  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004500);        // INIT IPI
  delay_hpet(10);
  printf("\nSending INIT SIPI...");
  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (APIC_id<<24));
  localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004600 | vector);     // SIPI IP]
  delay_hpet(200);
  if(!(*test_counter))
  {
    localapic_write(APIC_LOCAL_BASE, LAPIC_ICRHI, (APIC_id<<24));
    printf("\nReatempting INIT SIPI");
    localapic_write(APIC_LOCAL_BASE, LAPIC_ICRLO, 0x00004600 | vector);     // 2nd SIPI IP
  }
  *(uint32_t*)(vector_addr + rmode_AP_code + 8) = 0x3240;

  if(*test_counter)
    printf("\n\t\t%gThe AP #%x has been Booted in Real Mode Successfully %x %g",10,APIC_id,*test_counter, 0);
  asm volatile("sti");
}

void __attribute__((optimize("O0"))) AP_realMode_Execute(uint8_t intnum, regs16_t *regs)
{
/*  rmode_regs->ax = 0x428a;
  rmode_regs->ds = 0x9999;
  rmode_regs->di = 0x1111;
  *((uint16_t*)0x00001004) = intnum;
/*/
  memcpy(rmode_regs, regs, sizeof(regs16_t));
  *((uint16_t*)0x00001004) = intnum;
  while(*((uint16_t*)0x00001004));
  //delay_hpet(10);
  memcpy(regs, rmode_regs, sizeof(regs16_t));//*/
}
