#include "LibSymTable.h"
#include "LIBRARY.h"
#include "Processing/SysCalls/SysCalls.h"

void kill_tmp()
{
  //asm volatile("sti");
  printf("\nKilling the thread...");
  //switch_directory(system_dir);
  kill();
  while(1);
}

void _kmalloc()
{
  uint32_t tmp = _syscall_pop();
  tmp = kmalloc(tmp);
  _syscall_push(tmp);
}

void init_libsym()
{
  LIBSYMTABLE = pmem_4k(1);
  LIBSYMTABLE[0] = &printf;
  LIBSYMTABLE[1] = _kmalloc;
  LIBSYMTABLE[2] = kfree;
  LIBSYMTABLE[3] = &memcpy;
  LIBSYMTABLE[4] = &memset_fast;
  LIBSYMTABLE[5] = &kill;
}

void LibSym_MapPdir(PageDirectory_t* pdir)
{
  Map_non_identity(LIBSYMTABLE, 0xC0001000, 4096, pdir);
  printf("\n[%x]", LIBSYMTABLE);
  printf("\n[%x]", kill_tmp);
}
