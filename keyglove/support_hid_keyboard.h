// Keyglove controller source code - HID keyboard helper functions
// 7/4/2014 by Jeff Rowberg <jeff@rowberg.net>

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
 * @file support_hid_keyboard.h
 * @brief HID keyboard helper functions
 * @author Jeff Rowberg
 * @date 2014-07-04
 *
 * This file provides supporting code for sending HID keyboard data to a
 * connected wired or wireless host. This is only relevant if you have enabled
 * normal HID support over USB or Bluetooth.
 *
 * Normally it is not necessary to edit this file.
 */

#ifndef _SUPPORT_HID_KEYBOARD_H_
#define _SUPPORT_HID_KEYBOARD_H_

#ifndef MODIFIERKEY_ALT
    // these are defined by the Teensy libs, if included
    #define MODIFIERKEY_CTRL        0x04
    #define MODIFIERKEY_SHIFT       0x02
    #define MODIFIERKEY_ALT         0x01
    #define MODIFIERKEY_GUI         0x08
    
    #define KEY_A           4
    #define KEY_B           5
    #define KEY_C           6
    #define KEY_D           7
    #define KEY_E           8
    #define KEY_F           9
    #define KEY_G           10
    #define KEY_H           11
    #define KEY_I           12
    #define KEY_J           13
    #define KEY_K           14
    #define KEY_L           15
    #define KEY_M           16
    #define KEY_N           17
    #define KEY_O           18
    #define KEY_P           19
    #define KEY_Q           20
    #define KEY_R           21
    #define KEY_S           22
    #define KEY_T           23
    #define KEY_U           24
    #define KEY_V           25
    #define KEY_W           26
    #define KEY_X           27
    #define KEY_Y           28
    #define KEY_Z           29
    #define KEY_1           30
    #define KEY_2           31
    #define KEY_3           32
    #define KEY_4           33
    #define KEY_5           34
    #define KEY_6           35
    #define KEY_7           36
    #define KEY_8           37
    #define KEY_9           38
    #define KEY_0           39
    #define KEY_ENTER       40
    #define KEY_ESC         41
    #define KEY_BACKSPACE   42
    #define KEY_TAB         43
    #define KEY_SPACE       44
    #define KEY_MINUS       45
    #define KEY_EQUAL       46
    #define KEY_LEFT_BRACE  47
    #define KEY_RIGHT_BRACE 48
    #define KEY_BACKSLASH   49
    #define KEY_NON_US_NUM  50
    #define KEY_SEMICOLON   51
    #define KEY_QUOTE       52
    #define KEY_TILDE       53
    #define KEY_COMMA       54
    #define KEY_PERIOD      55
    #define KEY_SLASH       56
    #define KEY_CAPS_LOCK   57
    #define KEY_F1          58
    #define KEY_F2          59
    #define KEY_F3          60
    #define KEY_F4          61
    #define KEY_F5          62
    #define KEY_F6          63
    #define KEY_F7          64
    #define KEY_F8          65
    #define KEY_F9          66
    #define KEY_F10         67
    #define KEY_F11         68
    #define KEY_F12         69
    #define KEY_PRINTSCREEN 70
    #define KEY_SCROLL_LOCK 71
    #define KEY_PAUSE       72
    #define KEY_INSERT      73
    #define KEY_HOME        74
    #define KEY_PAGE_UP     75
    #define KEY_DELETE      76
    #define KEY_END         77
    #define KEY_PAGE_DOWN   78
    #define KEY_RIGHT       79
    #define KEY_LEFT        80
    #define KEY_DOWN        81
    #define KEY_UP          82
    #define KEY_NUM_LOCK    83
    #define KEYPAD_SLASH    84
    #define KEYPAD_ASTERIX  85
    #define KEYPAD_MINUS    86
    #define KEYPAD_PLUS     87
    #define KEYPAD_ENTER    88
    #define KEYPAD_1        89
    #define KEYPAD_2        90
    #define KEYPAD_3        91
    #define KEYPAD_4        92
    #define KEYPAD_5        93
    #define KEYPAD_6        94
    #define KEYPAD_7        95
    #define KEYPAD_8        96
    #define KEYPAD_9        97
    #define KEYPAD_0        98
    #define KEYPAD_PERIOD   99
#endif /* !MODIFYERKEY_ALT */

#ifndef KEYPAD_ASTERISK
    // these are not defined by the Teensy libs, but they should be
    #define KEYPAD_ASTERISK 85
    #define KEY_BACKSLASH2 100
    #define KEY_APP        101
    #define KEY_POWER      102
    #define KEYPAD_EQUAL   103
/*
    // added to Teensyduino v1.07
    #define KEY_F13        104
    #define KEY_F14        105
    #define KEY_F15        106
    #define KEY_F16        107
    #define KEY_F17        108
    #define KEY_F18        109
    #define KEY_F19        110
    #define KEY_F20        111
    #define KEY_F21        112
    #define KEY_F22        113
    #define KEY_F23        114
    #define KEY_F24        115
*/
    #define KEY_EXECUTE    116
    #define KEY_HELP       117
    #define KEY_MENU       118
    #define KEY_SELECT     119
    #define KEY_STOP       120
    #define KEY_AGAIN      121
    #define KEY_UNDO       122
    #define KEY_CUT        123
    #define KEY_COPY       124
    #define KEY_PASTE      125
    #define KEY_FIND       126
    #define KEY_MUTE       127
    #define KEY_VOLUP      128
    #define KEY_VOLDOWN    129
#endif /* !KEYPAD_ASTERISK */

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

#endif // _SUPPORT_HID_KEYBOARD_H_


