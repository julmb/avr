#include <stddef.h>
#include <stdint.h>

#include <usart.h>

#include <display.h>
void main()
{
	display display = display_initialize(pin_initialize(port_initialize(port_c), 5), pin_initialize(port_initialize(port_c), 4), pin_initialize(port_initialize(port_c), 3), port_initialize(port_d));

	display_printf(display, "lulz: %02X", 0x1F);

	// enables USART with a divider of 64, giving about 1 kbaud/s/MHz
	usart_initialize(1, 1, 0x003F, 0);

	while (1)
	{
		usart_wait();
	}

	usart_dispose();
}
