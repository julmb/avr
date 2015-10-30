include $(avr)/build/Makefile.devices

CC = avr-gcc
OC = avr-objcopy
PR = avrdude
CPPFLAGS += -I$(avr)/modules -DF_CPU=$(frequency)
CFLAGS += -mmcu=$(mmcu) -std=gnu99 -O3 -flto -save-temps=obj
LDFLAGS += $(if $(offset),-Ttext=$(offset))
OCFLAGS += -v
PRFLAGS += -c $(programmer) -p $(partno)

binary = obj/$(main).elf

build: $(binary)

erase:
	$(PR) $(PRFLAGS) -e
flash: $(binary)
	$(PR) $(PRFLAGS) $(if $(binary),-U flash:w:$(binary))
configure:
	$(PR) $(PRFLAGS) $(if $(eeprom),-U eeprom:w:$(eeprom))
lock:
	$(PR) $(PRFLAGS) $(if $(lock),-U lock:w:$(lock))
fuse:
	$(PR) $(PRFLAGS) $(if $(lfuse),-U lfuse:w:$(lfuse)) $(if $(hfuse),-U hfuse:w:$(hfuse)) $(if $(efuse),-U efuse:w:$(efuse))

.PHONY: build erase flash configure lock fuse

-include obj/$(main).d

obj:
	mkdir obj
obj/%.elf: %.c | obj
	$(CC)     $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $*.c -o  obj/$*.elf
	$(CC) -MM $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $*.c -MT obj/$*.elf > obj/$*.d
clean:
	rm -r obj

.PHONY: clean
