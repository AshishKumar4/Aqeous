#include "descriptors.h"
#include "stdio.h"
#include "string.h"
#include "apic.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "stdlib.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "phy_mm/mem.h"
#include "shell.h"
#include "tasking.h"
#include "console.h"
#include "sys.h"
#include "graphics.h"

#include "cpu/cpu.h"

uint32_t* LAPIC_EOI_send = (uint32_t*)(0x00b0+0xfee00000);
extern void switcher();

void divByZero_handler()
{
  printf("\nFault1");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}


void debug_handler()
{
  asm volatile("pop %eax; \
                pop %ebx; \
                pop %ecx; \
                pop %edx; \
                hlt;");
  uint32_t i;
  i = *(uint32_t*)(0xfee00020);
  i = i>>24;
  printf("\nFault2 %x", i);
  asm volatile("hlt");
  //Shell_Double_buffer();
  //while(1);*/
  asm volatile("iret");
}


void NMI_handler()
{
  asm volatile("pop %eax; \
              pop %ebx; \
              pop %ecx; \
              pop %edx; \
              hlt;");
  printf("\nFault3");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}

extern void breakpoint_handler();

void breakpoint_handler2()
{
  asm volatile("pop %eax;\
                iret");
  asm volatile("pop %eax; \
                pop %ebx; \
                pop %ecx; \
                pop %edx; \
                hlt;");
  printf("\nFault4");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}


void overflow_handler()
{
  printf("\nFault5");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}


void outOfBounds_handler()
{
  asm volatile("pop %eax; \
              pop %ebx; \
              pop %ecx; \
              pop %edx; \
              hlt;");
  printf("\nFault6 %x", Get_Scheduler()->identity);
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}

extern void invalidInstr_handler();

void invalidInstr_handler2()
{
  asm volatile("pop %eax; \
              pop %ebx; \
              pop %ecx; \
              pop %edx; \
              hlt;");
  /*printf("\nFault7 %x", Get_Scheduler()->identity);
  asm volatile("hlt");
  //Shell_Double_buffer();
  while(1);*/
  asm volatile("iret");
}


void noCoprocessor_handler()
{
  printf("\nFault8");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}

extern void doubleFault_handler();

void doubleFault_handler2()
{
  asm volatile("iret");
  asm volatile("pop %eax; \
                pop %ebx; \
                pop %ecx; \
                pop %edx; \
                hlt;");
  /*printf("\nFault9");
  asm volatile("hlt");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);*/
  asm volatile("iret");
}

extern void coprocessor_handler();

void coprocessor_handler2()
{
  printf("\nFault10");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}


void badTSS_handler()
{
  printf("\nFault11");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}


void segmentNotPresent_handler()
{
  printf("\nFault12");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}


void stackFault_handler()
{
  printf("\nFault13");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}

int avc = 0;

extern void generalProtectionFault_handler();

void __attribute__((optimize("O0"))) generalProtectionFault_handler2()
{
  asm volatile("pop %eax; \
                pop %ebx; \
                pop %ecx; \
                pop %edx; \
                hlt;");/*
  asm volatile("cli;\
                pop %%eax; pop %%eax; pop %%eax; pop %%eax;\
                mov %%eax, %0;":"=r"(avc)::"memory");
  uint32_t i;
  i = *(uint32_t*)(0xfee00020);
  i = i>>24;
  printf("\nGeneral Protection Fault! %x %x",i, avc);
  asm volatile("mov %0, %%eax;\
                hlt"::"r"(avc):"memory");*/
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}

extern uint32_t faulting_address;
extern uint32_t current_pdir;
uint32_t npf = 0;

void __attribute__((optimize("O2"))) pageFault_caller()
{
  /*printf("[%x] ", faulting_address);
//  npf += 1;
  if(((Process_t*)((task_t*)(Get_Scheduler())->current_task)->process)->privileges)
  {
    printf("\nUser tried to access unmapped memory %x !!", faulting_address);
    Shell_Dbuff_sync();
    asm volatile("hlt");
    return;
  }*/
  uint32_t addr = faulting_address;
  //printf(".^%x^.", addr);
  addr /= 0x1000;
  // Find the page table containing this address.
  uint32_t table_idx = addr / 1024;

  PageDirectory_t* dir = (PageDirectory_t*)current_pdir;
  //printf("{%u}", faulting_address);

  page_t* page;

  if (dir->table_entry[table_idx]) // If this table is already assigned
  {
      table_t* entry = &dir->table_entry[table_idx];
      PageTable_t* table=(PageTable_t*)PAGE_GET_PHYSICAL_ADDRESS(entry);
      page = &table->page_entry[addr%1024];
  }
  else
  {
      dir->table_entry[table_idx] = (table_t)phy_alloc4K();
      memset_fast((void*)dir->table_entry[table_idx], 0, 0x1000);
      table_t* entry = &dir->table_entry [table_idx];
      PageTable_t* table=(PageTable_t*)PAGE_GET_PHYSICAL_ADDRESS(entry);
      *entry |= I86_PDE_PRESENT;
      *entry |= I86_PDE_WRITABLE;
      *entry |= I86_PDE_USER;
      //pd_entry_set_frame (entry, (uint32_t)dir->m_entries[table_idx]);
    //  dir->tables[table_idx] = tmp | 0x7; // PRESENT, RW, US.
      page = &table->page_entry[addr%1024];
  }
  if(*page)
  {
    // Must be a case of Copy on Write
    /*printf("\nWRITE ERROR!!!!!");
    Shell_Dbuff_sync();
    asm volatile("hlt");//*/
    printf("[{*%x*}]",*page);
    *page |= I86_PTE_PRESENT;
    *page |= I86_PTE_WRITABLE;
    *page |= I86_PTE_USER;
    return;
  }
  uint32_t phy_frame = pop_frameStack();
  *page = phy_frame*4096;
  //printf("\n<%x:%x for %x>",*page, phy_frame, faulting_address);
  *page |= I86_PTE_PRESENT;
  *page |= I86_PTE_WRITABLE;
  *page |= I86_PTE_USER;
}

void unknownInterrupt_handler()
{
  printf("\nFaul15t");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}


void coprocessorFault_handler()
{
  printf("\nFault16");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}


void alignmentCheck_handler()
{
  printf("\nFault17");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}


void machineCheck_handler()
{
  printf("\nFault18");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}


void reserved_handler()
{
  printf("\nFault19");
  asm volatile("hlt");
  Shell_Double_buffer();
  while(1);
  asm volatile("iret");
}

//#ifdef PIC
void PIT_Handle()
{
  //asm volatile("cli");
 /* printf("a-");
  /*/
  //Screen_BuffSync();
  PIT_Task();//*/
//  while(1);
//  outb(0x20,0x20);
//  printf("\nHello!");
  ++(PIT_Counter);
//asm volatile("hlt");
}

uint32_t ag = 1,ab = 0;

void keyboardInterrupt_handler()
{
  int scancode=inb(0x60);
  if(scancode <= 0x80+0x60)
  {
    NormalKey_Func(scancode);
  }
  return;
}

void cascade_handler()
{
asm volatile("iret");
  asm volatile("cli");
  printf("\nInterrupt 2");
  while(1);
  asm volatile("iret");
}

//This particular interrupt is never raised
void COM2_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 3");
  while(1);
  asm volatile("iret");
}


void COM1_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 4");
  while(1);
  asm volatile("iret");
}


void LPT2_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 5");
  while(1);
  asm volatile("iret");
}


void floppyDisk_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 6");
  while(1);
  asm volatile("iret");
}


void LPT1_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 7");
  while(1);
  asm volatile("iret");
}


void RTC_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 8");
  while(1);
  asm volatile("iret");
}


void periph1_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 9");
  while(1);
  asm volatile("iret");
}


void periph2_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 10");
  while(1);
  asm volatile("iret");
}


void periph3_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 11");
  while(1);
  asm volatile("iret");
}

void mouse_handler()
{
  while(cycle<3)
    mouse_bytes[cycle++] = inb(0x60);
//  mousedeltax=0;
//  mousedeltay=0;
//  if (cycle == 3)
  { // if we have all the 3 bytes...
    cycle = 0; // reset the counter
    // do what you wish with the bytes, this is just a sample
    if ((mouse_bytes[0] & 0x80) || (mouse_bytes[0] & 0x40))
      return; // the mouse only sends information about overflowing, do not care about it and return
    /*if (!(mouse_bytes[0] & 0x20))
      mousedeltay |= 0xFFFFFF00; //delta-y is a negative value
    if (!(mouse_bytes[0] & 0x10))
      mousedeltax |= 0xFFFFFF00; //delta-x is a negative value
//*/
    /*if(mouse_bytes[0] & 0x1)    // Left Click
    {
      mouse_left_click = 1;
    }
    if(mouse_bytes[0] & 0x2)    // Right Click
    {
      mouse_right_click = 1;
    }//*/
    mouse_left_click = mouse_bytes[0] & 0x1;

    mousedeltax=mouse_bytes[1];
    mousedeltay=mouse_bytes[2];
  //  mousex+=(deltax);
  //  mousey-=(deltay);
  }
  //asm volatile("sti");*/
  ////Switch_back_from_System();
}

void FPU_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 13");
  while(1);
  asm volatile("iret");
}


void primaryHDD_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 14");
  while(1);
  asm volatile("iret");
}

//P -> Primary
void secondaryHDD_handler()
{
  asm volatile("cli");
  printf("\nInterrupt 15");
  while(1);
  asm volatile("iret");
}

//S -> Secondary
