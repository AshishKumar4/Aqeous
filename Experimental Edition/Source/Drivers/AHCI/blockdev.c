

#include "blockdev.h"

#include <assert.h>
#include <stdio.h>

ListNode fixed_block_devices;
ListNode removable_block_devices;

ListNode fixed_block_dev_controllers;
ListNode removable_block_dev_controllers;

typedef struct {
	StreamOps stream;
	BlockDev *blockdev;
	lba_t current_sector;
	lba_t end_sector;
} SimpleStream;

uint32_t simple_stream_read(StreamOps *me, uint32_t count, void *buffer)
{
	SimpleStream *stream = container_of(me, SimpleStream, stream);
	unsigned block_size = stream->blockdev->block_size;

	/* TODO(dehrenberg): implement buffering so that unaligned reads are
	 * possible because alignment constraints are obscure */
	if (count & (block_size - 1)) {
		printf("read_stream_simple(%lld) not LBA multiple\n", count);
		return 0;
	}

	uint64_t sectors = count / block_size;
	if (sectors > stream->end_sector - stream->current_sector) {
		printf("read_stream_simple past the end, "
		       "end_sector=%lld, current_sector=%lld, sectors=%lld\n",
		       stream->end_sector, stream->current_sector, sectors);
		return 0;
	}

	int ret = stream->blockdev->ops.read(&stream->blockdev->ops,
					     stream->current_sector, sectors,
					     buffer);
	if (ret != sectors)
		return ret;

	stream->current_sector += sectors;
	return count;
}

static void simple_stream_close(StreamOps *me)
{
	SimpleStream *stream = container_of(me, SimpleStream, stream);
	kfree(stream);
}

StreamOps *new_simple_stream(BlockDevOps *me, lba_t start, lba_t count)
{
	BlockDev *blockdev = (BlockDev *)me;
	SimpleStream *stream = kmalloc(sizeof(*stream));
	stream->blockdev = blockdev;
	stream->current_sector = start;
	stream->end_sector = start + count;
	stream->stream.read = simple_stream_read;
	stream->stream.close = simple_stream_close;
	/* Check that block size is a power of 2 */
	assert((blockdev->block_size & (blockdev->block_size - 1)) == 0);
	return &stream->stream;
}

int get_all_bdevs(blockdev_type_t type, ListNode **bdevs)
{
	ListNode *ctrlrs, *devs;
	int count = 0;

	if (type == BLOCKDEV_FIXED) {
		devs = &fixed_block_devices;
		ctrlrs = &fixed_block_dev_controllers;
	} else {
		devs = &removable_block_devices;
		ctrlrs = &removable_block_dev_controllers;
	}

	/* Update any controllers that need it. */
	BlockDevCtrlr *ctrlr;
	list_for_each(ctrlr, *ctrlrs, list_node) {
		if (ctrlr->ops.update && ctrlr->need_update &&
		    ctrlr->ops.update(&ctrlr->ops))
			printf("Updating storage controller failed.\n");
	}

	/* Count the devices. */
	for (ListNode *node = devs->next; node; node = node->next, count++)
		;

	*bdevs = devs;
	return count;
}
