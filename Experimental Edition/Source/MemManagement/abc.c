
uint32_t kmalloc_int(uint32_t sz, int align)
{
  /*
    uint32_t mb=0;   ///dont give kernel memory below this point
    if(purpose==1) mb=+400; //for kernel
    else if(purpose==3) mb+=52; //for tasking
    else if(purpose==4) mb+=300; //for tasking
    else if(purpose==5) mb+=200; //for filesystem
    else mb+=500;*/
    uint32_t volatile sz4;
    if(!sz%4)
    {
      sz4=sz/4;
    }
    else
    {
      sz+=(4-(sz%4));
      sz4=sz/4;
      //sz4++;
      //sz=sz*4;
    }
    ptable* table;
    page_t* page;
    MemMap_t* block;
    uint32_t phy_mem=0;
    uint32_t virt_addr=0;
    if(!sz4)//if size is less then 32 bytes, roundof it to 32 which is the least memory allocable
    {
      sz=4;
      sz4=1;
    }

    if(sz>=4096||align) ///4096-32 as 32 is the least allocable size
    {
      uint32_t pgs=sz/4096;
      uint32_t tsz4=sz%4096;
      if(tsz4)
      {
        if(tsz4 < 4)
          tsz4 = 1;
        tsz4 /= 4;
      }
      uint32_t temp=0;
      for(uint32_t i=20;i<1024;i++)
      {
        table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(&system_dir->m_entries[i]);
        page=&table->m_entries[0];
        for(uint32_t j=0;j<1024;j++)
        {
          page=&table->m_entries[j];
          block=BlockFinder(PAGE_GET_PHYSICAL_ADDRESS(page));
          if(!block->used)
          {
            temp++;
            if(temp==pgs)
            {
              page=&table->m_entries[j-temp+1];
              virt_addr=(i*4096*1024)+((j-temp+1)*4096); ///i=table offset, j=page offset
              phy_mem = virt_addr;
              for(uint32_t k=0;k<temp;k++)
              {
                phy_mem+=k*4096;
                //pt_entry_set_frame ( page, phy_mem);
                *page |= CUSTOM_PTE_AVAIL_1 | CUSTOM_PTE_AVAIL_2;
                block=BlockFinder(phy_mem);
                block->page=page;
                block->id=(curr_pgdir.ID & 0xff) | ((blockID&0xff)<<8);
                block->used=4096;
                ++block;
                ++page;
              }
              if(tsz4)
              {
                phy_mem+=4096;
                *page |= CUSTOM_PTE_AVAIL_1;
                block=BlockFinder(phy_mem);
                block->page=page;
                block->id=(curr_pgdir.ID & 0xff) | ((blockID&0xff)<<8);
                block->used=(tsz4+1)*4;
                uint32_t* strip = (uint32_t*)(phy_mem + (tsz4*4));
                *strip = 42847 | (tsz4 << 16);
                strip = (uint32_t*)(phy_mem + 4092);
                *strip = (1&0xffff) | (42847 << 16);
              }
              blockID++;
              return virt_addr;
            }
          }
          else
          {
            temp=0;
          }
          ++page;
        }
      }
    }
    else
    {
      int clrBlks=0;
      uint32_t phy_mem=0;
      for(uint32_t i=20; i<1024; i++)
      {
        table=(ptable*)PAGE_GET_PHYSICAL_ADDRESS(&system_dir->m_entries[i]);
        page=&table->m_entries[0];
        for(uint32_t j=0;j<1024;j++)
        {
          if(!(*page & CUSTOM_PTE_AVAIL_1)) //The whole page is unused, so its not been mapped yet
          {
            virt_addr = (i*4096*1024)+(j*4096);
            phy_mem = virt_addr;
            *page |= CUSTOM_PTE_AVAIL_1;
            block=BlockFinder(phy_mem);
            if(!block->used)
            {
              block->page=page;
              block->id=(curr_pgdir.ID & 0xff) | (1<<8);
              block->used=(sz4+1)*4;
              uint32_t* strip = (uint32_t*)(phy_mem + (sz4*4));
              *strip = (42847&0xFFFF) | (sz4 << 16);
              strip = (uint32_t*)(phy_mem + 4092);
              *strip = 1 | (42847 << 16);
              return virt_addr;
            }
          }
          else if(!(*page & CUSTOM_PTE_AVAIL_2))
          {
            virt_addr = (i*4096*1024)+(j*4096);
            phy_mem = virt_addr;
            block=BlockFinder(phy_mem);
            if(block->used<=(4092-(sz4*4)))
            {
              clrBlks=clearBlks((uint32_t*)phy_mem,sz4+1);
              if(clrBlks>=0)
              {
                uint32_t* strip = (uint32_t*)(phy_mem + (sz4*4) + (clrBlks*4));
                *strip = (42847&0xFFFF) | (sz4 << 16);
                uint16_t* last_strip = (uint16_t*)(phy_mem + 4092);
                ++*last_strip;
                block->used+=sz4*4;
                if(block->used > 4088) //block already full, no space
                {
                  pt_entry_add_attrib(page, CUSTOM_PTE_AVAIL_2);
                }
                return virt_addr+(4*clrBlks);
              }
            }
          }
          ++page;
        }
      }
    }
    printf("\n Something went wrong");
    return 0;
    //while(1);
}
