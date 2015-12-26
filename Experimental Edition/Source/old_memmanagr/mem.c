#include <mem.h>

void Init_mem(int mem_upper,u32int bitmap)
{
    memory_size=mem_upper;
    max_blocks = memory_size/4;
	memory_map	=	(uint32_t*) bitmap;
	used_blocks	=	max_blocks;

	//! By default, all of memory is in use
	memset (memory_map, 0xf, max_blocks );

}

inline unsigned int KBtoB(unsigned int kb)
{
    return kb*1024;
}

inline unsigned int blocks(unsigned int bytes) //Returns number of bytes given converted to descreate blocks of 4 kb
{
   /* int blocks=1;
    int temp=bytes;
    for(int i=0;;i++)
    {
        if(temp<=4096) return blocks;
        else
        {
            temp=temp-4096;
            blocks+=10;
        }
    }*/
    return bytes;
}

inline void mmap_set (int bit)
{
    memory_map[bit / 32] |= (1 << (bit % 32));
}

inline void mmap_unset (int bit)
{
    memory_map[bit / 32] &= ~ (1 << (bit % 32));
}

inline bool mmap_test (int bit)
{
    return memory_map[bit / 32] &  (1 << (bit % 32));
}

int mmap_first_free ()
{
	//! find the first free bit
	for (uint32_t i=0; i< max_blocks / 32; i++)
		if (memory_map[i] != 0xffffffff)
			for (int j=0; j<32; j++) {		//! test each bit in the dword

				int bit = 1 << j;
				if (! (memory_map[i] & bit) )
					return i*4*8+j;
			}

	return -1;
}

int mmap_first_free_s (size_t size)
{

	if (size==0)
		return -1;

	if (size==1)
		return mmap_first_free ();

	for (uint32_t i=0; i<max_blocks /32; i++)
		if (memory_map[i] != 0xffffffff)
			for (int j=0; j<32; j++) {	//! test each bit in the dword

				int bit = 1<<j;
				if (! (memory_map[i] & bit) ) {

					int startingBit = i*32;
					startingBit+=bit;		//get the free bit in the dword at index i

					uint32_t free=0; //loop through each bit to see if its enough space
					for (uint32_t count=0; count<=size;count++) {

						if (! mmap_test (startingBit+count) )
							free++;	// this bit is clear (free frame)

						if (free==size)
							return i*4*8+j; //free count==size needed; return index
					}
				}
			}

	return -1;
}

void pmmngr_init_region (u32int base, size_t size)
{

	int align = base / PMMNGR_BLOCK_SIZE;
	int blocks = size / PMMNGR_BLOCK_SIZE;

	for (; blocks>0; blocks--)
    {
		mmap_unset (align++);
		used_blocks--;
	}

	mmap_set (0);	//first block is always set. This insures allocs cant be 0
}

void pmmngr_deinit_region (u32int base, size_t size)
 {

	int align = base / PMMNGR_BLOCK_SIZE;
	int blocks = size / PMMNGR_BLOCK_SIZE;

	for (; blocks>0; blocks--)
    {
		mmap_set (align++);
		used_blocks++;
	}
}

void* kmalloc1 ()
{

	if (max_blocks-used_blocks <= 0)
		return 0;	//out of memory

	int frame = mmap_first_free();

	if (frame == -1)
		return 0;	//out of memory

	mmap_set(frame);

	u32int addr = frame * PMMNGR_BLOCK_SIZE;
	used_blocks++;

	return (void*)addr;
}

void*	kmalloc (size_t size)
{
    size=blocks(size);
	if (max_blocks-used_blocks <= size)
		return 0;	//not enough space

	int frame = mmap_first_free_s(size);

	if (frame == -1)
		return 0;	//not enough space

	for (uint32_t i=0; i<size; i++)
		mmap_set (frame+i);

	u32int addr = frame * PMMNGR_BLOCK_SIZE;
	used_blocks+=size;

	return (void*)addr;
}

void kfree1 (void* p)
{
    u32int addr = (u32int)p;
	int frame = addr / PMMNGR_BLOCK_SIZE;

	mmap_unset (frame);

	used_blocks--;
}
void kfree(void* p, size_t size)
{

	u32int addr = (u32int)p;
	int frame = addr / PMMNGR_BLOCK_SIZE;

	for (uint32_t i=0; i<size; i++)
		mmap_unset (frame+i);

	used_blocks-=size;
}
