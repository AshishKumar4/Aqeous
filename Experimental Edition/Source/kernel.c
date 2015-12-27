#include "mouse.c"
#include "console.c"
#include "interrupts.c"
#include "multiboot.h"
//#include "vfs.c"
//#include "initrd.c"
#include "timer.c"
#include "mem.c"
#include "paging.c"
#include "ordered_array.c"

u32int initial_esp;
void timertest()
{
    //RectD(50,100,20,30,1000,1000,1000);
}

uint32_t kernelSize=4096;

void kernel_early(struct multiboot *mboot_ptr,u32int initial_stack)
{
    init_descriptor_tables();

    initial_esp = initial_stack;
	//setVesa(0x117);
	initialise_paging();
	console_init();
	//setVesa(0x117);
	//buff=(u8int*)kmalloc(4096*768*2);
	int *abc;
	int b;
	for(int i=0;i<100;i++)
    {
        abc=(int*)vmalloc_id(512);
        *abc=*abc+10;
        console_write_dec(*abc);
        console_writestring("  ");
        b=abc;
        console_write_dec(b);
        console_writestring("  ");
    }
    console_writestring(" Just this much");
	/*pt_entry Page1;
    pt_entry_add_attrib (&Page1, I86_PTE_PRESENT);
	vmmngr_alloc_page(Page1);
	pt_entry Page2;
    pt_entry_add_attrib (&Page2, I86_PTE_PRESENT);
	vmmngr_alloc_page(Page2);

	pt_entry Page3;
    pt_entry_add_attrib (&Page3, I86_PTE_PRESENT);
	vmmngr_alloc_page(Page3);
    //setVesa(0x117,&Page1,&Page2,&Page3);
	//vmmngr_map_page(&vga_mem,&Page1);
	//RectL(0,0,1023,767,90,90,90);*/
    /*console_writestring("Mem Size: ");
    console_write_dec(max_blocks);
    console_writestring("  IS PAGING ENABLED? 2 means true, 0 means false: ");
    console_write_dec(pmmngr_is_paging());//*/
    asm volatile("sti");
    init_timer(50); //PIT WORKING
    //setVesa(0x117);
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
