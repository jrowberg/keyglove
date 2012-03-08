// Keyglove controller source code - RGB/Vibe/Piezo feedback module ATTinyX4 driver code
// 3/7/2012 by Jeff Rowberg <jeff@rowberg.net>
//
// Changelog:
//     2012-03-07 - second draft, almost there!
//     2012-02-15 - initial draft

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
#include <stdlib.h>

#define ENABLE_I2C_SLAVE // adds only 98 bytes to flash size, wow!

#ifdef ENABLE_I2C_SLAVE
    #include "usiTwiSlave.h"
    #define I2C_SLAVE_ADDRESS 0x57
#endif

// leave room for implmentations on alternate AVRs
#define ATTINYX4

#ifdef ATTINYX4
    // ATTinyX4 pins
    #define VIBP PORTA // vib = PA3
    #define VIBD DDRA
    #define VIBN 3

    #define SPKP PORTA // spk = PA5/OC1B
    #define SPKD DDRA
    #define SPKN 5

    #define REDP PORTA // red = PA0
    #define REDD DDRA
    #define REDN 0

    #define GRNP PORTA // green = PA1
    #define GRND DDRA
    #define GRNN 1

    #define BLUP PORTA // blue = PA2
    #define BLUD DDRA
    #define BLUN 2

    // PORT A: ..S.VBGR
#endif

// thanks to LadyAda for these helpers
// http://www.ladyada.net/learn/proj1/blinky.html
#define output_low(port, pin) port &= ~(1 << pin)
#define output_high(port, pin) port |= (1 << pin)
#define output_toggle(port, pin) port ^= (1 << pin)
#define set_input(portdir, pin) portdir &= ~(1 << pin)
#define set_output(portdir, pin) portdir |= (1 << pin)

#define NUM_REGISTERS 24

// timing counters (100 usec, 10 msec)
volatile uint8_t tickDev[5];
volatile uint8_t counterDev10[5];
volatile uint8_t counterDev256[5];

uint8_t i, j;

// I2C I/O vars
uint8_t waiting, regAddr, in, count;

// software PWM control levels for RGB
uint16_t devLevel[3];                       // levels for RGB only
uint8_t devMode[5], devInvert[5], devInLow;
uint8_t devBrightMin[3], devBrightMax[3];   // bright min/max for RGB only
uint16_t devPeriod[5];
uint16_t devSquare[5];
uint16_t devIncrement[5];
uint8_t vibOn;
uint16_t spkFreq1, spkFreq2;
int8_t spkStep;
uint8_t spkOn;
int16_t spkFreqActive;

// power-on register default values
uint8_t registers[NUM_REGISTERS] = {
    /* 0x00 RED_MODE   [3 - inv][2:0 - mode]  */ 0x00, // off, no inversion
    /* 0x01 RED_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x02 RED_SQUARE [7:0 - high time %]    */ 0x80, // 50% split high/low
    /* 0x03 RED_BRIGHT [7:4 - max][3:0 - min] */ 0xF0, // 100% max, 0% min

    /* 0x04 GRN_MODE   [3 - inv][2:0 - mode]  */ 0x00, // off, no inversion
    /* 0x05 GRN_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x06 GRN_SQUARE [7:0 - high time %]    */ 0x80, // 50% split high/low
    /* 0x07 GRN_BRIGHT [7:4 - max][3:0 - min] */ 0xF0, // 100% max, 0% min

    /* 0x08 BLU_MODE   [3 - inv][2:0 - mode]  */ 0x00, // off, no inversion
    /* 0x09 BLU_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x0A BLU_SQUARE [7:0 - high time %]    */ 0x80, // 50% split high/low
    /* 0x0B BLU_BRIGHT [7:4 - max][3:0 - min] */ 0xF0, // 100% max, 0% min

    /* 0x0C VIB_MODE   [3 - inv][2:0 - mode]  */ 0x00, // off, no inversion
    /* 0x0D VIB_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x0E VIB_SQUARE [7:0 - high time %]    */ 0x80, // 50% split high/low
    /* 0x0F RESERVED                          */ 0,

    /* 0x10 SPK_MODE   [3 - inv][2:0 - mode]  */ 0x00, // off, no inversion
    /* 0x11 SPK_PERIOD [7:0 - 10ms units]     */ 0x64, // 1sec period (10ms min, 2550ms max)
    /* 0x12 SPK_SQUARE [7:0 - high time %]    */ 0x80, // 50% split high/low
    /* 0x13 RESERVED                          */ 0,

    /* 0x14 SPK_FRQ1_H [7:0 - freq1 Hz MSB]   */ 0x06, // 0x06E0 = 1760Hz
    /* 0x15 SPK_FRQ1_L [7:0 - freq1 Hz LSB]   */ 0xE0, // ^^^
    /* 0x16 SPK_FRQ2_H [7:0 - freq2 Hz MSB]   */ 0x03, // 0x0370 = 880Hz
    /* 0x17 SPK_FRQ2_L [7:0 - freq2 Hz LSB]   */ 0x70  // ^^^
};

/*
RGB mode: 0 = off, 1 = solid, 2 = blink, 3 = fade, 4 = fade in, 5 = fade out, 6 = fade in once, 7 = fade out once
VIB mode: 0 = off, 1 = solid, 2 = periodic
SPK mode: 0 = off, 1 = solid, 2 = periodic, 3 = alternate, 4 = slide up, 5 = slide down, 6 = slide bounce
*/

void applyRegisterByte(uint8_t reg, uint8_t b) {
    i = reg / 4;
    if (reg == 0x14) {   /* 0x13 SPK_FRQ1_H [7:0 - freq1 Hz MSB]   */
        spkFreq1 = (spkFreq1 & 0x00FF) | (b << 8);
    } else if (reg == 0x15) {   /* 0x14 SPK_FRQ1_L [7:0 - freq1 Hz LSB]   */
        spkFreq1 = (spkFreq1 & 0xFF00) | b;
    } else if (reg == 0x16) {   /* 0x15 SPK_FRQ2_H [7:0 - freq2 Hz MSB]   */
        spkFreq2 = (spkFreq2 & 0x00FF) | (b << 8);
    } else if (reg == 0x17) {   /* 0x16 SPK_FRQ2_L [7:0 - freq2 Hz LSB]   */
        spkFreq2 = (spkFreq2 & 0xFF00) | b;
    } else if (reg < 0x14) {
        // combine same-structure registers to save flash space
        // "i" contains set # (0=red, 1=grn, 2=blu, 3=vib, 4=spk)
        // "reg" contains position (0=mode, 1=period, 2=square, 3=bright)
        reg = reg % 4;
        if (reg == 0x00) {          /* 0x00 ..._MODE   [3 - inv][2:0 - mode]  */
            devMode[i] = b & 0x07;
            devInvert[i] = b & 0x08;
        } else if (reg == 0x01) {   /* 0x01 ..._PERIOD [7:0 - 10ms units]     */
            devPeriod[i] = b;
        } else if (reg == 0x02) {   /* 0x02 ..._SQUARE [7:0 - high time %]    */
            devSquare[i] = b;
        } else if (reg == 0x03 && i < 3) {  /* 0x01 ..._BRIGHT [7:4 - max][3:0 - min] */
            devBrightMin[i] = (b & 0x0F) << 4; // scaled from 0-15 to 0-255
            devBrightMax[i] = (b & 0xF0);      // scaled from 0-15 to 0-255
            //if (devBrightMin[i] > 0) devBrightMin[i] += 8;
            //if (devBrightMax[i] > 0) devBrightMax[i] += 8;
        }
    }
    spkStep = abs(spkFreq2 - spkFreq1) >> 8;
}

void loadDefaultRegisters() {
    uint8_t i;
    for (i = 0; i < NUM_REGISTERS; i++) {
        applyRegisterByte(i, registers[i]); //pgm_read_byte(&(defaultRegisters[i])));
    }
}

// main entry point
int main(void) {
    // set feedback pins to output mode
    // SPK mode set to control actual output (INPUT disables OCR1B PWM CONTROL)
    /*set_output(VIBD, VIBN);
    set_input(SPKD, SPKN);
    set_output(REDD, REDN);
    set_output(GRND, GRNN);
    set_output(BLUD, BLUN);*/

    // ATTiny44 PORT A shortcut: ..S.VBGR (saves lots of precious bytes!)
    DDRA |= 0b00001111;

    // turn off all feedback elements
    /*output_high(VIBP, VIBN);
    output_low(SPKP, SPKN);
    output_low(REDP, REDN);
    output_low(GRNP, GRNN);
    output_low(BLUP, BLUN);*/

    // ATTiny44 PORT A shortcut: ..S.VBGR (saves lots of precious bytes!)
    //PORTA |= 0b00001000;

    #ifdef ATTINYX4
        // setup timers/interrupts (ATTINYx4)
        // Timer 0 is 8-bit, used for 100us and 10ms time slicing, soft PWM
        // Timer 1 is 16-bit, used for Piezo frequency control
        // (See Section 11 of ATTiny44A datasheet, esp. around page 80 and 106)
        // (Also: http://www.avrbeginners.net/architecture/timers/timers.html#timer_regs)
        // (Also: http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=109241)

        // Timer/Counter 0 Control Register A
        //    7        6        5        4      3   2     1       0
        // [COM0A1] [COM0A0] [COM0B1] [COM0B0] [-] [-] [WGM01] [WGM00]
        // -> WGM01=1: waveform generation mode is CTC, use OCRxA value as TOP
        TCCR0A = 0b00000010;

        // Timer/Counter 0 Control Register B
        //    7       6     5   4     3      2      1      0
        // [FOC0A] [FOC0B] [-] [-] [WGM02] [CS02] [CS01] [CS00]
        // -> CS01=1: clock source is CLK/8 (prescaled to 1MHz, CLK is 8MHz)
        TCCR0B = 0b00000010;

        // Timer/Counter 0 Interrupt Mask Register
        //  7   6   5   4   3     2        1        0
        // [-] [-] [-] [-] [-] [OCIE0B] [OCIE0A] [TOIE0]
        // -> OCIE0A=1: enable Output Compare Match A Interrupt (TIM0_COMPA_vect)
        //TIMSK0 = 0b00000010;

        // Output Compare A Register for Timer 0 (8-bit)
        // -> . 100 prescaled cycles per interrupt,
        // -> . running at 8MHz and prescaled to 1MHz,
        // -> . gives interrupts at 10KHz, or:
        // -> . 1 interrupt every 100us
        // -> . 100 interrupts every 10ms
        //
        // BUT ideally we have 256 interrupts every = 10ms, so...
        // 10000us / 256 = 39.0625 ~= 39
        OCR0A = 39;

        // GREAT NORTHERN BEANS! I rewrote this timer setup code from scratch
        // using the ATTiny44A datasheet for reference, and it worked EXACTLY
        // as it was supposed to on the first compile/flash. Unbelievable.

        // Timer/Counter 1 Control Register A
        //    7        6        5        4      3   2     1       0
        // [COM1A1] [COM1A0] [COM1B1] [COM1B0] [-] [-] [WGM11] [WGM10]
        // -> COM1Bx=10: Clear OC1A/OC1B on Compare Match, set at BOTTOM (fast PWM)
        // -> WGM11=1: waveform generation mode is 16-bit fast PWM (ICR1 is TOP)
        TCCR1A = 0b00100010;

        // Timer/Counter 1 Control Register B
        //    7       6     5     4       3      2      1      0
        // [ICNC1] [ICES1] [-] [WGM13] [WGM12] [CS12] [CS11] [CS10]
        // -> WGM13=1, WGM12=1: waveform generation mode is 16-bit fast PWM (ICR1 is TOP)
        // -> CS11=1: clock source is CLK/8 (prescaled to 1MHz, CLK is 8MHz)
        TCCR1B = 0b00110010;

        // (no interrupts required for Timer 1

        // Timer/Counter 1 Input Capture Register (used as TOP for Fast PWM)
        //ICR1 = (500000L / 1760) - 1;
        // ^^^ this gets set by speaker tick code

        // Output Compare B Register for Timer 1 (16-bit)
        //OCR1B = ICR1 / 2;
        // ^^^ this also gets set by speaker tick code
    #endif

    // enable interrupts
    sei();

    #ifdef ENABLE_I2C_SLAVE
        // start the USI/I2C process
        usiTwiSlaveInit(I2C_SLAVE_ADDRESS);
    #endif

    // read default register values into control variables
    loadDefaultRegisters();

    // DEBUG: set red LED in blinking mode
    applyRegisterByte(0x00, 0x02);

    // main loop
    for (;;) {
        // RGB control variables (same for each channel):
        // mode = 0-7
        // invert = true/false (reverse on/off time, not PWM waveform)
        // brightMin = 0-255 in increments of 8
        // brightMax = 0-255 in increments of 8
        // period = 0-255 (describes cycle time in 10ms units)
        // square = 0-255 (describes amount of "on" time for cycles)
        // counterX10 = 0-255 (increments every [2550/period] ms)

        for (i = 0; i < 3; i++) {
            if (tickDev[i]) { // runs every counterX10 increment
                tickDev[i] = 0;
                devInLow = counterDev10[i] > devSquare[i];

                // adjust RED LED according to timing
                devLevel[i] = devBrightMax[i];
                if (devMode[i] == 0) {
                    // off/min
                    devLevel[i] = devBrightMin[i];
                } /*else if (devMode[i] == 1) {
                    // on/max solid
                    //devLevel[i] = devBrightMax[i];
                }*/ else if (devMode[i] == 2) {
                    // hard blink:
                    // on/max for HIGH part of square wave, off/min for LOW
                    if (devInLow) devLevel[i] = devBrightMin[i];
                    //else devLevel[i] = devBrightMax[i];
                } else {
                    // fade modes, start with basic fade-in level from 0-255
                    devLevel[i] = counterDev10[i];
                    if (devMode[i] == 3) {
                        // pulsing fade:
                        // fade from MIN to MAX during HIGH part of square wave
                        // fade from MAX to MIN during LOW part of square wave
                        if (devInLow) {
                            // fade out (from MAX to MIN)
                            devLevel[i] = 255 - devLevel[i];
                        } //else {
                            // fade in (from MIN to MAX)
                        //}
                    //} else if (devMode[i] == 4) { // default behavior
                    } else if (devMode[i] == 5 || devMode[i] == 7) {
                        // fade out and repeat, or fade out once
                        devLevel[i] = devBrightMax[i] - devLevel[i];
                        if (counterDev10[i] == 255) devMode[i] = 0; // switch to solid off
                    } else if (devMode[i] == 6) {
                        // fade in and repeat, or fade in once
                        if (counterDev10[i] == 255) devMode[i] = 2; // switch to solid on
                    }
                    
                    // scale the brightness if necessary
                    if (devBrightMin[i] > 0 || devBrightMax[i] < 255) {
                        devLevel[i] *= (devBrightMax[i] - devBrightMin[i]);
                        devLevel[i] >>= 8;
                        devLevel[i] += devBrightMin[i];
                    }
                }
            }
        }
        if (tickDev[3]) {   // vibration motor
            tickDev[3] = 0;

            // adjust vibrating motor according to timing
            vibOn = 1;
            if (devMode[3] == 0) {
                // off
                vibOn = 0;
            } /*else if (devMode[3] == 1) {
                // on solid
                //vibOn = 1;
            }*/ else if (devMode[3] == 2) {
                // on/off cycling:
                // on for HIGH part of square wave, off for LOW
                if (counterDev10[3] > devSquare[3]) vibOn = 0;
                //else vibOn = 1;
                if (devInvert[3]) vibOn = !vibOn;
            }

            // (vibe is active low)
            if (vibOn) output_low(VIBP, VIBN);
            else output_high(VIBP, VIBN);
        }
        if (tickDev[4]) {   // piezo buzzer
            tickDev[4] = 0;
            devInLow = counterDev10[4] > devSquare[4];

            // adjust piezo buzzer according to timing
            spkFreqActive = spkFreq1;
            spkOn = 1;
            if (devMode[4] == 0) {
                // off
                spkOn = 0;
            } /*else if (devMode[4] == 1) {
                // on solid
                //spkOn = 1;
            }*/ else if (devMode[4] == 2) {
                // on/off cycling:
                // on for HIGH part of square wave, off for LOW
                if (devInLow) spkOn = 0;
                //else spkOn = 1;
                if (devInvert[4]) spkOn = !spkOn;
            } else if (devMode[4] == 3) {
                // alternate between freq1 and freq2
                if (devInvert[4] && !devInLow) spkFreqActive = spkFreq2;
                else if (devInLow) spkFreqActive = spkFreq2;
            } else if (devMode[4] > 3) {
                // slide from freq1 to freq2 (4), or from 2 to 1 (5), or bounce (6)
                spkFreqActive = spkFreq1 - (spkStep * counterDev10[4]);
                /*if (devMode[4] == 4) {
                    //spkFreqActive = spkFreq1 - (spkStep * counterDev10[4]);
                } else*/ if (devMode[4] == 5) {
                    spkFreqActive = spkFreq2 + (spkStep * counterDev10[4]);
                } else if (devMode[4] == 6) {
                    if (devInLow) {
                        spkFreqActive = spkFreq2 + (spkStep * counterDev10[4]);
                    //} else {
                        //spkFreqActive = spkFreq2 + (spkStep * counterDev10[4]);
                    }
                }
            }

            // (piezo is driven by hardware PWM)
            if (spkOn) {
                //ICR1 = (500000L / spkFreqActive) - 1;
                //OCR1B = ICR1 / 2;
                set_output(SPKD, SPKN);
            } else set_input(SPKD, SPKN);

            count = 0; // reset byte count for I2C traffic here
        }

        // software PWM for RGB LEDs (runs every ~100us w/1MHz CPU)
        if ((counterDev256[0] >= devLevel[0] && !devInvert[0]) || (counterDev256[0] < devLevel[0] && devInvert[0])) output_low(REDP, REDN);
        else output_high(REDP, REDN);
        if ((counterDev256[1] >= devLevel[1] && !devInvert[1]) || (counterDev256[1] < devLevel[1] && devInvert[1])) output_low(GRNP, GRNN);
        else output_high(GRNP, GRNN);
        if ((counterDev256[2] >= devLevel[2] && !devInvert[2]) || (counterDev256[2] < devLevel[2] && devInvert[2])) output_low(BLUP, BLUN);
        else output_high(BLUP, BLUN);

        #ifdef ENABLE_I2C_SLAVE
            // check for I2C commands from master
            while (usiTwiDataInReceiveBuffer()) {
                // byte available, yay!
                in = usiTwiReceiveByte();
                if (count == 0) {
                    // first byte, internal register address
                    regAddr = in;
                    count++;
                } else if (regAddr < NUM_REGISTERS) {
                    // 2nd byte, apply byte to register and reset count
                    applyRegisterByte(regAddr, in);
                    count = 0;
                }
            }
        #endif
    }
}

#ifdef ATTINYX4
    // ATTINYx4 Timer0 (8-bit) comparison A interrupt
    ISR(TIM0_COMPA_vect)
    {
        for (j = 0; j < 5; j++) {
            if (devPeriod[j] == ++counterDev256[j]) {
                tickDev[j] = 1;
                counterDev256[j] = 0;
                counterDev10[j]++;
            }
        }
    }
#endif