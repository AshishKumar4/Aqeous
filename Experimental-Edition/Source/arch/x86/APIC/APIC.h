#ifndef APIC_h
#define APIC_h

#include "acpi.h"

#define CPUID_FLAG_APIC 1<<9
uint32_t APIC_LOCAL_BASE=0xfee00000;
uint32_t APIC_IO_BASE=0xfec00000; //ONLY FOR QEMU AND BOCHS
uint32_t counter=0;
// ------------------------------------------------------------------------------------------------
// Local APIC Registers
#define LAPIC_ID                        0x0020  // Local APIC ID
#define LAPIC_VER                       0x0030  // Local APIC Version
#define LAPIC_TPR                       0x0080  // Task Priority
#define LAPIC_APR                       0x0090  // Arbitration Priority
#define LAPIC_PPR                       0x00a0  // Processor Priority
#define LAPIC_EOI                       0x00b0  // EOI
#define LAPIC_RRD                       0x00c0  // Remote Read
#define LAPIC_LDR                       0x00d0  // Logical Destination
#define LAPIC_DFR                       0x00e0  // Destination Format
#define LAPIC_SVR                       0x00f0  // Spurious Interrupt Vector
#define LAPIC_ISR                       0x0100  // In-Service (8 registers)
#define LAPIC_TMR                       0x0180  // Trigger Mode (8 registers)
#define LAPIC_IRR                       0x0200  // Interrupt Request (8 registers)
#define LAPIC_ESR                       0x0280  // Error Status
#define LAPIC_ICRLO                     0x0300  // Interrupt Command
#define LAPIC_ICRHI                     0x0310  // Interrupt Command [63:32]
#define LAPIC_TIMER                     0x0320  // LVT Timer
#define LAPIC_THERMAL                   0x0330  // LVT Thermal Sensor
#define LAPIC_PERF                      0x0340  // LVT Performance Counter
#define LAPIC_LINT0                     0x0350  // LVT LINT0
#define LAPIC_LINT1                     0x0360  // LVT LINT1
#define LAPIC_ERROR                     0x0370  // LVT Error
#define LAPIC_TICR                      0x0380  // Initial Count (for Timer)
#define LAPIC_TCCR                      0x0390  // Current Count (for Timer)
#define LAPIC_TDCR                      0x03e0  // Divide Configuration (for Timer)

// ------------------------------------------------------------------------------------------------
// Interrupt Command Register

// Delivery Mode
#define ICR_FIXED                       0x00000000
#define ICR_LOWEST                      0x00000100
#define ICR_SMI                         0x00000200
#define ICR_NMI                         0x00000400
#define ICR_INIT                        0x00000500
#define ICR_STARTUP                     0x00000600

// Destination Mode
#define ICR_PHYSICAL                    0x00000000
#define ICR_LOGICAL                     0x00000800

// Delivery Status
#define ICR_IDLE                        0x00000000
#define ICR_SEND_PENDING                0x00001000

// Level
#define ICR_DEASSERT                    0x00000000
#define ICR_ASSERT                      0x00004000

// Trigger Mode
#define ICR_EDGE                        0x00000000
#define ICR_LEVEL                       0x00008000

// Destination Shorthand
#define ICR_NO_SHORTHAND                0x00000000
#define ICR_SELF                        0x00040000
#define ICR_ALL_INCLUDING_SELF          0x00080000
#define ICR_ALL_EXCLUDING_SELF          0x000c0000

// Destination Field
#define ICR_DESTINATION_SHIFT           24

typedef struct __attribute__((packed)) MADT_ENTRY
{
  uint8_t type;
  uint8_t record_length;
  uint32_t rest_field[];
}madt_entry_t;

typedef struct __attribute__((packed)) IOAPIC_MADT_ENTRY
{
  uint8_t id;
  uint8_t reserved;
  uint32_t address;
  uint32_t gsib; //GLOBAL SYSTEM INTERRUPT BASE
  uint32_t rest_fields[];
}ioapic_entry_t;

typedef struct __attribute__((packed)) LAPIC_MADT_ENTRY
{
  uint8_t id;
  uint8_t reserved;
  uint32_t flags;
  uint32_t rest_fields[];
}lapic_entry_t;

typedef struct __attribute__((packed)) ISD_MADT_ENTRY
{
  uint8_t bus_source;
  uint8_t irq_source;
  uint32_t global_sys_int;
  uint16_t flags; //GLOBAL SYSTEM INTERRUPT BASE
  uint32_t rest_fields[];
}isd_entry_t;

ioapic_entry_t volatile *IOAPIC_entry;
lapic_entry_t volatile *LAPIC_entry;
isd_entry_t volatile *ISD_entry;

uint32_t localapic_read(uint32_t reg);
void localapic_write(uint32_t reg, uint32_t value);
void localapic_write_with_mask(uint32_t reg, uint32_t mask, uint32_t value);
void ioapic_set_irq(uint8_t irq, uint64_t apic_id, uint8_t vector);
void ioapic_write(uint32_t reg, uint32_t value);
uint32_t ioapic_read(uint32_t reg);

MADT_t* madt;

#endif
