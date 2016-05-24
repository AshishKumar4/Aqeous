#include <pci.h>
#include <mem.h>
#include <vmem.h>
#include <sys.h>

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

unsigned short pciConfigReadWord (unsigned short bus, unsigned short slot,unsigned short func, unsigned short offset){
   // unsigned long address;
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    unsigned short tmp = 0;

    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

    /* write out the address */

    outl (0xCF8, address);

    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the fisrt word of the 32 bits register */

    tmp = (unsigned short)((inl (0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
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

PciDevice_t checkDevice(uint8_t bus, uint8_t device)
{
     uint8_t function = 0;
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
         PciDevice_t null;
         null.Bus=-1;
         null.Device=-1;
         null.Function=-1;
         null.Type=-1;
         null.Header=(PciNativeHeader_t*)NULL;
         return null;
     }

     PciNativeHeader_t *pcs=(PciNativeHeader_t*)kmalloc(sizeof(PciNativeHeader_t));
	    for(int i=0;i<64;i+=16)
			{
				*(uint32_t*)((uint32_t)pcs + i) = PciRead32(bus, device, function, i);
				*(uint32_t*)((uint32_t)pcs + i + 4) = PciRead32(bus, device, function, i + 4);
				*(uint32_t*)((uint32_t)pcs + i + 8) = PciRead32(bus, device, function, i + 8);
				*(uint32_t*)((uint32_t)pcs + i + 12) = PciRead32(bus, device, function, i + 12);
			}
	   PciDevice_t dev;
	 	 dev.Header=pcs;
		 dev.Bus=bus;
		 dev.Device=device;
     dev.Function=function;
     return dev;
 }

 int checkSecBus(uint32_t bus, uint32_t device)
 {
		 PcitoPciHeader_t* secpcs=(PcitoPciHeader_t*)kmalloc(sizeof(PcitoPciHeader_t));
		 for(int i=0;i<64;i+=16)
		 {
				*(uint32_t*)((uint32_t)secpcs + i) = PciRead32(bus, device, 0, i);
				*(uint32_t*)((uint32_t)secpcs + i + 4) = PciRead32(bus, device, 0, i + 4);
				*(uint32_t*)((uint32_t)secpcs + i + 8) = PciRead32(bus, device, 0, i + 8);
				*(uint32_t*)((uint32_t)secpcs + i + 12) = PciRead32(bus, device, 0, i + 12);
		 }
		 bus=secpcs->SecondaryBus;
		 device=0;
		 for(int function=0;function<8;function++)
	 	 {
			 PciNativeHeader_t* pcs=(PciNativeHeader_t*)kmalloc(sizeof(PciNativeHeader_t));
		    for(int i=0;i<64;i+=16)
				{
					*(uint32_t*)((uint32_t)pcs + i) = PciRead32(bus, device, function, i);
					*(uint32_t*)((uint32_t)pcs + i + 4) = PciRead32(bus, device, function, i + 4);
					*(uint32_t*)((uint32_t)pcs + i + 8) = PciRead32(bus, device, function, i + 8);
					*(uint32_t*)((uint32_t)pcs + i + 12) = PciRead32(bus, device, function, i + 12);
				}
				++bus;
				++device;
			 if(pcs->VendorId == 0xFFFF)
			 {
				 kfree((uint32_t*)pcs);
			 }
			 else
		   {
				 PciDevice_t dev;
			 	 dev.Header=pcs;
				 dev.Bus=bus;
				 dev.Device=device;
		     dev.Function=function;
				 printf("\n\tSecondary DEVICE: %s CLASS: %x SUBCLASS: %x",pci_device_lookup(dev.Header->VendorId,dev.Header->DeviceId),
			 					dev.Header->Class,dev.Header->Subclass);
				}
			}
			return 0;
 }

void checkAllBuses(void)
 {
     uint8_t bus;
     uint8_t device;
     uint8_t flag=0;
		 tempPCS=(PciNativeHeader_t*)kmalloc(sizeof(PciNativeHeader_t));
     for(bus = 0; bus <255 ; bus++)
     {
         for(device = 0; device < 32; device++)
         {
             devices[bus][device]=checkDevice(bus, device);
             PciDevice_t dev=devices[bus][device];
						 if(dev.Header->HeaderType == 1)
						 {
							 	printf("\n\n  Secondary Bus number ");
								checkSecBus(bus,device);
								//PciDevice_t devsec=checkSecBus(bus,device);
						 }
						 else
             {
							 unsigned short vendorID = dev.Header->VendorId;
	             unsigned short deviceID = dev.Header->DeviceId;
	             unsigned char classcode = dev.Header->Class;
	             unsigned char subclasscode = dev.Header->Subclass;
	             if(devices[bus][device].Bus!=-1&&devices[bus][device].Header->Class!=255)
			          {
									 if(classcode==1&&subclasscode==6)
									 {
										 AHCI_Devices[TotalAHCIDevices]=dev;
										 ++TotalAHCIDevices;
									 }
	                 if(!flag)
	                 {
	                     flag=1;
	                     console_writestring("\n\n  BUS number ");
	                     console_write_dec(bus);
	                     console_writestring("\n");
	                 }
	                 console_writestring("\n\tDEVICE: ");
	                 console_writestring(pci_device_lookup(vendorID,deviceID));
	                 console_writestring(" CLASS: ");
	                 console_write_dec(classcode);
	                 console_writestring(" subCLASS: ");
	                 console_write_dec(subclasscode);
	             	}
					 		}
         }
        flag=0;
     }
     console_writestring("\n\n");
 }

uint32_t PciGetBar_1(int bus, int slot, int barno)
{
    if (!barno)
        return devices[bus][slot].Header->Bar0;
    else if(barno==1)
        return devices[bus][slot].Header->Bar1;
    else if(barno==2)
        return devices[bus][slot].Header->Bar2;
    else if(barno==3)
        return devices[bus][slot].Header->Bar3;
    else if(barno==4)
        return devices[bus][slot].Header->Bar4;
    else if(barno==5)
        return devices[bus][slot].Header->Bar5;
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
