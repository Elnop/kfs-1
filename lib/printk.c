#include "printk.h"
#include "string.h"
#include "../kernel/vga.h"

typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)

static void int_to_string(unsigned int value, char *str, int base, int is_negative)
{
	const char *digits = "0123456789abcdef";
	char buffer[32];
	int i = 0;
	if (value == 0) {
		str[0] = '0';
		str[1] = '\0';
		return;
	}
	while (value > 0) {
		buffer[i++] = digits[value % base];
		value /= base;
	}
	int pos = 0;
	if (is_negative) {
		str[pos++] = '-';
	}
	while (i > 0) {
		str[pos++] = buffer[--i];
	}

	str[pos] = '\0';
}

static void to_uppercase(char *str)
{
	while (*str) {
		if (*str >= 'a' && *str <= 'f') {
			*str = *str - 'a' + 'A';
		}
		str++;
	}
}

int printk(const char *format, ...)
{
	va_list args;
	const char *ptr;
	char buffer[32];
	int count = 0;

	va_start(args, format);

	for (ptr = format; *ptr; ptr++) {
		if (*ptr != '%') {
			vga_putchar(*ptr);
			count++;
			continue;
		}
		ptr++;
		switch (*ptr) {
		case 'c':
			vga_putchar((char)va_arg(args, int));
			count++;
			break;

		case 's':
			{
				const char *str = va_arg(args, const char *);
				if (str) {
					vga_puts(str);
					count += strlen(str);
				} else {
					vga_puts("(null)");
					count += 6;
				}
			}
			break;

		case 'd':
		case 'i':
			{
				int val = va_arg(args, int);
				int is_negative = (val < 0);
				unsigned int uval = is_negative ? -val : val;
				int_to_string(uval, buffer, 10, is_negative);
				vga_puts(buffer);
				count += strlen(buffer);
			}
			break;

		case 'u':
			int_to_string(va_arg(args, unsigned int), buffer, 10, 0);
			vga_puts(buffer);
			count += strlen(buffer);
			break;

		case 'x':
			int_to_string(va_arg(args, unsigned int), buffer, 16, 0);
			vga_puts(buffer);
			count += strlen(buffer);
			break;

		case 'X':
			int_to_string(va_arg(args, unsigned int), buffer, 16, 0);
			to_uppercase(buffer);
			vga_puts(buffer);
			count += strlen(buffer);
			break;

		case '%':
			vga_putchar('%');
			count++;
			break;

		default:
			vga_putchar('%');
			vga_putchar(*ptr);
			count += 2;
			break;
		}
	}

	va_end(args);
	return count;
}
