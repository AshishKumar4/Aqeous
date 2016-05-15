#ifndef KEYBOARD_h
#define KEYBOARD_h

#include "scancodes.h"

enum KYBRD_ENCODER_IO {

	KYBRD_ENC_INPUT_BUF	=	0x60,
	KYBRD_ENC_CMD_REG	=	0x60
};

enum KYBRD_ENC_CMDS {

	KYBRD_ENC_CMD_SET_LED				=	0xED,
	KYBRD_ENC_CMD_ECHO					=	0xEE,
	KYBRD_ENC_CMD_SCAN_CODE_SET			=	0xF0,
	KYBRD_ENC_CMD_ID					=	0xF2,
	KYBRD_ENC_CMD_AUTODELAY				=	0xF3,
	KYBRD_ENC_CMD_ENABLE				=	0xF4,
	KYBRD_ENC_CMD_RESETWAIT				=	0xF5,
	KYBRD_ENC_CMD_RESETSCAN				=	0xF6,
	KYBRD_ENC_CMD_ALL_AUTO				=	0xF7,
	KYBRD_ENC_CMD_ALL_MAKEBREAK			=	0xF8,
	KYBRD_ENC_CMD_ALL_MAKEONLY			=	0xF9,
	KYBRD_ENC_CMD_ALL_MAKEBREAK_AUTO	=	0xFA,
	KYBRD_ENC_CMD_SINGLE_AUTOREPEAT		=	0xFB,
	KYBRD_ENC_CMD_SINGLE_MAKEBREAK		=	0xFC,
	KYBRD_ENC_CMD_SINGLE_BREAKONLY		=	0xFD,
	KYBRD_ENC_CMD_RESEND				=	0xFE,
	KYBRD_ENC_CMD_RESET					=	0xFF
};

// keyboard controller ---------------------------------------

enum KYBRD_CTRL_IO {

	KYBRD_CTRL_STATS_REG=	0x64,
	KYBRD_CTRL_CMD_REG	=	0x64
};

enum KYBRD_CTRL_STATS_MASK {

	KYBRD_CTRL_STATS_MASK_OUT_BUF	=	1,		//00000001
	KYBRD_CTRL_STATS_MASK_IN_BUF	=	2,		//00000010
	KYBRD_CTRL_STATS_MASK_SYSTEM	=	4,		//00000100
	KYBRD_CTRL_STATS_MASK_CMD_DATA	=	8,		//00001000
	KYBRD_CTRL_STATS_MASK_LOCKED	=	0x10,	//00010000
	KYBRD_CTRL_STATS_MASK_AUX_BUF	=	0x20,	//00100000
	KYBRD_CTRL_STATS_MASK_TIMEOUT	=	0x40,	//01000000
	KYBRD_CTRL_STATS_MASK_PARITY	=	0x80	//10000000
};

enum KYBRD_CTRL_CMDS {

	KYBRD_CTRL_CMD_READ				=	0x20,
	KYBRD_CTRL_CMD_WRITE			=	0x60,
	KYBRD_CTRL_CMD_SELF_TEST		=	0xAA,
	KYBRD_CTRL_CMD_INTERFACE_TEST	=	0xAB,
	KYBRD_CTRL_CMD_DISABLE			=	0xAD,
	KYBRD_CTRL_CMD_ENABLE			=	0xAE,
	KYBRD_CTRL_CMD_READ_IN_PORT		=	0xC0,
	KYBRD_CTRL_CMD_READ_OUT_PORT	=	0xD0,
	KYBRD_CTRL_CMD_WRITE_OUT_PORT	=	0xD1,
	KYBRD_CTRL_CMD_READ_TEST_INPUTS	=	0xE0,
	KYBRD_CTRL_CMD_SYSTEM_RESET		=	0xFE,
	KYBRD_CTRL_CMD_MOUSE_DISABLE	=	0xA7,
	KYBRD_CTRL_CMD_MOUSE_ENABLE		=	0xA8,
	KYBRD_CTRL_CMD_MOUSE_PORT_TEST	=	0xA9,
	KYBRD_CTRL_CMD_MOUSE_WRITE		=	0xD4
};

// scan error codes ------------------------------------------

enum KYBRD_ERROR {

	KYBRD_ERR_BUF_OVERRUN			=	0,
	KYBRD_ERR_ID_RET				=	0x83AB,
	KYBRD_ERR_BAT					=	0xAA,	//note: can also be L. shift key make code
	KYBRD_ERR_ECHO_RET				=	0xEE,
	KYBRD_ERR_ACK					=	0xFA,
	KYBRD_ERR_BAT_FAILED			=	0xFC,
	KYBRD_ERR_DIAG_FAILED			=	0xFD,
	KYBRD_ERR_RESEND_CMD			=	0xFE,
	KYBRD_ERR_KEY					=	0xFF
};

volatile uint32_t call=0,_ctrl=0,_shift=0,_alt=0,_numlock=0,_capslock=0,_scrolllock=0;
volatile uint32_t* kb_buf = 0;

//! read status from keyboard controller
inline uint8_t kybrd_ctrl_read_status ()
{
	return inb (KYBRD_CTRL_STATS_REG);
}
//! send command byte to keyboard encoder
inline void kybrd_enc_send_cmd (uint8_t cmd)
{
	//! wait for kkybrd controller input buffer to be clear
	while (1)
		if ( (kybrd_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0)
			break;

	//! send command byte to kybrd encoder
	outb (KYBRD_ENC_CMD_REG, cmd);
}
//! sets leds
inline void kkybrd_set_leds (int num, int caps, int scroll)
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

int keyboard_scancodes(int key);

uint32_t StrToInt(char *str);

void keyboard_init();

#endif
