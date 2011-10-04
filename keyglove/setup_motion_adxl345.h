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

float opt_accel_kalman_constant = 0.4;
uint8_t opt_accel_autozero_samples = 50;
uint8_t opt_accel_autozero_threshold = 25;
uint8_t opt_accel_smooth_average = 4;
uint8_t opt_accel_rot90 = 0;
float opt_accel_lsb_scale = 4; // ADXL345 sensitivity is 4 LSBs per mg (1g = 250LSB = 9.8 m/s^2)

// accelerometer measurements
uint32_t accelMicros;        // microsecond timestamp of last accelerometer read
uint32_t accelDiff;          // time gap between readings
uint8_t accelTick;                    // history position counter
int16_t axRaw, ayRaw, azRaw;          // raw accel values (no calibration, no filtering)
int16_t ax, ay, az;                   // immediate accel values
int16_t ax0, ay0, az0;                // last-iteration accel values
int16_t axv, ayv, azv;                // axial velocities
int16_t axv0, ayv0, azv0;             // last iteration axial velocities
int16_t axp, ayp, azp;                // axial positions
int16_t axBase, ayBase, azBase;       // initial accel values
int16_t axAuto[50], ayAuto[50], azAuto[50];  // history for auto-zero (offset calibration)
int16_t axHist[20], ayHist[20], azHist[20];  // history for averaging (smoothing)
int16_t axMin, ayMin, azMin;          // minimum values (for calibration)
int16_t axMax, ayMax, azMax;          // maximum values (for calibration)
uint8_t aset = false;                 // bool to tell whether initial (*Base) values have been set yet
int16_t calAccel = 0;
uint8_t accelCalibrated = false;
bool axAutoZero, ayAutoZero, azAutoZero;

void setup_motion_accelerometer() {
    accelerometer.initialize();
    accelerometer.setRate(100);
    ax = ay = az = 0;
    axv = ayv = azv = 0;
    axp = ayp = azp = 0;
    accelMicros = 0;
    accelTick = 0;
    accelCalibrated = false;
    axMin = ayMin = azMin = 0;
    axMax = ayMax = azMax = 0;
}

void update_motion_accelerometer() {
    // read accelerometer with correct rotation settings
    if      (opt_accel_rot90 == 0) { // no rotation:            x = +x, y = +y, z = +z
        accelerometer.getAcceleration(&axRaw, &ayRaw, &azRaw);
    }
    else if (opt_accel_rot90 == 1) { // 90 around x axis:       x = +x, y = -z, z = +y
        accelerometer.getAcceleration(&axRaw, &azRaw, &ayRaw);
        azRaw = -azRaw;
    }
    else if (opt_accel_rot90 == 2) { // 90 around y axis:       x = -z, y = +y, z = +x
        accelerometer.getAcceleration(&azRaw, &ayRaw, &axRaw);
        azRaw = -azRaw;
    }
    else if (opt_accel_rot90 == 4) { // 90 around z axis;       x = -y, y = +x, z = +z
        accelerometer.getAcceleration(&ayRaw, &axRaw, &azRaw);
        ayRaw = -ayRaw;
    }
    else if (opt_accel_rot90 == 3) { // 90 around x, y axes:    x = -z, y = -x, z = +y
        accelerometer.getAcceleration(&azRaw, &axRaw, &ayRaw);
        axRaw = -axRaw;
        azRaw = -azRaw;
    }
    else if (opt_accel_rot90 == 5) { // 90 around x, z axes:    x = -y, y = -z, z = +x
        accelerometer.getAcceleration(&ayRaw, &azRaw, &axRaw);
        ayRaw = -ayRaw;
        azRaw = -azRaw;
    }
    else if (opt_accel_rot90 == 6) { // 90 around y, z axes:    x = -z, y = +x, z = -y
        accelerometer.getAcceleration(&azRaw, &axRaw, &ayRaw);
        ayRaw = -ayRaw;
        azRaw = -azRaw;
    }
    else if (opt_accel_rot90 == 7) { // 90 around x, y, z axes: x = -z, y = +y, z = +x
        accelerometer.getAcceleration(&azRaw, &ayRaw, &axRaw);
        azRaw = -azRaw;
    }
    
    // calculate reading time difference
    accelDiff = accelMicros > 0 ? (micros() - accelMicros) : 1; // eliminate error from large gaps of inactivity
    accelMicros = micros();

    // auto offset detection (like a ThinkPad TrackPoint)
    axAuto[accelTick % opt_accel_autozero_samples] = axRaw;
    ayAuto[accelTick % opt_accel_autozero_samples] = ayRaw;
    azAuto[accelTick % opt_accel_autozero_samples] = azRaw;
    axAutoZero = ayAutoZero = azAutoZero = true;
    for (i = 1; i < opt_accel_autozero_samples && i < accelTick; i++) {
        if (axAutoZero && abs(axRaw - axAuto[(accelTick + i) % opt_accel_autozero_samples]) > opt_accel_autozero_threshold) axAutoZero = false;
        if (ayAutoZero && abs(ayRaw - ayAuto[(accelTick + i) % opt_accel_autozero_samples]) > opt_accel_autozero_threshold) ayAutoZero = false;
        if (azAutoZero && abs(azRaw - azAuto[(accelTick + i) % opt_accel_autozero_samples]) > opt_accel_autozero_threshold) azAutoZero = false;
        if (!axAutoZero && !ayAutoZero && !azAutoZero) break;
    }
    if (axAutoZero) { opt_accel_offset[0] = -axRaw; axv = 0; }
    if (ayAutoZero) { opt_accel_offset[1] = -ayRaw; ayv = 0; }
    if (azAutoZero) { opt_accel_offset[2] = -azRaw; azv = 0; }

    // offset
    axRaw += opt_accel_offset[0];
    ayRaw += opt_accel_offset[1];
    azRaw += opt_accel_offset[2];

    // calibrate
    axRaw = (float)axRaw * opt_accel_calibrate[0] / opt_accel_lsb_scale;
    ayRaw = (float)ayRaw * opt_accel_calibrate[1] / opt_accel_lsb_scale;
    azRaw = (float)azRaw * opt_accel_calibrate[2] / opt_accel_lsb_scale;

    // store previous acceleration values    
    ax0 = ax;
    ay0 = ay;
    az0 = az;

    // Kalman filtering
    ax = ax0 + (opt_accel_kalman_constant * (axRaw - ax0));
    ay = ay0 + (opt_accel_kalman_constant * (ayRaw - ay0));
    az = az0 + (opt_accel_kalman_constant * (azRaw - az0));

    // averaging
    if (opt_accel_smooth_average > 0) {
        for (i = 1; i < opt_accel_smooth_average; i++) {
            ax += axHist[(accelTick + i) % opt_accel_smooth_average];
            ay += ayHist[(accelTick + i) % opt_accel_smooth_average];
            az += azHist[(accelTick + i) % opt_accel_smooth_average];
        }
        ax /= opt_accel_smooth_average;
        ay /= opt_accel_smooth_average;
        az /= opt_accel_smooth_average;
        axHist[accelTick % opt_accel_smooth_average] = ax;
        ayHist[accelTick % opt_accel_smooth_average] = ay;
        azHist[accelTick % opt_accel_smooth_average] = az;
    }

    // calculate linear velocity (dead reckoning)
    axv += ax;
    ayv += ay;
    azv += az;

    // calculate linear position (dead reckoning)
    axp += axv;
    ayp += ayv;
    azp += azv;

    if (opt_enable_calibration) {
        axMin = min(axMin, ax);
        ayMin = min(ayMin, ay);
        azMin = min(azMin, az);
        axMax = max(axMax, ax);
        ayMax = max(ayMax, ay);
        azMax = max(azMax, az);
        DEBUG_PRN_ACCELEROMETER("calibrateaccel\t");
        DEBUG_PRN_ACCELEROMETER(axMin); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRN_ACCELEROMETER(ayMin); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRN_ACCELEROMETER(azMin); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRN_ACCELEROMETER(axMax); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRN_ACCELEROMETER(ayMax); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRNL_ACCELEROMETER(azMax);
    }

    /*
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
    //azv -= ((az0 - az00) - (az - az0));*/
    
    DEBUG_PRN_ACCELEROMETER("accel\t");
    DEBUG_PRN_ACCELEROMETER(axRaw); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(ayRaw); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(azRaw); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(ax); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(ay); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(az); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(axv); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(ayv); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(azv); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(axp); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(ayp); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRNL_ACCELEROMETER(azp);

    accelTick++;
}  

#endif // _SETUP_MOTION_ADXL345_H_


