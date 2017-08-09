#include "shell.h"
#include "console.h"
#include "tasking.h"
#include "kb_handle.h"
#include "common.h"
#include "stdlib.h"
#include "virt_mm/vmem.h"
#include "virt_mm/paging.h"
#include "phy_mm/mem.h"
#include "vesa.h"
#include "fs.h"
#include "graphics.h"
#include "PIC.h"
#include "memfunc.h"

#include "Commands/Default_Commands.c"
#include "Commands/numerise/numerise.c"

#include "Commands/ProcManagement/ProcManagement.c"
#include "Commands/Echo/echo.c"
#include "Commands/CPU/cpu.c"
#include "Commands/lspci/lspci.c"
#include "Commands/Water/Water_command.c"

uint32_t VGA_size;

DECLARE_LOCK(a);

void init_shell()
{
	consolerow = 0;
	consolecolumn = 0;

	memset_fast((void*)console_dbuffer, 0, 4194304);
	VGA_size = VGA_WIDTH*VGA_HEIGHT/2;
	shell_buf = (char*)kmalloc(4096);
	Shell_Commands_list = (uint32_t*)kmalloc(8192);
	Shell_Istream = (volatile uint32_t*)kmalloc(4096*1024);
	Input_stream = (uint32_t)Shell_Istream;
	Istream_end = ((uint32_t)Shell_Istream) + (4096*1024);
	Istream_ptr = (volatile char*)Input_stream;
	Shell_Istream = (volatile uint32_t*)kmalloc(4096);
	memset_faster((uint32_t*)CSI_mem_start, 0, 18);
	CSI_entries_ptr = (uint32_t*)&Main_CSI_struct->entries;
	tot_entries = (uint32_t*)&Main_CSI_struct->total_entries;
	memset((void*)console_buffer,0,VGA_size*4);
	Enable_SSE();
	console_manager_init();
	shell_cmdTask = create_task("shell_cmdTask", Shell_cmdWrapper, 20, 0x202, Shell_proc);
	UNLOCK(a);
	UNLOCK(SHELL_CMDTASK);
}

void __attribute__((optimize("O0"))) Shell_Double_buffer()
{
	 uintptr_t _s, _c;
	 while(1)
	 {
		 //asm volatile("cli");

		 //TODO: COPY THE VGA_BUFFER TO THE ACTUAL VGA BUFFER (console_buffer)
		 _s = (uintptr_t)console_buffer;
		 _c = (uintptr_t)console_dbuffer;

		 memcpy_rep((uint32_t)_s, (uint32_t)_c,VGA_size);
		 //asm volatile("cli");
		 if(console_skip)
		 {
			console_dbuffer += (VGA_WIDTH*console_skip);
			consolerow -= console_skip;
			console_skip = 0;
			if((uint32_t)console_dbuffer >= console_dbuffer_limit)   //TODO: Make the console buffer mapping in such a way so that this method isnt required.
			{
				console_dbuffer = (uint16_t*)console_dbuffer_original;
				_s = (uintptr_t)console_dbuffer;
				_c = console_dbuffer_limit-VGA_size;
				memcpy_rep((uint32_t)_s, (uint32_t)_c,VGA_size);
				memset_sse((uint32_t)_s, 0, (4194304-VGA_size)/8);

			}
		 }
		 asm volatile("int $50");
	 }
}

void __attribute__((optimize("O2"))) Shell_Dbuff_sync()
{
	 uint32_t* _s, *_c;
	 _s = (uint32_t*)console_buffer;
	 _c = (uint32_t*)console_dbuffer;

	 memcpy_rep((uint32_t)_s, (uint32_t)_c,VGA_size);
	 //asm volatile("cli");
	 if(console_skip)
	 {
		console_dbuffer += (VGA_WIDTH*console_skip);
		consolerow -= console_skip;
		console_skip = 0;
		if((uint32_t)console_dbuffer >= console_dbuffer_limit)   //TODO: Make the console buffer mapping in such a way so that this method isnt required.
		{
			console_dbuffer = (uint16_t*)console_dbuffer_original;
			_s = (uint32_t*)console_dbuffer;
			_c = console_dbuffer_limit-VGA_size;
			memcpy_rep((uint32_t)_s, (uint32_t)_c, VGA_size);
			memset_sse((uint32_t)_s, 0, (4194304-VGA_size)/8);

		}
	 }
}

void __attribute__((optimize("O2"))) Shell_Ostream()
{
	// TODO: Constantly keep getting strings and keep printing them on console.
	while(1)
	{
		asm volatile("cli");
		if(console_q_elements)
		{
			char tmp[console_Start_q->size];
			strcpy(tmp, (char*)console_Start_q->buffer);
			_print(tmp, console_Start_q->size);

			Task_wakeup(console_Start_q->task);
		//	printf("%s",tmp);
			//Shell_wakeup();
			memset_faster((uint32_t*)console_Start_q, 0, 4);
			console_Start_q = console_Start_q->next;
			--console_q_elements;
		}
		asm volatile("int $50");
	}
}


void __attribute__((optimize("O2"))) Shell_Input()
{
	while(1)
	{
		asm volatile("cli");
		if(enter_pressed)
		{
			if(kb_buff)
			{
				_putchar((int)'\n');
				*(Istream_ptr) = '\0';
				if(kb_q_elements)
				{
					char* tmp = (char*)((uint32_t)Istream_ptr - (uint32_t)kb_buff);
					++Istream_ptr;
					(*Istream_ptr) = kb_buff;
					++Istream_ptr;
					strcpy((char*)kb_Start_q->buffer, tmp);
					kb_buff = 0;

					Task_wakeup(kb_Start_q->task);
				//	printf("%s",tmp);
					//Shell_wakeup();
					memset_faster((uint32_t*)kb_Start_q, 0, 4);
					kb_Start_q = kb_Start_q->next;
					--kb_q_elements;
					//TODO: Load the next element
					Current_buf = (uint8_t*)kb_Start_q->buffer;
					Current_strlen = 0;
					if(_ctrl_C_pressed)
					{
						Task_Swap(Shell_task, (task_t*)shell_cmdTask);
						Task_sleep((task_t*)shell_cmdTask);
						shell_awake = 1;
					}
				}
				else if(shell_harbor)
				{
					//_printf("ALERT %d %d %d", kb_q_elements, shell_sleeping, shlock);
					//Shell_wakeup();
					shell_in = 1;
					shell_buf = Istream_ptr - kb_buff;
					kb_buff = 0;
				}
				else if(shell_awake)  //TODO: Give all input to the Shell Directly!
				{
					//_printf("AOO %d %d %d", kb_q_elements, shell_sleeping, shlock);
					//Priority_promoter(Shell_task);
					Task_wakeup(Shell_task);
					shell_in = 1;
					shell_buf = Istream_ptr - kb_buff;
					kb_buff = 0;
				}
				else
				{
					_printf("ALERT %d %d %d", kb_q_elements, shell_sleeping, shlock);
					//Shell_wakeup();

				}

				++Istream_ptr;
				*(Istream_ptr) = '\0';
				++Istream_ptr;
			}
			enter_pressed = 0;
		}
		asm volatile("int $50");
	}
}

void __attribute__((optimize("O0"))) Shell_CFexecute(uint32_t* buffer, uint32_t sz)
{
	//while(shell_in) asm volatile("int $50");
	//Shell_sleep();
	char* pt = (char*)buffer;
	uint32_t c = 0;
	pt[sz] = '\0';
	memset_faster((uint32_t*)CSI_mem_start, 0, 4);
	CSI_entries_ptr = (uint32_t*)&Main_CSI_struct->entries;
	while(c < sz)
	{
		char* pp = pt;
		for(; *pp != '\n' && *pp != '\0'; pp++);
		*pp = '\0';
		printf("\n->\"%s\"",pt);
		Shell_command_locator_CC(pt);
		*Shell_Istream = (uint32_t)pt;
		++Shell_Istream;
		//shell_in = 0;
		//shell_buf = 0;
		c += pp - pt + 1;
		pt = pp + 1;
	}
	//Shell_wakeup();
}

void Shell_SFexecute(char* command)
{
	//Shell_sleep();
	char* pt = command;
	memset_faster((uint32_t*)CSI_mem_start, 0, 2 + *tot_entries);
	CSI_entries_ptr = (uint32_t*)&Main_CSI_struct->entries;
	//printf("\n->\"%s\"",pt);
	Shell_command_locator_CC(pt);
	*Shell_Istream = (uint32_t)pt;
	++Shell_Istream;
	//Shell_wakeup();
}

void __attribute__((optimize("O0"))) Shell()
{
	while(1)
	{
		if(shell_awake)
		{
			printf("\n%g%s%g>%g",12,curr_dir.full_name,10,11);
			//Shell_sleep();
			Task_sleep(Shell_task);
			while(!shell_in)
			{
		//		printf("B%x--",KitList[1].reached_bottom);
				asm volatile("int $50");
			}
			//asm volatile("cli");
			printf("%g",15);
			//asm volatile("cli");
			//console_manager((char*)shell_buf);
			Shell_command_locator((char*)shell_buf);
			*Shell_Istream = (uint32_t)shell_buf;
			++Shell_Istream;
			//printf("%s",shell_buf);
			shell_in = 0;
			shell_buf = 0;
			tmpIstream = 0;
		}
		asm volatile("int $50");
	}
}

void Shell_scrollUp()
{
	if(!tmpIstream)
		tmpIstream = Shell_Istream;
	//for(int i = 0; i < strlen(shell_buf); i++) backspace();

	--tmpIstream;
	shell_buf	= (char*)tmpIstream;
	printf("->%s", shell_buf);
}

void Shell_scrollDown()
{
	if(tmpIstream == Shell_Istream) return;
	for(uint32_t i = 0; i < strlen((const char*)shell_buf); i++) backspace();

	++tmpIstream;
	shell_buf	= (char*)tmpIstream;
	printf("%s", shell_buf);
}

void Shell_sleep()  //TODO: Make the Shell task to sleep.
{

	LOCK(a);
	//printf("\nAppxx");
	if(shell_sleeping == 0)
	{
		Task_sleep(Shell_task);
		shell_awake = 0;
		shell_sleeping = 0;
	}
	else if (shell_sleeping < 0)
	{
		printf("\nError_537");
		asm volatile("hlt");
	}
	++shell_sleeping;
	//shlock = 0;
	UNLOCK(a);
	//asm volatile("sti");
}

void Shell_wakeup()
{

	LOCK(a);
	--shell_sleeping;
	//printf("---IOPPOIOPI--");
	if(shell_sleeping == 0)
	{
		//Shell_task->Scheduler = FindLightestScheduler();
		Task_wakeup(Shell_task);
		shell_awake = 1;
		shell_sleeping = 0;
	}
	else if (shell_sleeping < 0)
	{
		printf("\nError_537");
		asm volatile("hlt");
	}
	//shlock = 0;
	UNLOCK(a);
	//asm volatile("sti");
}

void del_eos(char str[], char ch)
{
	 int i, j = 0;
	 int size;
	 char ch1;
	 char str1[10];

	 size = strlen(str);

	 for (i = 0; i < size; i++)
	 {
			if (str[i] != ch)
			{
				 ch1 = str[i];
				 str1[j] = ch1;
				 j++;
			}
	 }
	 str1[j] = '\0';
}

volatile int shcml = 0;

void shcmlRemove()
{
	shcml = 0;
}

void Shell_cmdWrapper()
{
	//LOCK(SHELL_CMDTASK);
	asm volatile("cli");
	//Shell_sleep();
	shell_awake = 0;
	shell_cmdFunc();

	Task_Swap(Shell_task, (task_t*)shell_cmdTask);
	Task_sleep((task_t*)shell_cmdTask);
	shell_awake = 1;
	asm volatile("int $50");
	while(1)
		asm volatile("int $50");
}

const char* space = " ";

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
				 *tot_entries = 0;
				// char* tmp2str = tmpstr;
				 while(1) //Provide them the options and values provided by the user.
				 {
						tmpstr = strtok(NULL, " ");

						if(tmpstr== NULL) break;
						*CSI_entries_ptr = (uint32_t)tmpstr;

						if(tmpstr[0] == '\"')
						{
							++tmpstr;

							*CSI_entries_ptr = (uint32_t)tmpstr;

							for(;tmpstr[strlen(tmpstr)-1] != '\"';)
							{
								tmpstr[strlen(tmpstr)] = ' ';
								tmpstr = strtok(NULL, " ");
							}
							tmpstr[strlen(tmpstr)-1] = '\0';
						}

						++CSI_entries_ptr;
						++*tot_entries;

				 }
				 asm volatile("cli");
				 shell_cmdFunc = func;
			//	 _printf("23");
				 Task_Refresh(shell_cmdTask, (task_t*)Shell_cmdWrapper);
				 Task_Swap((task_t*)shell_cmdTask, Shell_task);

				 	asm volatile("int $50");
				 //Task_sleep(shell_cmdFunc);
//*/
				// func();
				 memset_faster((uint32_t*)CSI_mem_start, 0, 2 + *tot_entries);
				 CSI_entries_ptr = (uint32_t*)&Main_CSI_struct->entries;
				 return ((Shell_Commands_t*)ptr)->reserved;
			}
			else
			{
					_printf("\n Command Not Recognized! type help for help %x\n", tmpstr ); //TODO: Search within other possible files/executables like in the PATH string.
					return -1;
			}
	 }
	 else
	 {
			_printf("\n Command Not Recognized! type help for help\n");
			return -1;
	 }
}

int Shell_command_locator_CC(char *inst)
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
			//	_printf("\n Command %x tried: %s, key = %d, value at key: %s", tmpstr, tmpstr, tmp, ((Shell_Commands_t*)ptr)->command); //TODO: Search within other possible files/executables like in the PATH string.

				 func_t func = ((Shell_Commands_t*)ptr)->func;
				 *tot_entries = 0;
				// char* tmp2str = tmpstr;
				 for(; ;) //Provide them the options and values provided by the user.
				 {
						tmpstr = strtok(NULL, " ");

						if(tmpstr== NULL) break;
						*CSI_entries_ptr = (uint32_t)tmpstr;

						if(tmpstr[0] == '\"')
						{
							++tmpstr;

							*CSI_entries_ptr = (uint32_t)tmpstr;

							for(;tmpstr[strlen(tmpstr)-1] != '\"';)
							{
								tmpstr[strlen(tmpstr)] = ' ';
								tmpstr = strtok(NULL, " ");
							}
							tmpstr[strlen(tmpstr)-1] = '\0';
						}

						++CSI_entries_ptr;
						++*tot_entries;
						//printf(" %s", tmpstr);
				 }
				 func();

				 memset_faster((uint32_t*)CSI_mem_start, 0, 2 + *tot_entries);
				 CSI_entries_ptr = (uint32_t*)&Main_CSI_struct->entries;
				 return ((Shell_Commands_t*)ptr)->reserved;
			}
			else
			{
					_printf("\n Command Not Recognized! type help for help %x\nCommand tried: %s, key = %d, value at key: %s", tmpstr, tmpstr, tmp, ((Shell_Commands_t*)ptr)->command); //TODO: Search within other possible files/executables like in the PATH string.
					return -1;
			}
	 }
	 else
	 {
			_printf("\n Command Not Recognized! type help for help\n");
			return -1;
	 }
}

void Shell_Add_Commands(func_t func, uint32_t command_no, int flag, char* name)
{
	 Shell_Commands_t* command = (Shell_Commands_t*)kmalloc(sizeof(Shell_Commands_t));
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


void console_manager_init()
{
	 Shell_Add_Commands(Command_help, 108, 0, "help");
	 Shell_Add_Commands(Command_shutdown, 525, 0, "shutdown");
	 Shell_Add_Commands(Command_mdbug, 131, 0, "mdbug");
	 Shell_Add_Commands(Command_vesa, 83, 0, "vesa");
	 Shell_Add_Commands(Command_memmap, 194, 0, "memmap");
	 Shell_Add_Commands(Command_start_counter, 1423, 0, "counter_start");
	 Shell_Add_Commands(Command_counter, 380, 0, "counter");
	 Shell_Add_Commands(Command_timeslice, 351, 0, "timeslice");
	 Shell_Add_Commands(Command_topq, 156, 0, "topq");
	 Shell_Add_Commands(Command_test_structs, 1309, 0, "test_structs");
	 Shell_Add_Commands(Command_test, 157, 0, "test");
	 Shell_Add_Commands(Command_test_multi, 858, 0, "test_multi");
	 Shell_Add_Commands(Command_ls, 47, 0, "ls");
	 Shell_Add_Commands(Command_cd, 8, 0, "cd");
	 Shell_Add_Commands(Command_clrscr, 259, 0, "clrscr");
	 Shell_Add_Commands(Command_baseln, 204, 0, "baseln");
	 Shell_Add_Commands(Command_baseshow, 477, 0, "baseshow");
	 Shell_Add_Commands(Command_qelements, 562, 0, "qelements");
	 Shell_Add_Commands(Command_dbuffplusplus, 1344, 0, "dbuffplusplus");
	 Shell_Add_Commands(Command_detect_cpu, 177, 0, "decpu");
	 Shell_Add_Commands(Command_trate, 149, 0, "trate");
	 Shell_Add_Commands(Command_init, 134, 0, "init");
	 Shell_Add_Commands(Command_Numerise, 396, 0, "numerise");
	 Shell_Add_Commands(Command_testfs, 290, 0, "testfs");
	 Shell_Add_Commands(Command_secalloc, 267, 0, "secalloc");
	 Shell_Add_Commands(Command_mkdir, 158, 0, "mkdir");
	 Shell_Add_Commands(Command_mkfl, 91, 0, "mkfl");
	 Shell_Add_Commands(Command_lspci, 140, 0, "lspci");
	 Shell_Add_Commands(Command_water, 180, 0, "water");
	 Shell_Add_Commands(Command_editfl, 201, 0, "editfl");
	 Shell_Add_Commands(Command_cp, 32, 0, "cp");
	 Shell_Add_Commands(Command_aptest, 307, 0, "aptest");
	 Shell_Add_Commands(Command_proc, 99, 0, "proc");
	 Shell_Add_Commands(Command_rn, 43, 0, "rn");
	 Shell_Add_Commands(Command_ann, 65, 0, "ann");
	 Shell_Add_Commands(Command_echo, 85, 0, "echo");
	 Shell_Add_Commands(Command_cat, 59, 0, "cat");
	 Shell_Add_Commands(Command_rm, 41, 0, "rm");
	 Shell_Add_Commands(Command_cpu, 92, 0, "cpu");
	 Shell_Add_Commands(Command_elfload, 176, 0, "elfload");
	 memset_faster((uint32_t*)CSI_mem_start, 0, 66);
}
