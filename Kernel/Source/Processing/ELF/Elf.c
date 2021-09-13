#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "common.h"

#include "Elf.h"
#include "MultiTasking/tasking.h"
#include "MultiTasking/task.h"
#include "MultiTasking/process.h"
#include "Shell/shell.h"
#include "Processing/processing.h"

#include "Shell/shell.h"
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
  for(int i = 0; i < ehdr->e_shnum - 1; i++)
  {
    printf("\nName: %s[%x]\tType: %d", Elf_GetSecName_id(bfn, i), Elf_GetSecName_id(bfn, i), shdr[i].sh_type);
    printf("\nFlags: %d\tAddr: %x\tOffset: %d", shdr[i].sh_flags, shdr[i].sh_addr, shdr[i].sh_offset);
    printf("\nSize: %d\tLink: %d\tInfo: %d", shdr[i].sh_size, shdr[i].sh_link, shdr[i].sh_info);
  }
}

BinFile_t* Elf_LoadFile(const char* str)
{
  FILE* elf_fl = fopen(str, "rbw");
  if(!elf_fl) 
  {
    return NULL;
  }
  uint32_t sz;
  
  fseek(elf_fl, 0, SEEK_END);
  sz = ftell(elf_fl);
  fseek(elf_fl, 0, SEEK_CUR);

  if(!sz) 
  {
    return NULL;
  }

  uint32_t* buffer = (uint32_t*)kmalloc(sz);
  if(!fread(buffer, 1, sz, elf_fl))
  {
    return NULL;
  }

  Elf32_Ehdr_t* hdr = (Elf32_Ehdr_t*)buffer;
  if(!Elf_VerifyMagic(hdr))
  {
    printf("\nNo ELF File Recognized!");
    return NULL;
  }

  BinFile_t* bfn = (BinFile_t*)phy_alloc4K();//kmalloc(sizeof(BinFile_t));
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
  printf("\nElf File loaded...");
  //printf("\nElf File Program Headers: %d, Section Headers: %d", hdr->e_phnum, hdr->e_shnum);
  fclose(elf_fl);
  return bfn;
}

Process_t* Elf_CreateProc_fl(BinFile_t* bfn)
{
  Process_t* proc = create_process(bfn->name, 0, 1, 0);
  proc->code = bfn;
  bfn->proc = proc;
  map_readOnly((uint32_t)bfn,4096,(PageDirectory_t*)proc->pgdir);
  map_readOnly((uint32_t)bfn->elf_ehdr,bfn->bin_sz,(PageDirectory_t*)proc->pgdir);

	page_t* page = get_page(0xC0003000, 1, (PageDirectory_t*)proc->pgdir);   // Shell Arguments 
	*page = bfn->sh_frame;
	*page |= I86_PTE_PRESENT;
	*page |= I86_PTE_WRITABLE;
  *page |= I86_PTE_USER;
  
  page = get_page(0xC0010000, 1, (PageDirectory_t*)proc->pgdir);   // Kernel Message Space
	*page = 0x10010000;
	*page |= I86_PTE_PRESENT;
	*page |= I86_PTE_WRITABLE;
	*page |= I86_PTE_USER;
  //map((uint32_t)console_dbuffer, 4096*1024, (PageDirectory_t*)proc->pgdir);
  //proc->pgdir = system_dir;

  return proc;
}
task_t* Elf_CreateThread_fl(BinFile_t* bfn)
{
  return create_task("mainthread", bfn->entry_point, 5, 0x202, bfn->proc);
}

void Elf_DeployThreads(BinFile_t* bfn)
{
  /*Activate_task_direct_SP(bfn->mainthread, &KitList[3]);
 /*/ 
  Activate_task_direct(bfn->mainthread); //*/
}

void Elf_LoadCode(BinFile_t* bfn)
{
  Elf32_Ehdr_t* ehdr = bfn->elf_ehdr;
  Elf32_Shdr_t* shdr = bfn->elf_shdr;
  //printf("\nLoading All Sections to memory...");
  for(int i = 0; i < ehdr->e_shnum - 1; i++)
  {
   /* printf("\nName: %s[%x]\tType: %d", Elf_GetSecName_id(bfn, i), Elf_GetSecName_id(bfn, i), shdr[i].sh_type);
    printf("\nFlags: %d\tAddr: %x\tOffset: %d", shdr[i].sh_flags, shdr[i].sh_addr, shdr[i].sh_offset);
    printf("\nSize: %d\tLink: %d\tInfo: %d", shdr[i].sh_size, shdr[i].sh_link, shdr[i].sh_info);*/
    if(shdr[i].sh_addr)
    {
      memcpy_fast(shdr[i].sh_addr, (((uint32_t)bfn->elf_ehdr) + shdr[i].sh_offset), shdr[i].sh_size);
    }
  }
}

static void Elf_LaunchThread()
{
  asm volatile("cli");
  switch_directory(system_dir);
  task_t* self = task_GetSelf();
  Process_t* self_Proc = self->process;
  BinFile_t* bfn = (BinFile_t*)self_Proc->code;
  printf("\nProcess Name: %s %x%g\n", self_Proc->name, self_Proc->pgdir, 15);

  switch_directory(self_Proc->pgdir);
  Elf_LoadCode(bfn);
 // Elf_ShowInfo(bfn);
  //asm volatile("int $50");
  switch_directory(system_dir);
  self_Proc->privileges = 0x3;
  bfn->mainthread = Elf_CreateThread_fl(bfn);
  bfn->mainthread->RtFlags |= THREAD_RT_FLG_ShellSleep;
  

// task_t* ct = (task_t*)(Get_Scheduler()->current_task);
// Process_t* proc = ct->process;
// printf("{%d}", self_Proc->first_task_entry);

  Elf_DeployThreads(bfn);
  switch_directory(self_Proc->pgdir);
  asm volatile("int $50");
  //printf("\tCore: %d\n", bfn->mainthread->Scheduler->identity);
  //switch_directory(self_Proc->pgdir);
 // hexdump_8(bfn->mainthread->func, 10);
  //asm volatile("int $50");

  //Task_Swap(bfn->mainthread, bfn->launcherThread);

  // If it reaches here again, destroy the process
  kill();
}

uint32_t Elf_ProcLauncher_file(const char* str, int core)
{
  BinFile_t* bfn = Elf_LoadFile(str);
  if(!bfn)
  {
    return 1;
  }
  bfn->sh_frame = Shell_InitArgTable(bfn->name);
  Process_t* proc = Elf_CreateProc_fl(bfn);

  Shell_Softsleep();
  task_t* elfLaunch = create_task("ElfLauncher", Elf_LaunchThread, 2, 0x202, proc);
  bfn->launcherThread = elfLaunch;

  if(core == -1)
    Activate_task_direct(elfLaunch);
  else 
    Activate_task_direct_SP(elfLaunch, &KitList[core]);
  return 0;
  //TODO: Get The Process Name;
  //TODO: Create a New Page Directory for the Process; Fix Page Fault problems
  //TODO: Go to the address specified by program header. Then Place .text, .data etc sections
  //      Accordingly with offset into the memory.
}

uint32_t Elf_ProcLauncher(BinFile_t* bfn, int core)
{
  bfn->sh_frame = Shell_InitArgTable(bfn->name);
  Process_t* proc = Elf_CreateProc_fl(bfn);

  Shell_Softsleep();
 // printf("\n\t{%d %d}", proc->first_task_entry, proc->total_tasks);
  task_t* elfLaunch = create_task("ElfLauncher", Elf_LaunchThread, 2, 0x202, proc);
 // printf("\n\t{%d}", proc->first_task_entry);
  bfn->launcherThread = elfLaunch;

  if(core == -1)
    Activate_task_direct(elfLaunch);
  else 
    Activate_task_direct_SP(elfLaunch, &KitList[core]);
  return 0;
  //TODO: Get The Process Name;
  //TODO: Create a New Page Directory for the Process; Fix Page Fault problems
  //TODO: Go to the address specified by program header. Then Place .text, .data etc sections
  //      Accordingly with offset into the memory.
}

static inline Elf32_Shdr_t *elf_sheader(Elf32_Ehdr_t *hdr)
{
	return (Elf32_Shdr_t *)((int)hdr + hdr->e_shoff);
}

static inline Elf32_Shdr_t *elf_section(Elf32_Ehdr_t *hdr, int idx)
{
	return &elf_sheader(hdr)[idx];
}