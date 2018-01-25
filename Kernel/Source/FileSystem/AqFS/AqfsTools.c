#include "Aqfs.h"
#include "AqfsTools.h"
#include "AqfsAllocator.c"

inline int AqWrite(AqLocation_t* loc, uint32_t* buf, uint32_t blocks)
{
    if(!loc) return 0;
    if(!(loc->lower32) && !(loc->higher32)) return 0;
    return AqDirect_write(curr_port, loc->lower32, loc->higher32, blocks, (DWORD)buf);
}

inline int AqRead(AqLocation_t* loc, uint32_t* buf, uint32_t blocks)
{
    if(!loc) return 0;
    if(!(loc->lower32) && !(loc->higher32)) return 0;
    return AqDirect_read(curr_port, loc->lower32, loc->higher32, blocks, (DWORD)buf);
}

inline int Aq_FindDepth(char* name)
{
    int tt = 0;
    while(*name != '\0' || *name != 0)
    {
        if(*name == '/')
        {
            ++tt;
        }
        ++name;
    }
    return tt;
}

inline int Aq_LocationCopy(AqLocation_t* d, AqLocation_t* s)
{
    //memcpy_fast((void*)d, (void*)s, sizeof(AqLocation_t));
    d->lower32 = s->lower32;
    d->higher32 = s->higher32;
    return 1;
}

inline AqLocation_t* Aq_LocationCopyCreate(AqLocation_t* loc)
{
    AqLocation_t* ll = kmalloc(sizeof(AqLocation_t));
    Aq_LocationCopy(ll, loc);
    return ll;
}

inline int Aq_LocationIncrement(AqLocation_t* loc, int size)
{
    loc->lower32 += size;
    return 0;
}

inline int Aq_LocationCompare(AqLocation_t* l1, AqLocation_t* l2)
{
    if(l1->lower32 == l2->lower32 && l1->higher32 == l2->higher32) return 0;
    return 1;
}

inline int Aq_DirSave_Dir(AqDirectory_t* dir)
{
    return AqWrite(&(dir->location), (uint32_t*)dir, 1);
}

inline int Aq_FileSave_File(AqFile_t* file)
{
    return AqWrite(&(file->location), (uint32_t*)file, 1);
}

inline int Aq_DirSave_Header(AqDhdr_t* hdr)
{
    return AqWrite(&(hdr->location), (uint32_t*)hdr, AQ_DHEADER_SIZE/AQ_BLOCK_SIZE);
}

inline int Aq_FileSave_Header(AqFhdr_t* hdr)
{
    return AqWrite(&(hdr->location), (uint32_t*)hdr, AQ_FHEADER_SIZE/AQ_BLOCK_SIZE);
}

inline int Aq_Save_MHeader(AqMainHeader_t* hdr)
{
    return AqWrite(&(hdr->location), (uint32_t*)hdr, 2);
}

inline int Aq_Save_MetaTable(AqAllocMeta_t* mm, int size)
{
    return AqWrite(&(mm->location), (uint32_t*)mm, size);
}

inline int Aq_Save_All()
{
    Aq_Save_MHeader(AqfsMainFile->MainHeader_ptr);
    Aq_Save_MetaTable(AqfsMainFile->metaTable_ptr, AqfsMainFile->MainHeader_ptr->metaSpread);
    return 0;
}

inline int Aq_DirRead_Header(AqLocation_t* loc, AqDhdr_t* hdr)
{
    return AqRead((loc), (uint32_t*)hdr, AQ_DHEADER_SIZE/AQ_BLOCK_SIZE);
}

inline int Aq_DirSave_Entry(AqDhdrEntry_t* entry)
{
   // printf("\n %d; %d", entry, ROUNDDOWN((uint32_t)entry, AQ_DHEADER_SIZE));
    return AqWrite(&(((AqDhdr_t*)(ROUNDDOWN((uint32_t)entry, AQ_DHEADER_SIZE)))->location), (uint32_t*)((AqDhdr_t*)(ROUNDDOWN((uint32_t)entry, AQ_DHEADER_SIZE))), AQ_DHEADER_SIZE/AQ_BLOCK_SIZE);
}

inline int Aq_FileSave_Entry(AqFhdrEntry_t* entry)
{
    return AqWrite(&(((AqFhdr_t*)(ROUNDDOWN((uint32_t)entry, AQ_FHEADER_SIZE)))->location), (uint32_t*)((AqFhdr_t*)(ROUNDDOWN((uint32_t)entry, AQ_FHEADER_SIZE))), AQ_FHEADER_SIZE/AQ_BLOCK_SIZE);
}

inline char* Aq_GetLname(char* path)
{
   /* path += strlen(path);
    --path;
    while(*path != '\0' || *path != 0 || *path != '/')
    {
        --path;
    }
    return path+1;//*/
   /* char* a = strtok(NULL, '/');
  //  printf("\n%s %d %s", a, strlen(a), path);
    if(!strlen(a))
    {
        return path;
    }
    return a;
/*/
    int a = Aq_FindDepth(path);
    if (!a)	return path;
    char* cc = path;
    for (int i = 0; i < a;)
    {
        if (*cc == '/')	++i;
        ++cc;
    }
    ++cc;
    return cc;//*/
}

inline AqDhdr_t* Aq_GetDhdr_fromEntry(AqDhdrEntry_t* entry)
{
    AqDhdr_t* t = (AqDhdr_t*)(ROUNDDOWN((uint32_t)entry, AQ_DHEADER_SIZE));
    //printf("\n%d %d", t, entry);
    if(t->magic == AQ_DHR_MAGIC)
    {
        return t;
    }
    return NULL;
}

inline AqFhdr_t* Aq_GetFhdr_fromEntry(AqFhdrEntry_t* entry)
{
    AqFhdr_t* t = (AqFhdr_t*)(ROUNDDOWN((uint32_t)entry, AQ_FHEADER_SIZE));
    //printf("\n%d %d", t, entry);
 /*   if(t->magic == AQ_FHR_MAGIC)
    {
        return t;
    }*/
    return t;
}

inline int Aq_HandleGet_FileSize(AqHandle_t* handle)
{
    return Aq_HandleGet_File(handle)->info.data_size;
}

AqDhdr_t* Aq_DirCreate_NewHeader(AqDirectory_t* dir)
{
    AqDhdr_t* hdr = (AqDhdr_t*)pmem_4k(AQ_DHEADER_SIZE/4096);
    memset(hdr, 0, AQ_DHEADER_SIZE);
    hdr->magic = AQ_DHR_MAGIC;
    hdr->maxEntrys = (AQ_DHEADER_SIZE - sizeof(AqDhdr_t))/(sizeof(AqDhdrEntry_t));
    hdr->usedEntrys = 0;
    
    AqLocation_t* ll = AqAlloc(AQ_DHEADER_SIZE/AQ_BLOCK_SIZE);
    Aq_LocationCopy(&(hdr->location), ll);
    //printf("\n [%d, %d]", hdr->location.lower32, ll->lower32);
    Aq_LocationCopy(&(hdr->parentDir), &(dir->location));
    //hdr->NextDirHeader = NULL;
    AqDhdr_t* lhdr = Aq_DirGet_Header(&(dir->LastDirHeader));
    if(!lhdr)
    {
        Aq_LocationCopy(&(dir->FirstDirHeader), &(hdr->location));
    }
    else
    {
        Aq_LocationCopy(&(lhdr->NextDirHeader), &(hdr->location));
        Aq_DirSave_Header(lhdr);
    }
   // printf("\n [%d]", hdr->location.lower32);
    Aq_DirSave_Header(hdr);
    ++dir->headers;
    Aq_LocationCopy(&(dir->LastDirHeader), &(hdr->location));
   //// Aq_DirSave_Dir(dir);
    
   // printf("\nHeader created successfully...");
   return hdr;
}

AqDhdrEntry_t* Aq_DirCreate_NewEntry(AqDirectory_t* dir, char* name, AqLocation_t* location)
{
    AqDhdrEntry_t* entry = Aq_DirGet_FreeEntry(dir);
    if(!entry) return 0;
    entry->magic = AQ_DHR_MAGIC;
    Aq_LocationCopy(&(entry->location), location);
    strcpy(entry->name, name);
    Aq_DirSave_Entry(entry);    // Save the Directory header.
    return entry;
}

AqDhdrEntry_t* Aq_DirGet_FreeEntry(AqDirectory_t* dir)
{
    AqDhdr_t* hdr = (AqDhdr_t*)pmem_4k(AQ_DHEADER_SIZE/4096);
    uint32_t blks = AQ_DHEADER_SIZE/AQ_BLOCK_SIZE;
    if(!AqRead(&(dir->FirstDirHeader), (uint32_t*)hdr, blks)) 
    {
        printf("\nNo header found...");
        return 0;
    }
    if(hdr->magic != AQ_DHR_MAGIC)
    {
        printf("\nHeader corrupt...");
        return 0;
    }
    for(uint32_t i = 0; i < dir->headers; i++)
    {
        if(hdr->usedEntrys < hdr->maxEntrys)
        {
           // printf("\nYureka! %d %d", hdr->usedEntrys, hdr->maxEntrys);
            AqDhdrEntry_t* te = &(hdr->Entrys[0]);
            for(uint32_t j = 0; j < hdr->maxEntrys; j++)
            {
                if(te->magic != AQ_DHR_MAGIC)
                {
                    ++hdr->usedEntrys;
                    te->magic = AQ_DHR_MAGIC;
                    te->offset = j;
                   //// Aq_DirSave_Header(hdr);
                    return te;
                }
                ++te;
            }
            printf("\nError, Empty Entry exists but not found");
            return 0;
        }
        AqRead(&(hdr->NextDirHeader), (uint32_t*)hdr, blks);
    }
    // Create a New Header
    hdr = Aq_DirCreate_NewHeader(dir);
    ++hdr->usedEntrys;
    hdr->Entrys[0].magic = AQ_FHR_MAGIC;
    //// Aq_DirSave_Header(hdr);
    return (hdr->Entrys);
}

AqDhdrEntry_t* Aq_DirGet_Entry(AqDirectory_t* dir, char* str)
{
    uint32_t sl = (uint32_t)strlen(str);
    AqDhdr_t* hdr;
    uint32_t blks = AQ_DHEADER_SIZE/AQ_BLOCK_SIZE;
    if(dir->OpenHeader)     // There is a good possibility that a header is already open, why waste memory?
    {   
        // Just seach the string first in this open header
        // TODO: Some important stuff
        hdr = (AqDhdr_t*)dir->OpenHeader;
        if(hdr->magic == AQ_DHR_MAGIC)
        {
            // Search for the string 
            for(uint32_t i = 0; i < hdr->maxEntrys; i++)
            {
                if(!strncmp(hdr->Entrys[i].name, str, sl))
                {
                    return &(hdr->Entrys[i]);
                }
            }
        }
    }
    else
    {  
       hdr = (AqDhdr_t*)pmem_4k(AQ_DHEADER_SIZE/4096);
    }
    AqRead(&(dir->FirstDirHeader), (uint32_t*)hdr, blks);
    uint32_t tmp = (uint32_t)-1;
    for(uint32_t j = 0; j < dir->headers; j++)
    {
        /*if(hdr->magic != AQ_DHR_MAGIC)
        {
            return NULL;
        }*/
        if(tmp != hdr->id)
        {
            // Search for the string
            for(uint32_t i = 0; i < hdr->maxEntrys; i++)
            {
                if(!strncmp(hdr->Entrys[i].name, str, sl))
                {
                  //  printf("\n[%s]", hdr->Entrys[i].name);
                    return &(hdr->Entrys[i]);
                }
            }
        }
        AqRead(&(hdr->NextDirHeader), (uint32_t*)hdr, blks);
    }
    return NULL;
}


AqDhdrEntry_t* Aq_DirGet_FileEntry(AqDirectory_t* dir, char* str)
{
    uint32_t sl = (uint32_t)strlen(str);
    AqDhdr_t* hdr;
    uint32_t blks = AQ_DHEADER_SIZE/AQ_BLOCK_SIZE;
    if(dir->OpenHeader)     // There is a good possibility that a header is already open, why waste memory?
    {   
        // Just seach the string first in this open header
        // TODO: Some important stuff
        hdr = (AqDhdr_t*)dir->OpenHeader;
        if(hdr->magic == AQ_DHR_MAGIC)
        {
            // Search for the string 
            AqDhdrEntry_t* tmp = hdr->Entrys;
            for(uint32_t i = 0; i < hdr->maxEntrys; i++)
            {
                if(tmp->magic == AQ_DHR_MAGIC && (!strncmp(tmp->name, str, sl)) && tmp->type == AQ_FILE)
                {
                    return tmp;
                }
                ++tmp;
            }
        }
    }
    else
    {  
       hdr = (AqDhdr_t*)pmem_4k(AQ_DHEADER_SIZE/4096);
    }
    AqRead(&(dir->FirstDirHeader), (uint32_t*)hdr, blks);
    uint32_t tmp = (uint32_t)-1;
    for(uint32_t j = 0; j < dir->headers; j++)
    {
        /*if(hdr->magic != AQ_DHR_MAGIC)
        {
            return NULL;
        }*/
        if(tmp != hdr->id)
        {
            // Search for the string
            AqDhdrEntry_t* tmp = hdr->Entrys;
            for(uint32_t i = 0; i < hdr->maxEntrys; i++)
            {
                if(tmp->magic == AQ_DHR_MAGIC && (!strncmp(tmp->name, str, sl)) && tmp->type == AQ_FILE)
                {
                    return tmp;
                }
                ++tmp;
            }
        }
        AqRead(&(hdr->NextDirHeader), (uint32_t*)hdr, blks);
    }
    return NULL;
}

inline AqDhdr_t* Aq_DirGet_Header(AqLocation_t* l)
{
    AqDhdr_t* d = (AqDhdr_t*)pmem_4k(AQ_DHEADER_SIZE/4096);
    if(!AqRead(l, (uint32_t*)d, AQ_DHEADER_SIZE/AQ_BLOCK_SIZE)) return 0;
    return d;
}

inline AqFhdr_t* Aq_FileGet_Header(AqLocation_t* l)
{
    AqFhdr_t* d = (AqFhdr_t*)pmem_4k(AQ_FHEADER_SIZE/4096);
    if(!AqRead(l, (uint32_t*)d, AQ_FHEADER_SIZE/AQ_BLOCK_SIZE)) return 0;
    return d;
}

AqDirectory_t* Aq_DirGet_Parent(char* path)
{
    char pp[strlen(path)];
    strcpy(pp, path);

    int depth = Aq_FindDepth(pp);
    if(pp[0] == '/')
    {
        // Starts from root Directory
        --depth;
    }
    if(depth <= 0) 
    {
      //  printf("\nasds");
        return Aq_CurrentDir;
    }
  //  printf("\n<%d>", depth);

    char* tmp = strtok(pp, "/");
    
    AqDirectory_t* td = (AqDirectory_t*)kmalloc(AQ_BLOCK_SIZE);
    if(!AqRead(&(Aq_DirGet_Entry(Aq_CurrentDir, tmp)->location), (uint32_t*)td, AQ_BLOCKS_PER_SECTOR))
    {
        printf("\nError! 0x1");
        return 0;
    }
    for(uint32_t i = 1; i < depth; i++)
    {
        tmp = strtok(NULL, "/");
        if(!AqRead(&(Aq_DirGet_Entry(td, tmp)->location), (uint32_t*)td, AQ_BLOCKS_PER_SECTOR))
        {
            printf("\nError! 0x1");
            return 0;
        }
    }
    // TODO: CLOSE ALL OPEN HEADERS OF DIRECTORIES
    return td;
}

AqDirectory_t* Aq_DirGet_Dir(char* path)
{
    char pp[strlen(path)];
    strcpy(pp, path);

    int depth = Aq_FindDepth(pp);

    if(pp[0] == '/')
    {
        // Starts from root Directory
        --depth;
    }

    char* tmp = strtok(pp, "/");

    AqDirectory_t* td = (AqDirectory_t*)kmalloc(AQ_BLOCK_SIZE);
    AqRead(&(Aq_DirGet_Entry(Aq_CurrentDir, tmp)->location), (uint32_t*)td, AQ_BLOCKS_PER_SECTOR);
    for(uint32_t i = 0; i < depth; i++)
    {
        tmp = strtok(NULL, "/");
        AqRead(&(Aq_DirGet_Entry(td, tmp)->location), (uint32_t*)td, AQ_BLOCKS_PER_SECTOR);
    }
    // TODO: CLOSE ALL OPEN HEADERS OF DIRECTORIES
    return td;
}

inline AqDhdrEntry_t* Aq_LoadEntry(char* path)
{
    AqDirectory_t* parent = Aq_DirGet_Parent(path);
    AqDhdrEntry_t* entry = Aq_DirGet_Entry(parent, Aq_GetLname(path));
    return entry;
}

/*********************** FILE HANDLING STUFF ***************************/

inline AqFile_t* Aq_HandleGet_File(AqHandle_t* handle)
{
    return (AqFile_t*)(handle->descriptor);
}

AqFhdr_t* Aq_FileCreate_NewHeader(AqFile_t* file)
{
    AqFhdr_t* hdr = (AqFhdr_t*)pmem_4k(AQ_FHEADER_SIZE/4096);
    memset(hdr, 0, AQ_FHEADER_SIZE);
    hdr->magic = AQ_FHR_MAGIC;
    hdr->maxEntrys = (AQ_FHEADER_SIZE - sizeof(AqFhdr_t))/(sizeof(AqFhdrEntry_t));
    hdr->usedEntrys = 0;
    
    AqLocation_t* ll = AqAlloc(AQ_FHEADER_SIZE/AQ_BLOCK_SIZE);
    Aq_LocationCopy(&(hdr->location), ll);
    //printf("\n [%d, %d]", hdr->location.lower32, ll->lower32);
    Aq_LocationCopy(&(hdr->parentFile), &(file->location));
    //hdr->NextDirHeader = NULL;
    AqFhdr_t* lhdr = Aq_FileGet_Header(&(file->LastFileHeader));
    if(!lhdr)
    {
        Aq_LocationCopy(&(file->FirstFileHeader), &(hdr->location));
    }
    else
    {
        Aq_LocationCopy(&(lhdr->NextFileHeader), &(hdr->location));
        Aq_FileSave_Header(lhdr);
    }
   // printf("\n [%d]", hdr->location.lower32);
    Aq_FileSave_Header(hdr);
    ++file->headers;
    Aq_LocationCopy(&(file->LastFileHeader), &(hdr->location));
   //// Aq_FileSave_File(file);
    
   // printf("\nHeader created successfully...");
   return hdr;
}

AqFhdrEntry_t* Aq_FileCreate_NewEntry(AqFile_t* file, AqLocation_t* location)
{
    AqFhdrEntry_t* entry = Aq_FileGet_FreeEntry(file);
    if(!entry) return 0;
   // entry->offset = offset;
    Aq_LocationCopy(&(entry->location), location);
    Aq_FileSave_Entry(entry);    // Save the Directory header.
    return entry;
}

AqFhdrEntry_t* Aq_FileGet_FreeEntry(AqFile_t* file)
{
    AqFhdr_t* hdr = (AqFhdr_t*)pmem_4k(AQ_FHEADER_SIZE/4096);
    uint32_t blks = AQ_FHEADER_SIZE/AQ_BLOCK_SIZE;
    if(!AqRead(&(file->FirstFileHeader), (uint32_t*)hdr, blks)) 
    {
        printf("\nNo header found...");
        return 0;
    }
    if(hdr->magic != AQ_FHR_MAGIC)
    {
        printf("\nHeader corrupt...");
        return 0;
    }
    for(uint32_t i = 0; i < file->headers; i++)
    {
        if(hdr->usedEntrys < hdr->maxEntrys)
        {
           // printf("\nYureka! %d %d", hdr->usedEntrys, hdr->maxEntrys);
            AqFhdrEntry_t* te = &(hdr->Entrys[0]);
            for(uint32_t j = 0; j < hdr->maxEntrys; j++)
            {
                if(te->magic != AQ_FHR_MAGIC)
                {
                    ++hdr->usedEntrys;
                    te->magic = AQ_FHR_MAGIC;
                    te->id = j;
                    //AqWrite(hdr->location, (uint32_t*)hdr, blks);
               ////     Aq_FileSave_Header(hdr);
                    return te;
                }
                ++te;
            }
            printf("\nError, Empty Entry exists but not found");
            return 0;
        }
        AqRead(&(hdr->NextFileHeader), (uint32_t*)hdr, blks);
    }
    // Create a New Header
    hdr = Aq_FileCreate_NewHeader(file);
    ++hdr->usedEntrys;
    hdr->Entrys[0].magic = AQ_FHR_MAGIC;
 ////   Aq_FileSave_Header(hdr);
    return (hdr->Entrys);
}

int Aq_FileDelete_Headers(AqFile_t* file)
{
    AqFhdr_t* hdr = (AqFhdr_t*)pmem_4k(AQ_FHEADER_SIZE/4096);
    uint32_t blks = AQ_FHEADER_SIZE/AQ_BLOCK_SIZE;
    if(!AqRead(&(file->FirstFileHeader), (uint32_t*)hdr, blks)) 
    {
        printf("\nNo header found...");
        return -1;
    }
    
    for(uint32_t i = 0; i < file->headers; i++)
    {
        AqFhdrEntry_t* te = &(hdr->Entrys[0]);
        for(uint32_t j = 0; j < hdr->maxEntrys; j++)
        {
            if(te->magic == AQ_FHR_MAGIC)
            {
                AqFree(&(te->location), te->spread);
                te->magic = 0;  // Mark this entry free
            }
            ++te;
        }
        AqLocation_t tmp;
        tmp.lower32 = hdr->location.lower32;
        tmp.higher32 = hdr->location.higher32;
        AqFree(&(hdr->location), blks);
        if(!AqRead(&(tmp), (uint32_t*)hdr, blks)) return -2;
    }
    return 0;
}

AqFhdr_t* Aq_FileGet_NextHeader(AqFhdr_t* fhdr)
{
    if(!AqRead(&(fhdr->NextFileHeader), (uint32_t*)fhdr, AQ_FHEADER_SIZE/AQ_BLOCK_SIZE))    return NULL;
    return fhdr;
}
/*
AqFhdr_t* Aq_FileGet_Header_id_relative(AqFile_t* file, AqFhdr_t* fhdr, uint32_t id)
{
    if(id >= fhdr->id)   // We have to traverse Forward
    {
        while(fhdr->id < id)
        {
            Aq_FileGet_NextHeader(fhdr);
        }
        return fhdr;
    }
    // Start from the begining
    if(!AqRead(&(file->FirstFileHeader), (uint32_t*)fhdr, AQ_FHEADER_SIZE/AQ_BLOCK_SIZE)) return NULL;
    while(fhdr->id < id)
    {
        Aq_FileGet_NextHeader(fhdr);
    }
    return fhdr;
}
*/
AqFhdrEntry_t* Aq_FileGet_NextEntry(AqFhdr_t* hdr, AqFhdrEntry_t* entry)
{
    if(hdr->id == ((AqFhdr_t*)(ROUNDDOWN((uint32_t)entry, AQ_FHEADER_SIZE)))->id)       // Next Entry exists in this header itself!
    {
        return &(hdr->Entrys[entry->nextOff]);
    }
    else                                // Next Entry in some other header
    {
        AqFhdr_t* tmpHdr = Aq_FileGet_Header(&entry->NextHdr);//Aq_FileGet_Header_id_relative(file, hdr, entry->nextHdr);
        if(!tmpHdr)
        {
            printf("\n{%d:%d}, ", entry->id, entry->NextHdr.lower32);
            return NULL;
        }
        return &(tmpHdr->Entrys[entry->nextOff]);
    }
}

AqFhdrEntry_t* Aq_FileGet_EntryCA(AqFile_t* file, uint32_t address)     // Entry containing address
{
    uint32_t raddress = 0;
    
    AqFhdr_t* hdr = (AqFhdr_t*)pmem_4k(AQ_FHEADER_SIZE/4096);
    AqRead(&(file->FirstFileHeader), (uint32_t*)hdr, AQ_FHEADER_SIZE/AQ_BLOCK_SIZE);
    AqFhdrEntry_t* tmp = hdr->Entrys;
    while(tmp)
    {
        if(tmp->magic != AQ_FHR_MAGIC)  break;
        if(raddress + tmp->used > address)
        {
            tmp->offset = address - raddress;   // Relative offset into the entry
            return tmp;
        }
        raddress += tmp->used;
        tmp = Aq_FileGet_NextEntry(hdr, tmp);
    }

    printf("\nFile Corrupt!");
    return NULL;
}

int Aq_File_LinkLastEntry(AqFile_t* file, AqFhdrEntry_t* entry)
{
    AqFhdr_t* hdr = Aq_FileGet_Header(&(file->lastEntryHdr));
    if(!hdr)
    {
        printf("\nSERIOUS TROUBLE!!!");
        return 1;
    }
    hdr->Entrys[file->lastEntryOff].nextOff = entry->id;
    Aq_LocationCopy(&(hdr->Entrys[file->lastEntryOff].NextHdr), &(Aq_GetFhdr_fromEntry(entry)->location));
    Aq_FileSave_Header(hdr);
}