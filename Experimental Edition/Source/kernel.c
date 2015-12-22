#include <mouse.c>
#include <kheap.c>
#include <console.c>
#include <paging.c>
#include <interrupts.c>
#include <timer.c>

void timertest()
{
    RectD(50,100,20,30,1000,1000,1000);
}
void kernel_early()
{
    //end1=end;
    //placement_address=(u32int)kmalloc(1);
    console_init();
    init_descriptor_tables();
    //initialise_paging();
    //console_writestring("HELLO WORLD");
    init_timer(1); //PIT WORKING
    init_timer_RTC();
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
        mouse_handler();
        Mouse_Plot(mousex,mousey);
        DBuff();
    }
   // u32int *ptr = (u32int*)0xA0000000;
   // u32int do_page_fault = *ptr;
}
