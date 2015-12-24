#include <mouse.c>
#include <kheap.c>
#include <console.c>
#include <interrupts.c>
#include "multiboot.h"
#include "vfs.c"
#include "ordered_array.c"
#include "initrd.c"
#include <timer.c>
#include "paging.c"
#include "task.c"

u32int initial_esp;
void timertest()
{
    //RectD(50,100,20,30,1000,1000,1000);
}
void kernel_early(struct multiboot *mboot_ptr,u32int initial_stack)
{
    initial_esp = initial_stack;
    //end1=end;
    console_init();
    init_descriptor_tables();
    //placement_address=0x100000;
    console_writestring("HELLO WORLD");
    asm volatile("sti");
    init_timer(50); //PIT WORKING
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    asm volatile ("cli");
    setVesa(0x117);
    placement_address=&vga_mem[0];
    //initialise_paging();
    asm volatile("sti");
    /**COMMENT OUT THESE TO ENABLE PAGING, But VESA WONT WORK**/
    //initialise_paging();
    //console_writestring("TEST");
    //initialise_tasking();
    /**JUST THIS MUCH TO ENABLE PAGING AND MULTITASKING**/
    //abc=PhyToVirtual(abc);
   // init_timer_RTC();
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
