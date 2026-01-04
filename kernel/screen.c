#include "screen.h"
#include "vga.h"
#include "../lib/string.h"

#define VGA_MEMORY 0xB8000
#define VGA_SIZE (VGA_WIDTH * VGA_HEIGHT * 2)

typedef struct {
	uint16_t buffer[VGA_WIDTH * VGA_HEIGHT];
	uint8_t cursor_x;
	uint8_t cursor_y;
	uint8_t color;
} screen_t;

static screen_t screens[MAX_SCREENS];
static uint8_t active_screen = 0;
static uint16_t *vga_memory = (uint16_t *)VGA_MEMORY;

void screen_init(void)
{
	for (int i = 0; i < MAX_SCREENS; i++) {
		memset(screens[i].buffer, 0, sizeof(screens[i].buffer));
		screens[i].cursor_x = 0;
		screens[i].cursor_y = 0;
		screens[i].color = 0x0F;
	}

	active_screen = 0;

	screen_save(0);
}

void screen_save(uint8_t screen_num)
{
	if (screen_num >= MAX_SCREENS)
		return;

	memcpy(screens[screen_num].buffer, vga_memory,
	       sizeof(screens[screen_num].buffer));

	vga_get_cursor(&screens[screen_num].cursor_x,
	               &screens[screen_num].cursor_y);
	screens[screen_num].color = vga_get_color();
}

void screen_restore(uint8_t screen_num)
{
	if (screen_num >= MAX_SCREENS)
		return;

	memcpy(vga_memory, screens[screen_num].buffer,
	       sizeof(screens[screen_num].buffer));

	vga_set_cursor(screens[screen_num].cursor_x,
	               screens[screen_num].cursor_y);
	vga_set_color(screens[screen_num].color & 0x0F,
	              screens[screen_num].color >> 4);
}

void screen_switch(uint8_t screen_num)
{
	if (screen_num >= MAX_SCREENS)
		return;

	if (screen_num == active_screen)
		return;

	screen_sync_current();

	active_screen = screen_num;
	screen_restore(active_screen);
}

uint8_t screen_get_active(void)
{
	return active_screen;
}

void screen_sync_current(void)
{
	screen_save(active_screen);
}
