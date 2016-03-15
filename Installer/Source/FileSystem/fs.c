#include "fs.h"
#include "fs_alloc.h"

uint64_t root_location;

void Setup_fs()
{
    uint32_t buf=fsalloc(512);
    read(curr_port,(uint32_t)root_location/512,0,1,(DWORD)buf);
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
    write(curr_port,(uint32_t)root_location/512,0,1,(DWORD)buf);
    free((uint32_t*)buf);
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
    uint64_t Next_available=fs_alloc(2);
    read(curr_port,(uint32_t)Next_available/512,0,1,(DWORD)buff);
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
    write(curr_port,(uint32_t)Next_available/512,0,1,(DWORD)buff);
    uint32_t buff2=fsalloc(512);
    read(curr_port,(uint32_t)parent/512,0,1,(DWORD)buff2);
    Directory_t* Parent=(Directory_t*)(uint32_t)(buff2+(uint32_t)(parent%512));
    uint64_t tm;
    //printf("\n%s %x",dir->name,dir->location);
    if(Parent->last_child)
    {
        tm=Parent->last_child;
        Parent->last_child=dir->location;
        Parent->folders++;
        write(curr_port,(uint32_t)parent/512,0,1,(DWORD)buff2);
        uint32_t buf=fsalloc(512);
        read(curr_port,(uint32_t)tm/512,0,1,(DWORD)buf);
        Directory_t* dir2=(Directory_t*)(buf+(uint32_t)(tm%512));
        dir2->Next_Friend=dir->location;
        write(curr_port,(uint32_t)tm/512,0,1,(DWORD)buf);
        free((uint32_t*)buf);
    }
    else
    {
      Parent->First_child=dir->location;
      Parent->last_child=dir->location;
      Parent->folders++;
      write(curr_port,(uint32_t)parent/512,0,1,(DWORD)buff2);
    }

    printf("\nFolder \"%s\" Created Successfully\n",name);
    name=0;
    destination=0;
    free((uint32_t*)dest);
    free((uint32_t*)buff);
    free((uint32_t*)buff2);
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
    uint64_t Next_available=fs_alloc(2);
    read(curr_port,Next_available/512,0,1,(DWORD)buff);
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
    write(curr_port,(uint32_t)file->location/512,0,1,(DWORD)buff);

    uint32_t buff2=fsalloc(512);
    read(curr_port,(uint32_t)parent/512,0,1,(DWORD)buff2);
    Directory_t* Parent=(Directory_t*)(uint32_t)(buff2+(uint32_t)(parent%512));
    uint64_t tm;
    //printf("\n%s %x",dir->name,dir->location);
    if(Parent->last_file)
    {
        tm=Parent->last_file;
        Parent->last_file=file->location;
        Parent->files++;
        write(curr_port,(uint32_t)parent/512,0,1,(DWORD)buff2);
        uint32_t buf=fsalloc(512);
        read(curr_port,(uint32_t)tm/512,0,1,(DWORD)buf);
        File_t* file2=(File_t*)(buf+(uint32_t)(tm%512));
        file2->Next_Friend=file->location;
        write(curr_port,(uint32_t)tm/512,0,1,(DWORD)buf);
        free((uint32_t*)buf);
    }
    else
    {
      Parent->First_file=file->location;
      Parent->last_file=file->location;
      Parent->files++;
      write(curr_port,(uint32_t)parent/512,0,1,(DWORD)buff2);
    }

    printf("\nFile \"%s\" Created Successfully\n",name);
    name=0;
    destination=0;
    free((uint32_t*)dest);
    free((uint32_t*)buff);
    free((uint32_t*)buff2);
}

void find_dir(char* name)
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
    uint32_t off=parent/512;
    uint32_t buff=(uint32_t)fsalloc(512);
    read(curr_port,off,0,1,(DWORD)buff);
    Directory_t* Parent=(Directory_t*)(buff+(uint32_t)(parent%512));
    uint64_t tmp=Parent->First_child;
    printf("\nShowing %x folders of %s\\",Parent->folders,Parent->name);
    uint32_t buf=fsalloc(512);
    Directory_t* temp;
    for(uint32_t i=0;i<Parent->folders;i++)
    {
      read(curr_port,tmp/512,0,1,(DWORD)buf);
      temp=(Directory_t*)((uint32_t)(buf+(uint32_t)(tmp%512)));
      printf("\n\t \\%s",temp->name);
      tmp=temp->Next_Friend;
    }
    free((uint32_t*)buf);
    printf("\n");
}

Directory_t* search_folder(char* name)
{
    uint32_t buf=(uint32_t)fsalloc(512);
    read(curr_port,curr_dir.dir.location/512,0,1,(DWORD)buf);
    Directory_t* dir=(Directory_t*)(buf+(uint32_t)(curr_dir.dir.location%512));
    uint64_t cdir=dir->First_child;
    if(!cdir)
    {
      printf("\nFolder not found1\n");
      return 0;
    }
    read(curr_port,cdir/512,0,1,(DWORD)buf);
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
      read(curr_port,cdir/512,0,1,(DWORD)buf);
      temp=(Directory_t*)(buf+(uint32_t)(cdir%512));
    }
    return temp;
}

File_handle_t* file_loader(char* name)
{
    uint32_t buf=(uint32_t)fsalloc(512);
    //printf("\nfile location 1 %x",buf);
    read(curr_port,curr_dir.dir.location/512,0,1,(DWORD)buf);
    Directory_t* dir=(Directory_t*)(buf+(uint32_t)(curr_dir.dir.location%512));
    uint64_t cdir=dir->First_file;
    if(!cdir)
    {
      printf("\nThe folder is empty!!!\n");
      return 0;
    }
    read(curr_port,(uint32_t)cdir/512,0,1,(DWORD)buf);
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
      read(curr_port,cdir/512,0,1,(DWORD)buf);
      temp=(File_t*)(buf+(uint32_t)(cdir%512));
    }
    File_handle_t* handle=(File_handle_t*)fsalloc(sizeof(File_handle_t));
    File_t* tb=(File_t*)fsalloc(sizeof(File_t));
    memcpy((void*)tb,(void*)temp,sizeof(File_t));
    handle->file=(uint32_t)tb;
    handle->name=tb->name;
    free((uint32_t*)buf);
    return handle;
}

void file_load(char *name, uint8_t ios)
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
    current->ios=ios;
    current->next=0;
    if(ios & open)
    {
      file_truncate(current);
    }
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
        free((uint32_t*)temp);
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
    uint32_t buf=fsalloc(512);
    read(curr_port,(uint32_t)location/512,0,1,(DWORD)buf);
    Directory_t* dir=(Directory_t*)(buf+(uint32_t)(location%512));
    curr_dir.dir=*dir;
    char** dir_name;
    char name[32]="";
    buf=fsalloc(512);
    Directory_t* temp=dir;
    uint64_t tmp=dir->parent;
    int i;
    for(i=0;strcmp(temp->name,"root")!=0;i++)
    {
      dir_name[i]=temp->name;
      read(curr_port,(uint32_t)tmp/512,0,1,(DWORD)buf);
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
    free((uint32_t*)buf);
}

void make_boot_sector()
{
    uint32_t buf=(uint32_t)fsalloc(1024);
    Boot_sectors_t* boot=(Boot_sectors_t*)(buf);
    strcpy(boot->name,"Yureka 1 :D");
    boot->Number_of_sectors=sectors;
    boot->partitions=1;
    boot->partition_locations[0]=root_location;
    boot->bytes_per_sector=512;
    write(curr_port,0,0,2,(DWORD)buf);
}

void Init_fs()
{
    curr_ahci=ahci_start;
    curr_disk=&curr_ahci->Disk[1];
    curr_port=&abar->ports[1];
    SATA_ident_t* info=(SATA_ident_t*)curr_disk->info;
    sectors=info->lba_capacity;
    printf("\nTotal Sectors: %x\n",sectors);
    memset((void*)BASE,0,512);
    start_off=2;
    bytes=sectors;
    off=(bytes/512)/512;
    ++off;

    start_handle=0;
    printf("\nFormating and Partitioning the Disk, May take a few minutes...\n");
    fs_alloc_init();
    root_location=fs_alloc(2);
    make_boot_sector();
    Setup_fs();
    create_directory("Aqeous",1,0);
    create_directory("Programs",1,0);
    create_directory("user",1,0);
    create_directory("System",1,0);
    create_directory("System",1,"Aqeous");
    create_file("test.txt",1,0);
    create_file("test2.txt",1,0);
    create_file("test3.txt",1,0);
    create_file("test4.txt",1,0);
    create_file("test5.txt",1,0);
    create_file("test6.txt",1,0);
    create_file("test7.txt",1,0);
    create_file("test8.txt",1,0);
}

File_Header_t* File_Header_Creator(File_t* file)
{
    //printf("\nAdding more available data space for the file %s \n",curr_file.name);
    fsbuf=fsalloc(1024);
    File_Header_t* header=(File_Header_t*)fsbuf;
    header->File_location = file->location;
    header->Next_Header=0;
    header->used=sizeof(File_Header_t);
    header->location=file_alloc();
    header->Previous_Header=file->last_header;
    if(!file->first_header)
    {
      file->first_header=header->location;
    }
    file->last_header=header->location;
    file->headers++;
    file->sz+=sizeof(File_Header_t);
  //  strcpy(file->name,"abc");
    return header;
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
      read(curr_port,(uint32_t)temp/512,0,2,(DWORD)buf);
      write(curr_port,(uint32_t)temp/512,0,2,(DWORD)buff);
      temp=header->Next_Header;
    }
    file->sz=0;
    file->first_header=0;
    file->headers=0;
}

void file_flush(char* name)
{
    File_handle_t* handle=file_search(name);
    if(!handle) return;
    uint32_t buff=fsalloc(512);
    File_t* file=(File_t*)handle->file;
    read(curr_port,(uint32_t)file->location/512,0,1,(DWORD)buff);
    File_t* ftmp=(File_t*)(buff +(uint32_t)(file->location%512));
    memcpy((void*)ftmp,(void*)file,sizeof(File_t));
    write(curr_port,(uint32_t)file->location/512,0,1,(DWORD)buff);
    memset((void*)buff,0,512);
}

void fwrite(uint32_t buffer,uint32_t size, char* file_name)
{
    File_handle_t* handle=file_search(file_name);
    if(!handle) return;
    File_t* file=(File_t*)handle->file;
    File_Header_t* header;
    if(!file->first_header)
    {
      header=File_Header_Creator(file);
    }
    else
    {
      uint32_t buf=fsalloc(1024);
      read(curr_port,(uint32_t)file->last_header/512,0,2,(DWORD)buf);
      header=(File_Header_t*)buf;
    }
    uint32_t tbuf=buffer;
    File_Header_t* hrd=header;
    int sz=size;
    for(int i=0;sz>0;i++)
    {
      uint32_t buff=(((uint32_t)header)+header->used);
      memcpy((void*)buff,(void*)tbuf,(header_data-header->used));
      tbuf+=(header_data-header->used);
      if(sz<=(header_data-header->used))
      {
        header->used+=sz;
        write(curr_port,(uint32_t)header->location/512,0,2,(DWORD)header);
        break;
      }
      else
      {
        header->used+=header_data-header->used;
        hrd=File_Header_Creator(file);
      }
      write(curr_port,(uint32_t)header->location/512,0,2,(DWORD)header);
      header=hrd;
      sz=sz-(header_data-header->used);
    }
    file->sz+=size;
    //printf("\nInfo written to the file %s Successfully\n",file->name);
}

void fread(uint32_t buffer, uint32_t size, uint32_t offset, char* name)
{
    File_handle_t* handle=file_search(name);
    if(!handle) return;
    uint32_t buf=buffer;
    uint32_t temp;
    File_t* file=(File_t*)handle->file;
    if(!size)
    {
      size=file->sz-offset;
    }
    if(file->sz<(size+offset))
    {
      printf("\nThe Requested size is beyond the actual file size");
      return;
    }
    File_Header_t* header=(File_Header_t*)fsalloc(1024);
    uint32_t tmp=(uint32_t)file->first_header/512;
    uint32_t off=(offset)/(1024-sizeof(File_Header_t));
    uint32_t sz=(size)/(1024-sizeof(File_Header_t));
    for(int i=0;i<off;i++)
    {
      if(!header)
      {
        printf("\nThe offset is beyond the size of the file");
        return;
      }
      read(curr_port,tmp,0,2,(DWORD)header);
      tmp=(uint32_t)header->Next_Header/512;
    }
    temp=((uint32_t)header)+sizeof(File_Header_t);
    for(int i=0;i<=sz;i++)
    {
      read(curr_port,tmp,0,2,(DWORD)header);
      memcpy((void*)buf,(void*)temp,(1024-sizeof(File_Header_t)));
      buf+=(1024-sizeof(File_Header_t));
      tmp=(uint32_t)header->Next_Header/512;
    }
    char* str=(char*)buffer;
    printf("\nFile %s reads as :\n %s\n",file->name,str);
}

uint32_t fread_whole(char* name) //read the whole file
{
    File_handle_t* handle=file_search(name);
    if(!handle) return 0;
    File_t* file=(File_t*)handle->file;
    uint32_t buf=fsalloc(file->sz);
    uint32_t tmp=fsalloc(1024);
    uint32_t tmp2;
    tmp2=tmp+sizeof(File_Header_t);
    uint32_t offset=buf;
    File_Header_t* header=(File_Header_t*)tmp;
    uint32_t temp=(uint32_t)file->first_header;
    for(uint32_t i=0; i<file->headers; i++)
    {
      read(curr_port,temp/512,0,2,(DWORD)tmp);
      memcpy((void*)offset,(void*)tmp2,(header->used-sizeof(File_Header_t)));
      offset+=(1024-sizeof(File_Header_t));
      temp=(uint32_t)header->Next_Header;
      memset((void*)tmp,0,1024);
    }
    char* str=(char*)buf;
    printf("\nFile %s reads as :\n %s\n",file->name,str);
    return buf;
}
