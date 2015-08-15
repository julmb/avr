#ifndef data_data_port_h
#define data_data_port_h

#include <avr/io.h>

typedef struct
{
	volatile uint8_t* control;
	volatile uint8_t* input;
	volatile uint8_t* output;
}
data_port;

data_port data_port_initialize(volatile uint8_t* control, volatile uint8_t* input, volatile uint8_t* output)
{
	data_port data_port =
	{
		.control = control,
		.input = input,
		.output = output,
	};

	return data_port;
}
void data_port_dispose(data_port data_port) { }

void data_port_set_input(data_port data_port)
{
	*data_port.control = 0x00;
}
void data_port_set_output(data_port data_port)
{
	*data_port.control = 0xFF;
}

void data_port_enable_pull_up_resistor(data_port data_port)
{
	*data_port.output = 0xFF;
}
void data_port_disable_pull_up_resistor(data_port data_port)
{
	*data_port.output = 0x00;
}

uint8_t data_port_read(data_port data_port)
{
	return *data_port.input;
}
void data_port_write(data_port data_port, uint8_t value)
{
	*data_port.output = value;
}

#endif
