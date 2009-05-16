TARGET = clock
SRCS = clock i2c input rtc shift

MMCPU = atmega168
F_CPU = 16000000


all: hex

hex: elf
	avr-objcopy -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex
    
elf: $(SRCS:=.o)
	avr-gcc -Wall -mmcu=$(MMCPU) -DF_CPU=$(F_CPU) -Os -std=gnu99 -o $(TARGET).elf $(SRCS:=.o)

.c.o:
	avr-gcc -Wall -mmcu=$(MMCPU) -DF_CPU=$(F_CPU) -Os -std=gnu99 -c $<
    
upload: hex
	avrdude -p $(MMCPU) -c usbtiny -U flash:w:$(TARGET).hex

clean:
	rm $(SRCS:=.o) $(TARGET).hex $(TARGET).elf
