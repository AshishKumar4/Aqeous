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
             if(!strcmpy(pci_device_lookup(vendorID,deviceID),"AHCI")||!strcmpy(pci_device_lookup(vendorID,deviceID),"ahci"))
             {
                console_writestring(" AHCI CONTROLLER FOUND ");
                return 1;
             }
            out:
                ;
         }
         return 0;
 }

void probe_port(HBA_MEM *abar)
{
    AHCI=checkAHCI();
    return;
	// Search disk in impelemented ports
	/*DWORD pi = abar->pi;
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
	}*/
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
