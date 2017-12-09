#include "Aqfs.h"
#include "AqfsTools.c"
#include "library.h"

int Aqfs2_InitStage1()
{
    printf("\nAttempting Initialization of AqFS 2...");
    SATA_ident_t* info=(SATA_ident_t*)curr_disk->info;
    AqfsMainFile = (AqVolatileFile_t*)phy_alloc4K();
    AqfsMainFile->magic = AQ_VFILE_MAGIC;
    AqfsMainFile->sectors = info->lba_capacity;
    AqfsMainFile->bytesPerSector = 512;
    AqfsMainFile->size = AqfsMainFile->sectors*AqfsMainFile->bytesPerSector;
    return 0;
}

int Aqfs2_InitStage2()
{
    Aq_CurrentDir = (AqDirectory_t*)kmalloc(AQ_BLOCK_SIZE);
    AqRead(&(AqfsMainFile->MainHeader_ptr->rootDir), (uint32_t*)Aq_CurrentDir, 1);
    AqfsMainFile->current_dir = Aq_CurrentDir;
    AqfsMainFile->root_dir = (AqDirectory_t*)kmalloc(sizeof(AqDirectory_t));
    memcpy_fast(AqfsMainFile->root_dir, Aq_CurrentDir, sizeof(AqDirectory_t));
    printf("\nRoot Dir location: %d, %d", Aq_CurrentDir->location.lower32, AqfsMainFile->MainHeader_ptr->rootDir);
    return 0;
}

int Aq_CreateRoot()
{
    AqDirectory_t* newD = (AqDirectory_t*)kmalloc(AQ_BLOCK_SIZE);
    AqDirectory_t* parent = newD;       // Root is parent of itself
    memset_fast((void*)newD, 0, AQ_BLOCK_SIZE);
    /*  Create the new file structure   */
    newD->magic = AQ_DIR_MAGIC;
    newD->headers = 0;
    newD->info.data_size = 0;
    newD->info.onDisk_size = sizeof(AqDirectory_t);
    newD->info.perm = 4;
    newD->info.type = AQ_DIR;

    Aq_LocationCopy(&(newD->location), AqAlloc(1));
    Aq_LocationCopy(&(newD->parentDir), &(parent->location));

    /*  Put the structure details in Parent Directory   */

    Aq_DirCreate_NewHeader(newD);
    AqDhdrEntry_t* entry = Aq_DirCreate_NewEntry(parent, "root", &(newD->location));
    entry->type = AQ_DIR;
    newD->dhdr = Aq_GetDhdr_fromEntry(entry)->location;
    newD->dhdrOffset = entry->offset;
    Aq_DirSave_Entry(entry);    // Save the Directory header.

    entry = Aq_DirCreate_NewEntry(parent, "..", &(newD->location));
    entry->type = AQ_DIR;


    Aq_DirSave_Dir(newD);               // Save the file structure on hard disk
    ++parent->n_dir;
    Aq_DirSave_Dir(parent);
    AqMainHeader_t* mhd = AqfsMainFile->MainHeader_ptr;
    Aq_LocationCopy(&(mhd->rootDir), &(newD->location));
    Aq_Save_MHeader(mhd);

    kfree(newD);
    return 0;
}

int Aqfs2_Checkfs()
{
    Aqfs2_InitStage1();

    uint32_t buf = (uint32_t)kmalloc(1024);
    AqDirect_read(curr_port, 0, 0, 2, buf);     // Read first 1024 bytes.

    AqMBR_Meta_t* identity=(AqMBR_Meta_t*)(buf + 436);  
	if(strncmp(identity->name, "AqFS2",5))
	{
		printf("\nFilesystem Not supported/Disk not partitioned Correctly %s", identity->name);
		return 1;
    }
    printf("\nAqfs initialising...");
    AqPartitionMeta_t* pp = &(identity->partitions[0]);
    for(int i = 0; i < 4; i++)  // Get partition
    {
        if(pp->sys_id == AQFS2_SYSID)
            break;
        ++pp;
    }
    AqLocation_t vl;
    vl.lower32 = pp->relative_sector;
    vl.higher32 = 0;
    
    AqMainHeader_t* mb = (AqMainHeader_t*)buf;
    AqRead(&vl, (uint32_t*)buf, 2);
    AqfsMainFile->MainHeader_ptr = mb;
    AqfsMainFile->metaTable_ptr = (AqAllocMeta_t*)pmem_4k(((mb->metaSpread * 512)/4096) + 1);
    AqRead(&mb->metaTable, (uint32_t*)AqfsMainFile->metaTable_ptr, mb->metaSpread);

    Aqfs2_InitStage2();
   // Aq_ListEntrys("root");
    return 0;
}

int Aqfs2_Partitioner(int partitionId, int start, int size)
{
    printf("\nPartitioning..."); 
    uint32_t buf = (uint32_t)kmalloc(1024);
    AqDirect_read(curr_port, 0, 0, 2, buf);     // Read first 1024 bytes.
    AqMBR_Meta_t* identity=(AqMBR_Meta_t*)(buf + 436); 

    AqPartitionMeta_t* pp = &(identity->partitions[partitionId]);
    pp->sys_id = AQFS2_SYSID;       // Create the partition
    pp->start = start;
    pp->end = start + size;
    pp->relative_sector = start;
    pp->total_sectors = size;
    
    identity->bootBytes[0] = 0x55;
    identity->bootBytes[1] = 0xAA;
    AqDirect_write(curr_port, 0, 0, 2, buf);     // Read first 1024 bytes.
    
    kfree(buf);
    return 0;
}

int Aqfs2_burn(int partitionId)
{
    Aqfs2_InitStage1();
    printf("\nPutting AqFS on Partition ID %d...", partitionId);

    uint32_t buf = (uint32_t)kmalloc(1024);
    AqDirect_read(curr_port, 0, 0, 2, buf);     // Read first 1024 bytes.
    AqMBR_Meta_t* identity=(AqMBR_Meta_t*)(buf + 436); 

    strcpy(identity->name, "AqFS2");

    AqPartitionMeta_t* pp = &(identity->partitions[partitionId]);
    pp->sys_id = AQFS2_SYSID;       // Create the partition
    int tsize = pp->total_sectors;
    if(!pp->relative_sector)
    {
        printf("\nPartition not readable!");
        return 1;
    }
    AqDirect_write(curr_port, 0, 0, 2, buf);     // Read first 1024 bytes.
    AqLocation_t vl;
    vl.lower32 = pp->relative_sector;
    vl.higher32 = 0;

    AqMainHeader_t* mb = (AqMainHeader_t*)buf;
    memset((void*)buf, 0, 1024);
    strcpy(mb->name, "AqFS2");
    mb->version = 1;
    mb->magic = AQ_MHDR_MAGIC;
    mb->metaTable.higher32 = 0;
    mb->metaTable.lower32 = vl.lower32 + 2;     // Meta table stored 2 sectors laters
    mb->location.lower32 = vl.lower32;
    mb->location.higher32 = 0;

    // Formula: each meta entry -> 512 bytes -> Each alloc table -> AQ_ALLOCTABLE_MAX_ENTRIES*512 bytes; 
    // tsize -> number of sectors; tsize/(AQ_ALLOCTABLE_MAX_ENTRIES*512) -> total alloc tables possible -> total entrys.

    mb->metaSpread = (((tsize/(AQ_ALLOCTABLE_MAX_ENTRIES*AQ_SECTOR_SIZE)) + sizeof(AqAllocMeta_t))/AQ_SECTOR_SIZE) + 1;  
    AqWrite(&vl, (uint32_t*)buf, 2);


    AqfsMainFile->MainHeader_ptr = (AqMainHeader_t*)buf;
    AqfsMainFile->metaTable_ptr = (AqAllocMeta_t*)pmem_4k(((mb->metaSpread * 512)/4096) + 1);
    AqfsMainFile->metaTable_ptr->location.lower32 = mb->metaTable.lower32;
    AqfsMainFile->metaTable_ptr->location.higher32 = mb->metaTable.higher32;
    AqAllocInit(vl.lower32 + 2 + ((mb->metaSpread)), tsize); // Initialize Disk Memory Management
    Aq_CreateRoot();
    Aq_Save_All();

    printf("\nPartition Successfully burned with AqFS 2...\nCreating Default Dirs...");
    Aqfs2_InitStage2();
    Aq_CreateNew_Directory("Home", 4);
    Aq_CreateNew_Directory("System", 4);
    Aq_CreateNew_Directory("Aqeous", 4);
    Aq_CreateNew_Directory("etc", 4);
    Aq_ListEntrys_direct(Aq_CurrentDir);
    return 0;
}

int Aq_CreateNew_File(char* name, uint32_t perm)
{
    AqDirectory_t* parent = Aq_DirGet_Parent(name);
    AqFile_t* newF = (AqFile_t*)kmalloc(AQ_BLOCK_SIZE);
    memset_fast((void*)newF, 0, sizeof(AqFile_t));
    /*  Create the new file structure   */
    newF->magic = AQ_FIL_MAGIC;
    newF->headers = 0;
    newF->info.data_size = 0;
    newF->info.onDisk_size = sizeof(AqFile_t);
    newF->info.perm = perm;
    newF->info.type = AQ_FILE;

    Aq_LocationCopy(&(newF->location), AqAlloc(1));
    Aq_LocationCopy(&(newF->parentDir), &(parent->location));
    /*  Put the structure details in Parent Directory   */
    //AqDhdrEntry_t* entry = Aq_DirCreate_NewEntry(parent, Aq_GetLname(name), &(newF->location));

    AqDhdrEntry_t* entry = Aq_DirGet_FreeEntry(parent);
    if(!entry) return 0;
    entry->magic = AQ_DHR_MAGIC;
    Aq_LocationCopy(&(entry->location), &(newF->location));
    printf("\n[{%d}]", entry->location.lower32);
    strcpy(entry->name, Aq_GetLname(name));

    entry->type = AQ_FILE;
    Aq_DirSave_Entry(entry);    // Save the Directory header.

    newF->dhdr = Aq_GetDhdr_fromEntry(entry)->location;
    newF->dhdrOffset = entry->offset;

    Aq_FileSave_File(newF);    // Save the file structure on hard disk
    ++parent->n_files;
    Aq_DirSave_Dir(parent);
    
    kfree(newF);    
    return 0;
}

int Aq_CreateNew_Directory(char* name, uint32_t perm)
{
    AqDirectory_t* parent = Aq_DirGet_Parent(name);
    if(!parent) return 0;
   // printf("\n[{%d}]", parent->location.lower32);
    AqDirectory_t* newD = (AqDirectory_t*)kmalloc(AQ_BLOCK_SIZE);
    memset_fast((void*)newD, 0, sizeof(AqDirectory_t));
    /*  Create the new file structure   */
    newD->magic = AQ_DIR_MAGIC;
    newD->headers = 0;
    newD->info.data_size = 0;
    newD->info.onDisk_size = sizeof(AqDirectory_t);
    newD->info.perm = perm;
    newD->info.type = AQ_DIR;

    Aq_LocationCopy(&(newD->location), AqAlloc(1));
    Aq_LocationCopy(&(newD->parentDir), &(parent->location));
    /*  Put the structure details in Parent Directory   */
  //  printf("\n[%s]", Aq_GetLname(name));

    //AqDhdrEntry_t* entry = Aq_DirCreate_NewEntry(parent, Aq_GetLname(name), &(newD->location));
    AqDhdrEntry_t* entry = Aq_DirGet_FreeEntry(parent);
    if(!entry) return 0;
    entry->magic = AQ_DHR_MAGIC;
    Aq_LocationCopy(&(entry->location), &(newD->location));
    strcpy(entry->name, Aq_GetLname(name));
    entry->type = AQ_DIR;
    Aq_DirSave_Entry(entry);    // Save the Directory header.
    
    newD->dhdr = Aq_GetDhdr_fromEntry(entry)->location;
    newD->dhdrOffset = entry->offset;
    Aq_DirCreate_NewHeader(newD);

    Aq_DirCreate_NewEntry(newD, ".", &(AqfsMainFile->root_dir->location))->type = AQ_DIR;     // Make link to Root
    Aq_DirCreate_NewEntry(newD, "..", &(parent->location))->type = AQ_DIR;     // Make link to Root

    Aq_DirSave_Dir(newD);  // Save the file structure on hard disk
    ++parent->n_dir;
    Aq_DirSave_Dir(parent);
                 
    kfree(newD);
    return 0;
}

int Aq_Delete_File(char* path)
{
    AqDirectory_t* parent = Aq_DirGet_Parent(path);
    AqDhdrEntry_t* entry = Aq_DirGet_FileEntry(parent, Aq_GetLname(path));
    AqFile_t* file = (AqFile_t*)kmalloc(512);
    if(!AqRead(&(entry->location), (uint32_t*)file, AQ_BLOCK_SIZE/AQ_SECTOR_SIZE)) return -1;

    // TODO: Delete and free all file headers and entrys
    Aq_FileDelete_Headers(file);

    // TODO: Delete File's Entry in parent table
    entry->magic = 0;
    AqDhdr_t* hdr = Aq_GetDhdr_fromEntry(entry);
    --hdr->usedEntrys;
    Aq_DirSave_Header(hdr);
    kfree(hdr);
    --parent->n_files;
    Aq_DirSave_Dir(parent);
    // TODO: Delete File structure
    AqFree(&(file->location), 1);
    kfree(file);
    return 0;
}

int Aq_Delete_Dir(char* path)
{
    // VERIFY IF DIRECTORY IS EMPTY, ELSE FIRST DELETE ALL FILES
    AqDirectory_t* parent = Aq_DirGet_Parent(path);
    AqDhdrEntry_t* entry = Aq_DirGet_FileEntry(parent, Aq_GetLname(path));
    AqDirectory_t* dir = (AqDirectory_t*)kmalloc(512);
    AqRead(&(entry->location), (uint32_t*)dir, AQ_BLOCK_SIZE/AQ_SECTOR_SIZE);
    
    entry->magic = 0;
    AqDhdr_t* hdr = Aq_GetDhdr_fromEntry(entry);
    --hdr->usedEntrys;
    Aq_DirSave_Header(hdr);
    kfree(hdr);

    AqFree(&(dir->location), 1);
    kfree(dir);
    return 0;
}

int Aq_ListEntrys(char* path)
{
    return Aq_ListEntrys_direct(Aq_DirGet_Dir(path));
}

int Aq_ListEntrys_direct(AqDirectory_t* dir)
{
    AqDhdr_t* hdr;
    if(dir->OpenHeader)
    {
        hdr = dir->OpenHeader;
    }
    else
    {
        hdr = (AqDhdr_t*)pmem_4k(AQ_DHEADER_SIZE/4096);
    }

    uint32_t blks = AQ_DHEADER_SIZE/AQ_BLOCK_SIZE;
    AqRead(&(dir->FirstDirHeader), (uint32_t*)hdr, blks);
    printf("\nListing Content...");
    for(uint32_t i = 0; i < dir->headers; i++)
    {
        if(hdr->magic == AQ_DHR_MAGIC)
        {
            for(uint32_t j = 0, c = 0; c < hdr->usedEntrys; j++)
            {
                if(hdr->Entrys[j].magic == AQ_DHR_MAGIC)
                {
                    printf("\n%s", hdr->Entrys[j].name);
                    ++c;
                }
            }
        }
        AqRead(&(hdr->NextDirHeader), (uint32_t*)hdr, blks);
    }
    return 0;
}

AqHandle_t* Aq_Load(char* path)
{
    AqDhdrEntry_t* entry = Aq_LoadEntry(path);
    AqHandle_t* handle = (AqHandle_t*)kmalloc(sizeof(AqHandle_t));
    uintptr_t desc = (uintptr_t)kmalloc(512);
    AqRead(&(entry->location), (uint32_t*)desc, AQ_BLOCK_SIZE);
    handle->descriptor = desc;
    strcpy(handle->name, entry->name);
    handle->type = entry->type;
    return handle;
}

AqHandle_t* Aq_LoadFile(char* path)
{
  //  printf("\n{%s}", path);
    AqDirectory_t* parent = Aq_DirGet_Parent(path);
    AqDhdrEntry_t* entry = Aq_DirGet_FileEntry(parent, Aq_GetLname(path));
    AqHandle_t* handle = (AqHandle_t*)kmalloc(sizeof(AqHandle_t));
    AqFile_t* desc = (AqFile_t*)kmalloc(512);
    AqRead(&(entry->location), (uint32_t*)desc, AQ_BLOCK_SIZE/AQ_SECTOR_SIZE);
  //  printf("\n{%s, %d; desc->magic %x}", entry->name, entry->location.lower32, desc->magic);
    handle->descriptor = (uintptr_t)desc;
    strcpy(handle->name, entry->name);
    handle->type = entry->type;
    // Put in some sort of loaded file table
    return handle;
}

int Aq_CloseFile(AqHandle_t* handle)
{
    Aq_Save_All();
    Aq_FileSave_File(Aq_HandleGet_File(handle));
    kfree(handle->descriptor);
    kfree(handle);
    return 0;
}

int Aq_ReadFile(uint32_t* buffer, uint32_t offset, uint32_t size, AqHandle_t* handle)
{
    int ts = size;
    if(!handle || !handle->descriptor)
    {
        return -1;
    }
    // First get the entry with the corresponding offset
  //  printf("<%d; size: %d>", offset, size);
    AqFhdrEntry_t* ie = Aq_FileGet_EntryCA(Aq_HandleGet_File(handle), offset);      // Initial Entry
    if(!ie) return -2;

  //  printf("\n{%d}", Aq_HandleGet_File(handle)->info.data_size);

    uint32_t t1 = MIN(ie->used, ie->offset + size);
    uint32_t t2 = ROUNDUP(t1, AQ_FENTRY_SIZE)/512;
    if(!t2) ++t2;
  //  printf("\n<ie->used=%d, t2: %d, t1rounded: %d, t1: %d>", ie->used, t2, ROUNDUP(t1, AQ_FENTRY_SIZE), t1);
    uint32_t tbuf = (uint32_t)kmalloc(t2 * 512);

    t1 -= ie->offset;

   // printf("{%x; %d}", Get_Scheduler()->current_pdir, Get_Scheduler());
    if(!AqRead(&ie->location, (uint32_t*)tbuf, t2)) return -3;
   // printf("{%x: [%d], %d}", Get_Scheduler()->current_pdir, tbuf, t2);
    memcpy((void*)buffer, (void*)(tbuf + ie->offset), t1);      // Extract the data from this entry
    size -= t1;
  //  printf("\n[Size: %d, t2: %d, t1: %d, %d]", size, t2, t1, t1 - ie->offset);
  //  printf("\n%s\n[%d, %d; size: %d, used: %d, ie->offset: %d; loc: %d]", tbuf, t1, strlen(tbuf), size, ie->used, ie->offset, ie->location.lower32);
    kfree(tbuf);
    
    uint32_t buf = (uint32_t)buffer;
    buf += t1;

    while(size > 0)   // Data spans multiple Entrys
    {
     //   printf("\n<MULTIPLE>");
        ie = Aq_FileGet_NextEntry(Aq_GetFhdr_fromEntry(ie), ie);
        if(!ie) return -4;

        t1 = MIN(ie->used, size);
        t2 = ROUNDUP(t1, AQ_FENTRY_SIZE)/512;
        tbuf = (uint32_t)kmalloc(t2 * 512);

        t1 -= ie->offset;

        if(!AqRead(&ie->location, (uint32_t*)tbuf, t2)) return -5;
        memcpy((void*)buf, (void*)(tbuf), t1);      // Extract the data from this entry

      //  printf("\n%s\n[%d, %d; size: %d, used: %d, ie->offset: %d; loc: %d]", tbuf, t1, strlen(tbuf), size, ie->used, ie->offset, ie->location.lower32);

        kfree(tbuf);

        size -= t1;
        buf += t1;
    }

    return ts - (int)size;  
}

int Aq_WriteFile_Append(uint32_t* buffer, uint32_t size, AqHandle_t* handle)
{
    int ts = size;
    printf("\nSize: %d", size);
    if(!handle)
    {
        return 0;
    }
    // First get the Last entry
    AqFile_t* file = Aq_HandleGet_File(handle);
    AqFhdr_t* hdr = Aq_FileGet_Header(&(file->lastEntryHdr));
    AqFhdrEntry_t* ie;
    if(!hdr) // File is new, create a new header
    {
        Aq_FileCreate_NewHeader(file);
        uint32_t size512 = ROUNDUP(size, AQ_FENTRY_SIZE)/512;
        AqLocation_t* loc = AqAlloc(size512);
       // printf("\n{{%d}}", loc->lower32);
        AqWrite(loc, buffer, size512);
        ie = Aq_FileCreate_NewEntry(file, loc);
        ie->spread = size512;
        ie->used = size;
        ie->nextOff = 0;
     //   printf("\t{{%d}}", ie->location.lower32);

        Aq_LocationCopy(&file->lastEntryHdr, &Aq_GetFhdr_fromEntry(ie)->location);
        file->lastEntryOff = 0;
        Aq_FileSave_Entry(ie);
        file->info.data_size += size;
        file->info.size += size512*512;
        Aq_FileSave_File(file);
      //  Aq_Save_All();
    }
    else  
    {
       // printf("\nVoila ");
        uint32_t buf = (uint32_t)buffer;

        ie = &(hdr->Entrys[file->lastEntryOff]);      // Last Entry
        if(!ie) return 0;

        uint32_t t1 = MIN((ie->spread*512) - ie->used, size);
        uint32_t t2 = ROUNDUP(t1, AQ_FENTRY_SIZE)/512;
        uint32_t tbuf = (uint32_t)kmalloc(t2 * 512);

        AqLocation_t loc;
        loc.lower32 = ie->location.lower32;
        loc.higher32 = ie->location.higher32;
        uint32_t t3 = ROUNDDOWN((uint32_t)(ie->used), AQ_FENTRY_SIZE);     // OR ie->spread*512 - ROUNDUP(ie->spread*512) - ie->used, 512)
        Aq_LocationIncrement(&loc, t3/AQ_SECTOR_SIZE);

        AqRead(&loc, (uint32_t*)tbuf, t2);
        memcpy((void*)(tbuf + (ie->used - t3)), (void*)buf, t1);
        AqWrite(&loc, (uint32_t*)tbuf, t2);

       // printf("\ie->used: %d t1 = %d", ie->used, t1);
        ie->used += t1;
        size -= t1;
        
        file->info.data_size += ts;
        kfree(tbuf);

        Aq_FileSave_Entry(ie);
        if(size)
        {
          //  printf("\nIt reaches here!");
            buf += t1;
            uint32_t size512 = ROUNDUP(size, AQ_FENTRY_SIZE)/512;
            AqLocation_t* loc = AqAlloc(size512);
            AqWrite(loc, (uint32_t*)buf, size512);
            
            AqFhdrEntry_t* te = ie;
            ie = Aq_FileCreate_NewEntry(file, loc);
            AqFhdr_t* fhd = Aq_GetFhdr_fromEntry(ie);
            if(!ie)
            {
                return -7;
            }
          //  Aq_File_LinkLastEntry(ie);
            ie->spread = size512;
        //    printf("\ie->used: %d t1 = %d", ie->used, size);
            ie->used += size;
            ie->nextOff = 0;
            
            if(fhd->id == Aq_GetFhdr_fromEntry(te)->id)
            {
            //    printf("\nWTF!!!!");
                te = &(fhd->Entrys[te->id]);
                te->nextOff = ie->id;
                Aq_LocationCopy(&te->NextHdr, &(fhd->location));
                Aq_FileSave_Entry(te);
            }
            else  
            {
                te->nextOff = ie->id;
                Aq_LocationCopy(&te->NextHdr, &(fhd->location));
            }
           // printf("\n[%d %d]", te, ie);
        //    printf("\t<%d>", te->NextHdr.lower32);
            Aq_LocationCopy(&file->lastEntryHdr, &(fhd->location));//te->NextHdr;
            file->lastEntryOff = ie->id;

            file->info.size += size512*512;
            Aq_FileSave_Entry(ie);
        }
        Aq_FileSave_File(file);
      //  Aq_Save_All();
    }
    return 1;
}

int Aq_WriteFile(uint32_t offset, uint32_t *buffer, uint32_t size, AqHandle_t* handle, uint32_t overwrite)
{
    return Aq_WriteFile_Append(buffer, size, handle);
}



