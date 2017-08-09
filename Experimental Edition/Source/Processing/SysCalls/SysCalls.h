#ifndef SYSCALL_H
#define SYSCALL_H

uint32_t* SYSCALL_SPACE, *SYSCALL_SPACE_top;
uint32_t SYSCALL_SPACE_bottom;

inline uint32_t _syscall_pop();
inline void _syscall_push(uint32_t fr);

void syscall_init();
void syscall_MapPdir(PageDirectory_t* pdir);

#endif
