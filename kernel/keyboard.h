#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../include/types.h"

#define KEY_ESC		0x01
#define KEY_F1		0x3B
#define KEY_F2		0x3C
#define KEY_F3		0x3D
#define KEY_F4		0x3E
#define KEY_F5		0x3F
#define KEY_F6		0x40
#define KEY_F7		0x41
#define KEY_F8		0x42
#define KEY_F9		0x43
#define KEY_F10		0x44

#define KEY_UP		0x48
#define KEY_DOWN	0x50
#define KEY_LEFT	0x4B
#define KEY_RIGHT	0x4D

#define MOD_SHIFT	0x01
#define MOD_CTRL	0x02
#define MOD_ALT		0x04

void keyboard_init(void);

uint8_t keyboard_read_scancode(void);

char keyboard_scancode_to_char(uint8_t scancode);

int keyboard_get_char(void);

int keyboard_available(void);

uint8_t keyboard_get_modifiers(void);

#endif
