#ifndef _AQEOUSLIB_H_
#define _AQEOUSLIB_H_

typedef void (*func_t)();	//void function pointer
extern func_t printf;
extern uint32_t _syscall_(uint32_t, uint32_t);
extern void _syscall_push(uint32_t);

#endif
