#include<ahci.h>
#include<string.h>
#include<sys.h>
#include<blockdev.c>
#include <list.c>

typedef struct _ahci
{
    PciDevice_t ahci;
    unsigned int diskID;
}ahci_t;

ahci_t ahci[];

typedef struct ahci_dev_
{

};

typedef struct
{
    uint32_t    phys_addr;
    uint32_t    something;
    uint32_t    reserved;
    uint32_t    len;
} ahci_prdt;

/**
 * AHCI controller data.
 */

/** PCI configuration fields. */
#define PCI_CONFIG_CAP                  0x34

#define PCI_CAP_ID_SATACR               0x12
#define VBOX_AHCI_NO_DEVICE 0xffff

#define RT_BIT_32(bit) ((uint32_t)(1L << (bit)))

/** Global register set. */
#define AHCI_HBA_SIZE 0x100

//@todo: what are the casts good for?
#define AHCI_REG_CAP ((uint32_t)0x00)
#define AHCI_REG_GHC ((uint32_t)0x04)
# define AHCI_GHC_AE RT_BIT_32(31)
# define AHCI_GHC_IR RT_BIT_32(1)
# define AHCI_GHC_HR RT_BIT_32(0)
#define AHCI_REG_IS  ((uint32_t)0x08)
#define AHCI_REG_PI  ((uint32_t)0x0c)
#define AHCI_REG_VS  ((uint32_t)0x10)

/** Per port register set. */
#define AHCI_PORT_SIZE     0x80

#define AHCI_REG_PORT_CLB  0x00
#define AHCI_REG_PORT_CLBU 0x04
#define AHCI_REG_PORT_FB   0x08
#define AHCI_REG_PORT_FBU  0x0c
#define AHCI_REG_PORT_IS   0x10
# define AHCI_REG_PORT_IS_DHRS RT_BIT_32(0)
# define AHCI_REG_PORT_IS_TFES RT_BIT_32(30)
#define AHCI_REG_PORT_IE   0x14
#define AHCI_REG_PORT_CMD  0x18
# define AHCI_REG_PORT_CMD_ST  RT_BIT_32(0)
# define AHCI_REG_PORT_CMD_FRE RT_BIT_32(4)
# define AHCI_REG_PORT_CMD_FR  RT_BIT_32(14)
# define AHCI_REG_PORT_CMD_CR  RT_BIT_32(15)
#define AHCI_REG_PORT_TFD  0x20
#define AHCI_REG_PORT_SIG  0x24
#define AHCI_REG_PORT_SSTS 0x28
#define AHCI_REG_PORT_SCTL 0x2c
#define AHCI_REG_PORT_SERR 0x30
#define AHCI_REG_PORT_SACT 0x34
#define AHCI_REG_PORT_CI   0x38

/** Returns the absolute register offset from a given port and port register. */
#define AHCI_PORT_REG(port, reg)    (AHCI_HBA_SIZE + (port) * AHCI_PORT_SIZE + (reg))

#define AHCI_REG_IDX   0
#define AHCI_REG_DATA  4

/** Writes the given value to a AHCI register. */
#define AHCI_WRITE_REG(iobase, reg, val)    \
    outl((iobase) + AHCI_REG_IDX, reg);    \
    outl((iobase) + AHCI_REG_DATA, val)

/** Reads from a AHCI register. */
#define AHCI_READ_REG(iobase, reg, val)     \
    outl((iobase) + AHCI_REG_IDX, reg);    \
    (val) = inl((iobase) + AHCI_REG_DATA)

/** Writes to the given port register. */
#define VBOXAHCI_PORT_WRITE_REG(iobase, port, reg, val)     \
    AHCI_WRITE_REG((iobase), AHCI_PORT_REG((port), (reg)), val)

/** Reads from the given port register. */
#define VBOXAHCI_PORT_READ_REG(iobase, port, reg, val)      \
    AHCI_READ_REG((iobase), AHCI_PORT_REG((port), (reg)), val)

unsigned int disks;

void start_cmd(HBA_PORT *port);
void stop_cmd(HBA_PORT *port);
int find_cmdslot(HBA_PORT *port);
unsigned int bus,device,func;

HBA_MEM *abar;
bool AHCI;

char *get_device_info(u16int vendorID, u16int deviceID)
{
	const device_info *info;
	for (info = kSupportedDevices; info->vendor; info++)
    {
		if (info->vendor == vendorID && info->device == deviceID)
        {
			return info->name;
		}
	}
	return 0;
}


int checkAHCI()
{
    for(int bus=0;bus<2;bus++)
    {
        for(int slot=0;slot<32;slot++)
        {
            if(devices[bus][slot].Header->Class==1)
            {
                console_writestring("Mass Storage Media found!\n");
                if(devices[bus][slot].Header->Subclass==6)
                {
                    ++disks;
                    ahci[disks].ahci=devices[bus][slot];
                    console_writestring(get_device_info(ahci[disks].ahci.Header->VendorId,ahci[disks].ahci.Header->DeviceId));
                    console_writestring("\n\tSATA DISK #");
                    console_write_dec(disks);
                    console_writestring(" FOUND, INITIALIZING AHCI CONTROLLER and Disk\n");
                    if(initAHCI(&ahci[disks].ahci))
                    {
                        console_writestring("\t\tSATA Disk Initialized\n");
                        ahci[disks].diskID=disks;
                    }
                    else
                    {
                        console_writestring("\t\tCouldn't Initialize SATA DISK #");
                        console_write_dec(disks);
                        console_writestring("\n");
                        --disks;
                    }
                }
                else
                    console_writestring("\tNOT AHCI/SATA Disk\n");
            }
        }
    }
    console_writestring("\n");
    return disks;
 }

void probe_port(HBA_MEM *abar)
{
	// Search disk in impelemented ports
	DWORD pi = abar->pi;
	int i = 0;
	while (i<32)
	{
		if (pi & 1)
		{
			int dt = check_type(&abar->ports[i]);
			if (dt == AHCI_DEV_SATA)
			{
			    AHCI=1;
				console_writestring("SATA drive found ");
			}
			else if (dt == AHCI_DEV_SATAPI)
			{
			    AHCI=1;
				console_writestring("SATAPI drive found ");
			}
			else if (dt == AHCI_DEV_SEMB)
			{
			    AHCI=1;
				console_writestring("SEMB drive found ");
			}
			else if (dt == AHCI_DEV_PM)
			{
			    AHCI=1;
				console_writestring("PM drive found ");
			}
			else
			{
			    AHCI=0;
				console_writestring("No drive found ");
			}
		}

		pi >>= 1;
		i ++;
	}
}

// Check device type
int check_type(HBA_PORT *port)
{
	DWORD ssts = port->ssts;

	BYTE ipm = (ssts >> 8) & 0x0F;
	BYTE det = ssts & 0x0F;

	if (det != HBA_PORT_DET_PRESENT)	// Check drive status
		return AHCI_DEV_NULL;
	if (ipm != HBA_PORT_IPM_ACTIVE)
		return AHCI_DEV_NULL;

	switch (port->sig)
	{
	case SATA_SIG_ATAPI:
		return AHCI_DEV_SATAPI;
	case SATA_SIG_SEMB:
		return AHCI_DEV_SEMB;
	case SATA_SIG_PM:
		return AHCI_DEV_PM;
	default:
		return AHCI_DEV_SATA;
	}
}

void port_rebase(HBA_PORT *port, int portno)
{
	stop_cmd(port);	// Stop command engine

	// Command list offset: 1K*portno
	// Command list entry size = 32
	// Command list entry maxim count = 32
	// Command list maxim size = 32*32 = 1K per port
	port->clb = AHCI_BASE + (portno<<10);
	port->clbu = 0;
	memset((void*)(port->clb), 0, 1024);

	// FIS offset: 32K+256*portno
	// FIS entry size = 256 bytes per port
	port->fb = AHCI_BASE + (32<<10) + (portno<<8);
	port->fbu = 0;
	memset((void*)(port->fb), 0, 256);

	// Command table offset: 40K + 8K*portno
	// Command table size = 256*32 = 8K per port
	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(port->clb);
	for (int i=0; i<32; i++)
	{
		cmdheader[i].prdtl = 8;	// 8 prdt entries per command table
					// 256 bytes per command table, 64+16+48+16*8
		// Command table offset: 40K + 8K*portno + cmdheader_index*256
		cmdheader[i].ctba = AHCI_BASE + (40<<10) + (portno<<13) + (i<<8);
		cmdheader[i].ctbau = 0;
		memset((void*)cmdheader[i].ctba, 0, 256);
	}

	start_cmd(port);	// Start command engine
}

// Start command engine
void start_cmd(HBA_PORT *port)
{
	// Wait until CR (bit15) is cleared
	while (port->cmd & HBA_PxCMD_CR);

	// Set FRE (bit4) and ST (bit0)
	port->cmd |= HBA_PxCMD_FRE;
	port->cmd |= HBA_PxCMD_ST;
}

// Stop command engine
void stop_cmd(HBA_PORT *port)
{
	// Clear ST (bit0)
	port->cmd &= ~HBA_PxCMD_ST;

	// Wait until FR (bit14), CR (bit15) are cleared
	while(1)
	{
		if (port->cmd & HBA_PxCMD_FR)
			continue;
		if (port->cmd & HBA_PxCMD_CR)
			continue;
		break;
	}

	// Clear FRE (bit4)
	port->cmd &= ~HBA_PxCMD_FRE;
}

int initAHCI(PciDevice_t *dev)
{
    if(dev->Header->Interface!=1)
    {
        printf("\nUnsupported SATA Interface\n");
        return -1;
    }
    HBA_MEM *h=dev->Header->Bar5;

    return 1;
}



int read(HBA_PORT *port, DWORD startl, DWORD starth, DWORD count, QWORD buf)
{
    //   buf = KERNBASE + buf;
        port->is = 0xffff;              // Clear pending interrupt bits
       // int spin = 0;           // Spin lock timeout counter
        int slot = find_cmdslot(port);
        if (slot == -1)
                return 0;
        uint64_t addr = 0;
//        print("\n clb %x clbu %x", port->clb, port->clbu);
        addr = (((addr | port->clbu) << 32) | port->clb);
        HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(KERNBASE + addr);

        //HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(port->clb);
        cmdheader += slot;
       cmdheader->cfl = sizeof(FIS_REG_H2D)/sizeof(DWORD);     // Command FIS size
        cmdheader->w = 0;               // Read from device
        cmdheader->c = 1;               // Read from device
        cmdheader->p = 1;               // Read from device
        // 8K bytes (16 sectors) per PRDT
        cmdheader->prdtl = (WORD)((count-1)>>4) + 1;    // PRDT entries count

        addr=0;
        addr=(((addr | cmdheader->ctbau)<<32)|cmdheader->ctba);
        HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(KERNBASE + addr);

        //memset(cmdtbl, 0, sizeof(HBA_CMD_TBL) + (cmdheader->prdtl-1)*sizeof(HBA_PRDT_ENTRY));
        int i = 0;
  //      print("[PRDTL][%d]", cmdheader->prdtl);
        // 8K bytes (16 sectors) per PRDT
        for (i=0; i<cmdheader->prdtl-1; i++)
        {
               cmdtbl->prdt_entry[i].dba = (DWORD)(buf & 0xFFFFFFFF);
                cmdtbl->prdt_entry[i].dbau = (DWORD)((buf << 32) & 0xFFFFFFFF);
                cmdtbl->prdt_entry[i].dbc = 8*1024-1;     // 8K bytes
                cmdtbl->prdt_entry[i].i = 0;
                buf += 4*1024;  // 4K words
                count -= 16;    // 16 sectors
       }
        /**If the final Data FIS transfer in a command is for an odd number of 16-bit words, the transmitter�s
Transport layer is responsible for padding the final Dword of a FIS with zeros. If the HBA receives one
more word than is indicated in the PRD table due to this padding requirement, the HBA shall not signal
this as an overflow condition. In addition, if the HBA inserts padding as required in a FIS it is transmitting,
an overflow error shall not be indicated. The PRD Byte Count field shall be updated based on the
number of words specified in the PRD table, ignoring any additional padding.**/

        // Last entry

        cmdtbl->prdt_entry[i].dba = (DWORD)(buf & 0xFFFFFFFF);
        cmdtbl->prdt_entry[i].dbau = (DWORD)((buf << 32) & 0xFFFFFFFF);
        cmdtbl->prdt_entry[i].dbc = count<<9;   // 512 bytes per sector
        cmdtbl->prdt_entry[i].i = 0;


        // Setup command
        FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);

        cmdfis->fis_type = FIS_TYPE_REG_H2D;
        cmdfis->c = 1;  // Command
        cmdfis->command = ATA_CMD_READ_DMA_EX;

        cmdfis->lba0 = (BYTE)startl;
        cmdfis->lba1 = (BYTE)(startl>>8);
        cmdfis->lba2 = (BYTE)(startl>>16);
        cmdfis->device = 1<<6;  // LBA mode

        cmdfis->lba3 = (BYTE)(startl>>24);
        cmdfis->lba4 = (BYTE)starth;
        cmdfis->lba5 = (BYTE)(starth>>8);

        cmdfis->countl = count & 0xff;
        cmdfis->counth = count>>8;

    //    print("[slot]{%d}", slot);
        port->ci = 1;    // Issue command
       // Wait for completion
        while (1)
        {
                // In some longer duration reads, it may be helpful to spin on the DPS bit
                // in the PxIS port field as well (1 << 5)
                if ((port->ci & (1<<slot)) == 0)
                        break;
                if (port->is & HBA_PxIS_TFES)   // Task file error
                {
                        //print("Read disk error\n");
                        return 0;
                }
        }
     //   print("\n after while 1");
     //   print("\nafter issue : %d" , port->tfd);
        // Check again
        if (port->is & HBA_PxIS_TFES)
        {
                //print("Read disk error\n");
                return 0;
        }

    //    print("\n[Port ci ][%d]", port->ci);
        int k = 0;
        while(port->ci != 0)
        {
            printf("[%d]", k++);
        }
        return 1;
}

int write(HBA_PORT *port, DWORD startl, DWORD starth, DWORD count, QWORD buf)
{
       port->is = 0xffff;              // Clear pending interrupt bits
       // int spin = 0;           // Spin lock timeout counter
        int slot = find_cmdslot(port);
        if (slot == -1)
                return 0;
        uint64_t addr = 0;
     //   print("\n clb %x clbu %x", port->clb, port->clbu);
        addr = (((addr | port->clbu) << 32) | port->clb);
        HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(KERNBASE + addr);

        //HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(port->clb);
        cmdheader += slot;
       cmdheader->cfl = sizeof(FIS_REG_H2D)/sizeof(DWORD);     // Command FIS size
        cmdheader->w = 1;               // Read from device
        cmdheader->c = 1;               // Read from device
        cmdheader->p = 1;               // Read from device
        // 8K bytes (16 sectors) per PRDT
        cmdheader->prdtl = (WORD)((count-1)>>4) + 1;    // PRDT entries count

        addr=0;
        addr=(((addr | cmdheader->ctbau)<<32)|cmdheader->ctba);
        HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(KERNBASE + addr);

        //memset(cmdtbl, 0, sizeof(HBA_CMD_TBL) + (cmdheader->prdtl-1)*sizeof(HBA_PRDT_ENTRY));
        int i = 0;
    //    print("[PRDTL][%d]", cmdheader->prdtl);
        // 8K bytes (16 sectors) per PRDT
        for (i=0; i<cmdheader->prdtl-1; i++)
        {
               cmdtbl->prdt_entry[i].dba = (DWORD)(buf & 0xFFFFFFFF);
                cmdtbl->prdt_entry[i].dbau = (DWORD)((buf << 32) & 0xFFFFFFFF);
                cmdtbl->prdt_entry[i].dbc = 8*1024-1;     // 8K bytes
                cmdtbl->prdt_entry[i].i = 0;
                buf += 4*1024;  // 4K words
                count -= 16;    // 16 sectors
       }
        /**If the final Data FIS transfer in a command is for an odd number of 16-bit words, the transmitter�s
Transport layer is responsible for padding the final Dword of a FIS with zeros. If the HBA receives one
more word than is indicated in the PRD table due to this padding requirement, the HBA shall not signal
this as an overflow condition. In addition, if the HBA inserts padding as required in a FIS it is transmitting,
an overflow error shall not be indicated. The PRD Byte Count field shall be updated based on the
number of words specified in the PRD table, ignoring any additional padding.**/

        // Last entry

        cmdtbl->prdt_entry[i].dba = (DWORD)(buf & 0xFFFFFFFF);
        cmdtbl->prdt_entry[i].dbau = (DWORD)((buf << 32) & 0xFFFFFFFF);
        cmdtbl->prdt_entry[i].dbc = count << 9 ;   // 512 bytes per sector
        cmdtbl->prdt_entry[i].i = 0;


        // Setup command
        FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);

        cmdfis->fis_type = FIS_TYPE_REG_H2D;
        cmdfis->c = 1;  // Command
        cmdfis->command = ATA_CMD_WRITE_DMA_EX;

        cmdfis->lba0 = (BYTE)startl;
        cmdfis->lba1 = (BYTE)(startl>>8);
        cmdfis->lba2 = (BYTE)(startl>>16);
        cmdfis->device = 1<<6;  // LBA mode

        cmdfis->lba3 = (BYTE)(startl>>24);
        cmdfis->lba4 = (BYTE)starth;
        cmdfis->lba5 = (BYTE)(starth>>8);

        cmdfis->countl = count & 0xff;
        cmdfis->counth = count>>8;

    //    print("[slot]{%d}", slot);
        port->ci = 1;    // Issue command
    //    print("\n[Port ci ][%d]", port->ci);
   //     print("\nafter issue : %d" , port->tfd);
    //    print("\nafter issue : %d" , port->tfd);

    //    print("\nbefore while 1--> %d", slot);
        // Wait for completion
        while (1)
        {
                // In some longer duration reads, it may be helpful to spin on the DPS bit
                // in the PxIS port field as well (1 << 5)
                if ((port->ci & (1<<slot)) == 0)
                        break;
                if (port->is & HBA_PxIS_TFES)   // Task file error
                {
                        printf("Read disk error\n");
                        return 0;
                }
        }
     //   print("\n after while 1");
     //   print("\nafter issue : %d" , port->tfd);
        // Check again
        if (port->is & HBA_PxIS_TFES)
        {
                printf("Read disk error\n");
                return 0;
        }

     //   print("\n[Port ci ][%d]", port->ci);
       // int k = 0;
        while(port->ci != 0)
        {
       //     print("[%d]", k++);
        }
        return 1;
}
// To setup command fing a free command list slot
int find_cmdslot(HBA_PORT *port)
{
    // An empty command slot has its respective bit cleared to �0� in both the PxCI and PxSACT registers.
        // If not set in SACT and CI, the slot is free // Checked
        DWORD slots = (port->sact | port->ci);
        int num_of_slots= (abar->cap & 0x0f00)>>8 ; // Bit 8-12
        int i;
        for (i=0; i<num_of_slots; i++)
        {

                if ((slots&1) == 0)
                {
                       // print("\n[command slot is : %d]", i);
                        return i;

                }
                slots >>= 1;
        }
                printf("Cannot find free command list entry\n");
        return -1;
}

