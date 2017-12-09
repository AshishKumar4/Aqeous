#include "LibSymTable.h"
#include "LIBRARY.h"
#include "Processing/SysCalls/SysCalls.h"
#include "IO_Handling/IO_Devices/Keyboard/kb_handle.h"
#include "Processing/SysCalls/SyscallVects.h"


void init_libsym()
{
  LIBSYMTABLE = pmem_4k(1);
  LIBSYMTABLE[__SYS_STDO__] = (uint32_t)&_sys_stdout;
  LIBSYMTABLE[__SYS_MALLOC__] = (uint32_t)&_sys_malloc;
  LIBSYMTABLE[__SYS_FREE__] = (uint32_t)&_sys_free;
  LIBSYMTABLE[__SYS_EXIT__] = (uint32_t)&_sys_kill;
  LIBSYMTABLE[__SYS_STDI__] = (uint32_t)&_sys_stdin;
  LIBSYMTABLE[__SYS_FORK__] = (uint32_t)&_sys_fork;
  
  LIBSYMTABLE[__SYS_AQ_FOPEN__] = (uint32_t)&_sys_openFile;
  LIBSYMTABLE[__SYS_AQ_FCLOSE__] = (uint32_t)&_sys_closeFile;
  LIBSYMTABLE[__SYS_AQ_FSEEK__] = (uint32_t)&_sys_fileSeek;
  LIBSYMTABLE[__SYS_AQ_FTELL__] = (uint32_t)&_sys_fileTell;
  LIBSYMTABLE[__SYS_AQ_FREAD__] = (uint32_t)&_sys_readFile;
  LIBSYMTABLE[__SYS_AQ_FWRITE__] = (uint32_t)&_sys_writeFile;
}

void LibSym_MapPdir(PageDirectory_t* pdir)
{
  Map_non_identity(LIBSYMTABLE, 0xC0001000, 4096, pdir);
}
