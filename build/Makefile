include $(avr)/build/Makefile.devices

CC = avr-gcc
OC = avr-objcopy
PR = avrdude
CPPFLAGS += -I$(avr)/modules -DF_CPU=$(frequency)
CFLAGS += -mmcu=$(mmcu) -std=gnu99 -Wall -O3 -flto -save-temps=obj
LDFLAGS += -Wl,--gc-sections $(if $(offset),-Ttext=$(offset))
PRFLAGS += -c $(programmer) -p $(partno)

binary = obj/$(main).elf
image = obj/$(main).bin

build: $(binary)
image: $(image)

erase:
	$(PR) $(PRFLAGS) -e
flash: $(binary)
	$(PR) $(PRFLAGS) $(if $(binary),-U flash:w:$(binary))
configure:
	$(PR) $(PRFLAGS) $(if $(eeprom),-U eeprom:w:$(eeprom))
fuse:
	$(PR) $(PRFLAGS) $(if $(lfuse),-U lfuse:w:$(lfuse)) $(if $(hfuse),-U hfuse:w:$(hfuse)) $(if $(efuse),-U efuse:w:$(efuse))
lock:
	$(PR) $(PRFLAGS) $(if $(lock),-U lock:w:$(lock))

.PHONY: build image erase flash configure fuse lock

-include obj/$(main).d

obj:
	mkdir obj
obj/%.elf: %.c | obj
	$(CC)     $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $*.c -o  obj/$*.elf
	$(CC) -MM $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $*.c -MT obj/$*.elf > obj/$*.d
obj/%.bin: obj/%.elf | obj
	$(OC) -O binary obj/$*.elf obj/$*.bin
clean:
	rm -r obj

.PHONY: clean
