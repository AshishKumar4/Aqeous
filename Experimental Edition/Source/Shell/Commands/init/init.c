#include "stdio.h"
#include "hpet.h"
#include "shell.h"
#include "tasking.h"
#include "string.h"

#include "ext2/ext2_fs.h"
#include "apic.h"
#include "NeuralNetwork/Neuron/neuron.h"

void Command_init()
{
  asm volatile("cli");
  char* device_name = (char*)CSI_Read(1);
  if(!strcmp("hpet", device_name))
  {
    init_hpet();
  }
  else if(!strcmp("ext2", device_name))
  {
    ext2_initialize(4096,0);
  }
  else if(!strcmp("aqfs", device_name))
  {
    AqFS_burn();
    Init_fs();
  }
  else if(!strcmp("apic", device_name))
  {
    MADTapic_parse();
  }
  else if(!strcmp("nn", device_name))
  {
    NeuralNet_init();
  }
  else
  {
    printf("\nNo such Device \"%s\" found to be initialized", device_name);
  }
}
