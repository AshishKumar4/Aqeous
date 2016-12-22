#ifndef MMANAGER_SYS_h
#define MMANAGER_SYS_h

uint32_t* mmads_stack_end,* mmads_stack_start, mmads_stack_size = 0;

uint32_t* MMADS_Process, *MMADS_csrbTweakerThread;

void Setup_MMADS();
void MMADS_csrbTweaker();

#endif
