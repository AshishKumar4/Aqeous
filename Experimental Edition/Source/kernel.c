#include "mouse.c"
#include "console.c"
#include "descriptors.c"
#include "int_handlers.c"
#include "cpu.c"
#include "multiboot.h"
//#include "vfs.c"
#include "cmos.c"
#include "pci.c"
#include "ahci.c"
#include "graphics.c"
#include "timer.c"
#include "apic.c"
#include "cpuid.c"
#include "phy_mm/mem.c"
#include "virt_mm/paging.c"
#include "ata.c"
#include "acpi.c"
#include "virt_mm/vmem.c"
#include "keyboard.c"
#include "task.c"
#include "tasking.c"
#include "process.c"
#include "hpet.c"
#include "fs.c"
#include "fs_alloc.c"
#include "Scheduler.c"
#include "Shell.c"
#include "kb_handle.c"
#include "parallels.c"
#include "serials.c"
#include "std_iohandling.c"
#include "FS_Handling.c"
#include "Qalloc.c"
#include "queues.c"
#include "vesa.c"
#include "pic.c"
#include "fonts.c"
#include "memfunc.c"
#include "math.c"

//#include "std_fs.c"
#include "ext2\ext2_fs.c"

uint32_t initial_esp;
uint32_t initial_ebp;
uint8_t calle=0;

void dbug()
{
	uint32_t *temp1=(uint32_t*)kmalloc(810),*temp2=temp1;
	*temp1=4284;
	//kmalloc(4096);
	uint32_t *test1=(uint32_t*)kmalloc(1024),*test2=test1;
	uint32_t *test3=(uint32_t*)kmalloc(1024*1024);
	uint32_t *test4=(uint32_t*)kmalloc(432);
	//printf("Ax%x",kmalloc(1024*1024*40));
	uint32_t *test5=(uint32_t*)kmalloc(128);

	printf("\n\tLocation of var 1: %x, var 2: %x var 3: %x var 4: %x var 5: %x \n",temp1,test1,test3,test4,test5);
//	printf("\nvar1->special: %x, var2->esp: %x", &((task_t*)temp1)->special, &((task_t*)test1)->esp);
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
	kfree(test1);
	kfree(temp2);
	//free(test3);
///*
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
	uint32_t *tmp1=(uint32_t*)kmalloc(32),*tmp2=tmp1;
	//kmalloc(4096);
	uint32_t *tst1=(uint32_t*)kmalloc(123),*tst2=tst1;

	printf("\tLocation of var 1: %x, var 2: %x %x\n",tmp1,tst1,kmalloc(12));
}

uint32_t initial_eip;

void kernel_main();

tss_struct_t *TSS;

void kernel_early(struct multiboot *mboot_ptr,uint32_t initial_stack)
{
	console_init();
	//setVesa(0x117);
	init_APIC();
	init_descriptor_tables();
	printf("\nDESCRIPTOR TABLES INITIALIZED \n");
	printf("\nEnabling ACPI!\n");
	initAcpi();
	if(!acpiEnable())
		printf("\nACPI Initialized\n");
	else printf("\nACPI CANT BE INITIALIZED\n");
		ioapic_init();

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
			mmap_info->reservedt = 0xFFE42;
			mmap_info++;
	}
	printf("\nInitializing Memory Manager!\n");
	mmap_info=(MemRegion_t*)mmap;//+mboot_ptr->size;
	max_mem=maxmem*1024;

	/*Mapper();

	printf("\nMemory block lists prepared!\n");
	bitmap_init();
	initialise_paging();
	enable_paging();
	switch_pdirectory(system_dir);*/
	Setup_Paging();
	printf("\n Paging Has been Enabled Successfully!");
	printf("\n Available Memory: %x KB\n",maxmem);
/*
	printf("C %x D %x %x", phy_alloc4K(),phy_alloc4K(), phy_alloc4K());

	CustomCSRB_M_header_t* cst = system_pdirCap->csrb_f;
	CustomCSRB_M_t* tmp = cst->head;

	for(int i=0; i < 20; i++)
	{
		tmp = tmp->addr;
		printf("A%x-%x  ",tmp->begin,tmp->size);
		++tmp;
	}
*/

//	dbug();
//	while(1);

	printf("\n\nEnumerating all devices on PCI BUS:\n");
	checkAllBuses();
	printf("\nEnabling Hard Disk\n");
	checkAHCI();
//	if(!ahci_found)
	//	init_ata();

	detect_cpu();

	printf("\nLOADING MAIN KERNEL...\n");
	mdbug=dbug;
	vesa=setVesa;

	printf("\n\n\tType shutdown to do ACPI shutdown (wont work on certain systems)");
	printf("\n\tType mdbug to test the Memory Manager");
//while(1);
	Init_fs();
	//while(1);
	printf("\nsize of HPET_Table_t: %x",sizeof(HPET_Table_t));
	if(cpuHasMSR()) printf("\nCPU has MSR");
	//switch_pdirectory(main_dir);
	
	console_dbuffer_original = kmalloc(4096*1024);
	console_dbuffer = (uint16_t*)console_dbuffer_original;
	console_dbuffer_limit = console_dbuffer_original + 4194304;
	printf("\n\nInitializing MultiThreading System");
	init_multitasking();//*/
	while(1);

//	RectL(0,0,1200,900,90,90,90);
	//We shall never get back here, Not until the universe ends. ;)
}

void kernel_start()
{
}

void kernel_main()
{
	while(1);
	/*
		printf("\n");
		char *inst=" ";
		uint8_t flg=0;
		//while(1);
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
		}//*/
}
