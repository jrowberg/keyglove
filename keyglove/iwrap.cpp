// Keyglove controller source code - PS/2 keyboard class library
// 4/13/2011 by Jeff Rowberg <jeff@rowberg.net>

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

#ifndef USE_LUFA
    #include "WProgram.h"
#endif

#include "iwrap.h"

#ifdef round
    #undef round
#endif

iWRAP::iWRAP(HardwareSerial *u) {
    keyboard_keys[0] = 0;
    keyboard_keys[1] = 0;
    keyboard_keys[2] = 0;
    keyboard_keys[3] = 0;
    keyboard_keys[4] = 0;
    keyboard_keys[5] = 0;
    keyboard_modifier_keys = 0;
    mouse_buttons = 0;
    uart = u;
}

void iWRAP::parse(int ch) {
    Serial.print(ch, BYTE);
}

void iWRAP::set_modifier(uint8_t c) {
    keyboard_modifier_keys = c;
}

void iWRAP::set_key1(uint8_t c) {
    keyboard_keys[0] = c;
}

void iWRAP::set_key2(uint8_t c) {
    keyboard_keys[1] = c;
}

void iWRAP::set_key3(uint8_t c) {
    keyboard_keys[2] = c;
}

void iWRAP::set_key4(uint8_t c) {
    keyboard_keys[3] = c;
}

void iWRAP::set_key5(uint8_t c) {
    keyboard_keys[4] = c;
}

void iWRAP::set_key6(uint8_t c) {
    keyboard_keys[5] = c;
}

void iWRAP::send_now() {
    uart -> print(0x9f, BYTE);
    uart -> print(0x0a, BYTE);
    uart -> print(0xa1, BYTE);
    uart -> print(0x01, BYTE);
    uart -> print(keyboard_modifier_keys, BYTE);
    uart -> print(0x00, BYTE);
    uart -> print(keyboard_keys[0], BYTE);
    uart -> print(keyboard_keys[1], BYTE);
    uart -> print(keyboard_keys[2], BYTE);
    uart -> print(keyboard_keys[3], BYTE);
    uart -> print(keyboard_keys[4], BYTE);
    uart -> print(keyboard_keys[5], BYTE);
}

void iWRAP::set_buttons(uint8_t left, uint8_t middle, uint8_t right) {
    uint8_t mask=0;
    if (left) mask |= 1;
    if (middle) mask |= 4;
    if (right) mask |= 2;
    mouse_buttons = mask;
    uart -> print(0x9f, BYTE);
    uart -> print(0x05, BYTE);
    uart -> print(0xa1, BYTE);
    uart -> print(0x02, BYTE);
    uart -> print(mouse_buttons, BYTE);
    uart -> print(0x00, BYTE);
    uart -> print(0x00, BYTE);
}

void iWRAP::move(uint8_t x, uint8_t y) {
    uart -> print(0x9f, BYTE);
    uart -> print(0x05, BYTE);
    uart -> print(0xa1, BYTE);
    uart -> print(0x02, BYTE);
    uart -> print(mouse_buttons, BYTE);
    uart -> print(x, BYTE);
    uart -> print(y, BYTE);
}

void iWRAP::move(uint8_t x, uint8_t y, uint8_t z) {
    uart -> print(0x9f, BYTE);
    uart -> print(0x06, BYTE);
    uart -> print(0xa1, BYTE);
    uart -> print(0x02, BYTE);
    uart -> print(mouse_buttons, BYTE);
    uart -> print(x, BYTE);
    uart -> print(y, BYTE);
    uart -> print(z, BYTE);
}

void iWRAP::scroll(uint8_t s) {
    move(0, 0, s);
}
