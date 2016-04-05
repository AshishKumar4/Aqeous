#ifndef AHCI_h
#define AHCI_h

#include <common.h>
#include <mem.h>
#include <ahci_device_list.h>
#include <ata.h>
#include <pci.h>


#define KERNBASE 4096*1024
#define SATA_SIG_ATA    0x00000101  // SATA drive
#define SATA_SIG_ATAPI  0xEB140101  // SATAPI drive
#define SATA_SIG_SEMB   0xC33C0101  // Enclosure management bridge
#define SATA_SIG_PM 0x96690101  // Port multiplier
#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SATAPI 4
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define HBA_PORT_DET_PRESENT 3
#define HBA_PORT_IPM_ACTIVE 1
#define AHCI_BASE   150*1024*1024    // 64Mb-90Mb
#define HBA_PxCMD_CR            (1 << 15) /* CR - Command list Running */
#define HBA_PxCMD_FR            (1 << 14) /* FR - FIS receive Running */
#define HBA_PxCMD_FRE           (1 <<  4) /* FRE - FIS Receive Enable */
#define HBA_PxCMD_SUD           (1 <<  1) /* SUD - Spin-Up Device */
#define HBA_PxCMD_ST            (1 <<  0) /* ST - Start (command processing) */
#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08
#define HBA_PxIS_TFES   (1 << 30)       /* TFES - Task File Error Status */
#define ATA_CMD_READ_DMA_EX     0x25
#define ATA_CMD_WRITE_DMA_EX     0x35

char test[20] = "6317065029";


#define AHCI_PCI_BAR		0x24
#define AHCI_MAX_SG		56 /* hardware max is 64K */
#define AHCI_CMD_SLOT_SZ	32
#define AHCI_RX_FIS_SZ		256
#define AHCI_CMD_TBL_HDR	0x80
#define AHCI_CMD_TBL_CDB	0x40
#define AHCI_CMD_TBL_SZ		AHCI_CMD_TBL_HDR + (AHCI_MAX_SG * 16)
#define AHCI_PORT_PRIV_DMA_SZ	AHCI_CMD_SLOT_SZ + AHCI_CMD_TBL_SZ	\
				+ AHCI_RX_FIS_SZ
#define AHCI_CMD_ATAPI		(1 << 5)
#define AHCI_CMD_WRITE		(1 << 6)
#define AHCI_CMD_PREFETCH	(1 << 7)
#define AHCI_CMD_RESET		(1 << 8)
#define AHCI_CMD_CLR_BUSY	(1 << 10)

#define RX_FIS_D2H_REG		0x40	/* offset of D2H Register FIS data */

/* Global controller registers */
#define HOST_CAP		0x00 /* host capabilities */
#define HOST_CTL		0x04 /* global host control */
#define HOST_IRQ_STAT		0x08 /* interrupt status */
#define HOST_PORTS_IMPL		0x0c /* bitmap of implemented ports */
#define HOST_VERSION		0x10 /* AHCI spec. version compliancy */
#define HOST_CAP2		0x24 /* host capabilities, extended */

/* HOST_CTL bits */
#define HOST_RESET		(1 << 0)  /* reset controller; self-clear */
#define HOST_IRQ_EN		(1 << 1)  /* global IRQ enable */
#define HOST_AHCI_EN		(1 << 31) /* AHCI enabled */

/* Registers for each SATA port */
#define PORT_LST_ADDR		0x00 /* command list DMA addr */
#define PORT_LST_ADDR_HI	0x04 /* command list DMA addr hi */
#define PORT_FIS_ADDR		0x08 /* FIS rx buf addr */
#define PORT_FIS_ADDR_HI	0x0c /* FIS rx buf addr hi */
#define PORT_IRQ_STAT		0x10 /* interrupt status */
#define PORT_IRQ_MASK		0x14 /* interrupt enable/disable mask */
#define PORT_CMD		0x18 /* port command */
#define PORT_TFDATA		0x20 /* taskfile data */
#define PORT_SIG		0x24 /* device TF signature */
#define PORT_CMD_ISSUE		0x38 /* command issue */
#define PORT_SCR		0x28 /* SATA phy register block */
#define PORT_SCR_STAT		0x28 /* SATA phy register: SStatus */
#define PORT_SCR_CTL		0x2c /* SATA phy register: SControl */
#define PORT_SCR_ERR		0x30 /* SATA phy register: SError */
#define PORT_SCR_ACT		0x34 /* SATA phy register: SActive */

/* PORT_IRQ_{STAT,MASK} bits */
#define PORT_IRQ_COLD_PRES	(1 << 31) /* cold presence detect */
#define PORT_IRQ_TF_ERR		(1 << 30) /* task file error */
#define PORT_IRQ_HBUS_ERR	(1 << 29) /* host bus fatal error */
#define PORT_IRQ_HBUS_DATA_ERR	(1 << 28) /* host bus data error */
#define PORT_IRQ_IF_ERR		(1 << 27) /* interface fatal error */
#define PORT_IRQ_IF_NONFATAL	(1 << 26) /* interface non-fatal error */
#define PORT_IRQ_OVERFLOW	(1 << 24) /* xfer exhausted available S/G */
#define PORT_IRQ_BAD_PMP	(1 << 23) /* incorrect port multiplier */

#define PORT_IRQ_PHYRDY		(1 << 22) /* PhyRdy changed */
#define PORT_IRQ_DEV_ILCK	(1 << 7) /* device interlock */
#define PORT_IRQ_CONNECT	(1 << 6) /* port connect change status */
#define PORT_IRQ_SG_DONE	(1 << 5) /* descriptor processed */
#define PORT_IRQ_UNK_FIS	(1 << 4) /* unknown FIS rx'd */
#define PORT_IRQ_SDB_FIS	(1 << 3) /* Set Device Bits FIS rx'd */
#define PORT_IRQ_DMAS_FIS	(1 << 2) /* DMA Setup FIS rx'd */
#define PORT_IRQ_PIOS_FIS	(1 << 1) /* PIO Setup FIS rx'd */
#define PORT_IRQ_D2H_REG_FIS	(1 << 0) /* D2H Register FIS rx'd */

#define PORT_IRQ_FATAL		PORT_IRQ_TF_ERR | PORT_IRQ_HBUS_ERR	\
				| PORT_IRQ_HBUS_DATA_ERR | PORT_IRQ_IF_ERR

#define DEF_PORT_IRQ		PORT_IRQ_FATAL | PORT_IRQ_PHYRDY	\
				| PORT_IRQ_CONNECT | PORT_IRQ_SG_DONE	\
				| PORT_IRQ_UNK_FIS | PORT_IRQ_SDB_FIS	\
				| PORT_IRQ_DMAS_FIS | PORT_IRQ_PIOS_FIS	\
				| PORT_IRQ_D2H_REG_FIS

/* PORT_CMD bits */
#define PORT_CMD_ATAPI		(1 << 24) /* Device is ATAPI */
#define PORT_CMD_LIST_ON	(1 << 15) /* cmd list DMA engine running */
#define PORT_CMD_FIS_ON		(1 << 14) /* FIS DMA engine running */
#define PORT_CMD_FIS_RX		(1 << 4) /* Enable FIS receive DMA engine */
#define PORT_CMD_CLO		(1 << 3) /* Command list override */
#define PORT_CMD_POWER_ON	(1 << 2) /* Power up device */
#define PORT_CMD_SPIN_UP	(1 << 1) /* Spin up device */
#define PORT_CMD_START		(1 << 0) /* Enable port DMA engine */

#define PORT_CMD_ICC_ACTIVE	(0x1 << 28) /* Put i/f in active state */
#define PORT_CMD_ICC_PARTIAL	(0x2 << 28) /* Put i/f in partial state */
#define PORT_CMD_ICC_SLUMBER	(0x6 << 28) /* Put i/f in slumber state */

#define AHCI_MAX_PORTS		32

/* SETFEATURES stuff */
#define SETFEATURES_XFER	0x03
#define XFER_UDMA_7		0x47
#define XFER_UDMA_6		0x46
#define XFER_UDMA_5		0x45
#define XFER_UDMA_4		0x44
#define XFER_UDMA_3		0x43
#define XFER_UDMA_2		0x42
#define XFER_UDMA_1		0x41
#define XFER_UDMA_0		0x40
#define XFER_MW_DMA_2		0x22
#define XFER_MW_DMA_1		0x21
#define XFER_MW_DMA_0		0x20
#define XFER_SW_DMA_2		0x12
#define XFER_SW_DMA_1		0x11
#define XFER_SW_DMA_0		0x10
#define XFER_PIO_4		0x0C
#define XFER_PIO_3		0x0B
#define XFER_PIO_2		0x0A
#define XFER_PIO_1		0x09
#define XFER_PIO_0		0x08
#define XFER_PIO_SLOW		0x00

#define ATA_FLAG_SATA		(1 << 3)
#define ATA_FLAG_NO_LEGACY	(1 << 4) /* no legacy mode check */
#define ATA_FLAG_MMIO		(1 << 6) /* use MMIO, not PIO */
#define ATA_FLAG_SATA_RESET	(1 << 7) /* (obsolete) use COMRESET */
#define ATA_FLAG_PIO_DMA	(1 << 8) /* PIO cmds via DMA */
#define ATA_FLAG_NO_ATAPI	(1 << 11) /* No ATAPI support */

typedef struct AhciCommandHeader {
	uint32_t opts;
	uint32_t status;
	uint32_t tbl_addr;
	uint32_t tbl_addr_hi;
	uint32_t reserved[4];
} AhciCommandHeader;

typedef struct AhciSg {
	uint32_t addr;
	uint32_t addr_hi;
	uint32_t reserved;
	uint32_t flags_size;
} AhciSg;

typedef struct AhciIoPort {
	void *cmd_addr;
	void *scr_addr;
	void *port_mmio;
	AhciCommandHeader *cmd_slot;
	AhciSg *cmd_tbl_sg;
	void *cmd_tbl;
	void *rx_fis;
	int index;
} AhciIoPort;

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;

typedef enum
{
    FIS_TYPE_REG_H2D    = 0x27, // Register FIS - host to device
    FIS_TYPE_REG_D2H    = 0x34, // Register FIS - device to host
    FIS_TYPE_DMA_ACT    = 0x39, // DMA activate FIS - device to host
    FIS_TYPE_DMA_SETUP  = 0x41, // DMA setup FIS - bidirectional
    FIS_TYPE_DATA       = 0x46, // Data FIS - bidirectional
    FIS_TYPE_BIST       = 0x58, // BIST activate FIS - bidirectional
    FIS_TYPE_PIO_SETUP  = 0x5F, // PIO setup FIS - device to host
    FIS_TYPE_DEV_BITS   = 0xA1, // Set device bits FIS - device to host
} FIS_TYPE;

typedef struct tagFIS_REG_H2D
{
    // DWORD 0
    BYTE    fis_type;   // FIS_TYPE_REG_H2D

    BYTE    pmport:4;   // Port multiplier
    BYTE    rsv0:3;     // Reserved
    BYTE    c:1;        // 1: Command, 0: Control

    BYTE    command;    // Command register
    BYTE    featurel;   // Feature register, 7:0

    // DWORD 1
    BYTE    lba0;       // LBA low register, 7:0
    BYTE    lba1;       // LBA mid register, 15:8
    BYTE    lba2;       // LBA high register, 23:16
    BYTE    device;     // Device register

    // DWORD 2
    BYTE    lba3;       // LBA register, 31:24
    BYTE    lba4;       // LBA register, 39:32
    BYTE    lba5;       // LBA register, 47:40
    BYTE    featureh;   // Feature register, 15:8

    // DWORD 3
    BYTE    countl;     // Count register, 7:0
    BYTE    counth;     // Count register, 15:8
    BYTE    icc;        // Isochronous command completion
    BYTE    control;    // Control register

    // DWORD 4
    BYTE    rsv1[4];    // Reserved
} FIS_REG_H2D;


typedef struct tagFIS_REG_D2H
{
    // DWORD 0
    BYTE    fis_type;    // FIS_TYPE_REG_D2H

    BYTE    pmport:4;    // Port multiplier
    BYTE    rsv0:2;      // Reserved
    BYTE    i:1;         // Interrupt bit
    BYTE    rsv1:1;      // Reserved

    BYTE    status;      // Status register
    BYTE    error;       // Error register

    // DWORD 1
    BYTE    lba0;        // LBA low register, 7:0
    BYTE    lba1;        // LBA mid register, 15:8
    BYTE    lba2;        // LBA high register, 23:16
    BYTE    device;      // Device register

    // DWORD 2
    BYTE    lba3;        // LBA register, 31:24
    BYTE    lba4;        // LBA register, 39:32
    BYTE    lba5;        // LBA register, 47:40
    BYTE    rsv2;        // Reserved

    // DWORD 3
    BYTE    countl;      // Count register, 7:0
    BYTE    counth;      // Count register, 15:8
    BYTE    rsv3[2];     // Reserved

    // DWORD 4
    BYTE    rsv4[4];     // Reserved
} FIS_REG_D2H;


typedef struct tagFIS_DATA
{
    // DWORD 0
    BYTE    fis_type;   // FIS_TYPE_DATA

    BYTE    pmport:4;   // Port multiplier
    BYTE    rsv0:4;     // Reserved

    BYTE    rsv1[2];    // Reserved

    // DWORD 1 ~ N
    DWORD   data[1];    // Payload
} FIS_DATA;


typedef struct tagFIS_PIO_SETUP
{
    // DWORD 0
    BYTE    fis_type;   // FIS_TYPE_PIO_SETUP

    BYTE    pmport:4;   // Port multiplier
    BYTE    rsv0:1;     // Reserved
    BYTE    d:1;        // Data transfer direction, 1 - device to host
    BYTE    i:1;        // Interrupt bit
    BYTE    rsv1:1;

    BYTE    status;     // Status register
    BYTE    error;      // Error register

    // DWORD 1
    BYTE    lba0;       // LBA low register, 7:0
    BYTE    lba1;       // LBA mid register, 15:8
    BYTE    lba2;       // LBA high register, 23:16
    BYTE    device;     // Device register

    // DWORD 2
    BYTE    lba3;       // LBA register, 31:24
    BYTE    lba4;       // LBA register, 39:32
    BYTE    lba5;       // LBA register, 47:40
    BYTE    rsv2;       // Reserved

    // DWORD 3
    BYTE    countl;     // Count register, 7:0
    BYTE    counth;     // Count register, 15:8
    BYTE    rsv3;       // Reserved
    BYTE    e_status;   // New value of status register

    // DWORD 4
    WORD    tc;     // Transfer count
    BYTE    rsv4[2];    // Reserved
} FIS_PIO_SETUP;



typedef struct tagFIS_DMA_SETUP
{
    // DWORD 0
    BYTE    fis_type;   // FIS_TYPE_DMA_SETUP

    BYTE    pmport:4;   // Port multiplier
    BYTE    rsv0:1;     // Reserved
    BYTE    d:1;        // Data transfer direction, 1 - device to host
    BYTE    i:1;        // Interrupt bit
    BYTE    a:1;            // Auto-activate. Specifies if DMA Activate FIS is needed

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

typedef volatile struct tagHBA_PORT
{
    DWORD   clb;        // 0x00, command list base address, 1K-byte aligned
    DWORD   clbu;       // 0x04, command list base address upper 32 bits
    DWORD   fb;     // 0x08, FIS base address, 256-byte aligned
    DWORD   fbu;        // 0x0C, FIS base address upper 32 bits
    DWORD   is;     // 0x10, interrupt status
    DWORD   ie;     // 0x14, interrupt enable
    DWORD   cmd;        // 0x18, command and status
    DWORD   rsv0;       // 0x1C, Reserved
    DWORD   tfd;        // 0x20, task file data
    DWORD   sig;        // 0x24, signature
    DWORD   ssts;       // 0x28, SATA status (SCR0:SStatus)
    DWORD   sctl;       // 0x2C, SATA control (SCR2:SControl)
    DWORD   serr;       // 0x30, SATA error (SCR1:SError)
    DWORD   sact;       // 0x34, SATA active (SCR3:SActive)
    DWORD   ci;     // 0x38, command issue
    DWORD   sntf;       // 0x3C, SATA notification (SCR4:SNotification)
    DWORD   fbs;        // 0x40, FIS-based switch control
    DWORD   rsv1[11];   // 0x44 ~ 0x6F, Reserved
    DWORD   vendor[4];  // 0x70 ~ 0x7F, vendor specific
} HBA_PORT;

typedef volatile struct tagHBA_MEM
{
    // 0x00 - 0x2B, Generic Host Control
    DWORD   cap;        // 0x00, Host capability
    DWORD   ghc;        // 0x04, Global host control
    DWORD   is;     // 0x08, Interrupt status
    DWORD   pi;     // 0x0C, Port implemented
    DWORD   vs;     // 0x10, Version
    DWORD   ccc_ctl;    // 0x14, Command completion coalescing control
    DWORD   ccc_pts;    // 0x18, Command completion coalescing ports
    DWORD   em_loc;     // 0x1C, Enclosure management location
    DWORD   em_ctl;     // 0x20, Enclosure management control
    DWORD   cap2;       // 0x24, Host capabilities extended
    DWORD   bohc;       // 0x28, BIOS/OS handoff control and status

    // 0x2C - 0x9F, Reserved
    BYTE    rsv[0xA0-0x2C];

    // 0xA0 - 0xFF, Vendor specific registers
    BYTE    vendor[0x100-0xA0];

    // 0x100 - 0x10FF, Port control registers
    HBA_PORT    ports[32];   // 1 ~ 32
} HBA_MEM;

typedef volatile struct tagHBA_FIS
{
	// 0x00
	FIS_DMA_SETUP	dsfis;		// DMA Setup FIS
	BYTE		pad0[4];

	// 0x20
	FIS_PIO_SETUP	psfis;		// PIO Setup FIS
	BYTE		pad1[12];

	// 0x40
	FIS_REG_D2H	rfis;		// Register – Device to Host FIS
	BYTE		pad2[4];

	// 0x58
	FIS_REG_H2D	sdbfis;		// Set Device Bit FIS

	// 0x60
	BYTE		ufis[64];

	// 0xA0
	BYTE		rsv[0x100-0xA0];
} HBA_FIS;

typedef struct tagHBA_CMD_HEADER
{
    // DW0
    BYTE    cfl:5;      // Command FIS length in DWORDS, 2 ~ 16
    BYTE    a:1;        // ATAPI
    BYTE    w:1;        // Write, 1: H2D, 0: D2H
    BYTE    p:1;        // Prefetchable

    BYTE    r:1;        // Reset
    BYTE    b:1;        // BIST
    BYTE    c:1;        // Clear busy upon R_OK
    BYTE    rsv0:1;     // Reserved
    BYTE    pmp:4;      // Port multiplier port

    WORD    prdtl;      // Physical region descriptor table length in entries

    // DW1
    volatile
    DWORD   prdbc;      // Physical region descriptor byte count transferred

    // DW2, 3
    DWORD   ctba;       // Command table descriptor base address
    DWORD   ctbau;      // Command table descriptor base address upper 32 bits

    // DW4 - 7
    DWORD   rsv1[4];    // Reserved
} HBA_CMD_HEADER;

typedef struct tagHBA_PRDT_ENTRY
{
    DWORD   dba;        // Data base address
    DWORD   dbau;       // Data base address upper 32 bits
    DWORD   rsv0;       // Reserved

    // DW3
    DWORD   dbc:22;     // Byte count, 4M max
    DWORD   rsv1:9;     // Reserved
    DWORD   i:1;        // Interrupt on completion
} HBA_PRDT_ENTRY;


typedef struct tagHBA_CMD_TBL
{
    // 0x00
    BYTE    cfis[64];   // Command FIS

    // 0x40
    BYTE    acmd[16];   // ATAPI command, 12 or 16 bytes

    // 0x50
    BYTE    rsv[48];    // Reserved

    // 0x80
    HBA_PRDT_ENTRY  prdt_entry[0] ;  // Physical region descriptor table entries, 0 ~ 65535
} HBA_CMD_TBL;

typedef struct tagSATA_SHADOW_REG
{

}SATA_REG_SHADOW_t;

typedef struct SATA_ident
{
		unsigned short	config;		/* lots of obsolete bit flags */
		unsigned short	cyls;		/* obsolete */
		unsigned short	reserved2;	/* special config */
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
		unsigned short  multi_count; /* Multiple Count */
		unsigned short	dword_io;	/* 0=not_implemented; 1=implemented */
		unsigned short	capability1;	/* vendor unique */
		unsigned short	capability2;	/* bits 0:DMA 1:LBA 2:IORDYsw 3:IORDYsup word: 50 */
		unsigned char	vendor5;	/* vendor unique */
		unsigned char	tPIO;		/* 0=slow, 1=medium, 2=fast */
		unsigned char	vendor6;	/* vendor unique */
		unsigned char	tDMA;		/* 0=slow, 1=medium, 2=fast */
		unsigned short	field_valid;	/* bits 0:cur_ok 1:eide_ok */
		unsigned short	cur_cyls;	/* logical cylinders */
		unsigned short	cur_heads;	/* logical heads word 55*/
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
		unsigned short  sata_capability;	/*  SATA Capabilities word 76*/
		unsigned short  sata_additional;	/*  Additional Capabilities */
		unsigned short  sata_supported;	/* SATA Features supported  */
		unsigned short  features_enabled;	/* SATA features enabled */
		unsigned short  major_rev_num;	/*  Major rev number word 80 */
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
		unsigned short	word92;			/* reserved (word 92) */
		unsigned short	comreset;		/* should be cleared to 0 */
		unsigned short  accoustic;		/*  accoustic management */
		unsigned short  min_req_sz;		/* Stream minimum required size */
		unsigned short  transfer_time_dma;	/* Streaming Transfer Time-DMA */
		unsigned short  access_latency;		/* Streaming access latency-DMA & PIO WORD 97*/
		unsigned int    perf_granularity;	/* Streaming performance granularity */
		unsigned int	total_usr_sectors[2]; /* Total number of user addressable sectors */
		unsigned short	transfer_time_pio; 	/* Streaming Transfer time PIO */
		unsigned short	reserved105; 		/* Word 105 */
		unsigned short	sector_sz; 			/* Puysical Sector size / Logical sector size */
		unsigned short	inter_seek_delay;	/* In microseconds */
		unsigned short	words108_116[9]; 	/*  */
		unsigned int	words_per_sector; 	/* words per logical sectors */
		unsigned short	supported_settings; /* continued from words 82-84 */
		unsigned short	command_set_3; 		/* continued from words 85-87 */
		unsigned short  words121_126[6];	/* reserved words 121-126 */
		unsigned short	word127;			/* reserved (word 127) */
		unsigned short	security_status;	/* device lock function
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
		unsigned short	cfa; /* CFA Power mode 1 */
		unsigned short	words161_175[15]; /* Reserved */
		unsigned char	media_serial[60]; /* words 176-205 Current Media serial number */
		unsigned short	sct_cmd_transport; /* SCT Command Transport */
		unsigned short	words207_208[2]; /* reserved */
		unsigned short	block_align; /* Alignement of logical blocks in larger physical blocks */
		unsigned int	WRV_sec_count; /* Write-Read-Verify sector count mode 3 only */
		unsigned int   	verf_sec_count; /* Verify Sector count mode 2 only */
		unsigned short	nv_cache_capability; /* NV Cache capabilities */
		unsigned short	nv_cache_sz; /* NV Cache size in logical blocks */
		unsigned short	nv_cache_sz2; /* NV Cache size in logical blocks */
		unsigned short	rotation_rate; /* Nominal media rotation rate */
		unsigned short	reserved218; /*  */
		unsigned short	nv_cache_options; /* NV Cache options */
		unsigned short	words220_221[2]; /* reserved */
		unsigned short	transport_major_rev; /*  */
		unsigned short	transport_minor_rev; /*  */
		unsigned short	words224_233[10]; /* Reserved */
		unsigned short	min_dwnload_blocks; /* Minimum number of 512byte units per DOWNLOAD MICROCODE
		 									command for mode 03h */
		unsigned short	max_dwnload_blocks; /* Maximum number of 512byte units per DOWNLOAD MICROCODE
		 									command for mode 03h */
		unsigned short	words236_254[19];	/* Reserved */
		unsigned short	integrity; 			/* Cheksum, Signature */
} SATA_ident_t;

typedef struct Disk_dev_
{
    uint32_t type;
	HBA_PORT *port; // Port control registers
	SATA_ident_t *info;
	struct Disk_dev_ *next;
}Disk_dev_t;

Disk_dev_t *Disk_dev,*Disk_dev_start;


typedef struct _ahci
{
    PciDevice_t *ahci;
    unsigned short ControllerID;
    unsigned short Disks;
    Disk_dev_t Disk[32];
}ahci_t;

ahci_t *ahci,*ahci_start;

typedef struct
{
    uint32_t    phys_addr;
    uint32_t    something;
    uint32_t    reserved;
    uint32_t    len;
} ahci_prdt;

 void probe_port(ahci_t *ahci_c);


uint32_t sata=0;

extern HBA_MEM *abar;

int read(HBA_PORT *port, QWORD start, DWORD count, DWORD buf);
int write(HBA_PORT *port, QWORD start, DWORD count, DWORD buf);
char fs_buf[1024];
int check_type(HBA_PORT *port);
void port_rebase(HBA_PORT *port, int portno);
int SATA_Commander(HBA_PORT *port, WORD Command, BYTE rw, DWORD buf, DWORD prdtl, DWORD dbc, DWORD startl, DWORD starth, DWORD count);

#endif
