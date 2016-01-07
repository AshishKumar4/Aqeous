#include<ahci.h>
#include<string.h>
#include<sys.h>

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
#define AHCI_BASE   0x400000    // 4M
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

void start_cmd(HBA_PORT *port);
void stop_cmd(HBA_PORT *port);
int find_cmdslot(HBA_PORT *port);
unsigned int bus,device;

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
     uint8_t function = 0;

     for(bus = 0; bus < 2; bus++)
         for(device = 0; device < 32; device++)
         {
             unsigned short vendorID = pciConfigReadWord(bus, device, function,0);
             unsigned short deviceID = pciConfigReadWord(bus, device, function,0x00|0x02);
             unsigned char classcode = pciConfigReadWord(bus, device, function,0x08);
             unsigned char subclasscode = pciConfigReadWord(bus, device, function,0x08|0x02);
             if(vendorID == 0xFFFF) goto out;        // Device doesn't exist

             if(get_device_info(vendorID,deviceID))
             {
                 console_writestring(get_device_info(vendorID,deviceID));
                 console_writestring("\nAHCI CONTROLLER FOUND \n");
                 return 1;
             }
             if(!strcmpy(pci_device_lookup(vendorID,deviceID),"AHCI")||!strcmpy(pci_device_lookup(vendorID,deviceID),"ahci"))
             {\
                console_writestring("\nAHCI CONTROLLER FOUND\n");
                return 1;
             }
            out:
                ;
         }
         return 0;
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

