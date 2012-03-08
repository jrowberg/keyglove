// Keyglove controller source code - RGB/Vibe/Piezo feedback module ATTinyX4 simple I2C test code
// 3/7/2012 by Jeff Rowberg <jeff@rowberg.net>
//
// Changelog:
//     2012-03-04 - initial release

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
#include "usiTwiSlave.h"

int main(void) {
    sei(); // enable interupts

    DDRA |= 0b00000001; // use PA0 for LED output
    PORTA &= 0b11111110; // make sure PA0 is OFF

    // raw 8-bit I2C write address = 0xAE
    // raw 8-bit I2C read address = 0xAF
    const uint8_t slaveAddress = 0x57;
    usiTwiSlaveInit(slaveAddress);
    for(;;) {
        if (usiTwiDataInReceiveBuffer()) {
            if (usiTwiReceiveByte() != 0) {
                // non-zero values set PA0 high
                PORTA |= 0b00000001;
            } else {
                // zero value sets PA0 low
                PORTA &= 0b11111110;
            }
        }
    }
}
