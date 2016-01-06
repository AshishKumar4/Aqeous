#ifndef ATA_h
#define ATA_h
#include <sys.h>
#include <system.h>

typedef unsigned char BYTE;
typedef unsigned short DWORD;
typedef unsigned long QWORD;
typedef unsigned short WORD;

typedef struct ident_device_t {
	unsigned short	config;		/* lots of obsolete bit flags */
	unsigned short	cyls;		/* "physical" cyls */
	unsigned short	reserved2;	/* reserved (word 2) */
	unsigned short	heads;		/* "physical" heads */
	unsigned short	track_bytes;	/* unformatted bytes per track */
	unsigned short	sector_bytes;	/* unformatted bytes per sector */
	unsigned short	sectors;	/* "physical" sectors per track */
	unsigned short	vendor0;	/* vendor unique */
	unsigned short	vendor1;	/* vendor unique */
	unsigned short	vendor2;	/* vendor unique */
	unsigned char	serial_no[20];	/* 0 = not_specified */
	unsigned short	buf_type;
	unsigned short	buf_size;	/* 512 byte increments; 0 = not_specified */
	unsigned short	ecc_bytes;	/* for r/w long cmds; 0 = not_specified */
	unsigned char	fw_rev[8];	/* 0 = not_specified */
	unsigned char	model[40];	/* 0 = not_specified */
	unsigned char	max_multsect;	/* 0=not_implemented */
	unsigned char	vendor3;	/* vendor unique */
	unsigned short	dword_io;	/* 0=not_implemented; 1=implemented */
	unsigned char	vendor4;	/* vendor unique */
	unsigned char	capability;	/* bits 0:DMA 1:LBA 2:IORDYsw 3:IORDYsup*/
	unsigned short	reserved50;	/* reserved (word 50) */
	unsigned char	vendor5;	/* vendor unique */
	unsigned char	tPIO;		/* 0=slow, 1=medium, 2=fast */
	unsigned char	vendor6;	/* vendor unique */
	unsigned char	tDMA;		/* 0=slow, 1=medium, 2=fast */
	unsigned short	field_valid;	/* bits 0:cur_ok 1:eide_ok */
	unsigned short	cur_cyls;	/* logical cylinders */
	unsigned short	cur_heads;	/* logical heads */
	unsigned short	cur_sectors;	/* logical sectors per track */
	unsigned short	cur_capacity0;	/* logical total sectors on drive */
	unsigned short	cur_capacity1;	/*  (2 words, misaligned int)     */
	unsigned char	multsect;	/* current multiple sector count */
	unsigned char	multsect_valid;	/* when (bit0==1) multsect is ok */
	unsigned int	lba_capacity;	/* total number of sectors */
	unsigned short	dma_1word;	/* single-word dma info */
	unsigned short	dma_mword;	/* multiple-word dma info */
	unsigned short  eide_pio_modes; /* bits 0:mode3 1:mode4 */
	unsigned short  eide_dma_min;	/* min mword dma cycle time (ns) */
	unsigned short  eide_dma_time;	/* recommended mword dma cycle time (ns) */
	unsigned short  eide_pio;       /* min cycle time (ns), no IORDY  */
	unsigned short  eide_pio_iordy; /* min cycle time (ns), with IORDY */
	unsigned short	words69_70[2];	/* reserved words 69-70 */
	unsigned short	words71_74[4];	/* reserved words 71-74 */
	unsigned short  queue_depth;	/*  */
	unsigned short  words76_79[4];	/* reserved words 76-79 */
	unsigned short  major_rev_num;	/*  */
	unsigned short  minor_rev_num;	/*  */
	unsigned short  command_set_1;	/* bits 0:Smart 1:Security 2:Removable 3:PM */
	unsigned short  command_set_2;	/* bits 14:Smart Enabled 13:0 zero */
	unsigned short  cfsse;		/* command set-feature supported extensions */
	unsigned short  cfs_enable_1;	/* command set-feature enabled */
	unsigned short  cfs_enable_2;	/* command set-feature enabled */
	unsigned short  csf_default;	/* command set-feature default */
	unsigned short  dma_ultra;	/*  */
	unsigned short	word89;		/* reserved (word 89) */
	unsigned short	word90;		/* reserved (word 90) */
	unsigned short	CurAPMvalues;	/* current APM values */
	unsigned short	word92;		/* reserved (word 92) */
	unsigned short	hw_config;	/* hardware config */
	unsigned short  words94_125[32];/* reserved words 94-125 */
	unsigned short	last_lun;	/* reserved (word 126) */
	unsigned short	word127;	/* reserved (word 127) */
	unsigned short	dlf;		/* device lock function
					 * 15:9	reserved
					 * 8	security level 1:max 0:high
					 * 7:6	reserved
					 * 5	enhanced erase
					 * 4	expire
					 * 3	frozen
					 * 2	locked
					 * 1	en/disabled
					 * 0	capability
					 */
	unsigned short  csfo;		/* current set features options
					 * 15:4	reserved
					 * 3	auto reassign
					 * 2	reverting
					 * 1	read-look-ahead
					 * 0	write cache
					 */
	unsigned short	words130_155[26];/* reserved vendor words 130-155 */
	unsigned short	word156;
	unsigned short	words157_159[3];/* reserved vendor words 157-159 */
	unsigned short	words160_255[95];/* reserved words 160-255 */
} IDENTIFYDEVICE;

IDENTIFYDEVICE ident;

typedef enum
{
	FIS_TYPE_REG_H2D	= 0x27,	// Register FIS - host to device
	FIS_TYPE_REG_D2H	= 0x34,	// Register FIS - device to host
	FIS_TYPE_DMA_ACT	= 0x39,	// DMA activate FIS - device to host
	FIS_TYPE_DMA_SETUP	= 0x41,	// DMA setup FIS - bidirectional
	FIS_TYPE_DATA		= 0x46,	// Data FIS - bidirectional
	FIS_TYPE_BIST		= 0x58,	// BIST activate FIS - bidirectional
	FIS_TYPE_PIO_SETUP	= 0x5F,	// PIO setup FIS - device to host
	FIS_TYPE_DEV_BITS	= 0xA1,	// Set device bits FIS - device to host
} FIS_TYPE;

typedef struct tagFIS_REG_H2D
{
	// DWORD 0
	BYTE	fis_type;	// FIS_TYPE_REG_H2D

	BYTE	pmport:4;	// Port multiplier
	BYTE	rsv0:3;		// Reserved
	BYTE	c:1;		// 1: Command, 0: Control

	BYTE	command;	// Command register
	BYTE	featurel;	// Feature register, 7:0

	// DWORD 1
	BYTE	lba0;		// LBA low register, 7:0
	BYTE	lba1;		// LBA mid register, 15:8
	BYTE	lba2;		// LBA high register, 23:16
	BYTE	device;		// Device register

	// DWORD 2
	BYTE	lba3;		// LBA register, 31:24
	BYTE	lba4;		// LBA register, 39:32
	BYTE	lba5;		// LBA register, 47:40
	BYTE	featureh;	// Feature register, 15:8

	// DWORD 3
	BYTE	countl;		// Count register, 7:0
	BYTE	counth;		// Count register, 15:8
	BYTE	icc;		// Isochronous command completion
	BYTE	control;	// Control register

	// DWORD 4
	BYTE	rsv1[4];	// Reserved
} FIS_REG_H2D;

typedef struct tagFIS_REG_D2H
{
	// DWORD 0
	BYTE	fis_type;    // FIS_TYPE_REG_D2H

	BYTE	pmport:4;    // Port multiplier
	BYTE	rsv0:2;      // Reserved
	BYTE	i:1;         // Interrupt bit
	BYTE	rsv1:1;      // Reserved

	BYTE	status;      // Status register
	BYTE	error;       // Error register

	// DWORD 1
	BYTE	lba0;        // LBA low register, 7:0
	BYTE	lba1;        // LBA mid register, 15:8
	BYTE	lba2;        // LBA high register, 23:16
	BYTE	device;      // Device register

	// DWORD 2
	BYTE	lba3;        // LBA register, 31:24
	BYTE	lba4;        // LBA register, 39:32
	BYTE	lba5;        // LBA register, 47:40
	BYTE	rsv2;        // Reserved

	// DWORD 3
	BYTE	countl;      // Count register, 7:0
	BYTE	counth;      // Count register, 15:8
	BYTE	rsv3[2];     // Reserved

	// DWORD 4
	BYTE	rsv4[4];     // Reserved
} FIS_REG_D2H;

typedef struct tagFIS_DATA
{
	// DWORD 0
	BYTE	fis_type;	// FIS_TYPE_DATA

	BYTE	pmport:4;	// Port multiplier
	BYTE	rsv0:4;		// Reserved

	BYTE	rsv1[2];	// Reserved

	// DWORD 1 ~ N
	DWORD	data[1];	// Payload
} FIS_DATA;

typedef struct tagFIS_PIO_SETUP
{
	// DWORD 0
	BYTE	fis_type;	// FIS_TYPE_PIO_SETUP

	BYTE	pmport:4;	// Port multiplier
	BYTE	rsv0:1;		// Reserved
	BYTE	d:1;		// Data transfer direction, 1 - device to host
	BYTE	i:1;		// Interrupt bit
	BYTE	rsv1:1;

	BYTE	status;		// Status register
	BYTE	error;		// Error register

	// DWORD 1
	BYTE	lba0;		// LBA low register, 7:0
	BYTE	lba1;		// LBA mid register, 15:8
	BYTE	lba2;		// LBA high register, 23:16
	BYTE	device;		// Device register

	// DWORD 2
	BYTE	lba3;		// LBA register, 31:24
	BYTE	lba4;		// LBA register, 39:32
	BYTE	lba5;		// LBA register, 47:40
	BYTE	rsv2;		// Reserved

	// DWORD 3
	BYTE	countl;		// Count register, 7:0
	BYTE	counth;		// Count register, 15:8
	BYTE	rsv3;		// Reserved
	BYTE	e_status;	// New value of status register

	// DWORD 4
	WORD	tc;		// Transfer count
	BYTE	rsv4[2];	// Reserved
} FIS_PIO_SETUP;




typedef struct tagFIS_DMA_SETUP
{
	// DWORD 0
	BYTE	fis_type;	// FIS_TYPE_DMA_SETUP

	BYTE	pmport:4;	// Port multiplier
	BYTE	rsv0:1;		// Reserved
	BYTE	d:1;		// Data transfer direction, 1 - device to host
	BYTE	i:1;		// Interrupt bit
	BYTE	a:1;            // Auto-activate. Specifies if DMA Activate FIS is needed

        BYTE    rsved[2];       // Reserved

	//DWORD 1&2

        QWORD   DMAbufferID;    // DMA Buffer Identifier. Used to Identify DMA buffer in host memory. SATA Spec says host specific and not in Spec. Trying AHCI spec might work.

        //DWORD 3
        DWORD   rsvd;           //More reserved

        //DWORD 4
        DWORD   DMAbufOffset;   //Byte offset into buffer. First 2 bits must be 0

        //DWORD 5
        DWORD   TransferCount;  //Number of bytes to transfer. Bit 0 must be 0

        //DWORD 6
        DWORD   resvd;          //Reserved

} FIS_DMA_SETUP;


 char HDD;
unsigned short drive;

#endif
