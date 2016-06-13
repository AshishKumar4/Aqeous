#ifndef ACPI_h
#define ACPI_h


typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef unsigned int qword;

dword *SMI_CMD;
byte ACPI_ENABLE;
byte ACPI_DISABLE;
dword *PM1a_CNT;
dword *PM1b_CNT;
word SLP_TYPa;
word SLP_TYPb;
word SLP_EN;
word SCI_EN;
byte PM1_CNT_LEN;



struct RSDPtr
{
   byte Signature[8];
   byte CheckSum;
   byte OemID[6];
   byte Revision;
   dword *RsdtAddress;
};

struct ACPI_SDTHeader
{
  char Signature[4];
  uint32_t length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
};

struct RSDT
{
  struct ACPI_SDTHeader h;
  uint32_t *PointerToOtherSDT;//[(h.Length - sizeof(h)) / 4];
};

struct FACP
{
   byte Signature[4];
   dword Length;
   byte unneded1[40 - 8];
   dword *DSDT;
   byte unneded2[48 - 44];
   dword *SMI_CMD;
   byte ACPI_ENABLE;
   byte ACPI_DISABLE;
   byte unneded3[64 - 54];
   dword *PM1a_CNT_BLK;
   dword *PM1b_CNT_BLK;
   byte unneded4[89 - 72];
   byte PM1_CNT_LEN;
};

typedef struct MADT
{
  char Signature[4];
  uint32_t Length;
  uint8_t revision;
  uint8_t checksum;
  char OEMID[6];
  char OEM_table_id[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorREV;
  uint32_t local_controller_addr;
  uint32_t flags;
  uint8_t rest_fields;
}MADT_t;


void acpiPowerOff(void);

int acpiCheckHeader(unsigned int *ptr, char *sig);

unsigned int *acpiGetRSDPtr(void);

unsigned int *acpiCheckRSDPtr(unsigned int *ptr);

#endif // ACPI_h
