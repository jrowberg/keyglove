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



#ifndef _SETUP_MOTION_ITG3200_H_
#define _SETUP_MOTION_ITG3200_H_

#ifdef NO_I2C_SUPPORT
    #error Selected board has no I2C support. I2C devices cannot be enabled under these circumstances.
#endif

#include "ITG3200.h"

ITG3200 gyroscope = ITG3200();

bool activeGyroscope;
volatile bool readyGyroscopeData;
uint8_t gyroPacket[15] = { 0xB8, 0x07, KG_PACKET_TYPE_REPORT_GYRO, 0,0,0, 0,0,0, 0,0,0 };

//int16_t opt_gyro_offset[] = { 0, 0, 0 };        // amount to offset raw gyroscope readings [x,y,z]
//float opt_gyro_calibrate[] = { 1, 1, 1 };       // amount to scale raw gyroscope readings [x,y,z] (post-offset)

int16_t opt_gyro_offset[] = { 0, 0, 0 }; //-590, 210, 50 };
float opt_gyro_calibrate[] = { 1, 1, 1 };

float opt_gyro_kalman_constant = 0.25;
uint8_t opt_gyro_autozero_samples = 50;
uint8_t opt_gyro_autozero_threshold = 25;
uint8_t opt_gyro_smooth_average = 0;
uint8_t opt_gyro_rot90 = 4;
float opt_gyro_lsb_scale = 14.375; // ITG-3200 sensitivity is 14.375 LSBs per degree/sec

// gyroscope measurements
uint32_t gyroGroup;
uint32_t gyroGroupTime, gyroGroupTime0;
uint8_t gyroTick;                     // history position counter
int16_t gxvRaw, gyvRaw, gzvRaw;       // raw gyro values (read from device)
int16_t gxv, gyv, gzv;                // immediate gyro acceleration values
int16_t gxv0, gyv0, gzv0;             // last-iteration gyro velocity values
int16_t gxp, gyp, gzp;                // immediate gyro position values
int16_t gxvMin, gyvMin, gzvMin;       // minimum values (for calibration)
int16_t gxvMax, gyvMax, gzvMax;       // maximum values (for calibration)

bool gyroEnableAutoZero;
int16_t gxvAvg, gyvAvg, gzvAvg;       // rolling average for auto-zero adjustment
int16_t gxvRef, gyvRef, gzvRef;       // reference points for auto-zero detection
uint8_t gyroZeroTick;

uint8_t calGyro = 0;
uint8_t gyroCalibrated = false;
bool gxAutoZero, gyAutoZero, gzAutoZero;

void itg3200_interrupt() {
    if (!(PINE & 0b00100000)) readyGyroscopeData = true;
}

void setup_motion_gyroscope() {
    gyroscope.initialize();
    gyroscope.setFullScaleRange(ITG3200_FULLSCALE_2000);
    gyroscope.setDLPFBandwidth(ITG3200_DLPF_BW_42); // 42 Hz DLPF, 1kHz internal sampling
    gyroscope.setRate(9); // 1kHz/(9+1) = 100Hz
    DDRE &= 0b11011111; // E5 = input
    PORTE |= 0b00100000; // E5 = pullup
    gyroscope.setInterruptMode(1); // active low
    gyroscope.setInterruptDrive(1); // open drain
    gyroscope.setInterruptLatchClear(1); // clear on any read
    gyroscope.setIntDataReadyEnabled(1); // trigger interrupt on data ready
    activeGyroscope = false;
    readyGyroscopeData = false;
    gyroEnableAutoZero = true;
}

void enable_motion_gyroscope() {
    gxv = gyv = gzv = 0;
    gxp = gyp = gzp = 0;
    gyroGroup = 0;
    gyroGroupTime = 0;
    gyroTick = 0;
    gyroZeroTick = 0;
    gyroCalibrated = false;
    gxvMin = gyvMin = gzvMin = 0;
    gxvMax = gyvMax = gzvMax = 0;
    activeGyroscope = true;
    attachInterrupt(5, itg3200_interrupt, CHANGE);
    readyGyroscopeData = true;
}

void disable_motion_gyroscope() {
    detachInterrupt(5);
    activeGyroscope = false;
}

void update_motion_gyroscope() {
    readyGyroscopeData = false;
    //gyroGroupTime0 = micros();

    // read gyroscope with correct rotation settings
    if      (opt_gyro_rot90 == 0) { // no rotation:            x = +x, y = +y, z = +z
        gyroscope.getRotation(&gxvRaw, &gyvRaw, &gzvRaw);
    }
    else if (opt_gyro_rot90 == 1) { // 90 around x axis:       x = +x, y = -z, z = +y
        gyroscope.getRotation(&gxvRaw, &gzvRaw, &gyvRaw);
        gzvRaw = -gzvRaw;
    }
    else if (opt_gyro_rot90 == 2) { // 90 around y axis:       x = -z, y = +y, z = +x
        gyroscope.getRotation(&gzvRaw, &gyvRaw, &gxvRaw);
        gzvRaw = -gzvRaw;
    }
    else if (opt_gyro_rot90 == 4) { // 90 around z axis;       x = -y, y = +x, z = +z
        gyroscope.getRotation(&gyvRaw, &gxvRaw, &gzvRaw);
        gyvRaw = -gyvRaw;
    }
    else if (opt_gyro_rot90 == 3) { // 90 around x, y axes:    x = -z, y = -x, z = +y
        gyroscope.getRotation(&gzvRaw, &gxvRaw, &gyvRaw);
        gxvRaw = -gxvRaw;
        gzvRaw = -gzvRaw;
    }
    else if (opt_gyro_rot90 == 5) { // 90 around x, z axes:    x = -y, y = -z, z = +x
        gyroscope.getRotation(&gyvRaw, &gzvRaw, &gxvRaw);
        gyvRaw = -gyvRaw;
        gzvRaw = -gzvRaw;
    }
    else if (opt_gyro_rot90 == 6) { // 90 around y, z axes:    x = -z, y = +x, z = -y
        gyroscope.getRotation(&gzvRaw, &gxvRaw, &gyvRaw);
        gyvRaw = -gyvRaw;
        gzvRaw = -gzvRaw;
    }
    else if (opt_gyro_rot90 == 7) { // 90 around x, y, z axes: x = -z, y = +y, z = +x
        gyroscope.getRotation(&gzvRaw, &gyvRaw, &gxvRaw);
        gzvRaw = -gzvRaw;
    }

    DEBUG_PRN_GYROSCOPE("gyro\t");
    DEBUG_PRN_GYROSCOPE(gxvRaw); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gyvRaw); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gzvRaw); DEBUG_PRN_GYROSCOPE("\t");

    // auto zero detection (like a ThinkPad TrackPoint)
    if (gyroEnableAutoZero) {
        if (gyroZeroTick == 0) {
            gyroZeroTick++;
            gxvRef = gxvAvg = gxvRaw;
            gyvRef = gyvAvg = gyvRaw;
            gzvRef = gzvAvg = gzvRaw;
        } else {
            if (abs(gxvRaw - gxvRef) > 300 || abs(gyvRaw - gyvRef) > 300 || abs(gzvRaw - gzvRef) > 300) {
                // too much deviation, reset zero detection
                gyroZeroTick = 0;
            } else if (gyroZeroTick == 50) {
                // no deviation for 50 iterations, so adjust auto zero offset
                gyroZeroTick = 0;   // reset zero detection to continue fine-tuning if necessary
                gxvRef = gxvAvg;    // reset the reference point to the new average
                gyvRef = gyvAvg;
                gzvRef = gzvAvg;
                opt_gyro_offset[0] = -gxvAvg;   // update offsets
                opt_gyro_offset[1] = -gyvAvg;
                opt_gyro_offset[2] = -gzvAvg;
                gyroEnableAutoZero = false;
            } else if (gyroZeroTick < 50) {
                // no deviation, but not enough data to zero
                // update running average: avg += (current - avg) / iteration
                gyroZeroTick++;
                gxvAvg += (gxvRaw - gxvAvg) / gyroZeroTick;
                gyvAvg += (gyvRaw - gyvAvg) / gyroZeroTick;
                gzvAvg += (gzvRaw - gzvAvg) / gyroZeroTick;
            }
        }
    }

    // offset
    gxvRaw += opt_gyro_offset[0];
    gyvRaw += opt_gyro_offset[1];
    gzvRaw += opt_gyro_offset[2];

    // rescale if necessary
    if (opt_gyro_calibrate[0] != 1) gxvRaw = (float)gxvRaw * opt_gyro_calibrate[0];
    if (opt_gyro_calibrate[1] != 1) gyvRaw = (float)gyvRaw * opt_gyro_calibrate[1];
    if (opt_gyro_calibrate[2] != 1) gzvRaw = (float)gzvRaw * opt_gyro_calibrate[2];

    // store previous acceleration values
    gxv0 = gxv;
    gyv0 = gyv;
    gzv0 = gzv;

    // Kalman filtering
    gxv = gxv0 + (opt_gyro_kalman_constant * (gxvRaw - gxv0));
    gyv = gyv0 + (opt_gyro_kalman_constant * (gyvRaw - gyv0));
    gzv = gzv0 + (opt_gyro_kalman_constant * (gzvRaw - gzv0));

    gyroPacket[3] = (uint8_t)gxv >> 6; // cut range down by 64x (1 LSB ~= 4.45 degree/sec)
    gyroPacket[4] = (uint8_t)gyv >> 6;
    gyroPacket[5] = (uint8_t)gzv >> 6;

    DEBUG_PRN_GYROSCOPE(gxv); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gyv); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gzv); DEBUG_PRN_GYROSCOPE("\t");

    /*
    // approximate position (time interval is constant @ gyro rate)
    gxp += ((gxv + gxv0) / 2) >> 2;
    gyp += ((gyv + gxv0) / 2) >> 2;
    gzp += ((gzv + gxv0) / 2) >> 2;

    gyroPacket[6] = (uint8_t)gxp;
    gyroPacket[7] = (uint8_t)gyp;
    gyroPacket[8] = (uint8_t)gzp;

    DEBUG_PRN_GYROSCOPE(gxp); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gyp); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gzp); DEBUG_PRN_GYROSCOPE("\t");

    //Serial.write(gyroPacket, 9);

    if (opt_enable_calibration) {
        gxvMin = min(gxvMin, gxv);
        gyvMin = min(gyvMin, gyv);
        gzvMin = min(gzvMin, gzv);
        gxvMax = max(gxvMax, gxv);
        gyvMax = max(gyvMax, gyv);
        gzvMax = max(gzvMax, gzv);

        DEBUG_PRN_GYROSCOPE("calibrategyro\t");
        DEBUG_PRN_GYROSCOPE(gxvMin); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRN_GYROSCOPE(gyvMin); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRN_GYROSCOPE(gzvMin); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRN_GYROSCOPE(gxvMax); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRN_GYROSCOPE(gyvMax); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRNL_GYROSCOPE(gzvMax);
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

#endif // _SETUP_MOTION_ITG3200_H_