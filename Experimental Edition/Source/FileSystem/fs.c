#include "fs.h"
#include "fs_alloc.h"

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
    //kfree((uint32_t*)buf);
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
        //kfree((uint32_t*)buf);
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
    //kfree((uint32_t*)dest);
    //kfree((uint32_t*)buff);
    //kfree((uint32_t*)buff2);
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
        //kfree((uint32_t*)buf);
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
    //kfree((uint32_t*)dest);
    //kfree((uint32_t*)buff);
    //kfree((uint32_t*)buff2);
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
    //kfree((uint32_t*)buf);
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
    //kfree((uint32_t*)buf);
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
  /*  if(ios & open)
    {
      file_truncate(current);
    }*/
    //***Load the File requested into the Memory***//
    /*
    File_t* file=(File_t*)handle->file;
    uint32_t buf=0;
    if(ios & in)
    {
      buf+=(file->sz-(file->headers*sizeof(File_Header_t)));
      handle->put_ptr=(file->sz-(file->headers*sizeof(File_Header_t)));
    }
    if(ios & out)
    {
      if(ios & app)
      {
        buf+=4096;
        handle->put_ptr=0;
      }
      else if(ios & ate)
      {
        buf+=(file->sz-(file->headers*sizeof(File_Header_t)))+4096;
        handle->put_ptr=(file->sz-(file->headers*sizeof(File_Header_t)));
      }
    }
    handle->buf=malloc(buf);
    file_read(handle->buf,0,0,name);
    handle->get_ptr=0;
    handle->buf_pot=0;*/
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
        //kfree((uint32_t*)temp);
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
    read(curr_port,location/512,1,(DWORD)buf);
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
    //kfree((uint32_t*)buf);
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
    write(curr_port,0,2,(DWORD)buf);
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
    uint32_t buf=fsalloc(1024);
    read(curr_port,0,2,(DWORD)buf);
    Boot_sectors_t* boot=(Boot_sectors_t*)buf;
    root_location=boot->partition_locations[0];
    printf("\nabc %s %x",boot->name,root_location);
    set_curr_dir(root_location);
    /*
    find_dir(0);
    find_dir("Aqeous");

    file_load("test.txt",app);
    file_load("test2.txt",app);
    file_load("test3.txt",app);

    char str[]="Hello!!! This is Aqeous OS Speaking And I am Testing the Filesystem. I need a really very very very big message to test the I/O operations of my file system's file handling.";//" So please let me do what I want. This is a completely new FileSystem and thus needs a lot of testing. There are Several bugs and I am gonna fix them all!!! As I Said that I need a really very long message to test wether the file handling can actually manage reading writing operations with data bigger then the sector size, that is 512 bytes, so thats why I am writing this long test. The text should be longer then 512 bytes, That is the text should contains 512 characters. I am writing this in block alignmnet to manage space :3 This filesystem can be potentially better then FAT and so Its necessary to remove allthe bugs before releasing it. Thats obvious LOL xD .So please be patient. Thank you :)";
    file_write((uint32_t)str,strlen(str),"test.txt");
    file_flush("test.txt");
    uint32_t buffer=fsalloc(8192);
    file_read(buffer,0,550,"test.txt");*/

}

File_Header_t* File_Header_Creator(File_t* file, uint16_t blocks)
{
    //printf("\nAdding more available data space for the file %s \n",curr_file.name);
    fsbuf=fsalloc(512);
    File_Header_t* header=(File_Header_t*)fsbuf;
    header->File_location = file->location;
    header->Next_Header=0;
    header->used=512;
    header->location=sec_alloc(blocks);
    header->spread=blocks;
    header->Previous_Header=file->last_header;
    uint32_t buf=fsalloc(512);
    read(curr_port,file->last_header/512,1,(DWORD)buf);
    File_Header_t* header2=(File_Header_t*)buf;
    header2->Next_Header=header->location;
    write(curr_port,file->last_header/512,1,(DWORD)buf);
    //kfree((uint32_t*)buf);
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
      read(curr_port,temp/512,2,(DWORD)buf);
      write(curr_port,temp/512,2,(DWORD)buff);
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
    read(curr_port,file->location/512,1,(DWORD)buff);
    File_t* ftmp=(File_t*)(buff +(uint32_t)(file->location%512));
    memcpy((void*)ftmp,(void*)file,sizeof(File_t));
    write(curr_port,file->location/512,1,(DWORD)buff);
    //kfree((uint32_t*)buff);
}

void file_write(uint32_t buffer,uint32_t size, char* file_name) //new
{
    File_handle_t* handle=file_search(file_name);
    if(!handle) return;
    File_t* file=(File_t*)handle->file;
    File_Header_t* header;
    uint32_t sz=size;
    uint32_t tbuf=buffer;
    uint64_t location;
    uint32_t blocks=sz/512;
    blocks++;
    if(!file->first_header)
    {
      header=File_Header_Creator(file,MIN((int)(blocks/2)+1,65535));
      location=header->location/512;
    }
    else
    {
      uint32_t buf=fsalloc(512);
      read(curr_port,file->last_header/512,1,(DWORD)buf);
      header=(File_Header_t*)buf;
      location=header->location/512;
      uint32_t tmp1=(header->spread*1024)-header->used;
      uint32_t tmp2=1024-tmp1;//header->used-((header->spread-1)*1024);
      uint32_t tmb=fsalloc(1024);
      read(curr_port,location+((header->spread-1)*2),2,(DWORD)tmb);
      memcpy((void*)(tmb+tmp2),(void*)(tbuf),MIN(tmp1,sz));
      write(curr_port,location+((header->spread-1)*2),2,(DWORD)tmb);
      //kfree((uint32_t*)tmb);
      tbuf+=MIN(tmp1,sz);
      header->used+=MIN(tmp1,sz);
      write(curr_port,location,1,(DWORD)buf);
      if(sz<=tmp1)
      {
        goto out;
      }
      sz-=tmp1;
      blocks=sz/512;
      blocks++;
      header=File_Header_Creator(file,MIN((int)(blocks/2)+1,65535));
      location=header->location/512;
    }
    for(int i=0;;i++)
    {
      if(blocks<65535*2)
      {
        if(blocks>1)
        {
          write(curr_port,location+1,(blocks)-1,(DWORD)tbuf);
          tbuf+=(blocks-1)*512;
        }
        uint32_t abuf=fsalloc(512);
        memcpy((void*)abuf,(void*)tbuf,sz-((blocks-1)*2));
        write(curr_port,location+blocks,1,(DWORD)abuf);
        //kfree((uint32_t*)abuf);
        header->used+=sz;
        write(curr_port,location,1,(DWORD)header);
        goto out;
      }
      write(curr_port,location+1,((65535)),(DWORD)tbuf);
      header->used+=65535;
      write(curr_port,location,1,(DWORD)header);
      header=File_Header_Creator(file,MIN((int)(blocks/2)+1,65535));
      location=header->location/512;
      tbuf+=(65535)*1024;
      blocks-=(65535)*2;
      sz-=(67107850);
    }
    out:
    file->sz+=size;
    //printf("\nInfo written to the file %s Successfully\n",file->name);
}

void file_read(uint32_t buffer, uint32_t size, uint32_t offset, char* name)
{
    File_handle_t* handle=file_search(name);
    if(!handle) return;
    uint32_t buf=buffer;
    char* str2=(char*)buffer;
    File_t* file=(File_t*)handle->file;
    if(file->sz-file->headers*sizeof(File_Header_t)<offset)
    {
      printf("\nThe Requested offset is beyond the actual file size");
      return;
    }
    if(!size)
    {
      size=file->sz-offset;
      size-=file->headers*sizeof(File_Header_t);
    }
    if(file->sz<size+offset)
    {
      printf("\nThe Requested size is beyond the actual file size");
      return;
    }
    File_Header_t* header=(File_Header_t*)fsalloc(512);
    uint64_t tmp=file->first_header/512;
    uint32_t tmp2=0;
    uint32_t sz=(size);
    for(int i=0;;i++)
    {
      if(!header)
      {
        printf("\nThe offset is beyond the size of the file");
        return;
      }
      read(curr_port,tmp,1,(DWORD)header);
      tmp2+=header->spread*1024;
      tmp2-=512;
      if(offset<=tmp2)
      {
        break;
      }
      tmp=header->Next_Header/512;
    }
    tmp2+=512;
    tmp2-=header->spread*1024;
    uint32_t off=offset-tmp2;
    uint32_t roff=off/512;
    roff=off-roff;
    uint32_t t=fsalloc(512);
    read(curr_port,tmp+(off/512)+1,1,(DWORD)t);
    memcpy((void*)buf,(void*)(t+roff),MIN(sz,512-roff));
    if(sz<512-roff)
    {
      goto out;
    }
    buf+=(512-roff);
    sz-=(512-roff);
    uint32_t blocks=sz/512;
    blocks++;
    tmp++;
    uint32_t temp=(header->spread*2)-1;
    --temp;
    for(;;)
    {
      if(temp)
      {
        read(curr_port,tmp+1,MIN(blocks,temp),(DWORD)buf);
        buf+=MIN(blocks,temp)*512;
        --blocks;
      }
      if(!blocks)
      {
        goto out;
      }
      tmp=header->Next_Header/512;
      read(curr_port,tmp,1,(DWORD)header);
      temp=(header->spread*2)-1;
    }
    out:
    printf("\nFile reads as: %s \nFile size: %x",buffer,file->sz);
    return;
}
