#include <pci.h>
#include <mem.h>
#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC
PciDevice_t devices[256][32];


void pci_write_field(uint32_t device, int field, int size, uint32_t value) {
	outl(PCI_ADDRESS_PORT, pci_get_addr(device, field));
	outl(PCI_VALUE_PORT, value);
}

uint32_t pci_read_field(uint32_t device, int field, int size) {
	outl(PCI_ADDRESS_PORT, pci_get_addr(device, field));

	if (size == 4) {
		uint32_t t = inl(PCI_VALUE_PORT);
		return t;
	} else if (size == 2) {
		uint16_t t = inw(PCI_VALUE_PORT + (field & 2));
		return t;
	} else if (size == 1) {
		uint8_t t = inb(PCI_VALUE_PORT + (field & 3));
		return t;
	}
	return 0xFFFF;
}

uint16_t pci_find_type(uint32_t dev) {
	return (pci_read_field(dev, PCI_CLASS, 1) << 8) | pci_read_field(dev, PCI_SUBCLASS, 1);
}

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

void pci_scan_hit(pci_func_t f, uint32_t dev, void * extra) {
	int dev_vend = (int)pci_read_field(dev, PCI_VENDOR_ID, 2);
	int dev_dvid = (int)pci_read_field(dev, PCI_DEVICE_ID, 2);

	f(dev, dev_vend, dev_dvid, extra);
}

void pci_scan_func(pci_func_t f, int type, int bus, int slot, int func, void * extra) {
	uint32_t dev = pci_box_device(bus, slot, func);
	if (type == -1 || type == pci_find_type(dev)) {
		pci_scan_hit(f, dev, extra);
	}
	if (pci_find_type(dev) == PCI_TYPE_BRIDGE) {
		pci_scan_bus(f, type, pci_read_field(dev, PCI_SECONDARY_BUS, 1), extra);
	}
}

void pci_scan_slot(pci_func_t f, int type, int bus, int slot, void * extra) {
	uint32_t dev = pci_box_device(bus, slot, 0);
	if (pci_read_field(dev, PCI_VENDOR_ID, 2) == PCI_NONE) {
		return;
	}
	pci_scan_func(f, type, bus, slot, 0, extra);
	if (!pci_read_field(dev, PCI_HEADER_TYPE, 1)) {
		return;
	}
	for (int func = 1; func < 8; func++) {
		uint32_t dev = pci_box_device(bus, slot, func);
		if (pci_read_field(dev, PCI_VENDOR_ID, 2) != PCI_NONE) {
			pci_scan_func(f, type, bus, slot, func, extra);
		}
	}
}

void pci_scan_bus(pci_func_t f, int type, int bus, void * extra) {
	for (int slot = 0; slot < 32; ++slot) {
		pci_scan_slot(f, type, bus, slot, extra);
	}
}

void pci_scan(pci_func_t f, int type, void * extra) {
	pci_scan_bus(f, type, 0, extra);

	if (!pci_read_field(0, PCI_HEADER_TYPE, 1)) {
		return;
	}

	for (int func = 1; func < 8; ++func) {
		uint32_t dev = pci_box_device(0, 0, func);
		if (pci_read_field(dev, PCI_VENDOR_ID, 2) != PCI_NONE) {
			pci_scan_bus(f, type, func, extra);
		} else {
			break;
		}
	}
}

// byte -- 8 bits
// word -- 16 bits
// long -- 64 bits

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


PciDevice_t checkDevice(uint8_t bus, uint8_t device)
{
     uint8_t function = 0;
     PciNativeHeader_t *pcs=kmalloc(sizeof(PciNativeHeader_t));
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
     /*unsigned short vendorID = dev.Header->VendorId;
     unsigned short deviceID = dev.Header->DeviceId;
     unsigned char classcode = dev.Header->Class;
     unsigned char subclasscode = dev.Header->Subclass;*/
     if(dev.Header->VendorId == 0xFFFF)     // Device doesn't exist
     {
         PciDevice_t null;
         null.Bus=-1;
         null.Device=-1;
         null.Function=-1;
         null.Type=-1;
         null.Header=(PciNativeHeader_t*)NULL;
         return null;
     }
     /*console_writestring("\n\tDEVICE: ");
     console_writestring(pci_device_lookup(vendorID,deviceID));
     console_writestring(" CLASS: ");
     console_write_dec(classcode);
     console_writestring(" subCLASS: ");
     console_write_dec(subclasscode);*/
     dev.Function=function;
     return dev;
 }

void checkAllBuses(void)
 {
     uint8_t bus;
     uint8_t device;
     uint8_t flag=0;
     for(bus = 0; bus <255 ; bus++)
     {
         for(device = 0; device < 32; device++)
         {
             devices[bus][device]=checkDevice(bus, device);
             PciDevice_t dev=devices[bus][device];
             unsigned short vendorID = dev.Header->VendorId;
             unsigned short deviceID = dev.Header->DeviceId;
             unsigned char classcode = dev.Header->Class;
             unsigned char subclasscode = dev.Header->Subclass;
             if(devices[bus][device].Bus!=-1&&devices[bus][device].Header->Class!=255)
             {
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
