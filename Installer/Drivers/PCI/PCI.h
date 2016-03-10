#ifndef PCI_h
#define PCI_h

#include <ata.h>
#include <pci_list.h>


#define PCI_VENDOR_ID            0x00 // 2
#define PCI_DEVICE_ID            0x02 // 2
#define PCI_COMMAND              0x04 // 2
#define PCI_STATUS               0x06 // 2
#define PCI_REVISION_ID          0x08 // 1

#define PCI_PROG_IF              0x09 // 1
#define PCI_SUBCLASS             0x0a // 1
#define PCI_CLASS                0x0b // 1
#define PCI_CACHE_LINE_SIZE      0x0c // 1
#define PCI_LATENCY_TIMER        0x0d // 1
#define PCI_HEADER_TYPE          0x0e // 1
#define PCI_BIST                 0x0f // 1
#define PCI_BAR0                 0x10 // 4
#define PCI_BAR1                 0x14 // 4
#define PCI_BAR2                 0x18 // 4
#define PCI_BAR3                 0x1C // 4
#define PCI_BAR4                 0x20 // 4
#define PCI_BAR5                 0x24 // 4

#define PCI_INTERRUPT_LINE       0x3C // 1

#define PCI_SECONDARY_BUS        0x09 // 1

#define PCI_HEADER_TYPE_DEVICE  0
#define PCI_HEADER_TYPE_BRIDGE  1
#define PCI_HEADER_TYPE_CARDBUS 2

#define PCI_TYPE_BRIDGE 0x0604
#define PCI_TYPE_SATA   0x0106

#define PCI_ADDRESS_PORT 0xCF8
#define PCI_VALUE_PORT   0xCFC

#define PCI_NONE 0xFFFF

typedef void (*pci_func_t)(uint32_t device, uint16_t vendor_id, uint16_t device_id, void * extra);

static inline int pci_extract_bus(uint32_t device) {
	return (uint8_t)((device >> 16));
}
static inline int pci_extract_slot(uint32_t device) {
	return (uint8_t)((device >> 8));
}
static inline int pci_extract_func(uint32_t device) {
	return (uint8_t)(device);
}

static inline uint32_t pci_get_addr(uint32_t device, int field) {
	return 0x80000000 | (pci_extract_bus(device) << 16) | (pci_extract_slot(device) << 11) | (pci_extract_func(device) << 8) | ((field) & 0xFC);
}

static inline uint32_t pci_box_device(int bus, int slot, int func) {
	return (uint32_t)((bus << 16) | (slot << 8) | func);
}

uint32_t pci_read_field(uint32_t device, int field, int size);
void pci_write_field(uint32_t device, int field, int size, uint32_t value);
uint16_t pci_find_type(uint32_t dev);
const char * pci_vendor_lookup(unsigned short vendor_id);
const char * pci_device_lookup(unsigned short vendor_id, unsigned short device_id);
void pci_scan_hit(pci_func_t f, uint32_t dev, void * extra);
void pci_scan_func(pci_func_t f, int type, int bus, int slot, int func, void * extra);
void pci_scan_slot(pci_func_t f, int type, int bus, int slot, void * extra);
void pci_scan_bus(pci_func_t f, int type, int bus, void * extra);
void pci_scan(pci_func_t f, int type, void * extra);
void checkAllBuses(void);

/* Structures */
typedef struct _PciNativeHeader
{
	/* 0x00 */
	uint16_t VendorId;
	uint16_t DeviceId;

	/* 0x04 */
	uint16_t Command;
	uint16_t Status;

	/* 0x08 */
	uint8_t  Revision;
	uint8_t  Interface;
	uint8_t  Subclass;
	uint8_t  Class;

	/* 0x0C */
	uint8_t  CacheLineSize;
	uint8_t  LatencyTimer;
	uint8_t  HeaderType;
	uint8_t  Bist;

	/* 0x10 */
	uint32_t Bar0;
	/* 0x14 */
	uint32_t Bar1;
	/* 0x18 */
	uint32_t Bar2;
	/* 0x1C */
	uint32_t Bar3;
	/* 0x20 */
	uint32_t Bar4;
	/* 0x24 */
	uint32_t Bar5;

	/* 0x28 */
	uint32_t CardbusCISPtr;
	/* 0x2C */
	uint16_t SubSystemVendorId;
	uint16_t SubSystemId;

	/* 0x30 */
	uint32_t ExpansionRomBaseAddr;

	/* 0x34 */
	uint32_t Reserved0;

	/* 0x38 */
	uint32_t Reserved1;

	/* 0x3C */
	uint8_t  InterruptLine;
	uint8_t  InterruptPin;
	uint8_t  MinGrant;
	uint8_t  MaxLatency;

} PciNativeHeader_t;

typedef struct _PcitoPciHeader
{
	/* 0x00 */
	uint16_t VendorId;
	uint16_t DeviceId;

	/* 0x04 */
	uint16_t Command;
	uint16_t Status;

	/* 0x08 */
	uint8_t  Revision;
	uint8_t  Interface;
	uint8_t  Subclass;
	uint8_t  Class;

	/* 0x0C */
	uint8_t  CacheLineSize;
	uint8_t  LatencyTimer;
	uint8_t  HeaderType;
	uint8_t  Bist;

	/* 0x10 */
	uint32_t Bar0;
	/* 0x14 */
	uint32_t Bar1;

	uint8_t	 PrimaryBus;
	uint8_t  SecondaryBus;
	uint8_t  SubOrdinateBus;
	uint8_t  SecLatencyTimer;

	uint8_t  ioBase;
	uint8_t  ioLimit;
	uint16_t SecondaryStatus;

	uint16_t MemBase;
	uint16_t MemLimit;

	uint32_t BaseUpper;

	uint32_t LimitUpper;

	uint16_t ioBaseUpper;
	uint16_t ioLimitUpper;

	uint8_t  CapabilityPtr;
	uint8_t  Reserved1;
	uint16_t Reserved2;

	uint32_t ExpansionRomBaseAddr;

	uint8_t  InterruptLine;
	uint8_t  InterruptPin;
	uint16_t BridgeControl;

} PcitoPciHeader_t;
/* The Driver Header */
typedef struct _PciDevice
{
	/* Type */
	uint32_t Type;

	/* Location */
 	s32int Bus;
	uint32_t Device;
	uint32_t Function;

	/* Information (Header) */
	PciNativeHeader_t *Header;

	/* Children (list.h) */
	void *Parent;
	void *Children;

} PciDevice_t;

static PciDevice_t devices[256][32];
static PciDevice_t AHCI_Devices[32];
static uint32_t TotalAHCIDevices=0;

#endif // SATA_h
