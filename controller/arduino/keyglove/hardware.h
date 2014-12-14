// Keyglove controller source code - Modular hardware/architecture option definitions
// 2014-12-13 by Jeff Rowberg <jeff@rowberg.net>

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
 * @file hardware.h
 * @brief Modular hardware/architecture option definitions
 * @author Jeff Rowberg
 * @date 2014-12-13
 *
 * This file defines all of the selectable options which you may want to use in
 * the "config.h" file. If you create new functionality which a modular add-on
 * beyond the already implemented Keyglove code, it should be defined here as a
 * new option and then enabled as a module in the "config.h" file. This file
 * will not normally need to be modified.
 *
 * @see config.h
 */

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

/* Main controller board options. Only one board may be selected at compile time. (defined in KG_BOARD) */

#define KG_BOARD_USER                   0x00        ///< Totally custom board, you better know what you're doing
#define KG_BOARD_TEENSYPP2_T37          0x10        ///< AT90USB128x MCU, 46 I/O pins and prototype/kit design, 37 touch sensors
#define KG_BOARD_TEENSYPP2_T19          0x11        ///< AT90USB128x MCU, 46 I/O pins and prototype/kit design, 19 touch sensors
#define KG_BOARD_ARDUINO_DUE            0x20        ///< **NOT SUPPORTED YET:** SAM3X8E MCU, 50+ I/O pins and prototype/Arduino design
#define KG_BOARD_KEYGLOVE100            0x30        ///< **NOT SUPPORTED YET:** SAM3X8E MCU, 50+ I/O pins and custom design



/* Flex sensor options. Only one choice may be selected at the same time. (defined in KG_FLEX) */

#define KG_FLEX_NONE                    0x00        ///< No flex sensors used
#define KG_FLEX_5FINGERS                0x01        ///< **NOT SUPPORTED YET:** Full 5-finger complement



/* Pressure sensor options. Only one choice may be selected at the same time. (defined in KG_PRESSURE) */

#define KG_PRESSURE_NONE                0x00        ///< No pressure sensors used
#define KG_PRESSURE_5TIPS               0x01        ///< **NOT SUPPORTED YET:** Full 5-fingertip complement



/* Hand selection options. Only one choice may be selected at the same time. (defined in KG_HAND) */

#define KG_HAND_RIGHT                   0x01        ///< Right-hand sensor connection orientation (default)
#define KG_HAND_LEFT                    0x02        ///< Left-hand sensor connection orientation (relevant for kit, others automatic)



/* Dual glove communication options. Only one choice may be selected at the same time. (defined in KG_DUALGLOVE) */

#define KG_DUALGLOVE_NONE               0x00        ///< No dual-glove support
#define KG_DUALGLOVE_RFM22B             0x01        ///< **NOT SUPPORTED YET:** Wireless connection with RFM22B



/* Host interface options. Multiple interfaces may be enabled at the same time. (defined in KG_HOSTIF) */

#define KG_HOSTIF_NONE                  0x00        ///< Don't communicate (weird, maybe you have a reason)
#define KG_HOSTIF_USB_SERIAL            0x01        ///< Hardware USB serial (requires ARM, AT90USB* or ATMega32U* MCU)
#define KG_HOSTIF_USB_RAWHID            0x02        ///< Hardware USB raw HID (requires ARM, AT90USB* or ATMega32U* MCU)
#define KG_HOSTIF_USB_HID               0x04        ///< Hardware USB HID (requires ARM, AT90USB* or ATMega32U* MCU)
#define KG_HOSTIF_BT2_SERIAL            0x08        ///< Bluetooth v2 serial (requires Bluegiga WT12 w/iWRAP)
#define KG_HOSTIF_BT2_RAWHID            0x10        ///< Bluetooth v2 raw HID (requires Bluegiga WT12 w/iWRAP v5+)
#define KG_HOSTIF_BT2_HID               0x20        ///< Bluetooth v2 HID (requires Bluegiga WT12 w/iWRAP)



/* HID support options. Multiple HID types may be enabled at the same time. (defined in KG_HID) */

#define KG_HID_NONE                     0x00        ///< No standard HID support
#define KG_HID_KEYBOARD                 0x01        ///< Include standard HID keyboard support routines
#define KG_HID_MOUSE                    0x02        ///< Include standard HID mouse support routines



/* Motion sensor options. Multiple sensors may be enabled, provided they do not compete. (defined in KG_MOTION) */

#define KG_MOTION_NONE                  0x00        ///< No motion support
#define KG_MOTION_MPU6050_HAND          0x01        ///< 6-axis I2C digital accel/gyro on back of hand
#define KG_MOTION_MPU6050_INDEXTIP      0x02        ///< 6-axis I2C digital accel/gyro on index fingertip



/* Sensory feedback. Multiple options may be enabled. (defined in KG_FEEDBACK) */

#define KG_FEEDBACK_NONE                0x00        ///< No feedback support
#define KG_FEEDBACK_BLINK               0x01        ///< Single LED (e.g. on Arduino/Teensy boards)
#define KG_FEEDBACK_PIEZO               0x02        ///< Piezo buzzer for sound
#define KG_FEEDBACK_VIBRATE             0x04        ///< Vibration motor for haptic feedback
#define KG_FEEDBACK_RGB                 0x08        ///< Combined RGB LED for visual feedback



/* Interface mode definitions. Multiple options may be enabled. */

#define KG_INTERFACE_MODE_NONE          0x00        ///< Don't use this interface for KGAPI data
#define KG_INTERFACE_MODE_OUTGOING_API  0x01        ///< Use this interface to send outgoing KGAPI responses and events
#define KG_INTERFACE_MODE_INCOMING_API  0x02        ///< Use this interface to receive incoming KGAPI commands
#define KG_INTERFACE_MODE_OUTGOING_LOG  0x04        ///< Use this interface to send outgoing log/info data



/* Interface number definitions. Used for differentiating between possible incoming/outgoing channels for KGAPI packets. */

#define KG_INTERFACENUM_USB_SERIAL      1           ///< KGAPI interface identifier for USB serial 
#define KG_INTERFACENUM_USB_RAWHID      2           ///< KGAPI interface identifier for USB raw HID
#define KG_INTERFACENUM_BT2_SERIAL      3           ///< KGAPI interface identifier for BT2 serial
#define KG_INTERFACENUM_BT2_RAWHID      4           ///< KGAPI interface identifier for BT2 raw HID
#define KG_INTERFACENUM_BT2_IAP         5           ///< KGAPI interface identifier for BT2 IAP

#endif // _HARDWARE_H_
