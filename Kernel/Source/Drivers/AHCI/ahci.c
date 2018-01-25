#include <ahci.h>
#include <string.h>
#include <sys.h>
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include <ata.h>
ahci_t *test1;
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
unsigned int controllers;

void start_cmd(HBA_PORT *port);
void stop_cmd(HBA_PORT *port);
int find_cmdslot(HBA_PORT *port);
unsigned int bus,device,func;

HBA_MEM *abar;
bool AHCI;

char *get_device_info(uint16_t vendorID, uint16_t deviceID)
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

uint8_t ahci_found=0;

uint32_t data_base=0;

void ahci_handler()
{
  printf("\nAHCI disk sent an intterupt\n");
}

int checkAHCI()
{
    AHCI_Controllers=(ahci_t*)phy_alloc4K();
    ahci_t* ahci = AHCI_Controllers;
	  //AHCI_BASE = (uint32_t)pmem_4k(64);//kmalloc(4*1024*1024);
    for(uint32_t ahcis=0;ahcis<TotalAHCIDevices;ahcis++)
    {
      ++controllers;
      ahci->ahci=&AHCI_Devices[ahcis];
      printf(get_device_info(ahci->ahci->Header->VendorId, ahci->ahci->Header->DeviceId));
      printf("\n\tAHCI CONTROLLER #");
      printint(controllers);
      printf(" FOUND, INITIALIZING AHCI CONTROLLER and Disks");

      abar=(HBA_MEM*)ROUNDDOWN(ahci->ahci->Header->Bar5, 4096);
      printf("\nABAR: %x, REST FIELDS: %x", abar, (ahci->ahci->Header->Bar5)%4096);

      probe_port(ahci);

      printf("\n\tAHCI CONTROLLER Initialized\n");
      ahci->ControllerID=controllers;
      ahci_found=1;
      ++ahci;
    }

    printf("\n");
    return controllers;
}

uint32_t *satatest;

int IDENTIFYdrive(Disk_dev_t* disk)
{
  disk->info = (SATA_ident_t*)phy_alloc4K();//kmalloc(512);
  SATA_Commander(disk->port,ATA_CMD_IDENTIFY,0,(DWORD)disk->info,1,512,0,0,0);
  SATA_ident_t* SATA_Identify_info = (SATA_ident_t*)disk->info;

  printf("\n\t The Data we Recieved from identity command:\n\t");
  printf("\n\t\tSerial Number: %s \n\t\tModel Number: %s\n\t\tFirmware: %s\n",
          SATA_Identify_info->serial_no,SATA_Identify_info->model,SATA_Identify_info->fw_rev);
  printf("\t\tTotal Logical Sectors: %x ",SATA_Identify_info->lba_capacity);
  printf("  Heads: %x Sectors: %x Cyclinders: %x Bytes per sectors: %x Bytes per track: %x"
          ,SATA_Identify_info->heads,SATA_Identify_info->sectors,SATA_Identify_info->cyls,
          SATA_Identify_info->sector_bytes,SATA_Identify_info->track_bytes);
  //str="YEPIEEE IT WORKS :D I CAN READ AND WRITE TO THIS HARD DISK :D \0";
  return 1;
}

void probe_port(ahci_t *ahci_c)//(HBA_MEM *abar)
{
  Disk_dev_t* Disk_dev;
	// Search disk in impelemented ports
	uint32_t pi = abar->pi;
  uint32_t temp2=0;
  int i = 0;
  printf("\nPorts: %x, CAP: %x, vs: %x", pi, abar->cap, abar->vs);
	while (i<32)
	{
		if (pi & 0x1)
		{
  			int dt = check_type(&abar->ports[i]);
        if(dt && !(abar->ports[i].cmd&(1<<0)))
    		{
            abar->ports[i].cmd|=(1<<0);
        }
        if (dt == AHCI_DEV_SATA)
  			{
            AHCI=1;
            Disk_dev = &ahci_c->Disk[disks];
            Disk_dev->type=1; //1=SATA
            Disk_dev->port=&abar->ports[i];
            ahci_c->Disks=disks;
            ++disks;
            ++sata;
            temp2|=(1<<i);
  				  //printf("\n\t\tSATA drive #%x found",sata);
            //port_rebase(&abar->ports[i],i);
            //test_sata(&abar->ports[i]);
            //int a=IDENTIFYdrive(&ahci_c->Disk[i]);
            //printf("\n\tDrive: %x ssts:%x sig:%x sact:%x ",a,port->ssts,port->sig,port->sact);

  			}
  			else if (dt == AHCI_DEV_SATAPI)
  			{
  			    AHCI=1;
            Disk_dev = &ahci_c->Disk[disks];
            Disk_dev->type=2; //2=SATAPI
            Disk_dev->port=&abar->ports[i];
            ahci_c->Disks=disks;
            ++Disk_dev;
            ++disks;
  				printf("\n\t\tSATAPI drive found \n");
  			}
  			else if (dt == AHCI_DEV_SEMB)
  			{
  			    AHCI=1;
            Disk_dev = &ahci_c->Disk[disks];
            Disk_dev->type=3; //3=SEMB
            Disk_dev->port=&abar->ports[i];
            ahci_c->Disks=disks;
            ++Disk_dev;
            ++disks;
  				printf("\n\t\tSEMB drive found \n");
  			}
  			else if (dt == AHCI_DEV_PM)
  			{
  			    AHCI=1;
            Disk_dev = &ahci_c->Disk[disks];
            Disk_dev->type=4; //4=PM
            Disk_dev->port=&abar->ports[i];
            ahci_c->Disks=disks;
            ++Disk_dev;
            ++disks;
  				printf("\n\t\tPM drive found \n");
  			}
  			else
  			{
  			}
      }
      pi>>=1;
  		//pi &=(1<<i);
  		i ++;
	}
  if(!AHCI)
  {
    printf("\n\t\tNo Drives Recognized on this controller\n");
    return;
  }
  for(int i=0,j=0;i<32;i++)
  {
    if((temp2 & (1<<i)))
    {
      printf("\n\n\t\tSata Disk #%x at port %x Found!\n",j,i);
      port_rebase(&abar->ports[i],i);
      IDENTIFYdrive(&ahci_c->Disk[j]);

      ++j;
      //test_sata(&abar->ports[i]);
    }
  }
}

// Check device type
int check_type(HBA_PORT *port)
{
  DWORD sact = port->sact;
	DWORD ssts = port->ssts;

	BYTE ipm = (ssts >> 8) & 0x0F;
  BYTE det = ssts & 0x0F;
  
  if(!sact)
    return AHCI_DEV_NULL;

  if(det == 1)
  {
    printf("\nDevice presence detected but Phy communication not established ");
  }
	else if (det != HBA_PORT_DET_PRESENT)	// Check drive status
	{
    return AHCI_DEV_NULL;
  }
	if (ipm != HBA_PORT_IPM_ACTIVE)
  {
    printf("\nInterface Not in active Mode");
    return AHCI_DEV_NULL;
  }
  
 // printf("\n{%x, %x, %x}", port->sig, port->sact, port->ssts);

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
  abar->ghc=(DWORD)(1<<31);
  abar->ghc=(DWORD)(1<<0);
  abar->ghc=(DWORD)(1<<31);
  abar->ghc=(DWORD)(1<<1);
	stop_cmd(port);	// Stop command engine

  port->cmd=port->cmd & 0xffff7fff; //Bit 15
  port->cmd=port->cmd & 0xffffbfff; //Bit 14
  port->cmd=port->cmd & 0xfffffffe; //Bit 0
  port->cmd=port->cmd & 0xfffffff7; //Bit 4

  port->serr = 0xffff;//For each implemented port, clear the PxSERR register, by writing 1 to each mplemented location
  port->is=0;
  //printf("command engine stopped");
	// Command list offset: 1K*portno
	// Command list entry size = 32
	// Command list entry maxim count = 32
	// Command list maxim size = 32*32 = 1K per port
	port->clb = phy_alloc4K();//AHCI_BASE + (portno*1024);
	port->clbu = 0;
	memset((void*)(port->clb), 0, 1024);

	// FIS offset: 32K+256*portno
	// FIS entry size = 256 bytes per port
	port->fb = phy_alloc4K();//AHCI_BASE + (32*1024) + (256*portno);
	port->fbu = 0;
	memset((void*)(port->fb), 0, 256);

	// Command table offset: 40K + 8K*portno
	// Command table size = 256*32 = 8K per port
  HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(port->clb);
  uint32_t base = phy_alloc4K();
	for (int i=0; i<16; i++)
	{
		cmdheader[i].prdtl = 8;	// 8 prdt entries per command table
					// 256 bytes per command table, 64+16+48+16*8
		// Command table offset: 40K + 8K*portno + cmdheader_index*256
		cmdheader[i].ctba = base + (i*256);//AHCI_BASE + (40*1024) + (portno*8*1024) + (i*256);
		cmdheader[i].ctbau = 0;
		memset((void*)cmdheader[i].ctba, 0, 256);
  }
  base = phy_alloc4K();
  for (int i=0; i<16; i++)
	{
		cmdheader[i].prdtl = 8;
		cmdheader[i].ctba = base + (i*256);
		cmdheader[i].ctbau = 0;
		memset((void*)cmdheader[i].ctba, 0, 256);
	}

	start_cmd(port);	// Start command engine
  port->is = 0;
  port->ie = 0;
}

// Start command engine
void start_cmd(HBA_PORT *port)
{
	// Wait until CR (bit15) is cleared
	//while (port->cmd & HBA_PxCMD_CR);

	// Set FRE (bit4) and ST (bit0)
	port->cmd |= HBA_PxCMD_FRE;
	port->cmd |= HBA_PxCMD_ST;
}

// Stop command engine
void stop_cmd(HBA_PORT *port)
{
	// Clear ST (bit0)
	port->cmd &= ~HBA_PxCMD_ST;
  port->cmd &= ~HBA_PxCMD_FRE;
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

int read(HBA_PORT *port, QWORD start, DWORD count, DWORD buf)
{
  if(!sata) return 0;
  for(DWORD i = 0; i < count/64; i++)
  {
    if(SATA_Commander(port,ATA_CMD_READ_SECTORS,0,buf,(WORD)((64-1)>>4) + 1,512*64,start & 0xffffffff,start >> 32, 64))
    {
      return 0;
    }
    buf += 32768;
    start += 64;
  }
  if(SATA_Commander(port,ATA_CMD_READ_SECTORS,0,buf,(WORD)((count-1)>>4) + 1,512*count,start & 0xffffffff,start >> 32,count))
  {
    return 0;
  }
	if (port->is & HBA_PxIS_TFES)
	{
    printf("Read disk error\n");
		return 0;
	}//*/
	return 1;

}

int write(HBA_PORT *port, QWORD start, DWORD count, DWORD buf)
{
  if(!sata) return 0;
  for(DWORD i = 0; i < count/64; i++)
  {
    if(SATA_Commander(port,ATA_CMD_WRITE_SECTORS,1,buf,(WORD)((64-1)>>4) + 1,512*64,start & 0xffffffff,start >> 32, 64))
    {
      return 0;
    }
    buf += 32768;
    start += 64;
  }
  if(SATA_Commander(port,ATA_CMD_WRITE_SECTORS,1,buf,(WORD)((count-1)>>4) + 1,512*count,start & 0xffffffff,start >> 32,count))
  {
    return 0;
  }
  // Check again
  if (port->is & HBA_PxIS_TFES)
  {
      printf("Write disk error\n");
      return 0;
  }//*/
  return 1;
}


int AqDirect_read(HBA_PORT *port, DWORD lower, DWORD higher, DWORD count, DWORD buf)
{
  if(!sata) return 0;
  for(DWORD i = 0; i < count/64; i++)
  {
    if(SATA_Commander(port,ATA_CMD_READ_SECTORS,0,buf,(WORD)((64-1)>>4) + 1,512*64, lower & 0xffffffff, higher, 64))
    {
      return 0;
    }
    buf += 32768;
    lower += 64;
  }
  if(SATA_Commander(port,ATA_CMD_READ_SECTORS,0,buf,(WORD)((count-1)>>4) + 1,512*count, lower & 0xffffffff, higher, count))
  {
    return 0;
  }
	if (port->is & HBA_PxIS_TFES)
	{
    printf("Read disk error\n");
    //while(1);
		return 0;
	}//*/
	return 1;

}

int AqDirect_write(HBA_PORT *port, DWORD lower, DWORD higher, DWORD count, DWORD buf)
{
  if(!sata) return 0;
  for(DWORD i = 0; i < count/64; i++)
  {
    if(SATA_Commander(port,ATA_CMD_WRITE_SECTORS,1,buf,(WORD)((64-1)>>4) + 1,512*64, lower & 0xffffffff, higher, 64))
    {
      return 0;
    }
    buf += 32768;
    lower += 64;
  }
  if(SATA_Commander(port,ATA_CMD_WRITE_SECTORS,1,buf,(WORD)((count-1)>>4) + 1,512*count, lower & 0xffffffff, higher,count))
  {
    return 0;
  }
  // Check again
  if (port->is & HBA_PxIS_TFES)
  {
      printf("Write disk error\n");
      //while(1);
      return 0;
  }//*/
  return 1;
}



/*** FOR EXT2 DRIVERS***/
int disk_read(HBA_PORT *port, DWORD start, DWORD count, uint32_t* buf)
{
  if(!sata) return 1;
  count /= SECTOR_SIZE;
  ++count;

  SATA_Commander(port,ATA_CMD_READ_SECTORS,0,(DWORD)buf,(WORD)((count-1)>>4) + 1,512*count,start & 0xffffffff, 0,count);
	if (port->is & HBA_PxIS_TFES)
	{
		printf("Read disk error\n");
		return 0;
	}
	return 1;

}

int disk_write_sector(HBA_PORT *port, uint32_t* buf, DWORD count, DWORD start)
{
  if(!sata) return 1;
  count /= SECTOR_SIZE;
  ++count;

  SATA_Commander(port,ATA_CMD_WRITE_SECTORS,1,(DWORD)buf,(WORD)((count-1)>>4) + 1,512*count,start & 0xffffffff, 0,count);
  // Check again
  if (port->is & HBA_PxIS_TFES)
  {
      printf("Write disk error\n");
      return 0;
  }
  return 1;
}

int disk_write(HBA_PORT *port, uint32_t* buf, DWORD count, DWORD start)
{
  if(!sata) return 1;
  uint32_t sz = count;
  count /= SECTOR_SIZE;

  if(count)
  {
    SATA_Commander(port,ATA_CMD_WRITE_SECTORS,1,(DWORD)buf,(WORD)((count-1)>>4) + 1,512*count,start & 0xffffffff, 0,count);
  	if (port->is & HBA_PxIS_TFES)
  	{
  		printf("Write disk error\n");
  		return 0;
  	}
    start += count;
    buf = (uint32_t*)((uint32_t)buf + (count*SECTOR_SIZE));
    sz %= SECTOR_SIZE;
  }

  if(sz)
  {
    uint32_t tmb = (uint32_t)kmalloc(SECTOR_SIZE);
    disk_read(port, start, 1, (uint32_t*)tmb);
    memcpy((void*)tmb, (void*)buf, sz);
    disk_write_sector(port, (uint32_t*)tmb, 1, start);
  }

	return 1;

}


/*** STATIC READ/WRITE ***/
int sec_read_static(HBA_PORT *port, QWORD start, DWORD count, DWORD buf)
{
  SATA_Commander(port,ATA_CMD_READ_SECTORS,0,buf,(WORD)((count-1)>>4) + 1,512*count,start & 0xffffffff,start >> 32,count);
	if (port->is & HBA_PxIS_TFES)
	{
		printf("Read disk error\n");
		return 0;
	}
	return 1;

}

int sec_write_static(HBA_PORT *port, QWORD start, DWORD count, DWORD buf)
{

  SATA_Commander(port,ATA_CMD_WRITE_SECTORS,1,buf,(WORD)((count-1)>>4) + 1,512*count,start & 0xffffffff,start >> 32,count);
  // Check again
  if (port->is & HBA_PxIS_TFES)
  {
      printf("Write disk error\n");
      return 0;
  }
  return 1;
}



int __attribute__((optimize("O0"))) SATA_Commander(HBA_PORT *port, WORD Command, BYTE rw, DWORD buf, DWORD prdtl, DWORD dbc, DWORD startl, DWORD starth, DWORD count)
{
  /***Make the Command Header***/
  HBA_CMD_HEADER *cmdhead=(HBA_CMD_HEADER*)port->clb;
  int free_slot = find_cmdslot(port);
  if(free_slot < 0) return 2;
  cmdhead += free_slot;
  cmdhead->cfl = sizeof(FIS_REG_H2D)/4;
  cmdhead->a=0;
  cmdhead->w = rw;
  cmdhead->prdtl = prdtl;	// PRDT entries count;
  cmdhead->p = 1;
  cmdhead->c = 1;

  /***Make the Command Table***/
  HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)cmdhead->ctba;//kmalloc(sizeof(HBA_CMD_TBL));
  //cmdhead->ctba = (DWORD)cmdtbl;

  uint32_t i=0;

  for(i=0;i<prdtl-1;i++)
  {
    cmdtbl->prdt_entry[i].dba = buf;
    cmdtbl->prdt_entry[i].dbau = 0;
    cmdtbl->prdt_entry[i].dbc = (8*1024) - 1;
    cmdtbl->prdt_entry[i].i = 0;   // interrupt when identify complete;
    buf += (8*1024);	// 4K words
    dbc -= (8*1024);
  }

  cmdtbl->prdt_entry[i].dba = buf;
  cmdtbl->prdt_entry[i].dbau = 0;
  cmdtbl->prdt_entry[i].dbc = dbc - 1;
  cmdtbl->prdt_entry[i].i = 0;   // interrupt

  /***Make the IDENTIFY DEVICE h2d FIS***/
  FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(cmdtbl->cfis);
  //printf("cmdfis %x ",cmdfis);
  memset((void*)cmdfis,0,sizeof(FIS_REG_H2D));
  cmdfis->fis_type = FIS_TYPE_REG_H2D;
  cmdfis->c = 1;
  cmdfis->command = Command;


  cmdfis->lba0 = (BYTE)startl;
	cmdfis->lba1 = (BYTE)(startl>>8);
	cmdfis->lba2 = (BYTE)(startl>>16);

	cmdfis->device = (1<<6);	// LBA mode

	cmdfis->lba3 = (BYTE)(startl>>24);
	cmdfis->lba4 = (BYTE)starth;
  cmdfis->lba5 = (BYTE)(starth>>8);

  cmdfis->countl = count & 0xff;
  cmdfis->counth = (count>>8);

/*
  int spin = 0;
  
  port->cmd |= HBA_PxCMD_FRE;
  port->cmd |= HBA_PxCMD_ST;
  port->cmd |= HBA_PxCMD_CLO;
  
  
  // The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ))  && spin < 1000000)
	{
  	spin++;
	}
	if (spin == 1000000)
	{
    printf("Port is hung: %x, %x", port->ci, port->tfd);
    asm volatile("hlt");
		return 2;
	}

 // printf("\n=>{%x}", port->ci);

  /***Send the Command***/

  port->ci |= (1<<free_slot);
  
  /***Wait for a reply***/
 // printf("{START}");
  while(1)
  {
    if ((port->ci & (1<<free_slot)) == 0) 
    {
      if (port->is & HBA_PxIS_TFES)
      {
        printf(" disk error\n");
        asm volatile("hlt");
        return 1;
      }
     // printf("{END} ");
      //stop_cmd(port);
      return 0;
    }
    if (port->is & HBA_PxIS_TFES)	// Task file error
		{
      printf(" disk error\n");
      asm volatile("hlt");
			return 1;
		}
    /*if(iii > 100)
    {
      break;
    }
    ++iii;*/
  }
  printf("\nError");
  asm volatile("hlt");
  return 2;
}
// To setup command fing a free command list slot
int find_cmdslot(HBA_PORT *port)
{
    // An empty command slot has its respective bit cleared to �0� in both the PxCI and PxSACT registers.
        // If not set in SACT and CI, the slot is free // Checked

        DWORD slots = (port->sact | port->ci);
        int cmdslots= (abar->cap & 0x0f00)>>8 ; // Bit 8-12
      	for (int i=0; i<cmdslots; i++)
      	{
      		if (!(slots&1))
      			return i;
      		slots >>= 1;
      	}
      	printf("Cannot find free command list entry\n");
      	return -1;
}
