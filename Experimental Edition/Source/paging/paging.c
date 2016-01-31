#include "paging.h"
#include "mem.h"

extern void copy_page_physical(uint32_t,uint32_t);
// The kernel's page directory
page_directory_t *kernel_directory=0;

// The current page directory;
page_directory_t *current_directory=0;

// A bitset of frames - used or free.
u32int *frames;
u32int nframes;

// Defined in kheap.c
extern u32int placement_address;

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))
static inline void flush_tlb(unsigned long addr)
{
   asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}
// Static function to set a bit in the frames bitset
static void set_frame(u32int frame_addr)
{
    u32int frame = frame_addr/0x1000;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(u32int frame_addr)
{
    u32int frame = frame_addr/0x1000;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}
/*
// Static function to test if a bit is set.
static u32int test_frame(u32int frame_addr)
{
    u32int frame = frame_addr/0x1000;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}*/

uint32_t mem=1024*1024*2;

uint32_t fframe=0;
// Static function to find the first free frame.
static u32int first_frame()
{
    uint32_t i,j;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
    {
        if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
        {
            // at least one bit is free here.
            for (j = 0; j < 32; j++)
            {
                u32int toTest = 0x1 << j;
                if ( !(frames[i]&toTest) )
                {
                    return i*4*8+j;
                }
            }
        }
    }
    return 0;
}
uint32_t phyaddr=0;
// Function to allocate a frame.
void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
        u32int idx = first_frame();
        if (idx == (u32int)-1)
        {
            printf("No space left\n");
            // PANIC! no free frames!!
        }
        set_frame(idx*0x1000);
        page->present = 1;
        page->rw = (is_writeable==1)?1:0;
        page->user = (is_kernel==1)?0:1;
        page->frame = idx;
}

// Function to deallocate a frame.
void free_frame(page_t *page)
{
    u32int frame;
    if (!(frame=page->frame))
    {
        return;
    }
    else
    {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

void initialise_paging()
{
    // The size of physical memory. For the moment we
    // assume it is 16MB big.
    u32int mem_end_page=(0xFFFFFFFF);
/**/
    nframes = mem_end_page / 0x1000;
    frames = (u32int*)kmalloc(nframes/8);
    memset(frames, 0, nframes/8);
    // Let's make a page directory.
    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    memset(kernel_directory, 0, sizeof(page_directory_t));
    kernel_directory->physicalAddr = (u32int)kernel_directory->tablesPhysical;
}

void enable_paging()
{
    printf("Allocating Pages and Page tables, This may take a while\n");
    tempBlock3=Mblock;
    for(uint32_t i=0;i<(1024*1024*100);i+=4096) //Make the pages and page tables for the whole usable memory
    {
        page_t* page=get_page(i,1,kernel_directory); //kernel Pages.
        page->present=1;
        page->rw=1;
        page->user=0;
        page->frame=i/4096;
        set_frame(i);
        tempBlock3->page=(uint32_t*)page;
        tempBlock3=(MemMap_t*)tempBlock3->link;
    }
    for(uint32_t i=1024*1024*100;i<(1024*maxmem);i+=4096) //Make the pages and page tables for the usable memory
    {
        page_t* page=get_page(i,1,kernel_directory); //user Pages.
        page->present=1;
        page->rw=1;
        page->user=1;
        page->frame=1024*1024*70/4096; //make the page point to 70th mb. If in case some un-allocated memory is used, it would go here.
        tempBlock3->page=(uint32_t*)page;
        tempBlock3=(MemMap_t*)tempBlock3->link;
    }
    for(uint32_t i=1024*maxmem;i<(1024*1024*4);i+=4096) //Make the pages and page tables for the restof the memory as Identity
    {
        page_t* page=get_page(i,1,kernel_directory); //user Pages.
        page->present=1;
        page->rw=1;
        page->user=0;
        if(tempBlock3->used==0) //if the address is in available memory
          page->frame=1024*1024*70/4096; //make the page point to 70th mb. If in case some un-allocated memory is used, it would go here.
        else
        {
          page->frame=i/4096; //the address is reserved, make it identity mapped
          set_frame(i);
        }
        tempBlock3->page=(uint32_t*)page;
        tempBlock3=(MemMap_t*)tempBlock3->link;
    }//*/
    switch_page_directory(kernel_directory);
    register_interrupt_handler(14, page_fault);
    pag=1;
    printf("\nenabled paging\n");
    /*current_directory = clone_directory(kernel_directory);
    switch_page_directory(current_directory);*/
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(dir->physicalAddr));
    u32int cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir)
{
    // Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    uint32_t table_idx = address / 1024;

    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        uint32_t tmp;
        dir->tables[table_idx] = (page_table_t*)pmalloc(sizeof(page_table_t), &tmp);
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
        printf(" Cant get the Page! ");
        return 0;
    }
}


void page_fault(registers_t regs)
{
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    // The error code gives us details of what happened.
    int present   = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;           // Write operation?
    int us = regs.err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

    // Output an error message.
    console_writestring("Page fault! ( ");
    if (present) {console_writestring("present ");}
    if (rw)
    {
        console_writestring("read-only ");
    }
    if (us) {console_writestring("user-mode ");}
    if (reserved) {console_writestring("reserved ");}

    if (id) {console_writestring("id "); console_write_dec(id);}
    console_writestring(") at 0x");
    console_write_dec(faulting_address);
    console_writestring(" - EIP: ");
    console_write_dec(regs.eip);
    console_writestring("\n");
    return;
   // PANIC("Page fault");
}

static page_table_t *clone_table(page_table_t *src, u32int *physAddr)
{
    // Make a new page table, which is page aligned.
    page_table_t *table = (page_table_t*)kmalloc_ap(sizeof(page_table_t), physAddr);
    // Ensure that the new table is blank.
    memset(table, 0, sizeof(page_directory_t));

    // For every entry in the table...
    int i;
    for (i = 0; i < 1024; i++)
    {
        // If the source entry has a frame associated with it...
        if (src->pages[i].frame)
        {
            // Get a new frame.
            alloc_frame(&table->pages[i], 0, 0);
            // Clone the flags from source to destination.
            if (src->pages[i].present) table->pages[i].present = 1;
            if (src->pages[i].rw) table->pages[i].rw = 1;
            if (src->pages[i].user) table->pages[i].user = 1;
            if (src->pages[i].accessed) table->pages[i].accessed = 1;
            if (src->pages[i].dirty) table->pages[i].dirty = 1;
            // Physically copy the data across. This function is in process.s.
            copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
        }
    }
    return table;
}

page_directory_t *clone_directory(page_directory_t *src)
{
    u32int phys;
    // Make a new page directory and obtain its physical address.
    page_directory_t *dir = (page_directory_t*)kmalloc_ap(sizeof(page_directory_t), &phys);
    // Ensure that it is blank.
    memset(dir, 0, sizeof(page_directory_t));

    // Get the offset of tablesPhysical from the start of the page_directory_t structure.
    u32int offset = (u32int)dir->tablesPhysical - (u32int)dir;

    // Then the physical address of dir->tablesPhysical is:
    dir->physicalAddr = phys + offset;

    // Go through each page table. If the page table is in the kernel directory, do not make a new copy.
    int i;
    for (i = 0; i < 1024; i++)
    {
        if (!src->tables[i])
            continue;

        if (kernel_directory->tables[i] == src->tables[i])
        {
            // It's in the kernel, so just use the same pointer.
            dir->tables[i] = src->tables[i];
            dir->tablesPhysical[i] = src->tablesPhysical[i];
        }
        else
        {
            // Copy the table.
            u32int phys;
            dir->tables[i] = clone_table(src->tables[i], &phys);
            dir->tablesPhysical[i] = phys | 0x07;
        }
    }
    return dir;
}

void map(u32int phy,size_t size)
{
    unsigned int j=phy;
    for (; j < phy+size;j+=0x1000)
    {
        page_t *page=get_page(j, 1, kernel_directory); //get pointer on a page entry

        set_frame(j);
        page->present = 1;
        page->rw = 1;
        page->user = 1;
        page->frame = j/0x1000;
    }
}
