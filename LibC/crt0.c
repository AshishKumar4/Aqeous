#include "library.c"
#include "aqlib.c"

extern int main(int argc, char *argv[]);


void __attribute__((optimize("O0"))) _start()
{
  STD_OUT_ptr[0] = 0;
  SYSCALL_SPACE[0] = 0;

  STD_IN_ptr[0] = 8;
  STD_IN_ptr[1] = 0;
 // KernelMessageSpace[0] = 8;
  create_fdTable();

  int argc = (int)argTable[0];
  char **argv = (char**)&argTable[1];
 // main();
 // printf("{%d,  %d %s, %d %s}\n", (uint32_t)argc, argv[0], argv[0], argv[1], argv[1]);
  int exit_code = main(argc, argv);
  exit(0);
  while(1);
}
