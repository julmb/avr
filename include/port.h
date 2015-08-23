#ifndef port_h
#define port_h

#include <avr/io.h>

#define PORT_B &DDRB, &PINB, &PORTB
#define PORT_C &DDRC, &PINC, &PORTC
#define PORT_D &DDRD, &PIND, &PORTD

typedef struct
{
	volatile uint8_t* control;
	volatile uint8_t* input;
	volatile uint8_t* output;
	uint8_t pin;
}
port;

void port_set_input(port port)
{
	*port.control &= ~_BV(port.pin);
}
void port_set_output(port port)
{
	*port.control |= _BV(port.pin);
}

void port_enable_pull_up_resistor(port port)
{
	*port.output |= _BV(port.pin);
}
void port_disable_pull_up_resistor(port port)
{
	*port.output &= ~_BV(port.pin);
}

uint8_t port_read(port port)
{
	return *port.input & _BV(port.pin);
}
void port_set(port port)
{
	*port.output |= _BV(port.pin);
}
void port_clear(port port)
{
	*port.output &= ~_BV(port.pin);
}
void port_toggle(port port)
{
	*port.input |= _BV(port.pin);
}
void port_write(port port, uint8_t value)
{
	if (value) port_set(port);
	else port_clear(port);
}

uint8_t port_low(port port)
{
	return *port.output & ~_BV(port.pin);
}
uint8_t port_high(port port)
{
	return *port.output | _BV(port.pin);
}

port port_initialize(volatile uint8_t* control, volatile uint8_t* input, volatile uint8_t* output, uint8_t pin)
{
	port port =
	{
		.control = control,
		.input = input,
		.output = output,
		.pin = pin
	};

	return port;
}
port port_initialize_input(volatile uint8_t* control, volatile uint8_t* input, volatile uint8_t* output, uint8_t pin, uint8_t pull_up_resistor)
{
	port port = port_initialize(control, input, output, pin);

	port_set_input(port);

	if (pull_up_resistor) port_enable_pull_up_resistor(port);
	else port_disable_pull_up_resistor(port);

	return port;
}
port port_initialize_output(volatile uint8_t* control, volatile uint8_t* input, volatile uint8_t* output, uint8_t pin)
{
	port port = port_initialize(control, input, output, pin);

	port_set_output(port);

	return port;
}
void port_dispose(port port)
{
	port_set_input(port);
	port_disable_pull_up_resistor(port);
}

#endif
