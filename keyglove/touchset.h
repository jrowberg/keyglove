// Keyglove controller source code - touchset definition file
// 10/1/2010 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2010 Jeff Rowberg

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

// TOTAL: 36 unique combinations used
// ===============================================================

// numeric sensor definitions

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

#define KS1_AY 0
#define KS2_AY 64

#define KS1_BY 0
#define KS2_BY 128

#define KS1_CY 0
#define KS2_CY 256

#define KS1_MY 0
#define KS2_MY 1024

#define KS1_NY 0
#define KS2_NY 4096

#define KS1_OY 0
#define KS2_OY 16384

#define KS1_ADG2 0
#define KS2_ADG2 4784128

#define KS1_AD2 0
#define KS2_AD2 589824

#define KS1_AG2 0
#define KS2_AG2 4259840

#define KS1_AJ2 0
#define KS2_AJ2 33619968

#define KS1_A2 0
#define KS2_A2 65536

#define KS1_DG2 0
#define KS2_DG2 4718592

#define KS1_DJ2 0
#define KS2_DJ2 34078720

#define KS1_D2 0
#define KS2_D2 524288

#define KS1_G2 0
#define KS2_G2 4194304

#define KS1_J2 0
#define KS2_J2 33554432

// conditional statements to test and trigger every used sensor

void test_sensors(long unsigned int sensors1, long unsigned int sensors2) {
    if ((sensors1 & KS1_JY) == 8 && (sensors2 & KS2_JY) == 0) {
keypress(KEY_O);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release JY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys o");
#endif
}
    else if ((sensors1 & KS1_KY) == 16 && (sensors2 & KS2_KY) == 0) {
keypress(KEY_R);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release KY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys r");
#endif
}
    else if ((sensors1 & KS1_LY) == 32 && (sensors2 & KS2_LY) == 0) {
keypress(KEY_U);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release LY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys u");
#endif
}
    else if ((sensors1 & KS1_VY) == 64 && (sensors2 & KS2_VY) == 0) {
keypress(KEY_Y);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release VY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys y");
#endif
}
    else if ((sensors1 & KS1_WY) == 128 && (sensors2 & KS2_WY) == 0) {
keypress(KEY_B);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release WY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys b");
#endif
}
    else if ((sensors1 & KS1_XY) == 256 && (sensors2 & KS2_XY) == 0) {
keypress(KEY_Q);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release XY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys q");
#endif
}
    else if ((sensors1 & KS1_GY) == 4096 && (sensors2 & KS2_GY) == 0) {
keypress(KEY_A);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release GY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys a");
#endif
}
    else if ((sensors1 & KS1_HY) == 8192 && (sensors2 & KS2_HY) == 0) {
keypress(KEY_S);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release HY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys s");
#endif
}
    else if ((sensors1 & KS1_IY) == 16384 && (sensors2 & KS2_IY) == 0) {
keypress(KEY_L);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release IY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys l");
#endif
}
    else if ((sensors1 & KS1_SY) == 32768 && (sensors2 & KS2_SY) == 0) {
keypress(KEY_F);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release SY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys f");
#endif
}
    else if ((sensors1 & KS1_TY) == 65536 && (sensors2 & KS2_TY) == 0) {
keypress(KEY_P);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release TY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys p");
#endif
}
    else if ((sensors1 & KS1_UY) == 131072 && (sensors2 & KS2_UY) == 0) {
keypress(KEY_X);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release UY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys x");
#endif
}
    else if ((sensors1 & KS1_DY) == 4194304 && (sensors2 & KS2_DY) == 0) {
keypress(KEY_T);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release DY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys t");
#endif
}
    else if ((sensors1 & KS1_EY) == 8388608 && (sensors2 & KS2_EY) == 0) {
keypress(KEY_N);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release EY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys n");
#endif
}
    else if ((sensors1 & KS1_FY) == 16777216 && (sensors2 & KS2_FY) == 0) {
keypress(KEY_D);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release FY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys d");
#endif
}
    else if ((sensors1 & KS1_PY) == 67108864 && (sensors2 & KS2_PY) == 0) {
keypress(KEY_M);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release PY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys m");
#endif
}
    else if ((sensors1 & KS1_QY) == 268435456 && (sensors2 & KS2_QY) == 0) {
keypress(KEY_G);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release QY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys g");
#endif
}
    else if ((sensors1 & KS1_RY) == 1073741824 && (sensors2 & KS2_RY) == 0) {
keypress(KEY_K);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release RY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys k");
#endif
}
    else if ((sensors1 & KS1_Z4) == 0 && (sensors2 & KS2_Z4) == 8) {
keypress(KEY_Z);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release Z4");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys z");
#endif
}
    else if ((sensors1 & KS1_Y4) == 0 && (sensors2 & KS2_Y4) == 16) {
keypress(KEY_J);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release Y4");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys j");
#endif
}
    else if ((sensors1 & KS1_AY) == 0 && (sensors2 & KS2_AY) == 64) {
keypress(KEY_E);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release AY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys e");
#endif
}
    else if ((sensors1 & KS1_BY) == 0 && (sensors2 & KS2_BY) == 128) {
keypress(KEY_I);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release BY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys i");
#endif
}
    else if ((sensors1 & KS1_CY) == 0 && (sensors2 & KS2_CY) == 256) {
keypress(KEY_H);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release CY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys h");
#endif
}
    else if ((sensors1 & KS1_MY) == 0 && (sensors2 & KS2_MY) == 1024) {
keypress(KEY_C);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release MY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys c");
#endif
}
    else if ((sensors1 & KS1_NY) == 0 && (sensors2 & KS2_NY) == 4096) {
keypress(KEY_W);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release NY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys w");
#endif
}
    else if ((sensors1 & KS1_OY) == 0 && (sensors2 & KS2_OY) == 16384) {
keypress(KEY_V);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release OY");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys v");
#endif
}
    else if ((sensors1 & KS1_A2) == 0 && (sensors2 & KS2_A2) == 65536) {
        if ((sensors1 & KS1_D2) == 0 && (sensors2 & KS2_D2) == 524288) {
            if ((sensors1 & KS1_ADG2) == 0 && (sensors2 & KS2_ADG2) == 4784128) {
keypress(KEY_7);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release ADG2");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys 7");
#endif
}
            else if ((sensors1 & KS1_AD2) == 0 && (sensors2 & KS2_AD2) == 589824) {
keypress(KEY_3);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release AD2");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys 3");
#endif
}
        }
        else if ((sensors1 & KS1_AG2) == 0 && (sensors2 & KS2_AG2) == 4259840) {
keypress(KEY_5);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release AG2");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys 5");
#endif
}
        else if ((sensors1 & KS1_AJ2) == 0 && (sensors2 & KS2_AJ2) == 33619968) {
keypress(KEY_9);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release AJ2");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys 9");
#endif
}
        else if ((sensors1 & KS1_A2) == 0 && (sensors2 & KS2_A2) == 65536) {
keypress(KEY_1);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release A2");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys 1");
#endif
}
    }
    else if ((sensors1 & KS1_D2) == 0 && (sensors2 & KS2_D2) == 524288) {
        if ((sensors1 & KS1_DG2) == 0 && (sensors2 & KS2_DG2) == 4718592) {
keypress(KEY_6);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release DG2");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys 6");
#endif
}
        else if ((sensors1 & KS1_DJ2) == 0 && (sensors2 & KS2_DJ2) == 34078720) {
keypress(KEY_0);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release DJ2");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys 0");
#endif
}
        else if ((sensors1 & KS1_D2) == 0 && (sensors2 & KS2_D2) == 524288) {
keypress(KEY_2);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release D2");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys 2");
#endif
}
    }
    else if ((sensors1 & KS1_G2) == 0 && (sensors2 & KS2_G2) == 4194304) {
keypress(KEY_4);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release G2");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys 4");
#endif
}
    else if ((sensors1 & KS1_J2) == 0 && (sensors2 & KS2_J2) == 33554432) {
keypress(KEY_8);
#ifdef SERIAL_DEBUG_TOUCH
    Serial.println("release J2");
#endif
#ifdef SERIAL_DEBUG_KEYBOARD
    Serial.println("sendkeys 8");
#endif
}
}
