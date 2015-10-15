#ifndef gpio_h
#define gpio_h

#include <avr/io.h>

#ifdef __AVR_ATtiny85__
typedef enum { portB = 1 } portId;
#endif
#ifdef __AVR_ATmega328P__
typedef enum { portB = 1, portC = 2, portD = 3 } portId;
#endif
#ifdef __AVR_ATmega1284P__
typedef enum { portA = 0, portB = 1, portC = 2, portD = 3 } portId;
#endif

typedef struct { volatile uint8_t* direction; volatile uint8_t* input; volatile uint8_t* output; } port;

void port_set_direction(port port, uint8_t value) { *port.direction = value; }
uint8_t port_read(port port) { return *port.input; }
void port_write(port port, uint8_t value) { *port.output = value; }

port port_initialize(portId portId)
{
	switch (portId)
	{
		#ifdef __AVR_ATtiny85__
		case portB: return (port) { &DDRB, &PINB, &PORTB };
		#endif
		#ifdef __AVR_ATmega328P__
		case portB: return (port) { &DDRB, &PINB, &PORTB };
		case portC: return (port) { &DDRC, &PINC, &PORTC };
		case portD: return (port) { &DDRD, &PIND, &PORTD };
		#endif
		#ifdef __AVR_ATmega1284P__
		case portA: return (port) { &DDRA, &PINA, &PORTA };
		case portB: return (port) { &DDRB, &PINB, &PORTB };
		case portC: return (port) { &DDRC, &PINC, &PORTC };
		case portD: return (port) { &DDRD, &PIND, &PORTD };
		#endif
	}
}
port port_initialize_direction(portId portId, uint8_t direction)
{
	port port = port_initialize(portId);

	port_set_direction(port, direction);

	return port;
}
port port_initialize_direction_state(portId portId, uint8_t direction, uint8_t state)
{
	port port = port_initialize_direction(portId, direction);

	port_write(port, state);

	return port;
}
void port_dispose(port port)
{
	port_set_direction(port, 0x00);
	port_write(port, 0x00);
}

typedef struct { port port; uint8_t index; } pin;

uint8_t pin_input(pin pin) { return *pin.port.direction & ~_BV(pin.index); }
uint8_t pin_output(pin pin) { return *pin.port.direction | _BV(pin.index); }
void pin_set_input(pin pin) { *pin.port.direction = pin_input(pin); }
void pin_set_output(pin pin) { *pin.port.direction = pin_output(pin); }
void pin_set_direction(pin pin, uint8_t value) { *pin.port.direction = value ? pin_output(pin) : pin_input(pin); }

uint8_t pin_low(pin pin) { return *pin.port.output & ~_BV(pin.index); }
uint8_t pin_high(pin pin) { return *pin.port.output | _BV(pin.index); }
void pin_clear(pin pin) { *pin.port.output = pin_low(pin); }
void pin_set(pin pin) { *pin.port.output = pin_high(pin); }
void pin_toggle(pin pin) { *pin.port.input |= _BV(pin.index); }
uint8_t pin_read(pin pin) { return *pin.port.input & _BV(pin.index); }
void pin_write(pin pin, uint8_t value) { *pin.port.output = value ? pin_high(pin) : pin_low(pin); }

pin pin_initialize(portId portId, uint8_t index)
{
	return (pin) { port_initialize(portId), index };
}
pin pin_initialize_direction(portId portId, uint8_t index, uint8_t direction)
{
	pin pin = pin_initialize(portId, index);

	pin_set_direction(pin, direction);

	return pin;
}
pin pin_initialize_direction_state(portId portId, uint8_t index, uint8_t direction, uint8_t state)
{
	pin pin = pin_initialize_direction(portId, index, direction);

	pin_write(pin, state);

	return pin;
}
void pin_dispose(pin pin)
{
	pin_set_input(pin);
	pin_clear(pin);
}

#endif
