#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "tasking.h"
#include "common.h"
#include "SysCalls.h"
#include "Scheduler/Scheduler.h"

#include "SyscallVects.c"

uint32_t _syscall_pop(uint32_t* SYSCALL_SPACE)
{
  uint32_t fr = SYSCALL_SPACE[SYSCALL_SPACE[0]];
  --SYSCALL_SPACE[0];
  return fr;
}

void _syscall_push(uint32_t* SYSCALL_SPACE, uint32_t fr)
{
  ++SYSCALL_SPACE[0];
  SYSCALL_SPACE[SYSCALL_SPACE[0]] = fr;
}

extern uint32_t tmp_esp;
extern uint32_t syscall_ia32_esp;
extern void kernel_main();

void __attribute__((optimize("O0"))) syscall_init()
{
  SYSCALL_SPACE_bottom = (uint32_t*)phy_alloc4K();//pmem_4k(1);
 /* SYSCALL_SPACE = (uint32_t*)(SYSCALL_SPACE_bottom);
  SYSCALL_SPACE_top = SYSCALL_SPACE;
//  tmp_esp = kmalloc(8192);//pmem_4k(2);
//  tmp_esp += 2047;

  /*syscall_ia32_esp = pmem_4k(2);
  syscall_ia32_esp += 2047;*/

  /************ SETTING UP SYSCALL HANDLERS FOR EACH CPU ************/

  SchedulerKits_t* kits = MotherSpace;
  uint32_t syscallvec_sz = (uint32_t)syscall_vector_end_t - (uint32_t)syscall_vector_t;
  printf("\nsyscallvec_sz: %d", syscallvec_sz);

  cpuGetMSR(IA32_SYSENTER_CS, 0x08, 0);
  cpuGetMSR(IA32_SYSENTER_ESP, 0xCD00000, 0);
  cpuGetMSR(IA32_SYSENTER_EIP, &kernel_main, 0);
  for(int i = 0; i < total_CPU_Cores; i++)
  {

    kits->syscall_vector = phy_alloc4K();

    printf("\n{Scheduler: %x}, Switcher: %x, syscall vec: %x", kits->scheduler, kits->switcher, kits->syscall_vector);

    uint32_t tmp = (uint32_t)kits->syscall_vector;
    tmp += 256;
    *((uint32_t*)tmp) = &syscall_handler;
    memcpy(kits->syscall_vector, syscall_vector_t, syscallvec_sz);
    ByteSequence_Replace(0x4284ACD1, 4, (uint32_t)&kits->current_pdir, 4, kits->syscall_vector, kits->syscall_vector + syscallvec_sz); //_cur_dir
    ByteSequence_Replace(0x4284ACD2, 4, (uint32_t)&system_dir, 4, kits->syscall_vector, kits->syscall_vector + syscallvec_sz); //_cur_dir
  //  ByteSequence_Replace(0x4284ACD3, 4, (uint32_t)&kits->curr_dir, 4, kits->syscall_vector, kits->syscall_vector + syscallvec_sz); //_cur_dir
    ByteSequence_Replace(0x4284ACD7, 4, (uint32_t)phy_alloc4K(), 4, kits->syscall_vector, kits->syscall_vector + syscallvec_sz); //_cur_dir
    ByteSequence_Replace(0x4284ACD5, 4, (uint32_t)tmp, 4, kits->syscall_vector, kits->syscall_vector + syscallvec_sz); //_cur_dir
    ByteSequence_Replace(0x4284ACD6, 4, (uint32_t)&kits->tmp_threadStack, 4, kits->syscall_vector, kits->syscall_vector + syscallvec_sz); //_cur_dir
    ByteSequence_Replace(0x4284CD00, 4, (uint32_t)&kits->stack, 4, kits->syscall_vector, kits->syscall_vector + syscallvec_sz); //_cur_dir

    uint32_t* idt_ptr = (uint32_t*)(kits->idt);
    idt_ptr += 2;
    idtSetEntry(54, (uint32_t)kits->syscall_vector, 0x08, makeFlagByte(1, USER_MODE), (uint64_t*)idt_ptr);

    pmode_IDT_lidt(i, kits->idt);
    ++kits;
  }
}

void syscall_MapPdir(PageDirectory_t* pdir)
{
  Map_non_identity(SYSCALL_SPACE_bottom, 0xC0004000, 4096, pdir);
}

extern void kill_tmp();

void syscall_handler()
{
  uint32_t* ptr = (uint32_t*)get_phyAddr((uint32_t)0xC0004000, (PageDirectory_t*)(Get_Scheduler()->current_pdir));
 
  uint32_t call = _syscall_pop(ptr);
  func_t handle = LIBSYMTABLE[call];
  if(!handle)
  {
    printf("\nNo Syscall Handle FOUND for call %d", call);
    Shell_Dbuff_sync();
    asm volatile("hlt");
    return;
  }
  handle(ptr);
//  if(call == 21)
/*  printf("Help");//, Get_Scheduler()->current_pdir);
  Shell_Dbuff_sync();*/
}
