#ifndef AQFSTOOLS_H
#define AQFSTOOLS_H 

#include "Aqfs.h"


typedef struct AqNoteEntrys
{
    AqLocation_t        location;
    uint32_t            size;
    uint32_t            next_off;
}AqNoteEntrys_t;


typedef struct AqNotes
{
    uint32_t            id;
    uint32_t            left;
    uint32_t            max;
    uint32_t            reserved;

    AqLocation_t        Next;
    AqLocation_t        Back;
    AqNoteEntrys_t      Notes[0];
}AqNotes_t;

int AqWrite(AqLocation_t* loc, uint32_t* buf, uint32_t blocks);
int AqRead(AqLocation_t* loc, uint32_t* buf, uint32_t blocks);

AqLocation_t* AqAlloc(uint32_t size);
int AqFree(AqLocation_t* loc, uint32_t size);
void AqAllocInit(uint32_t base, uint32_t disk_size);

inline int Aq_FindDepth(char* name);

inline int Aq_LocationCopy(AqLocation_t* d, AqLocation_t* s);
AqLocation_t* Aq_LocationCopyCreate(AqLocation_t* loc);
inline int Aq_LocationIncrement(AqLocation_t* loc, int size);

inline int Aq_DirSave_Dir(AqDirectory_t* dir);
inline int Aq_FileSave_File(AqFile_t* file);
inline int Aq_DirSave_Header(AqDhdr_t* hdr);
inline int Aq_FileSave_Header(AqFhdr_t* hdr);
inline int Aq_DirSave_Entry(AqDhdrEntry_t* entry);
inline int Aq_FileSave_Entry(AqFhdrEntry_t* entry);

inline int Aq_Save_MHeader(AqMainHeader_t* hdr);
inline int Aq_DirRead_Header(AqLocation_t* loc, AqDhdr_t* hdr);
inline int Aq_Save_MHeader(AqMainHeader_t* hdr);
inline int Aq_Save_MetaTable(AqAllocMeta_t* mm, int size);
inline int Aq_Save_All();

char* Aq_GetLname(char* path);

AqDhdr_t* Aq_GetDhdr_fromEntry(AqDhdrEntry_t* entry);
AqFhdr_t* Aq_GetFhdr_fromEntry(AqFhdrEntry_t* entry);

AqDhdr_t* Aq_DirCreate_NewHeader(AqDirectory_t* dir);
AqDhdrEntry_t* Aq_DirCreate_NewEntry(AqDirectory_t* dir, char* name, AqLocation_t* location);

int Aq_HandleGet_FileSize(AqHandle_t* handle);

AqDhdrEntry_t* Aq_DirGet_FreeEntry(AqDirectory_t* dir);
AqDhdrEntry_t* Aq_DirGet_Entry(AqDirectory_t* dir, char* str);
AqDhdrEntry_t* Aq_DirGet_FileEntry(AqDirectory_t* dir, char* str);
AqDhdr_t* Aq_DirGet_Header(AqLocation_t* l);
AqFhdr_t* Aq_FileGet_Header(AqLocation_t* l);
AqDirectory_t* Aq_DirGet_Parent(char* path);
AqDirectory_t* Aq_DirGet_Dir(char* path);

AqDhdrEntry_t* Aq_LoadEntry(char* path);

inline AqFile_t* Aq_HandleGet_File(AqHandle_t* handle);

AqFhdr_t* Aq_FileCreate_NewHeader(AqFile_t* file);
AqFhdrEntry_t* Aq_FileCreate_NewEntry(AqFile_t* file, AqLocation_t* location);
AqFhdrEntry_t* Aq_FileGet_FreeEntry(AqFile_t* file);
AqFhdrEntry_t* Aq_FileGet_Entry(AqFile_t* file, char* str);

int Aq_FileDelete_Headers(AqFile_t* file);

AqFhdr_t* Aq_FileGet_NextHeader(AqFhdr_t* fhdr);

AqFhdr_t* Aq_FileGet_Header_id_relative(AqFile_t* file, AqFhdr_t* fhdr, uint32_t id);
AqFhdrEntry_t* Aq_FileGet_NextEntry(AqFhdr_t* hdr, AqFhdrEntry_t* entry);
AqFhdrEntry_t* Aq_FileGet_EntryCA(AqFile_t* file, uint32_t address);
int Aq_File_LinkLastEntry(AqFile_t* file, AqFhdrEntry_t* entry);

#endif 