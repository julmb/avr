#ifndef display_h
#define display_h

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
	uint8_t character_count;
}
display;

uint8_t display_read_data(display display, uint8_t data)
{
	port_set_input(display.data);

	pin_write(display.register_select, data);
	pin_set(display.read_write);

	pin_set(display.enable);
	_delay_ms(1);

	uint8_t value = port_read(display.data);

	pin_clear(display.enable);
	_delay_ms(1);

	return value;
}
void display_write_data(display display, uint8_t data, uint8_t value)
{
	port_set_output(display.data);

	pin_write(display.register_select, data);
	pin_clear(display.read_write);

	pin_set(display.enable);
	_delay_ms(1);

	port_write(display.data, value);

	pin_clear(display.enable);
	_delay_ms(1);
}
void display_write_characters(display display, char* characters)
{
	uint8_t finished = 0;

	for (uint8_t index = 0; index < display.character_count; index++)
	{
		if (characters[index] == 0) finished = 1;

		display_write_data(display, 0, 0x80 | (index > 7 ? 0x40 + (index - 8) : index));
		display_write_data(display, 1, finished ? 0x20 : characters[index]);
	}
}
void display_printf(display display, const char* format, ...)
{
	char buffer[display.character_count + 1];

	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buffer, display.character_count + 1, format, argptr);
	va_end(argptr);

	display_write_characters(display, buffer);
}

display display_initialize(pin register_select, pin read_write, pin enable, port data, uint8_t character_count)
{
	display display =
	{
		.register_select = register_select,
		.read_write = read_write,
		.enable = enable,
		.data = data,
		.character_count = character_count
	};

	pin_set_output(register_select);
	pin_set_output(read_write);
	pin_set_output(enable);

	// clear display
	display_write_data(display, 0, 0b00000001);

	// hide cursor
	display_write_data(display, 0, 0b00001100);

	// set display size
	display_write_data(display, 0, 0b00111000);

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
