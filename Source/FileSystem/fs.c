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
      dest=search_folder(destination);
      if(!dest)
      {
        printf("\nDestination Folder %s Not found",name);
        return;
      }
    }
    else
    {
      dest=fsalloc(sizeof(Directory_t));
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
      dest=fsalloc(sizeof(Directory_t));
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
    file->sz=1024;
    file->Next_Friend=0;
    file->first_header=file_alloc();
    write(curr_port,file->location/512,0,1,(DWORD)buff);

    uint32_t buff2=fsalloc(512);
    read(curr_port,parent/512,0,1,(DWORD)buff2);
    Directory_t* Parent=(Directory_t*)(uint32_t)(buff2+(uint32_t)(parent%512));

    if(Parent->last_file)
    {
      if((Parent->last_file/512)==parent/512)
      {
        File_t* file2=(File_t*)(uint32_t)(buff2+(uint32_t)(Parent->last_file%512));
        file2->Next_Friend=file->location;
      }
      else
      {
        uint32_t buf=fsalloc(512);
        read(curr_port,Parent->last_child/512,0,1,(DWORD)buf);
        File_t* file2=(File_t*)(uint32_t)(buf+(Parent->last_file%512));
        file2->Next_Friend=file->location;
        write(curr_port,Parent->last_child/512,0,1,(DWORD)buf);
        free((uint32_t*)buf);
      }
    }
    else
    {
      Parent->First_file=file->location;
    }
    Parent->last_file=file->location;
    Parent->files++;
    write(curr_port,parent/512,0,1,(DWORD)buff2);

    printf("\nFile \"%s\" Created Successfully\n",name);
    name=0;
    destination=0;
    free((uint32_t*)dest);
    free((uint32_t*)buff);
    free((uint32_t*)buff2);
    //uint16_t tp=Next_available%512;
    //if((512-tp)<=sizeof(Directory_t))
    //  Next_available=fs_alloc(1);
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
      dest=fsalloc(sizeof(Directory_t));
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
      temp=(File_t*)(buf+(uint32_t)(cdir%512));
    }
    return temp;
}

void file_load(char* name)
{
    uint32_t buf=(uint32_t)fsalloc(512);
    read(curr_port,curr_dir.dir.location/512,0,1,(DWORD)buf);
    Directory_t* dir=(Directory_t*)(buf+(uint32_t)(curr_dir.dir.location%512));
    uint64_t cdir=dir->First_file;
    if(!cdir)
    {
      printf("\nFile not found1\n");
      return;
    }
    read(curr_port,cdir/512,0,1,(DWORD)buf);
    File_t* temp=(File_t*)(buf+(uint32_t)(cdir%512));
    for(int i=0;;i++)
    {
      if(!strcmp(temp->name,name))
        break;
      cdir=temp->Next_Friend;
      if(!cdir)
      {
        printf("\nFile not found\n");
        return;
      }
      read(curr_port,cdir/512,0,1,(DWORD)buf);
      temp=(File_t*)(buf+(uint32_t)(cdir%512));
    }
    curr_file=*temp;
    printf("\nFile %s Loaded\n",curr_file.name);
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
    uint32_t buf=fsalloc(1024);
    read(curr_port,0,0,2,(DWORD)buf);
    Boot_sectors_t* boot=(Boot_sectors_t*)buf;
    root_location=boot->partition_locations[0];
    printf("\nFormating and Partitioning the Disk, May take a few minutes...\n");
    fs_alloc_init();
    root_location=fs_alloc(2);
    make_boot_sector();
    Setup_fs();
    set_curr_dir(root_location);
    create_directory("Aqeous",1,0);
    create_directory("Programs",1,0);
    create_directory("user",1,0);
    create_directory("System",1,0);
    create_directory("System",1,"Aqeous");
    create_file("test.txt",1,0);
    search_folder("System");
    find_dir(0);
    find_dir("Aqeous"); //root
    file_load("test.txt");
}
