#ifndef display_h
#define display_h

#include <stdarg.h>
#include <stdio.h>
#include <util/delay.h>

#include "port.h"
#include "data_port.h"

typedef struct
{
	port register_select;
	port read_write;
	port enable;
	data_port data;
	uint8_t character_count;
}
display;

uint8_t display_read_data(display display, uint8_t data)
{
	data_port_set_input(display.data);

	port_write(display.register_select, data);
	port_set(display.read_write);

	port_set(display.enable);
	_delay_ms(1);

	uint8_t value = data_port_read(display.data);

	port_clear(display.enable);
	_delay_ms(1);

	return value;
}
void display_write_data(display display, uint8_t data, uint8_t value)
{
	data_port_set_output(display.data);

	port_write(display.register_select, data);
	port_clear(display.read_write);

	port_set(display.enable);
	_delay_ms(1);

	data_port_write(display.data, value);

	port_clear(display.enable);
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

display display_initialize(port register_select, port read_write, port enable, data_port data, uint8_t character_count)
{
	display display =
	{
		.register_select = register_select,
		.read_write = read_write,
		.enable = enable,
		.data = data,
		.character_count = character_count
	};

	port_set_output(register_select);
	port_set_output(read_write);
	port_set_output(enable);

	// clear display
	display_write_data(display, 0, 0b00000001);

	// hide cursor
	display_write_data(display, 0, 0b00001100);

	// set display size
	display_write_data(display, 0, 0b00111000);

	return display;
}
void display_dispose(display display) { }

#endif
