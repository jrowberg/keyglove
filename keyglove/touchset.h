// Keyglove controller source code - Custom touchset definition
// 4/5/2011 by Jeff Rowberg <jeff@rowberg.net>
// (Generated Fri, 17 Jun 2011 00:25:04 -0600 @ host132.hostmonster.com)

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

// TOTAL: 43 unique combinations used, 5 modes
// ===============================================================

// numeric mode definitions

#define KMODE_DEFAULT 0
#define KMODE_NUMBERS 1
#define KMODE_SYMBOLS 2
#define KMODE_FUNCTIONS 3
#define KMODE_GUI 4
 
static void activate_mode(uint8_t mode) {
    switch (mode) {
        case KMODE_DEFAULT:
            beep(2000, KG_PIEZO_TINYBEEP, 10);
            vibrate(KG_VIBRATE_TINYBUZZ, 10);
            redled(KG_RGB_OFF, 0);
            greenled(KG_RGB_SOLID, 0);
            blueled(KG_RGB_OFF, 0);
            return;
            break;
        case KMODE_NUMBERS:
            beep(2000, KG_PIEZO_TINYBEEP, 20);
            vibrate(KG_VIBRATE_TINYBUZZ, 20);
            redled(KG_RGB_OFF, 0);
            greenled(KG_RGB_OFF, 0);
            blueled(KG_RGB_SOLID, 0);
            return;
            break;
        case KMODE_SYMBOLS:
            beep(2000, KG_PIEZO_TINYBEEP, 30);
            vibrate(KG_VIBRATE_TINYBUZZ, 30);
            redled(KG_RGB_OFF, 0);
            greenled(KG_RGB_SOLID, 0);
            blueled(KG_RGB_SOLID, 0);
            return;
            break;
        case KMODE_FUNCTIONS:
            beep(2000, KG_PIEZO_TINYBEEP, 40);
            vibrate(KG_VIBRATE_TINYBUZZ, 40);
            redled(KG_RGB_SOLID, 0);
            greenled(KG_RGB_OFF, 0);
            blueled(KG_RGB_OFF, 0);
            return;
            break;
        case KMODE_GUI:
            beep(1200, KG_PIEZO_TINYBEEP, 50);
            vibrate(KG_VIBRATE_TINYBUZZ, 50);
            greenled(KG_RGB_SHORTBLINK, 0);
            modifierdown(MODIFIERKEY_GUI);
            return;
            break;
    }
}
 
static void deactivate_mode(uint8_t mode) {
    switch (mode) {
        case KMODE_GUI:
            modifierup(MODIFIERKEY_GUI);
            return;
            break;
    }
}
 
void check_sensors_touch(uint8_t *touches, uint8_t pos) {
    if (KS_D8 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        vibrate(KG_VIBRATE_SHORTPULSE, 10);
        mouseon(MOUSE_ACTION_SCROLL);
        return;
    } else if (KS_A8 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        vibrate(KG_VIBRATE_SHORTPULSE, 10);
        mouseon(MOUSE_ACTION_MOVE);
        return;
    } else if (KS_J3 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        vibrate(KG_VIBRATE_SHORTPULSE, 10);
        mousedown(MOUSE_RIGHT);
        return;
    } else if (KS_J2 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        vibrate(KG_VIBRATE_SHORTPULSE, 10);
        mousedown(MOUSE_MIDDLE);
        return;
    } else if (KS_J1 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        vibrate(KG_VIBRATE_SHORTPULSE, 10);
        mousedown(MOUSE_LEFT);
        return;
    }
 
    /* Fall throughs */
    if (modeCheck(KMODE_GUI, pos) && modeCheck(KMODE_DEFAULT, pos + 1)) {
        check_sensors_touch(touches, pos + 1);
        return;
    }
}

void check_sensors_release(uint8_t *touches, uint8_t pos) {
    if (KS_Y7) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_PERIOD);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_ESC);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_JY) {
        if (KS_GJY) {
            if (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos)) {
                keypress(KEY_ENTER);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             } else if (modeCheck(KMODE_NUMBERS, pos)) {
                keypress(KEYPAD_ENTER);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             }
        } else if (KS_AJY && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
            keypress(KEY_TAB);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
        } else {
             if (modeCheck(KMODE_DEFAULT, pos)) {
                keypress(KEY_S);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             } else if (modeCheck(KMODE_NUMBERS, pos)) {
                modifierdown(MODIFIERKEY_SHIFT);
                keypress(KEY_3);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             } else if (modeCheck(KMODE_SYMBOLS, pos)) {
                keypress(KEY_SEMICOLON);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
                keypress(KEY_F10);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             }
        }
    } else if (KS_KY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_C);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEY_0);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_SEMICOLON);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_F11);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_LY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_P);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_8);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_TILDE);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_F12);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_HY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_L);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEY_8);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_PERIOD);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_F8);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_VY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_Q);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEYPAD_PERIOD);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            keypress(KEY_TILDE);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_WY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_Y);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_MINUS);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_INSERT);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_XY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_X);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_BACKSLASH);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_PRINTSCREEN);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_Y6) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_COMMA);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_DELETE);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_GY) {
        if (KS_DGY) {
            if (modeCheck(KMODE_DEFAULT, pos)) {
                keypress(KEY_BACKSPACE);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             } else if (modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos)) {
                keypress(KEY_SPACE);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             }
        } else {
             if (modeCheck(KMODE_DEFAULT, pos)) {
                keypress(KEY_T);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             } else if (modeCheck(KMODE_NUMBERS, pos)) {
                keypress(KEY_7);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             } else if (modeCheck(KMODE_SYMBOLS, pos)) {
                modifierdown(MODIFIERKEY_SHIFT);
                keypress(KEY_COMMA);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
                keypress(KEY_F7);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             }
        }
    } else if (KS_IY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_G);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEY_9);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            keypress(KEY_SLASH);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_F9);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_SY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_W);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEYPAD_EQUAL);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            keypress(KEY_LEFT_BRACE);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_TY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_K);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_6);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            keypress(KEY_RIGHT_BRACE);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_UY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_Z);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_5);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            keypress(KEY_BACKSLASH);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_Y5 && modeCheck(KMODE_DEFAULT, pos)) {
        keypress(KEY_V);
        modifierup(MODIFIERKEY_SHIFT);
        modifierup(MODIFIERKEY_CTRL);
        modifierup(MODIFIERKEY_ALT);
        return;
    } else if (KS_D8 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        mouseoff(MOUSE_ACTION_SCROLL);
        return;
    } else if (KS_DY) {
        if (KS_ADY) {
            if (modeCheck(KMODE_DEFAULT, pos)) {
                keypress(KEY_SPACE);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             } else if (modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos)) {
                keypress(KEY_BACKSPACE);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             }
        } else {
             if (modeCheck(KMODE_DEFAULT, pos)) {
                keypress(KEY_H);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             } else if (modeCheck(KMODE_NUMBERS, pos)) {
                keypress(KEY_4);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             } else if (modeCheck(KMODE_SYMBOLS, pos)) {
                modifierdown(MODIFIERKEY_SHIFT);
                keypress(KEY_9);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
                keypress(KEY_F4);
                modifierup(MODIFIERKEY_SHIFT);
                modifierup(MODIFIERKEY_CTRL);
                modifierup(MODIFIERKEY_ALT);
                return;
             }
        }
    } else if (KS_EY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_M);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEY_5);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_0);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_F5);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_FY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_B);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEY_6);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_SLASH);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_F6);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_PY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_O);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEYPAD_ASTERIX);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_LEFT_BRACE);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_MUTE);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_QY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_U);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEYPAD_SLASH);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_RIGHT_BRACE);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_RY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_D);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_1);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_Y4 && modeCheck(KMODE_DEFAULT, pos)) {
        keypress(KEY_J);
        modifierup(MODIFIERKEY_SHIFT);
        modifierup(MODIFIERKEY_CTRL);
        modifierup(MODIFIERKEY_ALT);
        return;
    } else if (KS_A8 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        mouseoff(MOUSE_ACTION_MOVE);
        return;
    } else if (KS_MY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_A);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEYPAD_PLUS);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_QUOTE);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_AY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_R);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEY_1);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            keypress(KEY_PERIOD);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_F1);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_BY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_N);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEY_2);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            keypress(KEY_COMMA);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_F2);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_CY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_F);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEY_3);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_2);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_F3);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_NY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_E);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            keypress(KEYPAD_MINUS);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            keypress(KEY_QUOTE);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            modifierpress(MODIFIERKEY_GUI);
            return;
         }
    } else if (KS_OY) {
        if (modeCheck(KMODE_DEFAULT, pos)) {
            keypress(KEY_I);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_7);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            keypress(KEY_APP);
            modifierup(MODIFIERKEY_SHIFT);
            modifierup(MODIFIERKEY_CTRL);
            modifierup(MODIFIERKEY_ALT);
            return;
         }
    } else if (KS_A3) {
        if (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos)) {
            setmode(KMODE_FUNCTIONS);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            setmode(KMODE_DEFAULT);
            return;
         }
    } else if (KS_A2) {
        if (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_FUNCTIONS, pos)) {
            setmode(KMODE_SYMBOLS);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            setmode(KMODE_DEFAULT);
            return;
         }
    } else if (KS_A1) {
        if (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos)) {
            setmode(KMODE_NUMBERS);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            setmode(KMODE_DEFAULT);
            return;
         }
    } else if (KS_D3 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        // UNKNOWN: array (   'action' => 'togglemodifier',   'code' => 'MODIFIERKEY_ALT', )
        return;
    } else if (KS_D2 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        // UNKNOWN: array (   'action' => 'togglemodifier',   'code' => 'MODIFIERKEY_CTRL', )
        return;
    } else if (KS_D1 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        // UNKNOWN: array (   'action' => 'togglemodifier',   'code' => 'MODIFIERKEY_SHIFT', )
        return;
    } else if (KS_J3 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        mouseup(MOUSE_RIGHT);
        return;
    } else if (KS_J2 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        mouseup(MOUSE_MIDDLE);
        return;
    } else if (KS_J1 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        mouseup(MOUSE_LEFT);
        return;
    }
 
    /* Fall throughs */
    if (modeCheck(KMODE_GUI, pos) && modeCheck(KMODE_DEFAULT, pos + 1)) {
        check_sensors_release(touches, pos + 1);
        return;
    }
}
