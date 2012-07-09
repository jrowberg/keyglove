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



#ifndef _SUPPORT_TOUCH_H_
#define _SUPPORT_TOUCH_H_

void setmode(uint8_t mode);
void check_sensors_touch(uint32_t sensors1, uint32_t sensors2, uint8_t pos);
void check_sensors_release(uint32_t sensors1, uint32_t sensors2, uint8_t pos);

uint32_t touchBench0, touchBench;
uint8_t touchTick;

uint16_t opt_touch_detect_threshold = 20;   // number of milliseconds required for a touch to register as real

uint32_t touchTime; // detection timestamp
uint8_t adding = false;
uint8_t removing = false;
uint32_t detect1, detect2;
uint32_t verify1, verify2;
uint32_t sensors1, sensors2;
uint8_t mode;

bool activeTouch;

void setup_touch() {
    detect1 = detect2 = 0;
    verify1 = verify2 = 0;
    sensors1 = sensors2 = 0;

    // make sure we enable internal pullup resistors
    for (uint8_t i = 0; i < KG_TOTAL_SENSORS; i++) {
        pinMode(pins[i], INPUT);
        digitalWrite(pins[i], HIGH);
    }

    mode = 0; // set to base mode, no alt/shift/mouse/prog/etc.
    setmode(0); // default touchset mode is always 0

    activeTouch = true; // this is always on if touch control is enabled
}

void enable_touch() {
    activeTouch = true;
}

void disable_touch() {
    activeTouch = false;
}

void update_touch() {
    //touchBench0 = micros();

    detect1 = 0;
    detect2 = 0;
    removing = false;
    
    uint8_t p1, p2;
    uint8_t i;

    // loop through every possible 1-to-1 sensor combination and record levels
    for (i = 0; i < KG_BASE_COMBINATIONS; i++) {
        p1 = combinations[i][0];
        p2 = combinations[i][1];

        pinMode(p1, OUTPUT);    // change to OUTPUT mode
        digitalWrite(p1, LOW);  // bring LOW (default input level is HIGH)
        if (i < 32) {
            // write value to sensors1
            if (digitalRead(p2) == LOW) bitSet(detect1, i);
        } else {
            // write value to sensors2
            if (digitalRead(p2) == LOW) bitSet(detect2, i - 32);
        }
        pinMode(p1, INPUT);     // reset to INPUT mode
        digitalWrite(p1, HIGH); // enable pullup
    }

    // check to see if we need to reset detection threshold
    if (verify1 != detect1 || verify2 != detect2) {
        // current sensors different from last detection, so reset threshold
        touchTime = millis();
    } else if ((verify1 != sensors1 || verify2 != sensors2) && millis() - touchTime >= opt_touch_detect_threshold) {
        // detection is over threshold and current readings are different from previous readings

        #ifdef DEBUG_TOUCH
            DEBUG_TOUCH(Serial.print("touch "));
            for (i = 0; i < KG_BASE_COMBINATIONS; i++) {
                if (i < 32) DEBUG_TOUCH(Serial.print(bitRead(detect1, i)));
                else DEBUG_TOUCH(Serial.print(bitRead(detect2, i - 32)));
            }
            DEBUG_TOUCH(Serial.print("\n"));
        #endif /* DEBUG_TOUCH */

        // check to see if they've just initiated or removed any touches
        if (verify1 > sensors1 || verify2 > sensors2) {
            adding = true;

            // touch initiation, check for actions
            check_sensors_touch(verify1, verify2, 1);
        } else if (adding && (verify1 < sensors1 || verify2 < sensors2)) {
            adding = false;
            removing = true;

            // touch release, check for actions
            check_sensors_release(sensors1, sensors2, 1);
        }
        
        // set official sensor readings to current readings
        sensors1 = verify1;
        sensors2 = verify2;

        #if (KG_HOSTIF > 0)
            txPacketLength = create_packet(txPacket, packetFormatBinary, KG_PACKET_TOUCH_STATUS);
            send_keyglove_packet(txPacket, txPacketLength, true);
        #endif
    }
    
    verify1 = detect1;
    verify2 = detect2;
    
    /*touchBench += (micros() - touchBench0);
    touchTick++;
    if (touchTick % 100 == 0) {
        // starting point: 1596 us per cycle
        touchTick = 0;
        Serial.println(millis() - touchBench);
        Serial.println(touchBench / 100);
        Serial.println("100 touch");
        touchBench = 0;
    }*/
}

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
    DEBUG_TOUCHSET(Serial.print("touchset setmode "));
    DEBUG_TOUCHSET(Serial.println(mode));
    for (; modeStackPos > 0; modeStackPos--) deactivate_mode(modeStack[modeStackPos - 1]);
    modeStackPos = 1;
    modeStack[0] = mode;
    activate_mode(mode);
}

void pushmode(uint8_t mode) {
    DEBUG_TOUCHSET(Serial.print("touchset pushmode "));
    DEBUG_TOUCHSET(Serial.println(mode));
    if (modeStackPos < 10) {
        modeStack[modeStackPos] = mode;
        modeStackPos++;
        activate_mode(mode);
    }
}

void popmode() {
    DEBUG_TOUCHSET(Serial.println("touchset popmode"));
    if (modeStackPos > 0) deactivate_mode(modeStack[--modeStackPos]);
    if (modeStackPos == 0) modeStack[modeStackPos++] = 0;
    activate_mode(modeStack[modeStackPos - 1]);
}

void togglemode(uint8_t mode) {
    DEBUG_TOUCHSET(Serial.print("touchset togglemode "));
    DEBUG_TOUCHSET(Serial.println(mode));

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
    DEBUG_TOUCHSET(Serial.print("touchset mouseon "));
    DEBUG_TOUCHSET(Serial.println(mode));
    activeMouse = true;
    if (mode == MOUSE_ACTION_MOVE) {
        opt_mouse_mode = MOUSE_MODE_TILT_POSITION;
    } else if (mode == MOUSE_ACTION_SCROLL) {
        opt_scroll_mode = SCROLL_MODE_TILT_POSITION;
    }
    #ifdef ENABLE_ACCELEROMETER
        if (!activeAccelerometer) enable_motion_accelerometer();
    #endif
    #ifdef ENABLE_GYROSCOPE
        if (!activeGyroscope) enable_motion_gyroscope();
    #endif
    #ifdef ENABLE_ACCELGYRO
        if (!activeAccelGyro) enable_motion_accelgyro();
    #endif
}

void mouseoff(uint8_t mode) {
    DEBUG_TOUCHSET(Serial.print("touchset mouseoff "));
    DEBUG_TOUCHSET(Serial.println(mode));
    activeMouse = false;
    if (mode == MOUSE_ACTION_MOVE) {
        opt_mouse_mode = 0;
    } else if (mode == MOUSE_ACTION_SCROLL) {
        opt_scroll_mode = 0;
    }
    #ifdef ENABLE_ACCELEROMETER
        if (activeAccelerometer) disable_motion_accelerometer();
    #endif
    #ifdef ENABLE_GYROSCOPE
        if (activeGyroscope) disable_motion_gyroscope();
    #endif
    #ifdef ENABLE_ACCELGYRO
        if (activeAccelGyro) disable_motion_accelgyro();
    #endif
}

void mousedown(uint8_t button) {
    DEBUG_TOUCHSET(Serial.print("touchset mousedown "));
    DEBUG_TOUCHSET(Serial.println(button));
    mouseDown = mouseDown | button;
    #if KG_HOSTIF & KG_HOSTIF_USB_HID
        Mouse.set_buttons((mouseDown & 1) > 0 ? 1 : 0, (mouseDown & 2) > 0 ? 1 : 0, (mouseDown & 4) > 0 ? 1 : 0);
    #endif /* ENABLE_USB */
    #if KG_HOSTIF & KG_HOSTIF_R400_HID
        RX400.set_buttons((mouseDown & 1) > 0 ? 1 : 0, (mouseDown & 2) > 0 ? 1 : 0, (mouseDown & 4) > 0 ? 1 : 0);
    #endif /* ENABLE_RX400 */
}

void mouseup(uint8_t button) {
    DEBUG_TOUCHSET(Serial.print("touchset mouseup "));
    DEBUG_TOUCHSET(Serial.println(button));
    if ((mouseDown & button) > 0) {
        mouseDown -= button;
        #if KG_HOSTIF & KG_HOSTIF_USB_HID
            Mouse.set_buttons((mouseDown & 1) > 0 ? 1 : 0, (mouseDown & 2) > 0 ? 1 : 0, (mouseDown & 4) > 0 ? 1 : 0);
        #endif /* ENABLE_USB */
        #if KG_HOSTIF & KG_HOSTIF_R400_HID
            RX400.set_buttons((mouseDown & 1) > 0 ? 1 : 0, (mouseDown & 2) > 0 ? 1 : 0, (mouseDown & 4) > 0 ? 1 : 0);
        #endif /* ENABLE_RX400 */
    }
}

void mouseclick(uint8_t button) {
    DEBUG_TOUCHSET(Serial.print("touchset mouseclick "));
    DEBUG_TOUCHSET(Serial.println(button));
    mousedown(button);
    delay(5);
    mouseup(button);
}

void keydown(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset keydown "));
    DEBUG_TOUCHSET(Serial.println(code));
    uint8_t usePos = 0;
    for (usePos = 0; usePos < 6 && keysDown[usePos] != 0; usePos++);
    if (usePos == 6) return; // out of HID keyboard buffer space, REALLY weird for the Keyglove!
    keysDown[usePos] = code;
    switch (usePos) {
        case 0:
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_key1(code);
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                bluetooth.set_key1(code);
            #endif /* ENABLE_BLUETOOTH */
            #if KG_HOSTIF & KG_HOSTIF_R400_HID
                RX400.set_key1(code);
            #endif /* ENABLE_RX400 */
            break;
        case 1:
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_key2(code);
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                bluetooth.set_key2(code);
            #endif /* ENABLE_BLUETOOTH */
            #if KG_HOSTIF & KG_HOSTIF_R400_HID
                RX400.set_key2(code);
            #endif /* ENABLE_RX400 */
            break;
        case 2:
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_key3(code);
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                bluetooth.set_key3(code);
            #endif /* ENABLE_BLUETOOTH */
            #if KG_HOSTIF & KG_HOSTIF_R400_HID
                RX400.set_key3(code);
            #endif /* ENABLE_RX400 */
            break;
        case 3:
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_key4(code);
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                bluetooth.set_key4(code);
            #endif /* ENABLE_BLUETOOTH */
            #if KG_HOSTIF & KG_HOSTIF_R400_HID
                RX400.set_key4(code);
            #endif /* ENABLE_RX400 */
            break;
        case 4:
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_key5(code);
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                bluetooth.set_key5(code);
            #endif /* ENABLE_BLUETOOTH */
            #if KG_HOSTIF & KG_HOSTIF_R400_HID
                RX400.set_key5(code);
            #endif /* ENABLE_RX400 */
            break;
        case 5:
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_key6(code);
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                bluetooth.set_key6(code);
            #endif /* ENABLE_BLUETOOTH */
            #if KG_HOSTIF & KG_HOSTIF_R400_HID
                RX400.set_key6(code);
            #endif /* ENABLE_RX400 */
            break;
    }
    #if KG_HOSTIF & KG_HOSTIF_USB_HID
        Keyboard.send_now();
    #endif /* ENABLE_USB */
    #if KG_HOSTIF & KG_HOSTIF_BT2_HID
        bluetooth.send_now();
    #endif /* ENABLE_BLUETOOTH */
    #if KG_HOSTIF & KG_HOSTIF_R400_HID
        RX400.send_now();
    #endif /* ENABLE_RX400 */
}

void keyup(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset keyup "));
    DEBUG_TOUCHSET(Serial.println(code));
    uint8_t usePos = 0;
    for (usePos = 0; usePos < 6 && keysDown[usePos] != code; usePos++);
    if (usePos == 6) return; // key not currently down...oops.
    keysDown[usePos] = 0;
    switch (usePos) {
        case 0:
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_key1(0);
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                bluetooth.set_key1(0);
            #endif /* ENABLE_BLUETOOTH */
            #if KG_HOSTIF & KG_HOSTIF_R400_HID
                RX400.set_key1(0);
            #endif /* ENABLE_RX400 */
            break;
        case 1:
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_key2(0);
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                bluetooth.set_key2(0);
            #endif /* ENABLE_BLUETOOTH */
            #if KG_HOSTIF & KG_HOSTIF_R400_HID
                RX400.set_key2(0);
            #endif /* ENABLE_RX400 */
            break;
        case 2:
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_key3(0);
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                bluetooth.set_key3(0);
            #endif /* ENABLE_BLUETOOTH */
            #if KG_HOSTIF & KG_HOSTIF_R400_HID
                RX400.set_key3(0);
            #endif /* ENABLE_RX400 */
            break;
        case 3:
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_key4(0);
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                bluetooth.set_key4(0);
            #endif /* ENABLE_BLUETOOTH */
            #if KG_HOSTIF & KG_HOSTIF_R400_HID
                RX400.set_key4(0);
            #endif /* ENABLE_RX400 */
            break;
        case 4:
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_key5(0);
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                bluetooth.set_key5(0);
            #endif /* ENABLE_BLUETOOTH */
            #if KG_HOSTIF & KG_HOSTIF_R400_HID
                RX400.set_key5(0);
            #endif /* ENABLE_RX400 */
            break;
        case 5:
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_key6(0);
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                bluetooth.set_key6(0);
            #endif /* ENABLE_BLUETOOTH */
            #if KG_HOSTIF & KG_HOSTIF_R400_HID
                RX400.set_key6(0);
            #endif /* ENABLE_RX400 */
            break;
    }
    #if KG_HOSTIF & KG_HOSTIF_USB_HID
        Keyboard.send_now();
    #endif /* ENABLE_USB */
    #if KG_HOSTIF & KG_HOSTIF_BT2_HID
        bluetooth.send_now();
    #endif /* ENABLE_BLUETOOTH */
    #if KG_HOSTIF & KG_HOSTIF_R400_HID
        RX400.send_now();
    #endif /* ENABLE_RX400 */
}

void keypress(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset keypress "));
    DEBUG_TOUCHSET(Serial.println(code));
    keydown(code);
    delay(5);
    keyup(code);
}

void modifierdown(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset modifierdown "));
    DEBUG_TOUCHSET(Serial.println(code));
    modifiersDown = modifiersDown | code;
    #if KG_HOSTIF & KG_HOSTIF_USB_HID
        Keyboard.set_modifier(modifiersDown);
        Keyboard.send_now();
    #endif /* ENABLE_USB */
    #if KG_HOSTIF & KG_HOSTIF_R400_HID
        RX400.set_modifier(modifiersDown);
        RX400.send_now();
    #endif /* ENABLE_RX400 */
}

void modifierup(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset modifierup "));
    DEBUG_TOUCHSET(Serial.println(code));
    if ((modifiersDown & code) > 0) {
        modifiersDown -= code;
        #if KG_HOSTIF & KG_HOSTIF_USB_HID
            Keyboard.set_modifier(modifiersDown);
            Keyboard.send_now();
        #endif /* ENABLE_USB */
        #if KG_HOSTIF & KG_HOSTIF_R400_HID
            RX400.set_modifier(modifiersDown);
            RX400.send_now();
        #endif /* ENABLE_RX400 */
    }
}

void togglemodifier(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset modifiertoggle "));
    DEBUG_TOUCHSET(Serial.println(code));
    if ((modifiersDown & code) > 0) {
        modifierup(code);
    } else {
        modifierdown(code);
    }
}

void modifierpress(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset modifierpress "));
    DEBUG_TOUCHSET(Serial.println(code));
    modifierdown(code);
    delay(5);
    modifierup(code);
}

void redled(uint8_t mode, uint16_t duration) {
    DEBUG_TOUCHSET(Serial.print("touchset redled "));
    DEBUG_TOUCHSET(Serial.print(mode));
    DEBUG_TOUCHSET(Serial.print(" "));
    DEBUG_TOUCHSET(Serial.println(duration));
    #if (KG_FEEDBACK & KG_FEEDBACK_RGB) > 0
        if (mode <= 5) set_rgb_mode(mode, -1, -1);
    #endif /* KG_FEEDBACK_RGB */
}

void greenled(uint8_t mode, uint16_t duration) {
    DEBUG_TOUCHSET(Serial.print("touchset greenled "));
    DEBUG_TOUCHSET(Serial.print(mode));
    DEBUG_TOUCHSET(Serial.print(" "));
    DEBUG_TOUCHSET(Serial.println(duration));
    #if (KG_FEEDBACK & KG_FEEDBACK_RGB) > 0
        if (mode <= 5) set_rgb_mode(-1, mode, -1);
    #endif /* KG_FEEDBACK_RGB */
}

void blueled(uint8_t mode, uint16_t duration) {
    DEBUG_TOUCHSET(Serial.print("touchset blueled "));
    DEBUG_TOUCHSET(Serial.print(mode));
    DEBUG_TOUCHSET(Serial.print(" "));
    DEBUG_TOUCHSET(Serial.println(duration));
    #if (KG_FEEDBACK & KG_FEEDBACK_RGB) > 0
        if (mode <= 5) set_rgb_mode(-1, -1, mode);
    #endif /* KG_FEEDBACK_RGB */
}

void beep(uint16_t pitch, uint8_t mode, uint16_t duration) {
    DEBUG_TOUCHSET(Serial.print("touchset beep "));
    DEBUG_TOUCHSET(Serial.print(pitch));
    DEBUG_TOUCHSET(Serial.print(" "));
    DEBUG_TOUCHSET(Serial.print(mode));
    DEBUG_TOUCHSET(Serial.print(" "));
    DEBUG_TOUCHSET(Serial.println(duration));
    #if (KG_FEEDBACK & KG_FEEDBACK_PIEZO) > 0
        set_piezo_mode(mode, duration, pitch);
    #endif /* KG_FEEDBACK_PIEZO */
}

void vibrate(uint8_t mode, uint16_t duration) {
    DEBUG_TOUCHSET(Serial.print("touchset vibrate "));
    DEBUG_TOUCHSET(Serial.print(mode));
    DEBUG_TOUCHSET(Serial.print(" "));
    DEBUG_TOUCHSET(Serial.println(duration));
    #if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE) > 0
        set_vibrate_mode(mode, duration);
    #endif /* KG_FEEDBACK_VIBRATE */
}

#endif // _SUPPORT_TOUCH_H_