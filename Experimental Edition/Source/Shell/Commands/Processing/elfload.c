#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "shell.h"
#include "Processing/ELF/Elf.h"

void Command_elfload()
{
  char* str = CSI_Read(1);
  Elf_ProcLauncher_file(str);
}
