// Keyglove controller source code - Custom touchset definition
// 4/5/2011 by Jeff Rowberg <jeff@rowberg.net>
// (Generated Thu, 05 May 2011 14:57:09 +1200 @ mikenz-eeepc)

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

// TOTAL: 50 unique combinations used, 9 modes
// ===============================================================

// numeric mode definitions

#define KMODE_DEFAULT 0
#define KMODE_NUMBERS 1
#define KMODE_SYMBOLS 2
#define KMODE_FUNCTIONS 3
#define KMODE_CAPS 4
#define KMODE_SHIFT 5
#define KMODE_CTRL 6
#define KMODE_ALT 7
#define KMODE_GUI 8

void activate_mode(int mode) {
    switch (mode) {
        case KMODE_DEFAULT:
            beep(2000, KBEEP_SHORTBEEP, 1);
            redled(KLED_OFF, 0);
            greenled(KLED_SOLID, 0);
            blueled(KLED_OFF, 0);
            break;
        case KMODE_NUMBERS:
            beep(2000, KBEEP_SHORTBEEP, 2);
            redled(KLED_OFF, 0);
            greenled(KLED_OFF, 0);
            blueled(KLED_SOLID, 0);
            break;
        case KMODE_SYMBOLS:
            beep(2000, KBEEP_SHORTBEEP, 3);
            redled(KLED_OFF, 0);
            greenled(KLED_SOLID, 0);
            blueled(KLED_SOLID, 0);
            break;
        case KMODE_FUNCTIONS:
            beep(2000, KBEEP_SHORTBEEP, 4);
            redled(KLED_SOLID, 0);
            greenled(KLED_OFF, 0);
            blueled(KLED_OFF, 0);
            break;
        case KMODE_CAPS:
            beep(1000, KBEEP_SHORTPULSE, 1);
            greenled(KLED_LONGBLINK, 0);
            keypress(KEY_CAPS_LOCK);
            break;
        case KMODE_SHIFT:
            beep(1000, KBEEP_SHORTPULSE, 1);
            greenled(KLED_SHORTBLINK, 0);
            modifierdown(MODIFIERKEY_SHIFT);
            break;
        case KMODE_CTRL:
            beep(1200, KBEEP_SHORTPULSE, 1);
            redled(KLED_SHORTBLINK, 0);
            modifierdown(MODIFIERKEY_CTRL);
            break;
        case KMODE_ALT:
            beep(1400, KBEEP_SHORTPULSE, 1);
            blueled(KLED_SHORTBLINK, 0);
            modifierdown(MODIFIERKEY_ALT);
            break;
        case KMODE_GUI:
            beep(1600, KBEEP_SHORTPULSE, 1);
            greenled(KLED_SHORTBLINK, 0);
            modifierdown(MODIFIERKEY_GUI);
            break;
    }
}

void deactivate_mode(int mode) {
    switch (mode) {
        case KMODE_CAPS:
            keypress(KEY_CAPS_LOCK);
            break;
        case KMODE_SHIFT:
            modifierup(MODIFIERKEY_SHIFT);
            break;
        case KMODE_CTRL:
            modifierup(MODIFIERKEY_CTRL);
            break;
        case KMODE_ALT:
            modifierup(MODIFIERKEY_ALT);
            break;
        case KMODE_GUI:
            modifierup(MODIFIERKEY_GUI);
            break;
    }
}

// numeric sensor definitions

#define KS1_Y7 0x2
#define KS2_Y7 0x0

#define KS1_J8 0x4
#define KS2_J8 0x0

#define KS1_GJY 0x1008
#define KS2_GJY 0x0

#define KS1_AJY 0x8
#define KS2_AJY 0x40

#define KS1_JY 0x8
#define KS2_JY 0x0

#define KS1_KY 0x10
#define KS2_KY 0x0

#define KS1_LY 0x20
#define KS2_LY 0x0

#define KS1_VY 0x40
#define KS2_VY 0x0

#define KS1_WY 0x80
#define KS2_WY 0x0

#define KS1_XY 0x100
#define KS2_XY 0x0

#define KS1_Y6 0x400
#define KS2_Y6 0x0

#define KS1_DGY 0x401000
#define KS2_DGY 0x0

#define KS1_GY 0x1000
#define KS2_GY 0x0

#define KS1_HY 0x2000
#define KS2_HY 0x0

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
#define KS2_ADY 0x40

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

#define KS1_Z4 0x0
#define KS2_Z4 0x8

#define KS1_Y4 0x0
#define KS2_Y4 0x10

#define KS1_A8 0x0
#define KS2_A8 0x20

#define KS1_AY_J1 0x0
#define KS2_AY_J1 0x4000040

#define KS1_AY 0x0
#define KS2_AY 0x40

#define KS1_BY_J1 0x0
#define KS2_BY_J1 0x4000080

#define KS1_BY 0x0
#define KS2_BY 0x80

#define KS1_CY_J1 0x0
#define KS2_CY_J1 0x4000100

#define KS1_CY 0x0
#define KS2_CY 0x100

#define KS1_J1_MY 0x0
#define KS2_J1_MY 0x4000400

#define KS1_MY 0x0
#define KS2_MY 0x400

#define KS1_J1_NY 0x0
#define KS2_J1_NY 0x4001000

#define KS1_NY 0x0
#define KS2_NY 0x1000

#define KS1_J1_OY 0x0
#define KS2_J1_OY 0x4004000

#define KS1_OY 0x0
#define KS2_OY 0x4000

#define KS1_ADG2 0x0
#define KS2_ADG2 0x490000

#define KS1_AD2 0x0
#define KS2_AD2 0x90000

#define KS1_A2 0x0
#define KS2_A2 0x10000

#define KS1_D2 0x0
#define KS2_D2 0x80000

#define KS1_G2 0x0
#define KS2_G2 0x400000

#define KS1_J3 0x0
#define KS2_J3 0x1000000

#define KS1_J2 0x0
#define KS2_J2 0x2000000

#define KS1_J1 0x0
#define KS2_J1 0x4000000

void check_sensors_touch(long unsigned int sensors1, long unsigned int sensors2) {
    if ((sensors1 & KS1_D8) == KS1_D8 && (sensors2 & KS2_D8) == KS2_D8) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseon(KMOUSE_SCROLL);
        }
    }
    else if ((sensors1 & KS1_A8) == KS1_A8 && (sensors2 & KS2_A8) == KS2_A8) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseon(KMOUSE_MOVE);
        }
    }
    else if ((sensors1 & KS1_J3) == KS1_J3 && (sensors2 & KS2_J3) == KS2_J3) {
        if (modeCheck(KMODE_DEFAULT)) {
            mousedown(KMOUSE_RIGHT);
        }
    }
    else if ((sensors1 & KS1_J2) == KS1_J2 && (sensors2 & KS2_J2) == KS2_J2) {
        if (modeCheck(KMODE_DEFAULT)) {
            mousedown(KMOUSE_MIDDLE);
        }
    }
    else if ((sensors1 & KS1_J1) == KS1_J1 && (sensors2 & KS2_J1) == KS2_J1) {
        if (modeCheck(KMODE_DEFAULT)) {
            mousedown(KMOUSE_LEFT);
        }
    }
}

void check_sensors_release(long unsigned int sensors1, long unsigned int sensors2) {
    if ((sensors1 & KS1_Y7) == KS1_Y7 && (sensors2 & KS2_Y7) == KS2_Y7) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_ESC);
        }
    }
    else if ((sensors1 & KS1_J8) == KS1_J8 && (sensors2 & KS2_J8) == KS2_J8) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_T);
            keypress(KEY_E);
            keypress(KEY_S);
            keypress(KEY_T);
            keypress(KEY_SPACE);
        }
    }
    else if ((sensors1 & KS1_JY) == KS1_JY && (sensors2 & KS2_JY) == KS2_JY) {
        if ((sensors1 & KS1_GJY) == KS1_GJY && (sensors2 & KS2_GJY) == KS2_GJY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_ENTER);
            }
        }
        else if ((sensors1 & KS1_AJY) == KS1_AJY && (sensors2 & KS2_AJY) == KS2_AJY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_TAB);
            }
        }
        else if ((sensors1 & KS1_JY) == KS1_JY && (sensors2 & KS2_JY) == KS2_JY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_O);
            }
            else if (modeCheck(KMODE_NUMBERS)) {
                modifierdown(MODIFIERKEY_SHIFT);
                keypress(KEY_3);
                modifierup(MODIFIERKEY_SHIFT);
            }
            else if (modeCheck(KMODE_SYMBOLS)) {
                keypress(KEY_COMMA);
            }
            else if (modeCheck(KMODE_FUNCTIONS)) {
                keypress(KEY_F10);
            }
        }
    }
    else if ((sensors1 & KS1_KY) == KS1_KY && (sensors2 & KS2_KY) == KS2_KY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_R);
        }
        else if (modeCheck(KMODE_NUMBERS)) {
            keypress(KEY_0);
        }
        else if (modeCheck(KMODE_SYMBOLS)) {
            keypress(KEY_PERIOD);
        }
        else if (modeCheck(KMODE_FUNCTIONS)) {
            keypress(KEY_F11);
        }
    }
    else if ((sensors1 & KS1_LY) == KS1_LY && (sensors2 & KS2_LY) == KS2_LY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_U);
        }
        else if (modeCheck(KMODE_NUMBERS)) {
            modifierdown(MODIFIERKEY_SHIFT);
            keypress(KEY_8);
            modifierup(MODIFIERKEY_SHIFT);
        }
        else if (modeCheck(KMODE_SYMBOLS)) {
            keypress(KEY_SLASH);
        }
        else if (modeCheck(KMODE_FUNCTIONS)) {
            keypress(KEY_F12);
        }
    }
    else if ((sensors1 & KS1_VY) == KS1_VY && (sensors2 & KS2_VY) == KS2_VY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_Y);
        }
    }
    else if ((sensors1 & KS1_WY) == KS1_WY && (sensors2 & KS2_WY) == KS2_WY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_B);
        }
    }
    else if ((sensors1 & KS1_XY) == KS1_XY && (sensors2 & KS2_XY) == KS2_XY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_Q);
        }
    }
    else if ((sensors1 & KS1_Y6) == KS1_Y6 && (sensors2 & KS2_Y6) == KS2_Y6) {
        if (modeCheck(KMODE_DEFAULT)) {
            modifierpress(MODIFIERKEY_GUI);
        }
    }
    else if ((sensors1 & KS1_GY) == KS1_GY && (sensors2 & KS2_GY) == KS2_GY) {
        if ((sensors1 & KS1_DGY) == KS1_DGY && (sensors2 & KS2_DGY) == KS2_DGY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_BACKSPACE);
            }
        }
        else if ((sensors1 & KS1_GY) == KS1_GY && (sensors2 & KS2_GY) == KS2_GY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_A);
            }
            else if (modeCheck(KMODE_NUMBERS)) {
                keypress(KEY_7);
            }
            else if (modeCheck(KMODE_SYMBOLS)) {
                keypress(KEY_SEMICOLON);
            }
            else if (modeCheck(KMODE_FUNCTIONS)) {
                keypress(KEY_F7);
            }
        }
    }
    else if ((sensors1 & KS1_HY) == KS1_HY && (sensors2 & KS2_HY) == KS2_HY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_S);
        }
        else if (modeCheck(KMODE_NUMBERS)) {
            keypress(KEY_8);
        }
        else if (modeCheck(KMODE_SYMBOLS)) {
            keypress(KEY_QUOTE);
        }
        else if (modeCheck(KMODE_FUNCTIONS)) {
            keypress(KEY_F8);
        }
    }
    else if ((sensors1 & KS1_IY) == KS1_IY && (sensors2 & KS2_IY) == KS2_IY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_L);
        }
        else if (modeCheck(KMODE_NUMBERS)) {
            keypress(KEY_9);
        }
        else if (modeCheck(KMODE_FUNCTIONS)) {
            keypress(KEY_F9);
        }
    }
    else if ((sensors1 & KS1_SY) == KS1_SY && (sensors2 & KS2_SY) == KS2_SY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_F);
        }
    }
    else if ((sensors1 & KS1_TY) == KS1_TY && (sensors2 & KS2_TY) == KS2_TY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_P);
        }
    }
    else if ((sensors1 & KS1_UY) == KS1_UY && (sensors2 & KS2_UY) == KS2_UY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_X);
        }
    }
    else if ((sensors1 & KS1_Y5) == KS1_Y5 && (sensors2 & KS2_Y5) == KS2_Y5) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_APP);
        }
    }
    else if ((sensors1 & KS1_D8) == KS1_D8 && (sensors2 & KS2_D8) == KS2_D8) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseoff(KMOUSE_SCROLL);
        }
    }
    else if ((sensors1 & KS1_DY) == KS1_DY && (sensors2 & KS2_DY) == KS2_DY) {
        if ((sensors1 & KS1_ADY) == KS1_ADY && (sensors2 & KS2_ADY) == KS2_ADY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_SPACE);
            }
        }
        else if ((sensors1 & KS1_DY) == KS1_DY && (sensors2 & KS2_DY) == KS2_DY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_T);
            }
            else if (modeCheck(KMODE_NUMBERS)) {
                keypress(KEY_4);
            }
            else if (modeCheck(KMODE_SYMBOLS)) {
                keypress(KEY_LEFT_BRACE);
            }
            else if (modeCheck(KMODE_FUNCTIONS)) {
                keypress(KEY_F4);
            }
        }
    }
    else if ((sensors1 & KS1_EY) == KS1_EY && (sensors2 & KS2_EY) == KS2_EY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_N);
        }
        else if (modeCheck(KMODE_NUMBERS)) {
            keypress(KEY_5);
        }
        else if (modeCheck(KMODE_SYMBOLS)) {
            keypress(KEY_RIGHT_BRACE);
        }
        else if (modeCheck(KMODE_FUNCTIONS)) {
            keypress(KEY_F5);
        }
    }
    else if ((sensors1 & KS1_FY) == KS1_FY && (sensors2 & KS2_FY) == KS2_FY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_D);
        }
        else if (modeCheck(KMODE_NUMBERS)) {
            keypress(KEY_6);
        }
        else if (modeCheck(KMODE_SYMBOLS)) {
            keypress(KEY_BACKSLASH);
        }
        else if (modeCheck(KMODE_FUNCTIONS)) {
            keypress(KEY_F6);
        }
    }
    else if ((sensors1 & KS1_PY) == KS1_PY && (sensors2 & KS2_PY) == KS2_PY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_M);
        }
    }
    else if ((sensors1 & KS1_QY) == KS1_QY && (sensors2 & KS2_QY) == KS2_QY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_G);
        }
    }
    else if ((sensors1 & KS1_RY) == KS1_RY && (sensors2 & KS2_RY) == KS2_RY) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_K);
        }
    }
    else if ((sensors1 & KS1_Z4) == KS1_Z4 && (sensors2 & KS2_Z4) == KS2_Z4) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_Z);
        }
    }
    else if ((sensors1 & KS1_Y4) == KS1_Y4 && (sensors2 & KS2_Y4) == KS2_Y4) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_J);
        }
    }
    else if ((sensors1 & KS1_A8) == KS1_A8 && (sensors2 & KS2_A8) == KS2_A8) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseoff(KMOUSE_MOVE);
        }
    }
    else if ((sensors1 & KS1_AY) == KS1_AY && (sensors2 & KS2_AY) == KS2_AY) {
        if ((sensors1 & KS1_AY_J1) == KS1_AY_J1 && (sensors2 & KS2_AY_J1) == KS2_AY_J1) {
            if (modeCheck(KMODE_DEFAULT)) {
                setmode(KMODE_NUMBERS);
            }
            else if (modeCheck(KMODE_NUMBERS)) {
                setmode(KMODE_DEFAULT);
            }
            else if (modeCheck(KMODE_SYMBOLS)) {
                setmode(KMODE_NUMBERS);
            }
            else if (modeCheck(KMODE_FUNCTIONS)) {
                setmode(KMODE_NUMBERS);
            }
        }
        else if ((sensors1 & KS1_AY) == KS1_AY && (sensors2 & KS2_AY) == KS2_AY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_E);
            }
            else if (modeCheck(KMODE_NUMBERS)) {
                keypress(KEY_1);
            }
            else if (modeCheck(KMODE_SYMBOLS)) {
                keypress(KEY_MINUS);
            }
            else if (modeCheck(KMODE_FUNCTIONS)) {
                keypress(KEY_F1);
            }
        }
    }
    else if ((sensors1 & KS1_BY) == KS1_BY && (sensors2 & KS2_BY) == KS2_BY) {
        if ((sensors1 & KS1_BY_J1) == KS1_BY_J1 && (sensors2 & KS2_BY_J1) == KS2_BY_J1) {
            if (modeCheck(KMODE_DEFAULT)) {
                setmode(KMODE_SYMBOLS);
            }
            else if (modeCheck(KMODE_NUMBERS)) {
                setmode(KMODE_SYMBOLS);
            }
            else if (modeCheck(KMODE_SYMBOLS)) {
                setmode(KMODE_DEFAULT);
            }
            else if (modeCheck(KMODE_FUNCTIONS)) {
                setmode(KMODE_SYMBOLS);
            }
        }
        else if ((sensors1 & KS1_BY) == KS1_BY && (sensors2 & KS2_BY) == KS2_BY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_I);
            }
            else if (modeCheck(KMODE_NUMBERS)) {
                keypress(KEY_2);
            }
            else if (modeCheck(KMODE_SYMBOLS)) {
                keypress(KEY_EQUAL);
            }
            else if (modeCheck(KMODE_FUNCTIONS)) {
                keypress(KEY_F2);
            }
        }
    }
    else if ((sensors1 & KS1_CY) == KS1_CY && (sensors2 & KS2_CY) == KS2_CY) {
        if ((sensors1 & KS1_CY_J1) == KS1_CY_J1 && (sensors2 & KS2_CY_J1) == KS2_CY_J1) {
            if (modeCheck(KMODE_DEFAULT)) {
                setmode(KMODE_FUNCTIONS);
            }
            else if (modeCheck(KMODE_NUMBERS)) {
                setmode(KMODE_FUNCTIONS);
            }
            else if (modeCheck(KMODE_SYMBOLS)) {
                setmode(KMODE_FUNCTIONS);
            }
            else if (modeCheck(KMODE_FUNCTIONS)) {
                setmode(KMODE_DEFAULT);
            }
        }
        else if ((sensors1 & KS1_CY) == KS1_CY && (sensors2 & KS2_CY) == KS2_CY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_H);
            }
            else if (modeCheck(KMODE_NUMBERS)) {
                keypress(KEY_3);
            }
            else if (modeCheck(KMODE_FUNCTIONS)) {
                keypress(KEY_F3);
            }
        }
    }
    else if ((sensors1 & KS1_MY) == KS1_MY && (sensors2 & KS2_MY) == KS2_MY) {
        if ((sensors1 & KS1_J1_MY) == KS1_J1_MY && (sensors2 & KS2_J1_MY) == KS2_J1_MY) {
            if (modeCheck(KMODE_DEFAULT)) {
                togglemode(KMODE_SHIFT);
            }
            else if (modeCheck(KMODE_NUMBERS)) {
                togglemode(KMODE_SHIFT);
            }
            else if (modeCheck(KMODE_SYMBOLS)) {
                togglemode(KMODE_SHIFT);
            }
            else if (modeCheck(KMODE_FUNCTIONS)) {
                togglemode(KMODE_SHIFT);
            }
        }
        else if ((sensors1 & KS1_MY) == KS1_MY && (sensors2 & KS2_MY) == KS2_MY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_C);
            }
        }
    }
    else if ((sensors1 & KS1_NY) == KS1_NY && (sensors2 & KS2_NY) == KS2_NY) {
        if ((sensors1 & KS1_J1_NY) == KS1_J1_NY && (sensors2 & KS2_J1_NY) == KS2_J1_NY) {
            if (modeCheck(KMODE_DEFAULT)) {
                togglemode(KMODE_CTRL);
            }
            else if (modeCheck(KMODE_NUMBERS)) {
                togglemode(KMODE_CTRL);
            }
            else if (modeCheck(KMODE_SYMBOLS)) {
                togglemode(KMODE_CTRL);
            }
            else if (modeCheck(KMODE_FUNCTIONS)) {
                togglemode(KMODE_CTRL);
            }
        }
        else if ((sensors1 & KS1_NY) == KS1_NY && (sensors2 & KS2_NY) == KS2_NY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_W);
            }
        }
    }
    else if ((sensors1 & KS1_OY) == KS1_OY && (sensors2 & KS2_OY) == KS2_OY) {
        if ((sensors1 & KS1_J1_OY) == KS1_J1_OY && (sensors2 & KS2_J1_OY) == KS2_J1_OY) {
            if (modeCheck(KMODE_DEFAULT)) {
                togglemode(KMODE_ALT);
            }
            else if (modeCheck(KMODE_NUMBERS)) {
                togglemode(KMODE_ALT);
            }
            else if (modeCheck(KMODE_SYMBOLS)) {
                togglemode(KMODE_ALT);
            }
            else if (modeCheck(KMODE_FUNCTIONS)) {
                togglemode(KMODE_ALT);
            }
        }
        else if ((sensors1 & KS1_OY) == KS1_OY && (sensors2 & KS2_OY) == KS2_OY) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_V);
            }
        }
    }
    else if ((sensors1 & KS1_A2) == KS1_A2 && (sensors2 & KS2_A2) == KS2_A2) {
        if ((sensors1 & KS1_D2) == KS1_D2 && (sensors2 & KS2_D2) == KS2_D2) {
            if ((sensors1 & KS1_ADG2) == KS1_ADG2 && (sensors2 & KS2_ADG2) == KS2_ADG2) {
                if (modeCheck(KMODE_DEFAULT)) {
                    keypress(KEY_SEMICOLON);
                }
            }
            else if ((sensors1 & KS1_AD2) == KS1_AD2 && (sensors2 & KS2_AD2) == KS2_AD2) {
                if (modeCheck(KMODE_DEFAULT)) {
                    keypress(KEY_COMMA);
                }
            }
        }
        else if ((sensors1 & KS1_A2) == KS1_A2 && (sensors2 & KS2_A2) == KS2_A2) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_PERIOD);
            }
        }
    }
    else if ((sensors1 & KS1_J3) == KS1_J3 && (sensors2 & KS2_J3) == KS2_J3) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseup(KMOUSE_RIGHT);
        }
    }
    else if ((sensors1 & KS1_J2) == KS1_J2 && (sensors2 & KS2_J2) == KS2_J2) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseup(KMOUSE_MIDDLE);
        }
    }
    else if ((sensors1 & KS1_J1) == KS1_J1 && (sensors2 & KS2_J1) == KS2_J1) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseup(KMOUSE_LEFT);
        }
    }
}

