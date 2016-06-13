#include "kb_queue.h"
#include "queues.h"

uint32_t ax_add = 610*1024*1024;

uint32_t kb_Qalloc()
{
  uint32_t a = ax_add;
  ax_add+=32;
  return a;
}
