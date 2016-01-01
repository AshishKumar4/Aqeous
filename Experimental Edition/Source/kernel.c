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
	console_init();
	console_writestring("start: ");

	setVesa(0x117);
	//RectL(0,0,100,100,1000,1000,1000);
	int *abc=kmalloc(1);
    //abc=(int*)kmalloc(10*4096);
    /*for(int i=0;i<409610;i++)
    {
        abc[i]=100;
    }
    //*/
    placement_address=4096*768;
    initialise_paging();
    //buff=vmalloc(200);
    vga_mem=map(4244635648,200);
    //buff=vmalloc(10);
    /*abc=map(4244635648,1);
    for(int i=0;i<8392;i++)
    {
        abc[i]=100;
    }
    for(int i=8192;i<8292;i+=1)
    {
        console_write_dec(&abc[i]);
        console_write_dec(abc[i]);
        console_writestring("  ");
    }
    console_writestring("done");
    //map(&vga,4096*400);*/
	/*;
	//RectL(0,0,1024,768,1000,1000,1000);
	//abc=(int*)vmalloc(10);
	//setVesa(0x117);
	//buff=(u8int*)kmalloc(4096*768*2);
	int b;
	/*for(int i=0;i<10*4096;i+=4096)
    {
        abc[i]=&abc[i]-&abc[0];
        console_write_dec(abc[i]);
        console_writestring("  ");
        console_write_dec(&abc[i]);
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
    RectD(0,0,1023,767,90,90,90);
    while(1)
    {
        write_vesa(123456789,10,10);
        write_vesa((placement_address/100)%1000000000,10,100);
        //RectD(500,500,10,20,1000,1000,1000);
        //mouse_handler();
        Mouse_Plot(mousex,mousey);
        DBuff();
    }
   // u32int *ptr = (u32int*)0xA0000000;
   // u32int do_page_fault = *ptr;*/
}
