#include "SysCalls.h"
#include "Processing/LibSymTable/LibSymTable.h"
#include "library.h"
#include "Processing/SysCalls/SysCalls.h"
#include "IO_Handling/IO_Devices/Keyboard/kb_handle.h"
#include "SyscallVects.h"

void _sys_kill(uint32_t* syscall_stack)
{
  int code = _syscall_pop(syscall_stack);
  printf("\nExit code %d", code);
  //asm volatile("sti");
  task_t* ct = (task_t*)(Get_Scheduler()->current_task);
  Process_t* proc = ct->process;
  BinFile_t* bfn = (BinFile_t*)proc->code;
  if(proc->total_tasks > 1)
  {
    --(proc->total_tasks);
    kill();
   // printf("\n{44444444444444444444444444444444444444444444444444444444444444444}");
  }
  else
  {
    printf("{%d; %d}", proc->total_tasks, proc);
   // Delete_Process(proc);
    //kill();
   // _kill(ct);
    //asm volatile("int $50");
    threadRipper_push(proc);
  }
  _syscall_push(syscall_stack, 0);
  return;
  Shell_Dbuff_sync();
  while(1) asm volatile("int $50");
}

void _sys_malloc(uint32_t* syscall_stack)
{
  uint32_t tmp = _syscall_pop(syscall_stack);
 // printf("<<%d ,", tmp);
  tmp = (uint32_t)kmalloc(tmp);//phy_alloc4K();
 // printf("%x>>", tmp);
  //Shell_Dbuff_sync();
 // while(1);
  _syscall_push(syscall_stack, tmp);
}

void _sys_free(uint32_t* syscall_stack)
{

}

void _sys_stdout(uint32_t* syscall_stack)
{
  uint32_t sz = _syscall_pop(syscall_stack);
  uint32_t ptr = _syscall_pop(syscall_stack);
  // GET physical address;
  PageDirectory_t* dir = (PageDirectory_t*)(Get_Scheduler()->current_pdir);
  uint32_t* addr = get_phyAddr(ptr, dir);

  printf("%s", (char*)addr);
 // Shell_Dbuff_sync();
 // asm volatile("int $50");
  //print(addr, sz-1);
  char* str = (char*)addr;
 // printf("\nsz: %d, addr: %x, %x, %x, %x; %s", sz, ptr, addr, dir, (char*)addr);
  memset(addr, 0, sz);
  _syscall_push(syscall_stack, sz);
}

void __attribute__((optimize("O2"))) _sys_stdin(uint32_t* syscall_stack)
{

  uint32_t type = _syscall_pop(syscall_stack);
  uint32_t ptr = _syscall_pop(syscall_stack);

  PageDirectory_t* dir = (PageDirectory_t*)(Get_Scheduler()->current_pdir);
  uint32_t* addr = get_phyAddr(ptr, dir);
  char* str = (char*)addr;
  char* tstr = &(str[addr[0]]);
  kb_stdin_syscall(tstr, -1);
  /*printf("\n{{%s}}", tstr);
  addr[1] = strlen(tstr);*/
  _syscall_push(syscall_stack, 0);
}

void _sys_fork(uint32_t* syscall_stack)
{
    _syscall_push(syscall_stack, 0);
}

void _sys_openFile(uint32_t* syscall_stack)
{
  //switch_directory(Get_Scheduler()->current_pdir);
  PageDirectory_t* dir = (PageDirectory_t*)(Get_Scheduler()->current_pdir);
 // printf("{%x}", Get_Scheduler()->current_pdir);
  const char* mode = (const char*)_syscall_pop(syscall_stack);
  mode = (const char*)get_phyAddr((uint32_t)mode, dir);
  const char* fname = (const char*)_syscall_pop(syscall_stack);
  fname = (const char*)get_phyAddr((uint32_t)fname, dir);

  printf("{mode: %s}, {name: %s}", mode, fname);
  FILE* f = fopen(fname, mode);
 // printf("{%x}", Get_Scheduler()->current_pdir);
  _syscall_push(syscall_stack, (uint32_t)f);
}

void _sys_closeFile(uint32_t* syscall_stack)
{
  uintptr_t stream = (uintptr_t)_syscall_pop(syscall_stack);
//  fclose((FILE*)stream);
  _syscall_push(syscall_stack, 0);
}

void _sys_fileSeek(uint32_t* syscall_stack)
{
  int whence = (int)_syscall_pop(syscall_stack);
  int off = (int)_syscall_pop(syscall_stack);
  uintptr_t stream = (uintptr_t)_syscall_pop(syscall_stack);

  _syscall_push(syscall_stack, (uint32_t)fseek((FILE*)stream, off, whence));
}

void _sys_fileTell(uint32_t* syscall_stack)
{
  _syscall_push(syscall_stack, (uint32_t)ftell((FILE*)_syscall_pop(syscall_stack)));
}

void _sys_readFile(uint32_t* syscall_stack)
{
  uintptr_t stream = (uintptr_t)_syscall_pop(syscall_stack);
  size_t nmemb = (size_t)_syscall_pop(syscall_stack);
  size_t size = (size_t)_syscall_pop(syscall_stack);
  uintptr_t ptr = (uintptr_t)_syscall_pop(syscall_stack);
  PageDirectory_t* dir = (PageDirectory_t*)(Get_Scheduler()->current_pdir);
  ptr = (uintptr_t)get_phyAddr((uint32_t)ptr, dir);
  if(!nmemb)
  {
    nmemb = 1;
    size = fsize((FILE*)stream);
  }
  uint32_t ttll = (uint32_t)fread((void*)ptr, size, nmemb, (FILE*)stream);
  _syscall_push(syscall_stack, ttll);
//  printf("[%s; %d]", ptr, ttll);
}

void _sys_writeFile(uint32_t* syscall_stack)
{
  uintptr_t stream = (uintptr_t)_syscall_pop(syscall_stack);
  size_t nmemb = (size_t)_syscall_pop(syscall_stack);
  size_t size = (size_t)_syscall_pop(syscall_stack);
  uintptr_t ptr = (uintptr_t)_syscall_pop(syscall_stack);
  PageDirectory_t* dir = (PageDirectory_t*)(Get_Scheduler()->current_pdir);
 // printf("[[%x, %d, %d]]", ptr, nmemb, size);
  ptr = (uintptr_t)get_phyAddr((uint32_t)ptr, dir);
  if(!nmemb)
  {
    nmemb = 1;
    //size = fsize((FILE*)stream);
  }
  if(!size)
  {
    size = strlen(ptr);
    //size = fsize((FILE*)stream);
  }
 // printf("<write: %d>; [%d], %d, %d; %x", ptr, ptr, nmemb, size, dir);
 printf("\n<write: %s>", ptr);
  _syscall_push(syscall_stack, (uint32_t)fwrite((void*)ptr, size, nmemb, (FILE*)stream));
 //_syscall_push(syscall_stack, size);
 /* Shell_Dbuff_sync();
  asm volatile("hlt");*/
}