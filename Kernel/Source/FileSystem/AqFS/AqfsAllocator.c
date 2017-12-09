#include "Aqfs.h"
#include "AqfsTools.h"

/*
    pseudo Allocation Algorithm-->
        search for tables with greatest chunks bigger then requested size
        Load the table -> search for such a chunk -> divide the chunk -> 
        if chunk parted into two->
                search for a table with empty entry -> put the new block there
        else do nothing ->
        update 'greatest' value of the entry of each table.
        update everything else on the disk
*/

/*
    Each block would be put in the entry marked for the beginning of the block (block's first start address).
    To allocate, Search for a chunk big enough -> allocate
    To Deallocate, Get the last address of the block, search for the block starting from that address, using indexing
        If Found, Delete this block and merge it with the main block. Save any meta changes. Then continue repeating above
        If Not Found, Just place this block in its respective position in the table.
*/
// ALLOCATES WITH GRANULARITY = 1 sector

AqLocation_t* AqAlloc(uint32_t size)
{
    AqAllocMeta_t* aqmeta = (AqAllocMeta_t*)AqfsMainFile->metaTable_ptr;
    for(uint32_t i = 0; i < aqmeta->n_available; i++)
    {
       // printf("\n!--");
        if(aqmeta->entries[i].greatest >= size) 
        {
           // printf("\n >>%d", aqmeta->entries[i].greatest);
            // Load the table 
            AqAllocTable_t* tbl = (AqAllocTable_t*)kmalloc(AQ_ALLOCTABLE_SIZE);
            AqRead(&(aqmeta->entries[i].loc), (uint32_t*)tbl, AQ_ALLOCTABLE_BLOCKS);

            AqAllocEntry_t* tmp1, *tmp2, *tmp3;
            tmp1 = tmp2 = tmp3 = tbl->entries;
            for(uint32_t j = 0; j < AQ_ALLOCTABLE_MAX_ENTRIES; j++)
            {
                if(tmp1->size >= size)        // Best Fit
                {
                    if(tmp1->size < tmp2->size)
                    {
                        tmp2 = tmp1;    // Smallest fitable block
                    }   
                    else tmp3 = tmp1;   // Biggest block
                }
                ++tmp1;
            }
            AqLocation_t* loc = Aq_LocationCopyCreate(&tmp2->start);
            tmp2->size -= size;
          /*  Aq_LocationIncrement(&(tmp2->start), size);
          /*/
            Aq_LocationIncrement(loc, tmp2->size);
          //*/
            if(tmp2->size == 0)
            {
                // Remove the entry
            }
            AqWrite(&(aqmeta->entries[i].loc), (uint32_t*)tbl, AQ_ALLOCTABLE_BLOCKS);       // Update Allocation Table
            
            aqmeta->entries[i].greatest = tmp3->size;   
           // printf("\t >>%d", aqmeta->entries[i].greatest);
           // AqWrite(&(aqmeta->location), (uint32_t*)aqmeta, AqfsMainFile->MainHeader_ptr->metaSpread);      // Update greatest value
           // printf("\n<%d>", loc->lower32);
           // printf("\nA1");
            return loc;
        }
    }
    printf("\nASDASDSAD");
    return 0;
}

int AqFree(AqLocation_t* loc, uint32_t size)
{
    /*
        
    */
}

void AqAllocInit(uint32_t base, uint32_t disk_size)
{
    AqAllocMeta_t* aqmeta = (AqAllocMeta_t*)AqfsMainFile->metaTable_ptr;
    aqmeta->n_entries = ((((AqfsMainFile->MainHeader_ptr->metaSpread)*AQ_SECTOR_SIZE) - sizeof(AqAllocMeta_t)) / (sizeof(AqMetaEntry_t))) - 2;
    
    
    // Create the first meta entry
    aqmeta->n_available = 1;
    aqmeta->entries[0].loc.lower32 = base;
    aqmeta->entries[0].loc.higher32 = 0;

    aqmeta->entries[0].greatest = disk_size;
    aqmeta->entries[0].empty = AQ_ALLOCTABLE_MAX_ENTRIES - 1;

    AqWrite(&(aqmeta->location), (uint32_t*)aqmeta, (AqfsMainFile->MainHeader_ptr->metaSpread));  // Save Meta changes

    uint32_t buf = (uint32_t)kmalloc(AQ_ALLOCTABLE_SIZE);
    AqAllocTable_t* aqt = (AqAllocTable_t*)buf;
    aqt->entries[0].size = disk_size;
    
    aqt->entries[0].start.lower32 = base + AQ_ALLOCTABLE_BLOCKS + 2;
    aqt->entries[0].start.higher32 = 0;

    AqWrite(&(aqmeta->entries[0].loc), (uint32_t*)buf, AQ_ALLOCTABLE_BLOCKS);   // Save the alloctable
}
