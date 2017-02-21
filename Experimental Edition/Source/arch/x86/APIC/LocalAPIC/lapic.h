#ifndef LAPIC_H
#define LAPIC_H

#include "common.h"

typedef struct LAPIC_RegisterMAP
{
  uint32_t reserved1[4];
  uint32_t reserved2[4];
  uint32_t lapicID_Reg[4];     // Read/Write
  uint32_t lapicVER_Reg[4];
  uint32_t reserved3[4];
  uint32_t reserved4[4];
  uint32_t reserved5[4];
  uint32_t reserved6[4];
  uint32_t TaskPriority_Reg[4];
  uint32_t ArbitrationPriority_Reg[4];
  uint32_t ProcessorPriority_Reg[4];
  uint32_t EOI_Reg[4];    //0xB0
  uint32_t RemoreRead_Reg[4];
  uint32_t LogicalDestination_Reg[4];
  uint32_t DestinationFormat_Reg[4];
  uint32_t SpuriousINT_Vector_Reg[4];

  uint32_t ISR[4][8];   // In-Service Register
  uint32_t TMR[4][8];   // Trigger Mode Register
  uint32_t IRR[4][8];   // Interrupt Request Register

  uint32_t ErrorStatus_Reg[4];
  uint32_t reserved7[4][6];

  uint32_t LVT_CMCI_Reg[4];
  uint32_t ICR[2][4];
  uint32_t LVT_Timer_Reg[4];
  uint32_t LVT_ThermalSensor_Reg[4];
  uint32_t LVT_PerfMonitoringCounters_Reg[4];
  uint32_t LVT_LINT0_Reg[4];
  uint32_t LVT_LINT1_Reg[4];
  uint32_t LVT_Error_Reg[4];
  uint32_t InitialCount_Reg[4];
  uint32_t CurrentCount_Reg[4];
  uint32_t reserved8[4][4];
  uint32_t DivideConfig_Reg[4];
  uint32_t reserved9[4];
}LAPIC_RegisterMAP_t;

LAPIC_RegisterMAP_t* Lapic;


uint32_t localapic_read(uint32_t base, uint32_t reg);
void localapic_write(uint32_t base, uint32_t reg, uint32_t value);
void localapic_write_with_mask(uint32_t base, uint32_t reg, uint32_t mask, uint32_t value);

#endif
