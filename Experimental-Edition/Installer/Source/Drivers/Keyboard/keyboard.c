#include "sys.h"
#include "keyboard.h"
#include "scancodes.h"

static int call=0,_ctrl=0,_shift=0,_alt=0,_numlock=0,_capslock=0,_scrolllock=0;

//! read status from keyboard controller
uint8_t kybrd_ctrl_read_status ()
{
	return inb (KYBRD_CTRL_STATS_REG);
}
//! send command byte to keyboard encoder
void kybrd_enc_send_cmd (uint8_t cmd)
{
	//! wait for kkybrd controller input buffer to be clear
	while (1)
		if ( (kybrd_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0)
			break;

	//! send command byte to kybrd encoder
	outb (KYBRD_ENC_CMD_REG, cmd);
}
//! sets leds
void kkybrd_set_leds (int num, int caps, int scroll)
{

	uint8_t data = 0;
	//! set or clear the bit
	data = (scroll) ? (data | 1) : (data & 1);
	data = (num) ? (num | 2) : (num & 2);
	data = (caps) ? (num | 4) : (num & 4);

	//! send the command -- update keyboard Light Emetting Diods (LEDs)
	kybrd_enc_send_cmd (KYBRD_ENC_CMD_SET_LED);
	kybrd_enc_send_cmd (data);
}

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
void keyboard_handler()//(struct regs *r)
{
  if(kybrd_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_OUT_BUF)
  {
    int scancode=inb(0x60);
		if (scancode == 0xE0 || scancode == 0xE1)
      printf("extended ");
    else
    {
      if(scancode&0x80) //break code
      {
        scancode-=0x80; //make it make code
        int key=scancodes[scancode];
    		//! test if a special key has been released & set it
    		switch (key)
        {

    					case KEY_LCTRL:
    					case KEY_RCTRL:
    						_ctrl = 0;
    						break;

    					case KEY_LSHIFT:
    					case KEY_RSHIFT:
    						_shift = 0;
    						break;

    					case KEY_LALT:
    					case KEY_RALT:
    						_alt = 0;
    						break;
    			}
      }
      else //it is a make code
      {
        int key=scancodes[scancode];
        //printint(scancode);
        //! test if a special key has been released & set it
    		switch (key)
        {

    					case KEY_LCTRL:
                _ctrl = 1;
                break;
    					case KEY_RCTRL:
    						_ctrl = 2;
    						break;

    					case KEY_LSHIFT:
                _shift = 1;
                break;
    					case KEY_RSHIFT:
    						_shift = 2;
    						break;

    					case KEY_LALT:
                _alt = 1;
                break;
    					case KEY_RALT:
    						_alt = 2;
    						break;

    					case KEY_CAPSLOCK:
    						_capslock = (_capslock) ? false : true;
    						kkybrd_set_leds (_numlock, _capslock, _scrolllock);
    						break;

    					case KEY_KP_NUMLOCK:
    						_numlock = (_numlock) ? false : true;
    						kkybrd_set_leds (_numlock, _capslock, _scrolllock);
    						break;

    					case KEY_SCROLLLOCK:
    						_scrolllock = (_scrolllock) ? false : true;
    						kkybrd_set_leds (_numlock, _capslock, _scrolllock);
    						break;
              default:
              call=key;
    			}
        }
      }
  }
  //asm volatile("iret");
}

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
  register_interrupt_handler(IRQ1,keyboard_handler);
  kkybrd_set_leds(1,1,1);
  scancodes=scancode1;
}
