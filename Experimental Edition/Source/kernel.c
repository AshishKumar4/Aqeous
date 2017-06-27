#include "mouse.c"
#include "console.c"
#include "descriptors.c"
#include "int_handlers.c"
#include "cpu/cpu.c"
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
#include "ProcManager/ProcManager.c"
#include "MManager/mmanagerSys.c"
#include "hpet.c"
#include "fs.c"
#include "lowlvl_Lib/lib.c"
#include "fs_alloc.c"
#include "Scheduler/Scheduler.c"
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
#include "Intel_MP/mp.c"
#include "NeuralNetwork/Neuron/Neuron.c"

#include "LocalAPIC/lapic.c"

//#include "std_fs.c"
#include "ext2/ext2_fs.c"

#include "RandomLib/Random.c"

#include "NeuralNetwork/Neuron/NeuralProcessing.c"

#include "IPCInterface/IPCInterface.c"
#include "vfs.c"
#include "WindowSystem/window.c"
//#include "mathex.c"

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
		printf(" %d ",*temp1);
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
//	kfree(temp2);
	//free(test3);
///*
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
//	printf(" %x %x ",*test3,*test4);
	printf(" If you didnt saw any numbers above, It worked!!!\n");
	uint32_t *tmp1=(uint32_t*)kmalloc(32),*tmp2=tmp1;
	//kmalloc(4096);
	uint32_t *oooooopp=(uint32_t*)kmalloc(64),*tst2=oooooopp;

	printf("\tLocation of var 1: %x, var 2: %x %x\n",tmp1,oooooopp,kmalloc(12));//*/
}

uint32_t initial_eip;

void kernel_main();

tss_struct_t *TSS;

void kernel_early(struct multiboot *mboot_ptr,uint32_t initial_stack)
{
	console_init();

	UNLOCK(printlock);
	//setVesa(0x117);
	init_descriptor_tables();	// Setup Descriptor tables for BSP

	detect_cpu();
	BasicCPU_Init();		// Initialize all the processing units in the System

//	while(1);
	printf("\nDESCRIPTOR TABLES INITIALIZED \n");
	printf("\nEnabling ACPI!\n");
	initAcpi();
	if(!acpiEnable())
		printf("\nACPI Initialized\n");
	else printf("\nACPI CANT BE INITIALIZED\n");
	//*/
	//ioapic_init();
//*/
//	mouseinit();
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

	Setup_Paging();

	printf("\n Paging Has been Enabled Successfully!");
	printf("\n Available Memory: %x KB\n",maxmem);

/*
	Pdir_Capsule_t* dir = pgdir_maker();
	memset(&dir->pdir, 0, 4096);
	Kernel_Mapper(&dir->pdir);
	//memcpy(&dir->pdir, system_dir, 4*1024*1024);
	//printf("\n%d", pgdir);
	switch_pCap(dir);
	while(1);*/

	//	while(1);
	printf("\n\nEnumerating all devices on PCI BUS:\n");
	checkAllBuses();
	printf("\nEnabling Hard Disk\n");
	checkAHCI();

	printf("\nLOADING MAIN KERNEL...\n");
	mdbug=dbug;
	vesa=setVesa;

	printf("\n\n\tType shutdown to do ACPI shutdown (wont work on certain systems)");

	vfs_init();
	printf("\nsize of HPET_Table_t: %x",sizeof(HPET_Table_t));
//	if(cpuHasMSR()) printf("\nCPU has MSR");
	//switch_pdirectory(main_dir);

	console_dbuffer_original = kmalloc(4096*1024);
	console_dbuffer = (uint16_t*)console_dbuffer_original;
	console_dbuffer_limit = console_dbuffer_original + 4194304;
	printf("\n\nInitializing MultiThreading System");
	init_timer(60);
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
}
