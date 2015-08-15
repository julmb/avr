device = atmega328p
frequency = 20000000UL

all: build flash

build:
	avr-gcc -std=gnu99 -O3 -flto -save-temps -mmcu=atmega328p main.c port.c -o main.elf
	avr-objcopy -O ihex main.elf main.hex

flash:
	avrdude -c dragon_isp -p m328p -U flash:w:main.hex

clean:
	rm main.elf main.res main.hex main.i main.o main.s main.elf.ltrans0.o main.elf.ltrans0.s main.elf.ltrans.out

.PHONY: clean
