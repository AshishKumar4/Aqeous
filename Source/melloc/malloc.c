
#include <stddef.h>

#include <system.h>
#include <stdio.h>

// memset, sets a range of memory
#include <string.h>
/** MEMORY ALLOCATION **/
// a memory block
struct MemBlock_t {
	unsigned int		used : 1;
	unsigned int		size;
	struct MemBlock_t*	next;
};
typedef struct MemBlock_t MemBlock;

typedef struct multiboot_header
{
	unsigned long magic;
	unsigned long flags;
	unsigned long checksum;
	unsigned long header_addr;
	unsigned long load_addr;
	unsigned long load_end_addr;
	unsigned long bss_end_addr;
	unsigned long entry_addr;
} multiboot_header_t;

/* The symbol table for a.out. */
typedef struct aout_symbol_table
{
	unsigned long tabsize;
	unsigned long strsize;
	unsigned long addr;
	unsigned long reserved;
} aout_symbol_table_t;

/* The section header table for ELF. */
typedef struct elf_section_header_table
{
	unsigned long num;
	unsigned long size;
	unsigned long addr;
	unsigned long shndx;
} elf_section_header_table_t;

/* The Multiboot information. */
typedef struct multiboot_info
{
	unsigned long flags;
	unsigned long mem_lower;
	unsigned long mem_upper;
	unsigned long boot_device;
	unsigned long cmdline;
	unsigned long mods_count;
	unsigned long mods_addr;
	union
	{
		aout_symbol_table_t aout_sym;
		elf_section_header_table_t elf_sec;
	} u;
	unsigned long mmap_length;
	unsigned long mmap_addr;
} multiboot_info_t;

/* The module structure. */
typedef struct module
{
	unsigned long mod_start;
	unsigned long mod_end;
	unsigned long string;
	unsigned long reserved;
} module_t;

/* The memory map. Be careful that the offset 0 is base_addr_low
but no size. */
typedef struct memory_map
{
	unsigned long size;
	unsigned long base_addr_low;
	unsigned long base_addr_high;
	unsigned long length_low;
	unsigned long length_high;
	unsigned long type;
} memory_map_t;

// the location of the end of the kernel's image
extern char end;

// the kernel heap
static MemBlock* kheap;

// global multiboot info pointer
multiboot_info_t* MultiBootHeader;

// kernel's malloc
void* kmalloc( unsigned int size )
{
	// blocks for use later
	MemBlock* mblk, *newblk;
	char* cblk;
	unsigned int size2;

	// we can't allocate nothing!
	if( size == 0 )
		return NULL;

	// the real size
	size2 = size + sizeof( MemBlock );

	// is there a heap?
	if( kheap == (MemBlock*) NULL )
	{
		// set it to the end of the image
		kheap = mblk = (MemBlock*) &end;

		// set it up properly
		mblk->used = 0;
		mblk->size = ( 4 * 1024 * 1024 *1024) - sizeof( MemBlock ) - (uint_t) &end; // we allocate within the full memory available
		mblk->next = (MemBlock*) NULL;
	}

	// try to find a free block
	for( mblk = kheap; mblk != (MemBlock*) NULL; mblk = mblk->next )
	{
		if( ( !mblk->used ) && ( size2 <= mblk->size ) )
			break;
	}

	// see if one was found
	if( mblk == (MemBlock*) NULL )
		return NULL;

	// set up the block
	cblk = (char*) mblk + size2;
	newblk = (MemBlock*) cblk;

	// point to a new empty block, and init it
	newblk->used = 0;
	newblk->size = mblk->size - size2;
	newblk->next = mblk->next;

	// point to old block and init it
	mblk->used = 1;
	mblk->size = size;
	mblk->next = newblk;

	// return the pointer to the memory
	cblk = (char*) mblk;
	return cblk + sizeof( MemBlock );
}

// kernel's free
void kfree( void* block )
{
	// blocks
	MemBlock* mblk;
	char* cblk;

	// get the real block address
	cblk = (char*) block - sizeof( MemBlock );

	// find this address in the list
	for( mblk = kheap; mblk != (MemBlock*) NULL; mblk = mblk->next )
	{
		if( mblk == (MemBlock*) cblk )
			break;
	}

	// bad heap pointer
	if( mblk == (MemBlock*) NULL )
	{
		return;
	}

	// free the block
	mblk->used = 0;

	// this is the hard part: unlinking this block
	for( mblk = kheap; mblk != (MemBlock*) NULL; mblk = mblk->next )
	{
		// find one that isn't used
		while( ( !mblk->used ) && ( mblk->next != (MemBlock*) NULL ) && ( !mblk->next->used ) )
		{
			// resize this block
			mblk->size = (int) mblk->next - (int) mblk + mblk->next->size;

			// merge with the next block
			mblk->next = mblk->next->next;
		}
	}
}

// kernel's realloc
void* krealloc( void* blk, unsigned int size )
{
	// blocks
	MemBlock* mblk;
	void* newblk;
	char* cblk;

	// check to see if the block we're given is valid
	if( blk == (MemBlock*) NULL )
		return kmalloc( size );

	// check for a valid size, if invalid free the block
	if( size == 0 )
		kfree( blk );

	// allocate the new block
	newblk = kmalloc( size );

	// check that it's valid
	if( newblk == (MemBlock*) NULL )
		return NULL;

	// point to the data in the old block
	cblk = (char*) blk - sizeof( MemBlock );
	mblk = (MemBlock*) cblk;

	// copy the data
	memcpy( newblk, blk, mblk->size );

	// release the old block
	kfree( blk );

	// return it
	return newblk;
}

struct __freelist {
	size_t sz;
	struct __freelist *nx;
};


char *g_heapstart, *g_heapend;
#define __malloc_heap_start g_heapstart
#define __malloc_heap_end   g_heapend

char *__brkval;
struct __freelist *__flp;

void *
malloc(size_t len)
{
	struct __freelist *fp1, *fp2;
	char *cp;
	size_t s, avail;

	/*
	 * Our minimum chunk size is the size of a pointer (plus the
	 * size of the "sz" field, but we don't need to account for
	 * this), otherwise we could not possibly fit a freelist entry
	 * into the chunk later.
	 */
	if (len < sizeof(struct __freelist) - sizeof(size_t))
		len = sizeof(struct __freelist) - sizeof(size_t);

	/*
	 * First, walk the free list and try finding a chunk that
	 * would match exactly.  If we found one, we are done.  While
	 * walking, note down the size of the largest chunk we found
	 * that would still fit the request -- we need it for step 2.
	 *
	 */
	for (s = 0, fp1 = __flp, fp2 = 0;
	     fp1;
	     fp2 = fp1, fp1 = fp1->nx) {
		if (fp1->sz == len) {
			/*
			 * Found it.  Disconnect the chunk from the
			 * freelist, and return it.
			 */
			if (fp2)
				fp2->nx = fp1->nx;
			else
				__flp = fp1->nx;
			return &(fp1->nx);
		}
		if (fp1->sz > len) {
			if (s == 0 || fp1->sz < s)
				s = fp1->sz;
		}
	}
	/*
	 * Step 2: If we found a chunk on the freelist that would fit
	 * (but was too large), look it up again and use it, since it
	 * is our closest match now.  Since the freelist entry needs
	 * to be split into two entries then, watch out that the
	 * difference between the requested size and the size of the
	 * chunk found is large enough for another freelist entry; if
	 * not, just enlarge the request size to what we have found,
	 * and use the entire chunk.
	 */
	if (s) {
		if (s - len < sizeof(struct __freelist))
			len = s;
		for (fp1 = __flp, fp2 = 0;
		     fp1;
		     fp2 = fp1, fp1 = fp1->nx) {
			if (fp1->sz == s) {
				if (len == s) {
					/*
					 * Use entire chunk; same as
					 * above.
					 */
					if (fp2)
						fp2->nx = fp1->nx;
					else
						__flp = fp1->nx;
					return &(fp1->nx);
				}
				/*
				 * Split them up.  Note that we leave
				 * the first part as the new (smaller)
				 * freelist entry, and return the
				 * upper portion to the caller.  This
				 * saves us the work to fix up the
				 * freelist chain; we just need to
				 * fixup the size of the current
				 * entry, and note down the size of
				 * the new chunk before returning it
				 * to the caller.
				 */
				cp = (char *)fp1;
				s -= len;
				cp += s;
				fp2 = (struct __freelist *)cp;
				fp2->sz = len;
				fp1->sz = s - sizeof(size_t);
				return &(fp2->nx);
			}
		}
	}

	/*
	 * Step 3: If the request could not be satisfied from a
	 * freelist entry, just prepare a new chunk.  This means we
	 * need to obtain more memory first.  The largest address just
	 * not allocated so far is remembered in the brkval variable.
	 * Under Unix, the "break value" was the end of the data
	 * segment as dynamically requested from the operating system.
	 * Since we don't have an operating system, just make sure
	 * that we don't collide with the stack.
	 */
	if (__brkval == 0)
		__brkval = __malloc_heap_start;
	cp = __malloc_heap_end;
#if 0
	if (cp == 0)
		cp = STACK_POINTER() - __malloc_margin;
#endif
	avail = cp - __brkval;

	/*
	 * Both tests below are needed to catch the case len >= 0xfffe.
	 */
	if (avail >= len && avail >= len + sizeof(size_t)) {
		fp1 = (struct __freelist *)__brkval;
		__brkval += len + sizeof(size_t);
		fp1->sz = len;
		return &(fp1->nx);
	}

	/*
	 * Step 4: There's no help, just fail. :-/
	 */
	return 0;
}

void
free(void *p)
{
	struct __freelist *fp1, *fp2, *fpnew;
	char *cp1, *cp2, *cpnew;

	/* ISO C says free(NULL) must be a no-op */
	if (p == 0)
		return;

	cpnew = p;
	cpnew -= sizeof(size_t);
	fpnew = (struct __freelist *)cpnew;
	fpnew->nx = 0;

	/*
	 * Trivial case first: if there's no freelist yet, our entry
	 * will be the only one on it.
	 */
	if (__flp == 0) {
		__flp = fpnew;
		return;
	}

	/*
	 * Now, find the position where our new entry belongs onto the
	 * freelist.  Try to aggregate the chunk with adjacent chunks
	 * if possible.
	 */
	for (fp1 = __flp, fp2 = 0;
	     fp1;
	     fp2 = fp1, fp1 = fp1->nx) {
		if (fp1 < fpnew)
			continue;
		cp1 = (char *)fp1;
		fpnew->nx = fp1;
		if ((char *)&(fpnew->nx) + fpnew->sz == cp1) {
			/* upper chunk adjacent, assimilate it */
			fpnew->sz += fp1->sz + sizeof(size_t);
			fpnew->nx = fp1->nx;
		}
		if (fp2 == 0) {
			/* new head of freelist */
			__flp = fpnew;
			return;
		}
		break;
	}
	/*
	 * Note that we get here either if we hit the "break" above,
	 * or if we fell off the end of the loop.  The latter means
	 * we've got a new topmost chunk.  Either way, try aggregating
	 * with the lower chunk if possible.
	 */
	fp2->nx = fpnew;
	cp2 = (char *)&(fp2->nx);
	if (cp2 + fp2->sz == cpnew) {
		/* lower junk adjacent, merge */
		fp2->sz += fpnew->sz + sizeof(size_t);
		fp2->nx = fpnew->nx;
	}
}
