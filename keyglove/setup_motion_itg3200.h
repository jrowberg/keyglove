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

//uint16_t opt_gyro_offset[] = { 0, 0, 0 };            // amount to offset raw gyroscope readings [x,y,z]
//float opt_gyro_calibrate[] = { 1, 1, 1 };       // amount to scale raw gyroscope readings [x,y,z] (post-offset)
uint16_t opt_gyro_offset[] = { 0, 0, 0 };
float opt_gyro_calibrate[] = { 1, 1, 1 };

float opt_gyro_kalman_constant = 0.3;
uint8_t opt_gyro_smooth_average = 0;
uint8_t opt_gyro_rot90 = 4;

// gyroscope measurements
uint32_t gyroMicros;         // microsecond timestamp of last gyroscope read
uint32_t gyroDiff;           // time gap between readings
uint8_t gyroTick;                     // history position counter
int16_t gxRaw, gyRaw, gzRaw;          // raw accel values (no calibration, no filtering)
int16_t gx, gy, gz;                   // immediate gyro values
int16_t gx0, gy0, gz0;                // last-iteration gyro values
int16_t gx00, gy00, gz00;             // 2x-last-iteration gyro values (for speed)
int32_t cgx, cgy, cgz;               // calibration offsets
int16_t gxBase, gyBase, gzBase;       // initial gyro values
int16_t gxHist[20], gyHist[20], gzHist[20];  // history for averaging (smoothing)
int16_t gxMin, gyMin, gzMin;          // minimum values (for calibration)
int16_t gxMax, gyMax, gzMax;          // maximum values (for calibration)
uint8_t gset = false;             // bool to tell whether initial (*Base) values have been set yet
uint8_t calGyro = 0;
uint8_t gyroCalibrated = false;

void setup_motion_gyroscope() {
    //ITG3200_ADDR_AD0_LOW
    gyroscope.initialize();
    gx = gy = gz = 0;
    gyroMicros = 0;
    gyroTick = 0;
    gyroCalibrated = false;
    gxMin = gyMin = gzMin = 0;
    gxMax = gyMax = gzMax = 0;
    
    activeGyroscope = true;
    activeMouse = true;
}

void update_motion_gyroscope() {
    gx00 = gx0;
    gy00 = gy0;
    gz00 = gz0;
    gx0 = gx;
    gy0 = gy;
    gz0 = gz;

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

    // offset
    gxRaw += opt_gyro_offset[0];
    gyRaw += opt_gyro_offset[1];
    gzRaw += opt_gyro_offset[2];
    
    // calibrate
    gxRaw = (float)gxRaw * opt_gyro_calibrate[0] / 15; //14.375;
    gyRaw = (float)gyRaw * opt_gyro_calibrate[1] / 15; //14.375;
    gzRaw = (float)gzRaw * opt_gyro_calibrate[2] / 15; //14.375;
    
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
    
    if (opt_enable_calibration) {
        gxMin = min(gxMin, gx);
        gyMin = min(gyMin, gy);
        gzMin = min(gzMin, gz);
        gxMax = max(gxMax, gx);
        gyMax = max(gyMax, gy);
        gzMax = max(gzMax, gz);
        DEBUG_PRN_GYROSCOPE("calibrategyro ");
        DEBUG_PRN_GYROSCOPE(gxMin); DEBUG_PRN_GYROSCOPE(" ");
        DEBUG_PRN_GYROSCOPE(gyMin); DEBUG_PRN_GYROSCOPE(" ");
        DEBUG_PRN_GYROSCOPE(gzMin); DEBUG_PRN_GYROSCOPE(" ");
        DEBUG_PRN_GYROSCOPE(gxMax); DEBUG_PRN_GYROSCOPE(" ");
        DEBUG_PRN_GYROSCOPE(gyMax); DEBUG_PRN_GYROSCOPE(" ");
        DEBUG_PRNL_GYROSCOPE(gzMax);
    }

    DEBUG_PRN_GYROSCOPE("gyro ");
    DEBUG_PRN_GYROSCOPE(gxRaw); DEBUG_PRN_GYROSCOPE(" ");
    DEBUG_PRN_GYROSCOPE(gyRaw); DEBUG_PRN_GYROSCOPE(" ");
    DEBUG_PRN_GYROSCOPE(gzRaw); DEBUG_PRN_GYROSCOPE(" ");
    DEBUG_PRN_GYROSCOPE(gx); DEBUG_PRN_GYROSCOPE(" ");
    DEBUG_PRN_GYROSCOPE(gy); DEBUG_PRN_GYROSCOPE(" ");
    DEBUG_PRNL_GYROSCOPE(gz);

    gyroTick++;
}

#endif // _SETUP_MOTION_ITG3200_H_


