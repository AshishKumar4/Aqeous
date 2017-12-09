#include "stdio.h"
#include "hpet.h"
#include "shell.h"
#include "tasking.h"
#include "string.h"

#include "ext2/ext2_fs.h"
#include "apic.h"
#include "NeuralNetwork/Neuron/neuron.h"
#include "vfs.h"

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
    ext2_burn(4096*10,0);
    ext2_initialize(4096*10,0);
    vfs_setup_ext2();
  }
  else if(!strcmp("aqfs2", device_name))
  {
    if(strcmp(CSI_Read(2), "burn") == 0 || Aqfs2_Checkfs())
    {
      Aqfs2_Partitioner(0, 5, 200*1024);
      Aqfs2_burn(0);
      vfs_setup_aqfs();
    }
  }
  else if(!strcmp("aqfs", device_name))
  {
    AqFS_burn();
    AqFS_init();
    vfs_setup_aqfs();
  }
  else if(!strcmp("apic", device_name))
  {
    MADTapic_parse();
  }
  else if(!strcmp("nn", device_name))
  {
    NeuralNet_init();
  }
  else if(!strcmp("vfs", device_name))
  {
    vfs_init();
  }
  else if(!strcmp("vesa", device_name))
  {
    Init_Vesa(0x118);
  }
  else
  {
    printf("\nNo such Device \"%s\" found to be initialized", device_name);
  }
}
