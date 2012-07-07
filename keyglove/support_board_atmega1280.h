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



#ifndef _SUPPORT_BOARD_ATMEGA1280_H_
#define _SUPPORT_BOARD_ATMEGA1280_H_

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

#ifdef ENABLE_R400    
    #define KSP_5 2
    #define KSP_6 3
    #define KSP_7 4
    #define KSP_8 5
#endif /* ENABLE_RX400 */

#define KG_INTERRUPT_PIN_ACCEL 1    // E4 (internal/tiny, right)
#define KG_INTERRUPT_NUM_ACCEL 4    // Teensy++ interrupt #4

#define KG_INTERRUPT_PIN_GYRO 1     // E5 (internal/tiny, left)
#define KG_INTERRUPT_NUM_GYRO 5     // Teensy++ interrupt #5

#define KG_INTERRUPT_PIN_FUSION 1   // E4 (internal/tiny, right)
#define KG_INTERRUPT_NUM_FUSION 4   // Teensy++ interrupt #4

#define KG_PIN_BLINK 13 // Arduino Mega LED is pin 6

#define KG_PIN_PIEZO 12
#define KG_PIN_VIBRATE 11

#define KG_PIN_RGB_RED 10
#define KG_PIN_RGB_GREEN 9
#define KG_PIN_RGB_BLUE 8



#define BLINK_LED_PIN 13 // Arduino LED pin is 13
#define VIBRATE_PIN 10
#define SOUND_PIN 9

#define RGB_RED_PIN 11
#define RGB_GREEN_PIN 12
#define RGB_BLUE_PIN 13

#define KB_PIN_CLOCK    19
#define KB_PIN_DATA     18
#define KB_INT1         2
#define KB_INT2         3

#define MOUSE_PIN_CLOCK 3
#define MOUSE_PIN_DATA  2
#define MOUSE_INT1      0
#define MOUSE_INT2      1

void setup_board() {
}

#endif // _SUPPORT_BOARD_ATMEGA1280_H_


