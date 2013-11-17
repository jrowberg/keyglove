// Keyglove controller source code - Overall architecture/hardware option definitions
// 9/10/2013 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2013 Jeff Rowberg

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

#ifndef _CONFIG_H_
#define _CONFIG_H_

/* ===============================================
 * HARDWARE OPTIONS
=============================================== */

// attempt auto-detection of base hardware and interfaces

// Teensy++ 2.0
#if defined(CORE_TEENSY)
    #if defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__)
        #define KG_BOARD                        KG_BOARD_TEENSYPP2
        #ifdef CORE_TEENSY_SERIAL
            #define AUTO_KG_HOSTIF_USB_SERIAL   KG_HOSTIF_USB_SERIAL
        #endif
        #ifdef CORE_TEENSY_RAWHID
            #define AUTO_KG_HOSTIF_USB_RAWHID   KG_HOSTIF_USB_RAWHID
        #endif
        #ifdef CORE_TEENSY_HID
            #define AUTO_KG_HOSTIF_USB_HID      KG_HOSTIF_USB_HID
        #endif
    #else
        #error Only the Teensy++ 2.0 variant of the Teensy line is supported.
    #endif

// Arduino Due
#elif defined(_VARIANT_ARDUINO_DUE_X_)
    #define KG_BOARD            KG_BOARD_ARDUINO_DUE

// Keyglove
#elif defined(CORE_KEYGLOVE)
    #define KG_BOARD            KG_BOARD_KEYGLOVE

#else
    // ...if you're adding support for something else here, make sure you define it by this point!
    #warning No compatible board defined. This could be a problem.
#endif



// make sure these definitions exist even if they are "0"
#ifndef AUTO_KG_HOSTIF_USB_SERIAL
    #define AUTO_KG_HOSTIF_USB_SERIAL 0
#endif
#ifndef AUTO_KG_HOSTIF_USB_RAWHID
    #define AUTO_KG_HOSTIF_USB_RAWHID 0
#endif
#ifndef AUTO_KG_HOSTIF_USB_HID
    #define AUTO_KG_HOSTIF_USB_HID 0
#endif



#define KG_HOSTIF           (AUTO_KG_HOSTIF_USB_SERIAL | AUTO_KG_HOSTIF_USB_RAWHID | AUTO_KG_HOSTIF_USB_HID /*| KG_HOSTIF_BT2_HID | KG_HOSTIF_BT2_RAWHID */)

//#define KG_MOTION           KG_MOTION_NONE
#define KG_MOTION           KG_MOTION_MPU6050_HAND

#define KG_FEEDBACK         KG_FEEDBACK_BLINK
//NOT RE-IMPLEMENTED YET: #define KG_FEEDBACK         (KG_FEEDBACK_BLINK | KG_FEEDBACK_PIEZO | KG_FEEDBACK_VIBRATE | KG_FEEDBACK_RGB)

// DUALGLOVE / FLEX / PRESSURE (NOT IMPLEMENTED YET)
#define KG_DUALGLOVE        KG_DUALGLOVE_NONE
#define KG_FLEX             KG_FLEX_NONE
#define KG_PRESSURE         KG_PRESSURE_NONE



/* ===============================================
 * DEBUG SETTINGS
=============================================== */

#define KG_DEBUG            KG_DEBUG_NONE

/*
Debug options:
  KG_DEBUG_NONE
  KG_DEBUG_BENCHMARK
  KG_DEBUG_FEEDBACK
  KG_DEBUG_TOUCH
  KG_DEBUG_TOUCHSET
  KG_DEBUG_MOTION
  KG_DEBUG_HOSTIF_USB
  KG_DEBUG_HOSTIF_BT2
  KG_DEBUG_HID_KEYBOARD
  KG_DEBUG_HID_MOUSE
*/



/* ===============================================
 * DEBUG COMPILER MACROS
=============================================== */

#if (KG_DEBUG & KG_DEBUG_BENCHMARK)
    #define DEBUG_BENCHMARK(x)          x
#else
    #define DEBUG_BENCHMARK(x)
#endif

#if (KG_DEBUG & KG_DEBUG_TOUCH)
    #define DEBUG_TOUCH(x)              x
#else
    #define DEBUG_TOUCH(x)
#endif

#if (KG_DEBUG & KG_DEBUG_TOUCHSET)
    #define DEBUG_TOUCHSET(x)           x
#else
    #define DEBUG_TOUCHSET(x)
#endif

#if (KG_DEBUG & KG_DEBUG_MOTION)
    #define DEBUG_MOTION(x)             x
#else
    #define DEBUG_MOTION(x)
#endif

#if (KG_DEBUG & KG_DEBUG_FEEDBACK)
    #define DEBUG_FEEDBACK(x)           x
#else
    #define DEBUG_FEEDBACK(x)
#endif

#if (KG_DEBUG & KG_DEBUG_HOSTIF_USB)
    #define DEBUG_HOSTIF_USB(x)         x
#else
    #define DEBUG_HOSTIF_USB(x)
#endif

#if (KG_DEBUG & KG_DEBUG_HOSTIF_BT2)
    #define DEBUG_HOSTIF_BT2(x)         x
#else
    #define DEBUG_HOSTIF_BT2(x)
#endif

#if (KG_DEBUG & KG_DEBUG_HID_KEYBOARD)
    #define DEBUG_HID_KEYBOARD(x)       x
#else
    #define DEBUG_HID_KEYBOARD(x)
#endif

#if (KG_DEBUG & KG_DEBUG_HID_MOUSE)
    #define DEBUG_HID_MOUSE(x)          x
#else
    #define DEBUG_HID_MOUSE(x)
#endif

#endif // _CONFIG_H_
