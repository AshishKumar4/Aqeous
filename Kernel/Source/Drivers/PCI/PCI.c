#include <pci.h>
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include <sys.h>
#include "PCIe.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdio.h"
#include "common.h"
#include "acpi.h"


#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

const char * pci_vendor_lookup(unsigned short vendor_id) {
	for (unsigned int i = 0; i < PCI_VENTABLE_LEN; ++i) {
		if (PciVenTable[i].VenId == vendor_id) {
			return PciVenTable[i].VenFull;
		}
	}
	return "";
}

const char * pci_device_lookup(unsigned short vendor_id, unsigned short device_id) {
	for (unsigned int i = 0; i < PCI_DEVTABLE_LEN; ++i) {
		if (PciDevTable[i].VenId == vendor_id && PciDevTable[i].DevId == device_id) {
			return PciDevTable[i].ChipDesc;
		}
	}
	return "";
}

/* PCI Interface I/O */
uint32_t PciRead32(uint32_t Bus, uint32_t Device, uint32_t Function, uint32_t Register)
{
	/* Select Bus/Device/Function/Register */
	outl(PCI_ADDRESS_PORT, 0x80000000
		| (Bus << 16)
		| (Device << 11)
		| (Function << 8)
		| (Register & 0xFC));

	/* Read Data */
	return inl(PCI_VALUE_PORT);
}

uint16_t PciRead16(uint32_t Bus, uint32_t Device, uint32_t Function, uint32_t Register)
{
	/* Select Bus/Device/Function/Register */
	outl(PCI_ADDRESS_PORT, 0x80000000
		| (Bus << 16)
		| (Device << 11)
		| (Function << 8)
		| (Register & 0xFC));

	/* Read Data */
	return inw(PCI_VALUE_PORT + (Register & 0x2));
}

uint8_t PciRead8(uint32_t Bus, uint32_t Device, uint32_t Function, uint32_t Register)
{
	/* Select Bus/Device/Function/Register */
	outl(PCI_ADDRESS_PORT, 0x80000000
		| (Bus << 16)
		| (Device << 11)
		| (Function << 8)
		| (Register & 0xFC));

	/* Read Data */
	return inb(PCI_VALUE_PORT + (Register & 0x3));
}

/* Write functions */
void PciWrite32(uint32_t Bus, uint32_t Device, uint32_t Function, uint32_t Register, uint32_t Value)
{
	/* Select Bus/Device/Function/Register */
	outl(PCI_ADDRESS_PORT, 0x80000000
		| (Bus << 16)
		| (Device << 11)
		| (Function << 8)
		| (Register & 0xFC));

	/* Write DATA */
	outl(PCI_VALUE_PORT, Value);
}

void PciWrite16(uint32_t Bus, uint32_t Device, uint32_t Function, uint32_t Register, uint16_t Value)
{
	/* Select Bus/Device/Function/Register */
	outl(PCI_ADDRESS_PORT, 0x80000000
		| (Bus << 16)
		| (Device << 11)
		| (Function << 8)
		| (Register & 0xFC));

	/* Write DATA */
	outw(PCI_VALUE_PORT + (Register & 0x2), Value);
}

void PciWrite8(uint32_t Bus, uint32_t Device, uint32_t Function, uint32_t Register, uint8_t Value)
{
	/* Select Bus/Device/Function/Register */
	outl(PCI_ADDRESS_PORT, 0x80000000
		| (Bus << 16)
		| (Device << 11)
		| (Function << 8)
		| (Register & 0xFC));

	/* Write DATA */
	outb(PCI_VALUE_PORT + (Register & 0x03), Value);
}

/* Helpers */
uint32_t PciDeviceRead(PciDevice_t *Device, uint32_t Register, uint32_t Length)
{
	if (Length == 1)
		return (uint32_t)PciRead8(Device->Bus, Device->Device, Device->Function, Register);
	else if (Length == 2)
		return (uint32_t)PciRead16(Device->Bus, Device->Device, Device->Function, Register);
	else
		return PciRead32(Device->Bus, Device->Device, Device->Function, Register);
}

void PciDeviceWrite(PciDevice_t *Device, uint32_t Register, uint32_t Value, uint32_t Length)
{
	if (Length == 1)
		PciWrite8(Device->Bus, Device->Device, Device->Function, Register, (uint8_t)(Value & 0xFF));
	else if (Length == 2)
		PciWrite16(Device->Bus, Device->Device, Device->Function, Register, (uint16_t)(Value & 0xFFFFF));
	else
		PciWrite32(Device->Bus, Device->Device, Device->Function, Register, Value);
}

PciNativeHeader_t *tempPCS;
uint32_t lvl_depth = 0;

PciDevice_t* checkDevice(PciDevice_t* Parent, uint8_t bus, uint8_t device, uint8_t function)
{
	memset((void*)tempPCS,0,sizeof(PciNativeHeader_t));
	for(int i=0;i<64;i+=16)
	{
		*(uint32_t*)((uint32_t)tempPCS + i) = PciRead32(bus, device, function, i);
		*(uint32_t*)((uint32_t)tempPCS + i + 4) = PciRead32(bus, device, function, i + 4);
		*(uint32_t*)((uint32_t)tempPCS + i + 8) = PciRead32(bus, device, function, i + 8);
		*(uint32_t*)((uint32_t)tempPCS + i + 12) = PciRead32(bus, device, function, i + 12);
	}
	if(tempPCS->VendorId == 0xFFFF)     // Device doesn't exist
	{
			return NULL;
	}
	printf("\n\t DEVICE: %s CLASS: %x SUBCLASS: %x",pci_device_lookup(tempPCS->VendorId,tempPCS->DeviceId),
				 tempPCS->Class,tempPCS->Subclass);


	PciDevice_t* dev;
	if(tempPCS->Class==1&&tempPCS->Subclass==6)
	{
		dev = &AHCI_Devices[TotalAHCIDevices];
		++TotalAHCIDevices;
	}
	else
	{
		dev = kmalloc(sizeof(PciDevice_t));
	}

	if(tempPCS->HeaderType == 0)
	{
		PciNativeHeader_t *pcs = (PciNativeHeader_t*)&dev->Header_mem[0];
		memcpy(pcs, tempPCS, sizeof(PciNativeHeader_t));
		dev->Header = pcs;
		dev->PciBridgeHeader = NULL;
		dev->Bus=bus;
		dev->Device=device;
		dev->Function=function;

		if(Parent)
		{
			if(!Parent->Children) Parent->Children = dev;
			else Parent->last_child->friends = dev;
			Parent->last_child = dev;
			dev->friends = NULL;
		}
		else
		{
			if(!PCI_deviceNet_end)
			{
				PCI_deviceNet_start = dev;
				PCI_deviceNet_end = dev;
			}
			else
			{
				PCI_deviceNet_end->friends = dev;
				PCI_deviceNet_end = dev;
			}
			dev->friends = NULL;
		}
		dev->Parent = Parent;
		dev->Children = NULL;
	}
	else if(tempPCS->HeaderType == 1)
	{
		PcitoPciHeader_t *pcs = (PcitoPciHeader_t*)&dev->Header_mem[0];
		memcpy(pcs, tempPCS, sizeof(PcitoPciHeader_t));
		dev->Header = NULL;
		dev->PciBridgeHeader = pcs;
		dev->Bus=bus;
		dev->Device=device;
		dev->Function=function;

		if(Parent)
		{
			if(!Parent->Children) Parent->Children = dev;
			else Parent->last_child->friends = dev;
			Parent->last_child = dev;
			dev->friends = NULL;
		}
		else
		{
			if(!PCI_deviceNet_end)
			{
				PCI_deviceNet_start = dev;
				PCI_deviceNet_end = dev;
			}
			else
			{
				PCI_deviceNet_end->friends = dev;
				PCI_deviceNet_end = dev;
			}
			dev->friends = NULL;
		}
		dev->Parent = Parent;
		dev->Children = NULL;
	}
	else if(tempPCS->HeaderType == 0x80)
	{
		PciNativeHeader_t *pcs = (PciNativeHeader_t*)&dev->Header_mem[0];
		memcpy(pcs, tempPCS, sizeof(PciNativeHeader_t));
		dev->Header = pcs;
		dev->PciBridgeHeader = NULL;
		dev->Bus=bus;
		dev->Device=device;
		dev->Function=function;

		if(Parent)
		{
			if(!Parent->Children) Parent->Children = dev;
			else Parent->last_child->friends = dev;
			Parent->last_child = dev;
			dev->friends = NULL;
		}
		else
		{
			if(!PCI_deviceNet_end)
			{
				PCI_deviceNet_start = dev;
				PCI_deviceNet_end = dev;
			}
			else
			{
				PCI_deviceNet_end->friends = dev;
				PCI_deviceNet_end = dev;
			}
			dev->friends = NULL;
		}
		dev->Parent = Parent;
		dev->Children = NULL;



		if(lvl_depth >= 3)
		{
			lvl_depth = 0;
		}
		else 
		{
			++lvl_depth;
			for(int i = 1; i < 8; i++)
			{
				checkDevice(dev, bus, device, i);
			}
		}
	}
	else
	{
		printf("\nHeader Type: %x", tempPCS->HeaderType);
		return NULL;
	}
	/*if(dev->Header->Class==1&&dev->Header->Subclass==6)
	{
		memcpy(&AHCI_Devices[TotalAHCIDevices], dev, sizeof(PciDevice_t));
		++TotalAHCIDevices;
	}*/
	return dev;
}

int checkSecBus(PciDevice_t* dev, int bus, int device)
{
	printf("\nPCI-PCI Bridge, Sec bus: %d, SubOrdinateBus: %d, primary bus: %d, bus: %d, device: %d",
	 dev->PciBridgeHeader->SecondaryBus, dev->PciBridgeHeader->SubOrdinateBus, dev->PciBridgeHeader->PrimaryBus, bus, device);
}

void checkAllBuses(void)
{
		uint8_t bus;
		uint8_t device;
		tempPCS=(PciNativeHeader_t*)kmalloc(sizeof(PciNativeHeader_t));
		for(bus = 0; bus <255 ; bus++)
		{
			//printf("\n\nBUS: %d", bus);
				for(device = 0; device < 32; device++)
				{
						devices[bus][device] = checkDevice(NULL, bus, device, 0);
						PciDevice_t* dev=devices[bus][device];
						if(!dev) continue;
						if(dev->Header->HeaderType == 1)
						{
							 printf("\n\n  Secondary Bus number ");
							 checkSecBus(dev, bus,device);
							 //PciDevice_t devsec=checkSecBus(bus,device);
						}
				}
		}
}

void showDevInfo(PciDevice_t* dev)
{
	tempPCS = dev->Header;
	if(!dev->Device)	printf("\nBUS %d", dev->Bus);
	printf("\n\t DEVICE: %s CLASS: %x SUBCLASS: %x",pci_device_lookup(tempPCS->VendorId,tempPCS->DeviceId),
				 tempPCS->Class,tempPCS->Subclass);
	PciDevice_t* tmpDev = dev->Children;
	for(;tmpDev;)
	{
		//printf("\nSECONDARY ");
		showDevInfo(tmpDev);
		tmpDev = tmpDev->friends;
	}
	//printf("\n[%x]", dev->Header->HeaderType);
}

void ShowDevices()
{
	PciDevice_t* tmpDev = PCI_deviceNet_start;
	for(;tmpDev;)
	{
		showDevInfo(tmpDev);
		tmpDev = tmpDev->friends;
	}
}

uint32_t PciGetBar_1(int bus, int slot, int barno)
{
    if (!barno)
        return devices[bus][slot]->Header->Bar0;
    else if(barno==1)
        return devices[bus][slot]->Header->Bar1;
    else if(barno==2)
        return devices[bus][slot]->Header->Bar2;
    else if(barno==3)
        return devices[bus][slot]->Header->Bar3;
    else if(barno==4)
        return devices[bus][slot]->Header->Bar4;
    else if(barno==5)
        return devices[bus][slot]->Header->Bar5;
    else return -1;
}

uint32_t PciGetBar_2(PciDevice_t *device, int barno)
{
    if (!barno)
        return device->Header->Bar0;
    else if(barno==1)
        return device->Header->Bar1;
    else if(barno==2)
        return device->Header->Bar2;
    else if(barno==3)
        return device->Header->Bar3;
    else if(barno==4)
        return device->Header->Bar4;
    else if(barno==5)
        return device->Header->Bar5;
    else return -1;
}

uintptr_t find_MCFGtable()
{
  printf("\nSearching for MCFG Table:");
  uint32_t *addr;
  char *sig;

  // search below the 1mb mark for RSDP signature
  for (addr = (uint32_t *) 0x000E0000; (int) addr<0x00100000; addr ++)//= 0x10/sizeof(addr))
  {
    sig = (char*)addr;
    if(!strcmp(sig, "MCFG"))
    {
      printf("\nMCFG Table FOUND!");
      return (uintptr_t)addr;
    }
  }


  // at address 0x40:0x0E is the RM segment of the ebda
  int ebda = *((short *) 0x40E);   // get pointer
  ebda = ebda*0x10 &0x000FFFFF;   // transform segment into linear address

  // search Extended BIOS Data Area for the Root System Description Pointer signature
  for (addr = (uint32_t *) ebda; (int) addr<ebda+1024; addr+= 0x10/sizeof(addr))
  {
    sig = (char*)addr;
    if(!strcmp(sig, "MCFG"))
    {
      printf("\nMCFG Table FOUND!");
      return (uintptr_t)addr;
    }
  }
  printf("\n MCFG Table NOT FOUND!");
  return (uintptr_t)NULL;
}

void init_PCI()
{
  /*PCIe_base = find_MCFGtable();
  if(PCIe_base)
  {
    //Initialize PCIe
  }
  else
  {

  }*/
}
