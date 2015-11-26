include $(avr)/build/Makefile.devices

CC = avr-gcc
OC = avr-objcopy
PR = avrdude
CPPFLAGS += -I$(avr)/modules -DF_CPU=$(frequency)
CFLAGS += -mmcu=$(mmcu) -std=gnu99 -O3 -flto -save-temps=obj
# TODO: why does --gc-sections need the -Wl option
# TODO: maybe we want to invoke the compiler and linker separately for more control over the whole process?
# TODO: maybe we want to use the traditional .h/.c split?
# TODO: figure out if I need -fdata-sections -ffunction-sections
LDFLAGS += $(if $(offset),-Ttext=$(offset)) -Wl,--gc-sections
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
