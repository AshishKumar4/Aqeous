#include "fs.h"
#include "Shell.h"
#include "common.h"
#include "string.h"
#include "stdio.h"
#include "ext2/ext2_fs.h"
/*
void Command_cd()
{
  asm volatile("cli");
    char* dir = (char*)CSI_Read(1);

//   char* dir = shell_buf + 3;
   if(!strncmp(dir,"..",2))
   {
      set_curr_dir(curr_dir.dir->parent);
   }
   else
   {
      Directory_t* new_dir = search_folderOGP(dir);
      if(new_dir)
        set_curr_dir(new_dir->location);
    //  while(1);
      //  set_curr_dir(curr_dir.dir.location);
      //printf("%s",new_dir->name);
   }
}
*/