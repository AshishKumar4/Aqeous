#include <common.h>
#include <ext2/ext2_fs.h>
#include "ahci.h"

int shiftData(void *position, int shiftAmount, uint32_t lengthOfDataToShift)
{
	//TODO add right shift functionalbility
	if(shiftAmount < 0) //user wants to shift to the left
	{
		uint32_t start = (uint32_t)position, end = (uint32_t)position + shiftAmount;

		int i; //sifts the data to the left
		for(i = 0; i < (int)lengthOfDataToShift; i++)
			*(char*)(end + i) = *(char*)(start + i);

		memset((uint32_t*)(end + i), 0, -1 * shiftAmount);

		//success!
		return 0;
	}
	return 1;
}

uint32_t ext2_read(ext2_inode_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
	//if the user can read it
	if(node->mode & _Rlogged)
	{

		//a size of 0 or an offset greater than the node's size is impossible
		if(!size || offset > node->size)
			return 0; //fail

		//make sure we cap the size
		if(size + offset > node->size)
			size = node->size - offset;

		uint32_t blocks_to_read = ((size - 1) / EXT2_BLOCK_SZ) + 1, i, out;

		for(i = 0; i < blocks_to_read; i++)
		{
			out = ext2_block_of_set(node, i, (uint32_t*)(buffer + i * EXT2_BLOCK_SZ));

			if(!out)
				return 0; //fail
		}

		//sucess!
		return size;
	}
	return 0;
}

uint32_t ext2_read_meta_data(ext2_superblock_t **sblock, ext2_group_descriptor_t **gdesc)
{
	ext2_superblock_t *sdata;

	//get the sblock data
	sdata = (ext2_superblock_t*)kmalloc(sizeof(ext2_superblock_t));
	disk_read(curr_port, (uint32_t)((EXT2_SBLOCK_OFF) / SECTOR_SIZE), sizeof(ext2_superblock_t), (uint32_t*)sdata);

	//uint32_t* test = (uint32_t*)sdata;
	//printf("Sizeof ext2_superblock_t = %x\n", sizeof(ext2_superblock_t));
	//check here if the superblock exists
	if(sdata->magic != EXT2_MAGIC)
		return 1; //fail!

	uint32_t nblockgroups = sdata->total_blocks / sdata->blocks_per_group;

	ext2_group_descriptor_t *gdata;

	//get the group descriptor data
	gdata = (ext2_group_descriptor_t*)kmalloc(nblockgroups * sizeof(ext2_group_descriptor_t));
	disk_read(curr_port, (EXT2_SBLOCK_OFF + EXT2_BLOCK_SZ) / SECTOR_SIZE, nblockgroups * sizeof(ext2_group_descriptor_t), (uint32_t*)gdata);

	*sblock = sdata;
	//~ printf("junk location %h, actual %h\n", *sblock, sdata);

	*gdesc = gdata;

	//TODO make this work with the mutltiple sblocks and gdescs due to possible multiple block groups
	//~ memcpy(ext2_g_sblock, sdata, sizeof(ext2_superblock_t));
	//~ memcpy(ext2_g_gdesc, gdata, sizeof(ext2_group_descriptor_t));

	ext2_g_sblock = sdata;
	ext2_g_gdesc = gdata;

	//Sucess!
	return 0;
}

uint32_t *ext2_format_block_bitmap(ext2_group_descriptor_t *gdesc, uint32_t blocks_used)
{
	uint32_t location = gdesc->block_bitmap;

	//the offset from the beginning of the file (the end of the inode table)
	uint32_t begining_offset = gdesc->inode_table_id + BLOCKS_TO_SECTORS(gdesc->inode_table_size);

	uint8_t *block_bitmap;

	if(!ext2_g_bb)
	{
		block_bitmap = (uint8_t*)kmalloc(EXT2_BLOCK_SZ);
		ext2_g_bb = block_bitmap;
		disk_read(curr_port, location, EXT2_BLOCK_SZ, (uint32_t*)block_bitmap);
	}else
		block_bitmap = ext2_g_bb;

	//if we do not want to allocate any blocks, return 0
	if(!blocks_used)
		return 0;

	uint32_t *output;
	output = (uint32_t*)kmalloc(blocks_used * sizeof(uint32_t));

	memset(output, 0x0, blocks_used * sizeof(uint32_t));

	//the number of consecutive blocks free
	uint32_t consec_free = 0;
	int32_t bit_off;

	//TODO this loop looks for consecutive blocks, if none are found implement non-consecutive block allocation
	uint32_t off, bit_mask;
	for(off = 0; off < EXT2_BLOCK_SZ; off++)
	{
		//go through all bit mask starting from 0b10000000 to 0b1 inclusive
		for(bit_mask = 0b10000000, bit_off = 0; bit_mask >= 0b1; bit_mask >>= 1, bit_off++)
		{
			//if the bit isolated by bit_mask is 0
			if(!(*(block_bitmap + off) & bit_mask))
				++consec_free; // was consec_free++
			else //if the bit isolated by bit_mask is 1
				consec_free = 0;

			//we have found adequate space
			if(consec_free == blocks_used)
			{

				//offset output to the end of the place where we will be assigning block locations
				output += blocks_used;

				//going backwards
				for(consec_free; consec_free > 0; consec_free--)
				{
					//flip the current bit and go backwards
					*(block_bitmap + off) |= bit_mask;

					output--;
					//assign the offset in bits from the beginning
					*output = BLOCKS_TO_SECTORS((8 * off + bit_off)) + begining_offset;

					//decrement everything by one
					bit_mask <<= 1;
					bit_off--;

					//if bit_mask > 0b10000000, then reset bit_mask, bit_off, and move the off back one byte
					if(bit_mask > 0b10000000)
					{
						bit_mask = 0b1;
						bit_off = 7;
						off--;
					}
				}

				disk_write(curr_port, (uint32_t*)block_bitmap, EXT2_BLOCK_SZ, location);

				/*purposly not freeing since block_bitmap, regardless of update, is the
				 * global variable which should not be cleared*/

				//TODO in the case of non-consecutive blocks, make this optimized and work
				uint32_t *clear;
				clear = (uint32_t*)kmalloc(EXT2_BLOCK_SZ * blocks_used);

				memset(clear, 0x0, EXT2_BLOCK_SZ * blocks_used);

				//if we have exited, then all of the blocks are sequential, so write as one big chunk
				disk_write(curr_port, clear, EXT2_BLOCK_SZ * blocks_used, *output);

				kfree((uint32_t*)clear);

				return output;
			}
		}

	}

	/*purposly not freeing since block_bitmap, regardless of update,
	 * is the global variable which should not be cleared*/

	kfree((uint32_t*)output);

	//if we did not exit yet, there must be no space
	return 0;
}

uint32_t ext2_singly_create(uint32_t *block_locations, uint32_t offset, uint32_t nblocks, ext2_group_descriptor_t *gdesc)
{
	uint32_t *location, *block_data, blk, value;
	block_data = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

	location = ext2_format_block_bitmap(gdesc, 1);

	//write the locations of the singly indirect blocks in the singly block in the memory (block_data)
	for(blk = 0; blk < (nblocks < EXT2_NIND_BLOCK ? nblocks : EXT2_NIND_BLOCK); blk++)
		*(block_data + blk) = *(block_locations + offset + blk);

	//write the singly block to the physical floppy disk
	disk_write(curr_port, block_data, EXT2_BLOCK_SZ, *location);

	//store the value of location in another place so we can free location and return its location
	value = *location;

	kfree((uint32_t*)block_data);
	kfree((uint32_t*)location);

	//return the location of the singly block
	return value;

}

uint32_t ext2_doubly_create(uint32_t *block_locaitions, uint32_t offset, uint32_t nblocks, ext2_group_descriptor_t *gdesc)
{
	uint32_t *location, *block_data, blk, value;
	block_data = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

	//clear the block data for the new block
	memset(block_data, 0x0, EXT2_BLOCK_SZ);

	//get a new location for the doubly block
	location = ext2_format_block_bitmap(gdesc, 1);

	//write the locations of the singly indirect blocks to the doubly block in the tmp memory block (block_data)
	for(blk = 0; blk < (nblocks < EXT2_NDIND_BLOCK ? nblocks : EXT2_NDIND_BLOCK); blk += EXT2_NIND_BLOCK)
	{
		*(block_data + (blk / EXT2_NIND_BLOCK)) = ext2_singly_create(block_locaitions, offset + blk,
																																 (blk + EXT2_NIND_BLOCK) <= nblocks ? EXT2_NIND_BLOCK :
																																 nblocks % EXT2_NIND_BLOCK, gdesc);
	}

	disk_write(curr_port, block_data, EXT2_BLOCK_SZ, *location);

	value = *location;

	kfree((uint32_t*)block_data);
	kfree((uint32_t*)location);

	//return the location of the singly block
	return value;

}

uint32_t ext2_inode_entry_blocks(ext2_inode_t *node, ext2_group_descriptor_t *gdesc,
															 uint32_t *block_locations, uint32_t blocks_used)
{
	uint32_t blk;

	//to begin, write all of the blocks a 0, so that there is no accidental junk
	for(blk = 0; blk < EXT2_N_BLOCKS; blk++)
		node->blocks[blk] = 0;

	//if no blocks are used, simply exit
	if(!blocks_used)
		return 0;

	//write the locations of the direct blocks
	for(blk = 0; blk < (blocks_used < EXT2_NDIR_BLOCKS ? blocks_used : EXT2_NDIR_BLOCKS); blk++)
		node->blocks[blk] = *(block_locations + blk);

	if(blocks_used < EXT2_NDIR_BLOCKS)
		//we already wrote all of the data needed to be written above, return with sucess
		return 0;
	else{
		uint32_t *location, *block_data;
		block_data = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

		//clear the block data for the new block
		memset(block_data, 0x0, EXT2_BLOCK_SZ);

		//subtract the direct blocks that we have accounted for
		blocks_used -= EXT2_NDIR_BLOCKS;

		location = ext2_format_block_bitmap(gdesc, 1);

		//write the locations of the singly indirect blocks
		for(blk = 0; blk < (blocks_used < EXT2_NIND_BLOCK ? blocks_used : EXT2_NIND_BLOCK); blk++)
			*(block_data + blk) = *(block_locations + EXT2_NDIR_BLOCKS + blk);

		disk_write(curr_port, block_data, EXT2_BLOCK_SZ, *location);

		//write the location of the singly block
		node->blocks[EXT2_NDIR_BLOCKS] = *location;

		//write the doubly blocks
		if(blocks_used > EXT2_NIND_BLOCK)
		{
			//subtract the singly indirect blocks that we have accounted for
			blocks_used -= EXT2_NIND_BLOCK;

			//clear the block data for the new block
			memset(block_data, 0x0, EXT2_BLOCK_SZ);

			//get a new location for the doubly block
			location = ext2_format_block_bitmap(gdesc, 1);

			//write the locations of the doubly indirect blocks
			for(blk = 0; blk < (blocks_used < EXT2_NDIND_BLOCK ? blocks_used : EXT2_NDIND_BLOCK); blk += EXT2_NIND_BLOCK)
			{
				*(block_data + (blk / EXT2_NIND_BLOCK)) = ext2_singly_create(block_locations, EXT2_NDIR_BLOCKS + blk,
																																		 (blk + EXT2_NIND_BLOCK) <= blocks_used ?
																																		 EXT2_NIND_BLOCK : blocks_used % EXT2_NIND_BLOCK, gdesc);
			}

			disk_write(curr_port, block_data, EXT2_BLOCK_SZ, *location);

			//write the location of the doubly block
			node->blocks[EXT2_NDIR_BLOCKS + 1] = *location;

			//write the triply blocks
			if(blocks_used > EXT2_NDIND_BLOCK)
			{
				//subtract the singly indirect blocks that we have accounted for
				blocks_used -= EXT2_NTIND_BLOCK;

				//clear the block data for the new block
				memset(block_data, 0x0, EXT2_BLOCK_SZ);

				//get a new location for the triply block
				location = ext2_format_block_bitmap(gdesc, 1);

				//write the locations of the triply indirect blocks
				for(blk = 0; blk < (blocks_used < EXT2_NTIND_BLOCK ? blocks_used : EXT2_NTIND_BLOCK); blk += EXT2_NDIND_BLOCK)
				{
					*(block_data + (blk / EXT2_NIND_BLOCK)) = ext2_doubly_create(block_locations, EXT2_NIND_BLOCK + EXT2_NDIR_BLOCKS + blk,
																																			 (blk + EXT2_NDIND_BLOCK) <= blocks_used ? EXT2_NDIND_BLOCK :
																																			 blocks_used % EXT2_NDIND_BLOCK, gdesc);
				}

				disk_write(curr_port, block_data, EXT2_BLOCK_SZ, *location);

				//write the location of the triply block
				node->blocks[EXT2_NDIR_BLOCKS + 2] = *location;
			}
		}

		kfree((uint32_t*)location);
		kfree((uint32_t*)block_data);

	}
	return 1;
}

uint32_t ext2_data_to_inode_table(ext2_inode_t *data, ext2_group_descriptor_t *gdesc, ext2_superblock_t *sblock)
{
	ext2_inode_t *buffer;

	if(!ext2_g_inode_table)
	{
		buffer = (ext2_inode_t*)kmalloc(gdesc->inode_table_size * EXT2_BLOCK_SZ);
		ext2_g_inode_table = buffer;

		disk_read(curr_port, gdesc->inode_table_id, gdesc->inode_table_size * EXT2_BLOCK_SZ, (uint32_t*)buffer);

	}else
		buffer = ext2_g_inode_table;

	//loop until a free space has opened up
	uint32_t off = 0;
	while(buffer[off].nlinks && off < sblock->inodes_per_group)
		off++;

	//if we did not find enough space, return an error
	if(off == sblock->inodes_per_group)
		return 1; //error

	memcpy((uint8_t*)buffer + sizeof(ext2_inode_t) * off, data, sizeof(ext2_inode_t));

	//write the new inode table buffer
	disk_write(curr_port, (uint32_t*)buffer, gdesc->inode_table_size * EXT2_BLOCK_SZ, gdesc->inode_table_id);

	//purposly not freeing since buffer, either way with update, is the global variable which should not be cleared

	//sucess!, return where we put it
	return off;

}

uint32_t ext2_inode_from_inode_table(uint32_t inode_number, ext2_inode_t *output, ext2_group_descriptor_t *gdesc)
{
	ext2_inode_t *buffer;

	if(!ext2_g_inode_table)
	{
		buffer = ext2_get_inode_table(gdesc);
		ext2_g_inode_table = buffer;
	}else
		buffer = ext2_g_inode_table;

	memcpy(output, (uint8_t*)buffer + sizeof(ext2_inode_t) * inode_number, sizeof(ext2_inode_t));

	//purposly not freeing since buffer, either way with update, is the global variable which should not be cleared
	//~ kfree((uint32_t*)buffer);

	//sucess!
	return 0;
}

ext2_inode_t *ext2_file_from_dir(ext2_inode_t *dir, char *name)
{
	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;

	if(!ext2_g_sblock || !ext2_g_gdesc)
	{
		if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
			return 0; //error
	}else{
		sblock = ext2_g_sblock;
		gdesc = ext2_g_gdesc;
	}

	//get the inode table
	/**ext2_inode_t *inode_table;
	inode_table = ext2_get_inode_table(gdesc);**/

	ext2_inode_t *inode;
	inode = (ext2_inode_t*)kmalloc(sizeof(ext2_inode_t));

	if(dir->type == EXT2_DIR)
	{
		struct ext2_dirent *dirent2;

		uint32_t i;
		for(i = 0; i < sblock->total_inodes; i++)
		{
			//get the dirent information at index i
			dirent2 = ext2_dirent_from_dir(dir, i);

			if(dirent2)
			{
				//the input name matches to the dirent2.name that we got
				if(!strcmp(name, dirent2->name))
				{

					inode = ext2_inode_from_offset(dirent2->ino);

					kfree((uint32_t*)dirent2->name);
					//~ kfree((uint32_t*)inode);
					kfree((uint32_t*)sblock);
					kfree((uint32_t*)gdesc);

					return inode;
				}
			}
			//kfree((uint32_t*)dirent2->name);
		}
	}

	kfree((uint32_t*)inode);
	kfree((uint32_t*)sblock);
	kfree((uint32_t*)gdesc);
	//no file found, error
	return 0;
}

struct ext2_dirent *ext2_dirent_from_dir_data(ext2_inode_t *dir, uint32_t index, uint32_t *data)
{
	if(dir->type == EXT2_DIR) //just to check if the input node is a directory
	{

		uint32_t i = 0;
		uint32_t loop = 0;/**, b = 0;**/

		//this no data has been passed
		if(!data)
			return 0;

		//loop forever, we will break when we find it or return and exit if we do not
		for(;;)
		{
			//if the loop equals the index we are looking for
			if(loop == index)
			{
				//if the rec_len of the direct has contents
				if(*(uint16_t*)((uint8_t*)data + i + sizeof(dirent.ino)))
				{

					static struct ext2_dirent dirent2;

					//extract the dirent information at the offset of i
					dirent2.ino = *(uint32_t*)((uint8_t*)data + i);
					dirent2.rec_len = *(uint16_t*)((uint8_t*)data + i + sizeof(dirent2.ino));
					dirent2.name_len = *(uint8_t*)((uint8_t*)data + i + sizeof(dirent2.ino) + sizeof(dirent2.rec_len));
					dirent2.file_type = *(uint8_t*)((uint8_t*)data + i + sizeof(dirent2.ino) + sizeof(dirent2.rec_len) + sizeof(dirent2.name_len));

					//clears junk that may be contained in memory when kmallocing
					dirent2.name = (char*)kmalloc(dirent2.name_len + 1);

					//clears junk that may be contained in memory when kmallocing
					memset(dirent2.name, 0, dirent2.name_len + 1);

					//copies the name to dirent2.name
					memcpy(dirent2.name, (char*)((uint8_t*)data + i + sizeof(dirent2.ino) + sizeof(dirent2.rec_len) + sizeof(dirent2.name_len) + sizeof(dirent2.file_type)), dirent2.name_len + 1);

					*(dirent2.name + dirent2.name_len) = 0; //Adds terminating 0 to string

					return &dirent2;

				}else{
					//error
					return 0;
				}

			}else{

				//this dirent is not the last one (there are more dirents after this one)
				if(*(uint16_t*)((uint8_t*)data + i + sizeof(dirent.ino)))
				{
					//increase i with the rec_len that we get by moving fileheader sizeof(dirent.ino) (4 bytes) and reading its value
					i += *(uint16_t*)((uint8_t*)data + i + sizeof(dirent.ino));
					loop++;
				}else //this is the last direct, if we have found nothing, exit
						return 0;

			}

		}

	}else{
		return 0;
	}

}

struct ext2_dirent *ext2_dirent_from_dir(ext2_inode_t *dir, uint32_t index)
{
	if(dir->type == EXT2_DIR) //just to check if the input node is a directory
	{

		uint32_t i = 0;
		uint32_t loop = 0, b = 0, *block;

		block = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

		ext2_block_of_set(dir, b, block);

		//this dir has no blocks assigned
		if(!block)
		{
			kfree((uint32_t*)block);
			return 0; //error
		}

		//loop forever, we will break when we find it or return and exit if we do not
		for(;;)
		{
			//if the loop equals the index we are looking for
			if(loop == index)
			{
				//if the rec_len of the direct has contents
				if(*(uint16_t*)((uint8_t*)block + i + sizeof(dirent.ino)))
				{

					static struct ext2_dirent dirent2;

					//extract the dirent information at the offset of i
					dirent2.ino = *(uint32_t*)((uint8_t*)block + i);
					dirent2.rec_len = *(uint16_t*)((uint8_t*)block + i + sizeof(dirent2.ino));
					dirent2.name_len = *(uint8_t*)((uint8_t*)block + i + sizeof(dirent2.ino) + sizeof(dirent2.rec_len));
					dirent2.file_type = *(uint8_t*)((uint8_t*)block + i + sizeof(dirent2.ino) + sizeof(dirent2.rec_len) + sizeof(dirent2.name_len));

					//clears junk that may be contained in memory when kmallocing
					dirent2.name = (char*)kmalloc(dirent2.name_len + 1);

					//clears junk that may be contained in memory when kmallocing
					memset(dirent2.name, 0, dirent2.name_len + 1);

					//copies the name to dirent2.name
					memcpy(dirent2.name, (char*)((uint8_t*)block + i + sizeof(dirent2.ino) + sizeof(dirent2.rec_len) + sizeof(dirent2.name_len) + sizeof(dirent2.file_type)), dirent2.name_len + 1);

					*(dirent2.name + dirent2.name_len) = 0; //Adds terminating 0 to string

					kfree((uint32_t*)block);
					return &dirent2;

				}else{

					kfree((uint32_t*)block);
					//error
					return 0;
				}

			}else{

				//this dirent is not the last one (there are more dirents after this one)
				if(*(uint16_t*)((uint8_t*)block + i + sizeof(dirent.ino)))
				{
					//increase i with the rec_len that we get by moving fileheader sizeof(dirent.ino) (4 bytes) and reading its value
					i += *(uint16_t*)((uint8_t*)block + i + sizeof(dirent.ino));
					loop++;
				}else{ //this is the last direct, add 1 to block and reset the offset (i)
					i = 0;

					//recalculate the block address
					ext2_block_of_set(dir, ++b, block);

					//this dir has not blocks assigned
					if(!block)
					{
						kfree((uint32_t*)block);
						return 0;
					}
				}

			}

		}

	}else{
		return 0;
	}

}

uint32_t ext2_block_of_set(ext2_inode_t *file, uint32_t block_number, uint32_t *block_output)
{
	//if the block is a direct block
	if((int)block_number >= 0 && block_number < EXT2_NDIR_BLOCKS)
	{

		if(!file->blocks[block_number])
		{
			block_output = 0;
			return 0;
		}

		disk_read(curr_port, file->blocks[block_number], EXT2_BLOCK_SZ, block_output);

		return file->blocks[block_number];

	//if the block is in the singly set
	}else if(block_number >= EXT2_NDIR_BLOCKS && block_number < EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK)
	{
		//the offset to the block inside the singly
		uint32_t singly_offset = block_number - EXT2_NDIR_BLOCKS;

		uint32_t *singly;
		singly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

		if(!file->blocks[EXT2_IND_BLOCK])
		{
			kfree((uint32_t*)singly);
			block_output = 0;
			return 0;
		}

		disk_read(curr_port, file->blocks[EXT2_IND_BLOCK], EXT2_BLOCK_SZ, singly);

		uint32_t block = *(singly + singly_offset);

		if(!block)
		{
			kfree((uint32_t*)singly);
			block_output = 0;
			return 0;
		}

		disk_read(curr_port, block, EXT2_BLOCK_SZ, block_output);

		kfree((uint32_t*)singly);

		return block;

	//if the block is in the doubly set
	}else if(block_number >= EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK &&
					block_number < EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK + EXT2_NDIND_BLOCK)
	{
		uint32_t offset = block_number - (EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK);
		uint32_t doubly_offset = offset / EXT2_NIND_BLOCK;
		uint32_t singly_offset = offset % EXT2_NIND_BLOCK;

		uint32_t *doubly, *singly;
		singly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);
		doubly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

		//error checking is always good
		if(!file->blocks[EXT2_DIND_BLOCK])
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			block_output = 0;
			return 0;
		}

		disk_read(curr_port, file->blocks[EXT2_DIND_BLOCK], EXT2_BLOCK_SZ, doubly);

		uint32_t singly_block_location = *(doubly + doubly_offset);

		//error checking is always good
		if(!singly_block_location)
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			block_output = 0;
			return 0;
		}

		disk_read(curr_port, singly_block_location, EXT2_BLOCK_SZ, singly);

		uint32_t block = *(singly + singly_offset);

		//error checking is always good
		if(!block)
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			block_output = 0;
			return 0;
		}

		disk_read(curr_port, block, EXT2_BLOCK_SZ, block_output);

		//free the stuff allocated
		kfree((uint32_t*)doubly);
		kfree((uint32_t*)singly);

		return block;

	//if the block is in the triply set
	}else if(block_number >= EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK + EXT2_NDIND_BLOCK &&
					block_number < EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK + EXT2_NDIND_BLOCK + EXT2_NTIND_BLOCK)
	{
		uint32_t offset = block_number - (EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK + EXT2_NDIND_BLOCK);
		uint32_t triply_offset = offset / EXT2_NDIND_BLOCK;
		uint32_t doubly_offset = (offset % EXT2_NDIND_BLOCK) / EXT2_NIND_BLOCK;
		uint32_t singly_offset = (offset % EXT2_NDIND_BLOCK) % EXT2_NIND_BLOCK;

		uint32_t *triply, *doubly, *singly;
		triply = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);
		doubly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);
		singly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

		if(!file->blocks[EXT2_TIND_BLOCK])
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			kfree((uint32_t*)triply);
			block_output = 0;
			return 0;
		}

		disk_read(curr_port, file->blocks[EXT2_TIND_BLOCK], EXT2_BLOCK_SZ, triply);

		uint32_t doubly_block_location = *(triply + triply_offset);

		if(!doubly_block_location)
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			kfree((uint32_t*)triply);
			block_output = 0;
			return 0;
		}

		disk_read(curr_port, doubly_block_location, EXT2_BLOCK_SZ, doubly);

		uint32_t singly_block_location = *(doubly + doubly_offset);

		if(!singly_block_location)
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			kfree((uint32_t*)triply);
			block_output = 0;
			return 0;
		}

		disk_read(curr_port, singly_block_location, EXT2_BLOCK_SZ, singly);

		uint32_t block = *(singly + singly_offset);

		if(!block)
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			kfree((uint32_t*)triply);
			block_output = 0;
			return 0;
		}

		disk_read(curr_port, block, EXT2_BLOCK_SZ, block_output);

		kfree((uint32_t*)triply);
		kfree((uint32_t*)doubly);
		kfree((uint32_t*)singly);

		return block;
	}else{

		block_output = 0;

		//the block number is not in range, error
		return 0;
	}

}

uint32_t ext2_write_block_of_set(ext2_inode_t *file, uint32_t block_number, uint32_t *block_data, uint32_t size)
{
	if(!size)
		return 0;
	else if(size > EXT2_BLOCK_SZ)
		size = EXT2_BLOCK_SZ;

	//if the block is a direct block
	if((int)block_number >= 0 && block_number < EXT2_NDIR_BLOCKS)
	{
		if(!file->blocks[block_number])
			return 0;

		disk_write(curr_port, block_data, size, file->blocks[block_number]);

		return file->blocks[block_number];

	//if the block is in the singly set
	}else if(block_number >= EXT2_NDIR_BLOCKS && block_number < EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK)
	{
		//the offset to the block inside the singly
		uint32_t singly_offset = block_number - EXT2_NDIR_BLOCKS;

		uint32_t *singly;
		singly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

		if(!file->blocks[EXT2_IND_BLOCK])
		{
			kfree((uint32_t*)singly);
			return 0;
		}

		disk_read(curr_port, file->blocks[EXT2_IND_BLOCK], EXT2_BLOCK_SZ, singly);

		uint32_t block = *(singly + singly_offset);

		if(!block)
		{
			kfree((uint32_t*)singly);
			return 0;
		}

		disk_write(curr_port, block_data, size, block);

		kfree((uint32_t*)singly);

		return block;

	//if the block is in the doubly set
	}else if(block_number >= EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK &&
					block_number < EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK + EXT2_NDIND_BLOCK)
	{
		uint32_t offset = block_number - EXT2_NDIR_BLOCKS;
		uint32_t doubly_offset = offset / EXT2_NIND_BLOCK;
		uint32_t singly_offset = offset % EXT2_NIND_BLOCK;

		uint32_t *doubly, *singly;
		singly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);
		doubly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

		if(!file->blocks[EXT2_DIND_BLOCK])
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			return 0;
		}

		disk_read(curr_port, file->blocks[EXT2_DIND_BLOCK], EXT2_BLOCK_SZ, doubly);

		uint32_t singly_block_location = *(doubly + doubly_offset);

		if(!singly_block_location)
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			return 0;
		}

		disk_read(curr_port, singly_block_location, EXT2_BLOCK_SZ, singly);

		uint32_t block = *(singly + singly_offset);

		if(!block)
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			return 0;
		}

		disk_write(curr_port, block_data, size, block);

		kfree((uint32_t*)doubly);
		kfree((uint32_t*)singly);

		return block;

	//if the block is in the triply set
	}else if(block_number >= EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK + EXT2_NDIND_BLOCK &&
					block_number < EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK + EXT2_NDIND_BLOCK + EXT2_NTIND_BLOCK)
	{
		uint32_t offset = block_number - EXT2_NDIR_BLOCKS;
		uint32_t triply_offset = offset / EXT2_NDIND_BLOCK;
		uint32_t doubly_offset = (offset % EXT2_NDIND_BLOCK) / EXT2_NIND_BLOCK;
		uint32_t singly_offset = (offset % EXT2_NDIND_BLOCK) % EXT2_NIND_BLOCK;

		uint32_t *triply, *doubly, *singly;
		triply = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);
		doubly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);
		singly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

		if(!file->blocks[EXT2_TIND_BLOCK])
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			kfree((uint32_t*)triply);
			return 0;
		}

		disk_read(curr_port, file->blocks[EXT2_TIND_BLOCK], EXT2_BLOCK_SZ, triply);

		uint32_t doubly_block_location = *(triply + triply_offset);

		if(!doubly_block_location)
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			kfree((uint32_t*)triply);
			return 0;
		}

		disk_read(curr_port, doubly_block_location, EXT2_BLOCK_SZ, doubly);

		uint32_t singly_block_location = *(doubly + doubly_offset);

		if(!singly_block_location)
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			kfree((uint32_t*)triply);
			return 0;
		}

		disk_read(curr_port, singly_block_location, EXT2_BLOCK_SZ, singly);

		uint32_t block = *(singly + singly_offset);

		if(!block)
		{
			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			kfree((uint32_t*)triply);
			return 0;
		}

		disk_write(curr_port, block_data, size, block);

		kfree((uint32_t*)triply);
		kfree((uint32_t*)doubly);
		kfree((uint32_t*)singly);

		return block;
	}else
		return 0; //the block number is not in range, error
}

uint32_t ext2_add_file_to_dir(ext2_inode_t *parent_dir, ext2_inode_t *file, uint32_t file_type, char *filename)
{
	struct ext2_dirent dirent;

	uint32_t lengthOfName = strlen(filename);

	if(lengthOfName > 255)
		lengthOfName = 255;

	dirent.ino = file->inode;

	//The size of this whole struct basically
	dirent.rec_len = sizeof(dirent.ino) + sizeof(dirent.rec_len) + sizeof(dirent.name_len) + sizeof(dirent.file_type) + lengthOfName + 1; //+1 is NULL terminating \000
	dirent.name_len = (uint8_t)lengthOfName;
	dirent.file_type = file_type;

	//~ printf("dirent.name is :%d\n", *dirent.name);

	//+1 being the \000 NULL termination 0 byte at the end of the string
	dirent.name = (char*)kmalloc(lengthOfName + 1);

	//copy the name of the file (fileNode->name) to the dirent.name
	memcpy(dirent.name, filename, lengthOfName); //fileNode->name already had a \000 termination zero
	*(dirent.name + lengthOfName) = 0; //Just in case add a \000 at the end

	uint32_t i, b, *block, location;

	//TODO make this search by chunk not block
	block = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

	/*In this section, we find a valid offset (i) and block number (b)
	 * to an open dirent space
	 *
	 * length - 1 because if length == EXT2_BLOCK_SZ, there should be only one
	 * block checked, but w/o that -1, 2 will be checked */
	for(b = 0; b <= (uint32_t)((parent_dir->size - 1) / EXT2_BLOCK_SZ); b++)
	{
		i = 0;
		location = ext2_block_of_set(parent_dir, b, block);

		if(!location)
			return 1; //error

		//this dir has not blocks assigned
		if(!block)
		{
			kfree((uint32_t*)block);
			kfree((uint32_t*)dirent.name);
			return 1; //error
		}

		//loop until we hit the end of the current block or get an open dirent
		while(*(uint16_t*)((uint8_t*)block + i + sizeof(dirent.ino)))
		{
			//increase i with the rec_len that we get by moving fileheader sizeof(dirent.ino) (4 bytes) and reading its value
			i += *(uint16_t*)((uint8_t*)block + i + sizeof(dirent.ino));

			//if the offset (i) + the length of the contents in the struct dirent is greater than what a block can hold, exit and go to new block
			if(i + dirent.rec_len >= EXT2_BLOCK_SZ)
				break;

			/*if the offset (i) + the length of the contents in the struct dirent is
			 * greater than what a direcotory can hold, expand the directory*/
			if(b * EXT2_BLOCK_SZ + i + dirent.rec_len >= parent_dir->size)
				//expand parent_dir by one block
				if(ext2_expand(parent_dir, EXT2_BLOCK_SZ))
					return 1; //failed, out of directory left over space

		}

		//if i is a valid offset, do not go to a new block, just exit
		if(!*(uint16_t*)((uint8_t*)block + i + sizeof(dirent.ino)))
			break;

	}

	//assigns the contents of the struct dirent to the directory contents location
	memcpy((uint8_t*)block + i, &dirent, dirent.rec_len - dirent.name_len);

	strcpy((char*)((uint8_t*)block + i + sizeof(dirent.ino) + sizeof(dirent.rec_len) + sizeof(dirent.name_len) + sizeof(dirent.file_type)), dirent.name);
/**
	strcpy((uint8_t*)block + i + sizeof(dirent.ino) + sizeof(dirent.rec_len) + sizeof(dirent.name_len) + sizeof(dirent.file_type), dirent.name);
**/
	disk_write(curr_port, block, EXT2_BLOCK_SZ, location);

	kfree((uint32_t*)block);

	kfree((uint32_t*)dirent.name);

	return 0;
}

uint32_t ext2_free_blocks(uint32_t *block_locs, uint32_t nblocks)
{
	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;

	if(!ext2_g_sblock || !ext2_g_gdesc)
	{
		if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
			return 0; //error
	}else{
		sblock = ext2_g_sblock;
		gdesc = ext2_g_gdesc;
	}

	uint8_t *block_bitmap;

	if(!ext2_g_bb)
	{
		block_bitmap = (uint8_t*)kmalloc(EXT2_BLOCK_SZ);
		ext2_g_bb = block_bitmap;
		disk_read(curr_port, gdesc->block_bitmap, EXT2_BLOCK_SZ, (uint32_t*)block_bitmap);
	}else{
		block_bitmap = ext2_g_bb;
	}

	//the offset from the beginning of the file (the end of the inode table)
	//- the extra BLOCKS_TO_SECTORS(1), is to offset this to the begining of the last block
	uint32_t begining_offset = gdesc->inode_table_id + BLOCKS_TO_SECTORS(gdesc->inode_table_size) -
		BLOCKS_TO_SECTORS(1);

	uint32_t byte = 0, bit = 0, i, error = 0, changes = FALSE;
	for(i = 0; i < nblocks; i++)
	{
		//if the block location is 0, then move on to the next block location value
		if(!*(block_locs + i))
			continue;

		byte = SECTORS_TO_BLOCKS(*(block_locs + i) - begining_offset) / 8;
		bit = 0b10000000 >> ((SECTORS_TO_BLOCKS(*(block_locs + i) - begining_offset) % 8) - 1);

		//if byte is too large, continue on and set to return an error at the end
		if(byte > EXT2_BLOCK_SZ)
		{
			error = 1;
			continue;
		}

		//just checking so we do not xor a 0 (off) bit and make it 1 (on) accidentally
		if(*(block_bitmap + byte) & bit)
		{
			*(block_bitmap + byte) ^= bit;
			if(changes == FALSE)
				changes = TRUE;
		}
	}

	if(changes == TRUE)
		disk_write(curr_port, (uint32_t*)block_bitmap, EXT2_BLOCK_SZ, gdesc->block_bitmap);

	return error;
}

uint32_t ext2_update_node_in_inode_table(ext2_inode_t *node)
{
	ext2_inode_t *itable;

	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;

	uint32_t inode_table_size, i;
	if(!ext2_g_sblock || !ext2_g_gdesc)
	{
		if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
			return 0; //error
	}
	else
	{
		sblock = ext2_g_sblock;
		gdesc = ext2_g_gdesc;
	}

	//set the number of bytes the inode table is
	inode_table_size = gdesc->inode_table_size * EXT2_BLOCK_SZ;

	if(!ext2_g_inode_table)
	{
		itable = (ext2_inode_t*)kmalloc(inode_table_size);
		ext2_g_inode_table = itable;

		disk_read(curr_port, gdesc->inode_table_id, inode_table_size, (uint32_t*)itable);

	}
	else
		itable = ext2_g_inode_table;

	for(i = 0; i < inode_table_size; i += sizeof(ext2_inode_t))
	{
		//if we have found our node in the inode table
		if(((ext2_inode_t*)itable + i)->inode == node->inode)
		{
			//copy the new table over
			memcpy(itable + i, node, sizeof(ext2_inode_t));

			//write the new inode_table on the floppy disk
			disk_write(curr_port, (uint32_t*)itable, inode_table_size, gdesc->inode_table_id);

			return 0; //sucess!
		}
	}
	//if we exited the for loop, that means the inode was not found, thus cannot be updated
	return 1; //error
}

uint32_t ext2_expand(ext2_inode_t *node, uint32_t increase_bytes)
{
	//if we are not resizing anything, consider it a sucess! :)
	if(!increase_bytes)
		return 0;

	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;

	if(!ext2_g_sblock || !ext2_g_gdesc)
	{
		if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
			return 1; //error
	}else{
		sblock = ext2_g_sblock;
		gdesc = ext2_g_gdesc;
	}

	uint32_t *initial_locs, *all_locs, *added_locs, orig_nblocks, added_nblocks, all_nblocks, test_nblocks;

	//if the node->size is 0, set it manually to avoit signing issues
	orig_nblocks = node->size ? ((node->size - 1) / EXT2_BLOCK_SZ) + 1 : 0;
	test_nblocks = ((node->size + increase_bytes - 1) / EXT2_BLOCK_SZ) + 1;

	//if the increased size fits in the data already allocated, no need to alocate more, exit
	if(orig_nblocks == test_nblocks)
	{
		//increase the nodes size in the inode table
		node->size += increase_bytes;

		//simple error checking
		if(ext2_update_node_in_inode_table(node))
			return 1; //error
		else
			return 0; //sucess!
	}

	added_nblocks = ((increase_bytes - 1) / EXT2_BLOCK_SZ) + 1;
	all_nblocks = orig_nblocks + added_nblocks;

	all_locs = (uint32_t*)kmalloc(sizeof(uint32_t) * all_nblocks);

	//retrieve the preexisting block locations
	initial_locs = ext2_block_locs(node);

	//retrieve the new block locations
	added_locs = ext2_format_block_bitmap(gdesc, added_nblocks);

	//concatonate all of the blocks into one array
	memcpy(all_locs, initial_locs, sizeof(uint32_t) * orig_nblocks);
	memcpy(all_locs + orig_nblocks, added_locs, sizeof(uint32_t) * added_nblocks);

	//remove the old, soon to be replaced, singly, doubly, and triply blocks from the block bitmap
	uint32_t *blocks_to_rm, i;
	blocks_to_rm = (uint32_t*)kmalloc(sizeof(uint32_t) * (EXT2_N_BLOCKS - EXT2_NDIR_BLOCKS));

	for(i = 0; i < EXT2_N_BLOCKS - EXT2_NDIR_BLOCKS; i++)
		*(blocks_to_rm + i) = *(node->blocks + EXT2_NDIR_BLOCKS + i);

	//when the for loop exits, i will equal the number of blocks that have been saved in blocks_to_rm
	if(ext2_free_blocks(blocks_to_rm, i))
	{

		kfree((uint32_t*)initial_locs);
		kfree((uint32_t*)added_locs);
		kfree((uint32_t*)all_locs);
		kfree((uint32_t*)blocks_to_rm);

		return 1; //there was some sort of error
	}

	//index the new blocks into the node's data
	ext2_inode_entry_blocks(node, gdesc, all_locs, all_nblocks);

	//increase the nodes size in the inode table
	node->size += increase_bytes;

	//simple error checking
	if(ext2_update_node_in_inode_table(node))
		return 1; //error

	kfree((uint32_t*)initial_locs);
	kfree((uint32_t*)added_locs);
	kfree((uint32_t*)all_locs);
	kfree((uint32_t*)blocks_to_rm);

	//sucess!
	return 0;
}

uint32_t ext2_remove_inode_entry(ext2_inode_t *node)
{
	ext2_inode_t *buffer;
	uint8_t *ib;

	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;

	if(!ext2_g_sblock || !ext2_g_gdesc)
	{
		if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
			return 0; //error
	}else{
		sblock = ext2_g_sblock;
		gdesc = ext2_g_gdesc;
	}

	if(!ext2_g_inode_table)
	{
		buffer = (ext2_inode_t*)kmalloc(gdesc->inode_table_size * EXT2_BLOCK_SZ);
		ext2_g_inode_table = buffer;

		disk_read(curr_port, gdesc->inode_table_id, gdesc->inode_table_size * EXT2_BLOCK_SZ, (uint32_t*)buffer);

	}else
		buffer = ext2_g_inode_table;

	if(!ext2_g_ib)
	{
		ib = (uint8_t*)kmalloc(EXT2_BLOCK_SZ);
		ext2_g_ib = ib;

		disk_read(curr_port, gdesc->inode_bitmap, EXT2_BLOCK_SZ, (uint32_t*)ib);
	}else
		ib = ext2_g_ib;

	//loop until we found out inode entry
	uint32_t off = 0;
	while(buffer[off].inode != node->inode && off < sblock->inodes_per_group)
		off++;

	//we did not find out inode entry
	if(off == sblock->inodes_per_group)
		return 1; //error

	//flip the inode bitmap bit
	uint32_t byte, bit;
	byte = off / 8;
	bit = 0b10000000 >> ((off % 8) - 1);

	//check if the bit is 1, so we do not accidentally change a 0 to a 1
	if(*(ib + byte) & bit)
		 *(ib + byte) ^= bit;
	else
		return 1; //there was no trace of the inode in the inode bitmap, error

	//clear the inode entry
	memset(&buffer[off], 0x0, sizeof(ext2_inode_t));

	//write our changes
	disk_write(curr_port, (uint32_t*)buffer, gdesc->inode_table_size * EXT2_BLOCK_SZ, gdesc->inode_table_id);
	disk_write(curr_port, (uint32_t*)ib, EXT2_BLOCK_SZ, gdesc->inode_bitmap);

	//sucess!
	return 0;
}

uint32_t ext2_delete(ext2_inode_t *parent_dir, ext2_inode_t *node)
{
	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;

	if(!ext2_g_sblock || !ext2_g_gdesc)
	{
		if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
			return 1; //error
	}else{
		sblock = ext2_g_sblock;
		gdesc = ext2_g_gdesc;
	}

	//remove the inode entry from the inode table and the inode bitmap
	if(ext2_remove_inode_entry(node))
		return 1; //error

	//free the node's blocks
	if(ext2_free_data_blocks(parent_dir, node))
		return 1; //errror

	//remove the node's dirent
	if(ext2_remove_dirent(parent_dir, node))
		return 1; //error

	//sucess!
	return 0;
}

ext2_inode_t *ext2_create_dir(ext2_inode_t *parent_dir, char *name)
{
	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;

	if(!ext2_g_sblock || !ext2_g_gdesc)
	{
		if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
			return 0; //error
	}else{
		sblock = ext2_g_sblock;
		gdesc = ext2_g_gdesc;
	}

	ext2_inode_t *dir;
	dir = __create_dir__(sblock, gdesc);

	/*initrd_root (sblock->first_inode) is the "mother root" directory, a directory
	 * that contains root and only root directory, if the parentNode is
	 * initrd_root, then we are creating root, and root has its own special
	 * hard links that are made outside of this function, else, make those hard
	 * links */
	if(parent_dir->inode != sblock->first_inode)
	{
		printf("\noopp%d", parent_dir->inode, sblock->first_inode);
		ext2_add_file_to_dir(parent_dir, dir, dir->type, name);

		ext2_add_hardlink_to_dir(dir, dir, "."); //adds hardlink for the current directory
		ext2_add_hardlink_to_dir(dir, parent_dir, ".."); //adds hardlink for the parent directory
//*/
	}

	//purposly not freeing since buffer, either way with update, is the global variable which should not be cleared

	return dir;
}

static ext2_inode_t *__create_dir__(ext2_superblock_t *sblock, ext2_group_descriptor_t *gdesc)
{
	//a new directory will consist of one block for the '.' and '..' directories
	uint32_t blocks_used = 1, *block_locations;

	block_locations = ext2_format_block_bitmap(gdesc, blocks_used);

	ext2_inode_t *data;
	data = (ext2_inode_t*)kmalloc(sizeof(ext2_inode_t));

	data->magic = M_EXT2;
	data->inode = ext2_find_open_inode(sblock, gdesc);
	data->mode = EXT2_I_RUSR | EXT2_I_WUSR | EXT2_I_XUSR | EXT2_I_RGRP | EXT2_I_XGRP | EXT2_I_ROTH | EXT2_I_XOTH;
	data->type = EXT2_DIR;
	data->uid = 0;
	data->size = blocks_used * EXT2_BLOCK_SZ;
	data->atime = posix_time();
	data->ctime = posix_time();
	data->mtime = posix_time();
	data->dtime = 0;
	data->gid = 0;
	data->nlinks = 2;    //number of hard links (itself and the '.' directory)
	data->nblocks = BLOCKS_TO_SECTORS(blocks_used);   //blocks of 512 bytes
	data->flags = 0;
	data->osd1 = EXT2_OS_JSOS;

	//add the block information
	ext2_inode_entry_blocks(data, gdesc, block_locations, blocks_used);

	data->version = 0;
	data->fire_acl = 0;
	data->dir_acl = 0;
	data->fragment_addr = 0;

	//add the inode data to the table
	ext2_data_to_inode_table(data, gdesc, sblock);

	//decrement the number of free inodes and blocks there are
	gdesc->free_inodes--;
	gdesc->free_blocks -= blocks_used;

	disk_write(curr_port, (uint32_t*)gdesc, sizeof(ext2_group_descriptor_t), gdesc->gdesc_location);

	kfree((uint32_t*)block_locations);

	return data;
}

uint32_t ext2_find_open_inode(ext2_superblock_t *sblock, ext2_group_descriptor_t *gdesc)
{
	uint8_t *ib;

	if(!ext2_g_ib)
	{
		ib = (uint8_t*)kmalloc(EXT2_BLOCK_SZ);
		ext2_g_ib = ib;

		disk_read(curr_port, gdesc->inode_bitmap, EXT2_BLOCK_SZ, (uint32_t*)ib);
	}else
		ib = ext2_g_ib;

	//start inode at 1 since 0 is automatically taken by the mother root
	uint32_t bit = 0b10000000, byte = 0, inode = 1;

	while((*(ib + byte) & bit) && inode < sblock->total_inodes)
	{
		inode++;

		bit >>= 1;

		//if we have reached the end of bit, reset it and increment byte
		if(!bit)
		{
			bit = 0b10000000;
		 byte++;
		}
	}

	//we are out of inodes
	if(inode == sblock->total_inodes)
	{
		//we have found nothing
		return -1;
	}

	*(ib + byte) |= bit;

	disk_write(curr_port, (uint32_t*)ib, EXT2_BLOCK_SZ, gdesc->inode_bitmap);

	//purposly not freeing since sblock and gdesc, regardless of update, is the global variable which should not be cleared

	//after we exit the while loop, 'inode' is the open inode
	return inode;

}

ext2_inode_t *ext2_create_file(ext2_inode_t *parent_dir, char *name, uint32_t size)
{
	ext2_inode_t *file;
	file = __create_file__(size);

	ext2_add_file_to_dir(parent_dir, file, file->type, name);

	return file;
}

static ext2_inode_t *__create_file__(uint32_t size)
{
	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;

	//the the number of blocks the initial size will take up
	uint32_t blocks_used = size ? (uint32_t)((size - 1) / EXT2_BLOCK_SZ) + 1 : 0;

	if(!ext2_g_sblock || !ext2_g_gdesc)
	{
		if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
			return 0; //error
	}
	else
	{
		sblock = ext2_g_sblock;
		gdesc = ext2_g_gdesc;
	}

	uint32_t *block_locations;
	block_locations = ext2_format_block_bitmap(gdesc, blocks_used);

	ext2_inode_t *data;
	data = (ext2_inode_t*)kmalloc(sizeof(ext2_inode_t));

	data->magic = M_EXT2;
	data->inode = ext2_find_open_inode(sblock, gdesc);
	data->mode = EXT2_I_RUSR | EXT2_I_WUSR | EXT2_I_RGRP | EXT2_I_WGRP | EXT2_I_ROTH | EXT2_I_WOTH;
	data->type = EXT2_FILE;
	data->uid = 0;
	data->size = size;
	data->atime = posix_time();
	data->ctime = posix_time();
	data->mtime = posix_time();
	data->dtime = 0;
	data->gid = 0;
	data->nlinks = 1;    //number of hard links, itself
	data->nblocks = BLOCKS_TO_SECTORS(blocks_used);   //blocks of 512 bytes
	data->flags = 0;
	data->osd1 = EXT2_OS_JSOS;

	ext2_inode_entry_blocks(data, gdesc, block_locations, blocks_used);

	data->version = 0;
	data->fire_acl = 0;
	data->dir_acl = 0;
	data->fragment_addr = 0;

	//add the inode data to the table
	ext2_data_to_inode_table(data, gdesc, sblock);

	//decrement the number of free inodes and blocks there are
	gdesc->free_inodes--;
	gdesc->free_blocks -= blocks_used;

	disk_write(curr_port, (uint32_t*)gdesc, sizeof(ext2_group_descriptor_t), gdesc->gdesc_location);

	/*purposly not freeing since sblock and gdesc, regardless of update,
	 * is the global variable which should not be cleared*/

	if(block_locations)
		kfree((uint32_t*)block_locations);

	return data;
}

uint32_t ext2_write(ext2_inode_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
	//if the user can write to it
	if(node->mode & _Wlogged)
	{

		//a size of 0 or an offset greater than the node's size is impossible
		if(!size || offset > node->size)
			return 1; //fail

		//make sure we cap the size
		if(size + offset > node->size)
			size = node->size - offset;

		uint32_t blocks_to_write = ((size - 1) / EXT2_BLOCK_SZ) + 1, i, out;

		for(i = 0; i < blocks_to_write; i++)
		{
			out = ext2_write_block_of_set(node, i, (uint32_t*)(buffer + i * EXT2_BLOCK_SZ), size);

			if(!out)
				return 1; //fail
		}

		//sucess!
		return 0;
	}
	return 1;
}

uint32_t *ext2_get_singly(uint32_t location, uint32_t *nblocks)
{
	uint32_t i = 0, *singly, *locs;

	singly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);
	disk_read(curr_port, location, EXT2_BLOCK_SZ, singly);

	//after this while exits, i will equal the number of real blocks there are
	while(*(singly + i) && i < EXT2_NIND_BLOCK)
		i++;

	locs = (uint32_t*)kmalloc(sizeof(uint32_t) * i);

	//copies only the actual blocks, and not excessive 0's
	memcpy(locs, singly, i);

	*nblocks = i;

	kfree((uint32_t*)singly);

	return locs;
}

uint32_t *ext2_get_doubly(uint32_t location, uint32_t *nblocks)
{
	uint32_t i = 0, a = 0, *doubly, *max_locs, *locs = NULL, *tmp_nblocks, total_blocks = 0;

	doubly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);
	disk_read(curr_port, location, EXT2_BLOCK_SZ, doubly);

	//after this while exits, i will equal the number of real blocks there are in the doubly block
	while(*(doubly + i) && i < EXT2_NIND_BLOCK) //we use EXT2_NIND_BLOCK, since there are 256 singly block locations in a doubly block
		i++;

	//the maximum amout of blocks that the doubly block has
	max_locs = (uint32_t*)kmalloc(EXT2_NIND_BLOCK * sizeof(uint32_t) * i);

	tmp_nblocks = (uint32_t*)kmalloc(sizeof(uint32_t));

	for(a = 0; a < i; a++)
	{
		locs = ext2_get_singly(*(doubly + a), tmp_nblocks);
		memcpy(max_locs + a * EXT2_NIND_BLOCK, locs, *tmp_nblocks);
		total_blocks += *tmp_nblocks;
	}
	//(a - 1) equals the number of full singly blocks there were, the last tmp_nblocks contains the last singly's size
	locs = (uint32_t*)kmalloc((a - 1) * EXT2_NIND_BLOCK + *tmp_nblocks);

	//copies only the actual blocks, and not excessive 0's
	memcpy(locs, max_locs, (a - 1) * EXT2_NIND_BLOCK + *tmp_nblocks);

	*nblocks = total_blocks;

	kfree((uint32_t*)tmp_nblocks);
	kfree((uint32_t*)max_locs);
	kfree((uint32_t*)doubly);

	return locs;
}

uint32_t *ext2_get_triply(uint32_t location, uint32_t *nblocks)
{
	uint32_t i = 0, a = 0, *triply, *max_locs, *locs = NULL, *tmp_nblocks, total_blocks = 0;

	triply = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);
	disk_read(curr_port, location, EXT2_BLOCK_SZ, triply);

	//after this while exits, i will equal the number of real blocks there are in the doubly block
	while(*(triply + i) && i < EXT2_NIND_BLOCK) //we use EXT2_NIND_BLOCK, since there are 256 doubly block locations in a triply block
		i++;

	//the maximum amout of blocks that the triply block has
	max_locs = (uint32_t*)kmalloc(EXT2_NDIND_BLOCK * sizeof(uint32_t) * i);

	tmp_nblocks = (uint32_t*)kmalloc(sizeof(uint32_t));

	for(a = 0; a < i; a++)
	{
		locs = ext2_get_doubly(*(triply + a), tmp_nblocks);
		memcpy(max_locs + a * EXT2_NDIND_BLOCK, locs, *tmp_nblocks);
		total_blocks += *tmp_nblocks;
	}

	//(a - 1) equals the number of full doubly blocks there were, the last tmp_nblocks contains the last doubly's size
	locs = (uint32_t*)kmalloc((a - 1) * EXT2_NDIND_BLOCK + *tmp_nblocks);

	//copies only the actual blocks, and not excessive 0's
	memcpy(locs, max_locs, (a - 1) * EXT2_NDIND_BLOCK + *tmp_nblocks);

	*nblocks = total_blocks;

	kfree((uint32_t*)tmp_nblocks);
	kfree((uint32_t*)max_locs);
	kfree((uint32_t*)triply);

	return locs;
}

uint32_t *ext2_block_locs(ext2_inode_t *node)
{
	if(!node || !node->size)
		return 0; //error

	uint32_t *locs;
	locs = (uint32_t*)kmalloc((((node->size - 1) / EXT2_BLOCK_SZ) + 1) * sizeof(uint32_t));

	enum __block_types__ __block_types__;

	//checks what is the max type of block the file has, used for optimizations
	if(!*(node->blocks + EXT2_IND_BLOCK) && !*(node->blocks + EXT2_DIND_BLOCK) && !*(node->blocks + EXT2_TIND_BLOCK))
		__block_types__ = EXT2_DIRECT;
	if(*(node->blocks + EXT2_IND_BLOCK) && !*(node->blocks + EXT2_DIND_BLOCK) && !*(node->blocks + EXT2_TIND_BLOCK))
		__block_types__ = EXT2_SINGLY;
	if(*(node->blocks + EXT2_IND_BLOCK) && *(node->blocks + EXT2_DIND_BLOCK) && !*(node->blocks + EXT2_TIND_BLOCK))
		__block_types__ = EXT2_DOUBLY;
	if(*(node->blocks + EXT2_IND_BLOCK) && *(node->blocks + EXT2_DIND_BLOCK) && *(node->blocks + EXT2_TIND_BLOCK))
		__block_types__ = EXT2_TRIPLY;

	switch(__block_types__)
	{
		case EXT2_DIRECT:
		{
			uint32_t i = 0;

			//copy the direct blocks until they end
			while(*(node->blocks + i))
			{
				*(locs + i) = *(node->blocks + i);
				i++;
			}
			//exit
			break;
		}
		case EXT2_SINGLY:
		{
			uint32_t i, *singly, *nblocks;

			//copy all of the direct blocks
			for(i = 0; i < EXT2_NDIR_BLOCKS; i++)
				*(locs + i) = *(node->blocks + i);

			nblocks = (uint32_t*)kmalloc(sizeof(uint32_t*));

			singly = ext2_get_singly(*(node->blocks + EXT2_NDIR_BLOCKS), nblocks);

			//reset i and then copy all of the singly blocks until they end
			memcpy(locs + EXT2_NDIR_BLOCKS, singly, *nblocks);
			kfree((uint32_t*)nblocks);
			kfree((uint32_t*)singly);

			//exit
			break;
		}
		case EXT2_DOUBLY:
		{
			uint32_t i, *singly, *doubly, *nblocks;

			nblocks = (uint32_t*)kmalloc(sizeof(uint32_t*));

			//copy all of the direct blocks
			for(i = 0; i < EXT2_NDIR_BLOCKS; i++)
				*(locs + i) = *(node->blocks + i);

			singly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);
			disk_read(curr_port, *(node->blocks + EXT2_IND_BLOCK), EXT2_BLOCK_SZ, singly);

			//reset i and then copy all of the singly blocks until they end
			for(i = 0; i < EXT2_NIND_BLOCK; i++)
				*(locs + EXT2_NDIR_BLOCKS + i) = *(singly + i);

			doubly = ext2_get_doubly(*(node->blocks + EXT2_DIND_BLOCK), nblocks);
			memcpy(locs + EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK, doubly, *nblocks);

			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			kfree((uint32_t*)nblocks);
			//exit
			break;
		}
		case EXT2_TRIPLY:
		{
			uint32_t i, *singly, *doubly, *triply, *nblocks;

			nblocks = (uint32_t*)kmalloc(sizeof(uint32_t*));

			//copy all of the direct blocks
			for(i = 0; i < EXT2_NDIR_BLOCKS; i++)
				*(locs + i) = *(node->blocks + i);

			singly = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);
			disk_read(curr_port, *(node->blocks + EXT2_IND_BLOCK), EXT2_BLOCK_SZ, singly);

			//reset i and then copy all of the singly blocks until they end
			for(i = 0; i < EXT2_NIND_BLOCK; i++)
				*(locs + EXT2_NDIR_BLOCKS + i) = *(singly + i);

			doubly = ext2_get_doubly(*(node->blocks + EXT2_DIND_BLOCK), nblocks);
			memcpy(locs + EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK, doubly, *nblocks);

			triply = ext2_get_triply(*(node->blocks + EXT2_TIND_BLOCK), nblocks);
			memcpy(locs + EXT2_NDIR_BLOCKS + EXT2_NIND_BLOCK + EXT2_NDIND_BLOCK, triply, *nblocks);

			kfree((uint32_t*)singly);
			kfree((uint32_t*)doubly);
			kfree((uint32_t*)triply);
			kfree((uint32_t*)nblocks);
			//exit
			break;
		}
		default:
		{
			//error
			return 0;
		}
	}

	return locs;
}

uint32_t *ext2_chunk_data(uint32_t *blocks, uint32_t nblocks, uint32_t chunk, uint32_t *out_chunk_size)
{

	//set the output chunk size to default
	*out_chunk_size = 0;

	//chunck cannot be greater or equal to the number of blocks there are (chunk starts from 0)
	if(chunk >= nblocks)
		return 0;

	//if there are no blocks
	if(!nblocks)
		return 0; //error

	//if there is only one block
	if(nblocks == 1)
	{
		uint32_t *data;
		data = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

		disk_read(curr_port, *blocks, EXT2_BLOCK_SZ, data);

		*out_chunk_size = EXT2_BLOCK_SZ;
		return data;
	}

	//chunk_size is guaranteed to be atleast one block
	uint32_t chunks = 0, chunk_size = EXT2_BLOCK_SZ, first, second, i = 0, exit = FALSE, start, *data;

	//if we want to access the first chunk, then, we automatically want to exit at the beginning of the next chunk
	if(!chunk)
		exit = TRUE;

	first = *blocks;
	start = first;
	second = *(blocks + 1);

	//until we run out of blocks
	while(i < nblocks)
	{
		if(IS_CONS_BLOCKS(first, second) == FALSE)
		{
			//if we want the read the first chunk and the two blocks are not consecutive, just simply read from the beginning to first
			if(!chunk)
			{
				data = (uint32_t*)kmalloc(chunk_size);

				disk_read(curr_port, *blocks, chunk_size, data);

				*out_chunk_size = chunk_size;
				return data;
			}else{ //increment the first and second blocks, and the number of chunks, record where this chunk started
				chunks++;

				//if exit was to be TRUE, we need the location of our chunk, so do not override it
				if(exit == FALSE)
					 start = second;

				i++;

				//there is no more and we did not find out chunk
				if((i + 1) == nblocks && chunks != chunk)
					return 0;
				else if((i + 1) == nblocks && chunks == chunk) //we found our chunk at the very end
				{
					//'second' is our chunk
					data = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);

					disk_read(curr_port, second, EXT2_BLOCK_SZ, data);

					*out_chunk_size = EXT2_BLOCK_SZ;
					return data;
				}else{
					first = *(blocks + i);
					second = *(blocks + i + 1);
				}

				/*exit will only get set if we have entered our wanted block,
				 * once the program gets to this if statement, it is at the beginning
				 * of the next chunk, now we have the appropriot size for our chunk and
				 * its initial location*/
				if(exit == TRUE)
				{
					data = (uint32_t*)kmalloc(chunk_size);

					disk_read(curr_port, start, chunk_size, data);

					*out_chunk_size = chunk_size;
					return data;
				}
			}

		}else{ //increment the first and second blocks
			i++;
			chunk_size += EXT2_BLOCK_SZ;

			//there is no more and we did not find out chunk
			if((i + 1) == nblocks && chunks != chunk)
				return 0;
			else if((i + 1) == nblocks && chunks == chunk) //we found our chunk at the very end
			{
				//read the data of the chunk
				data = (uint32_t*)kmalloc(chunk_size);

				disk_read(curr_port, start, chunk_size, data);

				*out_chunk_size = chunk_size;
				return data;
			}else{
				first = *(blocks + i);
				second = *(blocks + i + 1);
			}

		}

		/*once the chunk we are at (chunks) and the chunk we want to return (chunk),
		 * we know that atleast one block must be in chunk_size, so set it to EXT2_BLOCK_SZ,
		 * set that we want to exit*/
		if(chunks == chunk && exit == FALSE)
		{
			chunk_size = EXT2_BLOCK_SZ;

			exit = TRUE;
		}
	}

	//as a fail safe, if we exited the while loop that is an error.
	return 0;

}

void ext2_sblock_set_data(ext2_superblock_t *data, uint32_t reserved_blocks, uint32_t sblock_location,
													uint32_t error_handling, uint32_t sblock_group_num, char *partition_name)
{
	memset_fast(data, 0, sizeof(ext2_superblock_t));
	data->total_inodes = 0;
	data->total_blocks = 0;
	data->reserved_blocks = reserved_blocks;
	data->first_data_block = sblock_location;
	data->block_size = EXT2_BLOCK_SZ;
	data->blocks_per_group = 0;
	data->mtime = 0;
	data->wtime = 0;
//  data->utime = 0;
	data->mnt_count = 0;
	data->max_mnt_count = EXT2_MAX_MNT_COUNT;
	data->magic = EXT2_MAGIC;
	data->fs_state = EXT2_VALID_FS;            //we are creating the fs, everything should be ok
	data->error_handling = error_handling;
	data->minor_revision_level = 0;
	data->lastcheck = 0;
	data->checkinterval = EXT2_CHECK_INTERVAL;
	data->creator_os = EXT2_OS_JSOS;           //we have created this fs on JS-OS
	data->revision_level = EXT2_GOOD_OLD_REV;
	data->first_inode = 0;
	data->inode_struct_max = sizeof(ext2_inode_t);
	data->block_group_number = sblock_group_num;
	data->feature_compatibility = 0;           //for any special features, none as of now
	data->feature_incompat = 0;
	data->feature_ro_compat = 0;

	//setting the unique id
	*(data->unique_id) = 0;
	*(data->unique_id + 1) = 0;
	*(data->unique_id + 2) = 0;
	*(data->unique_id + 3) = 0;

	//add the null terminating 0
	if(strlen(partition_name) > (sizeof(data->volume_name) - 1)) //if partition name is greater than the space for it
		*(data->volume_name + (sizeof(data->volume_name) - 1)) = 0; //cut it off at the end with a \000
	else
		*(data->volume_name + strlen(partition_name)) = 0; //else just add a 0 to the end of partition_name

	strcpy(data->volume_name, partition_name);

	memset(data->last_mounted_on, 0, sizeof(data->last_mounted_on));
	data->compression_info = EXT2_NO_ALG;

	//sucess!
	return;
}

void ext2_set_gdesc_table(ext2_group_descriptor_t *data)
{
	//just sets all contents to null

	data->block_bitmap = 0;
	data->inode_bitmap = 0;
	data->inode_table_id = 0;
	data->free_blocks = 0;
	data->free_inodes = 0;
	data->used_dirs = 0;
	data->pad = 0;

	data->reserved = 0;

	return;

}

uint32_t ext2_set_block_group(uint32_t size)
{
	//save the original size of the partition
	uint32_t orig_size = size;

	//we make uint32_t size equal the block size in bytes
	size = size > (EXT2_MAX_BLK_GRP_SZ * EXT2_BLOCK_SZ) ? (EXT2_MAX_BLK_GRP_SZ * EXT2_BLOCK_SZ) : size;
	uint32_t blocks_per_group = (uint32_t)(size / EXT2_BLOCK_SZ);
	uint32_t inodes_per_group = size == (EXT2_MAX_BLK_GRP_SZ * EXT2_BLOCK_SZ) ? EXT2_I_PER_GRP : EXT2_I_PER_GRP_FLPPY;
	uint32_t inode_table_size = (inodes_per_group * sizeof(ext2_inode_t)) / EXT2_BLOCK_SZ;
	uint32_t nblock_groups = (uint32_t)(orig_size / size);

	//super block data
	ext2_superblock_t *sblock_data;
	sblock_data = (ext2_superblock_t*)kmalloc(sizeof(ext2_superblock_t));

	if(!ext2_g_sblock)
		ext2_g_sblock = (ext2_superblock_t*)kmalloc(nblock_groups * sizeof(ext2_superblock_t));

	//group descriptor data
	ext2_group_descriptor_t *gdesc_table_data;
	//one group desciptor data
	gdesc_table_data = (ext2_group_descriptor_t*)kmalloc(sizeof(ext2_group_descriptor_t));

	//the buffer that will hold all of the group descriptors
	uint8_t *gdesc_buf;
	gdesc_buf = (uint8_t*)kmalloc(nblock_groups * EXT2_BLOCK_SZ);

	//for the gdesc cache
	if(!ext2_g_gdesc)
		ext2_g_gdesc = (ext2_group_descriptor_t*)kmalloc(nblock_groups * (nblock_groups * EXT2_BLOCK_SZ));

	//block bitmap, inode bitmap, inode table locations
	uint32_t block_BB, block_IB, block_IT;
	uint8_t write_data = 0x0;

	//block bitmap and inode bitmap data
	uint8_t *mem;
	mem = (uint8_t*)kmalloc(2 * EXT2_BLOCK_SZ); //two blocks for the block bitmap and inode bitmap combined
	memset(mem, write_data, 2 * EXT2_BLOCK_SZ);

	if(!ext2_g_bb)
		ext2_g_bb = (uint8_t*)kmalloc(nblock_groups * EXT2_BLOCK_SZ);

	if(!ext2_g_ib)
		ext2_g_ib = (uint8_t*)kmalloc(nblock_groups * EXT2_BLOCK_SZ);

	//inode table data
	uint8_t *i_tables;
	i_tables = (uint8_t*)kmalloc(sizeof(ext2_inode_t) * inodes_per_group);
	memset(i_tables, write_data, sizeof(ext2_inode_t) * inodes_per_group);

	if(!ext2_g_inode_table)
		ext2_g_inode_table = (ext2_inode_t*)kmalloc(nblock_groups * (sizeof(ext2_inode_t) * inodes_per_group));

	uint32_t nbgroup, group_offset;
	for(nbgroup = 0; nbgroup < nblock_groups; nbgroup++)
	{
		//group offset changes so the floppy_writes write to group 1, 2, 3... as nbgroup increments
		group_offset = nbgroup * blocks_per_group;

		//!write the primary super block to the floppy (SB)
		ext2_sblock_set_data(sblock_data, 0,
													group_offset + (uint32_t)((EXT2_SBLOCK_OFF) / SECTOR_SIZE),
													EXT2_ERRORS_CONTINUE, 0, "Primary");
		sblock_data->total_inodes = nblock_groups * inodes_per_group;
		sblock_data->total_blocks = (uint32_t)(orig_size / EXT2_BLOCK_SZ);
		sblock_data->blocks_per_group = (uint32_t)(blocks_per_group);
		sblock_data->inodes_per_group = inodes_per_group;
		sblock_data->block_group_number = nbgroup; //the block group number that this superblock is located in

		memcpy(ext2_g_sblock + sizeof(ext2_superblock_t) * nbgroup, sblock_data, sizeof(ext2_superblock_t));

		disk_write(curr_port, (uint32_t*)sblock_data, sizeof(ext2_superblock_t),
									group_offset + (uint32_t)((EXT2_SBLOCK_OFF) / SECTOR_SIZE)); //write to sector 2

		//!write zeros over the block bitmap and inode bitmap (BB + IB)
		/*Pertaining to block bitmap, inode bitmap, inode table:
		 *
		 * group_offset is the offset that says in what group to write the data in, 0, 1, 2, 3... etc
		 * (uint32_t)((EXT2_SBLOCK_OFF + EXT2_BLOCK_SZ) / EXT2_BLOCK_SZ) + nblock_groups is the offset from the super block and group descriptor
		 * the [(uint32_t)((EXT2_SBLOCK_OFF + EXT2_BLOCK_SZ) / EXT2_BLOCK_SZ] part is from the superblock,
		 * which ends 2 blocks from the begining of the floppy
		 * + nblock_groups is the offset that the group descriptor has as it takes
		 * up the (number of block groups) blocks, i.e. if there are 2 block groups, the group descriptor would take up 2 blocks
		 *
		 * the multiplication in front of the first EXT2_BLOCK_SZ is an additional offset
		 * for example, the inode bitmap is 1 block after the block bitmap, so another EXT2_BLOCK_SZ offset is necessary*/
		block_BB = group_offset + (uint32_t)((EXT2_SBLOCK_OFF + EXT2_BLOCK_SZ +
																				EXT2_BLOCK_SZ * nblock_groups) / SECTOR_SIZE);
		block_IB = group_offset + (uint32_t)((EXT2_SBLOCK_OFF + 2 * EXT2_BLOCK_SZ +
																				EXT2_BLOCK_SZ * nblock_groups) / SECTOR_SIZE);

		//~ printf("BB %d IB %d\n", block_BB, block_IB);
		memcpy(ext2_g_bb + EXT2_BLOCK_SZ * nbgroup, mem, EXT2_BLOCK_SZ);
		memcpy(ext2_g_ib + EXT2_BLOCK_SZ * nbgroup, mem, EXT2_BLOCK_SZ);

		disk_write(curr_port, (uint32_t*)mem, 2 * EXT2_BLOCK_SZ, block_BB); //write to sector 3 and 5

		//!write zeros over the inode tables (IT)
		block_IT = group_offset + (uint32_t)((EXT2_SBLOCK_OFF + 3 * EXT2_BLOCK_SZ +
																				EXT2_BLOCK_SZ * nblock_groups) / SECTOR_SIZE);

		memcpy(ext2_g_inode_table + sizeof(ext2_inode_t) * inodes_per_group *
					 nbgroup, i_tables, sizeof(ext2_inode_t) * inodes_per_group);
		disk_write(curr_port, (uint32_t*)i_tables, sizeof(ext2_inode_t) * inodes_per_group, block_IT); //write to sector 5

		uint32_t gdesc;
		for(gdesc = 0; gdesc < nblock_groups; gdesc++)
		{
			//!write the group block descriptors (GBD)
			ext2_set_gdesc_table(gdesc_table_data);

			//Now that I have the block numbers for the block bitmap, inode bitmap, and inode tables, add them to group descriptor
			gdesc_table_data->block_bitmap = block_BB;
			gdesc_table_data->inode_bitmap = block_IB;
			gdesc_table_data->inode_table_id = block_IT;

			/*takes the blocks in one group, subtracts 3 for the super block, block bitmap, and inode bitmap
			 * then subtracts the size of the inode table for this partition
			 * and subtracts the number of block groups there are, since for every block group the is, 1 block is added to the descriptor tables */
			gdesc_table_data->free_blocks = blocks_per_group - (3 + inode_table_size + nblock_groups);

			gdesc_table_data->free_inodes = inodes_per_group;
			gdesc_table_data->inode_table_size = inode_table_size;

			gdesc_table_data->gdesc_location = group_offset + (uint32_t)((EXT2_SBLOCK_OFF + EXT2_BLOCK_SZ) / SECTOR_SIZE);

			//write the actual contents in the beggining of the EXT2_BLOCK_SZ of the buffer
			memcpy((uint32_t*)(gdesc_buf + gdesc * EXT2_BLOCK_SZ), (uint32_t*)gdesc_table_data, sizeof(ext2_group_descriptor_t));

			//fill the rest of the EXT2_BLOCK_SZ of the buffer that was not used with zeros
			memset((uint32_t*)(gdesc_buf + gdesc * EXT2_BLOCK_SZ + sizeof(ext2_group_descriptor_t)), 0x0, EXT2_BLOCK_SZ - sizeof(ext2_group_descriptor_t));

		}

		memcpy(ext2_g_gdesc + nbgroup * nblock_groups * EXT2_BLOCK_SZ, gdesc_buf, nblock_groups * EXT2_BLOCK_SZ);

		disk_write(curr_port, (uint32_t*)gdesc_buf, nblock_groups * EXT2_BLOCK_SZ,
								group_offset + (uint32_t)((EXT2_SBLOCK_OFF + EXT2_BLOCK_SZ) / SECTOR_SIZE)); //write starting from sector 2
	}

	kfree((uint32_t*)mem);
	kfree((uint32_t*)i_tables);

	kfree((uint32_t*)sblock_data);
	kfree((uint32_t*)gdesc_table_data);
	kfree((uint32_t*)gdesc_buf);

	//sucess!
	return 0;
}

ext2_inode_t *ext2_inode_from_offset(uint32_t inode_number)
{
	//get the inode table
	ext2_inode_t *inode_table;

	if(!ext2_g_inode_table)
	{
		ext2_superblock_t *sblock;
		ext2_group_descriptor_t *gdesc;

		if(!ext2_g_sblock || !ext2_g_gdesc)
		{
			if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
				return 0; //error
		}else{
			sblock = ext2_g_sblock;
			gdesc = ext2_g_gdesc;

		}

		inode_table = ext2_get_inode_table(gdesc);
		ext2_g_inode_table = inode_table;
	}else
		inode_table = ext2_g_inode_table;

	ext2_inode_t *buffer;
	buffer = (ext2_inode_t*)kmalloc(sizeof(ext2_inode_t));

	memcpy(buffer, (uint8_t*)inode_table + sizeof(ext2_inode_t) * inode_number, sizeof(ext2_inode_t));

	//sucess!
	return buffer;
}

ext2_inode_t *ext2_get_inode_table(ext2_group_descriptor_t *gdesc)
{
	ext2_inode_t *buffer;

	if(!ext2_g_inode_table)
	{
		buffer = (ext2_inode_t*)kmalloc(gdesc->inode_table_size * EXT2_BLOCK_SZ);
		disk_read(curr_port, gdesc->inode_table_id, gdesc->inode_table_size * EXT2_BLOCK_SZ, (uint32_t*)buffer);
		ext2_g_inode_table = buffer;
	}else
		buffer = ext2_g_inode_table;

	return buffer;
}

static char *__get_name_of_file__(ext2_inode_t *directory, ext2_inode_t *file)
{
	uint32_t i = 0;

	struct ext2_dirent *dirent;

	//loop forever untill we exit by sucess of an error
	for(i;1;i++)
	{
		dirent = ext2_dirent_from_dir(directory, i);

		if(!dirent)
			return 0; //fail

		//have we found the name we are looking for
		if(dirent->ino == file->inode)
			return dirent->name;
	}

}

char *ext2_get_name_of_dir(ext2_inode_t *directory)
{
	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;

	if(!ext2_g_sblock || !ext2_g_gdesc)
	{
		if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
			return 0; //error
	}else{
		sblock = ext2_g_sblock;
		gdesc = ext2_g_gdesc;

	}

	//get the inode table
	ext2_inode_t *inode_table;

	if(!ext2_g_inode_table)
	{
		inode_table = ext2_get_inode_table(gdesc);
		ext2_g_inode_table = inode_table;
	}else
		inode_table = ext2_g_inode_table;

	if(directory->inode == ext2_root->inode)
		return ext2_root_name;

	struct ext2_dirent *dirent;
	//get the inode of the parent, always the second index (1) starting from 0
	dirent = ext2_dirent_from_dir(directory, 1);

	ext2_inode_t *parent;
	parent = ext2_inode_from_offset(dirent->ino);

	uint32_t i = 0, b = 0, *block;
	block = (uint32_t*)kmalloc(EXT2_BLOCK_SZ);
	if(!ext2_block_of_set(parent, b, block))
	{
		kfree((uint32_t*)parent);
		kfree((uint32_t*)block);
		return 0;
	}

	//now that we have the parent directory's data, scan through it for our directory
	do
	{
		dirent = ext2_dirent_from_dir_data(parent, i, block);
		i++;

		if(!dirent)
		{
			b++;
			if(!ext2_block_of_set(parent, b, block))
			{
				kfree((uint32_t*)parent);
				kfree((uint32_t*)block);
				return 0;
			}
		}
	}
	while(dirent->file_type != EXT2_DIR || dirent->ino != directory->inode);

	char *name;
	name = dirent->name;
	//~ name = (char*)kmalloc(dirent->name_len + 1);
	//~ *(name + dirent->name_len) = 0;

	kfree((uint32_t*)block);
	kfree((uint32_t*)parent);

	return name;
}

uint32_t ext2_free_data_blocks(ext2_inode_t *directory, ext2_inode_t *node)
{
	printf("\n%d", (uint32_t*)directory);
	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;

	if(!ext2_g_sblock || !ext2_g_gdesc)
	{
		if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
			return 1; //error
	}else{
		sblock = ext2_g_sblock;
		gdesc = ext2_g_gdesc;
	}

	//remove the singly, doubly, and triply block locations from the block bitmap, allong with the block_locs
	uint32_t *block_locs, nblocks, *blocks_to_rm, i;
	nblocks = ((node->size - 1) / EXT2_BLOCK_SZ) + 1;

	block_locs = ext2_block_locs(node);

	/*(EXT2_N_BLOCKS - EXT2_NDIR_BLOCKS) are the singly, doubly, and triply block locations in the
	 * inode structure, nblocks are the data blocks that must be freed, we compress these two block
	 * block locations into one for efficientcy*/
	blocks_to_rm = (uint32_t*)kmalloc(sizeof(uint32_t) * ((EXT2_N_BLOCKS - EXT2_NDIR_BLOCKS) + nblocks));

	for(i = 0; i < EXT2_N_BLOCKS - EXT2_NDIR_BLOCKS; i++)
		*(blocks_to_rm + i) = *(node->blocks + EXT2_NDIR_BLOCKS + i);

	memcpy(blocks_to_rm + i, block_locs, sizeof(uint32_t) * nblocks);

	//when the for loop exits, i + nblocks will equal the number of blocks that have been saved in blocks_to_rm
	if(ext2_free_blocks(blocks_to_rm, i + nblocks))
	{
		kfree((uint32_t*)block_locs);
		kfree((uint32_t*)blocks_to_rm);

		return 1; //there was some sort of error
	}

	//increment the number of free inodes and blocks freed, update the new gdesc
	gdesc->free_inodes++;
	gdesc->free_blocks += nblocks;

	disk_write(curr_port, (uint32_t*)gdesc, sizeof(ext2_group_descriptor_t), gdesc->gdesc_location);

	kfree((uint32_t*)block_locs);
	kfree((uint32_t*)blocks_to_rm);

	//sucess!
	return 0;
}

uint32_t ext2_remove_dirent(ext2_inode_t *directory, ext2_inode_t *node)
{
	if(directory->type == EXT2_DIR) //just to check if the input node is a directory
	{

		struct ext2_dirent *dirent2;
		dirent2 = (struct ext2_dirent*)kmalloc(sizeof(struct ext2_dirent));

		uint32_t i = 0, b = 0;
	/**	uint32_t i = 0, loop = 0, b = 0;**/

		//allocate a buffer to store a directory's data
		uint8_t *buf, **split, *dir_block;
		buf = (uint8_t*)kmalloc(directory->size);
		split = (uint8_t**)kmalloc(directory->size / EXT2_BLOCK_SZ);

		ext2_read(directory, 0, directory->size, buf);

		//split the buffer into 1024 byte sections
		for(i = 0; i < directory->size / EXT2_BLOCK_SZ; i++)
		{
			*(split + i) = (uint8_t*)kmalloc(EXT2_BLOCK_SZ);
			memcpy(*(split + i), buf + i * EXT2_BLOCK_SZ, EXT2_BLOCK_SZ);
		}

		//we do not need buf anymore, free it
		kfree((uint32_t*)buf);

		//set dir_block to the first block of data
		dir_block = *split;

		if(!dir_block)
			return 1; //error

		/*this section looks through the direcetory and finds the location
		 * of the dirent of the file that we want to remove */
		do
		{

			//if we found the file in the directory's contents
			if(*(uint32_t*)(*(uint32_t*)dir_block + i) == node->inode)
			{

				dirent2->ino = *(uint32_t*)(*(uint32_t*)dir_block + i);
				dirent2->rec_len = *(uint16_t*)(*(uint32_t*)dir_block + i + sizeof(dirent2->ino));

				break;

			}else{

				//if the next rec_length is 0, then we are currently at the last dirent, else increase the offset (i)
				if(*(uint16_t*)(*(uint32_t*)dir_block + i + sizeof(dirent2->ino)))
					/*increase i with the rec_len that we get by moving fileheader sizeof(dirent2->ino)
					 * (4 bytes) and reading its value*/
					i += *(uint16_t*)(*(uint32_t*)dir_block + i + sizeof(dirent2->ino));
				else{ //this is the last direct, add 1 to block and reset the offset (i)
					i = 0;
					b++;

					if(b > (uint32_t)((directory->size - 1) / EXT2_BLOCK_SZ))
						//error, block exceds the number of blocks dirNode has
						return 1;

					//recalculate the dir_block
					dir_block = *(split + b);

					if(!dir_block)
						return 1; //error

				}

			}

		}
		while(1);


		/*shifts the dirent data in the directory
		 * the "-1 *" is used to show shift to the left */
		shiftData((uint32_t*)(*(uint32_t*)dir_block + i + dirent2->rec_len),
							-1 * dirent2->rec_len, EXT2_BLOCK_SZ - i - dirent2->rec_len);

		//write the changed block contents to the actual directory
		ext2_write(directory, b * EXT2_BLOCK_SZ, EXT2_BLOCK_SZ, dir_block);

		//free split, it is not needed anymore
		for(i = 0; i < directory->size / EXT2_BLOCK_SZ; i++)
			kfree((uint32_t*)*(split + i));

		kfree((uint32_t*)split);

		kfree((uint32_t*)dirent2);

		//sucess!
		return 0;

	}
	return 1;
}

static ext2_inode_t *__create_root__()
{
	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;

	if(!ext2_g_sblock || !ext2_g_gdesc)
	{
		if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
		{
			printf("\nUnknown Error #111");
			return 0; //error
		}
	}else{
		sblock = ext2_g_sblock;
		gdesc = ext2_g_gdesc;

	}

	ext2_inode_t *data;
	data = (ext2_inode_t*)kmalloc(sizeof(ext2_inode_t));

	//the root is always with an inode of 0, the root inode
	data->magic = M_EXT2;
	data->inode = 0;

	data->mode = EXT2_I_RUSR | EXT2_I_WUSR | EXT2_I_XUSR | EXT2_I_RGRP | EXT2_I_XGRP | EXT2_I_ROTH | EXT2_I_XOTH;
	data->type = EXT2_DIR;
	data->uid = 0;
	data->size = 0;
	data->atime = posix_time();
	data->ctime = posix_time();
	data->mtime = posix_time();
	data->dtime = 0;
	data->gid = 0;
	data->nlinks = 2;    //number of hard links (itself and the '.' directory)
	data->nblocks = 0;   //blocks of 512 bytes
	data->flags = 0;
	data->osd1 = EXT2_OS_JSOS;

	//set all of the blocks to 0 (empty)
	uint32_t blank;
	for(blank = 0; blank < EXT2_N_BLOCKS; blank++)
		data->blocks[blank] = 0;

	data->version = 0;
	data->fire_acl = 0;
	data->dir_acl = 0;
	data->fragment_addr = 0;

	//add the inode data to the table
	ext2_data_to_inode_table(data, gdesc, sblock);

	//decrement the number of free inodes and blocks there are
	gdesc->free_inodes--;

	disk_write(curr_port, (uint32_t*)gdesc, sizeof(ext2_group_descriptor_t), gdesc->gdesc_location);

	//purposly not freeing since sblock and gdesc, regardless of update, is the global variable which should not be cleared

	return data;
}

void make_boot_sector_ext2()
{
		uint32_t buf=(uint32_t)fsalloc(1024);
		memset((void*)buf,0,1024);
		read(curr_port,0,2,(DWORD)buf);
		Identity_Sectors_t* identity=(Identity_Sectors_t*)(buf + 436);
		strcpy(identity->name,"EXT2");
		identity->active_partition = 446; //Partition 1

		uint8_t* boot_ptr = (uint8_t*)buf;
		boot_ptr += 510;
		*boot_ptr = 0x55;
		++boot_ptr;
		*boot_ptr = 0xAA;
		write(curr_port,0,2,(DWORD)buf); //2nd sector.
}

uint32_t ext2_burn(uint32_t size, const char* device)
{
	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;
	printf("\n<%s>", device);
	/*if ext2_read_meta_data returns with anything but a 0,
	 * then there is either an error in the sblock, or one does not exist*/
	if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
	{
		make_boot_sector_ext2();
		printf("\n\tFormating required ext2 structures, this may take some time\n");

		printf("\tCreating the superblock and group descriptor table...");
		ext2_set_block_group(size);
		printf("%ggdone%gw\n",10,15);

		ext2_inode_t *root; //, *file, *dir;

		printf("\tCreating the root directory...");
		//create the root directory
		root = ext2_create_dir(__create_root__(), "/");
		printf("%ggdone%gw\n", 10, 15);

		ext2_root = root;
		Ext2_currDir = ext2_root;

		if(!ext2_root)
		{
			printf("\nUnknown Error #1\n");
			return 1; //error
		}

		ext2_path = (char*)kmalloc(2);
		*(ext2_path) = '/';
		*(ext2_path + 1) = 0;

		printf("\tCreating root \".\" hardlink...");
		if(ext2_add_hardlink_to_dir(root, root, ".")) //adds hardlink to root
		{
			printf("\nUnknown Error #1\n");
			return 1; //error
		}
		printf("%ggdone%gw\n", 10, 15);

		printf("\tCreating root \"..\" hardlink...");
		if(ext2_add_hardlink_to_dir(root, root, "..")) //adds hardlink to root
		{
			printf("\nUnknown Error #1\n");
			return 1; //error
		}
		printf("%ggdone%gw\n", 10, 15);

		//set the name of the root to '/'
		ext2_root_name = (char*)kmalloc(2);
		*(ext2_root_name) = '/';
		*(ext2_root_name + 1) = 0;

		//~ ext2_set_current_dir(root);

		//sucess!
		printf("\nExt2 Formatted sucessfully\n");
		return 0;
	}
	return 1;
}

uint32_t ext2_initialize(uint32_t size, const char *device)
{
	ext2_superblock_t *sblock;
	ext2_group_descriptor_t *gdesc;

	printf("\n<%d, %s>", size, device);
	/*if ext2_read_meta_data returns with anything but a 0,
	 * then there is either an error in the sblock, or one does not exist*/
	if(ext2_read_meta_data((ext2_superblock_t**)&sblock, (ext2_group_descriptor_t**)&gdesc))
	{
		printf("The disk does not have preexisting data on it\n");
		return 0;
	}
	else
	{
		printf("The floppy disk has preexisting data on it!\n");

		printf("\n creator_os: %x \n Block_size: %x\n",sblock->creator_os,sblock->block_size);

		ext2_inode_t *inode_table;

		if(!ext2_g_inode_table)
		{
			inode_table = ext2_get_inode_table(gdesc);
			ext2_g_inode_table = inode_table;
		}else
			inode_table = ext2_g_inode_table;

		printf("\tAcquiring preexisting root directory...");
		//get the root, it is always second after the 'mother root'


		ext2_root = ext2_inode_from_offset(1);
		Ext2_currDir = ext2_root;
	/*	ext2_dirent_t* ext2_root_dirent = ext2_dirent_from_dir(ext2_root, 2);


		printf("%ggdone%gw\n%s>", 10, 15, ext2_root_dirent->name);
		//printf("%ggdone%gw\n", 10, 15);
s
		ext2_inode_t* fnode = ext2_root;
	  ext2_dirent_t* entry = 0;

	  for(int i = 0; i < 2; i++)
	  {
	    entry = ext2_dirent_from_dir(fnode, i);
	    printf("\n%s", entry->name);
	  }
*/
		if(!ext2_root)
			return 1; //error

		//set the name of the root to '/'
		ext2_root_name = (char*)kmalloc(2);
		*(ext2_root_name) = '/';
		*(ext2_root_name + 1) = 0;
	 	//~ ext2_set_current_dir(ext2_root);

		//sucess!
		return 0;
	}
}
