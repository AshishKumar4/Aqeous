
#ifndef __DRIVERS_STORAGE_STREAM_H__
#define __DRIVERS_STORAGE_STREAM_H__

#include <stdint.h>

/*
 * StreamOps is a single-use stream. You open it from a StreamCtrlr with a
 * particular offset and size, and do successive reads to it until it is
 * exhausted. The offset and size are in terms of physical parameters for
 * the underlying medium and the size found in practice may be smaller,
 * e.g., due to skipping bad blocks on NAND.
 */
typedef struct StreamOps {
	uint32_t (*read)(struct StreamOps *me, uint32_t count,
			 void *buffer);
	void (*close)(struct StreamOps *me);
} StreamOps;

/* Lazily initialized representation of a device; factory for streams to
 * read part of the device */
typedef struct StreamCtrlr {
	/* Create a new StreamOps */
	StreamOps *(*open)(struct StreamCtrlr *me, uint32_t offset,
			uint32_t size);
	/* Get the size of the underlying device in bytes, i.e., the upper
	 * bound for offset+size in open() calls */
	uint32_t (*size)(struct StreamCtrlr *me);
} StreamCtrlr;

#endif /* __DRIVERS_STORAGE_STREAM_H__ */

