#include "NeuralNetwork/Neuron/neuron.h"
#include "FileSystem/AqFS/fs.h"
#include "Shell/shell.h"
#include "MemManagement/phy_mm/mem.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "common.h"
#include "NeuralNetwork/Neuron/NeuralProcessing.h"

void Command_ann()
{
  char* fpath = CSI_Read(1);
  asm volatile("cli");
  if(!fpath)
  {
    uint32_t* buffer = (uint32_t*)Neuron_New;

    Neuron_t* tmp = buffer;

    for(int i = 0; i < Neurons - 1; i++)
    {
      printf("%x=%x--", tmp->id, tmp->totp);
      ++tmp;
      //if(!tmp->id) break;
    }
    asm volatile("sti");
    return;
  }

  if(!file_loadOGP(fpath))
  {
    printf("\n%s file dosent exist!\n",fpath);
    return;
  }
  uint32_t sz = file_size(fpath);
  char* option = CSI_ReadAPS("-o");
  //if(!strcmp(option, "read"))
  {
    printf("\nSize of the file: %x \n", sz);

    uint32_t* buffer = kmalloc(sz);

    file_readTM(buffer,0,sz,fpath);

    Neuron_t* tmp = buffer;

    for(int i = 0; i < (sz/sizeof(Neuron_t)) - 1; i++)
    {
      printf("%x=%x--", tmp->id, tmp->totp);
      ++tmp;
      if(!tmp->id) break;
    }

    printf("\nUpdating Neural Network");

    NeuralForwardUpdater();

    file_closeOGP(fpath);

    printf("%s\n", buffer);
    kfree(buffer);
  }
  asm volatile("sti");

}
