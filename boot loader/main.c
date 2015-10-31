#include <basic.h>
#include <usart.h>
#include <display.h>

#include <avr/pgmspace.h>
#include <avr/boot.h>

// TODO: remove display
// TODO: remove unused includes
// TODO: enable reading/writing all the pieces of memory that avrdude supports as well
// display_printf(display, "BL: 0x%04X", reset_type);

FUSES =
{
.low = LFUSE_DEFAULT,
.high = HFUSE_DEFAULT,
.extended = EFUSE_DEFAULT
};

typedef enum { read = 0x01, write = 0x02 } command;
typedef enum { flash = 0x01, eeprom = 0x02 } memory;

uint8_t read_byte(void* position)
{
	return pgm_read_byte(position);
}
void write_page(void* position, void* data)
{
	uint8_t* bytes = data;

	boot_page_erase(position);
	boot_spm_busy_wait();

	for (uint8_t byte_index = 0; byte_index < SPM_PAGESIZE; byte_index += 2)
		boot_page_fill(position + byte_index, (*bytes++ << 0) | (*bytes++ << 8));

	boot_page_write(position);
	boot_spm_busy_wait();

	boot_rww_enable();
}

void loader()
{
	display display = display_initialize(pin_initialize(port_initialize(port_c), 5), pin_initialize(port_initialize(port_c), 4), pin_initialize(port_initialize(port_c), 3), port_initialize(port_d));
	display_printf(display, "loader %u", sizeof(memory));

	// enable USART with a divider of 64 * 16, giving about 1 kBd/MHz
	usart_initialize(1, 1, 0x003F, 0);

	while (1)
	{
		command command;
		if (usart_read(&command, sizeof(command))) continue;

		memory memory;
		if (usart_read(&memory, sizeof(memory))) continue;

		void* position;
		if (usart_read(&position, sizeof(position))) continue;

		switch (command)
		{
			case read:
				switch (memory)
				{
					case flash:
					{
						uint8_t byte = read_byte(position);
						usart_write(&byte, sizeof(byte));
						break;
					}
				}
				break;
			case write:
				switch (memory)
				{
					case flash: break;
				}
				break;
		}
	}

	usart_dispose();
}

void main()
{
	reset_type reset_type = initialize();

	if (reset_type == external) loader();

	display display = display_initialize(pin_initialize(port_initialize(port_c), 5), pin_initialize(port_initialize(port_c), 4), pin_initialize(port_initialize(port_c), 3), port_initialize(port_d));
	display_printf(display, "APP");
	application();
}
