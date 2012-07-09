// Keyglove controller source code - Overall architecture/hardware option definitions
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

#ifndef _CONFIG_H_
#define _CONFIG_H_

/* ===============================================
 * HARDWARE OPTIONS
=============================================== */

//#define KG_BOARD            KG_BOARD_ARDUINO_MEGA2560
#define KG_BOARD            KG_BOARD_TEENSYPP2
//#define KG_BOARD            KG_BOARD_KEYGLOVE

#define KG_DUALGLOVE        KG_DUALGLOVE_NONE
#define KG_FLEX             KG_FLEX_NONE
#define KG_PRESSURE         KG_PRESSURE_NONE
#define KG_EEPROM           KG_EEPROM_NONE
#define KG_SRAM             KG_SRAM_NONE
#define KG_TOUCHCONN        KG_TOUCHCONN_DIRECT

//#define KG_HOSTIF           (KG_HOSTIF_USB_SERIAL | KG_HOSTIF_USB_HID | KG_HOSTIF_BT2_SERIAL | KG_HOSTIF_BT2_HID)
#define KG_HOSTIF           (KG_HOSTIF_USB_RAWHID | KG_HOSTIF_BT2_SERIAL | KG_HOSTIF_BT2_HID)

//#define KG_MOTION           (KG_MOTION_ADXL345 | KG_MOTION_ITG3200)
//#define KG_MOTION           (KG_MOTION_ADXL345 | KG_MOTION_ITG3200 | KG_MOTION_HMC5883L)
#define KG_MOTION           KG_MOTION_MPU6050
#define KG_FUSION           KG_FUSION_NONE
//#define KG_FUSION           KG_FUSION_6DOF_MPU6050
#define KG_FEEDBACK         (KG_FEEDBACK_BLINK | KG_FEEDBACK_PIEZO | KG_FEEDBACK_VIBRATE | KG_FEEDBACK_RGB)

//#define KG_FEEDBACKCONN     KG_FEEDBACKCONN_DIRECT
#define KG_FEEDBACKCONN     KG_FEEDBACKCONN_I2C



/* ===============================================
 * DEBUG SETTINGS
=============================================== */

#define KG_DEBUG            KG_DEBUG_NONE

/*
Debug options:
  KG_DEBUG_NONE
  KG_DEBUG_BENCHMARK
  KG_DEBUG_ACCELEROMETER
  KG_DEBUG_GYROSCOPE
  KG_DEBUG_ACCELGYRO
  KG_DEBUG_MAGNETOMETER
  KG_DEBUG_MOTIONFUSION
  KG_DEBUG_GESTURE
  KG_DEBUG_TOUCH
  KG_DEBUG_TOUCHSET
  KG_DEBUG_PS2
  KG_DEBUG_USB
  KG_DEBUG_R400
  KG_DEBUG_IWRAP
  KG_DEBUG_KEYBOARD
  KG_DEBUG_MOUSE
  KG_DEBUG_JOYSTICK
*/

#endif // _CONFIG_H_


