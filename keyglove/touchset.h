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
            beep(2000, KG_PIEZO_SHORTBEEP, 1);
            redled(KG_RGB_OFF, 0);
            greenled(KG_RGB_SOLID, 0);
            blueled(KG_RGB_OFF, 0);
            return;
            break;
        case KMODE_NUMBERS:
            beep(2000, KG_PIEZO_SHORTBEEP, 2);
            redled(KG_RGB_OFF, 0);
            greenled(KG_RGB_OFF, 0);
            blueled(KG_RGB_SOLID, 0);
            return;
            break;
        case KMODE_SYMBOLS:
            beep(2000, KG_PIEZO_SHORTBEEP, 3);
            redled(KG_RGB_OFF, 0);
            greenled(KG_RGB_SOLID, 0);
            blueled(KG_RGB_SOLID, 0);
            return;
            break;
        case KMODE_FUNCTIONS:
            beep(2000, KG_PIEZO_SHORTBEEP, 4);
            redled(KG_RGB_SOLID, 0);
            greenled(KG_RGB_OFF, 0);
            blueled(KG_RGB_OFF, 0);
            return;
            break;
        case KMODE_GUI:
            beep(1600, KG_PIEZO_SHORTPULSE, 1);
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
 
// numeric sensor definitions
 
#define KS1_Y7 0x2
#define KS2_Y7 0x0
 
#define KS1_GJY 0x2008
#define KS2_GJY 0x0
 
#define KS1_AJY 0x8
#define KS2_AJY 0x80
 
#define KS1_JY 0x8
#define KS2_JY 0x0
 
#define KS1_KY 0x10
#define KS2_KY 0x0
 
#define KS1_LY 0x20
#define KS2_LY 0x0
 
#define KS1_HY 0x40
#define KS2_HY 0x0
 
#define KS1_VY 0x80
#define KS2_VY 0x0
 
#define KS1_WY 0x100
#define KS2_WY 0x0
 
#define KS1_XY 0x200
#define KS2_XY 0x0
 
#define KS1_Y6 0x800
#define KS2_Y6 0x0
 
#define KS1_DGY 0x402000
#define KS2_DGY 0x0
 
#define KS1_GY 0x2000
#define KS2_GY 0x0
 
#define KS1_IY 0x4000
#define KS2_IY 0x0
 
#define KS1_SY 0x8000
#define KS2_SY 0x0
 
#define KS1_TY 0x10000
#define KS2_TY 0x0
 
#define KS1_UY 0x20000
#define KS2_UY 0x0
 
#define KS1_Y5 0x100000
#define KS2_Y5 0x0
 
#define KS1_D8 0x200000
#define KS2_D8 0x0
 
#define KS1_ADY 0x400000
#define KS2_ADY 0x80
 
#define KS1_DY 0x400000
#define KS2_DY 0x0
 
#define KS1_EY 0x800000
#define KS2_EY 0x0
 
#define KS1_FY 0x1000000
#define KS2_FY 0x0
 
#define KS1_PY 0x4000000
#define KS2_PY 0x0
 
#define KS1_QY 0x10000000
#define KS2_QY 0x0
 
#define KS1_RY 0x40000000
#define KS2_RY 0x0
 
#define KS1_Y4 0x0
#define KS2_Y4 0x10
 
#define KS1_A8 0x0
#define KS2_A8 0x20
 
#define KS1_MY 0x0
#define KS2_MY 0x40
 
#define KS1_AY 0x0
#define KS2_AY 0x80
 
#define KS1_BY 0x0
#define KS2_BY 0x100
 
#define KS1_CY 0x0
#define KS2_CY 0x200
 
#define KS1_NY 0x0
#define KS2_NY 0x1000
 
#define KS1_OY 0x0
#define KS2_OY 0x4000
 
#define KS1_A3 0x0
#define KS2_A3 0x8000
 
#define KS1_A2 0x0
#define KS2_A2 0x10000
 
#define KS1_A1 0x0
#define KS2_A1 0x20000
 
#define KS1_D3 0x0
#define KS2_D3 0x40000
 
#define KS1_D2 0x0
#define KS2_D2 0x80000
 
#define KS1_D1 0x0
#define KS2_D1 0x100000
 
#define KS1_J3 0x0
#define KS2_J3 0x1000000
 
#define KS1_J2 0x0
#define KS2_J2 0x2000000
 
#define KS1_J1 0x0
#define KS2_J1 0x4000000
 
void check_sensors_touch(uint32_t sensors1, uint32_t sensors2, uint8_t pos) {
    if ((sensors1 & KS1_D8) == KS1_D8 && (sensors2 & KS2_D8) == KS2_D8 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        vibrate(KG_VIBRATE_SHORTPULSE, 10);
        mouseon(MOUSE_ACTION_SCROLL);
        return;
    } else if ((sensors1 & KS1_A8) == KS1_A8 && (sensors2 & KS2_A8) == KS2_A8 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        vibrate(KG_VIBRATE_SHORTPULSE, 10);
        mouseon(MOUSE_ACTION_MOVE);
        return;
    } else if ((sensors1 & KS1_J3) == KS1_J3 && (sensors2 & KS2_J3) == KS2_J3 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        vibrate(KG_VIBRATE_SHORTPULSE, 10);
        mousedown(MOUSE_RIGHT);
        return;
    } else if ((sensors1 & KS1_J2) == KS1_J2 && (sensors2 & KS2_J2) == KS2_J2 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        vibrate(KG_VIBRATE_SHORTPULSE, 10);
        mousedown(MOUSE_MIDDLE);
        return;
    } else if ((sensors1 & KS1_J1) == KS1_J1 && (sensors2 & KS2_J1) == KS2_J1 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        vibrate(KG_VIBRATE_SHORTPULSE, 10);
        mousedown(MOUSE_LEFT);
        return;
    }
 
    /* Fall throughs */
    if (modeCheck(KMODE_GUI, pos) && modeCheck(KMODE_DEFAULT, pos + 1)) {
        check_sensors_touch(sensors1, sensors2, pos + 1);
        return;
    }
}

void check_sensors_release(uint32_t sensors1, uint32_t sensors2, uint8_t pos) {
    if ((sensors1 & KS1_Y7) == KS1_Y7 && (sensors2 & KS2_Y7) == KS2_Y7) {
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
    } else if ((sensors1 & KS1_JY) == KS1_JY && (sensors2 & KS2_JY) == KS2_JY) {
        if ((sensors1 & KS1_GJY) == KS1_GJY && (sensors2 & KS2_GJY) == KS2_GJY) {
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
        } else if ((sensors1 & KS1_AJY) == KS1_AJY && (sensors2 & KS2_AJY) == KS2_AJY && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
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
    } else if ((sensors1 & KS1_KY) == KS1_KY && (sensors2 & KS2_KY) == KS2_KY) {
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
    } else if ((sensors1 & KS1_LY) == KS1_LY && (sensors2 & KS2_LY) == KS2_LY) {
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
    } else if ((sensors1 & KS1_HY) == KS1_HY && (sensors2 & KS2_HY) == KS2_HY) {
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
    } else if ((sensors1 & KS1_VY) == KS1_VY && (sensors2 & KS2_VY) == KS2_VY) {
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
    } else if ((sensors1 & KS1_WY) == KS1_WY && (sensors2 & KS2_WY) == KS2_WY) {
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
    } else if ((sensors1 & KS1_XY) == KS1_XY && (sensors2 & KS2_XY) == KS2_XY) {
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
    } else if ((sensors1 & KS1_Y6) == KS1_Y6 && (sensors2 & KS2_Y6) == KS2_Y6) {
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
    } else if ((sensors1 & KS1_GY) == KS1_GY && (sensors2 & KS2_GY) == KS2_GY) {
        if ((sensors1 & KS1_DGY) == KS1_DGY && (sensors2 & KS2_DGY) == KS2_DGY) {
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
    } else if ((sensors1 & KS1_IY) == KS1_IY && (sensors2 & KS2_IY) == KS2_IY) {
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
    } else if ((sensors1 & KS1_SY) == KS1_SY && (sensors2 & KS2_SY) == KS2_SY) {
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
    } else if ((sensors1 & KS1_TY) == KS1_TY && (sensors2 & KS2_TY) == KS2_TY) {
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
    } else if ((sensors1 & KS1_UY) == KS1_UY && (sensors2 & KS2_UY) == KS2_UY) {
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
    } else if ((sensors1 & KS1_Y5) == KS1_Y5 && (sensors2 & KS2_Y5) == KS2_Y5 && modeCheck(KMODE_DEFAULT, pos)) {
        keypress(KEY_V);
        modifierup(MODIFIERKEY_SHIFT);
        modifierup(MODIFIERKEY_CTRL);
        modifierup(MODIFIERKEY_ALT);
        return;
    } else if ((sensors1 & KS1_D8) == KS1_D8 && (sensors2 & KS2_D8) == KS2_D8 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        mouseoff(MOUSE_ACTION_SCROLL);
        return;
    } else if ((sensors1 & KS1_DY) == KS1_DY && (sensors2 & KS2_DY) == KS2_DY) {
        if ((sensors1 & KS1_ADY) == KS1_ADY && (sensors2 & KS2_ADY) == KS2_ADY) {
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
    } else if ((sensors1 & KS1_EY) == KS1_EY && (sensors2 & KS2_EY) == KS2_EY) {
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
    } else if ((sensors1 & KS1_FY) == KS1_FY && (sensors2 & KS2_FY) == KS2_FY) {
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
    } else if ((sensors1 & KS1_PY) == KS1_PY && (sensors2 & KS2_PY) == KS2_PY) {
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
    } else if ((sensors1 & KS1_QY) == KS1_QY && (sensors2 & KS2_QY) == KS2_QY) {
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
    } else if ((sensors1 & KS1_RY) == KS1_RY && (sensors2 & KS2_RY) == KS2_RY) {
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
    } else if ((sensors1 & KS1_Y4) == KS1_Y4 && (sensors2 & KS2_Y4) == KS2_Y4 && modeCheck(KMODE_DEFAULT, pos)) {
        keypress(KEY_J);
        modifierup(MODIFIERKEY_SHIFT);
        modifierup(MODIFIERKEY_CTRL);
        modifierup(MODIFIERKEY_ALT);
        return;
    } else if ((sensors1 & KS1_A8) == KS1_A8 && (sensors2 & KS2_A8) == KS2_A8 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        mouseoff(MOUSE_ACTION_MOVE);
        return;
    } else if ((sensors1 & KS1_MY) == KS1_MY && (sensors2 & KS2_MY) == KS2_MY) {
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
    } else if ((sensors1 & KS1_AY) == KS1_AY && (sensors2 & KS2_AY) == KS2_AY) {
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
    } else if ((sensors1 & KS1_BY) == KS1_BY && (sensors2 & KS2_BY) == KS2_BY) {
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
    } else if ((sensors1 & KS1_CY) == KS1_CY && (sensors2 & KS2_CY) == KS2_CY) {
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
    } else if ((sensors1 & KS1_NY) == KS1_NY && (sensors2 & KS2_NY) == KS2_NY) {
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
    } else if ((sensors1 & KS1_OY) == KS1_OY && (sensors2 & KS2_OY) == KS2_OY) {
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
    } else if ((sensors1 & KS1_A3) == KS1_A3 && (sensors2 & KS2_A3) == KS2_A3) {
        if (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos)) {
            setmode(KMODE_FUNCTIONS);
            return;
         } else if (modeCheck(KMODE_FUNCTIONS, pos)) {
            setmode(KMODE_DEFAULT);
            return;
         }
    } else if ((sensors1 & KS1_A2) == KS1_A2 && (sensors2 & KS2_A2) == KS2_A2) {
        if (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_FUNCTIONS, pos)) {
            setmode(KMODE_SYMBOLS);
            return;
         } else if (modeCheck(KMODE_SYMBOLS, pos)) {
            setmode(KMODE_DEFAULT);
            return;
         }
    } else if ((sensors1 & KS1_A1) == KS1_A1 && (sensors2 & KS2_A1) == KS2_A1) {
        if (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos)) {
            setmode(KMODE_NUMBERS);
            return;
         } else if (modeCheck(KMODE_NUMBERS, pos)) {
            setmode(KMODE_DEFAULT);
            return;
         }
    } else if ((sensors1 & KS1_D3) == KS1_D3 && (sensors2 & KS2_D3) == KS2_D3 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        // UNKNOWN: array (   'action' => 'togglemodifier',   'code' => 'MODIFIERKEY_ALT', )
        return;
    } else if ((sensors1 & KS1_D2) == KS1_D2 && (sensors2 & KS2_D2) == KS2_D2 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        // UNKNOWN: array (   'action' => 'togglemodifier',   'code' => 'MODIFIERKEY_CTRL', )
        return;
    } else if ((sensors1 & KS1_D1) == KS1_D1 && (sensors2 & KS2_D1) == KS2_D1 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        // UNKNOWN: array (   'action' => 'togglemodifier',   'code' => 'MODIFIERKEY_SHIFT', )
        return;
    } else if ((sensors1 & KS1_J3) == KS1_J3 && (sensors2 & KS2_J3) == KS2_J3 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        mouseup(MOUSE_RIGHT);
        return;
    } else if ((sensors1 & KS1_J2) == KS1_J2 && (sensors2 & KS2_J2) == KS2_J2 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        mouseup(MOUSE_MIDDLE);
        return;
    } else if ((sensors1 & KS1_J1) == KS1_J1 && (sensors2 & KS2_J1) == KS2_J1 && (modeCheck(KMODE_DEFAULT, pos) || modeCheck(KMODE_NUMBERS, pos) || modeCheck(KMODE_SYMBOLS, pos) || modeCheck(KMODE_FUNCTIONS, pos))) {
        mouseup(MOUSE_LEFT);
        return;
    }
 
    /* Fall throughs */
    if (modeCheck(KMODE_GUI, pos) && modeCheck(KMODE_DEFAULT, pos + 1)) {
        check_sensors_release(sensors1, sensors2, pos + 1);
        return;
    }
}
 


