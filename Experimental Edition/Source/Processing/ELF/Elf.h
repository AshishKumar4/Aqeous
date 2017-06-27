#ifndef ELF_h
#define ELF_h

#include "stdgcc.h"

#define EI_NIDENT   16

typedef uint16_t    Elf32_Half;
typedef uint32_t    Elf32_Addr;
typedef uint32_t    Elf32_Off;
typedef int32_t    Elf32_Sword;
typedef uint32_t    Elf32_Word;


/* e_type */
#define ET_NONE   0       //  No file type
#define ET_REL    1       //  Relocatable File
#define ET_EXEC   2       //  Executable File
#define ET_DYN    3       //  Shared Object File
#define ET_CORE   4       //  Core file
#define ET_LOPROC 0xff00  //  Processor-specific
#define ET_HTPROC 0Xffff  //  Processor-specific

/* e_machine */
#define EM_NONE   0       //  No machine
#define EM_M32    1       //  AT&T WE32100
#define EM_SPARC  2       //  SPARC
#define EM_386    3       //  Intel 80386
#define EM_68K    4       //  Motorola 68000
#define EM_88K    5       //  Motorola 88000
#define EM_860    7       // Intel 80860
#define EM_MIPS   8       //  MIPS RS3000

/* e_version */
#define EV_NONE    0       //  Invalid version
#define EV_CURRENT 1       //  Current Version


typedef struct
{
  unsigned char e_ident[EI_NIDENT];
  Elf32_Half    e_type;
  Elf32_Half    e_machine;
  Elf32_Word    e_version;
  Elf32_Addr    e_entry;
  Elf32_Off     e_phoff;
  Elf32_Off     e_shoff;
  Elf32_Word    e_flags;
  Elf32_Half    e_ehsize;
  Elf32_Half    e_phentsize;
  Elf32_Half    e_phnum;
  Elf32_Half    e_shentsize;
  Elf32_Half    e_shnum;
  Elf32_Half    e_shstrndx;
}Elf32_Ehdr;

#endif
