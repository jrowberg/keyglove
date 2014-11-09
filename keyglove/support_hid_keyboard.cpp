// Keyglove controller source code - HID keyboard helper function implementations
// 2014-11-07 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2014 Jeff Rowberg

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

/**
 * @file support_hid_keyboard.cpp
 * @brief HID keyboard helper function implementations
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * This file provides supporting code for sending HID keyboard data to a
 * connected wired or wireless host. This is only relevant if you have enabled
 * normal HID support over USB or Bluetooth.
 *
 * Normally it is not necessary to edit this file.
 */

#include "keyglove.h"
#include "support_hid_keyboard.h"
#include "support_bluetooth2_iwrap.h"

uint8_t hidModifiersDown = 0;                   ///< Modifier keys currently down
uint8_t hidKeysDown[] = { 0, 0, 0, 0, 0, 0 };   ///< Normal keys currently down

/**
 * @brief Initialize HID keyboard behavior
 */
void setup_hid_keyboard() {
}

/*
// "update" not necessary due to inherent functionality
// anything that might go here is more logically handled with "update_touch()"
void update_hid_keyboard() {
}
*/

/**
 * @brief Send key press (down) HID report
 * @param[in] code Key code
 */
void keyboard_key_down(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset keydown "));
    DEBUG_TOUCHSET(Serial.println(code));
    #if KG_HID & KG_HID_KEYBOARD
        uint8_t usePos = 0;
        for (usePos = 0; usePos < 6 && hidKeysDown[usePos] != 0; usePos++);
        if (usePos == 6) return; // out of HID keyboard buffer space, REALLY weird for the Keyglove!
        hidKeysDown[usePos] = code;
        switch (usePos) {
            case 0:
                #if KG_HOSTIF & KG_HOSTIF_USB_HID
                    Keyboard.set_key1(code);
                #endif /* ENABLE_USB */
                #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                    BTKeyboard.set_key1(code);
                #endif /* ENABLE_BLUETOOTH */
                break;
            case 1:
                #if KG_HOSTIF & KG_HOSTIF_USB_HID
                    Keyboard.set_key2(code);
                #endif /* ENABLE_USB */
                #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                    BTKeyboard.set_key2(code);
                #endif /* ENABLE_BLUETOOTH */
                break;
            case 2:
                #if KG_HOSTIF & KG_HOSTIF_USB_HID
                    Keyboard.set_key3(code);
                #endif /* ENABLE_USB */
                #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                    BTKeyboard.set_key3(code);
                #endif /* ENABLE_BLUETOOTH */
                break;
            case 3:
                #if KG_HOSTIF & KG_HOSTIF_USB_HID
                    Keyboard.set_key4(code);
                #endif /* ENABLE_USB */
                #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                    BTKeyboard.set_key4(code);
                #endif /* ENABLE_BLUETOOTH */
                break;
            case 4:
                #if KG_HOSTIF & KG_HOSTIF_USB_HID
                    Keyboard.set_key5(code);
                #endif /* ENABLE_USB */
                #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                    BTKeyboard.set_key5(code);
                #endif /* ENABLE_BLUETOOTH */
                break;
            case 5:
                #if KG_HOSTIF & KG_HOSTIF_USB_HID
                    Keyboard.set_key6(code);
                #endif /* ENABLE_USB */
                #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                    BTKeyboard.set_key6(code);
                #endif /* ENABLE_BLUETOOTH */
                break;
        }
        #if KG_HOSTIF & KG_HOSTIF_USB_HID
            if (interfaceUSBHIDReady) {
                Keyboard.send_now();
            }
        #endif /* ENABLE_USB */
        #if KG_HOSTIF & KG_HOSTIF_BT2_HID
            BTKeyboard.send_now();
        #endif /* ENABLE_BLUETOOTH */
    #endif
}

/**
 * @brief Send key release (up) HID report
 * @param[in] code Key code
 */
void keyboard_key_up(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset keyup "));
    DEBUG_TOUCHSET(Serial.println(code));
    #if KG_HID & KG_HID_KEYBOARD
        uint8_t usePos = 0;
        for (usePos = 0; usePos < 6 && hidKeysDown[usePos] != code; usePos++);
        if (usePos == 6) return; // key not currently down...oops.
        hidKeysDown[usePos] = 0;
        switch (usePos) {
            case 0:
                #if KG_HOSTIF & KG_HOSTIF_USB_HID
                    Keyboard.set_key1(0);
                #endif /* ENABLE_USB */
                #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                    BTKeyboard.set_key1(0);
                #endif /* ENABLE_BLUETOOTH */
                break;
            case 1:
                #if KG_HOSTIF & KG_HOSTIF_USB_HID
                    Keyboard.set_key2(0);
                #endif /* ENABLE_USB */
                #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                    BTKeyboard.set_key2(0);
                #endif /* ENABLE_BLUETOOTH */
                break;
            case 2:
                #if KG_HOSTIF & KG_HOSTIF_USB_HID
                    Keyboard.set_key3(0);
                #endif /* ENABLE_USB */
                #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                    BTKeyboard.set_key3(0);
                #endif /* ENABLE_BLUETOOTH */
                break;
            case 3:
                #if KG_HOSTIF & KG_HOSTIF_USB_HID
                    Keyboard.set_key4(0);
                #endif /* ENABLE_USB */
                #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                    BTKeyboard.set_key4(0);
                #endif /* ENABLE_BLUETOOTH */
                break;
            case 4:
                #if KG_HOSTIF & KG_HOSTIF_USB_HID
                    Keyboard.set_key5(0);
                #endif /* ENABLE_USB */
                #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                    BTKeyboard.set_key5(0);
                #endif /* ENABLE_BLUETOOTH */
                break;
            case 5:
                #if KG_HOSTIF & KG_HOSTIF_USB_HID
                    Keyboard.set_key6(0);
                #endif /* ENABLE_USB */
                #if KG_HOSTIF & KG_HOSTIF_BT2_HID
                    BTKeyboard.set_key6(0);
                #endif /* ENABLE_BLUETOOTH */
                break;
        }
        #if KG_HOSTIF & KG_HOSTIF_USB_HID
            if (interfaceUSBHIDReady) {
                Keyboard.send_now();
            }
        #endif /* ENABLE_USB */
        #if KG_HOSTIF & KG_HOSTIF_BT2_HID
            if (interfaceBT2HIDReady) {
                BTKeyboard.send_now();
            }
        #endif /* ENABLE_BLUETOOTH */
    #endif
}

/**
 * @brief Send keypress (down-up) HID reports
 * @param[in] code Modifier key bitmask
 */
void keyboard_key_press(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset keypress "));
    DEBUG_TOUCHSET(Serial.println(code));
    keyboard_key_down(code);
    delay(5);
    keyboard_key_up(code);
}

/**
 * @brief Send modifier press (down) HID report
 * @param[in] code Modifier key bitmask
 */
void keyboard_modifier_down(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset modifierdown "));
    DEBUG_TOUCHSET(Serial.println(code));
    #if KG_HID & KG_HID_KEYBOARD
        hidModifiersDown = hidModifiersDown | code;
        #if KG_HOSTIF & KG_HOSTIF_USB_HID
            Keyboard.set_modifier(hidModifiersDown);
            Keyboard.send_now();
        #endif /* ENABLE_USB */
        #if KG_HOSTIF & KG_HOSTIF_BT2_HID
            BTKeyboard.set_modifier(hidModifiersDown);
            BTKeyboard.send_now();
        #endif /* ENABLE_USB */
    #endif
}

/**
 * @brief Send modifier release (up) HID report
 * @param[in] code Modifier key bitmask
 */
void keyboard_modifier_up(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset modifierup "));
    DEBUG_TOUCHSET(Serial.println(code));
    #if KG_HID & KG_HID_KEYBOARD
        if ((hidModifiersDown & code) > 0) {
            hidModifiersDown -= code;
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                Keyboard.set_modifier(hidModifiersDown);
                Keyboard.send_now();
            #endif /* ENABLE_USB */
            #if KG_HOSTIF & KG_HOSTIF_USB_HID
                BTKeyboard.set_modifier(hidModifiersDown);
                BTKeyboard.send_now();
            #endif /* ENABLE_USB */
        }
    #endif
}

/**
 * @brief Switch modifier state to opposite of what it was (down vs. up)
 * @param[in] code Modifier key bitmask
 */
void keyboard_modifier_toggle(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset modifiertoggle "));
    DEBUG_TOUCHSET(Serial.println(code));
    if ((hidModifiersDown & code) > 0) {
        keyboard_modifier_up(code);
    } else {
        keyboard_modifier_down(code);
    }
}

/**
 * @brief Send modifier keypress (down-up) HID reports
 * @param[in] code Modifier key bitmask
 */
void keyboard_modifier_press(uint8_t code) {
    DEBUG_TOUCHSET(Serial.print("touchset modifierpress "));
    DEBUG_TOUCHSET(Serial.println(code));
    keyboard_modifier_down(code);
    delay(5);
    keyboard_modifier_up(code);
}
