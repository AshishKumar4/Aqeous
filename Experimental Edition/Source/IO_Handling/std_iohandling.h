#ifndef STD_IOHANDLING_h
#define STD_IOHANDLING_h

typedef struct __attribute__((packed)) std_io_struct
{
  uint32_t TAN; // Type and Number : 0-27 bits -> Number of files. 28,29,30,31 bits -> Type (Input/Output)
  task_t* task;
  uint32_t type_io_struct[24];
}std_io_struct_t;

typedef struct __attribute__((packed)) fcontext_struct
{
  uint32_t entries[24];
}fcontext_struct_t;

uint32_t std_Qalloc();

enum IO_HANDLES_FLAGS
{
  STD_INPUT_FLAG      =        0b00000000000000000000000000000001,
  STD_OUTPUT_FLAG     =        0b00000000000000000000000000000000,
  FS_INPUT_FLAG       =        0b00000000000000000000000000000111,
  FS_OUTPUT_FLAG      =        0b00000000000000000000000000000110,
  KB_INPUT_FLAG       =        0b00000000000000000000000000000101,
  NUM_FILES_FLAG      =        0b11111111111111111111111111110000,
};

#endif
