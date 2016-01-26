#include "mem.h"
#include "paging.h"
#include "vmem.h"
// end is defined in the linker script.
extern u32int end;
u32int placement_address = 4096*300;
extern page_directory_t *kernel_directory;

uint32_t BlockFinder(uint32_t addr)
{
    return (addr/4096)+Mblock;
}

uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys,int kernel)
{
    int mb=0;
    if(kernel) mb=30;
    else mb=100;
    tempBlock1=Kblock;
    uint32_t addr;
    for(int i=0;;i++)
    {
        addr=tempBlock1->addr;
        if(addr>=mb*1024*1024&&tempBlock1->used==0)
        {
            if (align == 1 && (addr & 0xFFFFF000)) //if alignment required
            {
                in:
                addr&=0xFFFFF000; //get an aligned address.
                addr+=0x1000;
                while(addr<=tempBlock1->addr) //check if the aligned address is in some block.
                {
                    ++tempBlock1;
                }
                if(tempBlock1->used==0)
                {
                    break; //if that block is unused, return the address.
                }
                else goto in; //else search for some more such blocks
            }
            else
            {
                break; //used
            }
        }
        else if(tempBlock1->addr>=maxmem*1024)
        {
            printf("\nNo memory");
            return 0;
        }
        ++tempBlock1;
    }
    if(phys)
        *phys=addr;
    if(pag==0) //If paging not enabled
        for(int i=0;i<=(sz-1)/4096;i++,tempBlock1++) tempBlock1->used=10;
    else
    {
        for(int i=0;i<=(sz-1)/4096;i++,tempBlock1++)
        {
            tempBlock1->used=10;
            page_t* page=tempBlock1->page;
            page->present=1;
            page->rw=1;
            page->user=1;
            page->frame=tempBlock1->addr/4096;
        }
        switch_page_directory(kernel_directory);
    }
    return addr;
}

void kfree(void *p)
{

}

uint32_t kmalloc_a(uint32_t sz)
{
    return kmalloc_int(sz, 1, 0,1);
}

uint32_t kmalloc_p(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 0, phys,1);
}

uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 1, phys,1);
}

uint32_t pmalloc_ap(uint32_t sz, uint32_t *phys)
{
    return kmalloc_int(sz, 1, phys,1);
}

uint32_t kmalloc(uint32_t sz)
{
    return kmalloc_int(sz, 0, 0,1);
}

uint32_t malloc(uint32_t sz)
{
    return kmalloc_int(sz, 0, 0,0);
}
