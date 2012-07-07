// Keyglove controller source code - Special hardware setup file
// 5/7/2012 by Jeff Rowberg <jeff@rowberg.net>

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



#ifndef _SUPPORT_FEEDBACKCONN_DIRECT_H_
#define _SUPPORT_FEEDBACKCONN_DIRECT_H_

void set_rgb_logic(uint8_t r, uint8_t g, uint8_t b) {
    if (r != 255) digitalWrite(KG_PIN_RGB_RED, r);
    if (g != 255) digitalWrite(KG_PIN_RGB_GREEN, g);
    if (b != 255) digitalWrite(KG_PIN_RGB_BLUE, b);
}

void set_vibrate_logic(uint8_t vibe) {
    digitalWrite(KG_PIN_VIBRATE, vibe == 0 ? HIGH : LOW); // active low
}

void set_piezo_tone(uint16_t frequency) {
    if (frequency == 0) {
        noTone(KG_PIN_PIEZO);
    } else {
        tone(KG_PIN_PIEZO, frequency);
    }
}

void setup_feedbackconn() {
    // set all feedback elements to off/silent
    pinMode(KG_PIN_RGB_RED, OUTPUT);
    pinMode(KG_PIN_RGB_GREEN, OUTPUT);
    pinMode(KG_PIN_RGB_BLUE, OUTPUT);
    pinMode(KG_PIN_VIBRATE, OUTPUT);
    pinMode(KG_PIN_PIEZO, OUTPUT);
    digitalWrite(KG_PIN_RGB_RED, LOW);
    digitalWrite(KG_PIN_RGB_GREEN, LOW);
    digitalWrite(KG_PIN_RGB_BLUE, LOW);
    digitalWrite(KG_PIN_VIBRATE, HIGH); // transistor switch makes it active-low
    digitalWrite(KG_PIN_PIEZO, LOW);
}

//void update_feedbackconn() {
//}

#endif // _SUPPORT_FEEDBACKCONN_DIRECT_H_