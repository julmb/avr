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

port port_initialize(volatile uint8_t* control, volatile uint8_t* input, volatile uint8_t* output, uint8_t pin);

void port_set_input(port port);
void port_set_output(port port);

void port_enable_pull_up_resistor(port port);
void port_disable_pull_up_resistor(port port);

uint8_t port_read(port port);
void port_set(port port);
void port_clear(port port);
void port_toggle(port port);
void port_write(port port, uint8_t value);

#endif
