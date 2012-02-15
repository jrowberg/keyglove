PROJECT=kgm_feedback
SOURCES=kgm_feedback.c usiTwiSlave.c
CC=avr-gcc
OBJCOPY=avr-objcopy
PROGRAMMER=usbtiny

MMCU=attiny2313
F_CPU=10000000

CFLAGS=-mmcu=$(MMCU) -DF_CPU=$(F_CPU)UL -Wall

$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -O ihex $(PROJECT).out $(PROJECT).hex

$(PROJECT).out: $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROJECT).out $(SOURCES)

program: $(PROJECT).hex
	avrdude -p $(MMCU) -c $(PROGRAMMER) -e -U flash:w:$(PROJECT).hex

clean:
	rm -f $(PROJECT).out
	rm -f $(PROJECT).hex