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

//int16_t opt_gyro_offset[] = { 0, 0, 0 };        // amount to offset raw gyroscope readings [x,y,z]
//float opt_gyro_calibrate[] = { 1, 1, 1 };       // amount to scale raw gyroscope readings [x,y,z] (post-offset)

int16_t opt_gyro_offset[] = { -145, 45, 15 };
float opt_gyro_calibrate[] = { 1, 1, 1 };

float opt_gyro_kalman_constant = 0.4;
uint8_t opt_gyro_autozero_samples = 50;
uint8_t opt_gyro_autozero_threshold = 25;
uint8_t opt_gyro_smooth_average = 0;
uint8_t opt_gyro_rot90 = 4;
float opt_gyro_lsb_scale = 14.375; // ITG-3200 sensitivity is 14.375 LSBs per degree/sec

// gyroscope measurements
uint32_t gyroMicros;         // microsecond timestamp of last gyroscope read
uint32_t gyroDiff;           // time gap between readings
uint8_t gyroTick;                     // history position counter
int16_t gxRaw, gyRaw, gzRaw;          // raw accel values (no calibration, no filtering)
int16_t gx, gy, gz;                   // immediate gyro acceleration values
int16_t gx0, gy0, gz0;                // last-iteration gyro acceleration values
int16_t gxv, gyv, gzv;                // immediate gyro velocity values
int16_t gxv0, gyv0, gzv0;             // last-iteration gyro velocity values
int16_t gxp, gyp, gzp;                // immediate gyro position values
int32_t cgx, cgy, cgz;                // calibration offsets
int16_t gxBase, gyBase, gzBase;       // initial gyro values
int16_t gxAuto[50], gyAuto[50], gzAuto[50];  // history for auto-zero (offset calibration)
int16_t gxHist[20], gyHist[20], gzHist[20];  // history for averaging (smoothing)
int16_t gxMin, gyMin, gzMin;          // minimum values (for calibration)
int16_t gxMax, gyMax, gzMax;          // maximum values (for calibration)
uint8_t gset = false;             // bool to tell whether initial (*Base) values have been set yet
uint8_t calGyro = 0;
uint8_t gyroCalibrated = false;
bool gxAutoZero, gyAutoZero, gzAutoZero;

void setup_motion_gyroscope() {
    gyroscope.initialize();
    gx = gy = gz = 0;
    gxv = gyv = gzv = 0;
    gxp = gyp = gzp = 0;
    gyroMicros = 0;
    gyroTick = 0;
    gyroCalibrated = false;
    gxMin = gyMin = gzMin = 0;
    gxMax = gyMax = gzMax = 0;
}

void update_motion_gyroscope() {
    // read gyroscope with correct rotation settings
    if      (opt_gyro_rot90 == 0) { // no rotation:            x = +x, y = +y, z = +z
        gyroscope.getRotation(&gxRaw, &gyRaw, &gzRaw);
    }
    else if (opt_gyro_rot90 == 1) { // 90 around x axis:       x = +x, y = -z, z = +y
        gyroscope.getRotation(&gxRaw, &gzRaw, &gyRaw);
        gzRaw = -gzRaw;
    }
    else if (opt_gyro_rot90 == 2) { // 90 around y axis:       x = -z, y = +y, z = +x
        gyroscope.getRotation(&gzRaw, &gyRaw, &gxRaw);
        gzRaw = -gzRaw;
    }
    else if (opt_gyro_rot90 == 4) { // 90 around z axis;       x = -y, y = +x, z = +z
        gyroscope.getRotation(&gyRaw, &gxRaw, &gzRaw);
        gyRaw = -gyRaw;
    }
    else if (opt_gyro_rot90 == 3) { // 90 around x, y axes:    x = -z, y = -x, z = +y
        gyroscope.getRotation(&gzRaw, &gxRaw, &gyRaw);
        gxRaw = -gxRaw;
        gzRaw = -gzRaw;
    }
    else if (opt_gyro_rot90 == 5) { // 90 around x, z axes:    x = -y, y = -z, z = +x
        gyroscope.getRotation(&gyRaw, &gzRaw, &gxRaw);
        gyRaw = -gyRaw;
        gzRaw = -gzRaw;
    }
    else if (opt_gyro_rot90 == 6) { // 90 around y, z axes:    x = -z, y = +x, z = -y
        gyroscope.getRotation(&gzRaw, &gxRaw, &gyRaw);
        gyRaw = -gyRaw;
        gzRaw = -gzRaw;
    }
    else if (opt_gyro_rot90 == 7) { // 90 around x, y, z axes: x = -z, y = +y, z = +x
        gyroscope.getRotation(&gzRaw, &gyRaw, &gxRaw);
        gzRaw = -gzRaw;
    }
    
    // calculate reading time difference
    gyroDiff = gyroMicros > 0 ? (micros() - gyroMicros) : 1; // eliminate error from large gaps of inactivity
    gyroMicros = micros();

    // auto offset detection (like a ThinkPad TrackPoint)
    gxAuto[gyroTick % opt_gyro_autozero_samples] = gxRaw;
    gyAuto[gyroTick % opt_gyro_autozero_samples] = gyRaw;
    gzAuto[gyroTick % opt_gyro_autozero_samples] = gzRaw;
    gxAutoZero = gyAutoZero = gzAutoZero = true;
    for (i = 1; i < opt_gyro_autozero_samples && i < gyroTick; i++) {
        if (gxAutoZero && abs(gxRaw - gxAuto[(gyroTick + i) % opt_gyro_autozero_samples]) > opt_gyro_autozero_threshold) gxAutoZero = false;
        if (gyAutoZero && abs(gyRaw - gyAuto[(gyroTick + i) % opt_gyro_autozero_samples]) > opt_gyro_autozero_threshold) gyAutoZero = false;
        if (gzAutoZero && abs(gzRaw - gzAuto[(gyroTick + i) % opt_gyro_autozero_samples]) > opt_gyro_autozero_threshold) gzAutoZero = false;
        if (!gxAutoZero && !gyAutoZero && !gzAutoZero) break;
    }
    if (gxAutoZero) { opt_gyro_offset[0] = -gxRaw; gxv = 0; }
    if (gyAutoZero) { opt_gyro_offset[1] = -gyRaw; gyv = 0; }
    if (gzAutoZero) { opt_gyro_offset[2] = -gzRaw; gzv = 0; }

    // offset
    gxRaw += opt_gyro_offset[0];
    gyRaw += opt_gyro_offset[1];
    gzRaw += opt_gyro_offset[2];

    // calibrate
    if (opt_gyro_calibrate[0] != 1) gxRaw = (float)gxRaw * opt_gyro_calibrate[0] / opt_gyro_lsb_scale;
    if (opt_gyro_calibrate[1] != 1) gyRaw = (float)gyRaw * opt_gyro_calibrate[1] / opt_gyro_lsb_scale;
    if (opt_gyro_calibrate[2] != 1) gzRaw = (float)gzRaw * opt_gyro_calibrate[2] / opt_gyro_lsb_scale;

    // store previous acceleration values
    gx0 = gx;
    gy0 = gy;
    gz0 = gz;

    // Kalman filtering
    gx = gx0 + (opt_gyro_kalman_constant * (gxRaw - gx0));
    gy = gy0 + (opt_gyro_kalman_constant * (gyRaw - gy0));
    gz = gz0 + (opt_gyro_kalman_constant * (gzRaw - gz0));

    // averaging
    if (opt_gyro_smooth_average > 0) {
        for (i = 1; i < opt_gyro_smooth_average; i++) {
            gx += gxHist[(gyroTick + i) % opt_gyro_smooth_average];
            gy += gyHist[(gyroTick + i) % opt_gyro_smooth_average];
            gz += gzHist[(gyroTick + i) % opt_gyro_smooth_average];
        }
        gx /= opt_gyro_smooth_average;
        gy /= opt_gyro_smooth_average;
        gz /= opt_gyro_smooth_average;
        gxHist[gyroTick % opt_gyro_smooth_average] = gx;
        gyHist[gyroTick % opt_gyro_smooth_average] = gy;
        gzHist[gyroTick % opt_gyro_smooth_average] = gz;
    }

    // calculate linear velocity (dead reckoning)
    gxv += gx/100.0;
    gyv += gy/100.0;
    gzv += gz/100.0;

    // calculate linear position (dead reckoning)
    gxp += gxv;
    gyp += gyv;
    gzp += gzv;

    if (opt_enable_calibration) {
        gxMin = min(gxMin, gx);
        gyMin = min(gyMin, gy);
        gzMin = min(gzMin, gz);
        gxMax = max(gxMax, gx);
        gyMax = max(gyMax, gy);
        gzMax = max(gzMax, gz);
        DEBUG_PRN_GYROSCOPE("calibrategyro\t");
        DEBUG_PRN_GYROSCOPE(gxMin); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRN_GYROSCOPE(gyMin); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRN_GYROSCOPE(gzMin); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRN_GYROSCOPE(gxMax); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRN_GYROSCOPE(gyMax); DEBUG_PRN_GYROSCOPE("\t");
        DEBUG_PRNL_GYROSCOPE(gzMax);
    }

    DEBUG_PRN_GYROSCOPE("gyro\t");
    DEBUG_PRN_GYROSCOPE(gxRaw); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gyRaw); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gzRaw); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gx); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gy); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gz); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gxv); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gyv); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gzv); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gxp); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRN_GYROSCOPE(gyp); DEBUG_PRN_GYROSCOPE("\t");
    DEBUG_PRNL_GYROSCOPE(gzp);

    gyroTick++;
}

#endif // _SETUP_MOTION_ITG3200_H_


