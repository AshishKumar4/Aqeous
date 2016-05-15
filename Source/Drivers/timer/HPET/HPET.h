#ifndef HPET_h
#define HPET_h

typedef struct __attribute__((packed)) HPET_descriptor_table
{
    char     signature[4];      //‘HPET’.  Signature for the HPET Description Table.
    uint32_t length;            //Length, in bytes, of the entire Event Timer Description Table.
    uint8_t  revision;          //The revision of the HPET Description Table definition; currently 01h.
    uint8_t  checksum;          //Entire table must sum to zero.
    char     Oem_ID[6];            //Oem ID
    uint64_t Oem_tableID;       //For the Event Timer Description Table, the table ID is the manufacturer model ID.
    uint32_t Oem_revision_id;   //OEM revision of Event Timer Description Table for supplied OEM Table ID.
    uint32_t Creator_id;        //Vendor ID of utility that created the table. For the DSDT, RSDT, SSDT, and PSDT tables, this is the ID for the ASL Compiler.
    uint32_t Creator_revision;  //Revision of utility that created the table. For the DSDT, RSDT, SSDT, and PSDT tables, this is the revision for the ASL Compiler.
    uint32_t Evnt_timer_blk_id; //Hardware ID of Event Timer Block: Contents of General_Cap&ID Reg of Timer Block [31:16] = PCI Vendor ID of 1st Timer Block [15] = LegacyReplacement IRQ Routing Capable [14] = Reserved [13] = COUNT_SIZE_CAP counter size  [12:8] = Number of Comparators in 1st Timer Block [7:0] = Hardware Rev I
    uint32_t Base_address[3];   //The lower 32-bit base address of Event Timer Block.  Each Event Timer Block consumes 1K of system memory, regardless of how many comparators are actually implemented by the hardware.
    uint8_t  HPET_number;       //This one byte field indicates the HPET sequence number. 0 = 1st table, 1 = 2nd table and so forth. This field is written by BIOS at boot time and should not be altered by any other software.
    uint16_t Max_tick_main;     //Unit: Clock tick The minimum clock ticks can be set without lost interrupts while the counter is programmed to operate in periodic mode
    uint8_t  pg_protection;     //Also Oem Attribute. Rest is below
    //The lower 4-bit ( bit <0..3> ) of this field describes the timer hardware capability to guarantee the page protection. This information is required for the OSes that want to expose this timer to the user space:  0 = no guarantee for page protection.  1 = 4KB page protected, access to the adjacent 3KB space will not generate machine check or compromise the system security.  2 = 64KB page protected, access to the adjacent 63KB space will not generate machine check or compromise the system security.  3~ 15 = Reserved for future use.  The upper 4-bit (bits <4..7> of this field is reserved for OEM attributes: OEM can use this field for its implementation specific attributes. *
}HPET_descriptor_table_t;

typedef struct __attribute__((packed)) HPET_Table
{
    uint32_t GCIDReg[2];                        //General Capabilities and ID Register READ ONLY
    uint32_t reserved1[2];
    uint64_t GCReg;                          //General Configuration Register READ WRITE
    uint32_t reserved2[2];
    uint32_t GISReg[2];                         //General Interrupt Status Register READ/WRITE CLEAR. byte 40th.
    uint32_t reserved3[50];
    uint64_t Main_Counter_Reg;               //Main Counter Value Register, READ/WRITE byte 240
    uint32_t reserved4[2];
    uint32_t Timer0CCReg[2];
    uint64_t Timer0CVReg;
    uint32_t Timer0FSB_IntRReg[2];
    uint32_t reserved5[2];
    uint32_t Timer1CCReg[2];
    uint32_t Timer1CVReg[2];
    uint32_t Timer1FSB_IntRReg[2];
    uint32_t reserved6[2];
    uint32_t Timer2CCReg[2];
    uint32_t Timer2CVReg[2];
    uint32_t Timer2FSB_IntRReg[2];
    uint32_t reserved7[2];
    uint32_t reserved_Timer3to31[168];
}HPET_Table_t;

HPET_descriptor_table_t* hpet_sdt;

HPET_Table_t* hpet;

uint32_t* hpet_base = 0,hpet_virt_address=0;

#endif
