#include "sys.h"
#include "keyboard.h"
#include "scancodes.h"

int keyboard_scancodes(int key)
{
  if (_capslock || _shift)
    if (key >= 'a' && key <= 'z')
      key -= 32;

  if (_capslock && _shift)
    if (key >= 'A' && key <= 'Z')
      key += 32;

		if (_shift)
			{
				if (key >= '0' && key <= '9')
				switch (key)
        {

					case '0':
						key = KEY_RIGHTPARENTHESIS;
						break;
					case '1':
						key = KEY_EXCLAMATION;
						break;
					case '2':
						key = KEY_AT;
						break;
					case '3':
						key = KEY_HASH;
						break;
					case '4':
						key = KEY_DOLLAR;
						break;
					case '5':
						key = KEY_PERCENT;
						break;
					case '6':
						key = KEY_CARRET;
						break;
					case '7':
						key = KEY_AMPERSAND;
						break;
					case '8':
						key = KEY_ASTERISK;
						break;
					case '9':
						key = KEY_LEFTPARENTHESIS;
						break;
				}
			else
      {

				switch (key) {
					case KEY_COMMA:
						key = KEY_LESS;
						break;

					case KEY_DOT:
						key = KEY_GREATER;
						break;

					case KEY_SLASH:
						key = KEY_QUESTION;
						break;

					case KEY_SEMICOLON:
						key = KEY_COLON;
						break;

					case KEY_QUOTE:
						key = KEY_QUOTEDOUBLE;
						break;

					case KEY_LEFTBRACKET :
						key = KEY_LEFTCURL;
						break;

					case KEY_RIGHTBRACKET :
						key = KEY_RIGHTCURL;
						break;

					case KEY_GRAVE:
						key = KEY_TILDE;
						break;

					case KEY_MINUS:
						key = KEY_UNDERSCORE;
						break;

					case KEY_PLUS:
						key = KEY_EQUAL;
						break;

					case KEY_BACKSLASH:
						key = KEY_BAR;
						break;
				}
			}
		}
    return key;
}
/* Handles the keyboard interrupt */
extern void keyboardInterrupt_handler();

int checker(int i)
{
  printint(0);
  if(i) return 1;
  return 0;
}

uint8_t getch()
{
	asm volatile("sti");
  while(!call){}
	asm volatile("cli");
  uint8_t key=keyboard_scancodes(call);
  call=0;
  return key;
}

uint8_t getchar()
{
  back:
	asm volatile("sti");
  while(!call){}
	asm volatile("cli");
	if(!call)
		printf("wtf");
  uint8_t key=keyboard_scancodes(call);
  if(key!='\r' && key!='\0' && key!='\b')
    printf("%c",key);
  call=0;
  if(key=='\b')
  {
    backspace();
    goto back;
  }
  return key;
}

void getline(char* string)
{
  uint8_t buff;
  for(int i=0;;i++)
  {
    buff=getchar();
		asm volatile("cli");
    if(buff=='\r')
    {
      string[i]='\0';
      break;
    }
    else
		{
			string[i]=buff;
			//printf("a %c ",string[i]);
		}
  }
  printf("\n");
}

uint32_t StrToInt(char *str)
{
    uint32_t in=0;
    int ln=strlen(str);
    int arr[999],a=1;
    for(int i=0;i<=ln;i++) a=a*10;
    a=a/100;
    for(int i=0;i<=ln;i++)
    {
        arr[i]=str[i]-48;
        in=in+arr[i]*a;
        a=a/10;
    }
		return in;
}

uint32_t getint()
{
	uint32_t var=0;
	char *temp=" ";
	getline(temp);
	var=StrToInt(temp);
	return var;
}

void keyboard_init()
{
  kkybrd_set_leds(1,1,1);
  scancodes=scancode1;
}
