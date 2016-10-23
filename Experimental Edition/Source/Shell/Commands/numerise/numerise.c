#include "shell.h"
#include "common.h"
#include "stdio.h"
#include "string.h"

void Command_Numerise()
{
   uint32_t tmp = 0;
   char* tmpstr = CSI_Read(1);
   //uint32_t spaces = stroccr(inst, ' ');
   //int CSI_entries_ptr = 0;
   /*
   for( int i = 0 ; inst[i]!='\0' ; i++ )    //Alternate Algorithm
   {
      CSI_entries_ptr = (((int)inst[i])-((int)inst[i+1]));
      if(CSI_entries_ptr >= 0)
         tmp += CSI_entries_ptr*(i+1);
      else
         tmp += (-CSI_entries_ptr)*(i+1);
}*/
   int i;
   for( i = 0 ; tmpstr[i]!='\0' && i <= 16; i++ )
   {
      if((uint32_t)tmpstr[i] >= 97)
         tmp += (((uint32_t)tmpstr[i]) - 97)*(i+1);
      else
         tmp += (((uint32_t)tmpstr[i]) - 65)*(i+1);
   }
   _printf("\nNumber generated: %x\n",tmp);
   if(tmp <= 2048)
   {
      uint32_t* ptr = (uint32_t*)Shell_Commands_list[tmp];
      if(ptr)
      {
        _printf("\n THIS COMMAND EXISTS\n");
      }
      else
      {
    	  _printf("\n This Command does not exist\n"); //TODO: Search within other possible files/executables like in the PATH string.
        //return -1;
      }
   }
   else
   {
      _printf("\n This Command does not exist\n");
      //return -1;
   }
}
