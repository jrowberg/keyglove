// Keyglove controller source code - Touchset function definition header file
// 7/15/2010 by Jeff Rowberg <jeff@rowberg.net>

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

#ifndef _TOUCHSET_HELPERS_H_
#define _TOUCHSET_HELPERS_H_

// touchset key/mouse status and mode stack
uint8_t modifiersDown = 0;
uint8_t keysDown[] = { 0, 0, 0, 0, 0, 0 };
uint8_t mouseDown = 0;
uint8_t modeStack[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t modeStackPos = 0;

// declare these here so setmode() etc. have some context
static void activate_mode(uint8_t mode);
static void deactivate_mode(uint8_t mode);

uint8_t modeCheck(uint8_t mode, uint8_t pos) {
    if (modeStackPos < pos) {
        return false;
    }
    return modeStack[modeStackPos - pos] == mode;
}

void setmode(uint8_t mode) {
    DEBUG_PRN_TOUCHSET("touchset setmode ");
    DEBUG_PRNL_TOUCHSET(mode);
    for (; modeStackPos > 0; modeStackPos--) deactivate_mode(modeStack[modeStackPos - 1]);
    modeStackPos = 1;
    modeStack[0] = mode;
    activate_mode(mode);
}

void pushmode(uint8_t mode) {
    DEBUG_PRN_TOUCHSET("touchset pushmode ");
    DEBUG_PRNL_TOUCHSET(mode);
    if (modeStackPos < 10) {
        modeStack[modeStackPos] = mode;
        modeStackPos++;
        activate_mode(mode);
    }
}

void popmode() {
    DEBUG_PRNL_TOUCHSET("touchset popmode");
    if (modeStackPos > 0) deactivate_mode(modeStack[--modeStackPos]);
    if (modeStackPos == 0) modeStack[modeStackPos++] = 0;
    activate_mode(modeStack[modeStackPos - 1]);
}

void togglemode(uint8_t mode) {
    DEBUG_PRN_TOUCHSET("touchset togglemode ");
    DEBUG_PRNL_TOUCHSET(mode);

    // find the mode and disable it if it's in the stack
    uint8_t i;
    for (i = 0; i < modeStackPos && modeStack[i] != mode; i++);
    if (i < modeStackPos) {
        // enabled, so turn it off
        deactivate_mode(mode);
        for (i++; i < modeStackPos; i++) modeStack[i - 1] = modeStack[i];
        modeStackPos--;
    } else {
        // not enabled, so turn it on
        pushmode(mode);
    }
}

void mouseon(uint8_t mode) {
    DEBUG_PRN_TOUCHSET("touchset mouseon ");
    DEBUG_PRNL_TOUCHSET(mode);
    activeMouse = true;
    if (mode == MOUSE_ACTION_MOVE) {
        opt_mouse_mode = MOUSE_MODE_TILT_POSITION;
        if (!activeAccelerometer) enable_motion_accelerometer();
        if (!activeGyroscope) enable_motion_gyroscope();
    } else if (mode == MOUSE_ACTION_SCROLL) {
        opt_scroll_mode = SCROLL_MODE_TILT_POSITION;
        if (!activeAccelerometer) enable_motion_accelerometer();
        if (!activeGyroscope) enable_motion_gyroscope();
    }
}

void mouseoff(uint8_t mode) {
    DEBUG_PRN_TOUCHSET("touchset mouseoff ");
    DEBUG_PRNL_TOUCHSET(mode);
    activeMouse = false;
    if (mode == MOUSE_ACTION_MOVE) {
        opt_mouse_mode = 0;
        if (activeAccelerometer) disable_motion_accelerometer();
        if (activeGyroscope) disable_motion_gyroscope();
    } else if (mode == MOUSE_ACTION_SCROLL) {
        opt_scroll_mode = 0;
        if (activeAccelerometer) disable_motion_accelerometer();
        if (activeGyroscope) disable_motion_gyroscope();
    }
}

void mousedown(uint8_t button) {
    DEBUG_PRN_TOUCHSET("touchset mousedown ");
    DEBUG_PRNL_TOUCHSET(button);
    mouseDown = mouseDown | button;
    #ifdef ENABLE_USB
        Mouse.set_buttons((mouseDown & 1) > 0 ? 1 : 0, (mouseDown & 2) > 0 ? 1 : 0, (mouseDown & 4) > 0 ? 1 : 0);
    #endif /* ENABLE_USB */
    #ifdef ENABLE_RX400
        RX400.set_buttons((mouseDown & 1) > 0 ? 1 : 0, (mouseDown & 2) > 0 ? 1 : 0, (mouseDown & 4) > 0 ? 1 : 0);
    #endif /* ENABLE_RX400 */
}

void mouseup(uint8_t button) {
    DEBUG_PRN_TOUCHSET("touchset mouseup ");
    DEBUG_PRNL_TOUCHSET(button);
    if ((mouseDown & button) > 0) {
        mouseDown -= button;
        #ifdef ENABLE_USB
            Mouse.set_buttons((mouseDown & 1) > 0 ? 1 : 0, (mouseDown & 2) > 0 ? 1 : 0, (mouseDown & 4) > 0 ? 1 : 0);
        #endif /* ENABLE_USB */
        #ifdef ENABLE_RX400
            RX400.set_buttons((mouseDown & 1) > 0 ? 1 : 0, (mouseDown & 2) > 0 ? 1 : 0, (mouseDown & 4) > 0 ? 1 : 0);
        #endif /* ENABLE_RX400 */
    }
}

void mouseclick(uint8_t button) {
    DEBUG_PRN_TOUCHSET("touchset mouseclick ");
    DEBUG_PRNL_TOUCHSET(button);
    mousedown(button);
    delay(5);
    mouseup(button);
}

void keydown(uint8_t code) {
    DEBUG_PRN_TOUCHSET("touchset keydown ");
    DEBUG_PRNL_TOUCHSET(code);
    uint8_t usePos = 0;
    for (usePos = 0; usePos < 6 && keysDown[usePos] != 0; usePos++);
    if (usePos == 6) return; // out of HID keyboard buffer space, REALLY weird for the Keyglove!
    keysDown[usePos] = code;
    switch (usePos) {
        case 0:
            #ifdef ENABLE_USB
                Keyboard.set_key1(code);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.set_key1(code);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.set_key1(code);
            #endif /* ENABLE_RX400 */
            break;
        case 1:
            #ifdef ENABLE_USB
                Keyboard.set_key2(code);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.set_key2(code);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.set_key2(code);
            #endif /* ENABLE_RX400 */
            break;
        case 2:
            #ifdef ENABLE_USB
                Keyboard.set_key3(code);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.set_key3(code);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.set_key3(code);
            #endif /* ENABLE_RX400 */
            break;
        case 3:
            #ifdef ENABLE_USB
                Keyboard.set_key4(code);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.set_key4(code);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.set_key4(code);
            #endif /* ENABLE_RX400 */
            break;
        case 4:
            #ifdef ENABLE_USB
                Keyboard.set_key5(code);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.set_key5(code);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.set_key5(code);
            #endif /* ENABLE_RX400 */
            break;
        case 5:
            #ifdef ENABLE_USB
                Keyboard.set_key6(code);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.set_key6(code);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.set_key6(code);
            #endif /* ENABLE_RX400 */
            break;
    }
    #ifdef ENABLE_USB
        Keyboard.send_now();
    #endif /* ENABLE_USB */
    #ifdef ENABLE_BLUETOOTH
        bluetooth.send_now();
    #endif /* ENABLE_BLUETOOTH */
    #ifdef ENABLE_RX400
        RX400.send_now();
    #endif /* ENABLE_RX400 */
}

void keyup(uint8_t code) {
    DEBUG_PRN_TOUCHSET("touchset keyup ");
    DEBUG_PRNL_TOUCHSET(code);
    uint8_t usePos = 0;
    for (usePos = 0; usePos < 6 && keysDown[usePos] != code; usePos++);
    if (usePos == 6) return; // key not currently down...oops.
    keysDown[usePos] = 0;
    switch (usePos) {
        case 0:
            #ifdef ENABLE_USB
                Keyboard.set_key1(0);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.set_key1(0);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.set_key1(0);
            #endif /* ENABLE_RX400 */
            break;
        case 1:
            #ifdef ENABLE_USB
                Keyboard.set_key2(0);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.set_key2(0);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.set_key2(0);
            #endif /* ENABLE_RX400 */
            break;
        case 2:
            #ifdef ENABLE_USB
                Keyboard.set_key3(0);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.set_key3(0);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.set_key3(0);
            #endif /* ENABLE_RX400 */
            break;
        case 3:
            #ifdef ENABLE_USB
                Keyboard.set_key4(0);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.set_key4(0);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.set_key4(0);
            #endif /* ENABLE_RX400 */
            break;
        case 4:
            #ifdef ENABLE_USB
                Keyboard.set_key5(0);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.set_key5(0);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.set_key5(0);
            #endif /* ENABLE_RX400 */
            break;
        case 5:
            #ifdef ENABLE_USB
                Keyboard.set_key6(0);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.set_key6(0);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.set_key6(0);
            #endif /* ENABLE_RX400 */
            break;
    }
    #ifdef ENABLE_USB
        Keyboard.send_now();
    #endif /* ENABLE_USB */
    #ifdef ENABLE_BLUETOOTH
        bluetooth.send_now();
    #endif /* ENABLE_BLUETOOTH */
    #ifdef ENABLE_RX400
        RX400.send_now();
    #endif /* ENABLE_RX400 */
}

void keypress(uint8_t code) {
    DEBUG_PRN_TOUCHSET("touchset keypress ");
    DEBUG_PRNL_TOUCHSET(code);
    keydown(code);
    delay(5);
    keyup(code);
}

void modifierdown(uint8_t code) {
    DEBUG_PRN_TOUCHSET("touchset modifierdown ");
    DEBUG_PRNL_TOUCHSET(code);
    modifiersDown = modifiersDown | code;
    #ifdef ENABLE_USB
        Keyboard.set_modifier(modifiersDown);
        Keyboard.send_now();
    #endif /* ENABLE_USB */
    #ifdef ENABLE_RX400       
        RX400.set_modifier(modifiersDown);
        RX400.send_now();
    #endif /* ENABLE_RX400 */
}

void modifierup(uint8_t code) {
    DEBUG_PRN_TOUCHSET("touchset modifierup ");
    DEBUG_PRNL_TOUCHSET(code);
    if ((modifiersDown & code) > 0) {
        modifiersDown -= code;
        #ifdef ENABLE_USB
            Keyboard.set_modifier(modifiersDown);
            Keyboard.send_now();
        #endif /* ENABLE_USB */
        #ifdef ENABLE_RX400
            RX400.set_modifier(modifiersDown);
            RX400.send_now();
        #endif /* ENABLE_RX400 */
    }
}

void togglemodifier(uint8_t code) {
    DEBUG_PRN_TOUCHSET("touchset modifiertoggle ");
    DEBUG_PRNL_TOUCHSET(code);
    if ((modifiersDown & code) > 0) {
        modifierup(code);
    } else {
        modifierdown(code);
    }
}

void modifierpress(uint8_t code) {
    DEBUG_PRN_TOUCHSET("touchset modifierpress ");
    DEBUG_PRNL_TOUCHSET(code);
    modifierdown(code);
    delay(5);
    modifierup(code);
}

void redled(uint8_t mode, uint16_t duration) {
    DEBUG_PRN_TOUCHSET("touchset redled ");
    DEBUG_PRN_TOUCHSET(mode);
    DEBUG_PRN_TOUCHSET(" ");
    DEBUG_PRNL_TOUCHSET(duration);
    #if (KG_FEEDBACK & KG_FEEDBACK_RGB) > 0
        if (mode <= 5) set_rgb_mode(mode, -1, -1);
    #endif /* KG_FEEDBACK_RGB */
}

void greenled(uint8_t mode, uint16_t duration) {
    DEBUG_PRN_TOUCHSET("touchset greenled ");
    DEBUG_PRN_TOUCHSET(mode);
    DEBUG_PRN_TOUCHSET(" ");
    DEBUG_PRNL_TOUCHSET(duration);
    #if (KG_FEEDBACK & KG_FEEDBACK_RGB) > 0
        if (mode <= 5) set_rgb_mode(-1, mode, -1);
    #endif /* KG_FEEDBACK_RGB */
}

void blueled(uint8_t mode, uint16_t duration) {
    DEBUG_PRN_TOUCHSET("touchset blueled ");
    DEBUG_PRN_TOUCHSET(mode);
    DEBUG_PRN_TOUCHSET(" ");
    DEBUG_PRNL_TOUCHSET(duration);
    #if (KG_FEEDBACK & KG_FEEDBACK_RGB) > 0
        if (mode <= 5) set_rgb_mode(-1, -1, mode);
    #endif /* KG_FEEDBACK_RGB */
}

void beep(uint16_t pitch, uint8_t mode, uint16_t duration) {
    DEBUG_PRN_TOUCHSET("touchset beep ");
    DEBUG_PRN_TOUCHSET(pitch);
    DEBUG_PRN_TOUCHSET(" ");
    DEBUG_PRN_TOUCHSET(mode);
    DEBUG_PRN_TOUCHSET(" ");
    DEBUG_PRNL_TOUCHSET(duration);
    #if (KG_FEEDBACK & KG_FEEDBACK_PIEZO) > 0
        set_piezo_mode(mode, duration, pitch);
    #endif /* KG_FEEDBACK_PIEZO */
}

void vibrate(uint8_t mode, uint16_t duration) {
    DEBUG_PRN_TOUCHSET("touchset vibrate ");
    DEBUG_PRN_TOUCHSET(mode);
    DEBUG_PRN_TOUCHSET(" ");
    DEBUG_PRNL_TOUCHSET(duration);
    #if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE) > 0
        set_vibrate_mode(mode, duration);
    #endif /* KG_FEEDBACK_VIBRATE */
}

#endif /* _TOUCHSET_HELPERS_H_ */


