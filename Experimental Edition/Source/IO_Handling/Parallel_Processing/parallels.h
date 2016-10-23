#ifndef PARALLELS_h
#define PARALLELS_h

typedef struct __attribute__((packed)) parallels_i_struct
{
  uint32_t* buffer;
  uint32_t size;
  task_t* task;
  func_t function;
  uint32_t reserved5;
  uint32_t reserved6;
  uint32_t specific_struct[16];
  struct fcontext_struct* fcontext;
}parallels_i_struct_t;

typedef struct __attribute__((packed)) parallels_o_struct
{
  uint32_t* buffer;
  uint32_t size;
  task_t* task;
  func_t function;
  uint32_t reserved5;
  uint32_t reserved6;
  uint32_t specific_struct[16];
  struct fcontext_struct* fcontext;
}parallels_o_struct_t;

#endif
