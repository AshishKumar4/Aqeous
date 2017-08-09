#ifndef ELFn_h
#define ELFn_h

#include "stdint.h"
#include "Processing/processing.h"

#define EI_NIDENT   16

typedef uint16_t    Elf32_Half;
typedef uint32_t    Elf32_Addr;
typedef uint32_t    Elf32_Off;
typedef int32_t    	Elf32_Sword;
typedef uint32_t    Elf32_Word;

#define ELF_MAGIC 0x464C457FU  // "\x7FELF" in little endian

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

enum Elf_Ident {
	EI_MAG0		= 0, // 0x7F
	EI_MAG1		= 1, // 'E'
	EI_MAG2		= 2, // 'L'
	EI_MAG3		= 3, // 'F'
	EI_CLASS	= 4, // Architecture (32/64)
	EI_DATA		= 5, // Byte Order
	EI_VERSION	= 6, // ELF Version
	EI_OSABI	= 7, // OS Specific
	EI_ABIVERSION	= 8, // OS Specific
	EI_PAD		= 9  // Padding
};

# define ELFMAG0	0x7F // e_ident[EI_MAG0]
# define ELFMAG1	'E'  // e_ident[EI_MAG1]
# define ELFMAG2	'L'  // e_ident[EI_MAG2]
# define ELFMAG3	'F'  // e_ident[EI_MAG3]

# define ELFDATA2LSB	(1)  // Little Endian
# define ELFCLASS32	(1)  // 32-bit Architecture

typedef struct Elf32_Ehdr
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
}Elf32_Ehdr_t;

typedef struct Elf32_Shdr
{
  Elf32_Word    sh_name;
  Elf32_Word    sh_type;
  Elf32_Word    sh_flags;
  Elf32_Addr    sh_addr;
  Elf32_Off     sh_offset;
  Elf32_Word    sh_size;
  Elf32_Word    sh_link;
  Elf32_Word    sh_info;
  Elf32_Word    sh_addralign;
  Elf32_Word    sh_entsize;
}Elf32_Shdr_t;

typedef struct Elf32_Phdr
{
	Elf32_Word		p_type;
	Elf32_Off			p_offset;
	Elf32_Addr		p_vaddr;
	Elf32_Addr		p_paddr;
	Elf32_Word		p_filesz;
	Elf32_Word		p_memsz;
	Elf32_Word		p_flags;
	Elf32_Word		p_align;
}Elf32_Phdr_t;

typedef struct BinFile
{
  uint16_t type;
  uint16_t reserved;
  uint32_t bin_sz;

	Process_t* proc;
	task_t* mainthread;

	char name[64];

  uintptr_t entry_point;
  uintptr_t sec_data;
  uintptr_t sec_text;
  uintptr_t sec_bss;

  Elf32_Ehdr_t* elf_ehdr;
  Elf32_Shdr_t* elf_shdr;
  Elf32_Shdr_t** elf_phdr;
}BinFile_t;

BinFile_t* Elf_LoadFile(const char* str);
uint32_t Elf_ProcLauncher_file(const char* str);

#endif
