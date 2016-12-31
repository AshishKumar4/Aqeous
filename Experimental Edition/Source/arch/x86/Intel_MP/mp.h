#ifndef MP_h
#define MP_h

#include "mp.h"
#include "stdio.h"
#include "common.h"
#include "stdlib.h"
#include "string.h"
#include "apic.h"

typedef struct MP_FloatingPointer
{
  uint32_t signature;   //  '_MP_'
  uint32_t ConfigTable_Ptr; //MP Configuration table address
  uint8_t length;
  uint8_t version;
  uint8_t checksum;
  uint8_t Feature_info_1;
  uint8_t Feature_info_2;
  uint8_t Feature_info_3to5[3];
}MP_FloatingPointer_t;

MP_FloatingPointer_t* mpfp;


typedef struct __attribute__((packed)) MP_ConfigTbl_Header
{
  uint32_t signature;   // 'PCMP'
  uint16_t baseLength;     // Length of base config table in bytes
  uint8_t version;        // Version
  uint8_t checksum;
  uint8_t OEM_ID[8];
  uint8_t Product_ID[12];
  uint32_t OEM_Table_ptr; // Optional OEM Defined Configuration table
  uint16_t OEM_Table_sz;  // Size of the Base OEM table in bytes
  uint16_t entry_count;   // Number of entries in variable portion of base table
  uint32_t LAPIC_addr;    // Base address by which each processor accesses its LAPIC
  uint16_t extTable_length; //Length in bytes of the extended entries that are located in memory at the end of the base configuration table. A zero value in this field indicates that no extended entries are present
  uint8_t  extTable_checksum;
  uint8_t reserved;
}MP_ConfigTbl_Header_t;

MP_ConfigTbl_Header_t* mpcth;

typedef struct __attribute__((packed)) ProcessorEntry
{
  uint8_t entry_type;
  uint8_t id; //LAPIC ID
  uint8_t version;
  uint8_t CPU_flags;
  uint32_t CPU_Signature;
  uint32_t Feature_flags;
  uint64_t reserved;
}ProcessorEntry_t;

ProcessorEntry_t* Processor_Entries;


void MPtables_parse();

#endif
