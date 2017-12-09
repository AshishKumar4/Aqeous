#include <console.h>
#include <sys.h>
#include <ata.h>
#include <pci.h>
#include <ahci.h>
inline void write_sector(unsigned int addr)
{
    outb(0x1F1, 0x00);
    outb(0x1F2, 0x01);
    outb(0x1F3, (unsigned char)addr);
    outb(0x1F4, (unsigned char)(addr >> 8));
    outb(0x1F5, (unsigned char)(addr >> 16));
    outb(0x1F6, 0xE0 | (drive << 4) | ((addr >> 24) & 0x0F));
    outb(0x1F7, 0x30);
}

inline void sleep(int ms)
{
    for(int i=0;i<ms*100;i++)
    {

    }
}

inline void read_sector(unsigned int addr)
{
    outb(0x1F1, 0x00);
    outb(0x1F2, 0x01);
    outb(0x1F3, (unsigned char)addr);
    outb(0x1F4, (unsigned char)(addr >> 8));
    outb(0x1F5, (unsigned char)(addr >> 16));
    outb(0x1F6, 0xE0 | (drive << 4) | ((addr >> 24) & 0x0F));
    outb(0x1F7, 0x20);
}

void HDD_A_Handler()
{
  printf("\nSecondary Hard Disk sent you an Interrupt\n");
}

void HDD_B_Handler()
{
  printf("\nPrimary Hard Disk sent you an Interrupt\n");
}

void init_ata() /** this 1 uses IDENTITY COMMAND to detect drives **/
{
    if(1)
    {
        outb(0x1F7,0xA0);
        sleep(10);
        outb(0x1F2,0);
        outb(0x1F3,0);
        outb(0x1F4,0);
        outb(0x1F5,0);
        outb(0x1F7,0xEC);
        sleep(10);
        if(inb(0x1F7)&&!(inb(0x1F7)&0x01))
        {
            printf(" Drive 0xA0 exists ");
                int i;
                for(i=0;(((inb(0x1F7)) & (1 << 7)) != 0);i++)
                {
                    if(i>=1000) //timeout
                    {
                        return;
                    }
                }
            if(inb(0x1F5)||inb(0x1F3))
            {
                printf(" Drive 0xA0 not compatible with ATA SPECS ");
                return;
            }

            // wait for the data to arrive - but timeout if it doesn't
            if( ( inb( 0x1F7 ) & 0x08 ) )
            {
                // counter
                int idx;

                // read in all the data
                unsigned short* identdata = (unsigned short*) &ident;
                for( idx = 0; idx < 255; idx++ )
                {
                    identdata[idx] = inw( 0x1F0 );
                }
                HDD='P';
                outb(0x1F0+ATA_REG_CONTROL,0x02);
                printf("\nHard Disk Initialized\n");
                printf(" PATA Info: Heads: ");
                printint(ident.heads);
                printf(" Sectors: ");
                printint(ident.sectors);
                printf(" Cylinders: ");
                printint(ident.cyls);
                printf(" Bytes per Sector ");
                printint(ident.sector_bytes);
                printf(" Bytes per Track ");
                printint(ident.track_bytes);
                printf(" Serial No: ");
                printf((char*)ident.serial_no);
            }
        }
        else
        {
            outb(0x1F7,0xB0);
            sleep(10);
            outb(0x1F2,0);
            outb(0x1F3,0);
            outb(0x1F4,0);
            outb(0x1F5,0);
            outb(0x1F7,0xEC);
            sleep(10);
            if(inb(0x1F7)&&!(inb(0x1F7)&0x01))
            {
                printf(" Drive 0xB0 exists ");
                int i;
                for(i=0;(((inb(0x1F7)) & (1 << 7)) != 0)&&(inb(0x1F7)&0x01);i++)
                {
                    if(i>=1000) //timeout
                    {
                        return;
                    }
                }
                if(inb(0x1F5)||inb(0x1F3))
                {
                    printf(" Drive 0xB0 not compatible with ATA SPECS ");
                    return;
                }
                // wait for the data to arrive - but timeout if it doesn't
                if( ( inb( 0x1F7 ) & 0x08 ) )
                {
                    // counter
                    int idx;

                    // read in all the data
                    unsigned short* identdata = (unsigned short*) &ident;
                    for( idx = 0; idx < 255; idx++ )
                    {
                        identdata[idx] = inw( 0x1F0 );
                    }
                    HDD='S';
                    outb(0x1F0+ATA_REG_CONTROL,0x02);
                    printf("\nHard Disk Initialized\n");
                    printf(" PATA Info: Heads: ");
                    printint(ident.heads);
                    printf(" Sectors: ");
                    printint(ident.sectors);
                    printf(" Cylinders: ");
                    printint(ident.cyls);
                    printf(" Bytes per Sector ");
                    printint(ident.sector_bytes);
                    printf(" Bytes per Track ");
                    printint(ident.track_bytes);
                    printf(" Serial No: ");
                    printf((char*)ident.serial_no);
                }
            }
            else
            {
                printf("\nNO HARDDISK FOUND!!!\n");
                return;
            }
        }
    }
    else
    {
        HDD='S';
    }
}

void read_ata(unsigned int addr,uint8_t *buffer)
{
    read_sector(addr);
    while (!(inb(0x1F7) & 0x08)) {}
    unsigned char tmpword;
    for (int idx = 0; idx < 256; idx++)
    {
        tmpword = inw(0x1F0);
        buffer[idx * 2] = (unsigned char)tmpword;
        buffer[idx * 2 + 1] = (unsigned char)(tmpword >> 8);
    }
}

void write_ata(char *buffer,unsigned int addr)
{
    write_sector(addr);
    while (!(inb(0x1F7) & 0x08)) {}
    unsigned char tmpword;
    for (int idx = 0; idx < 256; idx++)
    {
        tmpword = buffer[8 + idx * 2] | (buffer[8 + idx * 2 + 1] << 8);
        outw(0x1F0, tmpword);
    }
}

#define SECTORSIZE		512
#define DISK_PORT		0x1F0

typedef struct {
	uint8_t  status;
	uint8_t  chs_first_sector[3];
	uint8_t  type;
	uint8_t  chs_last_sector[3];
	uint32_t lba_first_sector;
	uint32_t sector_count;
} partition_t;

typedef struct {
	uint16_t flags;
	uint16_t unused1[9];
	char     serial[20];
	uint16_t unused2[3];
	char     firmware[8];
	char     model[40];
	uint16_t sectors_per_int;
	uint16_t unused3;
	uint16_t capabilities[2];
	uint16_t unused4[2];
	uint16_t valid_ext_data;
	uint16_t unused5[5];
	uint16_t size_of_rw_mult;
	uint32_t sectors_28;
	uint16_t unused6[38];
	uint64_t sectors_48;
	uint16_t unused7[152];
} __attribute__((packed)) ata_identify_t;

typedef struct {
	uint8_t     boostrap[446];
	partition_t partitions[4];
	uint8_t     signature[2];
} __attribute__((packed)) mbr_t;
mbr_t mbr;


mbr_t mbr;

int read_partition_map()
{
	read_ata(0,(uint8_t*)&mbr);

	if (mbr.signature[0] == 0x55 && mbr.signature[1] == 0xAA) {
		printf("Partition table found.");

		for (int i = 0; i < 4; ++i) {
			if (mbr.partitions[i].status & 0x80) {
				printf("Partition #%d: @%d+%d");
			} else {
				printf("Partition #%d: inactive");
			}
		}

		return 0;
	}
	else
    {
		printf("Did not find partition table ");
		printint(mbr.signature[0]);
		printf(" ");
		printint(mbr.signature[1]);

		printf("Parsing anyone yields:");

		for (int i = 0; i < 4; ++i) {
			if (mbr.partitions[i].status & 0x80) {
				printf(" Partition ");
                printint(i+1);
                printf(" : ");
                printint(mbr.partitions[i].lba_first_sector);
                printf(" ");
                printint(mbr.partitions[i].sector_count);
			} else {
				printf("Partition inactive:");
                printint(i+1);
                printf(" ");
			}
		}


	}

	return 1;
}
