#include "keyboard.h"

#define KB_DATA_PORT	0x60
#define KB_STATUS_PORT	0x64

#define SC_LSHIFT_PRESS		0x2A
#define SC_RSHIFT_PRESS		0x36
#define SC_LSHIFT_RELEASE	0xAA
#define SC_RSHIFT_RELEASE	0xB6
#define SC_CTRL_PRESS		0x1D
#define SC_CTRL_RELEASE		0x9D
#define SC_ALT_PRESS		0x38
#define SC_ALT_RELEASE		0xB8

static uint8_t modifiers = 0;

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

/* Scancode to ASCII table (US layout, unshifted) */
static const char scancode_to_ascii[] = {
	0,   0x1B, '1',  '2',  '3',  '4',  '5',  '6',  /* 0x00-0x07 */
	'7', '8',  '9',  '0',  '-',  '=',  '\b', '\t', /* 0x08-0x0F */
	'q', 'w',  'e',  'r',  't',  'y',  'u',  'i',  /* 0x10-0x17 */
	'o', 'p',  '[',  ']',  '\n', 0,    'a',  's',  /* 0x18-0x1F */
	'd', 'f',  'g',  'h',  'j',  'k',  'l',  ';',  /* 0x20-0x27 */
	'\'','`',  0,    '\\', 'z',  'x',  'c',  'v',  /* 0x28-0x2F */
	'b', 'n',  'm',  ',',  '.',  '/',  0,    '*',  /* 0x30-0x37 */
	0,   ' ',  0,    0,    0,    0,    0,    0,    /* 0x38-0x3F */
	0,   0,    0,    0,    0,    0,    0,    '7',  /* 0x40-0x47 */
	'8', '9',  '-',  '4',  '5',  '6',  '+',  '1',  /* 0x48-0x4F */
	'2', '3',  '0',  '.'                           /* 0x50-0x53 */
};

/* Scancode to ASCII table (US layout, shifted) */
static const char scancode_to_ascii_shift[] = {
	0,   0x1B, '!',  '@',  '#',  '$',  '%',  '^',  /* 0x00-0x07 */
	'&', '*',  '(',  ')',  '_',  '+',  '\b', '\t', /* 0x08-0x0F */
	'Q', 'W',  'E',  'R',  'T',  'Y',  'U',  'I',  /* 0x10-0x17 */
	'O', 'P',  '{',  '}',  '\n', 0,    'A',  'S',  /* 0x18-0x1F */
	'D', 'F',  'G',  'H',  'J',  'K',  'L',  ':',  /* 0x20-0x27 */
	'"', '~',  0,    '|',  'Z',  'X',  'C',  'V',  /* 0x28-0x2F */
	'B', 'N',  'M',  '<',  '>',  '?',  0,    '*',  /* 0x30-0x37 */
	0,   ' ',  0,    0,    0,    0,    0,    0,    /* 0x38-0x3F */
	0,   0,    0,    0,    0,    0,    0,    '7',  /* 0x40-0x47 */
	'8', '9',  '-',  '4',  '5',  '6',  '+',  '1',  /* 0x48-0x4F */
	'2', '3',  '0',  '.'                           /* 0x50-0x53 */
};

void keyboard_init(void)
{
	modifiers = 0;
}

uint8_t keyboard_read_scancode(void)
{
	while (!(inb(KB_STATUS_PORT) & 0x01))
		;
	return inb(KB_DATA_PORT);
}

int keyboard_available(void)
{
	return (inb(KB_STATUS_PORT) & 0x01) != 0;
}

uint8_t keyboard_get_modifiers(void)
{
	return modifiers;
}

char keyboard_scancode_to_char(uint8_t scancode)
{
	switch (scancode) {
	case SC_LSHIFT_PRESS:
	case SC_RSHIFT_PRESS:
		modifiers |= MOD_SHIFT;
		return 0;
	case SC_LSHIFT_RELEASE:
	case SC_RSHIFT_RELEASE:
		modifiers &= ~MOD_SHIFT;
		return 0;
	case SC_CTRL_PRESS:
		modifiers |= MOD_CTRL;
		return 0;
	case SC_CTRL_RELEASE:
		modifiers &= ~MOD_CTRL;
		return 0;
	case SC_ALT_PRESS:
		modifiers |= MOD_ALT;
		return 0;
	case SC_ALT_RELEASE:
		modifiers &= ~MOD_ALT;
		return 0;
	}

	if (scancode & 0x80)
		return 0;

	if (scancode >= sizeof(scancode_to_ascii))
		return 0;

	if (modifiers & MOD_SHIFT)
		return scancode_to_ascii_shift[scancode];
	else
		return scancode_to_ascii[scancode];
}

int keyboard_get_char(void)
{
	while (1) {
		uint8_t scancode = keyboard_read_scancode();

		if (scancode >= KEY_F1 && scancode <= KEY_F10) {
			if (!(scancode & 0x80))
				return scancode;
			continue;
		}

		if (scancode == KEY_UP || scancode == KEY_DOWN ||
		    scancode == KEY_LEFT || scancode == KEY_RIGHT) {
			return scancode;
		}

		char c = keyboard_scancode_to_char(scancode);
		if (c != 0)
			return c;
	}
}
