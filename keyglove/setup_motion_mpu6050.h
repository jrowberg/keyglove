// Keyglove controller source code - Special hardware setup file
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



#ifndef _SETUP_MOTION_MPU6050_H_
#define _SETUP_MOTION_MPU6050_H_

#ifdef NO_I2C_SUPPORT
    #error Selected board has no I2C support. I2C devices cannot be enabled under these circumstances.
#endif

#include "MPU6050.h"

MPU6050 accelgyro = MPU6050();

bool activeAccelGyro;
volatile bool readyAccelGyroData;
uint8_t accelGyroPacket[15] = { 0xB8, 0x07, KG_PACKET_TYPE_REPORT_ACCELGYRO, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 };

int16_t opt_accelgyro_offset[] = { 0, 0, 0, 0, 0, 0 };        // amount to offset raw gyroscope readings [x,y,z]
float opt_accelgyro_calibrate[] = { 1, 1, 1, 1, 1, 1 };       // amount to scale raw gyroscope readings [x,y,z] (post-offset)

float opt_accelgyro_kalman_constant = 0.25;
uint8_t opt_accelgyro_autozero_samples = 50;
uint8_t opt_accelgyro_autozero_threshold = 25;
uint8_t opt_accelgyro_smooth_average = 0;
uint8_t opt_accelgyro_rot90 = 4;
//float opt_accelgyro_lsb_scale = 16.4; // MPU-6050 full sensitivity is 16.4 LSBs per degree/sec

// gyroscope measurements
uint32_t accelGyroGroup;
uint32_t accelGyroGroupTime, accelGyroGroupTime0;
uint8_t accelGyroTick;                     // history position counter
VectorInt16 aaRaw;                    // raw linear acceleration
VectorInt16 aa;                       // filtered linear acceleration
VectorInt16 aa0;                      // last-iteration filtered linear acceleration
VectorInt16 gvRaw;                    // raw rotational velocity
VectorInt16 gv;                       // filtered rotational velocity
VectorInt16 gv0;                      // last-iteration filtered rotational velocity
//VectorInt16 gp;                       // gyro position values
//VectorInt16 gvMin;                    // minimum values (for calibration)
//VectorInt16 gvMax;                    // maximum values (for calibration)

bool accelGyroEnableAutoZero;
VectorInt16 aaAvg;                    // rolling average for auto-zero adjustment
VectorInt16 aaRef;                    // reference points for auto-zero detection
VectorInt16 gvAvg;                    // rolling average for auto-zero adjustment
VectorInt16 gvRef;                    // reference points for auto-zero detection
uint8_t accelGyroZeroTick;

//uint8_t calGyro = 0;
//uint8_t gyroCalibrated = false;
//bool gxAutoZero, gyAutoZero, gzAutoZero;

void mpu6050_interrupt() {
    if (!(PINE & 0b00010000)) readyAccelGyroData = true;
}

void setup_motion_accelgyro() {
    accelgyro.initialize();
    delay(30);
    accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    accelgyro.setDLPFBandwidth(MPU6050_DLPF_BW_42); // 42 Hz DLPF, 1kHz internal sampling
    accelgyro.setRate(9); // 1kHz/(9+1) = 100Hz
    accelgyro.setInterruptMode(1); // active low
    accelgyro.setInterruptDrive(1); // open drain
    accelgyro.setInterruptLatch(1); // latch until read
    accelgyro.setInterruptLatchClear(1); // clear on any read
    accelgyro.setIntDataReadyEnabled(1); // trigger interrupt on data ready
    activeAccelGyro = false;
    readyAccelGyroData = false;
    accelGyroEnableAutoZero = true;
    DDRE  &= 0b11101111; // E4 = input
    PORTE |= 0b00010000; // E4 = pullup
}

void enable_motion_accelgyro() {
    aa.x = aa.y = aa.z = 0;
    gv.x = gv.y = gv.z = 0;
    accelGyroGroup = 0;
    accelGyroGroupTime = 0;
    accelGyroTick = 0;
    accelGyroZeroTick = 0;
    //accelGyroCalibrated = false;
    //aaMin.x = aaMin.y = aaMin.z = 0;
    //aaMax.x = aaMax.y = aaMax.z = 0;
    //gvMin.x = gvMin.y = gvMin.z = 0;
    //gvMax.x = gvMax.y = gvMax.z = 0;
    activeAccelGyro = true;
    readyAccelGyroData = true;
    attachInterrupt(4, mpu6050_interrupt, CHANGE);
    accelgyro.setSleepEnabled(false);
}

void disable_motion_accelgyro() {
    accelgyro.setSleepEnabled(true);
    detachInterrupt(4);
    activeAccelGyro = false;
}

void update_motion_accelgyro() {
    readyAccelGyroData = false;
    //accelGyroGroupTime0 = micros();

    // read gyroscope with correct rotation settings
    if      (opt_accelgyro_rot90 == 0) { // no rotation:            x = +x, y = +y, z = +z
        accelgyro.getMotion6(&aaRaw.x, &aaRaw.y, &aaRaw.z, &gvRaw.x, &gvRaw.y, &gvRaw.z);
    }
    else if (opt_accelgyro_rot90 == 1) { // 90 around x axis:       x = +x, y = -z, z = +y
        accelgyro.getMotion6(&aaRaw.x, &aaRaw.z, &aaRaw.y, &gvRaw.x, &gvRaw.z, &gvRaw.y);
        aaRaw.z = -aaRaw.z;
        gvRaw.z = -gvRaw.z;
    }
    else if (opt_accelgyro_rot90 == 2) { // 90 around y axis:       x = -z, y = +y, z = +x
        accelgyro.getMotion6(&aaRaw.z, &aaRaw.y, &aaRaw.x, &gvRaw.z, &gvRaw.y, &gvRaw.x);
        aaRaw.z = -aaRaw.z;
        gvRaw.z = -gvRaw.z;
    }
    else if (opt_accelgyro_rot90 == 4) { // 90 around z axis;       x = -y, y = +x, z = +z
        accelgyro.getMotion6(&aaRaw.y, &aaRaw.x, &aaRaw.z, &gvRaw.y, &gvRaw.x, &gvRaw.z);
        aaRaw.y = -aaRaw.y;
        gvRaw.y = -gvRaw.y;
    }
    else if (opt_accelgyro_rot90 == 3) { // 90 around x, y axes:    x = -z, y = -x, z = +y
        accelgyro.getMotion6(&aaRaw.z, &aaRaw.x, &aaRaw.y, &gvRaw.z, &gvRaw.x, &gvRaw.y);
        aaRaw.x = -aaRaw.x;
        aaRaw.z = -aaRaw.z;
        gvRaw.x = -gvRaw.x;
        gvRaw.z = -gvRaw.z;
    }
    else if (opt_accelgyro_rot90 == 5) { // 90 around x, z axes:    x = -y, y = -z, z = +x
        accelgyro.getMotion6(&aaRaw.y, &aaRaw.z, &aaRaw.x, &gvRaw.y, &gvRaw.z, &gvRaw.x);
        aaRaw.y = -aaRaw.y;
        aaRaw.z = -aaRaw.z;
        gvRaw.y = -gvRaw.y;
        gvRaw.z = -gvRaw.z;
    }
    else if (opt_accelgyro_rot90 == 6) { // 90 around y, z axes:    x = -z, y = +x, z = -y
        accelgyro.getMotion6(&aaRaw.z, &aaRaw.x, &aaRaw.y, &gvRaw.z, &gvRaw.x, &gvRaw.y);
        aaRaw.y = -aaRaw.y;
        aaRaw.z = -aaRaw.z;
        gvRaw.y = -gvRaw.y;
        gvRaw.z = -gvRaw.z;
    }
    else if (opt_accelgyro_rot90 == 7) { // 90 around x, y, z axes: x = -z, y = +y, z = +x
        accelgyro.getMotion6(&aaRaw.z, &aaRaw.y, &aaRaw.x, &gvRaw.z, &gvRaw.y, &gvRaw.x);
        aaRaw.z = -aaRaw.z;
        gvRaw.z = -gvRaw.z;
    }

    // convert to degrees/sec
    //gvRaw.x /= 16.4;
    //gvRaw.y /= 16.4;
    //gvRaw.z /= 16.4;

    DEBUG_PRN_ACCELGYRO("accelgyro\t");
    DEBUG_PRN_ACCELGYRO(aaRaw.x); DEBUG_PRN_ACCELGYRO("\t");
    DEBUG_PRN_ACCELGYRO(aaRaw.y); DEBUG_PRN_ACCELGYRO("\t");
    DEBUG_PRN_ACCELGYRO(aaRaw.z); DEBUG_PRN_ACCELGYRO("\t");
    DEBUG_PRN_ACCELGYRO(gvRaw.x); DEBUG_PRN_ACCELGYRO("\t");
    DEBUG_PRN_ACCELGYRO(gvRaw.y); DEBUG_PRN_ACCELGYRO("\t");
    DEBUG_PRN_ACCELGYRO(gvRaw.z); DEBUG_PRN_ACCELGYRO("\t");

    // auto zero detection (like a ThinkPad TrackPoint)
    if (accelGyroEnableAutoZero) {
        if (accelGyroZeroTick == 0) {
            accelGyroZeroTick++;
            aaRef.x = aaAvg.x = aaRaw.x;
            aaRef.y = aaAvg.y = aaRaw.y;
            aaRef.z = aaAvg.z = aaRaw.z;
            gvRef.x = gvAvg.x = gvRaw.x;
            gvRef.y = gvAvg.y = gvRaw.y;
            gvRef.z = gvAvg.z = gvRaw.z;
        } else {
            if (abs(gvRaw.x - gvRef.x) > 20 || abs(gvRaw.y - gvRef.y) > 20 || abs(gvRaw.z - gvRef.z) > 20) {
                // too much deviation, reset zero detection
                accelGyroZeroTick = 0;
            } else if (accelGyroZeroTick == 30) {
                // no deviation for 50 iterations, so adjust auto zero offset
                accelGyroZeroTick = 0;   // reset zero detection to continue fine-tuning if necessary
                aaRef.x = aaAvg.x;    // reset the reference point to the new average
                aaRef.y = aaAvg.y;
                aaRef.z = aaAvg.z;
                gvRef.x = gvAvg.x;    // reset the reference point to the new average
                gvRef.y = gvAvg.y;
                gvRef.z = gvAvg.z;
                opt_accelgyro_offset[0] = -aaAvg.x;   // update offsets
                opt_accelgyro_offset[1] = -aaAvg.y;
                opt_accelgyro_offset[2] = -aaAvg.z;
                opt_accelgyro_offset[3] = -gvAvg.x;   // update offsets
                opt_accelgyro_offset[4] = -gvAvg.y;
                opt_accelgyro_offset[5] = -gvAvg.z;
                accelGyroEnableAutoZero = false;
            } else if (accelGyroZeroTick < 30) {
                // no deviation, but not enough data to zero
                // update running average: avg += (current - avg) / iteration
                accelGyroZeroTick++;
                aaAvg.x += (aaRaw.x - aaAvg.x) / accelGyroZeroTick;
                aaAvg.y += (aaRaw.y - aaAvg.y) / accelGyroZeroTick;
                aaAvg.z += (aaRaw.z - aaAvg.z) / accelGyroZeroTick;
                gvAvg.x += (gvRaw.x - gvAvg.x) / accelGyroZeroTick;
                gvAvg.y += (gvRaw.y - gvAvg.y) / accelGyroZeroTick;
                gvAvg.z += (gvRaw.z - gvAvg.z) / accelGyroZeroTick;
            }
        }
    }

    // offset
    aaRaw.x += opt_accelgyro_offset[0];
    aaRaw.y += opt_accelgyro_offset[1];
    aaRaw.z += opt_accelgyro_offset[2];
    gvRaw.x += opt_accelgyro_offset[3];
    gvRaw.y += opt_accelgyro_offset[4];
    gvRaw.z += opt_accelgyro_offset[5];

    // rescale if necessary
    if (opt_accelgyro_calibrate[0] != 1) aaRaw.x = (float)aaRaw.x * opt_accelgyro_calibrate[0];
    if (opt_accelgyro_calibrate[1] != 1) aaRaw.y = (float)aaRaw.y * opt_accelgyro_calibrate[1];
    if (opt_accelgyro_calibrate[2] != 1) aaRaw.z = (float)aaRaw.z * opt_accelgyro_calibrate[2];
    if (opt_accelgyro_calibrate[3] != 1) gvRaw.x = (float)gvRaw.x * opt_accelgyro_calibrate[3];
    if (opt_accelgyro_calibrate[4] != 1) gvRaw.y = (float)gvRaw.y * opt_accelgyro_calibrate[4];
    if (opt_accelgyro_calibrate[5] != 1) gvRaw.z = (float)gvRaw.z * opt_accelgyro_calibrate[5];

    // store previous accel/gyro values
    aa0.x = aa.x;
    aa0.y = aa.y;
    aa0.z = aa.z;
    gv0.x = gv.x;
    gv0.y = gv.y;
    gv0.z = gv.z;

    // simplified Kalman filtering
    aa.x = aa0.x + (opt_accelgyro_kalman_constant * (aaRaw.x - aa0.x));
    aa.y = aa0.y + (opt_accelgyro_kalman_constant * (aaRaw.y - aa0.y));
    aa.z = aa0.z + (opt_accelgyro_kalman_constant * (aaRaw.z - aa0.z));
    gv.x = gv0.x + (opt_accelgyro_kalman_constant * (gvRaw.x - gv0.x));
    gv.y = gv0.y + (opt_accelgyro_kalman_constant * (gvRaw.y - gv0.y));
    gv.z = gv0.z + (opt_accelgyro_kalman_constant * (gvRaw.z - gv0.z));

    /*accelGyroPacket[3] = (uint8_t)(aa.x >> 8);
    accelGyroPacket[4] = (uint8_t)aa.x;
    accelGyroPacket[5] = (uint8_t)(aa.y >> 8);
    accelGyroPacket[6] = (uint8_t)aa.y;
    accelGyroPacket[7] = (uint8_t)(aa.z >> 8);
    accelGyroPacket[8] = (uint8_t)aa.z;

    accelGyroPacket[9] = (uint8_t)(gv.x >> 8);
    accelGyroPacket[10] = (uint8_t)gv.x;
    accelGyroPacket[11] = (uint8_t)(gv.y >> 8);
    accelGyroPacket[12] = (uint8_t)gv.y;
    accelGyroPacket[13] = (uint8_t)(gv.z >> 8);
    accelGyroPacket[14] = (uint8_t)gv.z;*/

    DEBUG_PRN_ACCELGYRO(aa.x); DEBUG_PRN_ACCELGYRO("\t");
    DEBUG_PRN_ACCELGYRO(aa.y); DEBUG_PRN_ACCELGYRO("\t");
    DEBUG_PRN_ACCELGYRO(aa.z); DEBUG_PRN_ACCELGYRO("\t");
    DEBUG_PRN_ACCELGYRO(gv.x); DEBUG_PRN_ACCELGYRO("\t");
    DEBUG_PRN_ACCELGYRO(gv.y); DEBUG_PRN_ACCELGYRO("\t");
    DEBUG_PRNL_ACCELGYRO(gv.z);

    /*
    // approximate position (time interval is constant @ gyro rate)
    gp.x += ((gv.x + gv0.x) / 2);
    gp.y += ((gv.y + gv0.x) / 2);
    gp.z += ((gv.z + gv0.x) / 2);

    accelGyroPacket[6] = (uint8_t)gp.x >> 2;
    accelGyroPacket[7] = (uint8_t)gp.y >> 2;
    accelGyroPacket[8] = (uint8_t)gp.z >> 2;

    /*
    DEBUG_PRN_GYROSCOPE(gp.x); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gp.y); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gp.z); DEBUG_PRN_GYROSCOPE("\t");

    //Serial.write(gyroPacket, 9);

    if (opt_enable_calibration) {
        gvMin.x = min(gvMin.x, gv.x);
        gvMin.y = min(gvMin.y, gv.y);
        gvMin.z = min(gvMin.z, gv.z);
        gvMax.x = max(gvMax.x, gv.x);
        gvMax.y = max(gvMax.y, gv.y);
        gvMax.z = max(gvMax.z, gv.z);

        DEBUG_PRN_GYROSCOPE("calibrategyro\t");
        DEBUG_PRN_GYROSCOPE(gvMin.x); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRN_GYROSCOPE(gvMin.y); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRN_GYROSCOPE(gvMin.z); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRN_GYROSCOPE(gvMax.x); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRN_GYROSCOPE(gvMax.y); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRNL_GYROSCOPE(gvMax.z);
    }

    /*gyroGroupTime += (micros() - gyroGroupTime0);
    gyroTick++;
    if (gyroTick % 100 == 0) {
        // 770 us per cycle @ 100 Hz = 7.7% duty cycle when gyro is active (w/o Serial.write)
        gyroTick = 0;
        Serial.println(millis() - gyroGroup);
        Serial.println(gyroGroupTime / 100);
        Serial.println("100 gyro");
        gyroGroup = millis();
        gyroGroupTime = 0;
    }*/
}

#endif // _SETUP_MOTION_MPU6050_H_