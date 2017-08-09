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

void __attribute__((optimize("O0"))) pageFault_caller()
{
  uint32_t addr = faulting_address;
  addr /= 0x1000;
  // Find the page table containing this address.
  uint32_t table_idx = addr / 1024;

  PageDirectory_t* dir = current_pdir;

  page_t* page;

  if (dir->table_entry[table_idx]) // If this table is already assigned
  {
      //printf("1 ");
      table_t* entry = &dir->table_entry[table_idx];
      PageTable_t* table=(PageTable_t*)PAGE_GET_PHYSICAL_ADDRESS(entry);
      page = &table->page_entry[addr%1024];
  }
  else
  {
      dir->table_entry[table_idx] = (table_t)phy_alloc4K();
      map(dir->table_entry[table_idx], 4096, dir);
      memset_fast((void*)dir->table_entry[table_idx], 0, 0x1000);
      table_t* entry = &dir->table_entry [table_idx];
      PageTable_t* table=(PageTable_t*)PAGE_GET_PHYSICAL_ADDRESS(entry);
      *entry |= I86_PDE_PRESENT;
      *entry |= I86_PDE_WRITABLE;
      //pd_entry_set_frame (entry, (uint32_t)dir->m_entries[table_idx]);
    //  dir->tables[table_idx] = tmp | 0x7; // PRESENT, RW, US.
      page = &table->page_entry[addr%1024];
  }
  uint32_t phy_frame = pop_frameStack();
  *page = (*page & ~I86_PTE_FRAME) | phy_frame;
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
  //printf("a-");
  Screen_BuffSync();
//  while(1);
//  outb(0x20,0x20);
  ++(*(uint32_t*)(40*1024*1024));
  //asm volatile("iret");
}

uint32_t ag = 1,ab = 0;

void keyboardInterrupt_handler()
{
  //printf("a-");
  //Switch_to_system_dir();
  //if(kybrd_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_OUT_BUF)
  {
    int scancode=inb(0x60);
    if(scancode&0x80) //break code
    {
      scancode-=0x80; //make it make code
      int key=Special_key_codes[scancode];
      //! test if a special key has been released & set it
      switch (key)
      {
            case KEY_LCTRL:
            case KEY_RCTRL:
              _ctrl = 0;
              break;

            case KEY_LSHIFT:
            case KEY_RSHIFT:
              _shift = 0;
              break;

            case KEY_LALT:
            case KEY_RALT:
              _alt = 0;
              break;
            case KEY_UP:
              _arrow_up = 0;
              break;
            case KEY_DOWN:
              _arrow_down = 0;
              break;
        }
    }
    else //it is a make code
    {
      int key=scancodes[scancode];
      if(!key)
      {
        key = Special_key_codes[scancode];
        switch (key)
        {

              case KEY_LCTRL:
                _ctrl = 1;
                break;
              case KEY_RCTRL:
                _ctrl = 2;
                break;

              case KEY_LSHIFT:
                _shift = 1;
                break;
              case KEY_RSHIFT:
                _shift = 2;
                break;

              case KEY_LALT:
                _alt = 1;
                break;
              case KEY_RALT:
                _alt = 2;
                break;

              case KEY_CAPSLOCK:
                _capslock = (_capslock) ? false : true;
                kkybrd_set_leds (_numlock, _capslock, _scrolllock);
                break;

              case KEY_KP_NUMLOCK:
                _numlock = (_numlock) ? false : true;
                kkybrd_set_leds (_numlock, _capslock, _scrolllock);
                break;

              case KEY_SCROLLLOCK:
                _scrolllock = (_scrolllock) ? false : true;
                kkybrd_set_leds (_numlock, _capslock, _scrolllock);
                break;
              case KEY_UP:
              //  _arrow_up = 1;
                up_input = 1;
              //  Shell_scrollUp();
                break;
              case KEY_DOWN:
                _arrow_down = 1;
              //  Shell_scrollDown();
                break;

              default:
                break;
          }
        }
        else
        {
          call = keyboard_scancodes(key);
          //  call = key;
          if(_ctrl && (call == 'c' || call == 'C'))
          {
            _ctrl_C_pressed = 1;
          }
          if(call == '\r')
          {
            enter_pressed = 1;
            //Priority_promoter(Shell_Istream_task);
          }
          else if(call == '\b') //TODO: Backspace is hit!
          {
            if(kb_buff)
            {
              --Istream_ptr;
              *Istream_ptr = 0;
              --kb_buff;
              backspace();
            }
          }
          else
          {
            *(Istream_ptr) = call;
            ++Istream_ptr;
            ++kb_buff;
            if((uint32_t)Istream_ptr == Istream_end)
             Istream_ptr = (char*)Input_stream;   //Reset
            _putchar((int)call);
        }
      }
    }
  }
  //Switch_back_from_System();
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
  //asm volatile("cli");
  ////Switch_to_system_dir();
  //  RectD(100,100,1000,1000,000,100,1000);
//  ab+=5;
//  Pixel_VESA(ab,ab,9,9,9);
//  ag = 0;

  //printf("\nInterrupt 12");
  //while(1);
///*
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
