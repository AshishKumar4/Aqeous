#include "mouse.c"
#include "console.c"
#include "descriptors.c"
#include "int_handlers.c"
#include "cpu.c"
#include "multiboot.h"
#include "vfs.c"
#include "cmos.c"
#include "pci.c"
#include "ahci.c"
#include "graphics.c"
#include "timer.c"
#include "apic.c"
#include "cpuid.c"
#include "mem.c"
#include "paging.c"
#include "ata.c"
#include "acpi.c"
#include "vmem.c"
#include "keyboard.c"
#include "task.c"
#include "tasking.c"
#include "process.c"
#include "hpet.c"
#include "fs.c"
#include "fs_alloc.c"
#include "Scheduler.c"
#include "Shell.c"

u32int initial_esp;
uint32_t initial_ebp;
u8int calle=0;

void dbug()
{
	int a[4];
	//printf("\n\tEnter the size of: ");
	for(int i=0;i<4;i++)
	{
	//	printf("\n\t\tvar %x: ",i+1);
		a[i]=4096;//getint();
	}
	uint32_t *temp1=(uint32_t*)malloc(40),*temp2=temp1;
	*temp1=4284;
	//kmalloc(4096);
	uint32_t *test1=(uint32_t*)malloc(40),*test2=test1;
	uint32_t *test3=(uint32_t*)malloc(8192);
	uint32_t *test4=(uint32_t*)malloc(a[3]);
	uint32_t *test5=(uint32_t*)malloc(128);
	printf("\n\tLocation of var 1: %x, var 2: %x var 3: %x var 4: %x var 5: %x \n",temp1,test1,test3,test4,test5);
	printf("\tPutting Magic Numbers into first two addresses\n");
	for(int i=0;i<8;i++)
	{
		*temp1=4284;
		++temp1;
	}
	temp1=temp2;
	for(int i=0;i<8;i++)
	{
		*test1=100;
		++test1;
	}
	printf("\tDone, Now Reading what we just wrote, 4284 on first few memory, 100 on others\n");
	test1=test2;
	for(int i=0;i<8;i++)
	{
		printf(" %x ",*temp1);
		++temp1;
	}
	for(int i=0;i<8;i++)
	{
		printf(" %x ",*test1);
		++test1;
	}
	temp1=temp2;
	test1=test2;
	*test3=12345678;
	*test4=87654321;
	printf(" %x %x ",*test3,*test4);
	printf("If you just saw few 4284's and 100's and nothing else, no extra space; everything worked fine!\n");
	printf("Now Freeing the memory!\n");
	//free(temp2);
//	free(test);
	free(temp2);
	free(test3);
/*
	for(int i=0;i<8;i++)
	{
		printf(" %x ",*temp1);
		++temp1;
	}///*/
	for(int i=0;i<8;i++)
	{
		printf(" %x ",*test1);
		++test1;
	}//*/
//	printf(" %x %x ",*test3,*test4);
	printf(" If you didnt saw any numbers above, It worked!!!\n");
	uint32_t *tmp1=(uint32_t*)malloc(40),*tmp2=tmp1;
	//kmalloc(4096);
	uint32_t *tst1=(uint32_t*)malloc(4096),*tst2=tst1;

	printf("\tLocation of var 1: %x, var 2: %x %x\n",tmp1,tst1,malloc(8192));
}

uint32_t initial_eip;

void kernel_main();

tss_struct_t *TSS;

extern void get_eax();
extern void reload_eax();
extern uint32_t cur_eax;

void kernel_early(struct multiboot *mboot_ptr,uint32_t initial_stack)
{
	 //Kernel stack located at 200th mb to 250th mb
    //initial_esp = 0xC800000;
		console_init();
		init_APIC();
    init_descriptor_tables();
    printf("\nDESCRIPTOR TABLES INITIALIZED \n");
		printf("\nEnabling ACPI!\n");
    initAcpi();
    if(!acpiEnable())
        printf("\nACPI Initialized\n");
    else printf("\nACPI CANT BE INITIALIZED\n");
		//init_hpet();
		//init_timer(1000);
		//while(1);

		mouseinit();
		printf("\nMouse Drivers initialized\n");
		keyboard_init();
		printf("\nKeyboard Drivers Initialized\n");

    printf("\nAvailable memory: ");
    printint(mboot_ptr->mem_upper);
    printf("\nMemory Map:");
    memAvailable=mboot_ptr->mem_upper;
    uint32_t *mmap=(uint32_t*)mboot_ptr->mmap_addr;
    mmap_info=(MemRegion_t*)mmap;//+mboot_ptr->size;
		maxmem=memAvailable;
    for(int i=0;i<20;i++)
    {
        if(mmap_info->startLo==0) break;
        printf("region %i address: %x size: %x Bytes Type: %i (%s)\n",i,mmap_info->startHi,mmap_info->sizeHi,
               mmap_info->type,strMemoryTypes[mmap_info->type-1]);
        mmap_info++;
    }
		printf("\nInitializing Memory Manager!\n");
    mmap_info=(MemRegion_t*)mmap;//+mboot_ptr->size;
		max_mem=maxmem*1024;
    Mapper();
		printf("\nMemory block lists prepared!\n");
		bitmap_init();
		initialise_paging();
		enable_paging();

		printf("\n Paging Has been Enabled Successfully!");
		printf("\n Available Memory: %x KB\n",maxmem);
		switch_pdirectory(system_dir);
		printf("\n\nEnumerating all devices on PCI BUS:\n");
		checkAllBuses();
		printf("\nEnabling Hard Disk\n");
		checkAHCI();
		if(!ahci_found)
			init_ata();
		//printf(" %x %x ",satatest,abcd);
		//printf(" %x %x %x  ",sizeof(unsigned short),sizeof(unsigned long),sizeof(unsigned long int));s
	//	while(1);
	//  map(AHCI_BASE,1024*1024*50);

 	 	detect_cpu();
		//initTasking();
	//	asm volatile("sti");

	//	init_cmos();
	//	printf("\nCMOS Clock Initialized\n");

   printf("\nLOADING MAIN KERNEL...\n");
	 mdbug=dbug;
	 vesa=setVesa;
	 uint32_t a=0;
	 asm volatile("movl %%cr0, %0":"=r"(a));
	 printf("\n%x Protected Mode Enabled? ",a);
	 if(a & (1<<0))
	 	printint(1);

	 printf("\n\n\tType shutdown to do ACPI shutdown (wont work on certain systems)");
	 printf("\n\tType mdbug to test the Memory Manager");
	 printf("\n location of kernel_main: %x ",kernel_main);
	 printf("sizeof Directory_t %x, File_t %x",sizeof(Directory_t),sizeof(File_t));

	 //Init_fs();
	 printf("\nsize of task_t: %x",sizeof(task_t));
	 printf("\nsize of HPET_Table_t: %x",sizeof(HPET_Table_t));
	 if(cpuHasMSR()) printf("\nCPU has MSR");//*/
	 //switch_pdirectory(main_dir);

	 printf("\n\nInitializing MultiThreading System");
	 init_multitasking();
	 while(1);
//
/*
	 printf(" %x",3|0x400);
	 for(int i=0; i<40960; i++)
	 {
		 kmalloc(40);
	 }//
//	 dbug();
		cur_eax = 4284;
		reload_eax();
		get_eax();
		printf("\n Current eax: %x ",cur_eax);
		reload_eax();
		get_eax();
		printf("\n Current eax: %x ",cur_eax);*/
	//	dbug();
}

void kernel_start()
{
}

void kernel_main()
{
		printf("\n");
		char *inst=" ";
		uint8_t flg=0;
    while(!flg)
    {
			//	asm volatile("cli");
				printf("\n%s>",curr_dir.full_name);
				getline(inst);
				printf(inst);
				flg=console_manager(inst);
			//	asm volatile("sti");
    }
		while(1)
		{
			Mouse_Plot(mousex,mousey);
			DBuff();
		}
}

uint8_t console_manager(char *inst)
{
	if(!strcmp(inst,"help"))
	{
		printf("\nAvialable Commands are:\n");
		printf("\n\thelp         To get all the available commands");
		printf("\n\tmdbug        To test the Memory Managment System");
		printf("\n\tshutdown     To ACPI Power off the system (may not work on few machines)");
		printf("\n\tstart vesa   To enter VESA Super VGA Mode");
		printf("\n\tmemmap       To view the Physical Memory Map of the system");
		printf("\n\ttest multi   To Test multitasking");
		printf("\n\tls           To show the current directory's contents");
		printf("\n\tcd <dir>     To move to a directory ('0' for root, '..' to move to parent)");
		printf("\n\tcreate <type> <name>  To create a file or a directory ('file' for file, 'dir' for directory)\n");
		printf("\n\tedit <name> {<data>}  To edit a file");
		printf("\n\tdel <name>    To delete a file");
		printf("\n\tremove        To remove a directory");
		printf("\n\tother commands\n");
		return 0;
	}
	else if(!strcmp(inst,"shutdown"))
	{
			printf("\n Turning Power off");
			Switch_to_system_dir();
			acpiPowerOff();
			return 0;
	}
	else if(!strcmp(inst,"mdbug"))
	{
			printf("\n Testing Virtual Memory Manager");
			Switch_to_system_dir();
			mdbug();
			Switch_back_from_System();
			return 0;
	}
	else if(!strcmp(inst,"start vesa"))
	{
		printf("\n Entering VESA SVGA mode 1024*768");
		Switch_to_system_dir();
		vesa(0x117);
		Switch_back_from_System();
		return 1;
	}
	else if(!strcmp(inst,"memmap"))
	{
    printf("\nMemory Map:");
    MemRegion_t* memmap_info=mmap_info;
		Switch_to_system_dir();

    for(int i=0;i<15;i++)
    {
        if(memmap_info->startLo==0) break;
        printf("region %i address: %x size: %x Bytes Type: %i (%s)\n",i,memmap_info->startHi,memmap_info->sizeHi,
               memmap_info->type,strMemoryTypes[memmap_info->type-1]);
        memmap_info++;
    }
		Switch_back_from_System();
		return 0;
	}
	else if(!strcmp(inst,"test multi"))
	{
		//schedule=switch_task;
		//printf("\n\tEnter the timer Value:");
		//uint32_t timer=getint();
		Switch_to_system_dir();
		//Scheduler_exec();
		return 0;
	}
	printf("\n Command Not Recognized! type help for help\n");
	return 0;
}
