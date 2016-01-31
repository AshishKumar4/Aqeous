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
	uint32_t *temp1=(uint32_t*)malloc(5000),*temp2=temp1,*test1=(uint32_t*)malloc(100),*test2=test1;
	printf("\n\tLocation of var 1: %x, var 2: %x \n",temp1,test1);
	printf("\n\tPutting Values into the addresses\n");
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
	printf("\n\tDone, Now Reading what we just wrote, 4284 on first few memory, 100 on others\n\n");
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
	printf("\n\nIf you just saw few 4284's and 100's and nothing else, no space; everything worked fine!\n\n");
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
    uint32_t *mmap=(uint32_t*)mboot_ptr->mmap_addr;
    mmap_info=(MemRegion_t*)mmap;//+mboot_ptr->size;
    for(int i=0;i<15;i++)
    {
        if(mmap_info->startLo==0) break;
        printf("region %i address: %x size: %x Bytes Type: %i (%s)\n",i,mmap_info->startHi,mmap_info->sizeHi,
               mmap_info->type,strMemoryTypes[mmap_info->type-1]);
        mmap_info++;
    }
		printf("\nInitializing Memory Manager!\n");
    mmap_info=(MemRegion_t*)mmap;//+mboot_ptr->size;
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

    asm volatile("sti");
    init_timer(50); //PIT WORKING
    printf("\nPIT TIMER Initialized\n");
    //setVesa(0x117);
   // init_timer_RTC()
   printf("LOADING MAIN KERNEL...\n");//*/
	 mdbug=dbug;
	 printf("\n\n\tType shutdown to do ACPI shutdown (wont work on certain systems)");
	 printf("\n\tType mdbug to test the Memory Manager");
}

void kernel_start()
{
}

void kernel_main()
{
		printf("\n");
		char *inst;
    while(1)
    {
				printf("\ncmd>");
				getline(inst);
				console_manager(inst);//*/
				/*Mouse_Plot(mousex,mousey);
				DBuff();//*/
    }
}
