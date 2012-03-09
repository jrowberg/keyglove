PROJECT=kgm_feedback
SOURCES=kgm_feedback.c usiTwiSlave.c
CC=avr-gcc
OBJCOPY=avr-objcopy
PROGRAMMER=usbtiny

MMCU=attiny44
F_CPU=80000000

CFLAGS=-mmcu=$(MMCU) -DF_CPU=$(F_CPU)UL -Wall

$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -j .data -O ihex $(PROJECT).out $(PROJECT).hex

$(PROJECT).out: $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROJECT).out $(SOURCES)

program: $(PROJECT).hex
	avrdude -p $(MMCU) -c $(PROGRAMMER) -e -U flash:w:$(PROJECT).hex

i2ctest:
	$(CC) $(CFLAGS) -o i2cslave_test.out i2cslave_test.c usiTwiSlave.c
	$(OBJCOPY) -j .text -j .data -O ihex i2cslave_test.out i2cslave_test.hex
	avrdude -p $(MMCU) -c $(PROGRAMMER) -e -U flash:w:i2cslave_test.hex

clean:
	rm -f $(PROJECT).out
	rm -f $(PROJECT).hex

fuses:
# run at 1 MHz
#	avrdude -p $(MMCU) -c $(PROGRAMMER) -U lfuse:w:0x42:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

# run at 8 MHz
	avrdude -p $(MMCU) -c $(PROGRAMMER) -U lfuse:w:0xc2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
