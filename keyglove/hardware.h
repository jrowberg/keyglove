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

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

/** Main controller board options. Only one board may be selected at compile time. (defined in KG_BOARD) */

#define KG_BOARD_USER                   0           /* You better know what you're doing */

#define KG_BOARD_ARDUINO_DUEMILANOVE    100         /* ATMega328 + FTDI, 20 I/O */
#define KG_BOARD_ARDUINO_PRO            105         /* ATMega328, 20 I/O */
#define KG_BOARD_ARDUINO_PRO_MINI       110         /* ATMega328, 20 I/O */
#define KG_BOARD_ARDUINO_UNO            120         /* ATMega328 + ATmega8U2, 20 I/O */
#define KG_BOARD_ARDUINO_MEGA1280       130         /* ATMega1280 + FTDI, 54 I/O */
#define KG_BOARD_ARDUINO_MEGA2560       135         /* ATMega2560 + ATmega8U2, 54 I/O */
#define KG_BOARD_ARDUINO_DUE            150         /* SAM3X8E */

#define KG_BOARD_TEENSY                 200         /* AT90USB162, 22 I/O, no I2C!, 512B EEPROM, 512B SRAM */
#define KG_BOARD_TEENSY2                205         /* ATMega32U4, 25 I/O, 1kB EEPROM, 3.3kB SRAM */
#define KG_BOARD_TEENSYPP               210         /* AT90USB646, 46 I/O */
#define KG_BOARD_TEENSYPP2              215         /* AT90USB1286, 46 I/O */

#define KG_BOARD_KEYGLOVE01             301         /* AT90USB1287, 48 I/O */
#define KG_BOARD_KEYGLOVE02             302         /* AT90USB1287, 48 I/O */
#define KG_BOARD_KEYGLOVE03             303         /* AT91SAM3X8E, bzillion I/O */
#define KG_BOARD_KEYGLOVE   KG_BOARD_KEYGLOVE03     /* easy alias for current version */



/** Flex sensor options. Only one choice may be selected at the same time. (defined in KG_FLEX) */

#define KG_FLEX_NONE                    0           /* No flex sensors used */
#define KG_FLEX_FULL                    1           /* Full 5-finger complement */



/** Pressure sensor options. Only one choice may be selected at the same time. (defined in KG_PRESSURE) */

#define KG_PRESSURE_NONE                0           /* No pressure sensors used */
#define KG_PRESSURE_FULL                1           /* Full 5-fingertip complement */



/** Hand selection options. Only one choice may be selected at the same time. (defined in KG_HAND) */

#define KG_HAND_RIGHT                   1           /* Right-hand sensor connection orientation (default) */
#define KG_HAND_LEFT                    2           /* Left-hand sensor connection orientation (relevant for kit, others automatic) */



/** Dual glove communication options. Only one choice may be selected at the same time. (defined in KG_DUALGLOVE) */

#define KG_DUALGLOVE_NONE               0           /* No dual-glove support */



/** Touch sensor connection. Only one interface may be selected at a time. (defined in KG_TOUCHCONN) */

#define KG_TOUCHCONN_NONE               0           /* Don't read sensors (weird, maybe you have a reason) */
#define KG_TOUCHCONN_DIRECT             1           /* Connect sensors directly to MCU I/O pins */
#define KG_TOUCHCONN_TCA6424A           2           /* Connect sensors to 2x TI TCA6424A I2C I/O expanders */
#define KG_TOUCHCONN_PCA9505            3           /* Connect sensors to 1x NXP PCA9505 I2C I/O expander */



/** Host interface options. Multiple interfaces may be enabled at the same time. (defined in KG_HOSTIF) */

#define KG_HOSTIF_NONE                  0           /* Don't communicate (weird, maybe you have a reason) */
#define KG_HOSTIF_USB_SERIAL            1           /* Hardware USB serial (requires ARM, AT90USB* or ATMega32U* MCU) */
#define KG_HOSTIF_USB_RAWHID            2           /* Hardware USB raw HID (requires ARM, AT90USB* or ATMega32U* MCU) */
#define KG_HOSTIF_USB_HID               4           /* Hardware USB HID (requires ARM, AT90USB* or ATMega32U* MCU) */
#define KG_HOSTIF_BT2_SERIAL            8           /* Bluetooth v2 serial (requires Bluegiga WT12 w/iWRAP) */
#define KG_HOSTIF_BT2_RAWHID            16          /* Bluetooth v2 raw HID (requires Bluegiga WT12 w/iWRAP v5) */
#define KG_HOSTIF_BT2_HID               32          /* Bluetooth v2 HID (requires Bluegiga WT12 w/iWRAP) */
#define KG_HOSTIF_PS2                   1024        /* PS/2 "bit-bang" interface */
#define KG_HOSTIF_R400_HID              2048        /* R400 wireless HID (requires Logitech R400) */



/** Motion sensor options. Multiple sensors may be enabled, provided they do not compete. (defined in KG_MOTION) */

#define KG_MOTION_ADXL345               1           /* 3-axis I2C digital accelerometer */
#define KG_MOTION_ITG3200               2           /* 3-axis I2C digital gyroscope */
#define KG_MOTION_HMC5843               4           /* 3-axis I2C digital magnetometer */
#define KG_MOTION_HMC5883L              8           /* 3-axis I2C digital magnetometer (updated) */
#define KG_MOTION_MPU6050               16          /* 6-axis I2C digital accel/gyro */
#define KG_MOTION_MPU6050_HMC5883L      32          /* 6-axis I2C digital accel/gyro + 3-axis magnetometer (9-DOF) */
#define KG_MOTION_MPU6050_AK8975        64          /* 6-axis I2C digital accel/gyro + 3-axis magnetometer (9-DOF) */
#define KG_MOTION_MPU9150               128         /* 9-axis I2C digital accel/gyro/magnetometer (9-DOF) */



/** Sensor fusion options. Only one choice may be selected at a time (defined in KG_FUSION) */

#define KG_FUSION_NONE                  0           /* No fusion algorithms, only raw sensor measurements */
#define KG_FUSION_RAWIMU_6DOF           1           /* Seb Madgwick's 6-DOF IMU filter, all software */
#define KG_FUSION_RAWMARG_9DOF          2           /* Seb Madgwick's 9-DOF MARG filter, all software */
#define KG_FUSION_MPU6050_6DOF          3           /* DMP-based 6-DOF filter, all hardware */
#define KG_FUSION_MPU6050_AK8975_9ODF   4           /* DMP-based 9-DOF filter, some software */
#define KG_FUSION_MPU6050_HMC5883L_9DOF 5           /* DMP-based 9-DOF filter, some software */
#define KG_FUSION_MPU9150               6           /* DMP-based 9-DOF filter, some software */



/** Sensory feedback. Multiple options may be enabled. (defined in KG_FEEDBACK) */

#define KG_FEEDBACK_BLINK               1           /* Single LED (e.g. on Arduino/Teensy boards) */
#define KG_FEEDBACK_PIEZO               2           /* Piezo buzzer for sound */
#define KG_FEEDBACK_VIBRATE             4           /* Vibration motor for haptic feedback */
#define KG_FEEDBACK_RGB                 8           /* Combined RGB LED for visual feedback */



/** Sensory feedback connection. Only one choice may be selected at a time. (defined in KG_FEEDBACKCONN) */

#define KG_FEEDBACKCONN_NONE            0           /* Don't do anything with feedback */
#define KG_FEEDBACKCONN_DIRECT          1           /* Direct connection to I/O pins */
#define KG_FEEDBACKCONN_I2C             2           /* Custom I2C-based RGB/vibe/piezo module */



/** Debug settings. Multiple options may be enabled. (defined in KG_DEBUG) */

#define KG_DEBUG_NONE                   0
#define KG_DEBUG_BENCHMARK              1
#define KG_DEBUG_ACCELEROMETER          2
#define KG_DEBUG_GYROSCOPE              4
#define KG_DEBUG_ACCELGYRO              8
#define KG_DEBUG_MAGNETOMETER           16
#define KG_DEBUG_MOTIONFUSION           32
#define KG_DEBUG_GESTURE                64
#define KG_DEBUG_TOUCH                  128
#define KG_DEBUG_TOUCHSET               256
#define KG_DEBUG_PS2                    512
#define KG_DEBUG_USB                    1024
#define KG_DEBUG_R400                   2048
#define KG_DEBUG_IWRAP                  4096
#define KG_DEBUG_KEYBOARD               8192
#define KG_DEBUG_MOUSE                  16384
#define KG_DEBUG_JOYSTICK               32768



/* ===============================================
 * ARCHITECTURE PIN ALIASES
=============================================== */

/*
#if (defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__))
     // 32-pin ATMegaxxx MCUs
     #define SS   PB0
     #define SCLK PB1
     #define MOSI PB2
     #define MISO PB3
     #define SCL  PD0
     #define SDA  PD1
#endif

#if (defined(__AVR_AT90USB162__))
     // 32-pin AT90USB MCUs
     #define SS   PB0
     #define SCLK PB1
     #define MOSI PB2
     #define MISO PB3
     #define SCL  PD0
     #define SDA  PD1
#endif

#if (defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__))
     // 44-pin ATMEGAxxUx MCUs
     #define SS   PB0
     #define SCLK PB1
     #define MOSI PB2
     #define MISO PB3
     #define SCL  PD0
     #define SDA  PD1
#endif

#if (defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__))
     // 100-pin ATMegaxxx0 MCUs
     #define SS   PB0
     #define SCLK PB1
     #define MOSI PB2
     #define MISO PB3
     #define SCL  PD0
     #define SDA  PD1
#endif

#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) || \
     defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) || \
     defined(__AVR_ATmega32U6__))
     // 64-pin AT90USB MCUs
     #define SS   PB0
     #define SCLK PB1
     #define MOSI PB2
     #define MISO PB3
     #define SCL  PD0
     #define SDA  PD1
#endif
*/

#endif // _HARDWARE_H_


