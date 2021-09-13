#include "loader/loader.h"
#include "AqFS/fs.h"
#include "stdio.h"
#include "stdlib.h"
#include "common.h"
#include "MultiTasking/tasking.h"
#include "MultiTasking/process.h"
#include "ELF/Elf.h"


void process_bin(char* path)
{
// BinHeader_t* file = load_bin(path);

}
#include "processing.h"
#include "library.h"


Process_t* threadRipper_pop()
{
    if(!((int*)THREAD_GRAVEYARD[0])) return 0;
    Process_t* fr = THREAD_GRAVEYARD[(int)THREAD_GRAVEYARD[0]];
    THREAD_GRAVEYARD[(int)THREAD_GRAVEYARD[0]] = 0;
    --(THREAD_GRAVEYARD[0]);
    return fr;
}

void threadRipper_push(Process_t* fr)
{
    ++(THREAD_GRAVEYARD[0]);
    THREAD_GRAVEYARD[(int)THREAD_GRAVEYARD[0]] = fr;
}

void ThreadRipper_init()
{
    THREAD_GRAVEYARD = (Process_t**)phy_alloc4K();
    THREAD_GRAVEYARD[0] = (Process_t*)0;
}

void ThreadRipper()
{
    if(((int*)THREAD_GRAVEYARD[0]))
    {
        Delete_Process(threadRipper_pop());
    }
}