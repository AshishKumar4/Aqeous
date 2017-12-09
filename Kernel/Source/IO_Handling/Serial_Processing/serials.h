#ifndef SERIALS_h
#define SERIALS_h

typedef struct __attribute__((packed)) serials_i_struct
{
  uint32_t* buffer;
  uint32_t size;
  task_t* task;
  uint32_t reserved4;
  uint32_t reserved5;
  uint32_t reserved6;
  uint32_t reserved7;
  struct serials_i_struct* next;
}serials_i_struct_t;

typedef struct __attribute__((packed)) serials_o_struct
{
  uint32_t* buffer;
  uint32_t size;
  task_t* task;
  uint32_t reserved4;
  uint32_t reserved5;
  uint32_t reserved6;
  uint32_t reserved7;
  struct serials_o_struct* next;
}serials_o_struct_t;

void Serial_input(char* str, uint32_t length, uint32_t INPUT_FLAG, uint32_t task, uint32_t shell_kill, serials_i_struct_t** q_start, serials_i_struct_t** q_end, uint32_t* queue_counter);
void Serial_output(char* str, uint32_t length, uint32_t OUTPUT_FLAG, uint32_t task, uint32_t shell_kill, serials_o_struct_t** q_start, serials_o_struct_t** q_end, uint32_t* queue_counter);
#endif
