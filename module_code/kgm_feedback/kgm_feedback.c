#include <avr/io.h>
#include <avr/interrupt.h>

//#define ENABLE_I2C_SLAVE

#ifdef ENABLE_I2C_SLAVE
    #include "usiTwiSlave.h"
#endif

uint8_t registers[] = {
    /* 0x00 RED_MODE   [2:0 - mode]           */ 0x00, // off
    /* 0x01 RED_BRIGHT [7:4 - max][3:0 - min] */ 0xA0, // 100% max, 0% min
    /* 0x02 RED_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x03 RED_SQUARE [7:0 - high time %]    */ 0x32, // 50% split high/low

    /* 0x04 GRN_MODE   [2:0 - mode]           */ 0x00, // off
    /* 0x05 GRN_BRIGHT [7:4 - max][3:0 - min] */ 0xA0, // 100% max, 0% min
    /* 0x06 GRN_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x07 GRN_SQUARE [6:0 - high time %]    */ 0x32, // 50% split high/low

    /* 0x08 BLU_MODE   [2:0 - mode]           */ 0x00, // off
    /* 0x09 BLU_BRIGHT [7:4 - max][3:0 - min] */ 0xA0, // 100% max, 0% min
    /* 0x0A BLU_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x0B BLU_SQUARE [6:0 - high time %]    */ 0x32, // 50% split high/low

    /* 0x0C VIB_MODE   [2:0 - mode]           */ 0x00, // off
    /* 0x0D VIB_SPEED  [7:4 - max][3:0 - min] */ 0xA0, // 100% max, 0% min
    /* 0x0E VIB_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x0F VIB_SQUARE [6:0 - high time %]    */ 0x32, // 50% split high/low

    /* 0x10 SPK_MODE   [2:0 - mode]           */ 0x00, // off
    /* 0x11 SPK_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x12 SPK_SQUARE [6:0 - high time %]    */ 0x32, // 50% split high/low
    /* 0x13 SPK_FRQ1_H [7:0 - freq1 Hz MSB]   */ 0x06, // 0x06E0 = 1760Hz
    /* 0x14 SPK_FRQ1_L [7:0 - freq1 Hz LSB]   */ 0xE0, // ^^^
    /* 0x15 SPK_FRQ2_H [7:0 - freq2 Hz MSB]   */ 0x06, // 0x06E0 = 1760Hz
    /* 0x16 SPK_FRQ2_L [7:0 - freq2 Hz LSB]   */ 0xE0  // ^^^
};

#define I2C_SLAVE_ADDRESS 0x57

/*
RGB mode: 0 = off, 1 = solid, 2 = blink, 3 = fade, 4 = fade in, 5 = fade out, 6 = fade in once, 7 = fade out once
VIB mode: 0 = off, 1 = solid, 2 = periodic
SPK mode: 0 = off, 1 = solid, 2 = periodic, 3 = alternate, 4 = slide up, 5 = slide down, 6 = slide bounce
*/

//#define ATTINYX4
#define ATTINY2313

#ifdef ATTINYX4
    // ATTinyX4 pins
    #define VIBP PORTB // vib = PA1
    #define VIBD DDRA
    #define VIBN 1

    #define SPKP PORTB // spk = PA2
    #define SPKD DDRA
    #define SPKN 2

    #define REDP PORTB // red = PB2/OC0A
    #define REDD DDRB
    #define REDN 2

    #define GRNP PORTB // green = PA7/OC0B
    #define GRND DDRA
    #define GRNN 7

    #define BLUP PORTB // blue = PA5/OC1B
    #define BLUD DDRA
    #define BLUN 5
#endif

#ifdef ATTINY2313
    // ATTiny2313 pins
    #define VIBP PORTB // vib = PB0
    #define VIBD DDRB
    #define VIBN 0

    #define SPKP PORTB // spk = PB1
    #define SPKD DDRB
    #define SPKN 1

    #define REDP PORTB // red = PB2/OC0A
    #define REDD DDRB
    #define REDN 2

    #define GRNP PORTB // green = PD5/OC0B
    #define GRND DDRD
    #define GRNN 5

    #define BLUP PORTB // blue = PB4/OC1B
    #define BLUD DDRB
    #define BLUN 4
#endif

// thanks to LadyAda for these helpers
// http://www.ladyada.net/learn/proj1/blinky.html
#define output_low(port, pin) port &= ~(1 << pin)
#define output_high(port, pin) port |= (1 << pin)
#define output_toggle(port, pin) port ^= (1 << pin)
#define set_input(portdir, pin) portdir &= ~(1 << pin)
#define set_output(portdir, pin) portdir |= (1 << pin)

// very basic delay function
/*void delay_ms(uint8_t ms) {
    uint16_t delay_count = F_CPU / 17500;
    volatile uint16_t i;
    while (ms != 0) {
        for (i =0; i != delay_count; i++);
        ms--;
    }
}*/

uint16_t counter1 = 0;
uint8_t counter2 = 0;

// main entry point
int main(void) {
    uint8_t waiting, address, in, count;
    uint8_t redLevel = 0, grnLevel = 0, bluLevel = 0;

    // set feedback pins to output
    set_output(VIBD, VIBN);
    set_output(SPKD, SPKN);
    set_output(REDD, REDN);
    set_output(GRND, GRNN);
    set_output(BLUD, BLUN);

    // turn off all feedback elements
    output_high(VIBP, VIBN);
    output_low(SPKP, SPKN);
    output_low(REDP, REDN);
    output_low(GRNP, GRNN);
    output_low(BLUP, BLUN);

    // setup timers/interrupts
    TCCR1B |= (1 << WGM12);
    TIMSK |= (1 << OCIE1A);
    sei();
    OCR1A = 100; // Timer1 ticks every 100us
    TCCR1B |= (1 << CS10);

    #ifdef ENABLE_I2C_SLAVE
        // start the USI/I2C process
        usiTwiSlaveInit(I2C_SLAVE_ADDRESS);
    #endif

    registers[0] = 0x04;
    registers[4] = 0x02;

    // main loop
    while (1) {
        if (counter1 >= 10000) { // runs every 1s
            counter1 = 0;
        }
        if (counter2 >= 100) { // runs every 10ms
            if (registers[0x04] == 0) {
                redLevel = 0;
            } else if (registers[0] == 1) {
                redLevel = 1;
            } else if (registers[0] == 2) {
                if (counter1 > (1000 * 5)) redLevel = 0;
                else redLevel = 100;
            } else if (registers[0] == 3) {
                redLevel = 100 - abs(counter1 - 5000)/50;
            } else if (registers[0] == 4 || registers[0] == 6) {
                redLevel = counter1/100;
            } else if (registers[0] == 5 || registers[0] == 7) {
                redLevel = 100 - counter1/100;
            }

            if (registers[0x04] == 0) {
                grnLevel = 0;
            } else if (registers[0] == 1) {
                grnLevel = 1;
            } else if (registers[0] == 2) {
                if (counter1 > (1000 * 5)) grnLevel = 0;
                else grnLevel = 100;
            } else if (registers[0] == 3) {
                grnLevel = 100 - abs(counter1 - 5000)/50;
            } else if (registers[0] == 4 || registers[0] == 6) {
                grnLevel = counter1/100;
            } else if (registers[0] == 5 || registers[0] == 7) {
                grnLevel = 100 - counter1/100;
            }

            if (registers[0x08] == 0) {
                bluLevel = 0;
            } else if (registers[0] == 1) {
                bluLevel = 1;
            } else if (registers[0] == 2) {
                if (counter1 > (1000 * 5)) bluLevel = 0;
                else bluLevel = 100;
            } else if (registers[0] == 3) {
                bluLevel = 100 - abs(counter1 - 5000)/50;
            } else if (registers[0] == 4 || registers[0] == 6) {
                bluLevel = counter1/100;
            } else if (registers[0] == 5 || registers[0] == 7) {
                bluLevel = 100 - counter1/100;
            }

            counter2 = 0;
        }

        // software PWM for RGB LEDs (runs every ~100us)
        if (counter2 >= redLevel) output_low(REDP, REDN);
        else output_high(REDP, REDN);
        if (counter2 >= grnLevel) output_low(GRNP, GRNN);
        else output_high(GRNP, GRNN);
        if (counter2 >= bluLevel) output_low(BLUP, BLUN);
        else output_high(BLUP, BLUN);

        #ifdef ENABLE_I2C_SLAVE
            // check for I2C commands from master
            if ((waiting = usiTwiDataInReceiveBuffer())) {
                // bytes available
                for (count = 0; count < waiting; count++) {
                    in = usiTwiReceiveByte();
                    if (count == 0) address = in;
                    else if (address < sizeof(registers)) registers[address + count] = in;
                }
            }
        #endif
    }
}

ISR(TIMER1_COMPA_vect)
{
    counter1++;
    counter2++;
}

//void usiTwiSlaveInit(uint8_t slaveAddr); // initialize I2C lib
//void usiTwiTransmitByte(uint8_t data); // send data back to master
//uint8_t usiTwiDataInReceiveBuffer(); // the bytes available that haven't been read yet
//uint8_t usiTwiReceiveByte(); // returns the bytes received one at a time