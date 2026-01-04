#include "../include/multiboot.h"
#include "../include/types.h"
#include "vga.h"
#include "keyboard.h"
#include "screen.h"
#include "../lib/printk.h"

static void display_welcome_message(void)
{
	printk("42\n\n");
	vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
	printk("Welcome to KFS-1 Kernel!\n");
	vga_set_color(VGA_WHITE, VGA_BLACK);
	printk("=======================\n\n");
	printk("Features:\n");
	printk("  - VGA text mode driver\n");
	printk("  - Color support (16 colors)\n");
	printk("  - Hardware cursor\n");
	printk("  - Automatic scrolling\n");
	printk("  - Keyboard input\n");
	printk("  - Multi-screen support (F1-F5)\n");
	printk("  - printf-style formatting\n\n");
	vga_set_color(VGA_YELLOW, VGA_BLACK);
	printk("Press F1-F5 to switch screens\n");
	printk("Type to see keyboard input\n\n");
	vga_set_color(VGA_WHITE, VGA_BLACK);
}

static void init_all_screens(void)
{
	for (int i = 0; i < 5; i++) {
		screen_switch(i);
		vga_set_color(VGA_LIGHT_CYAN, VGA_BLACK);
		printk("[Screen %d]\n\n", i + 1);
		vga_set_color(VGA_WHITE, VGA_BLACK);
		if (i == 0) {
			display_welcome_message();
		}
	}
	screen_switch(0);
}

static void keyboard_loop(void)
{
	while (1) {
		int key = keyboard_get_char();

		if (key >= KEY_F1 && key <= KEY_F5) {
			uint8_t screen_num = key - KEY_F1;
			screen_switch(screen_num);
		} else {
			printk("%c", (char)key);
		}
	}
}

void kernel_main(uint32_t magic, uint32_t addr)
{
	(void)magic;
	(void)addr;

	vga_init();
	vga_clear();
	keyboard_init();
	screen_init();

	init_all_screens();
	keyboard_loop();
}
