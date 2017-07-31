#include "neuron.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "phy_mm/mem.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "common.h"
#include "RandomLib/random.h"
#include "math.h"

void NeuralNet_init()
{
  printf("\nSizeof Neuron_t: %x\n%gBuilding Neurons\n", sizeof(Neuron_t), 9);
  asm volatile("int $50");
  asm volatile("cli");

  char* tmpss = (char*)CSI_Read(2);
  uint32_t power = StrToInt(tmpss);

  tmpss = (char*)CSI_Read(3);
  uint32_t multiple = StrToInt(tmpss);

  tmpss = (char*)CSI_Read(4);
  uint32_t base = StrToInt(tmpss);

  if(!power) power = 4;
  if(!base) base = 15;
  if(!multiple) multiple = 1;

  Neurons = pow(base, power)*multiple - 1;
  NeuralSize = Neurons*sizeof(Neuron_t);
  NeuralBase = (uint32_t)kmalloc(NeuralSize);

  Neuron_t* tmp = (Neuron_t*)NeuralBase,* tmpbase;

  for(uint32_t i = 0; i < Neurons; i++ )
  {
    tmp->id = i;
    tmp->Value = 0;
    tmp->totp = 0;
    tmp->totc = 0;
    ++tmp;
  }

  tmp = (Neuron_t*)NeuralBase;
  tmpbase = tmp;
  uint32_t rand1, randnum;
  init_hpet();
  printf("\n%gLinking Neural Networks, Building Random Connections...",12);
  uint32_t tmp11;
  for(uint32_t i = 0; i < Neurons - 1; i++ )
  {
    Randomizer();
    for(uint32_t j = 0; j < NEURONS_PER_NEURON; j++)
    {
      tmp11 = 0;
      //tmp22 = 0;
      back:
      randnum = random();
      rand1 = (randnum%(Neurons)); //Some random value between 0 to Neurons present.
      if(tmp11 == Neurons)
      {
        break;
      }
      //_printf("%x--", rand1);
      if(tmpbase[rand1].totp == NEURONS_PER_NEURON)
      {
        ++tmp11;
        goto back;
      }
      for(int k = 0; k < NEURONS_PER_NEURON; k++)
      {
        if((uintptr_t)&tmpbase[rand1] == (uintptr_t)tmp->connections[k])
        {
          //++tmp22;
          goto back;
        }
      }
      tmp->connections[j] = (uintptr_t)&tmpbase[rand1];
      tmp->Aweights[j] = 2;
      tmp->threshold = (4096*1024) + (randnum/Neurons)%8193;
      ++tmp->totc;
      tmpbase[rand1].parents[tmpbase[rand1].totp] = (uintptr_t)tmp;
      tmpbase[rand1].Bweights[tmpbase[rand1].totp] = 2;
      ++tmpbase[rand1].totp;
    }
    ++tmp;
  }
  printf("\nNeural Network setup complete.\n%gVerification:%g \n",13, 15);
  tmp = tmpbase;
  for(uint32_t i = 0; i < Neurons - 1; i++)
  {
    if(tmp->totc < NEURONS_PER_NEURON) printf("C%x=%x--", tmp->id, tmp->totc);
    if(tmp->totp < NEURONS_PER_NEURON) printf("P%x=%x--", tmp->id, tmp->totp);
  //  if(i >  (Neurons*8)/10)
  //    printf("Y%x=%x--", tmp->id, tmp->totc);
    ++tmp;
    if(!tmp->id) break;
  }
  Neuron_New = NeuralBase;
  NeuralNet_SaveToFile((uint32_t*)NeuralBase, NeuralSize);
  asm volatile("sti");
}

void NeuralNet_SaveToFile(uint32_t* base, uint32_t size)
{
  char* tmpc = annCounter;
  //strcpy(annCounter, "/NeuralNetworks/annCounter.dat");
  printf("\n\nSaving the Contents to File...\n" );
  if(!search_folderOGP(annDirName))
  {
    printf("\"%s\" Directory not found, Creating one...\n", annDirName);
    char tmp[50];
    strcpy(tmp, "mkdir ");
    strcat(tmp, annDirName);
    Shell_SFexecute(tmp);
  }
  if(!file_loadOGP(tmpc))
  {
    printf("File \"%s\" dosent exist, Creating...\n",tmpc);
    char tmp[40];
    strcpy(tmp, "mkfl ");
    strcat(tmp, tmpc);
    Shell_SFexecute(tmp);
  }
  int csz = file_size(tmpc), count = 0;
  char ctmp[10];
  printf("Size of file: %d\n", csz);
  if(csz)
  {
    file_readTM((uint32_t*)ctmp, 0, csz, tmpc);
    count = StrToInt(ctmp);
  }
  else
  {
    char tmp[10];
    memset(tmp, 0, 10);
    file_writeAppend((uint32_t*)tmp, 10, tmpc);
  }
  ++count;
  char ctmp2[10];
  itoa(count, ctmp2, 10);
  file_editFM(0, 10, (uint32_t*)ctmp2, 10, tmpc);
  file_closeOGP(tmpc);

  char tmps2[40];
  strcpy(tmps2, annFileNames);
  strcat(tmps2, ctmp2);
  strcat(tmps2, ".ann");

  printf("Saving the Neural Network to file \"%s\"\n", tmps2);

  char tmpe[50];
  strcpy(tmpe, "mkfl ");
  strcat(tmpe, tmps2);
  Shell_SFexecute(tmpe);

  if(file_loadOGP(tmps2))
  {
    printf("Saving...\nFile Size: %d\n", size);
    file_writeAppend(base, size, tmps2);
    file_closeOGP(tmps2);
  }
  printf("Done\n");
}
