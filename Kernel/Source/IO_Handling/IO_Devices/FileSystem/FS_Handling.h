#ifndef FS_HANDLING_h
#define FS_HANDLING_h

/*********SPECIFIC STRUCTURES (specific_struct) for FILESYSTEM*********/

typedef struct __attribute__((packed)) FS_i_handle
{
  char* file_name;
  uint32_t* file_header_creator_local;
  uint32_t* read_sec_func;
  uint32_t* write_sec_func;
  uint32_t* fsalloc_func;
  uint32_t offset;
  uint32_t kill_func;
  uint32_t ios;
}FS_i_handle_t;

typedef struct __attribute__((packed)) FS_o_handle
{
  char* file_name;
  uint32_t* file_header_creator_local;
  uint32_t* read_sec_func;
  uint32_t* write_sec_func;
  uint32_t* fsalloc_func;
  uint32_t kill_func;
  uint32_t ios;
}FS_o_handle_t;

Process_t* FS_Handling_Process;

void FS_Process_Init();

//void test_save();

void FS_Load_File();

void FS_Read_Proc();

void FS_Write_Proc();

#endif
