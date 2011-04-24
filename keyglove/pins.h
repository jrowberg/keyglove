// Keyglove controller source code - pins constant definition file
// 10/1/2010 by Jeff Rowberg <jeff@rowberg.net>

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

#ifdef USE_ARDUINO
    #define KSP_A 16
    #define KSP_B 17
    #define KSP_C 22
    #define KSP_D 23
    #define KSP_E 24
    #define KSP_F 25
    #define KSP_G 26
    #define KSP_H 27
    #define KSP_I 28
    #define KSP_J 29
    #define KSP_K 30
    #define KSP_L 31
    #define KSP_M 32
    #define KSP_N 33
    #define KSP_O 34
    #define KSP_P 35
    #define KSP_Q 36
    #define KSP_R 37
    #define KSP_S 38
    #define KSP_T 39
    #define KSP_U 40
    #define KSP_V 41
    #define KSP_W 42
    #define KSP_X 43
    #define KSP_Y 44
    #define KSP_Z 45
    #define KSP_1 46
    #define KSP_2 47
    #define KSP_3 48
    #define KSP_4 49
    #define KSP_5 50
    #define KSP_6 51
    #define KSP_7 52
    #define KSP_8 53

    #define BLINK_LED_PIN 13 // Arduino LED pin is 13

    #define TRICOLOR_RED_PIN 11
    #define TRICOLOR_GREEN_PIN 12
    #define TRICOLOR_BLUE_PIN 13

    #define KB_PIN_CLOCK    19
    #define KB_PIN_DATA     18
    #define KB_INT1         2
    #define KB_INT2         3
    
    #define MOUSE_PIN_CLOCK 3
    #define MOUSE_PIN_DATA  2
    #define MOUSE_INT1      0
    #define MOUSE_INT2      1
#endif /* USE_ARDUINO */

#ifdef USE_TEENSY
    #define KSP_A 4 // D4
    #define KSP_B 5 // D5
    #define KSP_C 7 // D7
    #define KSP_D 8 // E0
    #define KSP_E 9 // E1
    #define KSP_F 10 // C0
    #define KSP_G 11 // C1
    #define KSP_H 12 // C2
    #define KSP_I 13 // C3
    #define KSP_J 14 // C4
    #define KSP_K 15 // C5
    #define KSP_L 16 // C6
    #define KSP_M 17 // C7
    #define KSP_N 18 // E6
    #define KSP_O 19 // E7
    #define KSP_P 20 // B0
    #define KSP_Q 21 // B1
    #define KSP_R 22 // B2
    #define KSP_S 23 // B3
    #define KSP_T 24 // B4
    #define KSP_U 25 // B5
    #define KSP_V 26 // B6
    #define KSP_W 27 // B7 (top left)
    #define KSP_X 28 // PA0 (internal, top right)
    #define KSP_Y 29 // PA1
    #define KSP_Z 30 // PA2
    #define KSP_1 31 // PA3
    #define KSP_2 32 // PA4
    #define KSP_3 33 // PA5
    #define KSP_4 34 // PA6
    #define KSP_5 35 // PA7 (internal, bottom left)
    #define KSP_6 38 // F0
    #define KSP_7 39 // F1
    #define KSP_8 40 // F2

    #define BLINK_LED_PIN 6 // Teensy++ LED pin 6

    #define VIBRATE_PIN 41 // F3
    #define SOUND_PIN 42 // F4

    #define TRICOLOR_RED_PIN 43   // F5
    #define TRICOLOR_GREEN_PIN 44 // F6
    #define TRICOLOR_BLUE_PIN 45  // F7
    
    #define KB_PIN_CLOCK    19
    #define KB_PIN_DATA     18

    #define MOUSE_PIN_CLOCK 3
    #define MOUSE_PIN_DATA  2
#endif /* USE_TEENSY */

#define KG_TOTAL_SENSORS 34
#define KG_BASE_COMBINATIONS 60

int pins[KG_TOTAL_SENSORS] = {
    KSP_A, KSP_B, KSP_C, KSP_D, KSP_E,
    KSP_F, KSP_G, KSP_H, KSP_I, KSP_J,
    KSP_K, KSP_L, KSP_M, KSP_N, KSP_O,
    KSP_P, KSP_Q, KSP_R, KSP_S, KSP_T,
    KSP_U, KSP_V, KSP_W, KSP_X, KSP_Y,
    KSP_Z, KSP_1, KSP_2, KSP_3, KSP_4,
    KSP_5, KSP_6, KSP_7, KSP_8 };
    
int combinations[KG_BASE_COMBINATIONS][2] = {
    { KSP_Z, KSP_7 /* Z7 */ },
    { KSP_Y, KSP_7 /* Y7 */ },
    { KSP_J, KSP_8 /* J8 */ },
    { KSP_J, KSP_Y /* JY */ },
    { KSP_K, KSP_Y /* KY */ },
    { KSP_L, KSP_Y /* LY */ },
    { KSP_V, KSP_Y /* VY */ },
    { KSP_W, KSP_Y /* WY */ },
    { KSP_X, KSP_Y /* XY */ },
    { KSP_Z, KSP_6 /* Z6 */ },
    { KSP_Y, KSP_6 /* Y6 */ },
    { KSP_G, KSP_8 /* G8 */ },
    { KSP_G, KSP_Y /* GY */ },
    { KSP_H, KSP_Y /* HY */ },
    { KSP_I, KSP_Y /* IY */ },
    { KSP_S, KSP_Y /* SY */ },
    { KSP_T, KSP_Y /* TY */ },
    { KSP_U, KSP_Y /* UY */ },
    { KSP_G, KSP_7 /* G7 */ },
    { KSP_Z, KSP_5 /* Z5 */ },
    { KSP_Y, KSP_5 /* Y5 */ },
    { KSP_D, KSP_8 /* D8 */ },
    { KSP_D, KSP_Y /* DY */ },
    { KSP_E, KSP_Y /* EY */ },
    { KSP_F, KSP_Y /* FY */ },
    { KSP_P, KSP_Z /* PZ */ },
    { KSP_P, KSP_Y /* PY */ },
    { KSP_Q, KSP_Z /* QZ */ },
    { KSP_Q, KSP_Y /* QY */ },
    { KSP_R, KSP_Z /* RZ */ },
    { KSP_R, KSP_Y /* RY */ },
    { KSP_D, KSP_4 /* D4 */ },
    { KSP_D, KSP_6 /* D6 */ },
    { KSP_D, KSP_7 /* D7 */ },
    { KSP_D, KSP_M /* DM */ },
    { KSP_Z, KSP_4 /* Z4 */ },
    { KSP_Y, KSP_4 /* Y4 */ },
    { KSP_A, KSP_8 /* A8 */ },
    { KSP_A, KSP_Y /* AY */ },
    { KSP_B, KSP_Y /* BY */ },
    { KSP_C, KSP_Y /* CY */ },
    { KSP_M, KSP_Z /* MZ */ },
    { KSP_M, KSP_Y /* MY */ },
    { KSP_N, KSP_Z /* NZ */ },
    { KSP_N, KSP_Y /* NY */ },
    { KSP_O, KSP_Z /* OZ */ },
    { KSP_O, KSP_Y /* OY */ },
    { KSP_A, KSP_3 /* A3 */ },
    { KSP_A, KSP_2 /* A2 */ },
    { KSP_A, KSP_1 /* A1 */ },
    { KSP_D, KSP_3 /* D3 */ },
    { KSP_D, KSP_2 /* D2 */ },
    { KSP_D, KSP_1 /* D1 */ },
    { KSP_G, KSP_3 /* G3 */ },
    { KSP_G, KSP_2 /* G2 */ },
    { KSP_G, KSP_1 /* G1 */ },
    { KSP_J, KSP_3 /* J3 */ },
    { KSP_J, KSP_2 /* J2 */ },
    { KSP_J, KSP_1 /* J1 */ },
    { KSP_Y, KSP_1 /* Y1 */ }
};
