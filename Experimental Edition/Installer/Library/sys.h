#ifndef SYS_H
#define SYS_H
#include <common.h>
#include <acpi.h>
#include <string.h>
#include <common.h>
#include <acpi.h>

void delay(uint32_t delay)
{
    float k=0;
    for(uint32_t i=0;i<delay;i++)
    {
      for(uint32_t j=0;j<delay;j++)
      {
        if(k)
          k=0;
        else k=1;
      }
    }
}

inline void outb(u16int port, u8int val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
}

inline u8int inb(u16int port)
{
    u8int ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
    return ret;
}

inline void outw(u16int port, u16int val)
{
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
}

inline u16int inw(u16int port)
{
    u16int ret;
    asm volatile ( "inw %1, %0" : "=a"(ret) : "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
    return ret;
}

inline void outpd(u16int port, u16int val)
{
    asm volatile ( "outpd %0, %1" : : "a"(val), "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
}

inline u16int inpd(u16int port)
{
    u16int ret;
    asm volatile ( "inpd %1, %0" : "=a"(ret) : "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
    return ret;
}


inline void outl(u16int port, u32int val)
{
    asm volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
}

inline u32int inl(u16int port)
{
    u32int ret;
    asm volatile ( "inl %1, %0" : "=a"(ret) : "Nd"(port) );
    /* TODO: Is it wrong to use 'N' for the port? It's not a 8-bit constant. */
    /* TODO: Should %1 be %w1? */
    return ret;
}

inline void io_wait(void)
{
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
    /* TODO: Is there any reason why al is forced? */
}

inline void sysManager(unsigned int todo) // 1 : reboot; 2 : shutdown; 3 : reserved
{
    if(todo==1)
        outb(0x64, 0xFE);
    else if(todo==2)
        acpiPowerOff();
    else if(todo==3)
        return;
}

#endif // SYS_H
