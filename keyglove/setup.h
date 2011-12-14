// Keyglove controller source code - Global variables and initialization functions
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



#ifndef _SETUP_H_
#define _SETUP_H_

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

#define KG_PACKET_TYPE_REPORT_ACCEL         0x11
#define KG_PACKET_TYPE_REPORT_GYRO          0x27

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



/* ===============================================
 * MAIN BOARD SELECTION
=============================================== */

#if (KG_BOARD == KG_BOARD_ARDUINO_DUEMILANOVE) || (KG_BOARD == KG_BOARD_ARDUINO_PRO) || \
    (KG_BOARD == KG_BOARD_ARDUINO_PRO_MINI) || (KG_BOARD == KG_BOARD_ARDUINO_UNO)
    #include "setup_board_atmega328.h"
    #define MAX_ONBOARD_IO_PINS 20
    #define NO_USB_SUPPORT // could be a problem
#endif

#if (KG_BOARD == KG_BOARD_ARDUINO_MEGA1280) || (KG_BOARD == KG_BOARD_ARDUINO_MEGA2560)
    #include "setup_board_atmega1280.h" // still applies for 2560
    #define MAX_ONBOARD_IO_PINS 54
    #define NO_USB_SUPPORT // could be a problem
#endif

#if (KG_BOARD == KG_BOARD_TEENSY)
    #include "setup_board_at90usb.h"
    #define MAX_ONBOARD_IO_PINS 22
    #define NO_I2C_SUPPORT // could be a *big* problem
#endif

#if (KG_BOARD == KG_BOARD_TEENSY2)
    #include "setup_board_atmega32u.h"
    #define MAX_ONBOARD_IO_PINS 25
#endif

#if (KG_BOARD == KG_BOARD_TEENSYPP) || (KG_BOARD == KG_BOARD_TEENSYPP2)
    #include "setup_board_at90usb.h"
    #define MAX_ONBOARD_IO_PINS 46
#endif

#if (KG_BOARD == KG_BOARD_KEYGLOVE)
    #include "setup_board_at90usb.h"
    #define MAX_ONBOARD_IO_PINS 48
#endif



/* ===============================================
 * EXTERNAL EEPROM
=============================================== */

#if (KG_EEPROM == KG_EEPROM_25LC512)
    #define ENABLE_EEPROM
    #include "setup_eeprom_25lc512.h"
#endif



/* ===============================================
 * EXTERNAL SRAM
=============================================== */

#if (KG_SRAM == KG_SRAM_23K256)
    #define ENABLE_SRAM
    #include "setup_sram_23k256.h"
#endif



/* ===============================================
 * TOUCH SENSOR CONNECTION
=============================================== */
#if (KG_TOUCHCONN == KG_TOUCHCONN_DIRECT)
    #include "setup_touchconn_direct.h"
#endif

#if (KG_TOUCHCONN == KG_TOUCHCONN_TCA6424A)
    #define ENABLE_TOUCHCONN
    #include "setup_touchconn_tca6424a.h"
#endif



/* ===============================================
 * HOST INTERFACE SETUP
=============================================== */

#if (KG_HOSTIF & KG_HOSTIF_SERIAL)
    #define ENABLE_SERIAL
#endif

#if (KG_HOSTIF & KG_HOSTIF_PS2)
    #define ENABLE_PS2
    #include "setup_hostif_ps2.h"
#endif

#if (KG_HOSTIF & KG_HOSTIF_USB)
    #ifdef NO_USB_SUPPORT
        #error Selected board has no USB support. Hardware USB cannot be enabled with this board.
    #endif
    #define ENABLE_USB
    #include "setup_hostif_usb.h"
#endif

#if (KG_HOSTIF & KG_HOSTIF_BT_HID)
    #define ENABLE_BLUETOOTH
    #include "setup_hostif_bt_hid.h"
#endif

#if (KG_HOSTIF & KG_HOSTIF_BT_SPP)
    #ifndef ENABLE_BLUETOOTH
        #define ENABLE_BLUETOOTH
    #endif
    #include "setup_hostif_bt_spp.h"
#endif

#if (KG_HOSTIF & KG_HOSTIF_R400)
    #define ENABLE_R400
    #include "setup_hostif_r400.h"
#endif


/* ===============================================
 * MOTION SENSOR SETUP
=============================================== */

#include "helper_3dmath.h"

#if (KG_MOTION & KG_MOTION_ADXL345)
    #define ENABLE_ACCELEROMETER // used in main loop()
    #include "setup_motion_adxl345.h"
#endif

#if (KG_MOTION & KG_MOTION_ITG3200)
    #define ENABLE_GYROSCOPE // used in main loop()
    #include "setup_motion_itg3200.h"
#endif

#if (KG_MOTION & KG_MOTION_HMC5843)
    #define ENABLE_MAGNETOMETER // used in main loop()
    #include "setup_motion_hmc5843.h"
#endif

#if (KG_MOTION & KG_MOTION_HMC5883L)
    #if (KG_MOTION & KG_MOTION_HMC5843)
        #error Cannot use both HMC5843 and HMC5883L (competing magnetometers)
    #endif
    #define ENABLE_MAGNETOMETER // used in main loop()
    #include "setup_motion_hmc5883l.h"
#endif

#if (KG_MOTION & KG_MOTION_MPU6050)
    #if (KG_MOTION & KG_MOTION_ADXL345)
        #error Cannot use both ADXL345 and MPU-6050 (competing accelerometers)
    #endif
    #if (KG_MOTION & KG_MOTION_ITG3200)
        #error Cannot use both ITG3200 and MPU-6050 (competing gyroscopes)
    #endif
    #define ENABLE_FUSION // used in main loop()
    #include "setup_motion_mpu6050.h"
#endif

#if (KG_MOTION & KG_MOTION_MPU6050_HMC5883L)
    #if (KG_MOTION & KG_MOTION_ADXL345)
        #error Cannot use both ADXL345 and MPU-6050 (competing accelerometers)
    #endif
    #if (KG_MOTION & KG_MOTION_ITG3200)
        #error Cannot use both ITG3200 and MPU-6050 (competing gyroscopes)
    #endif
    #if (KG_MOTION & KG_MOTION_HMC5843)
        #error Cannot use both HMC5843 and InvenSense MotionFusion (HMC5843 should be connected to aux SDA/SCL of MPU-6050)
    #endif
    #if (KG_MOTION & KG_MOTION_HMC5883L)
        #error Cannot use both HMC5843 and InvenSense MotionFusion (HMC5883L should be connected to aux SDA/SCL of MPU-6050)
    #endif
    #define ENABLE_FUSION // used in main loop()
    #define MOTIONFUSION_9DOF_HMC5883
    #include "setup_motion_mpu6050.h"
#endif

#if defined(ENABLE_ACCELEROMETER) && defined(ENABLE_GYROSCOPE)
    #include "setup_motion_rawimu.h"
#endif

/* ===============================================
 * FEEDBACK SETUP
=============================================== */

#if (KG_FEEDBACK & KG_FEEDBACK_BLINK)
    #include "setup_feedback_blink.h"
#endif

#if (KG_FEEDBACK & KG_FEEDBACK_PIEZO)
    #include "setup_feedback_piezo.h"
#endif

#if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE)
    #include "setup_feedback_vibrate.h"
#endif

#if (KG_FEEDBACK & KG_FEEDBACK_RGB)
    #include "setup_feedback_rgb.h"
#endif

/* ===============================================
 * HOST INTERFACE SETUP PART 2, SERIAL (INCLUDE ORDER MATTERS)
=============================================== */

#if (KG_HOSTIF & KG_HOSTIF_SERIAL)
    #include "setup_hostif_serial.h"
#endif



/* ===============================================
 * CONSTANT INCLUDES
=============================================== */
#include "setup_keyboard.h"
#include "setup_mouse.h"
#include "setup_joystick.h"
#include "setup_touch.h"



/* ===============================================
 * MAIN INIT/SETUP FUNCTION (called by setup())
=============================================== */

void keyglove_setup() {
    // single-option controller board
    #if (KG_BOARD > 0)
        setup_board();
    #endif

    /* ===============================================
     * INITIALIZE SERIAL DEBUGGING AT THIS POINT
    =============================================== */
    #ifdef ENABLE_SERIAL
        Serial.begin(38400);
        Serial.println("info Keyglove device activated");
    #endif

    // single-option external EEPROM
    #if (KG_EEPROM > 0)
        setup_eeprom();
    #endif

    // single-option external SRAM
    #if (KG_SRAM > 0)
        setup_sram();
    #endif

    // single-option touch sensor connection
    #if (KG_TOUCHCONN > 0)
        setup_touchconn();
    #endif

    // multi-option host interface
    #if (KG_HOSTIF & KG_HOSTIF_SERIAL)
        setup_hostif_serial();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_PS2)
        setup_hostif_ps2();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_USB)
        setup_hostif_usb();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_BT_HID) || (KG_HOSTIF & KG_HOSTIF_BT_SPP)
        setup_hostif_bt();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_BT_HID)
        setup_hostif_bt_hid();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_BT_SPP)
        setup_hostif_bt_spp();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_R400)
        setup_hostif_r400();
    #endif
    
    // multi-option motion sensors
    #ifdef ENABLE_ACCELEROMETER
        setup_motion_accelerometer();
    #endif
    #ifdef ENABLE_GYROSCOPE
        setup_motion_gyroscope();
    #endif
    #ifdef ENABLE_MAGNETOMETER
        setup_motion_magnetometer();
    #endif
    #ifdef ENABLE_FUSION
        setup_motion_fusion();
    #endif
    #if defined(ENABLE_ACCELEROMETER) && defined(ENABLE_GYROSCOPE)
        setup_motion();
    #endif

    // multi-option feedback
    #if (KG_FEEDBACK & KG_FEEDBACK_BLINK)
        setup_feedback_blink();
    #endif
    #if (KG_FEEDBACK & KG_FEEDBACK_PIEZO)
        setup_feedback_piezo();
    #endif
    #if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE)
        setup_feedback_vibrate();
    #endif
    #if (KG_FEEDBACK & KG_FEEDBACK_RGB)
        setup_feedback_rgb();
    #endif
    
    setup_touch();
    setup_keyboard();
    setup_mouse();
    setup_joystick();
}

#endif // _SETUP_H_


