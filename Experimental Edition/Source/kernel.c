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
	/*int *abc=4096*1024;
    //abc=(int*)kmalloc(10*4096);
    for(int i=0;i<4096*1024*10;i++)
    {
        abc[i]=100;
    }
    //
    console_writestring("test ");*/
    placement_address=4096*768;
    initialise_paging();
    map(0xFD000000,1024*768*2);
    enable_paging();
    buff=vmalloc(384);
    //buff=vmalloc(200);
    //vbeModeInfo=map(vbeModeInfo,10);
    /*vga_mem=map(vbeModeInfo->PhysBasePtr,200);
   // buff=(u8int*)vmalloc(1024*768*2);
    //buff=vmalloc(10);*/
    /*abc=map(4096*1024,20);
    for(int i=0;i<100;i+=1)
    {
        console_write_dec(&abc[i]);
        console_writestring("  ");
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
    while(1)
    {
        write_vesa(123456789,10,10);
        write_vesa((placement_address/100)%1000000000,10,100);
        //RectD(500,500,10,20,1000,1000,1000);
        //mouse_handler();
        Mouse_Plot(mousex,mousey);
        DBuff();
    }
}
