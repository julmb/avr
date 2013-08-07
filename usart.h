#ifndef usart_h
#define usart_h

#include <avr/io.h>

#include "timer.h"
#include "port.h"

typedef struct
{
	port status_port;
}
usart;

usart usart_initialize(port status_port)
{
	usart usart =
	{
		.status_port = status_port
	};

	port_clear(usart.status_port);

	timer_initialize();

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

	return usart;
}
void usart_dispose(usart usart)
{
	port_clear(usart.status_port);

	timer_dispose();
}

void usart_rx_flush_buffer(usart usart)
{
	uint8_t data;

	// read from usart data register until the rx complete flag is no longer set
	while (UCSR0A & _BV(RXC0)) data = UDR0;
}

uint8_t usart_rx_has_byte(usart usart)
{
	// check for rx complete flag
	return UCSR0A & _BV(RXC0);
}
uint8_t usart_rx_get_byte(usart usart)
{
	// read usart data register
	return UDR0;
}

// read a byte, waiting indefinitely for data, toggling the status port while waiting for data
void usart_rx_read_byte(usart usart, uint8_t* data)
{
	timer_restart();
	timer_reset();

	uint16_t retryCount = 0;

	while (!usart_rx_has_byte(usart))
		if (timer_has_elapsed())
		{
			retryCount++;

			if (retryCount == 1000)
			{
				retryCount = 0;

				port_toggle(usart.status_port);
			}
		}

	*data = usart_rx_get_byte(usart);

	port_set(usart.status_port);
}
// read a byte, giving up after 1 ms of waiting for data
uint8_t usart_rx_try_read_byte(usart usart, uint8_t* data)
{
	timer_restart();
	timer_reset();

	while (!usart_rx_has_byte(usart))
		if (timer_has_elapsed())
			return 1;

	*data = usart_rx_get_byte(usart);

	return 0;
}

// read many bytes, waiting indefinitely for data
void usart_rx_read_bytes(usart usart, uint8_t* data, uint16_t data_length)
{
	port_set(usart.status_port);

	for (uint16_t index = 0; index < data_length; index++) usart_rx_read_byte(usart, &data[index]);

	port_clear(usart.status_port);
}
// read many bytes, giving up after 1 ms of waiting for data
uint8_t usart_rx_try_read_bytes(usart usart, uint8_t* data, uint16_t data_length)
{
	port_set(usart.status_port);

	for (uint16_t index = 0; index < data_length; index++)
		if (usart_rx_try_read_byte(usart, &data[index]))
		{
			port_clear(usart.status_port);

			return 1;
		}

	port_clear(usart.status_port);

	return 0;
}

// read many bytes, waiting indefinitely for the first byte, then giving up after 1 ms of waiting for data
uint8_t usart_rx_read_stream(usart usart, uint8_t* data, uint16_t data_length)
{
	if (data_length == 0) return 0;

	port_set(usart.status_port);

	usart_rx_read_byte(usart, &data[0]);

	for (uint16_t index = 1; index < data_length; index++)
		if (usart_rx_try_read_byte(usart, &data[index]))
		{
			port_clear(usart.status_port);

			return 1;
		}

	port_clear(usart.status_port);

	return 0;
}

#endif
