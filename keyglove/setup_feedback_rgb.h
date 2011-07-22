// Keyglove controller source code - Special hardware setup file
// 7/17/2011 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

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



#ifndef _SETUP_FEEDBACK_RGB_H_
#define _SETUP_FEEDBACK_RGB_H_

#define KG_RGB_OFF 0
#define KG_RGB_LONGBLINK 1
#define KG_RGB_LONGPULSE 2
#define KG_RGB_SHORTBLINK 3
#define KG_RGB_SHORTPULSE 4
#define KG_RGB_SOLID 5

uint8_t rgbBlinkRed;
uint8_t rgbBlinkGreen;
uint8_t rgbBlinkBlue;

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    if (r != 255) digitalWrite(KG_PIN_RGB_RED, r);
    if (g != 255) digitalWrite(KG_PIN_RGB_GREEN, g);
    if (b != 255) digitalWrite(KG_PIN_RGB_BLUE, b);
}

void setup_feedback_rgb() {
    pinMode(KG_PIN_RGB_RED, OUTPUT);
    pinMode(KG_PIN_RGB_GREEN, OUTPUT);
    pinMode(KG_PIN_RGB_BLUE, OUTPUT);
    set_rgb(1, 1, 1); // turn everything on and wait 1/10 sec
    delay(100);
    set_rgb(0, 0, 0); // turn everything off again
}

#endif // _SETUP_FEEDBACK_RGB_H_