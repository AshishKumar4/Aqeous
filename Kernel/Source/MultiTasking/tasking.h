#ifndef TASKING_h
#define TASKING_h

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "stdint.h"
#include "fs.h"
#include "MultiTasking/task.h"

//uint32_t QUEUE_START=33554432;//209715200
//uint32_t LAST_QUEUE=33636352;//209797120
#define TOTAL_QUEUES 20
const uint32_t TIME_MASK=0x000ff000;


void bxd();
void a();
uint32_t aad = 10;
void bxend();

void tbcd1();
void tbcd2();
void tbcd3();

void tvkc1();

uint32_t adeg = 0;

//func_t Shell_dbuff;

//uint32_t* volatile top_queue=(uint32_t*)33636352;
//volatile uint32_t reached_bottom=0,bottom_task=1;
int volatile cli_already = 0;
volatile int multitasking_ON = 0;

//uint32_t *bottom_queue = (uint32_t *)33636352;
volatile uint32_t sas_bottom_task = 1;

//task_t* current_task,*old_task;

//extern uint32_t current_task;
//extern uint32_t old_task;
extern uint32_t old_process;
extern uint32_t new_process;
extern uint32_t current_esp;
extern uint32_t old_esp;
//extern uint32_t time_slice;
extern void Scheduler_init_t();
extern void Scheduler_init_end_t();
extern void Scheduler_init();
void MP_init_Sequence();

void Spawner_Task();


void test_process();
void tasking_initiator();
void init_multitasking();

void Spurious_task_func_t();
void Spurious_task_func_end_t();

#endif
