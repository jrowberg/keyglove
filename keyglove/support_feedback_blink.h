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



#ifndef _SUPPORT_FEEDBACK_BLINK_H_
#define _SUPPORT_FEEDBACK_BLINK_H_

#define KG_BLINK_OFF            0
#define KG_BLINK_SOLID          1
#define KG_BLINK_200_100        2
#define KG_BLINK_200_50         3
#define KG_BLINK_1000_500       4
#define KG_BLINK_1000_100       5
#define KG_BLINK_1000_100_2X    6
#define KG_BLINK_1000_100_3X    7
#define KG_BLINK_3000_1000      8
#define KG_BLINK_3000_100       9
#define KG_BLINK_3000_100_2X    10
#define KG_BLINK_3000_100_3X    11

uint8_t blinkMode; // blink LED mode
uint8_t blinkTick;
uint8_t blinkLoop;
uint8_t blinkMod;

void set_blink_logic(uint8_t logic) {
    // simple wrapper for consistency
    digitalWrite(KG_PIN_BLINK, logic);
}

void set_blink_mode(uint8_t mode) {
    blinkMode = mode;
    blinkTick = 0;
    blinkLoop = 0;
    // logic: mode=0 -> off, mode=5 -> on, else no immediate change
    if (blinkMode == KG_BLINK_OFF) set_blink_logic(0);
    else if (blinkMode == KG_BLINK_SOLID) set_blink_logic(1);
    else if (blinkMode == KG_BLINK_200_100) blinkLoop = 4;
    else if (blinkMode == KG_BLINK_200_50) blinkLoop = 4;
    else if (blinkMode == KG_BLINK_1000_500) blinkLoop = 20;
    else if (blinkMode == KG_BLINK_1000_100) blinkLoop = 20;
    else if (blinkMode == KG_BLINK_1000_100_2X) blinkLoop = 20;
    else if (blinkMode == KG_BLINK_1000_100_3X) blinkLoop = 20;
    else if (blinkMode == KG_BLINK_3000_1000) blinkLoop = 60;
    else if (blinkMode == KG_BLINK_3000_100) blinkLoop = 60;
    else if (blinkMode == KG_BLINK_3000_100_2X) blinkLoop = 60;
    else if (blinkMode == KG_BLINK_3000_100_3X) blinkLoop = 60;

    // send kg_evt_feedback_blink_mode packet (if we aren't just intentionally setting it already)
    if (!inBinPacket) {
        uint8_t payload[1] = { blinkMode };
        send_keyglove_packet(KG_PACKET_TYPE_EVENT, 1, KG_PACKET_CLASS_FEEDBACK, KG_PACKET_ID_EVT_FEEDBACK_BLINK_MODE, payload);
    }
}

void setup_feedback_blink() {
    pinMode(KG_PIN_BLINK, OUTPUT);
    digitalWrite(KG_PIN_BLINK, LOW);
    set_blink_mode(KG_BLINK_3000_100);
    blinkTick = 0;
}

void update_feedback_blink() {
    // each "keygloveTick" is 10ms, loops at 100 (1 second)
    // each "blinkTick" is 50ms, loops at cycle period (max 12.5 seconds = 250, near 255)
    if (blinkLoop && (keygloveTick % 5) == 0) {
        blinkMod = blinkTick % blinkLoop;
        if (blinkMode == KG_BLINK_200_100 && blinkTick % 2 == 0) {
            set_blink_logic(blinkMod >= 2 ? 0 : 1);
        } else if (blinkMode == KG_BLINK_200_50) {
            set_blink_logic(blinkMod >= 1 ? 0 : 1);
        } else if (blinkMode == KG_BLINK_1000_500 && blinkTick % 10 == 0) {
            set_blink_logic(blinkMod >= 10 ? 0 : 1);
        } else if (blinkMode == KG_BLINK_1000_100 && blinkTick % 2 == 0) {
            set_blink_logic(blinkMod >= 2 ? 0 : 1);
        } else if (blinkMode == KG_BLINK_1000_100_2X && blinkTick % 2 == 0) {
            set_blink_logic(((blinkMod >= 2 && blinkMod < 4) || (blinkMod >= 6)) ? 0 : 1);
        } else if (blinkMode == KG_BLINK_1000_100_3X && blinkTick % 2 == 0) {
            set_blink_logic(((blinkMod >= 2 && blinkMod < 4) || (blinkMod >= 6 && blinkMod < 8) || (blinkMod >= 10)) ? 0 : 1);
        } else if (blinkMode == KG_BLINK_3000_1000 && blinkTick % 20 == 0) {
            set_blink_logic(blinkMod >= 20 ? 0 : 1);
        } else if (blinkMode == KG_BLINK_3000_100 && blinkTick % 2 == 0) {
            set_blink_logic(blinkMod >= 2 ? 0 : 1);
        } else if (blinkMode == KG_BLINK_3000_100_2X && blinkTick % 2 == 0) {
            set_blink_logic(((blinkMod >= 2 && blinkMod < 4) || (blinkMod >= 6)) ? 0 : 1);
        } else if (blinkMode == KG_BLINK_3000_100_3X && blinkTick % 2 == 0) {
            set_blink_logic(((blinkMod >= 2 && blinkMod < 4) || (blinkMod >= 6 && blinkMod < 8) || (blinkMod >= 10)) ? 0 : 1);
        }
        blinkTick++;
        if (blinkTick == blinkLoop) blinkTick = 0;
    }
}

#endif // _SUPPORT_FEEDBACK_BLINK_H_
