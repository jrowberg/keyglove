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



#ifndef _SUPPORT_HOSTIF_PS2_H_
#define _SUPPORT_HOSTIF_PS2_H_

#include "PS2keyboard.h"
#include "PS2mouse.h"

PS2dev ps2k(KB_PIN_CLOCK, KB_PIN_DATA);
PS2keyboard keyboard(&ps2k);

PS2dev ps2m(MOUSE_PIN_CLOCK, MOUSE_PIN_DATA);
PS2mouse mouse(&ps2m);

void ps2keyboardInterrupt() {
    keyboard.process_command();
}

void ps2mouseInterrupt() {
    mouse.process_command();
}

void setup_hostif_ps2() {
    keyboard.initialize();
    mouse.initialize();
    attachInterrupt(KB_INT1, ps2keyboardInterrupt, LOW);
    attachInterrupt(KB_INT2, ps2keyboardInterrupt, LOW);
    attachInterrupt(MOUSE_INT1, ps2mouseInterrupt, LOW);
    attachInterrupt(MOUSE_INT2, ps2mouseInterrupt, LOW);
}

#endif // _SUPPORT_HOSTIF_PS2_H_


