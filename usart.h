#ifndef usart_h
#define usart_h

#include <avr/io.h>

#include "timer.h"

void usart_initialize()
{
	// set UBBR0 to 0 for 2 Mbaud at 16 MHz CPU clock frequency in double speed mode
	UBRR0 = 0x0000;

	// enable double speed mode (U2X0 = 0b1)
	// disable multi-processor communication mode (MPCM0 = 0b0)
	// disable all interrupts (RXCIE0 = 0b0, TXCIE0 = 0b0, UDRIE0 = 0b0)
	// enable receiver and transmitter (RXEN0 = 0b1, TXEN0 = 0b1)
	// set mode to asynchronous USART (UMSEL0 = 0b00)
	// set parity mode to disabled (UPM0 = 0b00)
	// set stop bit mode to 1 stop bit (USBS0 = 0b0)
	// set character size to 8 bit (UCSZ0 = 0b011)
	// set clock polarity bit to zero, not used in asynchronous mode (UCPOL0 = 0b0)
	UCSR0A = (1 << U2X0) | (0 << MPCM0);
	UCSR0B = (0 << RXCIE0) | (0 << TXCIE0) | (0 << UDRIE0) | (1 << RXEN0) | (1 << TXEN0) | (0 << UCSZ02);
	UCSR0C = (0 << UMSEL01) | (0 << UMSEL00) | (0 << UPM01) | (0 << UPM00) | (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00) | (0 << UCPOL0);

	timer_initialize();
}
void usart_dispose()
{
	timer_dispose();
}

void usart_rx_flush_buffer()
{
	uint8_t data;

	// read from usart data register until the rx complete flag is no longer set
	while (UCSR0A & _BV(RXC0)) data = UDR0;
}

uint8_t usart_rx_has_byte()
{
	// check for rx complete flag
	return UCSR0A & _BV(RXC0);
}
uint8_t usart_rx_get_byte()
{
	// read usart data register
	return UDR0;
}
uint8_t usart_rx_read_byte()
{
	while (!usart_rx_has_byte());

	return usart_rx_get_byte();
}

uint8_t usart_rx_try_read_byte(uint8_t* data)
{
	timer_restart();
	timer_reset();

	while (!usart_rx_has_byte())
		if (timer_has_elapsed())
			return 1;

	*data = usart_rx_get_byte();

	return 0;
}

uint8_t usart_rx_try_read_bytes(uint8_t* data, uint16_t data_length)
{
	for (uint16_t index = 0; index < data_length; index++)
		if (usart_rx_try_read_byte(&data[index]))
			return 1;

	return 0;
}

#endif
