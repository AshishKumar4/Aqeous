#include "mouse.c"
#include "console.c"
#include "interrupts.c"
#include "multiboot.h"
#include "vfs.c"
#include "cmos.c"
#include "pci.c"
#include "ahci.c"
#include "timer.c"
#include "cpuid.c"
#include "mem.c"
#include "paging.c"
#include "ata.c"
#include "acpi.c"
#include "vmem.c"
#include "keyboard.c"
#include "task.c"
#include "fs.c"
#include "fs_alloc.c"

u32int initial_esp;
uint32_t initial_ebp;
u8int calle=0;

void dbug()
{
	int a[4];
	printf("\n\tEnter the size of: ");
	for(int i=0;i<4;i++)
	{
		printf("\n\t\tvar %x: ",i+1);
		a[i]=32;//getint();
	}
	uint32_t *temp1=(uint32_t*)malloc(a[0]),*temp2=temp1;
	//malloc(4096);
	uint32_t *test1=(uint32_t*)malloc(a[1]),*test2=test1;
	uint32_t *test3=(uint32_t*)malloc(a[2]);
	uint32_t *test4=(uint32_t*)malloc(a[3]);
	uint32_t *test5=(uint32_t*)malloc(1);
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
	free(test1);

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
	printf(" %x %x ",*test3,*test4);
	printf(" If you didnt saw any numbers above, It worked!!!\n");
	uint32_t *tmp1=(uint32_t*)malloc(a[0]),*tmp2=tmp1;
	//malloc(4096);
	uint32_t *tst1=(uint32_t*)malloc(a[1]),*tst2=tst1;
	printf("\tLocation of var 1: %x, var 2: %x \n",tmp1,tst1);
}

uint32_t initial_eip;

void kernel_main();

tss_struct_t *TSS;

void kernel_early(struct multiboot *mboot_ptr,u32int initial_stack)
{
	 //Kernel stack located at 60th mb to 70th mb
    initial_esp = 0x3C00000;
		console_init();
    init_descriptor_tables();
		int_init();
    printf("DESCRIPTOR TABLES INITIALIZED \n");
		mouseinit();
		printf("\nMouse Drivers initialized\n");
		keyboard_init();
		printf("\nKeyboard Drivers Initialized\n");
		tss_struct_t tss;
		TSS=&tss;
		gdt_set_gate(5,(uint32_t)&tss,sizeof(tss_struct_t),0x89,0x40);
	  gdt_flush((u32int)&gdt_ptr);
		tss.ss0=0x10;
		tss.esp0=initial_esp;
		tss.iomap=sizeof(tss_struct_t);
		printf("TSS setuped\n");
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
    mmap_info=(MemRegion_t*)mmap;//+mboot_ptr->size;*/
    Mapper();
		printf("\nEnabling ACPI!\n");
    initAcpi();
    if(!acpiEnable())
        printf("\nACPI Initialized\n");
    else printf("\nACPI CANT BE INITIALIZED\n");

		printf("\n\nEnumerating all devices on PCI BUS:\n");
		checkAllBuses();
		printf("\nEnabling Hard Disk\n");
		checkAHCI();
		if(!ahci_found)
			init_ata();
		//printf(" %x %x ",satatest,abcd);
		//printf(" %x %x %x  ",sizeof(unsigned short),sizeof(unsigned long),sizeof(unsigned long int));s
	//	while(1);
		printf("\nEnabling Paging\n");
		initialise_paging();
		enable_paging();
		MapPage((void*)mmap_info,(void*)mmap_info);
	//  map(AHCI_BASE,1024*1024*50);
	  map(&abar->ports[1],sizeof(HBA_PORT));
		printf("\n Paging Has been Enabled Successfully!");
		printf("\n Available Memory: %x KB\n",maxmem);
 	 	detect_cpu();
		initTasking();
		asm volatile("sti");

		init_cmos();
		printf("\nCMOS Clock Initialized\n");

   printf("\nLOADING MAIN KERNEL...\n");
	 mdbug=dbug;
	 vesa=setVesa;
	 printf("\nsize of task_t: %x",sizeof(task_t));/*
	 uint32_t a=0;
	 asm volatile("movl %%cr0, %0":"=r"(a));
	 printf("\n%x Protected Mode Enabled? ",a);
	 if(a & (1<<0))
	 	printint(1);*/
	 printf("\n\n\tType shutdown to do ACPI shutdown (wont work on certain systems)");
	 printf("\n\tType mdbug to test the Memory Manager");
	 printf("\n location of kernel_main: %x ",kernel_main);
	 printf("sizeof Directory_t %x, File_t %x",sizeof(Directory_t),sizeof(File_t));
	 Init_fs();
	 //Scheduler_exec();
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
			  /*uint32_t esp=0;
			  asm volatile("mov %%esp, %0":"=r"(esp)::"memory");
				IOLock(IRQ1);
				taskSleep();
				asm volatile("cli");
				printf("abc");*/
				getline(inst);
				printf(inst);
				flg=console_manager(inst);//*/
			//	asm volatile("sti");
    }
		while(1)
		{
			Mouse_Plot(mousex,mousey);
			DBuff();//*/
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
			acpiPowerOff();
			return 0;
	}
	else if(!strcmp(inst,"mdbug"))
	{
			printf("\n Testing Virtual Memory Manager");
			mdbug();
			return 0;
	}
	else if(!strcmp(inst,"start vesa"))
	{
		printf("\n Entering VESA SVGA mode 1024*768");
		vesa(0x117);
		return 1;
	}
	else if(!strcmp(inst,"memmap"))
	{
    printf("\nMemory Map:");
    MemRegion_t* memmap_info=mmap_info;

    for(int i=0;i<15;i++)
    {
        if(memmap_info->startLo==0) break;
        printf("region %i address: %x size: %x Bytes Type: %i (%s)\n",i,memmap_info->startHi,memmap_info->sizeHi,
               memmap_info->type,strMemoryTypes[memmap_info->type-1]);
        memmap_info++;
    }
		return 0;
	}
	else if(!strcmp(inst,"test multi"))
	{
		//schedule=switch_task;
		//printf("\n\tEnter the timer Value:");
		//uint32_t timer=getint();
		Scheduler_exec();
		return 0;
	}
	//else if(!strcmp(inst,""))
	else if(!strcmp(inst,"rand-gen"))
	{
		rand_test();
		return 0;
	}
	printf("\n Command Not Recognized! type help for help\n");
	return 0;
}
