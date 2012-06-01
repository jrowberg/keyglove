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



#ifndef _SETUP_FEEDBACK_BLINK_H_
#define _SETUP_FEEDBACK_BLINK_H_

#define KG_BLINK_OFF 0
#define KG_BLINK_LONGBLINK 1
#define KG_BLINK_LONGPULSE 2
#define KG_BLINK_SHORTBLINK 3
#define KG_BLINK_SHORTPULSE 4
#define KG_BLINK_SOLID 5

uint8_t blinkMode; // blink LED mode

void set_blink_logic(uint8_t logic) {
    // simple wrapper for consistency
    digitalWrite(KG_PIN_BLINK, logic);
}

void set_blink_mode(uint8_t mode) {
    blinkMode = mode;
    // logic: mode=0 -> off, mode=5 -> on, else no immediate change
    if (blinkMode == 0) set_blink_logic(0);
    else if (blinkMode == 5) set_blink_logic(1);
}

void setup_feedback_blink() {
    pinMode(KG_PIN_BLINK, OUTPUT);
    digitalWrite(KG_PIN_BLINK, LOW);
    blinkMode = KG_BLINK_LONGBLINK;
}

void update_feedback_blink() {
    if      (blinkMode == 1 && keygloveTick % 50 == 0) set_blink_logic(keygloveTick % 100 >= 50 ? 0 : 1);
    else if (blinkMode == 2 && keygloveTick % 25 == 0) set_blink_logic(keygloveTick % 100 >= 25 ? 0 : 1);
    else if (blinkMode == 3 && keygloveTick % 10 == 0) set_blink_logic(keygloveTick %  20 >= 10 ? 0 : 1);
    else if (blinkMode == 4 && keygloveTick %  5 == 0) set_blink_logic(keygloveTick %  20 >=  5 ? 0 : 1);
}

#endif // _SETUP_FEEDBACK_BLINK_H_


