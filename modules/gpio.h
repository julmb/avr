#ifndef gpio_h
#define gpio_h

#include <stddef.h>
#include <stdint.h>

#include <avr/io.h>

#define port_a &DDRA, &PINA, &PORTA
#define port_b &DDRB, &PINB, &PORTB
#define port_c &DDRC, &PINC, &PORTC
#define port_d &DDRD, &PIND, &PORTD

typedef struct { volatile uint8_t* control; volatile uint8_t* input; volatile uint8_t* output; } port;

inline port port_initialize(volatile uint8_t* control, volatile uint8_t* input, volatile uint8_t* output)
{
	return (port) { control, input, output };
}
inline void port_dispose(port port)
{
	*port.control = 0x00;
	*port.output = 0x00;
}

inline void port_set_direction(port port, uint8_t value) { *port.control = value; }
inline void port_set_input(port port) { port_set_direction(port, 0x00); }
inline void port_set_output(port port) { port_set_direction(port, 0xFF); }

inline uint8_t port_read(port port) { return *port.input; }
inline void port_write(port port, uint8_t value) { *port.output = value; }

typedef struct { port port; uint8_t index; } pin;

inline pin pin_initialize(port port, uint8_t index)
{
	return (pin) { port, index };
}
inline void pin_dispose(pin pin)
{
	*pin.port.control &= ~_BV(pin.index);
	*pin.port.output &= ~_BV(pin.index);
}

inline void pin_set_input(pin pin) { *pin.port.control &= ~_BV(pin.index); }
inline void pin_set_output(pin pin) { *pin.port.control |= _BV(pin.index); }

inline uint8_t pin_low(pin pin) { return *pin.port.output & ~_BV(pin.index); }
inline uint8_t pin_high(pin pin) { return *pin.port.output | _BV(pin.index); }
inline void pin_clear(pin pin) { *pin.port.output = pin_low(pin); }
inline void pin_set(pin pin) { *pin.port.output = pin_high(pin); }
inline void pin_toggle(pin pin) { *pin.port.input = _BV(pin.index); }
inline uint8_t pin_read(pin pin) { return *pin.port.input & _BV(pin.index); }
inline void pin_write(pin pin, uint8_t value) { *pin.port.output = value ? pin_high(pin) : pin_low(pin); }

inline pin pin_input(port port, uint8_t index, uint8_t pull_up)
{
	pin pin = pin_initialize(port, index);

	pin_set_input(pin);
	pin_write(pin, pull_up);

	return pin;
}
inline pin pin_output(port port, uint8_t index, uint8_t state)
{
	pin pin = pin_initialize(port, index);

	pin_set_output(pin);
	pin_write(pin, state);

	return pin;
}

#endif
