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

#define KG_BOARD_TEENSY                 200         /* AT90USB162, 22 I/O, no I2C!, 512B EEPROM, 512B SRAM */
#define KG_BOARD_TEENSY2                205         /* ATMega32U4, 25 I/O, 1kB EEPROM, 3.3kB SRAM */
#define KG_BOARD_TEENSYPP               210         /* AT90USB646, 46 I/O */
#define KG_BOARD_TEENSYPP2              215         /* AT90USB1286, 46 I/O */

#define KG_BOARD_KEYGLOVE01             301         /* AT90USB1287, 48 I/O */
#define KG_BOARD_KEYGLOVE02             302         /* AT90USB1287, 48 I/O */
#define KG_BOARD_KEYGLOVE03             303         /* AT90USB1287, 48 I/O + 2x I2C expanders w/48 more */
#define KG_BOARD_KEYGLOVE   KG_BOARD_KEYGLOVE02     /* easy alias for current version */



/** External EEPROM options. Only one choice may be selected at the same time. (defined in KG_EEPROM) */

#define KG_EEPROM_NONE                  0           /* Use AVR's internal EEPROM only */
#define KG_EEPROM_25LC512               1           /* Microchip 25LC512 512kbit */



/** External SRAM options. Only one choice may be selected at the same time. (defined in KG_SRAM) */

#define KG_SRAM_NONE                    0           /* Use AVR's internal SRAM only */
#define KG_SRAM_23K256                  1           /* Microchip 23K256 256kbit */



/** Flex sensor options. Only one choice may be selected at the same time. (defined in KG_FLEX) */

#define KG_FLEX_NONE                    0           /* No flex sensors used */



/** Pressure sensor options. Only one choice may be selected at the same time. (defined in KG_PRESSURE) */

#define KG_PRESSURE_NONE                0           /* No pressure sensors used */



/** Dual glove communication options. Only one choice may be selected at the same time. (defined in KG_DUALGLOVE) */

#define KG_DUALGLOVE_NONE               0           /* No dual-glove support */



/** Touch sensor connection. Only one interface may be selected at a time. (defined in KG_TOUCHCONN) */

#define KG_TOUCHCONN_NONE               0           /* Don't read sensors (weird, maybe you have a reason) */
#define KG_TOUCHCONN_DIRECT             1           /* Connect sensors directly to MCU I/O pins */
#define KG_TOUCHCONN_TCA6424A           2           /* Connect sensors to 2x TI TCA6424A I2C I/O expanders */



/** Host interface options. Multiple interfaces may be enabled at the same time. (defined in KG_HOSTIF) */

#define KG_HOSTIF_NONE                  0           /* Don't communicate (weird, maybe you have a reason) */
#define KG_HOSTIF_SERIAL                1           /* Hardware UART (Arduino's "Serial" object) */
#define KG_HOSTIF_PS2                   2           /* PS/2 "bit-bang" interface */
#define KG_HOSTIF_USB                   4           /* Hardware USB (requires AT90USB* or ATMega32U* MCU) */
#define KG_HOSTIF_BT_HID                8           /* Bluetooth HID interface (requires Bluegiga WT12 w/iWRAP) */
#define KG_HOSTIF_BT_SPP                16          /* Bluetooth SPP interface (requires Bluegiga WT12 w/iWRAP) */
#define KG_HOSTIF_R400                  32          /* R400 wireless (requires Logitech R400) */



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
#define KG_FUSION_MPU6050_AK8975_9ODF   4           /* DMP-based 9-DOF filter, all hardware (?) */
#define KG_FUSION_MPU6050_HMC5883L_9DOF 5           /* DMP-based 9-DOF filter, all hardware (?) */
#define KG_FUSION_MPU9150               6           /* DMP-based 9-DOF filter, all hardware */



/** Sensory feedback. Multiple options may be enabled. (defined in KG_FEEDBACK) */

#define KG_FEEDBACK_BLINK               1           /* Single LED (e.g. on Arduino/Teensy boards) */
#define KG_FEEDBACK_PIEZO               2           /* Piezo buzzer for sound */
#define KG_FEEDBACK_VIBRATE             4           /* Vibration motor for haptic feedback */
#define KG_FEEDBACK_RGB                 8           /* Combined RGB LED for visual feedback */



/** Sensory feedback connection. Only one choice may be selected at a time. (defined in KG_FEEDBACKCONN) */

#define KG_FEEDBACKCONN_NONE            0           /* Don't do anything with feedback */
#define KG_FEEDBACKCONN_DIRECT          1           /* Direct connection to I/O pins */
#define KG_FEEDBACKCONN_I2C             2           /* Custom I2C-based RGB/vibe/piezo module */

#endif // _HARDWARE_H_


