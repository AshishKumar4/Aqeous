#include "mouse.c"
#include "console.c"
#include "interrupts.c"
#include "multiboot.h"
//#include "vfs.c"
//#include "initrd.c"
#include "timer.c"
#include "mem.c"
#include "paging.c"

u32int initial_esp;
void timertest()
{
    //RectD(50,100,20,30,1000,1000,1000);
}

uint32_t kernelSize=4096;

void kernel_early(struct multiboot *mboot_ptr,u32int initial_stack)
{
    Init_mem(4096*1024*128,0x100000 + kernelSize*8);

    initial_esp = initial_stack;
    pmmngr_init_region(0x100000 + 4096*8,4096*32);
    pmmngr_init_region(0x100000 + 4096*32,4096*1024*128);
	pmmngr_deinit_region (0x100000, kernelSize*8);
    //end1=end;
	vmmngr_initialize ();
    console_init();
    init_descriptor_tables();
    console_writestring("Mem Size: ");
    console_write_dec(max_blocks);
    console_writestring("  IS PAGING ENABLED? 2 means true, 0 means false ");
    console_write_dec(pmmngr_is_paging());
    asm volatile("sti");
    init_timer(50); //PIT WORKING
   // init_timer_RTC();
    //setVesa(0x117);
    mouseinit();
}

void kernel_start()
{
}

void kernel_main()
{
    RectD(0,0,1023,767,150,150,150);
    while(1)
    {
        //RectD(500,500,10,20,1000,1000,1000);
        //mouse_handler();
        Mouse_Plot(mousex,mousey);
        DBuff();
    }
   // u32int *ptr = (u32int*)0xA0000000;
   // u32int do_page_fault = *ptr;*/
}
