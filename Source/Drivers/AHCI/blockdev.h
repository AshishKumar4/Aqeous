

#ifndef __DRIVERS_STORAGE_BLOCKDEV_H__
#define __DRIVERS_STORAGE_BLOCKDEV_H__

#include <stdint.h>

#include "list.h"
#include "stream.h"

typedef uint32_t lba_t;

typedef struct BlockDevOps {
	lba_t (*read)(struct BlockDevOps *me, lba_t start, lba_t count,
		      void *buffer);
	lba_t (*write)(struct BlockDevOps *me, lba_t start, lba_t count,
		       const void *buffer);
	lba_t (*fill_write)(struct BlockDevOps *me, lba_t start, lba_t count,
			    uint8_t fill_byte);
	StreamOps *(*new_stream)(struct BlockDevOps *me, lba_t start,
				 lba_t count);
} BlockDevOps;

typedef struct BlockDev {
	BlockDevOps ops;

	const char *name;
	int removable;
	int external_gpt;
	unsigned block_size;
	/* If external_gpt = 0, then stream_block_count may be 0, indicating
	 * that the block_count value applies for both read/write and streams */
	lba_t block_count;		/* size addressable by read/write */
	lba_t stream_block_count;	/* size addressible by new_stream */

	ListNode list_node;
} BlockDev;

extern ListNode fixed_block_devices;
extern ListNode removable_block_devices;

typedef struct BlockDevCtrlrOps {
	int (*update)(struct BlockDevCtrlrOps *me);
	/*
	 * Check if a block device is owned by the ctrlr. 1 = success, 0 =
	 * failure
	 */
	int (*is_bdev_owned)(struct BlockDevCtrlrOps *me, BlockDev *bdev);
} BlockDevCtrlrOps;

typedef struct BlockDevCtrlr {
	BlockDevCtrlrOps ops;

	int need_update;
	ListNode list_node;
} BlockDevCtrlr;

extern ListNode fixed_block_dev_controllers;
extern ListNode removable_block_dev_controllers;

StreamOps *new_simple_stream(BlockDevOps *me, lba_t start, lba_t count);

typedef enum {
	BLOCKDEV_FIXED,
	BLOCKDEV_REMOVABLE,
} blockdev_type_t;

int get_all_bdevs(blockdev_type_t type, ListNode **bdevs);

#endif /* __DRIVERS_STORAGE_BLOCKDEV_H__ */
