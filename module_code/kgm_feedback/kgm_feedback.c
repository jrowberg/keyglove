// Keyglove controller source code - RGB/Vibe/Piezo feedback module ATTinyX4 driver code
// 3/8/2012 by Jeff Rowberg <jeff@rowberg.net>
//
// Changelog:
//     2012-03-08 - reduced functionality
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

#include "usiTwiSlave.h"
#define I2C_SLAVE_ADDRESS 0x57

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

#define NUM_REGISTERS 7

// I2C I/O vars
uint8_t regAddr, count;

uint8_t counter;
uint8_t rgbLevel[3];
uint16_t spkFreq;

// power-on register default values
uint8_t registers[NUM_REGISTERS] = {
    /* 0x00 RED_LEVEL */ 0x00, // off
    /* 0x01 GRN_LEVEL */ 0x00, // off
    /* 0x02 BLU_LEVEL */ 0x00, // off
    /* 0x03 VIB_LEVEL */ 0x00, // off
    /* 0x04 SPK_LEVEL */ 0x00, // off
    /* 0x05 SPK_FREQ_H */ 0x06, // \_____ 1760 Hz = 0x06E0
    /* 0x06 SPK_FREQ_L */ 0xE0, // /
};

void applyRegisterByte(uint8_t reg, uint8_t b) {
    if (reg == 0x05) {   /* 0x13 SPK_FREQ_H [7:0 - freq Hz MSB]   */
        spkFreq = (spkFreq & 0x00FF) | (b << 8);
    } else if (reg == 0x06) {   /* 0x14 SPK_FREQ_L [7:0 - freq Hz LSB]   */
        spkFreq = (spkFreq & 0xFF00) | b;
    } else if (reg == 0x04) {
        // speaker controlled by pin mode (INPUT = off/zero, OUTPUT = on/non-zero)
        // (driven by hardware PWM on 16-bit counter)
        if (b) set_output(SPKD, SPKN);
        else set_input(SPKD, SPKN);
    } else if (reg == 0x03) {
        // vibe is active low
        if (b) output_low(VIBP, VIBN);
        else output_high(VIBP, VIBN);
    } else if (reg < 0x03) {
        // RGB leds are PWM levels from 0 (off) to 255 (full)
        rgbLevel[reg] = b;
    }
    
    // make sure Timer 1 is set to correct frequency
    ICR1 = (500000L / spkFreq) - 1;
    OCR1B = ICR1 / 2;
}

void loadDefaultRegisters() {
    uint8_t i;
    for (i = 0; i < NUM_REGISTERS; i++) {
        applyRegisterByte(i, registers[i]);
    }
}

void resetI2CRXCounter() {
    count = 0;
}

// main entry point
int main(void) {
    // set feedback pins to output mode
    // SPK mode set to control actual output (INPUT disables OCR1B PWM CONTROL)
    // ATTiny44 PORT A shortcut: ..S.VBGR (saves lots of precious bytes!)
    DDRA |= 0b00001111;

    #ifdef ATTINYX4
        // setup timers/interrupts (ATTINYx4)
        // Timer 1 is 16-bit, used for Piezo frequency control
        // (See Section 11 of ATTiny44A datasheet, esp. around page 80 and 106)
        // (Also: http://www.avrbeginners.net/architecture/timers/timers.html#timer_regs)
        // (Also: http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=109241)

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
        // -> CS11=1: clock source is CLK (CPU running at 1MHz for low power)
        TCCR1B = 0b00110010;

        // (no interrupts required for Timer 1

        // Timer/Counter 1 Input Capture Register (used as TOP for Fast PWM)
        //ICR1 = (500000L / 1760) - 1;
        // ^^^ this gets set by applyRegisterByte()

        // Output Compare B Register for Timer 1 (16-bit)
        //OCR1B = ICR1 / 2;
        // ^^^ this also gets set by applyRegisterByte()
    #endif

    // enable interrupts
    sei();

    // start the USI/I2C process
    usiTwiSlaveInit(I2C_SLAVE_ADDRESS);
    usiTwiOnStart(resetI2CRXCounter);

    // read default register values into control variables
    loadDefaultRegisters();

    // DEBUG: turn on red LED at 100%
    //applyRegisterByte(0x00, 0xFF);

    // main loop
    for (;; counter++) {
        // software PWM for RGB LEDs
        if (counter >= rgbLevel[0]) output_low(REDP, REDN);
        else output_high(REDP, REDN);
        if (counter >= rgbLevel[1]) output_low(GRNP, GRNN);
        else output_high(GRNP, GRNN);
        if (counter >= rgbLevel[2]) output_low(BLUP, BLUN);
        else output_high(BLUP, BLUN);

        // check for I2C commands from master
        if (usiTwiDataInReceiveBuffer()) {
            // data available, yay!
            if (count == 0) {
                // first byte, internal register address
                regAddr = usiTwiReceiveByte();
            } else if (regAddr < NUM_REGISTERS) {
                // 2nd byte, apply byte to register and reset count
                applyRegisterByte(regAddr++, usiTwiReceiveByte());
            }
            count++;
        }
    }
}