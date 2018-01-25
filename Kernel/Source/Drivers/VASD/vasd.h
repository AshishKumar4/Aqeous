#ifndef VASD_H
#define VASD_H 

#include "common.h"
#include "stdlib.h"

typedef struct vasd_desc_busPCI
{
    uint16_t     type;

}vasd_desc_busPCI_t;

typedef struct vasd_device
{
    uint8_t     controller_type;
    uint8_t     interface_type;
    uint8_t     storage_type;
    uint8_t     flags;
    char        name[4];
    uint32_t    storage_size;
    
    uintptr_t   bus_desc;
    uintptr_t   interface_desc;
    uintptr_t   device_desc;
}vasd_device_t;

#endif