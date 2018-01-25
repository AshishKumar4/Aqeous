#include "LIBRARY.c"

#include "mouse.c"
#include "console.c"
#include "descriptors.c"
#include "int_handlers.c"
#include "cpu/cpu.c"
#include "multiboot.h"
//#include "multiboot2.h"
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
#include "Keyboard.c"
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

#include "Aqfs.c"
#include "ext2/ext2_fs.c"

#include "RandomLib/Random.c"
#include "lodepng/lodepng.c"

#include "NeuralNetwork/Neuron/NeuralProcessing.c"

#include "IPCInterface/IPCInterface.c"
#include "vfs.c"
#include "WindowSystem/window.c"
#include "Water/Water.c"
#include "Interpolation/Interpolator.c"
#include "Processing/ELF/Elf.c"
#include "Processing/processing.c"
#include "IO_Handling/IO_Devices/Console/Console_handle.c"

#include "Processing/LibSymTable/LibSymTable.c"
#include "Processing/SysCalls/SysCalls.c"
#include "ThreadTable.c"


uint32_t initial_esp;
uint32_t initial_ebp;
uint8_t calle=0;

void idle_test()
{

}

void kernel_early(struct multiboot *mboot_ptr)
{
	PIT_Task = &idle_test;
	mboot_struct = mboot_ptr;
	console_init();

	/*printf("\n%d, num: %d, size: %d, vbe_mode: %d, vbe_mode_info: %d", mboot_struct, mboot_struct->num, mboot_struct->size, mboot_struct->vbe_mode, mboot_struct->vbe_mode_info );
	multiboot_header_t* tmb = mboot_ptr;
	printf("\n%d, %d",tmb->magic, mboot_struct->mem_upper);*/

	UNLOCK(printlock);
	Screen_BuffSync= idle_test;

	//init_descriptor_tables();	// Setup Descriptor tables for BSP

	detect_cpu();
	BasicCPU_Init();		// Initialize all the processing units in the System
	Enable_SSE();

	printf("\nDESCRIPTOR TABLES INITIALIZED \n");
	printf("\nEnabling ACPI!\n");
	initAcpi();
	if(!acpiEnable())
		printf("\nACPI Initialized\n");
	else printf("\nACPI CANT BE INITIALIZED\n");

	keyboard_init();
	printf("\nKeyboard Drivers Initialized\n");

	printf("\nAvailable memory: ");
	printint(mboot_ptr->mem_upper);
	memAvailable=mboot_ptr->mem_upper;
	uint32_t *mmap=(uint32_t*)mboot_ptr->mmap_addr;
	mmap_info=(MemRegion_t*)mmap;//+mboot_ptr->size;
	maxmem=memAvailable;

	/*for(int i=0;i<20;i++)
	{
		if(mmap_info->startLo==0) break;
			printf("region %i address: %x size: %x Bytes Type: %i (%s)\n",i,mmap_info->startHi,mmap_info->sizeHi,
			mmap_info->type,strMemoryTypes[mmap_info->type-1]);
			mmap_info->reservedt = 0xFFE42;
			mmap_info++;
	}*/
	printf("\nInitializing Memory Manager!\n");
	mmap_info=(MemRegion_t*)mmap;//+mboot_ptr->size;
	max_mem=maxmem*1024;

	Setup_Paging();

	printf("\n Paging Has been Enabled Successfully!");
	printf("\n Available Memory: %x KB\n",maxmem);

	Create_ScodeTables();
	
	printf("\n\nEnumerating all devices on PCI BUS:\n");
	checkAllBuses();
	//while(1);
	printf("\nEnabling Hard Disk\n");
	checkAHCI();

	vfs_init();

//*/
	find_MCFGtable();

	printf("\n\n\tType shutdown to do ACPI shutdown (wont work on certain systems)");

	
	printf("\nsize of HPET_Table_t: %x",sizeof(HPET_Table_t));
	if(cpuHasMSR()) printf("\nCPU has MSR");
	
	printf("\n\nInitializing MultiThreading System");
	init_timer(60);
	init_multitasking();//*/

	while(1);

	pmode_GDT_init(0);
	asm volatile("sti");
	clearIRQMask(0);
	clearIRQMask(1);
	asm volatile("int $55");
	//apic_start_timer(APIC_LOCAL_BASE);       //The respective Timer initialization function of the timer of choice
	//init_idt();
	while(1);
	//We shall never get back here, Not until the universe ends. ;)
}

void kernel_main()
{
	printf("\n\nHello from Userspace");
//	asm volatile("hlt");
	while(1);
}
