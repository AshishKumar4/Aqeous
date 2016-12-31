#include "kb_handle.h"
#include "queues.h"

uint32_t ax_add = 610*1024*1024;

uint32_t std_Qalloc()
{
  return kmalloc(sizeof(std_io_struct_t));
}

/*
uint32_t kb_Qalloc()
{
   return kmalloc(32);
}
*/
