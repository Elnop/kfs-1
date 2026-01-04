#include "vga.h"
#include "../lib/string.h"

#define VGA_MEMORY 0xB8000

#define VGA_CTRL_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

static uint16_t *vga_buffer = (uint16_t *)VGA_MEMORY;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t current_color = 0x0F;

static inline void outb(uint16_t port, uint8_t value)
{
	__asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline uint16_t vga_entry(char c, uint8_t color)
{
	return (uint16_t)c | ((uint16_t)color << 8);
}

static inline uint8_t vga_entry_color(uint8_t fg, uint8_t bg)
{
	return fg | (bg << 4);
}

void vga_init(void)
{
	cursor_x = 0;
	cursor_y = 0;
	current_color = vga_entry_color(VGA_WHITE, VGA_BLACK);
}

void vga_clear(void)
{
	uint16_t blank = vga_entry(' ', current_color);

	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		vga_buffer[i] = blank;

	cursor_x = 0;
	cursor_y = 0;
	vga_update_cursor();
}

void vga_scroll(void)
{
	for (size_t row = 0; row < VGA_HEIGHT - 1; row++) {
		for (size_t col = 0; col < VGA_WIDTH; col++) {
			vga_buffer[row * VGA_WIDTH + col] =
				vga_buffer[(row + 1) * VGA_WIDTH + col];
		}
	}

	uint16_t blank = vga_entry(' ', current_color);
	for (size_t col = 0; col < VGA_WIDTH; col++)
		vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = blank;
}

void vga_putchar(char c)
{
	if (c == '\n') {
		cursor_x = 0;
		cursor_y++;
	} else if (c == '\r') {
		cursor_x = 0;
	} else if (c == '\t') {
		cursor_x = (cursor_x + 4) & ~(4 - 1);
	} else if (c == '\b') {
		if (cursor_x > 0)
			cursor_x--;
	} else {
		size_t index = cursor_y * VGA_WIDTH + cursor_x;
		vga_buffer[index] = vga_entry(c, current_color);
		cursor_x++;
	}

	if (cursor_x >= VGA_WIDTH) {
		cursor_x = 0;
		cursor_y++;
	}

	if (cursor_y >= VGA_HEIGHT) {
		vga_scroll();
		cursor_y = VGA_HEIGHT - 1;
	}

	vga_update_cursor();
}

void vga_puts(const char *str)
{
	while (*str)
		vga_putchar(*str++);
}

void vga_set_color(uint8_t fg, uint8_t bg)
{
	current_color = vga_entry_color(fg, bg);
}

uint8_t vga_get_color(void)
{
	return current_color;
}

void vga_set_cursor(uint8_t x, uint8_t y)
{
	if (x < VGA_WIDTH && y < VGA_HEIGHT) {
		cursor_x = x;
		cursor_y = y;
		vga_update_cursor();
	}
}

void vga_get_cursor(uint8_t *x, uint8_t *y)
{
	if (x)
		*x = cursor_x;
	if (y)
		*y = cursor_y;
}

void vga_update_cursor(void)
{
	uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

	outb(VGA_CTRL_PORT, 0x0F);
	outb(VGA_DATA_PORT, (uint8_t)(pos & 0xFF));

	outb(VGA_CTRL_PORT, 0x0E);
	outb(VGA_DATA_PORT, (uint8_t)((pos >> 8) & 0xFF));
}
