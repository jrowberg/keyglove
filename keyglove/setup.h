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



#ifndef _SUPPORT_H_
#define _SUPPORT_H_

// firmware version
#include "version.h"

// hardware constant option definitions
#include "hardware.h"

// minimal Arduino support wrapper if we're outside of the Arduino environment
#ifndef ARDUINO
    #include "ArduinoWrapper.h"
#endif



/* ===============================================
 * DEBUG COMPILER MACROS
=============================================== */

#if (KG_DEBUG & KG_DEBUG_BENCHMARK)
    #define DEBUG_BENCHMARK(x)          x
#else
    #define DEBUG_BENCHMARK(x)
#endif

#if (KG_DEBUG & KG_DEBUG_ACCELEROMETER)
    #define DEBUG_ACCELEROMETER(x)      x
#else
    #define DEBUG_ACCELEROMETER(x)
#endif

#if (KG_DEBUG & KG_DEBUG_GYROSCOPE)
    #define DEBUG_GYROSCOPE(x)          x
#else
    #define DEBUG_GYROSCOPE(x)
#endif

#if (KG_DEBUG & KG_DEBUG_ACCELGYRO)
    #define DEBUG_ACCELGYRO(x)          x
#else
    #define DEBUG_ACCELGYRO(x)
#endif

#if (KG_DEBUG & KG_DEBUG_MAGNETOMETER)
    #define DEBUG_MAGNETOMETER(x)       x
#else
    #define DEBUG_MAGNETOMETER(x)
#endif

#if (KG_DEBUG & KG_DEBUG_MOTIONFUSION)
    #define DEBUG_MOTIONFUSION(x)       x
#else
    #define DEBUG_MOTIONFUSION(x)
#endif

#if (KG_DEBUG & KG_DEBUG_GESTURE)
    #define DEBUG_GESTURE(x)            x
#else
    #define DEBUG_GESTURE(x)
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

#if (KG_DEBUG & KG_DEBUG_PS2)
    #define DEBUG_PS2(x)                x
#else
    #define DEBUG_PS2(x)
#endif

#if (KG_DEBUG & KG_DEBUG_USB)
    #define DEBUG_USB(x)                x
#else
    #define DEBUG_USB(x)
#endif

#if (KG_DEBUG & KG_DEBUG_R400)
    #define DEBUG_R400(x)               x
#else
    #define DEBUG_R400(x)
#endif

#if (KG_DEBUG & KG_DEBUG_IWRAP)
    #define DEBUG_IWRAP(x)              x
#else
    #define DEBUG_IWRAP(x)
#endif

#if (KG_DEBUG & KG_DEBUG_KEYBOARD)
    #define DEBUG_KEYBOARD(x)           x
#else
    #define DEBUG_KEYBOARD(x)
#endif

#if (KG_DEBUG & KG_DEBUG_MOUSE)
    #define DEBUG_MOUSE(x)              x
#else
    #define DEBUG_MOUSE(x)
#endif

#if (KG_DEBUG & KG_DEBUG_JOYSTICK)
    #define DEBUG_JOYSTICK(x)           x
#else
    #define DEBUG_JOYSTICK(x)
#endif



/* ===============================================
 * MAIN BOARD SELECTION
=============================================== */

#if (KG_BOARD == KG_BOARD_ARDUINO_DUEMILANOVE) || (KG_BOARD == KG_BOARD_ARDUINO_PRO) || \
    (KG_BOARD == KG_BOARD_ARDUINO_PRO_MINI) || (KG_BOARD == KG_BOARD_ARDUINO_UNO)
    #include "support_board_arduino_uno.h"
    #define MAX_ONBOARD_IO_PINS 20
    #define NO_USB_SUPPORT // could be a problem
#endif

#if (KG_BOARD == KG_BOARD_ARDUINO_MEGA1280) || (KG_BOARD == KG_BOARD_ARDUINO_MEGA2560)
    #include "support_board_arduio_mega1280.h" // still applies for 2560
    #define MAX_ONBOARD_IO_PINS 54
    #define NO_USB_SUPPORT // could be a problem
#endif

#if (KG_BOARD == KG_BOARD_TEENSY)
    #include "support_board_teensy.h"
    #define MAX_ONBOARD_IO_PINS 22
    #define NO_I2C_SUPPORT // could be a *big* problem
#endif

#if (KG_BOARD == KG_BOARD_TEENSY2)
    #include "support_board_teensy2.h"
    #define MAX_ONBOARD_IO_PINS 25
#endif

#if (KG_BOARD == KG_BOARD_TEENSYPP) || (KG_BOARD == KG_BOARD_TEENSYPP2)
    #include "support_board_teensypp2.h"
    #define MAX_ONBOARD_IO_PINS 46
#endif

#if (KG_BOARD == KG_BOARD_KEYGLOVE)
    #include "support_board_at90usb.h"
    #define MAX_ONBOARD_IO_PINS 48
#endif



/* ===============================================
 * HOST INTERFACE PRELIMINARY SETUP
=============================================== */

#if (KG_HOSTIF > 0)
    // every major host interface must support this protocol
    #include "support_hostif_protocol_declarations.h"
#endif



/* ===============================================
 * EXTERNAL EEPROM
=============================================== */

#if (KG_EEPROM == KG_EEPROM_25LC512)
    #define ENABLE_EEPROM
    #include "support_eeprom_25lc512.h"
#endif



/* ===============================================
 * EXTERNAL SRAM
=============================================== */

#if (KG_SRAM == KG_SRAM_23K256)
    #define ENABLE_SRAM
    #include "support_sram_23k256.h"
#endif



/* ===============================================
 * TOUCH SENSOR CONNECTION
=============================================== */

#if (KG_TOUCHCONN == KG_TOUCHCONN_DIRECT)
    #define ENABLE_TOUCH
    #include "support_touchconn_direct.h"
#endif

#if (KG_TOUCHCONN == KG_TOUCHCONN_TCA6424A)
    #define ENABLE_TOUCH
    #include "support_touchconn_tca6424a.h"
#endif



/* ===============================================
 * FEEDBACK CONNECTION
=============================================== */
#if (KG_FEEDBACKCONN == KG_FEEDBACKCONN_DIRECT)
    #include "support_feedbackconn_direct.h"
#endif

#if (KG_FEEDBACKCONN == KG_FEEDBACKCONN_I2C)
    #include "support_feedbackconn_i2c.h"
#endif



/* ===============================================
 * FEEDBACK SETUP
=============================================== */

#if (KG_FEEDBACK & KG_FEEDBACK_BLINK)
    #include "support_feedback_blink.h"
#endif

#if (KG_FEEDBACK & KG_FEEDBACK_PIEZO)
    #include "support_feedback_piezo.h"
#endif

#if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE)
    #include "support_feedback_vibrate.h"
#endif

#if (KG_FEEDBACK & KG_FEEDBACK_RGB)
    #include "support_feedback_rgb.h"
#endif



/* ===============================================
 * MOTION SENSOR SETUP
=============================================== */

#include "helper_3dmath.h"

#if (KG_MOTION & KG_MOTION_ADXL345)
    #define ENABLE_ACCELEROMETER // used in main loop()
    #include "support_motion_adxl345.h"
#endif

#if (KG_MOTION & KG_MOTION_ITG3200)
    #define ENABLE_GYROSCOPE // used in main loop()
    #include "support_motion_itg3200.h"
#endif

#if (KG_MOTION & KG_MOTION_HMC5843)
    #define ENABLE_MAGNETOMETER // used in main loop()
    #include "support_motion_hmc5843.h"
#endif

#if (KG_MOTION & KG_MOTION_HMC5883L)
    #if (KG_MOTION & KG_MOTION_HMC5843)
        #error Cannot use both HMC5843 and HMC5883L (competing magnetometers)
    #endif
    #define ENABLE_MAGNETOMETER // used in main loop()
    #include "support_motion_hmc5883l.h"
#endif

#if (KG_MOTION & KG_MOTION_MPU6050)
    #if (KG_MOTION & KG_MOTION_ADXL345)
        #error Cannot use both ADXL345 and MPU-6050 (competing accelerometers)
    #endif
    #if (KG_MOTION & KG_MOTION_ITG3200)
        #error Cannot use both ITG3200 and MPU-6050 (competing gyroscopes)
    #endif
    #define ENABLE_ACCELGYRO // used in main loop()
    #include "support_motion_mpu6050.h"
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
        #error Cannot use both HMC5883l and InvenSense MotionFusion (HMC5883L should be connected to aux SDA/SCL of MPU-6050)
    #endif
    #define ENABLE_ACCELGYRO // used in main loop()
    #include "support_motion_mpu6050.h"
#endif

#if (KG_MOTION & KG_MOTION_MPU6050_AK8975)
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
    #if (KG_MOTION & KG_MOTION_MPU9150)
        #error Cannot use both MPU-6050 and MPU-9150 (competing accelerometers and gyroscopes)
    #endif
    #define ENABLE_ACCELGYRO // used in main loop()
    #include "support_motion_mpu6050.h"
#endif

#if (KG_MOTION & KG_MOTION_MPU9150)
    #if (KG_MOTION & KG_MOTION_ADXL345)
        #error Cannot use both ADXL345 and MPU-6050 (competing accelerometers)
    #endif
    #if (KG_MOTION & KG_MOTION_ITG3200)
        #error Cannot use both ITG3200 and MPU-6050 (competing gyroscopes)
    #endif
    #if (KG_MOTION & KG_MOTION_MPU6050)
        #error Cannot use both MPU-6050 and MPU-9150 (competing accelerometers and gyroscopes)
    #endif
    #if (KG_MOTION & KG_MOTION_HMC5843)
        #error Cannot use both HMC5843 and InvenSense MotionFusion (HMC5843 should be connected to aux SDA/SCL of MPU-6050)
    #endif
    #if (KG_MOTION & KG_MOTION_HMC5883L)
        #error Cannot use both HMC5843 and InvenSense MotionFusion (HMC5883L should be connected to aux SDA/SCL of MPU-6050)
    #endif
    #define ENABLE_ACCELGYRO // used in main loop()
    #include "support_motion_mpu9150.h"
#endif

#if (KG_FUSION == KG_FUSION_RAWIMU_6DOF)
    #include "support_motion_rawimu_6dof.h"
#elif (KG_FUSION == KG_FUSION_RAWMARG_9DOF)
    #include "support_motion_rawmarg_9dof.h"
#endif



/* ===============================================
 * HOST INTERFACE SETUP
=============================================== */

#if (KG_HOSTIF & KG_HOSTIF_PS2)
    #define ENABLE_PS2
    #include "support_hostif_ps2.h"
#endif

#if (KG_HOSTIF & KG_HOSTIF_USB_SERIAL)
    #ifdef NO_USB_SUPPORT
        #error Selected board has no USB support. Hardware USB cannot be enabled with this board.
    #endif
    #include "support_hostif_usb_serial.h"
#endif

#if (KG_HOSTIF & KG_HOSTIF_USB_RAWHID)
    #ifdef NO_USB_SUPPORT
        #error Selected board has no USB support. Hardware USB cannot be enabled with this board.
    #endif
    #include "support_hostif_usb_rawhid.h"
#endif

#if (KG_HOSTIF & KG_HOSTIF_USB_HID)
    #ifdef NO_USB_SUPPORT
        #error Selected board has no USB support. Hardware USB cannot be enabled with this board.
    #endif
    #include "support_hostif_usb_hid.h"
#endif

#if (KG_HOSTIF & KG_HOSTIF_BT2_SERIAL)
    #include "support_hostif_bt2_serial.h"
#endif

#if (KG_HOSTIF & KG_HOSTIF_BT2_RAWHID)
    #include "support_hostif_bt2_rawhid.h"
#endif

#if (KG_HOSTIF & KG_HOSTIF_BT2_HID)
    #include "support_hostif_bt2_hid.h"
#endif

#if (KG_HOSTIF & KG_HOSTIF_R400)
    #define ENABLE_R400
    #include "support_hostif_r400.h"
#endif

#if ((KG_HOSTIF & KG_HOSTIF_USB_HID) || (KG_HOSTIF & KG_HOSTIF_BT2_HID))
    #include "support_keyboard.h"
    #include "support_mouse.h"
    #include "support_joystick.h"
#endif



/* ===============================================
 * TOUCH DETECTION SETUP
=============================================== */

#if (KG_TOUCHCONN > 0)
    #include "support_touch.h"
    #include "touchset.h"
#endif



/* ===============================================
 * HOST INTERFACE SETUP PART 2, SERIAL (INCLUDE ORDER MATTERS)
=============================================== */

#if (KG_HOSTIF > 0)
    // every major host interface must support this protocol
    #include "support_hostif_protocol.h"
#endif

#if (KG_HOSTIF & KG_HOSTIF_SERIAL)
    #include "support_hostif_serial.h"
#endif



/* ===============================================
 * MAIN INIT/SETUP FUNCTION (called by setup())
=============================================== */

void keyglove_setup() {
    // single-option controller board
    #if (KG_BOARD > 0)
        setup_board();
    #endif

    // initialize host interfaces
    #if ((KG_HOSTIF & KG_HOSTIF_USB_SERIAL) || (KG_HOSTIF & KG_HOSTIF_USB_HID))
        setup_hostif_usb();
        #if (KG_HOSTIF & KG_HOSTIF_USB_SERIAL)
            setup_hostif_usb_serial();
        #endif
        #if (KG_HOSTIF & KG_HOSTIF_USB_HID)
            setup_hostif_usb_hid();
        #endif
    #endif
    #if ((KG_HOSTIF & KG_HOSTIF_BT2_SERIAL) || (KG_HOSTIF & KG_HOSTIF_BT2_HID))
        setup_hostif_bt2();
        #if (KG_HOSTIF & KG_HOSTIF_BT2_SERIAL)
            setup_hostif_bt2_serial();
        #endif
        #if (KG_HOSTIF & KG_HOSTIF_BT2_HID)
            setup_hostif_bt2_hid();
        #endif
    #endif

    // initialize I2C transceiver and timing
    #ifndef NO_I2C_SUPPORT
        Wire.begin();
        TWBR = 32; // set 100kHz mode
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
    #if (KG_HOSTIF & KG_HOSTIF_PS2)
        setup_hostif_ps2();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_USB_SERIAL) || (KG_HOSTIF & KG_HOSTIF_USB_RAWHID) || (KG_HOSTIF & KG_HOSTIF_USB_HID)
        setup_hostif_usb();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_USB_SERIAL)
        setup_hostif_usb_serial();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_USB_RAWHID)
        setup_hostif_usb_rawhid();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_USB_HID)
        setup_hostif_usb_hid();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_BT2_SERIAL) || (KG_HOSTIF & KG_HOSTIF_BT2_RAWHID) || (KG_HOSTIF & KG_HOSTIF_BT2_HID)
        setup_hostif_bt2();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_BT2_SERIAL)
        setup_hostif_bt2_serial();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_BT2_RAWHID)
        setup_hostif_bt2_rawhid();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_BT2_HID)
        setup_hostif_bt2_hid();
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
    #ifdef ENABLE_ACCELGYRO
        setup_motion_accelgyro();
    #endif
    #ifdef ENABLE_MAGNETOMETER
        setup_motion_magnetometer();
    #endif
    #ifdef ENABLE_FUSION
        setup_motion_fusion();
    #endif

    // multi-option feedback
    #if (KG_FEEDBACKCONN > 0)
        setup_feedbackconn();
    #endif
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

    send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Keyglove ready!"));
}

#endif // _SUPPORT_H_


