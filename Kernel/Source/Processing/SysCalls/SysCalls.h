#ifndef SYSCALL_H
#define SYSCALL_H

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


#define IA32_SYSENTER_CS        0x174
#define IA32_SYSENTER_ESP       0x175
#define IA32_SYSENTER_EIP       0x176

extern void syscall_vector_t();
extern void syscall_vector_end_t();

uint32_t *SYSCALL_SPACE_bottom;

uint32_t _syscall_pop(uint32_t* SYSCALL_SPACE);
void _syscall_push(uint32_t* SYSCALL_SPACE, uint32_t fr);

void syscall_init();
void syscall_MapPdir(PageDirectory_t* pdir);

void syscall_handler();

#endif
