#include "stdio.h"
#include "stdlib.h"
#include "common.h"
#include "string.h"
#include "Shell/shell.h"
#include "Water/Water.h"

void Command_water()
{
  Water_Task = create_task("WaterTask", Water_shell, 18, 0x202, Shell_proc);
  Shell_Softsleep();
  Activate_task_direct(Water_Task);
}
