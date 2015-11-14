#ifndef usart_h
#define usart_h

#include <stddef.h>
#include <stdint.h>

#include <avr/io.h>

#include "timer.h"

#ifndef __AVR_ATmega328P__
	#error "The USART module is not available on the chosen MCU."
#endif

// full receive buffer sets this flag
// reset or reading UDR0 clear this flag
uint8_t usart_received_byte() { return UCSR0A & _BV(RXC0); }
// reset clears this flag
// empty transmit buffer and empty transmit shift register set this flag
uint8_t usart_sent_byte() { return UCSR0A & _BV(TXC0); }
// full transmit buffer clears this flag
// reset or empty transmit buffer set this flag
uint8_t usart_needs_byte() { return UCSR0A & _BV(UDRE0); }

void usart_wait_receive()
{
	while (!usart_received_byte());
}
void usart_wait_send()
{
	while (!usart_sent_byte());
}

uint8_t usart_try(uint16_t tick_count)
{
	if (usart_received_byte()) return 0;

	timer_restart();

	for (uint16_t index = 0; index < tick_count; index++)
	{
		timer_reset();

		while (!timer_has_elapsed())
			if (usart_received_byte())
				return 0;
	}

	return 1;
}

uint8_t usart_read(void* data, size_t length)
{
	uint8_t* bytes = data;

	for (size_t index = 0; index < length; index++)
	{
		if (usart_try(1)) return 1;

		*bytes++ = UDR0;
	}

	return 0;
}
void usart_write(void* data, size_t length)
{
	uint8_t* bytes = data;

	for (size_t index = 0; index < length; index++)
	{
		while (!usart_needs_byte());

		// clear the TXC0 flag since this does not happen automatically
		UCSR0A |= _BV(TXC0);
		UDR0 = *bytes++;
	}
}

void usart_initialize(uint8_t receiver, uint8_t transmitter, uint16_t divider, uint8_t double_speed)
{
	// set double speed mode (U2X0 = double_speed)
	// disable multi-processor communication mode (MPCM0 = 0b0)
	// disable all interrupts (RXCIE0 = 0b0, TXCIE0 = 0b0, UDRIE0 = 0b0)
	// set receiver and transmitter (RXEN0 = receiver, TXEN0 = transmitter)
	// set mode to asynchronous USART (UMSEL0 = 0b00)
	// set parity mode to disabled (UPM0 = 0b00)
	// set stop bit mode to 1 stop bit (USBS0 = 0b0)
	// set character size to 8 bit (UCSZ0 = 0b011)
	// set clock polarity bit to zero, not used in asynchronous mode (UCPOL0 = 0b0)
	UCSR0A = ((double_speed ? 1 : 0) << U2X0) | (0 << MPCM0);
	UCSR0B = (0 << RXCIE0) | (0 << TXCIE0) | (0 << UDRIE0) | ((receiver ? 1 : 0) << RXEN0) | ((transmitter ? 1 : 0) << TXEN0) | (0 << UCSZ02);
	UCSR0C = (0 << UMSEL01) | (0 << UMSEL00) | (0 << UPM01) | (0 << UPM00) | (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00) | (0 << UCPOL0);

	// set baud rate
	UBRR0 = divider;

	timer_initialize();
}
void usart_dispose()
{
	// reset configuration and status
	UCSR0A = 0x00;
	UCSR0B = 0x00;
	UCSR0C = 0x00;

	// reset baud rate
	UBRR0 = 0x0000;

	// reset data
	UDR0 = 0x00;

	timer_dispose();
}

#endif
