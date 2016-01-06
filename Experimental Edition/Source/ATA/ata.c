#include <console.h>
#include <sys.h>
#include <ata.h>
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

int AHCI_test()
{
    if(inb(0x1F7)&0x01)
    {
        if(inb(0x1F4)||inb(0x1F5))
        {
            console_write_dec(inb(0x1F5));
            console_writestring("  ");
            console_write_dec(0x3C);
            return 1;
        }
    }
    return 0;
}

void init_ata() /** this 1 uses IDENTITY COMMAND to detect drives **/
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
        console_writestring(" Drive 0xA0 exists ");
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
            console_writestring(" Drive 0xA0 not compatible with ATA SPECS ");
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
            HDD="P";
            console_writestring(" initialized ");
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
            console_writestring(" Drive 0xB0 exists ");
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
                console_writestring(" Drive 0xB0 not compatible with ATA SPECS ");
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
                HDD="P";
                console_writestring(" initialized ");
            }
        }
        else
        {
            sleep(1000);
            if(0x147)
            if(AHCI_test())
            {
                console_writestring(" SATA DISK FOUND, INITIALIZED ");
            }
            else console_writestring(" NO COMPATIBLE DISK FOUND, RETURNING ");
            console_writestring(" NOT ATA COMPATIBLE ");
            return;
        }
    }
}

void initialise_ata() /** this 1 uses detection of controllers and non standard methods **/
{
    unsigned short controller=0x1F3,tmpword;
    while(1)
    {
        outb(0x1F3, 0x88);
        if(inb(0x1F3)==0x88)
        {
            console_writestring("controller: 0x1F3  ");
            break;
        }
        else
        {
            outb(0x176, 0x88);
            if(inb(0x176)==0x88)
            {
                controller=0x176;
                console_writestring("controller: 0x176  ");
                break;
            }
        }
    }
    if(controller==0x1F3)
    {
        outb(0x1F6, 0xA0); // use 0xB0 instead of 0xA0 to test the second drive on the controller
        for(int i=0;i<10000;i++){} // wait 1/250th of a second
        tmpword = inb(0x1F7); // read the status port
        if (tmpword & 0x40) // see if the busy bit is set
        {
            drive=0xA0;
            console_writestring("drive: 0xA0  ");
            goto init;
        }
        else
        {
            outb(0x1F6, 0xB0); // use 0xB0 instead of 0xA0 to test the second drive on the controller
            for(int i=0;i<10000;i++){} // wait 1/250th of a second
            tmpword = inb(0x1F7); // read the status port
            if (tmpword & 0x40) // see if the busy bit is set
            {
                drive=0xB0;
                console_writestring("drive: 0xB0  ");
                goto init;
            }
            else goto out;
        }
    }
    else if(controller==0x176)
    {
        outb(0x176, 0xA0); // use 0xB0 instead of 0xA0 to test the second drive on the controller
        for(int i=0;i<10000;i++){} // wait 1/250th of a second
        tmpword = inb(0x1F7); // read the status port
        if (tmpword & 0x40) // see if the busy bit is set
        {
            drive=0xA0;
            console_writestring("drive: 0xA0  ");
            goto init;
        }
        else
        {
            outb(0x1F6, 0xB0); // use 0xB0 instead of 0xA0 to test the second drive on the controller
            for(int i=0;i<10000;i++){} // wait 1/250th of a second
            tmpword = inb(0x1F7); // read the status port
            if (tmpword & 0x40) // see if the busy bit is set
            {
                drive=0xB0;
                console_writestring("drive: 0xB0  ");
                goto init;
            }
            else goto out;
        }
    }
    init:
    return;
    out:
        return;
}

char *read_ata(unsigned int addr)
{
    read_sector(addr);
    while (!(inb(0x1F7) & 0x08)) {}
    char *buffer;
    unsigned char tmpword;
    for (int idx = 0; idx < 256; idx++)
    {
        tmpword = inw(0x1F0);
        buffer[idx * 2] = (unsigned char)tmpword;
        buffer[idx * 2 + 1] = (unsigned char)(tmpword >> 8);
    }
    return buffer;
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

