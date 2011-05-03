// Keyglove controller source code - Custom touchset definition
// 4/5/2011 by Jeff Rowberg 
// (Generated Wed, 06 Apr 2011 19:17:47 -0600 @ 70.188.20.207)

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

#define KS1_Y7 2
#define KS2_Y7 0

#define KS1_J8 4
#define KS2_J8 0

#define KS1_GJY 4104
#define KS2_GJY 0

#define KS1_AJY 8
#define KS2_AJY 64

#define KS1_JY 8
#define KS2_JY 0

#define KS1_KY 16
#define KS2_KY 0

#define KS1_LY 32
#define KS2_LY 0

#define KS1_VY 64
#define KS2_VY 0

#define KS1_WY 128
#define KS2_WY 0

#define KS1_XY 256
#define KS2_XY 0

#define KS1_Y6 1024
#define KS2_Y6 0

#define KS1_DGY 4198400
#define KS2_DGY 0

#define KS1_GY 4096
#define KS2_GY 0

#define KS1_HY 8192
#define KS2_HY 0

#define KS1_IY 16384
#define KS2_IY 0

#define KS1_SY 32768
#define KS2_SY 0

#define KS1_TY 65536
#define KS2_TY 0

#define KS1_UY 131072
#define KS2_UY 0

#define KS1_Y5 1048576
#define KS2_Y5 0

#define KS1_D8 2097152
#define KS2_D8 0

#define KS1_ADY 4194304
#define KS2_ADY 64

#define KS1_DY 4194304
#define KS2_DY 0

#define KS1_EY 8388608
#define KS2_EY 0

#define KS1_FY 16777216
#define KS2_FY 0

#define KS1_PY 67108864
#define KS2_PY 0

#define KS1_QY 268435456
#define KS2_QY 0

#define KS1_RY 1073741824
#define KS2_RY 0

#define KS1_Z4 0
#define KS2_Z4 8

#define KS1_Y4 0
#define KS2_Y4 16

#define KS1_A8 0
#define KS2_A8 32

#define KS1_AY_J1 0
#define KS2_AY_J1 67108928

#define KS1_AY 0
#define KS2_AY 64

#define KS1_BY_J1 0
#define KS2_BY_J1 67108992

#define KS1_BY 0
#define KS2_BY 128

#define KS1_CY_J1 0
#define KS2_CY_J1 67109120

#define KS1_CY 0
#define KS2_CY 256

#define KS1_J1_MY 0
#define KS2_J1_MY 67109888

#define KS1_MY 0
#define KS2_MY 1024

#define KS1_J1_NY 0
#define KS2_J1_NY 67112960

#define KS1_NY 0
#define KS2_NY 4096

#define KS1_J1_OY 0
#define KS2_J1_OY 67125248

#define KS1_OY 0
#define KS2_OY 16384

#define KS1_ADG2 0
#define KS2_ADG2 4784128

#define KS1_AD2 0
#define KS2_AD2 589824

#define KS1_A2 0
#define KS2_A2 65536

#define KS1_D2 0
#define KS2_D2 524288

#define KS1_G2 0
#define KS2_G2 4194304

#define KS1_J3 0
#define KS2_J3 16777216

#define KS1_J2 0
#define KS2_J2 33554432

#define KS1_J1 0
#define KS2_J1 67108864

void check_sensors_touch(long unsigned int sensors1, long unsigned int sensors2) {
    if ((sensors1 & KS1_D8) == 2097152 && (sensors2 & KS2_D8) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseon(KMOUSE_SCROLL);
        }
    }
    else if ((sensors1 & KS1_A8) == 0 && (sensors2 & KS2_A8) == 32) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseon(KMOUSE_MOVE);
        }
    }
    else if ((sensors1 & KS1_J3) == 0 && (sensors2 & KS2_J3) == 16777216) {
        if (modeCheck(KMODE_DEFAULT)) {
            mousedown(KMOUSE_RIGHT);
        }
    }
    else if ((sensors1 & KS1_J2) == 0 && (sensors2 & KS2_J2) == 33554432) {
        if (modeCheck(KMODE_DEFAULT)) {
            mousedown(KMOUSE_MIDDLE);
        }
    }
    else if ((sensors1 & KS1_J1) == 0 && (sensors2 & KS2_J1) == 67108864) {
        if (modeCheck(KMODE_DEFAULT)) {
            mousedown(KMOUSE_LEFT);
        }
    }
}

void check_sensors_release(long unsigned int sensors1, long unsigned int sensors2) {
    if ((sensors1 & KS1_Y7) == 2 && (sensors2 & KS2_Y7) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_ESC);
        }
    }
    else if ((sensors1 & KS1_J8) == 4 && (sensors2 & KS2_J8) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_T);
            keypress(KEY_E);
            keypress(KEY_S);
            keypress(KEY_T);
            keypress(KEY_SPACE);
        }
    }
    else if ((sensors1 & KS1_JY) == 8 && (sensors2 & KS2_JY) == 0) {
        if ((sensors1 & KS1_GJY) == 4104 && (sensors2 & KS2_GJY) == 0) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_ENTER);
            }
        }
        else if ((sensors1 & KS1_AJY) == 8 && (sensors2 & KS2_AJY) == 64) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_TAB);
            }
        }
        else if ((sensors1 & KS1_JY) == 8 && (sensors2 & KS2_JY) == 0) {
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
    else if ((sensors1 & KS1_KY) == 16 && (sensors2 & KS2_KY) == 0) {
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
    else if ((sensors1 & KS1_LY) == 32 && (sensors2 & KS2_LY) == 0) {
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
    else if ((sensors1 & KS1_VY) == 64 && (sensors2 & KS2_VY) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_Y);
        }
    }
    else if ((sensors1 & KS1_WY) == 128 && (sensors2 & KS2_WY) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_B);
        }
    }
    else if ((sensors1 & KS1_XY) == 256 && (sensors2 & KS2_XY) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_Q);
        }
    }
    else if ((sensors1 & KS1_Y6) == 1024 && (sensors2 & KS2_Y6) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            modifierpress(MODIFIERKEY_GUI);
        }
    }
    else if ((sensors1 & KS1_GY) == 4096 && (sensors2 & KS2_GY) == 0) {
        if ((sensors1 & KS1_DGY) == 4198400 && (sensors2 & KS2_DGY) == 0) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_BACKSPACE);
            }
        }
        else if ((sensors1 & KS1_GY) == 4096 && (sensors2 & KS2_GY) == 0) {
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
    else if ((sensors1 & KS1_HY) == 8192 && (sensors2 & KS2_HY) == 0) {
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
    else if ((sensors1 & KS1_IY) == 16384 && (sensors2 & KS2_IY) == 0) {
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
    else if ((sensors1 & KS1_SY) == 32768 && (sensors2 & KS2_SY) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_F);
        }
    }
    else if ((sensors1 & KS1_TY) == 65536 && (sensors2 & KS2_TY) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_P);
        }
    }
    else if ((sensors1 & KS1_UY) == 131072 && (sensors2 & KS2_UY) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_X);
        }
    }
    else if ((sensors1 & KS1_Y5) == 1048576 && (sensors2 & KS2_Y5) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_APP);
        }
    }
    else if ((sensors1 & KS1_D8) == 2097152 && (sensors2 & KS2_D8) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseoff(KMOUSE_SCROLL);
        }
    }
    else if ((sensors1 & KS1_DY) == 4194304 && (sensors2 & KS2_DY) == 0) {
        if ((sensors1 & KS1_ADY) == 4194304 && (sensors2 & KS2_ADY) == 64) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_SPACE);
            }
        }
        else if ((sensors1 & KS1_DY) == 4194304 && (sensors2 & KS2_DY) == 0) {
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
    else if ((sensors1 & KS1_EY) == 8388608 && (sensors2 & KS2_EY) == 0) {
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
    else if ((sensors1 & KS1_FY) == 16777216 && (sensors2 & KS2_FY) == 0) {
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
    else if ((sensors1 & KS1_PY) == 67108864 && (sensors2 & KS2_PY) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_M);
        }
    }
    else if ((sensors1 & KS1_QY) == 268435456 && (sensors2 & KS2_QY) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_G);
        }
    }
    else if ((sensors1 & KS1_RY) == 1073741824 && (sensors2 & KS2_RY) == 0) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_K);
        }
    }
    else if ((sensors1 & KS1_Z4) == 0 && (sensors2 & KS2_Z4) == 8) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_Z);
        }
    }
    else if ((sensors1 & KS1_Y4) == 0 && (sensors2 & KS2_Y4) == 16) {
        if (modeCheck(KMODE_DEFAULT)) {
            keypress(KEY_J);
        }
    }
    else if ((sensors1 & KS1_A8) == 0 && (sensors2 & KS2_A8) == 32) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseoff(KMOUSE_MOVE);
        }
    }
    else if ((sensors1 & KS1_AY) == 0 && (sensors2 & KS2_AY) == 64) {
        if ((sensors1 & KS1_AY_J1) == 0 && (sensors2 & KS2_AY_J1) == 67108928) {
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
        else if ((sensors1 & KS1_AY) == 0 && (sensors2 & KS2_AY) == 64) {
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
    else if ((sensors1 & KS1_BY) == 0 && (sensors2 & KS2_BY) == 128) {
        if ((sensors1 & KS1_BY_J1) == 0 && (sensors2 & KS2_BY_J1) == 67108992) {
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
        else if ((sensors1 & KS1_BY) == 0 && (sensors2 & KS2_BY) == 128) {
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
    else if ((sensors1 & KS1_CY) == 0 && (sensors2 & KS2_CY) == 256) {
        if ((sensors1 & KS1_CY_J1) == 0 && (sensors2 & KS2_CY_J1) == 67109120) {
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
        else if ((sensors1 & KS1_CY) == 0 && (sensors2 & KS2_CY) == 256) {
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
    else if ((sensors1 & KS1_MY) == 0 && (sensors2 & KS2_MY) == 1024) {
        if ((sensors1 & KS1_J1_MY) == 0 && (sensors2 & KS2_J1_MY) == 67109888) {
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
        else if ((sensors1 & KS1_MY) == 0 && (sensors2 & KS2_MY) == 1024) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_C);
            }
        }
    }
    else if ((sensors1 & KS1_NY) == 0 && (sensors2 & KS2_NY) == 4096) {
        if ((sensors1 & KS1_J1_NY) == 0 && (sensors2 & KS2_J1_NY) == 67112960) {
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
        else if ((sensors1 & KS1_NY) == 0 && (sensors2 & KS2_NY) == 4096) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_W);
            }
        }
    }
    else if ((sensors1 & KS1_OY) == 0 && (sensors2 & KS2_OY) == 16384) {
        if ((sensors1 & KS1_J1_OY) == 0 && (sensors2 & KS2_J1_OY) == 67125248) {
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
        else if ((sensors1 & KS1_OY) == 0 && (sensors2 & KS2_OY) == 16384) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_V);
            }
        }
    }
    else if ((sensors1 & KS1_A2) == 0 && (sensors2 & KS2_A2) == 65536) {
        if ((sensors1 & KS1_D2) == 0 && (sensors2 & KS2_D2) == 524288) {
            if ((sensors1 & KS1_ADG2) == 0 && (sensors2 & KS2_ADG2) == 4784128) {
                if (modeCheck(KMODE_DEFAULT)) {
                    keypress(KEY_SEMICOLON);
                }
            }
            else if ((sensors1 & KS1_AD2) == 0 && (sensors2 & KS2_AD2) == 589824) {
                if (modeCheck(KMODE_DEFAULT)) {
                    keypress(KEY_COMMA);
                }
            }
        }
        else if ((sensors1 & KS1_A2) == 0 && (sensors2 & KS2_A2) == 65536) {
            if (modeCheck(KMODE_DEFAULT)) {
                keypress(KEY_PERIOD);
            }
        }
    }
    else if ((sensors1 & KS1_J3) == 0 && (sensors2 & KS2_J3) == 16777216) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseup(KMOUSE_LEFT);
        }
    }
    else if ((sensors1 & KS1_J2) == 0 && (sensors2 & KS2_J2) == 33554432) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseup(KMOUSE_LEFT);
        }
    }
    else if ((sensors1 & KS1_J1) == 0 && (sensors2 & KS2_J1) == 67108864) {
        if (modeCheck(KMODE_DEFAULT)) {
            mouseup(KMOUSE_LEFT);
        }
    }
}

