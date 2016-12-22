#include "fs.h"
#include "Shell.h"
#include "common.h"
#include "string.h"
#include "stdio.h"
#include "ext2/ext2_fs.h"

void Command_ls()
{
   asm volatile("cli");
   find_friendDirs(0);
   find_childFiles(0);
}
