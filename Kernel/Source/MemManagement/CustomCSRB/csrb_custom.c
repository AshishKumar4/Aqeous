#include "streams.h"
#include "mem.h"
#include "vmem.h"

/*
    CUSTOM M CSRB STRUCTURE:
    we start here -> [ENTRIES][(free/used csrb?) (frame of other csrb)][TAIL_ADDRESS][HEAD_ADDRESS][<1st entry>][<data>].....[<last_entry>][<data>][<1st entry>][blank]
*/

inline uint32_t* Custom_M_CSRB_make(uint32_t entries)
{
    uint32_t* new_buff = phy_alloc4K(); //Allocate 4kb space.
    uint32_t* bb = new_buff;    //Start of CSRB
    *new_buff = entries;    //Entries
    ++new_buff; 
    ++new_buff; //Reserved 
    uint32_t* tail = new_buff;
    ++new_buff; 
    *new_buff = (uint32_t)new_buff + 4;
    ++new_buff;
    CircularSR_Buffer_t* tmp = new_buff;
    for(int i = 0; i < entries; i++)
    {
        tmp->addr = (uint32_t*)tmp;
        ++tmp;
    }
    --tmp;
    *tail = (uint32_t*)tmp; 
    ++tmp;
    tmp->addr = new_buff;   //Make last one to point to first one.
    return bb;
}
