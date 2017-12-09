#include "streams.h"
#include "mem.h"
#include "vmem.h"

CircularSRB_header_t* CircularSRB_Setup(uint32_t entries)
{
	CircularSRB_header_t* nb_hr = malloc(sizeof(CircularSRB_header_t));
	nb_hr->head = phy_alloc4K();
	CircularSR_Buffer_t* tmp = nb_hr->head;
	CircularSR_Buffer_t* tt = tmp;
	++tmp;
	nb_hr->real_head = tmp;
	uint32_t te = entries;
	for(int i = 0; i<ROUNDUP(entries,4088)/(4088); i++)
	{
		for(int i = 0;i<MIN(te,4088);i++)
		{
			tmp->addr = (uint32_t)tmp;
			tmp->data = 0;
			++tmp;
		}
		te -= 4088;
		if(te > 4088)
		{
			tmp->addr = phy_alloc4k();
			tmp = tmp->addr;
		}
	}
	
	nb_hr->tail = (uint32_t*)tmp;
	nb_hr->curr_ptr = tmp;
	tmp->addr = nb_hr->real_head;
	tt->addr = (uint32_t*)(tmp-1);
}

inline void CSRB_push(CircularSRB_header_t* csrb_hr, uint32_t* data)
{
	CircularSR_Buffer_t* tmp = csrb_hr->tail;
	if((((uint32_t)tmp) + 4)%4096)
	{
		tmp->addr = tmp;
		tmp->data = data;
		++tmp;
		csrb_hr->tail = tmp;
		tmp->addr = csrb_hr->real_head;
	}
	else 
	{
		tmp->addr = phy_alloc4K();
		tmp = tmp->addr;
		tmp->data = data;
		++tmp;
		csrb_hr->tail = tmp;
		tmp->addr = csrb_hr->real_head;
	}
}

inline uint32_t* CSRB_pop(CircularSRB_header_t* csrb_hr)
{
	CircularSR_Buffer_t* tmp = csrb_hr->real_head;
	
}