// Keyglove controller source code - Motion support implementations for hand-mounted MPU-6050 sensor
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
 * @file support_motion_mpu6050_hand.cpp
 * @brief Motion support implementations for hand-mounted MPU-6050 sensor
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * This file provides a communications layer and capture interface for using the
 * InvenSense MPU-6050 motions sensor, mounted on the back of the hand. Only the
 * raw sensor data is currently used from this chip; the DMP is not used for on-
 * module motion fusion.
 *
 * Normally it is not necessary to edit this file.
 */

#include "keyglove.h"
#include "support_board.h"
#include "support_protocol.h"
#include "support_motion.h"
//#include "support_motion_mpu6050_hand.h"    // <-- included by "support_motion.h"

MPU6050 mpuHand = MPU6050(0x68);        ///< MPU-6050 motion sensor I2Cdevlib object
bool mpuHandInterrupt;                  ///< Interrupt flag for motion data availability

VectorInt16 aaRaw;                      ///< Raw linear acceleration
VectorInt16 aa;                         ///< Filtered linear acceleration
VectorInt16 aa0;                        ///< Last-iteration filtered linear acceleration
VectorInt16 gvRaw;                      ///< Raw rotational velocity
VectorInt16 gv;                         ///< Filtered rotational velocity
VectorInt16 gv0;                        ///< Last-iteration filtered rotational velocity

/**
 * @brief Interrupt handler for INT pin from MPU-6050
 * @see mpuHandInterrupt
 */
void motion_mpu6050_hand_interrupt() {
    mpuHandInterrupt = true;
}

/**
 * @brief Sets the MPU-6050 sensor mode
 * @param[in] mode Sensor mode (0=disabled, 1=enabled)
 */
void motion_set_mpu6050_hand_mode(uint8_t mode) {
    if (mode) {
        aa.x = aa.y = aa.z = 0;
        gv.x = gv.y = gv.z = 0;
        mpuHandInterrupt = true;
        attachInterrupt(KG_INTERRUPT_NUM_MPU6050_HAND, motion_mpu6050_hand_interrupt, FALLING);
        //mpuHand.setSleepEnabled(false);
        //I2Cdev::writeByte(0x68, MPU6050_RA_PWR_MGMT_1, 0x01);
    } else {
        //mpuHand.setSleepEnabled(true);
        //I2Cdev::writeByte(0x68, MPU6050_RA_PWR_MGMT_1, 0x41);
        detachInterrupt(KG_INTERRUPT_NUM_MPU6050_HAND);
    }
}

/**
 * @brief Initialize MPU-6050 communications and interrupt handler
 *
 * This function sets the MPU-6050 to 100Hz output, 2000 deg/sec resolution for
 * the gyroscope, and enables a latching active-low interrupt on DRDY (raw data
 * ready), MOT (motion detected), and ZMOT (zero motion detected).
 */
void setup_motion_mpu6050_hand() {
    // set INT4 pin (Arduino Pin 36) to INPUT/HIGH so MPU can drive interrupt pin as active-low
    pinMode(KG_INTERRUPT_PIN_MPU6050_HAND, INPUT);
    digitalWrite(KG_INTERRUPT_PIN_MPU6050_HAND, HIGH);

    // setup MPU-6050
    mpuHandInterrupt = false;

    /*
    // initialization with friendly function names
    mpuHand.initialize();
    delay(30);
    mpuHand.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    mpuHand.setDLPFMode(MPU6050_DLPF_BW_42);        // 42 Hz DLPF, 1kHz internal sampling
    mpuHand.setRate(9);                             // 1kHz/(9+1) = 100Hz
    mpuHand.setMotionDetectionThreshold(20);
    mpuHand.setMotionDetectionDuration(50);         // 1*50 = 50ms
    mpuHand.setZeroMotionDetectionThreshold(20);
    mpuHand.setZeroMotionDetectionDuration(45);     // 64*45 = ~3 sec
    mpuHand.setInterruptMode(1);                    // active low
    mpuHand.setInterruptDrive(1);                   // open drain
    mpuHand.setInterruptLatch(0);                   // latch until read
    mpuHand.setInterruptLatchClear(1);              // clear on any read
    mpuHand.setIntEnabled(0x41);    // trigger interrupt on data ready, motion, zero-motion
    */

    // initialization with manual register writes (faster than above, fewer transactions)
    // NOTE: InvenSense docs state Mot/ZMot interrupts no longer supported (?!)
    //I2Cdev::writeByte(0x68, MPU6050_RA_MOT_THR, 0x14);
    //I2Cdev::writeByte(0x68, MPU6050_RA_MOT_DUR, 0x32);
    //I2Cdev::writeByte(0x68, MPU6050_RA_ZRMOT_THR, 0x14);
    //I2Cdev::writeByte(0x68, MPU6050_RA_ZRMOT_DUR, 0x2D);
    I2Cdev::writeByte(0x68, MPU6050_RA_INT_PIN_CFG, 0xD0);
    //I2Cdev::writeByte(0x68, MPU6050_RA_INT_ENABLE, 0x61);
    I2Cdev::writeByte(0x68, MPU6050_RA_INT_ENABLE, 0x01);
    I2Cdev::writeByte(0x68, MPU6050_RA_ACCEL_CONFIG, 0x00);
    I2Cdev::writeByte(0x68, MPU6050_RA_GYRO_CONFIG, 0x18);
    I2Cdev::writeByte(0x68, MPU6050_RA_CONFIG, 0x03);
    I2Cdev::writeByte(0x68, MPU6050_RA_SMPLRT_DIV, 0x09);
    I2Cdev::writeByte(0x68, MPU6050_RA_PWR_MGMT_1, 0x01);

    // test motion sensor
    //motion_set_mpu6050_hand_mode(1); // enable motion detection
}

/**
 * @brief Update motion data from MPU-6050
 *
 * This function is called whenever the motion interrupt occurs and the flag is
 * set by the interrupt handler, and then detected as such from inside the main
 * loop() function. All six axes of raw data are read, then filtered and stored.
 *
 * @see API event: kg_evt_motion_data()
 */
void update_motion_mpu6050_hand() {
    uint8_t mpuInt = mpuHand.getIntStatus();
    if (mpuInt & 0x01) {
        // read raw motion data
        mpuHand.getMotion6(&aaRaw.x, &aaRaw.y, &aaRaw.z, &gvRaw.x, &gvRaw.y, &gvRaw.z);

        // store previous accel/gyro values
        aa0.x = aa.x;
        aa0.y = aa.y;
        aa0.z = aa.z;
        gv0.x = gv.x;
        gv0.y = gv.y;
        gv0.z = gv.z;

        // simple smoothing filter
        aa.x = aa0.x + (0.25 * (aaRaw.x - aa0.x));
        aa.y = aa0.y + (0.25 * (aaRaw.y - aa0.y));
        aa.z = aa0.z + (0.25 * (aaRaw.z - aa0.z));
        gv.x = gv0.x + (0.25 * (gvRaw.x - gv0.x));
        gv.y = gv0.y + (0.25 * (gvRaw.y - gv0.y));
        gv.z = gv0.z + (0.25 * (gvRaw.z - gv0.z));

        // build and send kg_evt_motion_data packet
        uint8_t payload[15];
        payload[0] = 0x00;  // sensor 0
        payload[1] = 0x03;  // 1=accel, 2=gyro, 1|2 = 0x03
        payload[2] = 0x0C;  // 12 bytes of motion data (6 axes, 2 bytes each)
        payload[3] = aa.x & 0xFF;
        payload[4] = aa.x >> 8;
        payload[5] = aa.y & 0xFF;
        payload[6] = aa.y >> 8;
        payload[7] = aa.z & 0xFF;
        payload[8] = aa.z >> 8;
        payload[9] = gv.x & 0xFF;
        payload[10] = gv.x >> 8;
        payload[11] = gv.y & 0xFF;
        payload[12] = gv.y >> 8;
        payload[13] = gv.z & 0xFF;
        payload[14] = gv.z >> 8;
        skipPacket = 0;
        if (kg_evt_motion_data) skipPacket = kg_evt_motion_data(payload[0], payload[1], payload[2], payload + 3);
        if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, sizeof(payload), KG_PACKET_CLASS_MOTION, KG_PACKET_ID_EVT_MOTION_DATA, payload);
    }
    if (mpuInt & 0x20) {
        send_keyglove_log(KG_LOG_LEVEL_VERBOSE, 10, F("MOTION INT"));
    } else if (mpuInt & 0x40) {
        send_keyglove_log(KG_LOG_LEVEL_VERBOSE, 10, F("ZEROMO INT"));
    }
}
