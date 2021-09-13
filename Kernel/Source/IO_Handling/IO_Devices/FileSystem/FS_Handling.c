
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "Shell/shell.h"
#include "stdio.h"
#include "MultiTasking/tasking.h"
#include "FS_Handling.h"
#include "string.h"
#include "Shell/shell.h"
#include "fs.h"
#include "ahci.h"
#include "PCI.h"

//uint32_t a = 0;
//uint32_t* b = 0;

void FS_Process_Init()
{
  //TODO: Create copy of FS_Read_Proc/Write_proc and create FS_Handling process with all the structures necessarry in Memory.
  FS_Handling_Process = create_process("FS_Handling_Process", 0, 1, kernel_proc);

}

/*
  printf("\n%d %d",&FS_Read_Proc,FS_Write_Proc);
  uint32_t* test_func = kmalloc(100);
  printf("\n%d",test_func);
  memcpy(test_func, &test_save, 100);
  printf("Ax%d ",a);
  b = &_putchar;
  ((func_t)test_func)();
  printf("Bx%d",a);*/
/*
void test_save()
{
  int (*test_char)(int);
  test_char = b;
  test_char('b');
  memcpy(&a,&b,MIN(4,8));
  a = strlen("asdsad");
}
*/

void FS_Worker(uint32_t job, char* fname)
{
  //TODO: Save the Process structures and put it on sleep.
  //TODO: Find the file, check its size. If its a small file, put the process in serial queue and do the work.
  //TODO: Setup serial structures and leave everything on the serial handler.
  {

  }
  //TODO: Else, if the size is big enough, put the file in parallel mode, do the below things->
  //TODO: Setup std_io structs, parallels structs, specific structs, copies of all required functions, and replace the original process
  //      with the FS process after creating it based on the job required.
  {/*
    switch(job)
    {
      case 1 :

        break;
      case 2 :
        break;
      case 3 :
        break;
      case 4 :
        break;
      default :
    }*/
  }
}

void FS_Load_File()
{
  /* Locations of all important structures */
  SchedulerKits_t* kit = Get_Scheduler();
  std_io_struct_t* io_struct_ = (std_io_struct_t*)(((Process_t*)((task_t*)kit->current_task)->process)->code + (4*((task_t*)kit->current_task)->task_id));
  parallels_i_struct_t* parallels_struct_ = (parallels_i_struct_t*)((std_io_struct_t*)io_struct_)->type_io_struct;
  FS_i_handle_t* specific_struct_ = (FS_i_handle_t*)((parallels_i_struct_t*)parallels_struct_)->specific_struct;
  char* name = specific_struct_->file_name;
  int (*sec_read_func)(HBA_PORT*, QWORD, DWORD, DWORD);
  sec_read_func = (int (*)(HBA_PORT*, QWORD, DWORD, DWORD))specific_struct_->read_sec_func;
  //int (*sec_write_func)(HBA_PORT*, QWORD, DWORD, DWORD);
  //sec_write_func = (int (*)(HBA_PORT*, QWORD, DWORD, DWORD))specific_struct_->write_sec_func;
  uint32_t (*fsalloc_local)(uint32_t);
  fsalloc_local = (uint32_t (*)(uint32_t))specific_struct_->fsalloc_func;
  func_t kill_func = (func_t)specific_struct_->kill_func;
  uint8_t ios = specific_struct_->ios;
  //uint32_t num_files = io_struct->TAN

  /* Function Code */
  uint32_t buf=(uint32_t)fsalloc_local(512);
  //printf("\nfile location 1 %x",buf);
  sec_read_func(curr_port,curr_dir.dir->location/512,1,(DWORD)buf);
  Directory_t* dir=(Directory_t*)(buf);
  uint64_t cdir=dir->First_file;
  if(!cdir)
  {
    printf("\nThe folder is empty!!!\n");
    kill_func(); return;
  }
  sec_read_func(curr_port,cdir/512,1,(DWORD)buf);
  File_t* temp=(File_t*)(buf+(uint32_t)(cdir%512));
  for(int i=0;;i++)
  {
    if(!strcmp(temp->name,name))
      break;
    cdir=temp->Next_Friend;
    if(!cdir)
    {
      printf("\nFile not found\n");
      kill_func(); return;
    }
    sec_read_func(curr_port,cdir/512,1,(DWORD)buf);
    temp=(File_t*)(buf+(uint32_t)(cdir%512));
  }
  File_handle_t* handle=(File_handle_t*)fsalloc_local(sizeof(File_handle_t));
  File_t* tb=(File_t*)fsalloc_local(sizeof(File_t));
  memcpy((void*)tb,(void*)temp,sizeof(File_t));
  handle->file=(uint32_t)tb;
  handle->name=tb->name;
  //kfree((uint32_t*)buf);

  if(!handle)
  {
    printf("\nCant load the file requested\n");
    kill_func();
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
  //kill_func();
  //TODO: Spawn the original process which requested the file.
}

void FS_Read_Proc()
{
  SchedulerKits_t* kit = Get_Scheduler();
  /* Locations of all important structures */
  std_io_struct_t* io_struct_ = (std_io_struct_t*)(((Process_t*)((task_t*)kit->current_task)->process)->code + (4*((task_t*)kit->current_task)->task_id));
  parallels_i_struct_t* parallels_struct_ = (parallels_i_struct_t*)((std_io_struct_t*)io_struct_)->type_io_struct;
  FS_i_handle_t* specific_struct_ = (FS_i_handle_t*)((parallels_i_struct_t*)parallels_struct_)->specific_struct;
  //File_Header_t* (*File_Header_Creator_local)(File_t*, uint16_t);
  //File_Header_Creator_local = (File_Header_t* (*)(File_t*, uint16_t))specific_struct_->file_header_creator_local;
  uint32_t size = parallels_struct_->size;
  uint32_t buffer = (uint32_t)parallels_struct_->buffer;
  uint32_t offset = specific_struct_->offset;
  int (*sec_read_func)(HBA_PORT*, QWORD, DWORD, DWORD);
  sec_read_func = (int (*)(HBA_PORT*, QWORD, DWORD, DWORD))specific_struct_->read_sec_func;
  //int (*sec_write_func)(HBA_PORT*, QWORD, DWORD, DWORD);
  //sec_write_func = (int (*)(HBA_PORT*, QWORD, DWORD, DWORD))specific_struct_->write_sec_func;
  uint32_t (*fsalloc_local)(uint32_t);
  fsalloc_local = (uint32_t (*)(uint32_t))specific_struct_->fsalloc_func;
  func_t kill_func = (func_t)specific_struct_->kill_func;

  /* Function Code */
  File_handle_t* handle=start_handle;
  char* name = specific_struct_->file_name;
  for(int i=0;start_handle;i++)
  {
    if(!strcmp(handle->name,name))
    {
      goto out1;
    }
    if(!handle->next) break;
    handle=handle->next;
  }
  printf("\nFile %s not loaded yet!\n",name);
  //kill_func(); return;
  while(1);

  out1:
  if(!handle) {kill_func(); return;}
  uint32_t buf=buffer;
//   char* str2=(char*)buffer;
  File_t* file=(File_t*)handle->file;
  if(file->sz-file->headers*sizeof(File_Header_t)<offset)
  {
    printf("\nThe Requested offset is beyond the actual file size");
    kill_func();
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
  File_Header_t* header=(File_Header_t*)fsalloc_local(512);
  uint64_t tmp=file->first_header/512;
  uint32_t tmp2=0;
  uint32_t sz=(size);
  for(int i=0;;i++)
  {
    if(!header)
    {
      printf("\nThe offset is beyond the size of the file");
      kill_func();
      return;
    }
    sec_read_func(curr_port,tmp,1,(DWORD)header);
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
  uint32_t t=fsalloc_local(512);
  sec_read_func(curr_port,tmp+(off/512)+1,1,(DWORD)t);
  memcpy((void*)buf,(void*)(t+roff),MIN(sz,512-roff));
  if(sz<512-roff)
  {
    goto out2;
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
      sec_read_func(curr_port,tmp+1,MIN(blocks,temp),(DWORD)buf);
      buf+=MIN(blocks,temp)*512;
      --blocks;
    }
    if(!blocks)
    {
      goto out2;
    }
    tmp=header->Next_Header/512;
    sec_read_func(curr_port,tmp,1,(DWORD)header);
    temp=(header->spread*2)-1;
  }
  out2:
  printf("\nFile reads as: %s \nFile size: %x",buffer,file->sz);
  kill_func();
}

void FS_Write_Proc()
{
  SchedulerKits_t* kit = Get_Scheduler();
  /* Locations of all important structures */
  std_io_struct_t* io_struct_ = (std_io_struct_t*)(((Process_t*)((task_t*)kit->current_task)->process)->code + (4*((task_t*)kit->current_task)->task_id));
  parallels_o_struct_t* parallels_struct_ = (parallels_o_struct_t*)((std_io_struct_t*)io_struct_)->type_io_struct;
  FS_o_handle_t* specific_struct_ = (FS_o_handle_t*)((parallels_o_struct_t*)parallels_struct_)->specific_struct;
  File_Header_t* (*File_Header_Creator_local)(File_t*, uint16_t);
  File_Header_Creator_local = (File_Header_t* (*)(File_t*, uint16_t))specific_struct_->file_header_creator_local;
  uint32_t size = parallels_struct_->size;
  uint32_t buffer = (uint32_t)parallels_struct_->buffer;
  int (*sec_read_func)(HBA_PORT*, QWORD, DWORD, DWORD);
  sec_read_func = (int (*)(HBA_PORT*, QWORD, DWORD, DWORD))specific_struct_->read_sec_func;
  int (*sec_write_func)(HBA_PORT*, QWORD, DWORD, DWORD);
  sec_write_func = (int (*)(HBA_PORT*, QWORD, DWORD, DWORD))specific_struct_->write_sec_func;
  uint32_t (*fsalloc_local)(uint32_t);
  fsalloc_local = (uint32_t (*)(uint32_t))specific_struct_->fsalloc_func;
  func_t kill_func = (func_t)specific_struct_->kill_func;

  /* Function Code */
  File_handle_t* handle=start_handle;
  char* name = specific_struct_->file_name;
  for(int i=0;start_handle;i++)
  {
    if(!strcmp(handle->name,name))
    {
      goto out1;
    }
    if(!handle->next) break;
    handle=handle->next;
  }
  printf("\nFile %s not loaded yet!\n",name);
  //kill_func(); return;
  while(1);

  out1:
  if(!handle) {kill_func(); return;}
  File_t* file=(File_t*)handle->file;
  File_Header_t* header;
  uint32_t sz=size;
  uint32_t tbuf=buffer;
  uint64_t location;
  uint32_t blocks=sz/512;
  blocks++;
  if(!file->first_header)
  {
    header=File_Header_Creator_local(file,MIN((int)(blocks/2)+1,65535));
    location=header->location/512;
  }
  else
  {
    uint32_t buf=fsalloc_local(512);
    sec_read_func(curr_port,file->last_header/512,1,(DWORD)buf);
    header=(File_Header_t*)buf;
    location=header->location/512;
    uint32_t tmp1=(header->spread*1024)-header->used;
    uint32_t tmp2=1024-tmp1;//header->used-((header->spread-1)*1024);
    uint32_t tmb=fsalloc_local(1024);
    sec_read_func(curr_port,location+((header->spread-1)*2),2,(DWORD)tmb);
    memcpy((void*)(tmb+tmp2),(void*)(tbuf),MIN(tmp1,sz));
    sec_write_func(curr_port,location+((header->spread-1)*2),2,(DWORD)tmb);
    //kfree((uint32_t*)tmb);
    tbuf+=MIN(tmp1,sz);
    header->used+=MIN(tmp1,sz);
    sec_write_func(curr_port,location,1,(DWORD)buf);
    if(sz<=tmp1)
    {
      goto out2;
    }
    sz-=tmp1;
    blocks=sz/512;
    blocks++;
    header=File_Header_Creator_local(file,MIN((int)(blocks/2)+1,65535));
    location=header->location/512;
  }
  for(int i=0;;i++)
  {
    if(blocks<65535*2)
    {
      if(blocks>1)
      {
        sec_write_func(curr_port,location+1,(blocks)-1,(DWORD)tbuf);
        tbuf+=(blocks-1)*512;
      }
      uint32_t abuf=fsalloc_local(512);
      memcpy((void*)abuf,(void*)tbuf,sz-((blocks-1)*2));
      sec_write_func(curr_port,location+blocks,1,(DWORD)abuf);
      //kfree((uint32_t*)abuf);
      header->used+=sz;
      sec_write_func(curr_port,location,1,(DWORD)header);
      goto out2;
    }
    sec_write_func(curr_port,location+1,((65535)),(DWORD)tbuf);
    header->used+=65535;
    sec_write_func(curr_port,location,1,(DWORD)header);
    header=File_Header_Creator_local(file,MIN((int)(blocks/2)+1,65535));
    location=header->location/512;
    tbuf+=(65535)*1024;
    blocks-=(65535)*2;
    sz-=(67107850);
  }
  out2:
  file->sz+=size;
  kill_func();
}
