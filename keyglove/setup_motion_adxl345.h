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



#ifndef _SETUP_MOTION_ADXL345_H_
#define _SETUP_MOTION_ADXL345_H_

#ifdef NO_I2C_SUPPORT
    #error Selected board has no I2C support. I2C devices cannot be enabled under these circumstances.
#endif

#ifndef LUFA
    #include <Wire.h>
#endif

#include "ADXL345.h"

ADXL345 accelerometer;

//int16_t opt_accel_offset[] = { 0, 0, 0 };           // amount to offset raw accelerometer readings [x,y,z]
//float opt_accel_calibrate[] = { 1, 1, 1 };      // amount to scale raw accelerometer readings [x,y,z] (post-offset)
int16_t opt_accel_offset[] = { 7, -16, 1 };
float opt_accel_calibrate[] = { 1, 1, 1 };
// mine @ room temp:
// x={-246,232}, y={-226,259}, z={-304,210}
// x offset = -12, x calibrate = 0.952
// y offset = 5, y calibrate = 0.945
// z offset = -47, z calibrate = 1

float opt_accel_kalman_constant = 0.6;
uint8_t opt_accel_smooth_average = 4;
uint8_t opt_accel_rot90 = 0;

// accelerometer measurements
uint32_t accelMicros;        // microsecond timestamp of last accelerometer read
uint32_t accelDiff;          // time gap between readings
uint8_t accelTick;                    // history position counter
int16_t xRaw, yRaw, zRaw;             // raw accel values (no calibration, no filtering)
int16_t x, y, z;                      // immediate accel values
int16_t x0, y0, z0;                   // last-iteration accel values
int16_t x00, y00, z00;                // 2x-last-iteration accel values (for speed)
int16_t xv, yv, zv;                   // axial vectors
int32_t cx, cy, cz;                  // calibration offsets
int16_t xBase, yBase, zBase;          // initial accel values
int16_t xHist[20], yHist[20], zHist[20];  // history for averaging (smoothing)
int16_t xMin, yMin, zMin;             // minimum values (for calibration)
int16_t xMax, yMax, zMax;             // maximum values (for calibration)
int16_t ax, ay, az;                   // immediate accel tilt angles
int16_t ax0, ay0, az0;                // last-iteration accel tilt angles
int16_t ax00, ay00, az00;             // 2x-last-iteration accel tilt angles (for speed)
int16_t axBase, ayBase, azBase;       // initial accel tilt angles
uint8_t aset = false;             // bool to tell whether initial (*Base) values have been set yet
int16_t calAccel = 0;
uint8_t accelCalibrated = false;

void setup_motion_accelerometer() {
    accelerometer.powerOn();
    accelerometer.setRate(100);
    x = y = z = 0;
    cx = cy = cz = 0;
    ax = ay = az = 0;
    accelMicros = 0;
    accelTick = 0;
    accelCalibrated = false;
    xMin = yMin = zMin = 0;
    xMax = yMax = zMax = 0;
}

void update_motion_accelerometer() {
    x00 = x0;
    y00 = y0;
    z00 = z0;
    x0 = x;
    y0 = y;
    z0 = z;
    
    // read accelerometer with correct rotation settings
    if      (opt_accel_rot90 == 0) { // no rotation:            x = +x, y = +y, z = +z
        accelerometer.readAccel(&xRaw, &yRaw, &zRaw);
    }
    else if (opt_accel_rot90 == 1) { // 90 around x axis:       x = +x, y = -z, z = +y
        accelerometer.readAccel(&xRaw, &zRaw, &yRaw);
        zRaw = -zRaw;
    }
    else if (opt_accel_rot90 == 2) { // 90 around y axis:       x = -z, y = +y, z = +x
        accelerometer.readAccel(&zRaw, &yRaw, &xRaw);
        zRaw = -zRaw;
    }
    else if (opt_accel_rot90 == 4) { // 90 around z axis;       x = -y, y = +x, z = +z
        accelerometer.readAccel(&yRaw, &xRaw, &zRaw);
        yRaw = -yRaw;
    }
    else if (opt_accel_rot90 == 3) { // 90 around x, y axes:    x = -z, y = -x, z = +y
        accelerometer.readAccel(&zRaw, &xRaw, &yRaw);
        xRaw = -xRaw;
        zRaw = -zRaw;
    }
    else if (opt_accel_rot90 == 5) { // 90 around x, z axes:    x = -y, y = -z, z = +x
        accelerometer.readAccel(&yRaw, &zRaw, &xRaw);
        yRaw = -yRaw;
        zRaw = -zRaw;
    }
    else if (opt_accel_rot90 == 6) { // 90 around y, z axes:    x = -z, y = +x, z = -y
        accelerometer.readAccel(&zRaw, &xRaw, &yRaw);
        yRaw = -yRaw;
        zRaw = -zRaw;
    }
    else if (opt_accel_rot90 == 7) { // 90 around x, y, z axes: x = -z, y = +y, z = +x
        accelerometer.readAccel(&zRaw, &yRaw, &xRaw);
        zRaw = -zRaw;
    }
    
    // calculate reading time difference
    accelDiff = accelMicros > 0 ? (micros() - accelMicros) : 1; // eliminate error from large gaps of inactivity
    accelMicros = micros();
    
    // offset
    xRaw += opt_accel_offset[0];
    yRaw += opt_accel_offset[1];
    zRaw += opt_accel_offset[2];
    
    // calibrate
    xRaw = (float)xRaw * opt_accel_calibrate[0];
    yRaw = (float)yRaw * opt_accel_calibrate[1];
    zRaw = (float)zRaw * opt_accel_calibrate[2];
    
    // Kalman filtering
    x = x0 + (opt_accel_kalman_constant * (xRaw - x0));
    y = y0 + (opt_accel_kalman_constant * (yRaw - y0));
    z = z0 + (opt_accel_kalman_constant * (zRaw - z0));

    // averaging
    if (opt_accel_smooth_average > 0) {
        for (i = 1; i < opt_accel_smooth_average; i++) {
            x += xHist[(accelTick + i) % opt_accel_smooth_average];
            y += yHist[(accelTick + i) % opt_accel_smooth_average];
            z += zHist[(accelTick + i) % opt_accel_smooth_average];
        }
        x /= opt_accel_smooth_average;
        y /= opt_accel_smooth_average;
        z /= opt_accel_smooth_average;
        xHist[accelTick % opt_accel_smooth_average] = x;
        yHist[accelTick % opt_accel_smooth_average] = y;
        zHist[accelTick % opt_accel_smooth_average] = z;
    }
    
    // calculate linear velocity (INACCURATE SO FAR)
    xv += (x - x0) - (x0 - x00);
    yv += (y - y0) - (y0 - y00);
    zv += (z - z0) - (z0 - z00);

    if (opt_enable_calibration) {
        xMin = min(xMin, x);
        yMin = min(yMin, y);
        zMin = min(zMin, z);
        xMax = max(xMax, x);
        yMax = max(yMax, y);
        zMax = max(zMax, z);
        DEBUG_PRN_ACCELEROMETER("calibrateaccel ");
        DEBUG_PRN_ACCELEROMETER(xMin); DEBUG_PRN_ACCELEROMETER(" ");
        DEBUG_PRN_ACCELEROMETER(yMin); DEBUG_PRN_ACCELEROMETER(" ");
        DEBUG_PRN_ACCELEROMETER(zMin); DEBUG_PRN_ACCELEROMETER(" ");
        DEBUG_PRN_ACCELEROMETER(xMax); DEBUG_PRN_ACCELEROMETER(" ");
        DEBUG_PRN_ACCELEROMETER(yMax); DEBUG_PRN_ACCELEROMETER(" ");
        DEBUG_PRNL_ACCELEROMETER(zMax);
    }

    // calculate tilt angle and angular velocity
    ax00 = ax0; // 2x-last-iteration
    ay00 = ay0;
    az00 = az0;
    ax0 = ax; // last-iteration
    ay0 = ay;
    az0 = az;
    
    float xc = (float)x/256, yc = (float)y/256, zc = (float)z/256;
    
    // regular atan method, more work and does not adjust for quadrants automatically
    ax = degrees(atan(xc / sqrt(yc*yc + zc*zc)));
    ay = degrees(atan(yc / sqrt(xc*xc + zc*zc)));
    //az = degrees(atan(sqrt(xc*xc + yc*yc) / zc));
    az = degrees(atan(zc / sqrt(xc*xc + yc*yc)));
    // magnitude seems accurate for z, but the sign is weird and the data is not really valid (see below)
    
    // atan2 is definitely very simple
    //ax = degrees(atan2(yc, zc));
    //ay = degrees(atan2(xc, zc));
    //az = degrees(atan(sqrt(xc*xc + yc*yc) / zc));

    int16_t axa = abs(ax), aya = abs(ay); //, aza = abs(az);
    if (axa > 90 || aya > 90) az += 180;
    
    // you can't really get z rotation with just an accelerometer.
    // you need a magnetometer (compass) for absolute heading,
    // or clever use of a gyroscope for relative heading.
    
    // skip angular velocity because it comes from the gyro
    //axv -= ((ax0 - ax00) - (ax - ax0));
    //ayv -= ((ay0 - ay00) - (ay - ay0));
    //azv -= ((az0 - az00) - (az - az0));
    
    DEBUG_PRN_ACCELEROMETER("accel ");
    DEBUG_PRN_ACCELEROMETER(xRaw); DEBUG_PRN_ACCELEROMETER(" ");
    DEBUG_PRN_ACCELEROMETER(yRaw); DEBUG_PRN_ACCELEROMETER(" ");
    DEBUG_PRN_ACCELEROMETER(zRaw); DEBUG_PRN_ACCELEROMETER(" ");
    DEBUG_PRN_ACCELEROMETER(x); DEBUG_PRN_ACCELEROMETER(" ");
    DEBUG_PRN_ACCELEROMETER(y); DEBUG_PRN_ACCELEROMETER(" ");
    DEBUG_PRN_ACCELEROMETER(z); DEBUG_PRN_ACCELEROMETER(" ");
    DEBUG_PRN_ACCELEROMETER(xv); DEBUG_PRN_ACCELEROMETER(" ");
    DEBUG_PRN_ACCELEROMETER(yv); DEBUG_PRN_ACCELEROMETER(" ");
    DEBUG_PRN_ACCELEROMETER(zv); DEBUG_PRN_ACCELEROMETER(" ");
    DEBUG_PRN_ACCELEROMETER(ax); DEBUG_PRN_ACCELEROMETER(" ");
    DEBUG_PRN_ACCELEROMETER(ay); DEBUG_PRN_ACCELEROMETER(" ");
    DEBUG_PRNL_ACCELEROMETER(az);

    accelTick++;
}  

#endif // _SETUP_MOTION_ADXL345_H_