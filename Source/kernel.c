#include <mouse.c>
//#include <kheap.c>
#include <console.c>
#include <interrupts.c>
#include "multiboot.h"
#include "vfs.c"
#include "initrd.c"
#include <timer.c>

extern u32int placement_address;
void timertest()
{
    RectD(50,100,20,30,1000,1000,1000);
}
void kernel_early(struct multiboot *mboot_ptr)
{
    //end1=end;
    //placement_address=(u32int)kmalloc(1);
    console_init();
    init_descriptor_tables();
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    fs_root = initialise_initrd(initrd_location); fs_root = initialise_initrd(initrd_location);

    // list the contents of /
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0)
    {
        console_writestring("Found file ");
        console_writestring(node->name);
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);

        if ((fsnode->flags&0x7) == FS_DIRECTORY)
        {
            console_writestring("\n\t(directory)\n");
        }
        else
        {
            console_writestring("\n\t contents: \"");
            char buf[256];
            u32int sz = read_fs(fsnode, 0, 256, buf);
            int j;
            for (j = 0; j < sz; j++)
                console_write_dec((unsigned int)buf[j]);

            console_writestring("\"\n");
        }
        i++;
    }
    //initialise_paging();
    console_writestring("HELLO WORLD");
    init_timer(1); //PIT WORKING
   // init_timer_RTC();
    mouseinit();
    setVesa(0x117);
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
        mouse_handler();
        Mouse_Plot(mousex,mousey);
        DBuff();
    }
   // u32int *ptr = (u32int*)0xA0000000;
   // u32int do_page_fault = *ptr;*/
}
