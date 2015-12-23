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
extern u32int placement_address;
void timertest()
{
    RectD(50,100,20,30,1000,1000,1000);
}
void kernel_early(struct multiboot *mboot_ptr,u32int initial_stack)
{
    initial_esp = initial_stack;
    //end1=end;
    //placement_address=(u32int)kmalloc(1);
    console_init();
    init_descriptor_tables();
    console_writestring("HELLO WORLD");
    asm volatile("sti");
    init_timer(50); //PIT WORKING
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    fs_root = initialise_initrd(initrd_location);
    fs_root = initialise_initrd(initrd_location);
    //int *abc=(int*)kmalloc(sizeof(int));
    /**COMMENT OUT THESE TO ENABLE PAGING, But VESA WONT WORK**/
    //initialise_paging();
    //console_writestring("TEST");
    //initialise_tasking();
    /**JUST THIS MUCH TO ENABLE PAGING AND MULTITASKING**/
    //abc=PhyToVirtual(abc);
   // init_timer_RTC();
    mouseinit();
    setVesa(0x117);
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
