// Keyglove controller source code - Debug output declarations
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



#ifndef _DEBUG_H_
#define _DEBUG_H_

/* ===============================================
 * DEBUG SETTINGS
=============================================== */

#if (defined DEBUG_BENCHMARK) || (defined DEBUG_ACCELEROMETER) || \
    (defined DEBUG_GYROSCOPE) || (defined DEBUG_MAGNETOMETER) || \
    (defined DEBUG_MOTIONFUSION) || (defined DEBUG_GESTURE) || \
    (defined DEBUG_TOUCH) || (defined DEBUG_TOUCHSET) || \
    (defined DEBUG_PS2) || (defined DEBUG_USB) || \
    (defined DEBUG_R400) || (defined DEBUG_IWRAP) || \
    (defined DEBUG_KEYBOARD) || (defined DEBUG_MOUSE) || \
    (defined DEBUG_JOYSTICK)

    #ifndef ENABLE_SERIAL
        #define ENABLE_SERIAL
    #endif

#endif

#ifdef DEBUG_BENCHMARK
    #define DEBUG_PRN_BENCHMARK(x)          Serial.print(x)
    #define DEBUG_PRNF_BENCHMARK(x, y)      Serial.print(x, y)
    #define DEBUG_PRNL_BENCHMARK(x)         Serial.println(x)
    #define DEBUG_PRNLF_BENCHMARK(x, y)     Serial.println(x, y)
#else
    #define DEBUG_PRN_BENCHMARK(x)
    #define DEBUG_PRNF_BENCHMARK(x, y)
    #define DEBUG_PRNL_BENCHMARK(x)
    #define DEBUG_PRNLF_BENCHMARK(x, y)
#endif

#ifdef DEBUG_ACCELEROMETER
    #define DEBUG_PRN_ACCELEROMETER(x)      Serial.print(x)
    #define DEBUG_PRNF_ACCELEROMETER(x, y)  Serial.print(x, y)
    #define DEBUG_PRNL_ACCELEROMETER(x)     Serial.println(x)
    #define DEBUG_PRNLF_ACCELEROMETER(x, y) Serial.println(x, y)
#else
    #define DEBUG_PRN_ACCELEROMETER(x)
    #define DEBUG_PRNF_ACCELEROMETER(x, y)
    #define DEBUG_PRNL_ACCELEROMETER(x)
    #define DEBUG_PRNLF_ACCELEROMETER(x, y)
#endif

#ifdef DEBUG_GYROSCOPE
    #define DEBUG_PRN_GYROSCOPE(x)          Serial.print(x)
    #define DEBUG_PRNF_GYROSCOPE(x, y)      Serial.print(x, y)
    #define DEBUG_PRNL_GYROSCOPE(x)         Serial.println(x)
    #define DEBUG_PRNLF_GYROSCOPE(x, y)     Serial.println(x, y)
#else
    #define DEBUG_PRN_GYROSCOPE(x)
    #define DEBUG_PRNF_GYROSCOPE(x, y)
    #define DEBUG_PRNL_GYROSCOPE(x)
    #define DEBUG_PRNLF_GYROSCOPE(x, y)
#endif

#ifdef DEBUG_MAGNETOMETER
    #define DEBUG_PRN_MAGNETOMETER(x)       Serial.print(x)
    #define DEBUG_PRNF_MAGNETOMETER(x, y)   Serial.print(x, y)
    #define DEBUG_PRNL_MAGNETOMETER(x)      Serial.println(x)
    #define DEBUG_PRNLF_MAGNETOMETER(x, y)  Serial.println(x, y)
#else
    #define DEBUG_PRN_MAGNETOMETER(x)
    #define DEBUG_PRNF_MAGNETOMETER(x, y)
    #define DEBUG_PRNL_MAGNETOMETER(x)
    #define DEBUG_PRNLF_MAGNETOMETER(x, y)
#endif

#ifdef DEBUG_MOTIONFUSION
    #define DEBUG_PRN_MOTIONFUSION(x)       Serial.print(x)
    #define DEBUG_PRNF_MOTIONFUSION(x, y)   Serial.print(x, y)
    #define DEBUG_PRNL_MOTIONFUSION(x)      Serial.println(x)
    #define DEBUG_PRNLF_MOTIONFUSION(x, y)  Serial.println(x, y)
#else
    #define DEBUG_PRN_MOTIONFUSION(x)
    #define DEBUG_PRNF_MOTIONFUSION(x, y)
    #define DEBUG_PRNL_MOTIONFUSION(x)
    #define DEBUG_PRNLF_MOTIONFUSION(x, y)
#endif

#ifdef DEBUG_GESTURE
    #define DEBUG_PRN_GESTURE(x)            Serial.print(x)
    #define DEBUG_PRNF_GESTURE(x, y)        Serial.print(x, y)
    #define DEBUG_PRNL_GESTURE(x)           Serial.println(x)
    #define DEBUG_PRNLF_GESTURE(x, y)       Serial.println(x, y)
#else
    #define DEBUG_PRN_GESTURE(x)
    #define DEBUG_PRNF_GESTURE(x, y)
    #define DEBUG_PRNL_GESTURE(x)
    #define DEBUG_PRNLF_GESTURE(x, y)
#endif

#ifdef DEBUG_TOUCH
    #define DEBUG_PRN_TOUCH(x)              Serial.print(x)
    #define DEBUG_PRNF_TOUCH(x, y)          Serial.print(x, y)
    #define DEBUG_PRNL_TOUCH(x)             Serial.println(x)
    #define DEBUG_PRNLF_TOUCH(x, y)         Serial.println(x, y)
#else
    #define DEBUG_PRN_TOUCH(x)
    #define DEBUG_PRNF_TOUCH(x, y)
    #define DEBUG_PRNL_TOUCH(x)
    #define DEBUG_PRNLF_TOUCH(x, y)
#endif

#ifdef DEBUG_TOUCHSET
    #define DEBUG_PRN_TOUCHSET(x)           Serial.print(x)
    #define DEBUG_PRNF_TOUCHSET(x, y)       Serial.print(x, y)
    #define DEBUG_PRNL_TOUCHSET(x)          Serial.println(x)
    #define DEBUG_PRNLF_TOUCHSET(x, y)      Serial.println(x, y)
#else
    #define DEBUG_PRN_TOUCHSET(x)
    #define DEBUG_PRNF_TOUCHSET(x, y)
    #define DEBUG_PRNL_TOUCHSET(x)
    #define DEBUG_PRNLF_TOUCHSET(x, y)
#endif

#ifdef DEBUG_PS2
    #define DEBUG_PRN_PS2(x)                Serial.print(x)
    #define DEBUG_PRNF_PS2(x, y)            Serial.print(x, y)
    #define DEBUG_PRNL_PS2(x)               Serial.println(x)
    #define DEBUG_PRNLF_PS2(x, y)           Serial.println(x, y)
#else
    #define DEBUG_PRN_PS2(x)
    #define DEBUG_PRNF_PS2(x, y)
    #define DEBUG_PRNL_PS2(x)
    #define DEBUG_PRNLF_PS2(x, y)
#endif

#ifdef DEBUG_USB
    #define DEBUG_PRN_USB(x)                Serial.print(x)
    #define DEBUG_PRNF_USB(x, y)            Serial.print(x, y)
    #define DEBUG_PRNL_USB(x)               Serial.println(x)
    #define DEBUG_PRNLF_USB(x, y)           Serial.println(x, y)
#else
    #define DEBUG_PRN_USB(x)
    #define DEBUG_PRNF_USB(x, y)
    #define DEBUG_PRNL_USB(x)
    #define DEBUG_PRNLF_USB(x, y)
#endif

#ifdef DEBUG_R400
    #define DEBUG_PRN_R400(x)               Serial.print(x)
    #define DEBUG_PRNF_R400(x, y)           Serial.print(x, y)
    #define DEBUG_PRNL_R400(x)              Serial.println(x)
    #define DEBUG_PRNLF_R400(x, y)          Serial.println(x, y)
#else
    #define DEBUG_PRN_R400(x)
    #define DEBUG_PRNF_R400(x, y)
    #define DEBUG_PRNL_R400(x)
    #define DEBUG_PRNLF_R400(x, y)
#endif

#ifdef DEBUG_IWRAP
    #define DEBUG_PRN_IWRAP(x)              Serial.print(x)
    #define DEBUG_PRNF_IWRAP(x, y)          Serial.print(x, y)
    #define DEBUG_PRNL_IWRAP(x)             Serial.println(x)
    #define DEBUG_PRNLF_IWRAP(x, y)         Serial.println(x, y)
#else
    #define DEBUG_PRN_IWRAP(x)
    #define DEBUG_PRNF_IWRAP(x, y)
    #define DEBUG_PRNL_IWRAP(x)
    #define DEBUG_PRNLF_IWRAP(x, y)
#endif

#ifdef DEBUG_KEYBOARD
    #define DEBUG_PRN_KEYBOARD(x)           Serial.print(x)
    #define DEBUG_PRNF_KEYBOARD(x, y)       Serial.print(x, y)
    #define DEBUG_PRNL_KEYBOARD(x)          Serial.println(x)
    #define DEBUG_PRNLF_KEYBOARD(x, y)      Serial.println(x, y)
#else
    #define DEBUG_PRN_KEYBOARD(x)
    #define DEBUG_PRNF_KEYBOARD(x, y)
    #define DEBUG_PRNL_KEYBOARD(x)
    #define DEBUG_PRNLF_KEYBOARD(x, y)
#endif

#ifdef DEBUG_MOUSE
    #define DEBUG_PRN_MOUSE(x)              Serial.print(x)
    #define DEBUG_PRNF_MOUSE(x, y)          Serial.print(x, y)
    #define DEBUG_PRNL_MOUSE(x)             Serial.println(x)
    #define DEBUG_PRNLF_MOUSE(x, y)         Serial.println(x, y)
#else
    #define DEBUG_PRN_MOUSE(x)
    #define DEBUG_PRNF_MOUSE(x, y)
    #define DEBUG_PRNL_MOUSE(x)
    #define DEBUG_PRNLF_MOUSE(x, y)
#endif

#ifdef DEBUG_JOYSTICK
    #define DEBUG_PRN_JOYSTICK(x)           Serial.print(x)
    #define DEBUG_PRNF_JOYSTICK(x, y)       Serial.print(x, y)
    #define DEBUG_PRNL_JOYSTICK(x)          Serial.println(x)
    #define DEBUG_PRNLF_JOYSTICK(x, y)      Serial.println(x, y)
#else
    #define DEBUG_PRN_JOYSTICK(x)
    #define DEBUG_PRNF_JOYSTICK(x, y)
    #define DEBUG_PRNL_JOYSTICK(x)
    #define DEBUG_PRNLF_JOYSTICK(x, y)
#endif

#endif // _DEBUG_H_


