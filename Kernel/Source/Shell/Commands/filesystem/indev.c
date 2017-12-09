
/****************************** New Commands ported from JS-OS ******************************/

/*
  PORT TABLE: 
    QsetCurrentDir -> vfs_SetCurrDir
    Q__open__  -> vfs_Open
    Qf_finddir ->  vfs_findDir
    Qf_finddir_close -> vfs_findDir_Close
    ptr_currentDir  ->VFS_ptr_currentDir
*/

int Command_cd(char *arguements)
{
  int argLen = strlen(arguements);
  int i = 0, originalI = 0;

  int isInitDirRoot = FALSE;
  void *initDir = VFS_ptr_currentDir;

  char *cd;

  FILE *dir;

  do
  {
    originalI = i;

    /*Parcing the arguments*/
    for(i; i < argLen; i++)
      if(*(arguements + i) == '/')
      {
        if(i) //if the first character is "/" it is ok to not break and continue
          break;
        else{
          isInitDirRoot = TRUE;
          i++;
          break;
        }
      }

    /*(i - originalI) is the length of the portion after the portion
     * we just processed. IE: arguments = "./poop", when we process
     * the "." i == 2 and originalI == 2. Continuing on, for "poop",
     * i will equal 6 and originalI equals 2. 6 - 2 is 4, which is
     * how long poop is */
    cd = (char*)kmalloc((i - originalI) + 1); //+1 being the \000 at the end of a string

    memcpy(cd, (arguements + originalI), (i - originalI));
    *(cd + (i - originalI)) = 0; //adds the \000 at the end

    if(i == 1 && isInitDirRoot == TRUE)
    {
      //the directory is "/" so we are starting in the root instead of currentDir
      dir = vfs_Open(fs_root, "/", 0, FALSE);
      isInitDirRoot = FALSE;
    }else{
      dir = vfs_findDir(VFS_ptr_currentDir, cd);
      i++;
    }

    if(dir && dir->node_type == TYPE_DIRECTORY) //if dir exists and is a directory
    {

      vfs_SetCurrDir(dir->node);
      kfree(cd);

    }else{
      /*if the string cd has contents, usually if the char *arguements is "//"
       * for some reason, cd will have no content (begins with a \000)*/
      if(*cd)
        k_printf("In \"%s\", \"%s\" is not a directory\n", arguements, cd);

      kfree(cd);

      /*if we have an initial (starting point) dir, set the current dir
       * to that dir as it was before the user typed an incorrect path name */
      vfs_SetCurrDir(initDir);

      //close our file desc opened with vfs_findDir
      vfs_findDir_Close(dir);

      //failure!
      return 1;
    }
  }
  while(i < argLen);

  //close our file desc opened with vfs_findDir
  vfs_findDir_Close(dir);

  //success!
  return 0;

}

u32int program_cp(char *arguments)
{
  u32int error = FALSE;
  void *initDir = ptr_currentDir;

  u32int nArgs = countArgs(arguments);

  switch(nArgs)
  {
  case 0: //if the user did not input any args
    k_printf("cp: missing file operand\n");
    return 1; //error
  case 1: //we must have an input and an output file
    k_printf("cp: missing destination file operand after `%s'\n", arguments);
    return 1; //error
  }

  char *args[nArgs];
  getArgs(arguments, args);

  u32int dirCount, fileCount;

  dirFilePathCount(args[0], &dirCount, &fileCount);

  char *dirPath, *filePath;

  dirPath = (char*)kmalloc(dirCount + 1);
  filePath = (char*)kmalloc(fileCount + 1);

  cdFormatArgs(args[0], dirPath, filePath);

  FILE *src;

  //save the directory inode of the source file, may come in use later
  void *old_currentDir = ptr_currentDir;

  src = f_finddir(ptr_currentDir, filePath); //gets the structure of the first argument input

  if(src) //if there is a file with the name of the first argument
  {
    setCurrentDir(initDir); //sets the current dir back to the original

    s32int i = 0, length = strlen(args[1]), count = -1;

    for(i; i < length; i++)
      /* using the following, after it is done executing, the integer
       * count will be equeal to the number of character before and
       * including the last "/" in the destination string of input */
      if(*(args[1] + i) == '/') //for every "/" in the dest of input, increment count to i
        count = i;

    char *destPath = (char*)kmalloc(count + 2); //the extra +1 is for the \000 and the other 1 is because i, and thus count start from 0 on the first element
    memcpy(destPath, args[1], count + 1);
    *(destPath + count + 1) = 0; //add \000 to the end

    char *restPath = (char*)kmalloc(length - count);
    memcpy(restPath, args[1] + count + 1, length - count - 1);
    *(restPath + length - count - 1) = 0; //add the \000 to the end

    //By default, we have not cd'd yet, so u32int work should be set to fail (1)
    u32int work = 1;

    //if there is something in the destPath
    if(*(destPath))
      work = program_cd(destPath);

    u32int b;

    FILE *restSrc;
    restSrc = f_finddir(ptr_currentDir, restPath);

    /*if the rest of the user text after the final "/" is a directory,
     * go to that directory and then create the file */
    if(restSrc && restSrc->node_type == TYPE_DIRECTORY)
    {
      //cd to the directory to place the new file
      program_cd(restPath);

      //create the new file empty file or delete an existing one
      FILE *copiedFile;
      copiedFile = f_open(src->name, ptr_currentDir, "wd");

      //open the file to copy from
      FILE *orig = f_open(src->name, old_currentDir, "r");

      //copy the contents into a buffer
      u8int *data;
      data = (u8int*)kmalloc(orig->size);
      f_read(orig, 0, orig->size, data);

      //write the data
      f_write(copiedFile, 0, orig->size, data);

      //close the files
      f_close(copiedFile);
      f_close(orig);
      
      //free allocations
      kfree(data);

    }else if(*restPath) //if there are contents after the last "/", make the file with that name
    {
      FILE *copiedFile;
      copiedFile = f_open(restPath, ptr_currentDir, "wd");

      //open the files to copy to and the new one
      FILE *orig = f_open(src->name, old_currentDir, "r");

      //copy the contents into a buffer
      u8int *data;
      data = (u8int*)kmalloc(orig->size);
      f_read(orig, 0, orig->size, data);

      //write the data
      f_write(copiedFile, 0, orig->size, data);

      //close the files
      f_close(copiedFile);
      f_close(orig);
      
      //free allocations
      kfree(data);

    }else if(!work) //if the cd function above did not fail, i.e., we should still copy the file, but with its original name
    {
      FILE *copiedFile;
      copiedFile = f_open(src->name, ptr_currentDir, "wd");

      //open the files to copy to and the new one
      FILE *orig = f_open(src->name, old_currentDir, "r");

      //copy the contents into a buffer
      u8int *data;
      data = (u8int*)kmalloc(orig->size);
      f_read(orig, 0, orig->size, data);

      //write the data
      f_write(copiedFile, 0, orig->size, data);

      //close the files
      f_close(copiedFile);
      f_close(orig);
      
      //free allocations
      kfree(data);

    }
   
    //sets the current dir back to the original
    setCurrentDir(initDir);

    //free the allocations
    kfree(destPath);
    kfree(restPath);

    //close restSrc that was oppened by f_finddir
    f_finddir_close(restSrc);
  }else{
    //sets the current dir back to the original    
    setCurrentDir(initDir);
    error = TRUE;
    k_printf("%s: No such file or directory\n", args[0]);
  }

  u32int i;
  for(i = 0; i < nArgs; i++)
    kfree(args[i]);

  kfree(dirPath);
  kfree(filePath);

  //close src that was oppened by f_finddir
  f_finddir_close(src);

  //return either true or false, depending on what u32int error is set to
  return error;
}

void program_mv(char *arguements)
{
  u32int nArgs = countArgs(arguements);

  char *args[nArgs];
  getArgs(arguements, args);

  if(program_cp(arguements) == TRUE) //copy the source to the dest
    return; //if the cp function returned TRUE, something went wrong

  program_rm(args[0]);

  //sucess!
  return;
}

static void *find_init_dir;

void find_set_current_dir()
{
  find_init_dir = ptr_currentDir;
}

void find_print_format(char *dirPath, struct generic_dirent *node)
{
  /*if the current dir is the initial dir set when find was first called,
   * then just print the node->name*/
  if(find_init_dir == ptr_currentDir)
  {
    /*if there is no contents in dirPath, that means
     * the dirPath is the current directory, so print a "./" before
     * the actual node->name*/
    if(!*dirPath)
      k_printf("./%s\n", node->name);
    else //print the dirPath and then the node->name of the file
      k_printf("%s%s\n", dirPath, node->name);
  }else{
    /*copy the location of the node name to another place, becuase the
     * structure (struct) dirent is used and is overwritting in the finddir_fs
     * function, copying the address will keep it still accessible*/
    char *node_name;
    node_name = node->name;
    
    char *place;
    u32int size = 0, nfdescs = 15, i = 0;

    //TODO store the file descs in an array so we do not need to look them up in the second loop

    FILE *parent_dir, **fdesc_array;

    fdesc_array = (FILE**)kmalloc(sizeof(FILE*) * nfdescs);

    //set parent_dir to the current directory
    parent_dir = f_finddir(ptr_currentDir, 0);

    do
    {
      //if we have no space left in out fdesc_array, reallocate more space
      if(i == nfdescs)
        krealloc((u32int*)fdesc_array, sizeof(FILE*) * nfdescs, sizeof(FILE*) * (nfdescs *= 2));

      /*increment the string size by the size of the name, +1 is for the '/'
       * following the directory name*/
      size += strlen(parent_dir->name) + 1;

      //add this parent_dir to our array
      *(fdesc_array + i) = parent_dir;

      //increment i
      i++;

    }
    while((parent_dir = f_finddir(parent_dir, ".."))->node != find_init_dir);

    place = (char*)kmalloc(size + 1); //kmalloc +1 for the \000

    //reset parent_dir to the current directory, the first element in out array
    parent_dir = *fdesc_array;

    //reset i back to 0
    i = 0;

    u32int offset = size;
    u32int name_len;

    do
    {
      //get the name length of the directory
      name_len = strlen(parent_dir->name);
      
      //decrease offset by the size of the name, +1 for the '/' followind any directory
      offset -= (name_len + 1);

      //copy the name over to place
      memcpy(place + offset, parent_dir->name, name_len);

      //add the "/" to the end of the directory
      *(place + offset + name_len) = '/';
    }
    while((parent_dir = *(fdesc_array + (++i)))->node != find_init_dir);

    *(place + size) = 0; //add the \000 to place

    k_printf("./%s%s\n", place, node_name);

    //free the files opened by the f_finddir
    for(i = 0; *(fdesc_array + i); i++)
      f_finddir_close(*(fdesc_array + i));

    kfree(fdesc_array);

  }
    
  
}

void program_find(char *arguments)
{
  //save the initial dir inode
  void *initDir = ptr_currentDir;

  //get the name sizes of the dir and file portions of the char *arguments
  u32int dirCount, fileCount;

  dirFilePathCount(arguments, &dirCount, &fileCount);

  //save the dir and file names from char *arguments
  char *dirPath, *filePath;

  dirPath = (char*)kmalloc(dirCount + 1);
  filePath = (char*)kmalloc(fileCount + 1);

  /*set the first characters to 0, just in case that cdFormatArgs gives error
   * (likely) since if the search arg has a '*' to symbolize any character, then
   * that file with the name e.g., "*name", itself will not exist, if cdFormatArgs
   * returns an error, then it has not touched dirPath nor filePath, having the initial
   * 0 prevents any junk following it to not appear*/
  *(dirPath) = 0;
  *(filePath) = 0;

  //cd to the dirpath locations
  u32int work = cdFormatArgs(arguments, dirPath, filePath);

  s32int i;
  u32int argLen = strlen(arguments);
  for(i = argLen - 1; i >= strlen(arguments) - fileCount; i--)
    //there is an asterisc, so cd would have not found the file, but still let it pass
    if(*(arguments + i) == '*')
    {
      work = 0; //correct error
      /*becuase the arguements file has a "*", in it, most of the time,
       * our cdFormatArgs() function will return 1 (error) since for
       * the most part, there will be no file named (ex: if there is a file
       * name "test.txt" and the user will search "t*", cd will literally
       * search for "t*", not find it and complain, thus the char *filePath
       * will be empty and nothing will be saved to it, we just copy the contents
       * of our file name in the arguments to filePath manually */
      memcpy(filePath, arguments + argLen - fileCount, fileCount);

      *(filePath + fileCount) = 0; //add the \000 at the end
      break;
    }

  if(!work) //there was no error
  {
    void *fsnode;

    fsnode = ptr_currentDir;
    struct generic_dirent *node = 0;
    i = 0;

    //go through all of the files in the fsnode directory and compare them
    while(node = f_readdir(fsnode, i))
    {
      //if the files names are the same (includeing using "*" to represent and characters)
      if(!compareFileName(filePath, node->name))
      {
        /*if there is no contents in dirPath, that means
         * the dirPath is the current directory, so print a "./" before
         * the actual node->name*/
        find_print_format(dirPath, node);
      }

      /*if the node is a directory, recurse through its contents also
       * it also makes sure that the supposed directory is not the "." or ".."
       * directories, if that was not acounted for, this function would loop forever */

      //TODO make recursive find store inodes so it does not repeat inode searches, avoids hard link problems
      //TODO remove this "brute force" meathod and apply the technique above
      if(node->file_type == TYPE_DIRECTORY &&
         strcmp(node->name, ".") &&
         strcmp(node->name, ".."))
      {
        char *recurse_str;
        
        u32int node_name_len = strlen(node->name);
        /*kmalloc memory 2 for the "./", node->name, + 1 for "/" to separate
         * the directory to recurce and the search string (filePath), and
         * filePath, + 1 (\000), to be concatenated */
        recurse_str = (char*)kmalloc(2 + node_name_len + 1 + fileCount + 1);

        //copy the path to the beginning
        memcpy(recurse_str, "./", 2);

        //copy the node->name (the directory we will search)
        memcpy(recurse_str + 2, node->name, node_name_len);

        //copy the "/" between the dir to recurse and the search string (filePath)
        memcpy(recurse_str + 2 + node_name_len, "/", 1);
        
        //copy the filePath search string that was passed in the char *arguments
        memcpy(recurse_str + 2 + node_name_len + 1, filePath, fileCount);

        *(recurse_str + 2 + node_name_len + 1 + fileCount) = 0; //add the \000 at the end

        //send recurse to find
        program_find(recurse_str);
      }
      
      i++;
    }

  }else{
    k_printf("%s: not a proper file path\n", arguments);
  }

  setCurrentDir(initDir);

  kfree(dirPath);
  kfree(filePath);
}

void program_cat(char *arguments)
{
  void *initDir = ptr_currentDir;

  s32int dirCount, fileCount;

  dirFilePathCount(arguments, &dirCount, &fileCount);

  char *dirPath, *filePath;

  dirPath = (char*)kmalloc(dirCount + 1);
  filePath = (char*)kmalloc(fileCount + 1);

  cdFormatArgs(arguments, dirPath, filePath);

  FILE *file;

  file = f_finddir(ptr_currentDir, filePath);

  if(file && file->node_type == TYPE_FILE) //if the file exists and is a file
  {

    //open the file
    FILE *f_file = f_open(file->name, ptr_currentDir, "r");

    //error in opening of file, exit the if loop
    if(!f_file)
    {
      k_printf("cat: cannot open file: %s", file->name);

      setCurrentDir(initDir);

      kfree(dirPath);
      kfree(filePath);

      return;      
    }

    /*the purpose of the following section is to one address with a max
     * size of 1KB (to conserve space) to display any size file
     * We do this by dividing the file into blocks and printing the
     * file's data one block at a time instead of all at once */
    u32int j, a, next_size, kmalloc_size = blockSizeAtIndex(file->size, 0, 0);
    u32int fs_blk_sz = block_size_of_node(file);
    char *buf = (char*)kmalloc(kmalloc_size);

    /* ((u32int)(root_nodes[i].length / fs_blk_sz) + 1) calculates the
     * number of blocks this file will take up at a given length */
    for(a = 0; a < ((u32int)(file->size / fs_blk_sz) + 1); a++)
    {
      //assign the content of file to char *buf
      u32int sz = f_read(file, a * fs_blk_sz, kmalloc_size, buf);

      for (j = 0; j < sz; j++)
        k_putChar(buf[j]);

      //if there is a block after this one (we are not at the last block)
      if(a != (u32int)(file->size / fs_blk_sz))
      {

        //calculate the size to kmalloc for the next block
        next_size = blockSizeAtIndex(file->size, a + 1, 0);

        /*if our current kmalloc'd space is not the same as the next block's,
         * then we have to free the current buf and kmalloc it with the
         * new size, else we do not need to touch as it is the same size */
        if(kmalloc_size != next_size)
        {
          kfree(buf);
          kmalloc_size = next_size;
          buf = (char*)kmalloc(kmalloc_size);
        }
      }else{
        //free the buffer
        kfree(buf);
        f_close(f_file);
      }
    }

  }else if(file->node_type == TYPE_DIRECTORY)
    k_printf("%s: Is a directory\n", arguments);
  else
    k_printf("%s: No such file\n", arguments);

  setCurrentDir(initDir);

  kfree(dirPath);
  kfree(filePath);

  //free the file opened by f_finddir
  f_finddir_close(file);

  return;
}

void program_rm(char *arguments)
{
  void *initialDir = ptr_currentDir;
  u32int length = strlen(arguments);
  s32int a;

  //goes from the back, when it breaks, a == the position of the very last "/"
  for(a = length - 1; a >= 0; a--)
    if(*(arguments + a) == '/')
      break;

  if(a < 0) //if the char *arguments had no "/"
  {
    FILE *isDir;

    isDir = f_finddir(ptr_currentDir, arguments);

    if(!isDir) //if there is no such file
    {
      k_printf("%s: No such file\n", arguments);

      //free the isDir
      f_finddir_close(isDir);

      return;
    }else if(isDir && isDir->node_type == TYPE_DIRECTORY) //if the input is just a directory, return error
    {
      k_printf("cannot remove '%s': Is a directory\n", arguments);

      //free the isDir
      f_finddir_close(isDir);

      return;
    }else if(isDir && isDir->node_type == TYPE_FILE) 
    {
      //if the input is just an existing filename (which is perfectly legal)
      a = -1; //set s32int a to be just right for those kmallocs below

      //free the isDir
      f_finddir_close(isDir);
    }
  }else if(a == length - 1) //if char *arguments ends in "/", therefore no file has been specified
  {
    k_printf("%s: No such file\n", arguments);
    return;
  }

  char *dirPath, *filePath;

  dirPath = (char*)kmalloc(a + 2);
  filePath = (char*)kmalloc(length - a);

  cdFormatArgs(arguments, dirPath, filePath);

  FILE *file;

  file = f_finddir(ptr_currentDir, filePath);

  if(file && file->node_type == TYPE_FILE) //if the file exists and is a file
    f_remove(ptr_currentDir, file);
  else if(file && file->node_type == TYPE_DIRECTORY)
    k_printf("Cannot remove '%s': Is a directory\n", filePath);
  else
    k_printf("%s: No such file\n", arguments);

  //close the file that was opened by f_finddir
  f_finddir_close(file);

  kfree(dirPath);
  kfree(filePath);

  setCurrentDir(initialDir);

  //sucess!
  return;
}

void program_mkdir(char *arguments)
{
  void *initDir = ptr_currentDir;
  u32int length = strlen(arguments);

  u32int i;

  /*s32int count would equal the number of characters before and
   * including the last '/' in the char *arguments, if it equals
   * -1 then that means there were no '/' in the char *arguments */
  s32int count = -1;
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

    u32int work = program_cd(dirString);

    if(!work) //if the program_cd() did not return an error
    {
      char *restString;
      restString = (char*)kmalloc(length - count);

      //get the contents of char *arguments after the final '/'
      memcpy(restString, arguments + count + 1, length - count - 1);
      *(restString + length - count - 1) = 0; //add the \000 at the end

      void *dir;

      dir = f_make_dir(ptr_currentDir, restString);

      setCurrentDir(initDir);

      kfree(restString);
    }

    kfree(dirString);
  }else{
    void *dir;

    dir = f_make_dir(ptr_currentDir, arguments);
  }
  
  return;
}
