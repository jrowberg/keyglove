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



#ifndef _SETUP_FEEDBACK_PIEZO_H_
#define _SETUP_FEEDBACK_PIEZO_H_

#define KG_PIEZO_OFF 0
#define KG_PIEZO_LONGBEEP 1
#define KG_PIEZO_LONGPULSE 2
#define KG_PIEZO_SHORTBEEP 3
#define KG_PIEZO_SHORTPULSE 4
#define KG_PIEZO_TINYBEEP 5
#define KG_PIEZO_TINYPULSE 6
#define KG_PIEZO_SOLID 7

#define KG_PIEZO_DEFAULT_FREQ 1760

uint8_t piezoMode;
uint16_t piezoTick;
uint16_t piezoTickLimit;
uint16_t piezoFrequency;

void set_piezo_mode(uint8_t mode, uint8_t duration, uint16_t frequency) {
    piezoMode = mode;
    piezoFrequency = frequency;
    if (mode == KG_PIEZO_OFF) noTone(KG_PIN_PIEZO);
    else if (mode == KG_PIEZO_SOLID) tone(KG_PIN_PIEZO, frequency);
    piezoTick = 0;
    piezoTickLimit = duration;
}

void set_piezo_mode(uint8_t mode, uint8_t duration) {
    set_piezo_mode(mode, duration, KG_PIEZO_DEFAULT_FREQ);
}

void set_piezo_mode(uint8_t mode) {
    set_piezo_mode(mode, 0, KG_PIEZO_DEFAULT_FREQ);
}

void setup_feedback_piezo() {
    pinMode(KG_PIN_PIEZO, OUTPUT);
    digitalWrite(KG_PIN_PIEZO, LOW);
    // SELF-TEST
    //set_piezo_mode(KG_PIEZO_TINYPULSE, 20);
}

void update_feedback_piezo() {
    if (piezoMode > 0) {
        if      (piezoMode == 1 && piezoTick % 50 == 0) { if (piezoTick % 100 >= 50) noTone(KG_PIN_PIEZO); else tone(KG_PIN_PIEZO, piezoFrequency); }
        else if (piezoMode == 2 && piezoTick % 25 == 0) { if (piezoTick % 100 >= 25) noTone(KG_PIN_PIEZO); else tone(KG_PIN_PIEZO, piezoFrequency); }
        else if (piezoMode == 3 && piezoTick % 10 == 0) { if (piezoTick %  20 >= 10) noTone(KG_PIN_PIEZO); else tone(KG_PIN_PIEZO, piezoFrequency); }
        else if (piezoMode == 4 && piezoTick %  5 == 0) { if (piezoTick %  20 >=  5) noTone(KG_PIN_PIEZO); else tone(KG_PIN_PIEZO, piezoFrequency); }
        else if (piezoMode == 5 && piezoTick %  5 == 0) { if (piezoTick %  10 >=  5) noTone(KG_PIN_PIEZO); else tone(KG_PIN_PIEZO, piezoFrequency); }
        else if (piezoMode == 6 && piezoTick %  2 == 0) { if (piezoTick %  10 >=  2) noTone(KG_PIN_PIEZO); else tone(KG_PIN_PIEZO, piezoFrequency); }
        if (++piezoTick >= piezoTickLimit && piezoTickLimit > 0) set_piezo_mode(KG_PIEZO_OFF);
    }
}

#endif // _SETUP_FEEDBACK_PIEZO_H_


