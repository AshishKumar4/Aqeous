#include "stdio.h"
#include "stdlib.h"
#include "shell.h"
#include "string.h"
#include "common.h"
#include "CPU/cpu.h"
#include "vesa.h"

void Command_cpu()
{
  char* arg = CSI_Read(1);
  if(!strcmp(arg, "info"))
  {
    detect_cpu();
    check_SSE();
    if(SSE_available) printf("\nSSE is Available");
  }
  else if(!strcmp(arg, "reboot"))
  {
    uint32_t APIC_id = StrToInt(CSI_Read(2));
    Boot_AP(APIC_id);
  }
  else if(!strcmp(arg, "stall"))
  {

  }
  else if(!strcmp(arg, "realmode"))
  {
    uint32_t APIC_id = StrToInt(CSI_Read(2));
    AP_RealMode_Setup(APIC_id);

  /*  VESA_INFO* vesa_info = 0x1500;

    printf("\nVesa Signature: %s\n", vesa_info->VESASignature);
    printf("\n\nVesa Version: %d.%d\n", (uint32_t)vesa_info->VESAVersion[1], (uint32_t)vesa_info->VESAVersion[0]);
    printf("\n\nVesa Video Modes: %x\n", (uint32_t)vesa_info->VideoModePtr[1]*16 + vesa_info->VideoModePtr[0]);
    printf("\nVesa Video Total Memory: %d\n", vesa_info->TotalMemory);*/

  }
}
