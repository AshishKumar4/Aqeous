/*** AqFS File System, Aqeous OS's default FS ***/

#include "fs.h"
#include "fs_alloc.h"

extern volatile int multitasking_ON;

uint64_t root_location;

void Setup_fs()
{
    uint32_t buf=fsalloc(512);
    read(curr_port,root_location/512,1,(DWORD)buf);
    Directory_t* root=(Directory_t*)(buf+(uint32_t)(root_location%512));
    strcpy(root->name,"root");
    root->perm=0;
    root->files=0;
    root->folders=0;
    root->location=root_location;
    root->Next_Friend=0;
    root->last_child=0;
    root->First_child=0;
    root->First_file=0;
    root->last_file=0;
    //Next_available+=sizeof(Directory_t);
    current_=*root;
    curr_dir.dir=*root;
    curr_dir.type=1;
    strcpy(curr_dir.full_name,"root");
    write(curr_port,root_location/512,1,(DWORD)buf);
    ////kfree((uint32_t*)buf);
}

void create_directory(char *name, uint16_t perm, char* destination)
{
    Directory_t* dest;
    if(destination)
    {
      dest=(Directory_t*)search_folder(destination);
      if(!dest)
      {
        printf("\nDestination Folder %s Not found",name);
        return;
      }
    }
    else
    {
      dest=(Directory_t*)fsalloc(sizeof(Directory_t));
      *dest=curr_dir.dir;
    }
    uint64_t parent=dest->location;
    uint32_t buff=fsalloc(512);
    uint64_t Next_available=sec_alloc(1);
    read(curr_port,Next_available/512,1,(DWORD)buff);
    Directory_t* dir=(Directory_t*)((buff)+(uint32_t)(Next_available%512));
    dir->files=0;
    dir->location=Next_available;
    dir->folders=0;
    strcpy(dir->name,name);
    dir->parent=parent;
    dir->perm=perm;
    dir->Next_Friend=0;
    dir->last_child=0;
    dir->First_child=0;//*/
    dir->last_file=0;
    dir->First_file=0;
    write(curr_port,Next_available/512,1,(DWORD)buff);
    uint32_t buff2=fsalloc(512);
    read(curr_port,parent/512,1,(DWORD)buff2);
    Directory_t* Parent=(Directory_t*)(uint32_t)(buff2+(uint32_t)(parent%512));
    uint64_t tm;
    //printf("\n%s %x",dir->name,dir->location);
    if(Parent->last_child)
    {
        tm=Parent->last_child;
        Parent->last_child=dir->location;
        Parent->folders++;
        write(curr_port,parent/512,1,(DWORD)buff2);
        uint32_t buf=fsalloc(512);
        read(curr_port,tm/512,1,(DWORD)buf);
        Directory_t* dir2=(Directory_t*)(buf+(uint32_t)(tm%512));
        dir2->Next_Friend=dir->location;
        write(curr_port,tm/512,1,(DWORD)buf);
        ////kfree((uint32_t*)buf);
    }
    else
    {
      Parent->First_child=dir->location;
      Parent->last_child=dir->location;
      Parent->folders++;
      write(curr_port,parent/512,1,(DWORD)buff2);
    }

    printf("\nFolder \"%s\" Created Successfully\n",name);
    name=0;
    destination=0;
    ////kfree((uint32_t*)dest);
    ////kfree((uint32_t*)buff);
    ////kfree((uint32_t*)buff2);
}

void create_file(char *name,uint16_t perm, char* destination)
{
    Directory_t* dest;
    if(destination)
    {
      dest=search_folder(destination);
      if(!dest)
      {
        printf("\nDestination Folder %s Not found",name);
        return;
      }
    }
    else
    {
      dest=(Directory_t*)fsalloc(sizeof(Directory_t));
      *dest=curr_dir.dir;
    }
    uint64_t parent=dest->location;
    uint32_t buff=fsalloc(512);
    memset((void*)buff,0,512);
    uint64_t Next_available=sec_alloc(1);
    read(curr_port,Next_available/512,1,(DWORD)buff);
    File_t* file=(File_t*)(buff+(uint32_t)(Next_available%512));
    file->location=Next_available;
    Next_available+=sizeof(File_t);
    file->perm=perm;
    strcpy(file->name,name);
    file->parent=parent;
    file->sz=0;
    file->Next_Friend=0;
    file->first_header=0;
    file->last_header=0;
    file->headers=0;
    write(curr_port,file->location/512,1,(DWORD)buff);

    uint32_t buff2=fsalloc(512);
    read(curr_port,parent/512,1,(DWORD)buff2);
    Directory_t* Parent=(Directory_t*)(uint32_t)(buff2+(uint32_t)(parent%512));
    uint64_t tm;
    //printf("\n%s %x",dir->name,dir->location);
    if(Parent->last_file)
    {
        tm=Parent->last_file;
        Parent->last_file=file->location;
        Parent->files++;
        write(curr_port,parent/512,1,(DWORD)buff2);
        uint32_t buf=fsalloc(512);
        read(curr_port,tm/512,1,(DWORD)buf);
        File_t* file2=(File_t*)(buf+(uint32_t)(tm%512));
        file2->Next_Friend=file->location;
        write(curr_port,tm/512,1,(DWORD)buf);
        ////kfree((uint32_t*)buf);
    }
    else
    {
      Parent->First_file=file->location;
      Parent->last_file=file->location;
      Parent->files++;
      write(curr_port,parent/512,1,(DWORD)buff2);
    }

    printf("\nFile \"%s\" Created Successfully\n",name);
    name=0;
    destination=0;
    ////kfree((uint32_t*)dest);
    ////kfree((uint32_t*)buff);
    ////kfree((uint32_t*)buff2);
}

void find_friendDirs(char* name)
{
    Directory_t* dest;
    if(name)
    {
      dest=search_folder(name);
      if(!dest)
      {
        printf("\nSearched Folder %s Not found",name);
        return;
      }
    }
    else
    {
      dest=(Directory_t*)fsalloc(sizeof(Directory_t));
      *dest=curr_dir.dir;
    }
    name=0;
    uint64_t parent=dest->location;
    uint32_t buff=(uint32_t)fsalloc(512);
    read(curr_port,parent/512,1,(DWORD)buff);
    Directory_t* Parent=(Directory_t*)(buff+(uint32_t)(parent%512));
    uint64_t tmp=Parent->First_child;
    printf("\nShowing %x folders of %s\\",Parent->folders,Parent->name);
    uint32_t buf=fsalloc(512);
    Directory_t* temp;
    for(uint32_t i=0;i<Parent->folders;i++)
    {
      read(curr_port,tmp/512,1,(DWORD)buf);
      temp=(Directory_t*)((uint32_t)(buf+(uint32_t)(tmp%512)));
      printf("\n\t \\%s",temp->name);
      tmp=temp->Next_Friend;
    }
    ////kfree((uint32_t*)buf);
    printf("\n");
}

void find_childFiles(char* name)
{
  Directory_t* dest;
  if(name)
  {
    dest=search_folder(name);
    if(!dest)
    {
      printf("\nSearched Folder %s Not found",name);
      return;
    }
  }
  else
  {
    dest=(Directory_t*)fsalloc(sizeof(Directory_t));
    *dest=curr_dir.dir;
  }
  uint32_t buf= fsalloc(512);
  read(curr_port, dest->First_file/512, 1, buf);
  File_t* file = (File_t*)(buf + (uint32_t)(dest->First_file%512));

  printf("\nShowing %x files of %s\\",dest->files, dest->name);
  for(uint32_t i = 0; i<dest->files; i++)
  {
    printf("\n\t\t \\%s",file->name);
    read(curr_port, file->Next_Friend/512, 1, buf);
    file = (File_t*)(buf + (uint32_t)(file->Next_Friend%512));
  }
  printf("\n");
}

Directory_t* search_folder(char* name)
{
    uint32_t buf=(uint32_t)fsalloc(512);
    read(curr_port,curr_dir.dir.location/512,1,(DWORD)buf);
    Directory_t* dir=(Directory_t*)(buf+(uint32_t)(curr_dir.dir.location%512));
    uint64_t cdir=dir->First_child;
    if(!cdir)
    {
      printf("\nFolder not found1\n");
      return 0;
    }
    read(curr_port,cdir/512,1,(DWORD)buf);
    Directory_t* temp=(Directory_t*)(buf+(uint32_t)(cdir%512));
    for(int i=0;;i++)
    {
      if(!strcmp(temp->name,name))
        break;
      cdir=temp->Next_Friend;
      if(!cdir)
      {
        printf("\nFolder not found\n");
        return 0;
      }
      read(curr_port,cdir/512,1,(DWORD)buf);
      temp=(Directory_t*)(buf+(uint32_t)(cdir%512));
    }
    return temp;
}

File_handle_t* file_loader(char* name)
{
    uint32_t buf=(uint32_t)fsalloc(512);
    //printf("\nfile location 1 %x",buf);
    read(curr_port,curr_dir.dir.location/512,1,(DWORD)buf);
    Directory_t* dir=(Directory_t*)(buf+(uint32_t)(curr_dir.dir.location%512));
    uint64_t cdir=dir->First_file;
    if(!cdir)
    {
      printf("\nThe folder is empty!!!\n");
      return 0;
    }
    read(curr_port,cdir/512,1,(DWORD)buf);
    File_t* temp=(File_t*)(buf+(uint32_t)(cdir%512));
    for(int i=0;;i++)
    {
      if(!strcmp(temp->name,name))
        break;
      cdir=temp->Next_Friend;
      if(!cdir)
      {
        printf("\nFile not found\n");
        return 0;
      }
      read(curr_port,cdir/512,1,(DWORD)buf);
      temp=(File_t*)(buf+(uint32_t)(cdir%512));
    }
    File_handle_t* handle=(File_handle_t*)fsalloc(sizeof(File_handle_t));
    File_t* tb=(File_t*)fsalloc(sizeof(File_t));
    memcpy((void*)tb,(void*)temp,sizeof(File_t));
    handle->file=(uint32_t)tb;
    handle->name=tb->name;
    ////kfree((uint32_t*)buf);
    return handle;
}

void file_load(char *name)
{
    File_handle_t* handle=file_loader(name);
    if(!handle)
    {
      printf("\nCant load the file requested\n");
      return;
    }
    if(!start_handle)
    {
      start_handle=handle;
    }
    else
      current->next=handle;
    current=handle;
    current->next=0;
    printf("\nFile %s Loaded \n",name);
}

void file_close(char *name)
{
    file_flush(name);
    File_handle_t* temp=start_handle,*temp2=temp;
    for(int i=0;start_handle;i++)
    {
      if(!strcmp(temp->name,name))
      {
        if(!i)
            start_handle=temp->next;
        else temp2->next=temp->next;
        ////kfree((uint32_t*)temp);
        goto out;
      }
      if(!temp->next) break;
      temp2=temp;
      temp=temp->next;
    }
    return;
    out:

    return;
}

File_handle_t* file_search(char* name)
{
    File_handle_t* temp=start_handle;
    for(int i=0;start_handle;i++)
    {
      if(!strcmp(temp->name,name))
      {
        goto out;
      }
      if(!temp->next) break;
      temp=temp->next;
    }
    printf("\nFile %s not loaded yet!\n",name);
    return 0;
    out:
    return temp;
}

void set_curr_dir(uint64_t location)
{
    uint32_t buf=fsalloc(4096);
    read(curr_port,location/512,1,(DWORD)buf);
    Directory_t* dir=(Directory_t*)(buf+(uint32_t)(location%512));
    curr_dir.dir=*dir;
    char** dir_name;
    char name[32]="";
    buf=fsalloc(4096);
    Directory_t* temp=dir;
    uint64_t tmp=dir->parent;
    int i;
    for(i=0;strcmp(temp->name,"root")!=0;i++)
    {
      dir_name[i]=temp->name;
      printf("\n%s",dir_name[i]);
      read(curr_port,tmp/512,1,(DWORD)buf);
      temp=(Directory_t*)(buf+(uint32_t)(tmp%512));
      tmp=temp->parent;
    }
    dir_name[i]="root";
    for(;i>0;)
    {
      strcat(name,dir_name[i]);
      strcat(name,"\\");
      --i;
    }
    strcat(name,dir_name[0]);
    strcpy(curr_dir.full_name,name);
    printf("\ncurr dir: %s",name);
  //  if(multitasking_ON)
  //    while(1);
    ////kfree((uint32_t*)buf);
}

File_Header_t* nx_header(File_Header_t* prev_header)
{
  if(!prev_header || !prev_header->Next_Header) return 0;
  File_Header_t* tmp = fsalloc(512);
  read(curr_port, (prev_header->Next_Header/512), 1, tmp);
  if(tmp->magic == FHR_MAGIC)
    return tmp;
  //kfree(tmp);
  return 0;
}

File_Header_t* get_header(uint64_t location)
{
  if(!location) return 0;
  File_Header_t* tmp = fsalloc(512);
  read(curr_port, location/512, 1, tmp);
  if(tmp->magic == FHR_MAGIC)
    return tmp;
  //kfree(tmp);
  return 0;
}

int del_header(File_Header_t* header)
{
  File_Header_t* pheader = get_header(header->Previous_Header);
  File_Header_t* nheader = get_header(header->Next_Header);

  pheader->Next_Header = nheader->location;
  nheader->Previous_Header = pheader->location;
  flush_header(nheader);
  flush_header(nheader);
}

File_Header_t* File_Header_Creator(File_t* file, uint16_t blocks) //Creates a header at the last of the file.
{
    //printf("\nAdding more available data space for the file %s \n",curr_file.name);
    fsbuf=fsalloc(512);
    File_Header_t* header=(File_Header_t*)fsbuf;
    header->File_location = file->location;
    header->Next_Header=0;
    header->used=0;
    header->location=sec_alloc(blocks+2);
    header->spread=blocks*512;
    header->magic = FHR_MAGIC;
    header->Previous_Header=file->last_header;
    if(file->last_header)
    {
      uint32_t buf=fsalloc(512);
      read(curr_port,file->last_header/512,1,(DWORD)buf);
      File_Header_t* header2=(File_Header_t*)(buf+(uint32_t)(file->last_header%512));
      header2->Next_Header=header->location;
      write(curr_port,file->last_header/512,1,(DWORD)buf);
    }
    else
    {
      file->first_header=header->location;
    }
    write(curr_port,header->location/512,1,(DWORD)fsbuf);

    file->last_header=header->location;
    file->headers++;
    file->sz+=sizeof(File_Header_t);

    return header;
}

File_Header_t* File_Header_Creator_sdw(File_t* file, File_Header_t* left_header,  uint16_t blocks) //Sandwiched Header creator (in between two existing headers)
{
    //printf("\nAdding more available data space for the file %s \n",curr_file.name);
    fsbuf=fsalloc(512);
    File_Header_t* header=(File_Header_t*)fsbuf;
    header->File_location = file->location;
    header->Next_Header=left_header->Next_Header;
    header->used=0;
    header->location=sec_alloc(blocks+2); //Extra for extra assurance
    header->spread=blocks*512;
    header->magic = FHR_MAGIC;
    header->Previous_Header=left_header->location;
    uint32_t buf=fsalloc(512);
    left_header->Next_Header=header->location;
    write(curr_port,left_header->location/512,1,(DWORD)buf);

    header->Previous_Header = left_header->location;

    write(curr_port,header->location/512,1,(DWORD)fsbuf);
    //kfree(buf);

    if(!file->first_header)
    {
      file->first_header=header->location;
    }
    file->last_header=header->location;
    file->headers++;
    file->sz+=sizeof(File_Header_t);

    return header;
}

void file_flush(char* name)
{
    File_handle_t* handle=file_search(name);
    if(!handle) return;
    uint32_t buff=fsalloc(512);
    File_t* file=(File_t*)handle->file;
    read(curr_port,file->location/512,1,(DWORD)buff);
    File_t* ftmp=(File_t*)(buff +(uint32_t)(file->location%512));
    memcpy((void*)ftmp,(void*)file,sizeof(File_t));
    write(curr_port,file->location/512,1,(DWORD)buff);
    ////kfree((uint32_t*)buff);
}

void file_truncate(File_handle_t* handle)
{
    File_t* file=(File_t*)handle->file;
    uint32_t buf=fsalloc(1024);
    File_Header_t* header=(File_Header_t*)buf;
    uint32_t buff=fsalloc(1024);
    memset((void*)buff,0,1024);
    uint64_t temp=file->first_header;
    for(uint32_t i=0;i<file->headers;i++)
    {
      read(curr_port,temp/512,2,(DWORD)buf);
      write(curr_port,temp/512,2,(DWORD)buff);
      temp=header->Next_Header;
    }
    file->sz=0;
    file->first_header=0;
    file->headers=0;
}

File_Header_t* file_header_search(uint32_t foffset, File_t* file) //Finds the header of a file which contains the memory regions of the offset
{
  uint32_t* tm = fsalloc(1024);
  read(curr_port, file->first_header/512, 1, tm);
  File_Header_t* tmp = (File_Header_t*)(tm + (uint32_t)(file->first_header%512));

  uint32_t ts = 0;
  uint64_t tmp2 = 0;
  for(int i = 0; i < file->headers; i++)
  {
    ts += tmp->used;
    if(ts >= foffset)
    {
      tmp->reserved = ts - foffset; //Offset into header
      return tmp;
    }
    tmp2 = tmp->Next_Header;
    read(curr_port,tmp2/512,1,(DWORD)tm);
    tmp = (File_Header_t*)(tm + (uint32_t)(file->first_header%512));
  }
  return -1;
}

inline void flush_header(File_Header_t* header)
{
  write(curr_port, header->location/512, 1, (uint32_t)header);
}

int file_readTM(uint32_t* buffer, uint32_t offset, uint32_t size, char* file_name) //Read file content and write to memory.
{
  File_handle_t* handle=file_search(file_name);
  if(!handle) return -1; //File not loaded yet.

  File_t* file_st = handle->file;
  File_Header_t* header = file_header_search(offset, file_st); //find which header has the offset memory.

  if(!header) return -1; //Some error!!!!

  if(header->magic != FHR_MAGIC)
  {
    //kfree(header);
    return -2; //Not a valid Header.
  }

  uint32_t a1 = MIN(header->reserved, size);
  uint32_t b1 = header->used - header->reserved; //Local offset

  uint32_t tbuff = fsalloc(ROUNDUP(a1,1024));

  read(curr_port, (1 + ((header->location+b1)/512)), (a1/512)+2, tbuff); //Read the first part of buffer.

  uint32_t cpdone = 0;

  uint32_t bb = (uint32_t)buffer;
  memcpy(buffer, tbuff + (b1%512), a1); //Get to the local offset and copy the data which has to be copied.

  //kfree(tbuff);
  cpdone += a1;

  uint32_t left = size - cpdone;

  while(left)   //Keep extracting data from file until we extract all the required data successfully.
  {
    header = nx_header(header);

    if(!header) return 0; //The Size requested from offset is more then the size of the file/Invalid header.

    if(left < header->used) //Only 1 header left to be read.
    {
      tbuff = fsalloc(ROUNDUP(left,512));
      read(curr_port, 1 + (header->location/512), 1 + (left/512), tbuff);

      memcpy((bb + cpdone), tbuff, left);

      //kfree(tbuff);
      //kfree(header);
      return 1; //Everything went fine.
    }
    tbuff = fsalloc(header->used);
    read(curr_port, 1 + (header->location/512), 1 + (header->used/512), tbuff);

    memcpy((bb+cpdone), tbuff, header->used);

    cpdone += header->used;
    //kfree(tbuff);

    left -= header->used;
  }
  //kfree(header);
  return 1; //Everything went fine.
}

int file_writeAppend(uint32_t* buffer, uint32_t size, char* file_name) //Write to a file from memory.
{
  File_handle_t* handle=file_search(file_name);
  if(!handle) return -1; //File not loaded yet.

  uint32_t bb = buffer;
  File_t* file_st = handle->file;
  File_Header_t* header;
  if(file_st->last_header)
  {
    uint32_t buf = fsalloc(512);
    read(curr_port, file_st->last_header/512, 1, buf);
    header = (File_Header_t*)(buf);

    //printf("\nheader->spread: %x %x %x\n", header->spread, header->used, file_st->headers);

    if(header->magic != FHR_MAGIC) return -3;
    int t = header->spread - header->used;
    if(t) //If there is some space in the last header, fill it.
    {
      uint32_t tbuff = fsalloc(header->spread);
      memset(tbuff,0,header->spread);

      read(curr_port, 1 + (header->location/512), 1 + (header->used/512), tbuff);
      memcpy(tbuff + header->used, buffer, MIN(size, t));
      write(curr_port, 1 + (header->location/512), 1 + (header->spread/512), tbuff);
      header->used += MIN(size,t);
      size -= MIN(size, t);
      bb += MIN(size, t);
      buffer = bb;

      flush_header(header);
      //kfree(tbuff);
      //kfree(header);

      if(!size) return 1;
    }
  }
  uint32_t blks = ROUNDUP(size,512)/512;

  header = File_Header_Creator(file_st, blks);
  if(!header) return -1;

  uint32_t tbuff = fsalloc(ROUNDUP(size,512));
  memcpy(tbuff, buffer, size);

  write(curr_port, 1 + (header->location/512), blks, tbuff);
  //kfree(tbuff);
  header->used = size;
  flush_header(header);
  //kfree(header);

  return 1;
}

int file_editFM(uint32_t offset, uint32_t osize, uint32_t *buffer, uint32_t fsize, char* file_name)
{
  File_handle_t* handle=file_search(file_name);
  if(!handle) return -1; //File not loaded yet.

  File_t* file_st = handle->file;
  File_Header_t* header = file_header_search(offset, file_st); //find which header has the offset memory.

  if(!header) return -1; //Some error!!!!

  if(header->magic != FHR_MAGIC)
  {
    //kfree(header);
    return -2; //Not a valid Header.
  }

  uint32_t left_end = header->used - header->reserved;

  uint32_t sz = header->spread - left_end;
  uint32_t bb = (uint32_t)buffer;

  uint32_t tl = header->spread - left_end;
  uint32_t cpdone = 0;

  //Several cases are possible->
  // [********------], [******------    ], [********-----****   ], [*****------][-----******]

  if(left_end + osize <= header->used)  // [********------], [******------    ], [********-----****], [*******----***   ]
  {
    //Local header offset [********----*****_____]
    //                            ^    ^   ^
    //                            1    2    3
    //  1 = left_end, 2 = a1, 3 - 2 = b1.

    uint32_t a1 = left_end + osize;
    uint32_t b1 = header->used - a1;



    uint32_t buff = fsalloc(ROUNDUP(fsize + b1, 1024));
    read(curr_port, 1 + ((header->location + left_end)/512), 1, buff);

    memcpy(buff+(left_end%512), bb, fsize);

    if(b1)
    {
      uint32_t tbuff = fsalloc(ROUNDUP(b1,1024));
      read(curr_port, 1 + ((header->location + a1)/512), 2 + (b1/512), tbuff);

      memcpy(buff+(left_end%512)+fsize, tbuff + (a1%512), b1);
      //kfree(tbuff);
    }

    fsize += b1;

    write(curr_port, 1 + ((header->location + left_end)/512), ROUNDUP(tl, 512)/512, buff);
    cpdone += tl;

    if(fsize > tl)
    {
      uint32_t tdone = fsize - tl;
      File_Header_t* nxHeader = File_Header_Creator_sdw(file_st, header, (tdone/512)+1);
      nxHeader->used = tdone;
      write(curr_port, 1 + (nxHeader->location/512), 1 + (tdone/512), buff+cpdone);
      flush_header(nxHeader);
      //kfree(nxHeader);
    }

    flush_header(header);
    //kfree(buff);
    //kfree(header);
    return 1;
  }
  else
  {
    // cases:        [*****------][-----******], [*******-----][-------][-----*****], [*******-----][-------][**********],
    //               [*******-----__][-------__][-----*****], [*****----][-----][-----**    ]


    //    [******-------][--------][------*****____]
    //           ^                       ^    ^
    //           1                       2    3
    // 1 = left_end, 2 = a1, 3 - 2 = b1, 2-1 = osize

    uint32_t cp = header->used - left_end;

    File_Header_t* theader = nx_header(header);
    File_Header_t* tmp = nx_header(theader);

    while(1)
    {
      if(!theader) return -1;

      cp += theader->used;

      if(cp >= osize) break;

      del_header(theader);
      theader = tmp;
      tmp = nx_header(tmp);
    }

    uint32_t b1 = cp - osize;
    uint32_t a1 = header->used - b1;

    uint32_t bb = fsalloc(ROUNDUP(fsize + b1, 1024));

    read(curr_port, 1 + (header->location/512), 1, bb);
    memcpy(bb+(left_end%512), buffer, fsize);

    if(b1)
    {
      uint32_t tbuff = fsalloc(ROUNDUP(b1, 1024));
      read(curr_port, 1 + ((theader->location+a1)/512), 2 + (b1/512), tbuff);
      memcpy(bb+(left_end%512)+fsize, tbuff + (a1%512), b1);

      //kfree(tbuff);
      fsize += b1;
    }
    del_header(theader);


    write(curr_port, 1 + ((header->location + left_end)/512), ROUNDUP(tl, 512)/512, bb);
    cpdone += tl;

    if(fsize > tl)
    {
      uint32_t tdone = fsize - tl;
      File_Header_t* nxHeader = File_Header_Creator_sdw(file_st, header, (tdone/512)+1);
      nxHeader->used = tdone;
      write(curr_port, 1 + (nxHeader->location/512), 1 + (tdone/512), bb+cpdone);
      flush_header(nxHeader);
      //kfree(nxHeader);
    }

    flush_header(header);
    //kfree(bb);
    //kfree(header);
    return 1;
  }

}

void make_boot_sector()
{
    uint32_t buf=(uint32_t)fsalloc(1024);
    memset(buf,0,1024);
    read(curr_port,0,2,(DWORD)buf);
    Identity_Sectors_t* identity=(Identity_Sectors_t*)(buf + 436);
    strcpy(identity->name,"AqFS472");
    identity->active_partition = 446; //Partition 1

    uint8_t* boot_ptr = buf;
    boot_ptr += 510;
    *boot_ptr = 0x55;
    ++boot_ptr;
    *boot_ptr = 0xAA;
    write(curr_port,0,2,(DWORD)buf); //2nd sector.
    memset(buf,0,1024);
    uint32_t* tmp = buf;
    *tmp = root_location;
    write(curr_port,start_off,2,(DWORD)buf);
}

void Init_fs()
{
    curr_ahci=ahci_start;
    curr_disk=&curr_ahci->Disk[1];
    curr_port=&abar->ports[1];
    SATA_ident_t* info=(SATA_ident_t*)curr_disk->info;
    sectors=info->lba_capacity;
    printf("\nTotal Sectors: %x\n",sectors);
    start_off=10240;
    bytes=sectors;
    off=(bytes/512)/512;
    off += 16;
    ++off;
    bytemap_off = off+start_off;
    bytemap_end = start_off + (bytes/512);

    start_handle=0;
    printf("\nFormating and Partitioning the Disk, May take a few minutes...\n");
    fs_alloc_init();
    printf("\nFile System formated successfully");
    root_location=fs_alloc(2);
    make_boot_sector();

    printf("\nMaster Partition made successfully");
    Setup_fs();
    printf("\nLoaded the root directory");
    create_directory("Aqeous",1,0);
    create_directory("Programs",1,0);
    create_directory("user",1,0);
    create_directory("System",1,0);
    create_directory("System",1,"Aqeous");
    printf("\n");
    create_file("test.txt",1,0);
    create_file("test2.txt",1,0);
    create_file("test3.txt",1,0);
    create_file("test4.txt",1,0);
    printf("\nASDASDASDADASD");
    find_friendDirs(0);
    find_childFiles(0);
}
