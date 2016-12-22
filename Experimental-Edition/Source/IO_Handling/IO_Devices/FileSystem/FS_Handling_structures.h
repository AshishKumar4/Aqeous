#ifndef FS_HANDLING_STRUCTURES_h
#define FS_HANDLING_STRUCTURES_h

#include "FS_Handling.h"
#include "parallels.h"
#include "std_iohandling.h"

std_io_struct_t* io_struct_ = (std_io_struct_t*)((Process_t*)((task_t*)current_task)->process)->code;
parallels_i_struct_t* parallels_struct_ = (parallels_i_struct_t*)((std_io_struct_t*)io_struct_)->type_io_struct;
FS_i_handle_t* specific_struct_ = (FS_i_handle_t*)((parallels_i_struct_t*)parallels_struct_)->specific_struct;
File_Header_t* (*File_Header_Creator_local)(File_t*, uint16_t);
File_Header_Creator_local = specific_struct_->file_header_creator_local;
uint32_t size = parallels_struct_->size;
uint32_t buffer = (uint32_t)parallels_struct_->buffer;
uint32_t offset = specific_struct_->offset;
int (*sec_read_func)(HBA_PORT*, QWORD, DWORD, DWORD);
sec_read_func = specific_struct_->read_sec_func;
int (*sec_write_func)(HBA_PORT*, QWORD, DWORD, DWORD);
sec_write_func = specific_struct_->write_sec_func;
uint32_t (*fsalloc_local)(uint32_t);
fsalloc_local = specific_struct_->fsalloc_func;
func_t kill_func = (func_t)specific_struct_->kill_func;

#endif
