TARGET = clock

MMCPU = atmega168
F_CPU = 16000000


all: hex

hex: elf
	avr-objcopy -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex
    
elf: $(TARGET).c
	avr-gcc -Wall -mmcu=$(MMCPU) -DF_CPU=$(F_CPU) -Os -std=gnu99 -o $(TARGET).elf $(TARGET).c
    
upload: hex
	avrdude -p $(MMCPU) -c usbtiny -U flash:w:clock.hex
