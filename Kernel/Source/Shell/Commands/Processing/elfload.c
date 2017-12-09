#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "shell.h"
#include "Processing/ELF/Elf.h"

void Command_elfload()
{
  char* str = CSI_Read(1);
  int core = -1;
  char* tmp = (char*)CSI_Read(2);
  if(tmp)
    core = StrToInt(tmp);
  printf("\nTrying to load in core: %d", core);
  if(Elf_ProcLauncher_file(str, core))
  {
    printf("\nFile does not exist/Not compatible");
  }
 // Shell_sleep();
}
