// Keyglove controller source code - RGB/Vibe/Piezo feedback module ATTiny44 driver code
// 2/15/2012 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h> // for abs()

#define ENABLE_I2C_SLAVE

#ifdef ENABLE_I2C_SLAVE
    #include "usiTwiSlave.h"
    #define I2C_SLAVE_ADDRESS 0x57
#endif

// leave room for implmentations on alternate AVRs
#define ATTINY44

#ifdef ATTINY44
    // ATTinyX4 pins
    #define VIBP PORTA // vib = PA1
    #define VIBD DDRA
    #define VIBN 1

    #define SPKP PORTA // spk = PA2
    #define SPKD DDRA
    #define SPKN 2

    #define REDP PORTB // red = PB2/OC0A
    #define REDD DDRB
    #define REDN 2

    #define GRNP PORTA // green = PA7/OC0B
    #define GRND DDRA
    #define GRNN 7

    #define BLUP PORTA // blue = PA5/OC1B
    #define BLUD DDRA
    #define BLUN 5
#endif

// thanks to LadyAda for these helpers
// http://www.ladyada.net/learn/proj1/blinky.html
#define output_low(port, pin) port &= ~(1 << pin)
#define output_high(port, pin) port |= (1 << pin)
#define output_toggle(port, pin) port ^= (1 << pin)
#define set_input(portdir, pin) portdir &= ~(1 << pin)
#define set_output(portdir, pin) portdir |= (1 << pin)

// timing counters (100 usec, 10 msec)
uint16_t counter1 = 0;
uint8_t counter2 = 0;

// I2C I/O vars
uint8_t waiting, address, in, count;

// software PWM control levels for RGB
uint8_t redLevel = 0, grnLevel = 0, bluLevel = 0;

uint8_t redMode, redInvert;
uint8_t redBrightMin, redBrightMax;
uint8_t redPeriod;
uint8_t redSquare;

uint8_t grnMode, grnInvert;
uint8_t grnBrightMin, grnBrightMax;
uint8_t grnPeriod;
uint8_t grnSquare;

uint8_t bluMode, bluInvert;
uint8_t bluBrightMin, bluBrightMax;
uint8_t bluPeriod;
uint8_t bluSquare;

uint8_t vibMode, vibInvert;
uint8_t vibSpeedMin, vibSpeedMax;
uint8_t vibPeriod;
uint8_t vibSquare;

uint8_t spkMode, spkInvert;
uint8_t spkPeriod;
uint8_t spkSquare;
uint16_t spkFreq1, spkFreq2;

uint8_t registers[] = {
    /* 0x00 RED_MODE   [3 - inv][2:0 - mode]  */ 0x00, // off, no inversion
    /* 0x01 RED_BRIGHT [7:4 - max][3:0 - min] */ 0xA0, // 100% max, 0% min
    /* 0x02 RED_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x03 RED_SQUARE [7:0 - high time %]    */ 0x32, // 50% split high/low

    /* 0x04 GRN_MODE   [3 - inv][2:0 - mode]  */ 0x00, // off, no inversion
    /* 0x05 GRN_BRIGHT [7:4 - max][3:0 - min] */ 0xA0, // 100% max, 0% min
    /* 0x06 GRN_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x07 GRN_SQUARE [6:0 - high time %]    */ 0x32, // 50% split high/low

    /* 0x08 BLU_MODE   [3 - inv][2:0 - mode]  */ 0x00, // off, no inversion
    /* 0x09 BLU_BRIGHT [7:4 - max][3:0 - min] */ 0xA0, // 100% max, 0% min
    /* 0x0A BLU_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x0B BLU_SQUARE [6:0 - high time %]    */ 0x32, // 50% split high/low

    /* 0x0C VIB_MODE   [3 - inv][2:0 - mode]  */ 0x00, // off, no inversion
    /* 0x0D VIB_SPEED  [7:4 - max][3:0 - min] */ 0xA0, // 100% max, 0% min
    /* 0x0E VIB_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x0F VIB_SQUARE [6:0 - high time %]    */ 0x32, // 50% split high/low

    /* 0x10 SPK_MODE   [3 - inv][2:0 - mode]  */ 0x00, // off, no inversion
    /* 0x11 SPK_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x12 SPK_SQUARE [6:0 - high time %]    */ 0x32, // 50% split high/low
    /* 0x13 SPK_FRQ1_H [7:0 - freq1 Hz MSB]   */ 0x06, // 0x06E0 = 1760Hz
    /* 0x14 SPK_FRQ1_L [7:0 - freq1 Hz LSB]   */ 0xE0, // ^^^
    /* 0x15 SPK_FRQ2_H [7:0 - freq2 Hz MSB]   */ 0x06, // 0x06E0 = 1760Hz
    /* 0x16 SPK_FRQ2_L [7:0 - freq2 Hz LSB]   */ 0xE0  // ^^^
};

/*
RGB mode: 0 = off, 1 = solid, 2 = blink, 3 = fade, 4 = fade in, 5 = fade out, 6 = fade in once, 7 = fade out once
VIB mode: 0 = off, 1 = solid, 2 = periodic
SPK mode: 0 = off, 1 = solid, 2 = periodic, 3 = alternate, 4 = slide up, 5 = slide down, 6 = slide bounce
*/

void applyRegisterConfig() {
    redMode         =  registers[0x00] & 0x07;
    redInvert       =  registers[0x00] & 0x08;
    redBrightMin    =  registers[0x01] & 0x0F;
    redBrightMax    = (registers[0x01] & 0xF0) >> 4;
    redPeriod       =  registers[0x02];
    redSquare       =  registers[0x03] & 0x7F;

    grnMode         =  registers[0x04] & 0x07;
    grnInvert       =  registers[0x04] & 0x08;
    grnBrightMin    =  registers[0x05] & 0x0F;
    grnBrightMax    = (registers[0x05] & 0xF0) >> 4;
    grnPeriod       =  registers[0x06];
    grnSquare       =  registers[0x07] & 0x7F;

    bluMode         =  registers[0x08] & 0x07;
    bluInvert       =  registers[0x08] & 0x08;
    bluBrightMin    =  registers[0x09] & 0x0F;
    bluBrightMax    = (registers[0x09] & 0xF0) >> 4;
    bluPeriod       =  registers[0x0A];
    bluSquare       =  registers[0x0B] & 0x7F;

    vibMode         =  registers[0x0C] & 0x07;
    vibInvert       =  registers[0x0C] & 0x08;
    vibSpeedMin     =  registers[0x0D] & 0x0F;
    vibSpeedMax     = (registers[0x0D] & 0xF0) >> 4;
    vibPeriod       =  registers[0x0E];
    vibSquare       =  registers[0x0F] & 0x7F;

    spkMode         =  registers[0x10] & 0x07;
    spkInvert       =  registers[0x10] & 0x08;
    spkPeriod       =  registers[0x11];
    spkSquare       =  registers[0x12] & 0x7F;
    spkFreq1        = (registers[0x13] << 8 | registers[0x14]);
    spkFreq2        = (registers[0x15] << 8 | registers[0x16]);
}

// main entry point
int main(void) {
    // set feedback pins to output mode
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

    #ifdef ATTINY44
        // setup timers/interrupts (ATTINYx4)
        TCCR1B |= ((1 << WGM12) | (1 << CS10));
        TIMSK1 |= (1 << OCIE1A); // enable
        OCR1A = 200; // Timer1 ticks every 100us
        sei();  // enable interrupts
    #endif

    #ifdef ENABLE_I2C_SLAVE
        // start the USI/I2C process
        usiTwiSlaveInit(I2C_SLAVE_ADDRESS);
    #endif

    //registers[0x00] = 0x04;

    // read initial register values into control variables
    applyRegisterConfig();

    // main loop
    while (1) {
        if (counter1 >= 10000) { // runs every 1s
            counter1 = 0;
        }
        if (counter2 >= 100) { // runs every 10ms
            // adjust RED LED according to timing
            if (redMode == 0) {
                redLevel = 0;
            } else if (redMode == 1) {
                redLevel = 100;
            } else if (redMode == 2) {
                if (counter1 > (1000 * 5)) redLevel = 0;
                else redLevel = 100;
            } else if (redMode == 3) {
                redLevel = 100 - abs(counter1 - 5000)/50;
            } else if (redMode == 4 || redMode == 6) {
                redLevel = counter1/100;
            } else if (redMode == 5 || redMode == 7) {
                redLevel = 100 - counter1/100;
            }

            // adjust GREEN LED according to timing
            if (grnMode == 0) {
                grnLevel = 0;
            } else if (grnMode == 1) {
                grnLevel = 100;
            } else if (grnMode == 2) {
                if (counter1 > (1000 * 5)) grnLevel = 0;
                else grnLevel = 100;
            } else if (grnMode == 3) {
                grnLevel = 100 - abs(counter1 - 5000)/50;
            } else if (grnMode == 4 || grnMode == 6) {
                grnLevel = counter1/100;
            } else if (grnMode == 5 || grnMode == 7) {
                grnLevel = 100 - counter1/100;
            }

            // adjust BLUE LED according to timing
            if (bluMode == 0) {
                bluLevel = 0;
            } else if (bluMode == 1) {
                bluLevel = 100;
            } else if (bluMode == 2) {
                if (counter1 > (1000 * 5)) bluLevel = 0;
                else bluLevel = 100;
            } else if (bluMode == 3) {
                bluLevel = 100 - abs(counter1 - 5000)/50;
            } else if (bluMode == 4 || bluMode == 6) {
                bluLevel = counter1/100;
            } else if (bluMode == 5 || bluMode == 7) {
                bluLevel = 100 - counter1/100;
            }

            counter2 = 0;
        }

        // software PWM for RGB LEDs (runs every ~100us w/1MHz CPU)
        if ((counter2 >= redLevel && !redInvert) || (counter2 < redLevel && redInvert)) output_low(REDP, REDN);
        else output_high(REDP, REDN);
        if ((counter2 >= grnLevel && !grnInvert) || (counter2 < grnLevel && grnInvert)) output_low(GRNP, GRNN);
        else output_high(GRNP, GRNN);
        if ((counter2 >= bluLevel && !bluInvert) || (counter2 < bluLevel && bluInvert)) output_low(BLUP, BLUN);
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
                // read new register values into control variables
                applyRegisterConfig();
            }
        #endif
    }
}

#ifdef ATTINY44
    // ATTINYx4 Timer1 comparison interrupt
    ISR(TIM1_COMPA_vect)
    {
        counter1++;
        counter2++;
    }
#endif