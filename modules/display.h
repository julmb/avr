#ifndef display_h
#define display_h

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#include <util/delay.h>

#include "gpio.h"

typedef struct
{
	pin register_select;
	pin read_write;
	pin enable;
	port data;
}
display;

void display_write(display display, uint8_t register_select, uint8_t value)
{
	port_set_output(display.data);

	pin_write(display.register_select, register_select);
	pin_clear(display.read_write);
	port_write(display.data, value);
	_delay_us(100);

	pin_set(display.enable);
	_delay_us(100);

	pin_clear(display.enable);
	_delay_us(100);
}

void display_write_characters(display display, char* characters)
{
	uint8_t finished = 0;

	for (uint8_t index = 0; index < 16; index++)
	{
		if (characters[index] == 0) finished = 1;

		display_write(display, 0, 0x80 | (index > 7 ? 0x40 + (index - 8) : index));
		display_write(display, 1, finished ? 0x20 : characters[index]);
	}
}
void display_printf(display display, const char* format, ...)
{
	char buffer[16 + 1];

	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buffer, 16 + 1, format, argptr);
	va_end(argptr);

	display_write_characters(display, buffer);
}

display display_initialize(pin register_select, pin read_write, pin enable, port data)
{
	display display =
	{
		.register_select = register_select,
		.read_write = read_write,
		.enable = enable,
		.data = data
	};

	pin_set_output(register_select);
	pin_clear(register_select);

	pin_set_output(read_write);
	pin_clear(read_write);

	pin_set_output(enable);
	pin_clear(enable);

	// clear display, reset cursor
	display_write(display, 0, 0b00000001);
	_delay_us(1640);

	// enable display, disable cursor, disable blinking
	display_write(display, 0, 0b00001100);
	_delay_us(40);

	// enable 8-bit mode, enable double-line (8+8) display, set font to 5x7 dots
	display_write(display, 0, 0b00111000);
	_delay_us(40);

	return display;
}
void display_dispose(display display)
{
	pin_dispose(display.register_select);
	pin_dispose(display.read_write);
	pin_dispose(display.enable);
	port_dispose(display.data);
}

#endif
