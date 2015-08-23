#ifndef port_data_h
#define port_data_h

#include <avr/io.h>

typedef struct
{
	volatile uint8_t* control;
	volatile uint8_t* input;
	volatile uint8_t* output;
}
port_data;

void port_data_set_input(port_data port_data)
{
	*port_data.control = 0x00;
}
void port_data_set_output(port_data port_data)
{
	*port_data.control = 0xFF;
}

void port_data_enable_pull_up_resistor(port_data port_data)
{
	*port_data.output = 0xFF;
}
void port_data_disable_pull_up_resistor(port_data port_data)
{
	*port_data.output = 0x00;
}

uint8_t port_data_read(port_data port_data)
{
	return *port_data.input;
}
void port_data_write(port_data port_data, uint8_t value)
{
	*port_data.output = value;
}

port_data port_data_initialize(volatile uint8_t* control, volatile uint8_t* input, volatile uint8_t* output)
{
	port_data port_data =
	{
		.control = control,
		.input = input,
		.output = output,
	};

	return port_data;
}
void port_data_dispose(port_data port_data)
{
    port_data_set_input(port_data);
    port_data_disable_pull_up_resistor(port_data);
}

#endif
