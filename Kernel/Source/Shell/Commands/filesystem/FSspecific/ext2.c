#include "fs.h"
#include "Shell.h"
#include "common.h"
#include "string.h"
#include "stdio.h"
#include "ext2/ext2_fs.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "tasking.h"
#include "kb_handle.h"

void ext2_mkdir(char* arguments)
{
  asm volatile("cli");
//  ext2_inode_t* Ext2_currDir = ext2_root;
  void *initDir = Ext2_currDir;
  uint32_t length = strlen(arguments);

  uint32_t i;

  /*int32_t count would equal the number of characters before and
   * including the last '/' in the char *arguments, if it equals
   * -1 then that means there were no '/' in the char *arguments */
  int32_t count = -1;
  for(i = 0; i < length; i++)
    if(*(arguments + i) == '/')
      count = i;

  if(count != -1) //if count was changed, ie: in the char *arguments there was atleast one "/"
  {
    char *dirString;
    /* +1 being the \000 at the end, and the other +1 is due to
     * that we start counting from 0, 1, 2, etc. so if count was to equal
     * 0, that would mean 1 character, so we boost count up by one to equal
     * the number of characters */
    dirString = (char*)kmalloc(count + 2);

    // +1 for same reason as above, count starts from 0 for the first character
    memcpy(dirString, arguments, count + 1);
    *(dirString + count + 1) = 0; // add the \000 at the end of the string "dirString"

    uint32_t work = 1;//program_cd(dirString);

    if(!work) //if the program_cd() did not return an error
    {
      char *restString;
      restString = (char*)kmalloc(length - count);

      //get the contents of char *arguments after the final '/'
      memcpy(restString, arguments + count + 1, length - count - 1);
      *(restString + length - count - 1) = 0; //add the \000 at the end

      void *dir;

      dir = ext2_create_dir(Ext2_currDir, restString);

    //  setCurrentDir(initDir);

    //  kfree(restString);
    }

  //  kfree(dirString);
  }
  else
  {
    void *dir;
    printf("\nasds");
    dir = ext2_create_dir(Ext2_currDir, arguments);
  }

  return;
}

void ext2_ls()
{
  ext2_inode_t* fnode = Ext2_currDir;
  ext2_dirent_t* entry = 0;

  for(int i = 0; entry = ext2_dirent_from_dir(fnode, i); i++)
  {
    printf("\n%s", entry->name);
  }
}

void ext2_cd(char* arguments)
{
  int argLen = strlen(arguments);
  int i = 0, originalI = 0;

 // int isInitDirRoot = FALSE;
 // void *initDir = Ext2_currDir;
  ext2_inode_t* ndir = ext2_file_from_dir(Ext2_currDir, arguments);
  Ext2_currDir = ndir;
}

void ext2_cp(char* dpath, char* spath, char* nname)
{

}

void ext2_del(char* path)
{

}

void ext2_mkfl(char *path, char* dir_name)
{

}

void ext2_editfl(char* path, uint32_t* data, uint32_t* type, uint32_t off, uint32_t osz)
{

}

void ext2_rfl(char* path, uint32_t off, uint32_t sz)
{

}

void ext2_mv(char* path, char* spath)
{
  
}
