#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "stdint.h"

#define _SYSCALL_()                       asm volatile("int $54;")      

#define __SYS_STDO__                0
#define __SYS_MALLOC__              1
#define __SYS_FREE__                2
#define __SYS_REALLOC__             3
#define __SYS_SLEEP__               4
#define __SYS_EXIT__                5
#define __SYS_WAIT__                6
#define __SYS_OPEN__                7
#define __SYS_WRITE__               8
#define __SYS_READ__                9
#define __SYS_CLOSE__               10
#define __SYS_FORK__                11
#define __SYS_EXEC__                12
#define __SYS_GETPID__              13
#define __SYS_MKDIR__               14
#define __SYS_CHDIR__               15
#define __SYS_STDI__                16

#define __SYS_AQ_FOPEN__            17
#define __SYS_AQ_FCLOSE__           18
#define __SYS_AQ_FSEEK__            19
#define __SYS_AQ_FTELL__            20
#define __SYS_AQ_FREAD__            21
#define __SYS_AQ_FWRITE__           22

#define __SYS_AQ_CLONETHREAD__      23
#define __SYS_AQ_CLONEPROCESS__     24


uint32_t _syscall_pop();
void _syscall_push(uint32_t fr);
uint32_t _syscall_(uint32_t call);

#endif
