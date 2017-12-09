#ifndef PROCESSING_H
#define PROCESSING_H

#include "ELF/Elf.h"

Process_t** THREAD_GRAVEYARD;

Process_t* threadRipper_pop();
void threadRipper_push(Process_t* fr);
void ThreadRipper_init();
void ThreadRipper();

#endif
