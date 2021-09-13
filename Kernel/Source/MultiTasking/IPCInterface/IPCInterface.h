#ifndef IPCINTERFACE_h
#define IPCINTERFACE_h

#include "MultiTasking/task.h"

#define TASK_PRIORITY_REFRESH 1
#define TASK_ACTIVATE         2
#define TASK_SLEEP            3
#define TASK_WAKEUP           4
#define TASK_PRIORITY_CHANGE  5
#define TASK_REFRESH          6
#define TASK_KILL             7
#define TASK_SWAP             8

typedef struct IPCPacket
{
  int type;
  SchedulerKits_t* source;
  SchedulerKits_t* destination;
  task_t* task;
  uint32_t no_opt;
  uint32_t options[4];
  uint32_t done;
  struct IPCPacket* next;
}IPCPacket_t;

typedef struct IPCRecord
{
  uint32_t entries;
  uint32_t lock;

  func_t IPCprocessorP;
  task_t* IPCtask;

  IPCPacket_t* start;
  IPCPacket_t* last;
}IPCRecord_t;

void IPC_init();

void IPCdispatch(IPCPacket_t* pp);

void IPCprocessorP_t();

void IPCprocessorP_end_t();

#endif
