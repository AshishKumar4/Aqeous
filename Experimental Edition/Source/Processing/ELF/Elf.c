#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "common.h"

#include "ELF.h"
#include "tasking.h"
#include "task.h"
#include "process.h"
#include "Shell.h"
#include "Processing/processing.h"

#include "Shell.h"
#include "console.h"

uint32_t Elf_VerifyMagic(Elf32_Ehdr_t* hdr)
{
//  if(hdr->e_ident[0] == 0x7f && hdr->e_ident[1] == 'E' && hdr->e_ident[2] == 'L' && hdr->e_ident[3] == 'F')
  uint32_t* hmagic = (uint32_t*)hdr->e_ident;
  if(*hmagic == ELF_MAGIC)
    return 1;
  return 0;
}

char* Elf_GetSecName_id(BinFile_t* bfn, uint32_t index)
{
  Elf32_Shdr_t* strSh = &bfn->elf_shdr[bfn->elf_ehdr->e_shstrndx];
  char* str = (char*)(strSh->sh_offset + ((uint32_t)bfn->elf_ehdr));
  str += bfn->elf_shdr[index].sh_name;
  return str;
}

char* Elf_GetSecName(BinFile_t* bfn, Elf32_Shdr_t* shdr)
{
  Elf32_Shdr_t* strSh = &bfn->elf_shdr[bfn->elf_ehdr->e_shstrndx];
  char* str = (char*)(strSh->sh_offset + ((uint32_t)bfn->elf_ehdr));
  str += shdr->sh_name;
  return str;
}


void Elf_ShowInfo(BinFile_t* bfn)
{
  Elf32_Ehdr_t* ehdr = bfn->elf_ehdr;
  printf("\nElf Information =>");
  printf("\nType: %d\tMachine: %d\tVersion: %d", ehdr->e_type, ehdr->e_machine, ehdr->e_version);
  printf("\nEntry: %d\tPhOff: %d\tShOff: %d", ehdr->e_entry, ehdr->e_phoff, ehdr->e_shoff);
  printf("\nFlags: %d\tEhSize: %d\tPhentSize: %d\tPhNum: %d", ehdr->e_flags, ehdr->e_ehsize, ehdr->e_phentsize, ehdr->e_phnum);
  printf("\nShentSize: %d\tShNum: %d\tShshtndx: %d", ehdr->e_shentsize, ehdr->e_shnum, ehdr->e_shstrndx);

  Elf32_Shdr_t* shdr = bfn->elf_shdr;
  printf("\n\nSections-->");
  for(int i = 0; i < ehdr->e_shnum; i++)
  {
  //  printf("\nName: %s[%x]\tType: %d", Elf_GetSecName_id(bfn, i), Elf_GetSecName_id(bfn, i), shdr[i].sh_type);
    printf("\nFlags: %d\tAddr: %x\tOffset: %d", shdr[i].sh_flags, shdr[i].sh_addr, shdr[i].sh_offset);
    printf("\nSize: %d\tLink: %d\tInfo: %d", shdr[i].sh_size, shdr[i].sh_link, shdr[i].sh_info);
  }
}

BinFile_t* Elf_LoadFile(const char* str)
{
  FILE* elf_fl = fopen(str, "rbw");
  uint32_t sz = file_size(str);

  uint32_t* buffer = (uint32_t*)kmalloc(sz);
  fread(buffer, 1, sz, elf_fl);

  Elf32_Ehdr_t* hdr = (Elf32_Ehdr_t*)buffer;
  if(!Elf_VerifyMagic(hdr))
  {
    printf("\nNo ELF File Recognized!");
    return NULL;
  }

  BinFile_t* bfn = (BinFile_t*)kmalloc(sizeof(BinFile_t));
  bfn->bin_sz = sz;
  bfn->elf_ehdr = hdr;
  uint32_t phd = (uint32_t)buffer;
  phd += hdr->e_phoff;
  bfn->elf_phdr = (Elf32_Phdr_t**)phd;
  phd = (uint32_t)buffer;
  phd += hdr->e_shoff;
  bfn->elf_shdr = (Elf32_Shdr_t*)phd;

  bfn->entry_point = hdr->e_entry;

  strcpy(bfn->name, str);

  printf("\nElf File Program Headers: %d, Section Headers: %d", hdr->e_phnum, hdr->e_shnum);
  fclose(elf_fl);
  return bfn;
}

Process_t* Elf_CreateProc_fl(BinFile_t* bfn)
{
  Process_t* proc = create_process(bfn->name, (uintptr_t)bfn, 1, 0);
  bfn->proc = proc;
  map((uint32_t)bfn,4096,(PageDirectory_t*)proc->pgdir);
  map((uint32_t)bfn->elf_ehdr,bfn->bin_sz,(PageDirectory_t*)proc->pgdir);

  map((uint32_t)console_dbuffer, 4096*1024, (PageDirectory_t*)proc->pgdir);

  return proc;
}

task_t* Elf_CreateThread_fl(BinFile_t* bfn)
{
  printf("\n{%d}", bfn->entry_point);
  return create_task("test", bfn->entry_point, 29, 0x202, bfn->proc);
}

void Elf_DeployThreads(BinFile_t* bfn)
{
  Activate_task_direct(bfn->mainthread);
}

void Elf_LoadCode(BinFile_t* bfn)
{
  asm volatile("sti");
  Elf32_Ehdr_t* ehdr = bfn->elf_ehdr;
  Elf32_Shdr_t* shdr = bfn->elf_shdr;
  printf("\nLoading All Sections to memory...");
  for(int i = 0; i < ehdr->e_shnum; i++)
  {
/*    printf("\nName: %s[%x]\tType: %d", Elf_GetSecName_id(bfn, i), Elf_GetSecName_id(bfn, i), shdr[i].sh_type);
    printf("\nFlags: %d\tAddr: %x\tOffset: %d", shdr[i].sh_flags, shdr[i].sh_addr, shdr[i].sh_offset);
    printf("\nSize: %d\tLink: %d\tInfo: %d", shdr[i].sh_size, shdr[i].sh_link, shdr[i].sh_info);*/
    if(shdr[i].sh_addr)
    {
      memcpy_fast(shdr[i].sh_addr, (((uint32_t)bfn->elf_ehdr) + shdr[i].sh_offset), shdr[i].sh_size);
    }
    uint8_t* tmp = bfn->entry_point;
  }
}

void Elf_LaunchThread()
{
  task_t* self = task_GetSelf();
  Process_t* self_Proc = self->process;
  _stdprintf("\nProcess Name: %s", self_Proc->name);
  BinFile_t* bfn = (BinFile_t*)self_Proc->code;

  Elf_LoadCode(bfn);
  //Elf_ShowInfo(bfn);
  switch_directory(system_dir);
  bfn->mainthread = Elf_CreateThread_fl(bfn);
  Elf_DeployThreads(bfn);
  switch_directory(self_Proc->pgdir);

  //hexdump_8(bfn->entry_point, 10);
  kill();
}

uint32_t Elf_ProcLauncher_file(const char* str)
{
  BinFile_t* bfn = Elf_LoadFile(str);
  Process_t* proc = Elf_CreateProc_fl(bfn);
  task_t* elfLaunch = create_task("ElfLauncher", Elf_LaunchThread, 20, 0x202, proc);
  Activate_task_direct(elfLaunch);
  //TODO: Get The Process Name;
  //TODO: Create a New Page Directory for the Process; Fix Page Fault problems
  //TODO: Go to the address specified by program header. Then Place .text, .data etc sections
  //      Accordingly with offset into the memory.
  //Create Processes, Threads; Run it.
  //Activate_task_direct(tmpthread);
}

static inline Elf32_Shdr_t *elf_sheader(Elf32_Ehdr_t *hdr)
{
	return (Elf32_Shdr_t *)((int)hdr + hdr->e_shoff);
}

static inline Elf32_Shdr_t *elf_section(Elf32_Ehdr_t *hdr, int idx)
{
	return &elf_sheader(hdr)[idx];
}
