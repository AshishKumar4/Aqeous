#include "mouse.c"
#include "console.c"
#include "interrupts.c"
#include "multiboot.h"
#include "vfs.c"
#include "cmos.c"
#include "pci.c"
#include "ahci.c"
#include "timer.c"
#include "mem.c"
#include "paging.c"
#include "ata.c"
#include "acpi.c"
#include "vmem.c"
#include "ordered_array.c"
#include "keyboard.c"

u32int initial_esp;
u8int calle=0;

void timertest()
{
   /* RectD(0,0,500,500,700,700,700);
    Mouse_Plot(mousex,mousey);
    DBuff();*/
}

void dbug()
{
	uint32_t *temp1=malloc(5000),*temp2=temp1,*test1=malloc(100),*test2=test1;
	printf("\n\tLocation of var 1: %x, var 2: %x \n",temp1,test1);
	printf("Putting Values into the addresses\n");
	for(int i=0;i<1000;i++)
	{
		*temp1=4284;
		++temp1;
	}
	temp1=temp2;
	for(int i=0;i<100;i++)
	{
		*test1=100;
		++test1;
	}
	test1=test2;
	for(int i=0;i<1000;i+=50)
	{
		printf(" %x ",*temp1);
		++temp1;
	}
	for(int i=0;i<100;i+=5)
	{
		printf(" %x ",*test1);
		++test1;
	}
	printf("\ndone\n");
}

void kernel_early(struct multiboot *mboot_ptr,u32int initial_stack)
{
		console_init();
		mouseinit();
		printf("\nMouse Drivers initialized\n");
		keyboard_init();
		printf("\nKeyboard Drivers Initialized\n");
    init_descriptor_tables();
    printf("DESCRIPTOR TABLES INITIALIZED \n");
    printint(mboot_ptr->mmap_addr);
    printf("\n");
    printint(mboot_ptr->mmap_length);
    printf("\nAvailable memory: ");
    printint(mboot_ptr->mem_upper);
    printf("\nMemory Map:");
    maxmem=mboot_ptr->mem_upper;
    uint32_t *mmap=mboot_ptr->mmap_addr;
    mmap_info=mmap;//+mboot_ptr->size;
    for(int i=0;i<15;i++)
    {
        if(mmap_info->startLo==0) break;
        printf("region %i address: %x size: %x Bytes Type: %i (%s)\n",i,mmap_info->startHi,mmap_info->sizeHi,
               mmap_info->type,strMemoryTypes[mmap_info->type-1]);
        mmap_info++;
    }
		printf("\nInitializing Memory Manager!\n");
    mmap_info=mmap;//+mboot_ptr->size;
    Mapper();
		printf("\nEnabling ACPI!\n");
    initAcpi();
    if(!acpiEnable())
        printf("\nACPI Initialized\n");
    else printf("\nACPI CANT BE INITIALIZED\n");

		printf("\n\nEnumerating all devices on PCI BUS:\n");
    checkAllBuses();

		checkAHCI();
		if(!ahci_found) init_ata();

		//setVesa(0x117);
	  printf("\nEnabling Paging\n");
    initialise_paging();
    //map(vbeModeInfo->PhysBasePtr,1024*768*2);
    enable_paging();

  	printf("\n Paging Has been Enabled Successfully!");
		printf("\n Available Memory: %x KB\n",maxmem);

    /*init_ata();
    printf("\nHard Disk Initialized\n");
    console_writestring(" PATA Info: Heads: ");
    console_write_dec(ident.heads);
    console_writestring(" Sectors: ");
    console_write_dec(ident.sectors);
    console_writestring(" Cylinders: ");
    console_write_dec(ident.cyls);
    console_writestring(" Bytes per Sector ");
    console_write_dec(ident.sector_bytes);
    console_writestring(" Bytes per Track ");
    console_write_dec(ident.track_bytes);
    console_writestring(" Serial No: ");
    console_writestring(ident.serial_no);//*/

    asm volatile("sti");
    init_timer(50); //PIT WORKING
    printf("\nPIT TIMER Initialized\n");
    //setVesa(0x117);
   // init_timer_RTC()
   printf("LOADING MAIN KERNEL...\n");//*/
	 mdbug=dbug;
	 mdbug();
}

void kernel_start()
{
}

void kernel_main()
{
		printf("\n");
		uint8_t *inst;
    while(1)
    {
				/*printf("\ncmd>");
				getline(inst);
				console_manager(inst);//*/
				Mouse_Plot(mousex,mousey);
				DBuff();//*/
    }
}
