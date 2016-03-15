#ifndef ATA_h
#define ATA_h
#include <sys.h>
#include <system.h>

#define ATA_SR_BSY     0x80
#define ATA_SR_DRDY    0x40
#define ATA_SR_DF      0x20
#define ATA_SR_DSC     0x10
#define ATA_SR_DRQ     0x08
#define ATA_SR_CORR    0x04
#define ATA_SR_IDX     0x02
#define ATA_SR_ERR     0x01

#define ATA_ER_BBK      0x80
#define ATA_ER_UNC      0x40
#define ATA_ER_MC       0x20
#define ATA_ER_IDNF     0x10
#define ATA_ER_MCR      0x08
#define ATA_ER_ABRT     0x04
#define ATA_ER_TK0NF    0x02
#define ATA_ER_AMNF     0x01
/*
#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1*/
#define ATA_CMD_IDENTIFY          0xEC

#define ATAPI_CMD_READ       0xA8
#define ATAPI_CMD_EJECT      0x1B

#define IDE_ATA        0x00
#define IDE_ATAPI      0x01

#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01

#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

// Channels:
#define ATA_PRIMARY      0x00
#define ATA_SECONDARY    0x01

// Directions:
#define ATA_READ      0x00
#define ATA_WRITE     0x01


typedef enum AtaCommand {
	ATA_CMD_NOP = 0x00,
	ATA_CMD_CFA_REQUEST_EXTENDED_ERROR = 0x03,
	ATA_CMD_DEVICE_RESET = 0x08,
	ATA_CMD_READ_SECTORS = 0x20,
	ATA_CMD_READ_SECTORS_EXT = 0x24,
	ATA_CMD_READ_DMA_EXT = 0x25,
	ATA_CMD_READ_DMA_QUEUED_EXT = 0x26,
	ATA_CMD_READ_NATIVE_MAX_ADDRESS_EXT = 0x27,
	ATA_CMD_READ_MULTIPLE_EXT = 0x29,
	ATA_CMD_READ_STREAM_DMA_EXT = 0x2a,
	ATA_CMD_READ_STREAM_EXT = 0x2b,
	ATA_CMD_READ_LOG_EXT = 0x2f,
	ATA_CMD_WRITE_SECTORS = 0x30,
	ATA_CMD_WRITE_SECTORS_EXT = 0x34,
	ATA_CMD_WRITE_DMA_EXT = 0x35,
	ATA_CMD_WRITE_DMA_QUEUED_EXT = 0x36,
	ATA_CMD_SET_MAX_ADDRESS_EXT = 0x37,
	ATA_CMD_CFA_WRITE_SECTORS_WITHOUT_ERASE = 0x38,
	ATA_CMD_WRITE_MULTIPLE_EXT = 0x39,
	ATA_CMD_WRITE_STREAM_DMA_EXT = 0x3a,
	ATA_CMD_WRITE_STREAM_EXT = 0x3b,
	ATA_CMD_WRITE_DMA_FUA_EXT = 0x3d,
	ATA_CMD_WRITE_DMA_QUEUED_FUA_EXT = 0x3e,
	ATA_CMD_WRITE_LOG_EXT = 0x3f,
	ATA_CMD_READ_VERIFY_SECTORS = 0x40,
	ATA_CMD_READ_VERIFY_SECTORS_EXT = 0x42,
	ATA_CMD_WRITE_UNCORRECTABLE_EXT = 0x45,
	ATA_CMD_READ_LOG_DMA_EXT = 0x47,
	ATA_CMD_CONFIGURE_STREAM = 0x51,
	ATA_CMD_WRITE_LOG_DMA_EXT = 0x57,
	ATA_CMD_TRUSTED_RECEIVE = 0x5c,
	ATA_CMD_TRUSTED_RECEIVE_DMA = 0x5d,
	ATA_CMD_TRUSTED_SEND = 0x5e,
	ATA_CMD_TRUSTED_SEND_DMA = 0x5f,
	ATA_CMD_CFA_TRANSLATE_SECTOR = 0x87,
	ATA_CMD_EXECUTE_DEVICE_DIAGNOSTIC = 0x90,
	ATA_CMD_DOWNLOAD_MICROCODE = 0x92,
	ATA_CMD_PACKET = 0xa0,
	ATA_CMD_IDENTIFY_PACKET_DEVICE = 0xa1,
	ATA_CMD_SERVICE = 0xa2,
	ATA_CMD_SMART = 0xb0,
	ATA_CMD_DEVICE_CONFIGURATION_OVERLAY = 0xb1,
	ATA_CMD_NV_CACHE = 0xb6,
	ATA_CMD_CFA_ERASE_SECTORS = 0xc0,
	ATA_CMD_READ_MULTIPLE = 0xc4,
	ATA_CMD_WRITE_MULTIPLE = 0xc5,
	ATA_CMD_SET_MULTIPLE_MODE = 0xc6,
	ATA_CMD_READ_DMA_QUEUED = 0xc7,
	ATA_CMD_READ_DMA = 0xc8,
	ATA_CMD_WRITE_DMA = 0xca,
	ATA_CMD_WRITE_DMA_QUEUED = 0xcc,
	ATA_CMD_CFA_WRITE_MULTIPLE_WITHOUT_ERASE = 0xcd,
	ATA_CMD_WRITE_MULTIPLE_FUA_EXT = 0xce,
	ATA_CMD_CHECK_MEDIA_CARD_TYPE = 0xd1,
	ATA_CMD_STANDBY_IMMEDIATE = 0xe0,
	ATA_CMD_IDLE_IMMEDIATE = 0xe1,
	ATA_CMD_STANDBY = 0xe2,
	ATA_CMD_IDLE = 0xe3,
	ATA_CMD_READ_BUFFER = 0xe4,
	ATA_CMD_CHECK_POWER_MODE = 0xe5,
	ATA_CMD_SLEEP = 0xe6,
	ATA_CMD_FLUSH_CACHE = 0xe7,
	ATA_CMD_WRITE_BUFFER = 0xe8,
	ATA_CMD_FLUSH_CACHE_EXT = 0xea,
	ATA_CMD_IDENTIFY_DEVICE = 0xec,
	ATA_CMD_SET_FEATURES = 0xef,
	ATA_CMD_SECURITY_SET_PASSWORD = 0xf1,
	ATA_CMD_SECURITY_UNLOCK = 0xf2,
	ATA_CMD_SECURITY_EARASE_PREPARE = 0xf3,
	ATA_CMD_SECURITY_ERASE_UNIT = 0xf4,
	ATA_CMD_SECURITY_FREEZE_LOCK = 0xf5,
	ATA_CMD_SECURITY_DISABLE_PASSWORD = 0xf6,
	ATA_CMD_READ_NATIVE_MAX_ADDRESS = 0xf8,
	ATA_CMD_SET_MAX_ADDRESS = 0xf9
} AtaCommand;

typedef enum AtaStatus {
	ATA_STAT_BUSY = 0x80,
	ATA_STAT_READY = 0x40,
	ATA_STAT_FAULT = 0x20,
	ATA_STAT_SEEK = 0x10,
	ATA_STAT_DRQ = 0x08,
	ATA_STAT_CORR = 0x04,
	ATA_STAT_INDEX = 0x02,
	ATA_STAT_ERR = 0x01,
} AtaStatus;


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

 char HDD;
unsigned short drive;

#endif /* __DRIVERS_STORAGE_ATA_H__ */

