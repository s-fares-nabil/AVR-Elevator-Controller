MCU = atmega32
F_CPU = 1000000UL
TARGET = main

all: $(TARGET).hex

$(TARGET).elf: $(TARGET).c MDIO_program.c HLED_program.c HSWITCH_program.c HSSD_program.c HDC_Motor_program.c
	avr-gcc -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -o $(TARGET).elf $(TARGET).c MDIO_program.c HLED_program.c HSWITCH_program.c HSSD_program.c HDC_Motor_program.c

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex

clean:
	rm -f *.elf *.hex