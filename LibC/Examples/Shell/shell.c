#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "shell.h"

void main()
{
    char* shell_buf;
    Main_CSI_struct = malloc(4096);
    CSI_mem_start = Main_CSI_struct;
    console_manager_init();
    printf("\nInitializing the Shell...\n");
	while(1)
	{
        printf("\n%g%s%g>%g",12, "Rabbit",10,11);
        shell_buf = (char*)malloc(128);
        scanf("%s", shell_buf);
        
		printf("[%s]%g",10, shell_buf);
		Shell_command_locator((char*)shell_buf);
	}
}

void Shell_ArgParse(char* str)
{
	*tot_entries = 0;
	while(1) //Provide the Arguments provided by the user
	{
		str = strtok(NULL, " ");
		if(str== NULL) break;
		*CSI_entries_ptr = (uint32_t)str;

		if(str[0] == '\"')
		{
			++str;

			*CSI_entries_ptr = (uint32_t)str;

			for(;str[strlen(str)-1] != '\"';)
			{
				 str[strlen(str)] = ' ';
				 str = strtok(NULL, " ");
			}
			str[strlen(str)-1] = '\0';
		}

		++CSI_entries_ptr;
		++*tot_entries;
	}
}

int Shell_command_locator(char *inst)
{
	 uint32_t tmp = 0;

	 char* tmpstr = strtok(inst, " ");
	 int i;
	 for( i = 0 ; tmpstr[i]!='\0' && i <= 16; i++ )				// Hash Function
	 {
			if((uint32_t)tmpstr[i] >= 97)
				 tmp += (((uint32_t)tmpstr[i]) - 97)*(i+1);
			else
				 tmp += (((uint32_t)tmpstr[i]) - 65)*(i+1);
	 }
	 if(tmp <= 2048)
	 {
			uint32_t* ptr = (uint32_t*)Shell_Commands_list[tmp];
			if(!strcmpx(((Shell_Commands_t*)ptr)->command, tmpstr))
			{
				 func_t func = ((Shell_Commands_t*)ptr)->func;

                 Shell_ArgParse(tmpstr);
                  
				func();
				memset((uint32_t*)CSI_mem_start, 0, 2 + *tot_entries);
				CSI_entries_ptr = (uint32_t*)&Main_CSI_struct->entries;
				return ((Shell_Commands_t*)ptr)->reserved;
			}
			else
			{
                printf("\n%s", tmpstr);
                printf("\n Command Not Recognized! type help for help %x\n", tmpstr ); //TODO: Search within other possible files/executables like in the PATH string.
				return -1;
			}
	 }
	 else
	 {
			printf("\n Command Not Recognized! type help for help\n");
			return -1;
	 }
}

void Shell_Add_Commands(func_t func, uint32_t command_no, int flag, char* name)
{
	 Shell_Commands_t* command = (Shell_Commands_t*)malloc(sizeof(Shell_Commands_t));
	 command->func = func;
	 command->reserved = flag;
	 Shell_Commands_list[command_no] = (uint32_t)command;
	 strcpy(command->command, name);
}

uint32_t* CSI_Read(uint32_t entry)
{
	uint32_t* tmp = (uint32_t*)&Main_CSI_struct->entries;
	if(entry)
	{
		return (uint32_t*)tmp[entry - 1];
	}
	return 0;
}

uint32_t* CSI_ReadAPS(char* str) //Read As the Provided String!
{
	uint32_t* tmp = (uint32_t*)&Main_CSI_struct->entries;
	for(uint32_t i = 0; i<Main_CSI_struct->total_entries; i+=2)
	{
	//  printf(tmp[i]);
		//printf(tmp[i+1]);
		if(strcmp((char*)tmp[i], str) == 0)
			return (uint32_t*)tmp[i+1];
	}
	return 0;
}

void Command_help()
{
    printf("\nHelping in a second...");
}

void Command_test()
{
    printf("\nTesting in a second...");
}

void Command_infinity()
{
    printf("\nTesting in a second...");
    double te = 10.12322; 
    while(1)
    {
        printf("[-a-]");
        te = 10.12322; 
        for(int i = 0; i < 10000; i++)
        {
            te *= te;
        }
    }
}

void console_manager_init()
{
	 Shell_Add_Commands(Command_help, 108, 0, "help");
	 Shell_Add_Commands(Command_test, 157, 0, "test");
	 Shell_Add_Commands(Command_infinity, 0x207, 0, "infinity");
	 memset((uint32_t*)CSI_mem_start, 0, 66);
}
