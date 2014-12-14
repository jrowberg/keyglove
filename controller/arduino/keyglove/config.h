// Keyglove controller source code - Modular hardware/architecture configuration definitions
// 2014-11-07 by Jeff Rowberg <jeff@rowberg.net>

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
 * @file config.h
 * @brief **USER-DEFINED:** Modular hardware/architecture configuration definitions
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * Use this file to specify which microcontroller board design and connected
 * peripheral devices you have in your Keyglove build. For example, if you are
 * creating a Teensy++-based build with a single MPU-6050 I2C motion sensor, a
 * Bluegiga WT12 Bluetooth module, and raw HID USB support, you would define
 * this here.
 *
 * The board selection is done through the Arduino IDE and build system, based
 * on whatever you have selected from the Tools -> Board menu. From this info,
 * the compiler macros can tell for instance whether you have chosen "Raw HID"
 * as the Teensy++ USB type, or "Serial", or other options. However, some other
 * behaviors are based only on which external devices and sensors you have used
 * in your design (e.g. motion sensors, flex sensors, etc.) and must be manually
 * specified below.
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
        /**
         * @brief Host controller board selection
         * @see KG_BOARD_USER
         * @see KG_BOARD_TEENSYPP2_T19
         * @see KG_BOARD_TEENSYPP2_T37
         * @see KG_BOARD_ARDUINO_DUE
         * @see KG_BOARD_KEYGLOVE100
         */
        #define KG_BOARD                        KG_BOARD_TEENSYPP2_T19
        //#define KG_BOARD                        KG_BOARD_TEENSYPP2_T37
        #ifdef CORE_TEENSY_SERIAL
            /**
             * @brief Automatic USB serial host interface option based on Arduino IDE board selection
             * @see KG_HOSTIF
             */
            #define AUTO_KG_HOSTIF_USB_SERIAL   KG_HOSTIF_USB_SERIAL
        #endif
        #ifdef CORE_TEENSY_RAWHID
            /**
             * @brief Automatic USB raw HID host interface option based on Arduino IDE board selection
             * @see KG_HOSTIF
             */
            #define AUTO_KG_HOSTIF_USB_RAWHID   KG_HOSTIF_USB_RAWHID
        #endif
        #ifdef CORE_TEENSY_HID
            /**
             * @brief Automatic USB HID host interface option based on Arduino IDE board selection
             * @see KG_HOSTIF
             */
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
    #define KG_BOARD            KG_BOARD_KEYGLOVE100

#else
    // ...if you're adding support for something else here, make sure you define it by this point!
    #error No compatible board defined. This could be a problem.
#endif



// make sure these definitions exist even if they are "0"
#ifndef AUTO_KG_HOSTIF_USB_SERIAL
    /**
     * @brief Automatic USB serial host interface option based on Arduino IDE board selection
     * @see KG_HOSTIF
     */
    #define AUTO_KG_HOSTIF_USB_SERIAL 0
#endif
#ifndef AUTO_KG_HOSTIF_USB_RAWHID
    /**
     * @brief Automatic USB raw HID host interface option based on Arduino IDE board selection
     * @see KG_HOSTIF
     */
    #define AUTO_KG_HOSTIF_USB_RAWHID 0
#endif
#ifndef AUTO_KG_HOSTIF_USB_HID
    /**
     * @brief Automatic USB HID host interface option based on Arduino IDE board selection
     * @see KG_HOSTIF
     */
    #define AUTO_KG_HOSTIF_USB_HID 0
#endif



/**
 * @brief Host interface selection
 *
 * Controls which host interfaces are enabled. With an AT90USB128x (Teensy++) or
 * the SAM3X8E MCU on the Arduino Due and Keyglove Controller board, you can
 * support various kinds of USB connectivity, as well as wireless serial and HID
 * over UART through a Bluetooth module.
 *
 * @see KG_HOSTIF_USB_SERIAL
 * @see KG_HOSTIF_USB_RAWHID
 * @see KG_HOSTIF_USB_HID
 * @see KG_HOSTIF_BT2_SERIAL
 * @see KG_HOSTIF_BT2_RAWHID
 * @see KG_HOSTIF_BT2_HID
 * @see AUTO_KG_HOSTIF_USB_SERIAL
 * @see AUTO_KG_HOSTIF_USB_RAWHID
 * @see AUTO_KG_HOSTIF_USB_HID
 */
//#define KG_HOSTIF           (AUTO_KG_HOSTIF_USB_SERIAL | AUTO_KG_HOSTIF_USB_RAWHID | AUTO_KG_HOSTIF_USB_HID)    // <-- no Bluetooth
#define KG_HOSTIF           (AUTO_KG_HOSTIF_USB_SERIAL | AUTO_KG_HOSTIF_USB_RAWHID | AUTO_KG_HOSTIF_USB_HID | KG_HOSTIF_BT2_SERIAL | KG_HOSTIF_BT2_HID | KG_HOSTIF_BT2_RAWHID)

/**
 * @brief KGAPI traffic mode for USB serial interface
 *
 * Controls whether KGAPI response and event packets (outgoing) will be sent if
 * generated, and whether those interfaces will be checked for command packets
 * (incoming) from a connected host.
 */
#define KG_APIMODE_USB_SERIAL   KG_INTERFACE_MODE_OUTGOING_API | KG_INTERFACE_MODE_INCOMING_API

/**
 * @brief KGAPI traffic mode for USB serial interface
 *
 * Controls whether KGAPI response and event packets (outgoing) will be sent if
 * generated, and whether those interfaces will be checked for command packets
 * (incoming) from a connected host.
 */
#define KG_APIMODE_USB_RAWHID   KG_INTERFACE_MODE_OUTGOING_API | KG_INTERFACE_MODE_INCOMING_API

/**
 * @brief KGAPI traffic mode for Bluetooth v2 serial interface
 *
 * Controls whether KGAPI response and event packets (outgoing) will be sent if
 * generated, and whether those interfaces will be checked for command packets
 * (incoming) from a connected host.
 */
#define KG_APIMODE_BT2_SERIAL   KG_INTERFACE_MODE_OUTGOING_API | KG_INTERFACE_MODE_INCOMING_API

/**
 * @brief KGAPI traffic mode for Bluetooth v2 iAP interface
 *
 * Controls whether KGAPI response and event packets (outgoing) will be sent if
 * generated, and whether those interfaces will be checked for command packets
 * (incoming) from a connected host.
 */
#define KG_APIMODE_BT2_IAP      KG_INTERFACE_MODE_OUTGOING_API | KG_INTERFACE_MODE_INCOMING_API

/**
 * @brief KGAPI traffic mode for Bluetooth v2 raw HID interface
 *
 * Controls whether KGAPI response and event packets (outgoing) will be sent if
 * generated, and whether those interfaces will be checked for command packets
 * (incoming) from a connected host.
 */
#define KG_APIMODE_BT2_RAWHID   KG_INTERFACE_MODE_OUTGOING_API | KG_INTERFACE_MODE_INCOMING_API

/**
 * @brief HID support type selection
 *
 * Controls whether to include helper macros and functions/classes for standard
 * mouse and keyboard support.
 *
 * @see KG_HID_KEYBOARD
 * @see KG_HID_MOUSE
 */
#define KG_HID              KG_HID_MOUSE | KG_HID_KEYBOARD

/**
 * @brief Motion sensor support selection
 * @see KG_MOTION_NONE
 * @see KG_MOTION_MPU6050_HAND
 */
//#define KG_MOTION           KG_MOTION_NONE
#define KG_MOTION           KG_MOTION_MPU6050_HAND

/**
 * @brief Feedback generator selection
 * @see KG_FEEBACK_BLINK
 * @see KG_FEEBACK_PIEZO
 * @see KG_FEEBACK_RGB
 * @see KG_FEEBACK_VIBRATE
 */
//#define KG_FEEDBACK         KG_FEEDBACK_BLINK
#define KG_FEEDBACK         (KG_FEEDBACK_BLINK | KG_FEEDBACK_PIEZO | KG_FEEDBACK_VIBRATE | KG_FEEDBACK_RGB)

/**
 * @brief Dual-glove support selection (NOT IMPLEMENTED YET)
 * @see KG_DUALGLOVE_NONE
 */
#define KG_DUALGLOVE        KG_DUALGLOVE_NONE

/**
 * @brief Hand orientation selection (NOT IMPLEMENTED YET)
 * @see KG_HAND_LEFT
 * @see KG_HAND_RIGHT
 */
#define KG_HAND             KG_HAND_RIGHT

/**
 * @brief Flex sensor support selection (NOT IMPLEMENTED YET)
 * @see KG_FLEX_NONE
 * @see KG_FLEX_5FINGERS
 */
#define KG_FLEX             KG_FLEX_NONE

/**
 * @brief Pressure sensor support selection (NOT IMPLEMENTED YET)
 * @see KG_PRESSURE_NONE
 * @see KG_PRESSURE_5TIPS
 */
#define KG_PRESSURE         KG_PRESSURE_NONE



#endif // _CONFIG_H_
