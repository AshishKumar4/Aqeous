#ifndef TASKING_h
#define TASKING_h

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "phy_mm\mem.h"
#include "virt_mm\vmem.h"
#include "virt_mm\paging.h"
#include "stdint.h"
#include "fs.h"

#include "task.h"

#define QUEUE_START 33554432//209715200
#define LAST_QUEUE 33636352//209797120
#define TOTAL_QUEUES 20
const uint32_t TIME_MASK=0x000ff000;

//func_t Shell_dbuff;

uint32_t* volatile top_queue=(uint32_t*)LAST_QUEUE;
volatile uint32_t reached_bottom=0,bottom_task=1;
int volatile cli_already = 0;
volatile int multitasking_ON = 0;

uint32_t *bottom_queue = (uint32_t *)LAST_QUEUE;
volatile uint32_t sas_bottom_task = 1;

volatile uint32_t last_highest_priority = 0;
volatile uint32_t sas_cleaning_complete = 1;
volatile uint32_t* highest_task_entry = 0;

//task_t* current_task,*old_task;

extern uint32_t current_task;
extern uint32_t old_task;
extern uint32_t old_process;
extern uint32_t new_process;
extern uint32_t current_esp;
extern uint32_t old_esp;
extern uint32_t time_slice;
extern void Scheduler_init();

#endif
