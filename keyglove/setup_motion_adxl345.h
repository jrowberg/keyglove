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

bool activeAccelerometer = false;
volatile bool readyAccelerometerData = false;
uint8_t accelPacket[9] = { 0xB8, 0x0A, KG_PACKET_TYPE_REPORT_ACCEL, 0,0, 0,0, 0,0 };

int16_t opt_accel_offset[] = { 0, 0, 0 };       // amount to offset raw accelerometer readings [x,y,z]
float opt_accel_calibrate[] = { 1, 1, 1 };      // amount to scale raw accelerometer readings [x,y,z] (post-offset)

// mine @ room temp:
// x={-246,232}, y={-226,259}, z={-304,210}
// x offset = -12, x calibrate = 0.952
// y offset = 5, y calibrate = 0.945
// z offset = -47, z calibrate = 1

float opt_accel_kalman_constant = 0.25; // (equivalent to 1/[value]-iteration rolling average)
uint8_t opt_accel_autozero_samples = 50;
uint8_t opt_accel_autozero_threshold = 25;
uint8_t opt_accel_rot90 = 0;

// ADXL345 sensitivity is 4mg per LSB, so:
//   1 LSB = 4mg
// 256 LSB = 1g
// 256 LSB = 9.8 m/s^2
// 256 LSB = 9800 mm/s^2
//   1 LSB = 38.3 mm/s^2 ~= 4 cm/s^2
//float opt_accel_lsb_scale = 4;
//float opt_accel_rate = 100;

// accelerometer measurements
uint32_t accelGroup;
uint32_t accelGroupTime, accelGroupTime0;
uint8_t accelTick;                  // history position counter
VectorInt16 aaRaw;                  // raw acceleleration
VectorInt16 aa;                     // filtered acceleration
VectorInt16 aa0;                    // last-iteration acceleration
//VectorInt16 av;                     // axial velocities
//VectorInt16 av0;                    // previous iteration axial velocities
//VectorInt16 ap;                     // axial positions
//VectorInt16 aaMin;                  // minimum values (for calibration)
//VectorInt16 aaMax;                  // maximum values (for calibration)

bool accelEnableAutoZero;
VectorInt16 aaAvg;                  // rolling average for auto-zero adjustment
VectorInt16 aaRef;                  // reference points for auto-zero detection
uint8_t accelZeroTick;

void adxl345_interrupt1() {
    if (!(PINE & 0b00010000)) readyAccelerometerData = true;
}

void setup_motion_accelerometer() {
    accelerometer.initialize();
    accelerometer.setRate(ADXL345_RATE_100);
    //accelerometer.setRange(ADXL345_RANGE_2G);
    //accelerometer.setAutoSleepEnabled(true);
    //accelerometer.setLowPowerEnabled(true);
    DDRE &= 0b11101111; // E4 = input
    PORTE |= 0b00010000; // E4 = pullup
    accelerometer.setInterruptMode(1);
    accelerometer.setIntDataReadyPin(0);
    accelerometer.setIntDataReadyEnabled(1);
}

void enable_motion_accelerometer() {
    aa.x = aa.y = aa.z = 0;
    //av.x = av.y = av.z = 0;
    //ap.x = ap.y = ap.z = 0;
    accelGroup = 0;
    accelGroupTime = 0;
    accelTick = 0;
    //aaMin.x = aaMin.y = aaMin.z = 0;
    //aaMax.x = aaMax.y = aaMax.z = 0;
    activeAccelerometer = true;
    attachInterrupt(4, adxl345_interrupt1, CHANGE);
    readyAccelerometerData = true;
    accelEnableAutoZero = true;
}

void disable_motion_accelerometer() {
    detachInterrupt(4);
    activeAccelerometer = false;
}

void update_motion_accelerometer() {
    readyAccelerometerData = false;
    //accelGroupTime0 = micros();

    // read accelerometer with correct rotation settings
    if      (opt_accel_rot90 == 0) { // no rotation:            x = +x, y = +y, z = +z
        accelerometer.getAcceleration(&aaRaw.x, &aaRaw.y, &aaRaw.z);
    }
    else if (opt_accel_rot90 == 1) { // 90 around x axis:       x = +x, y = -z, z = +y
        accelerometer.getAcceleration(&aaRaw.x, &aaRaw.z, &aaRaw.y);
        aaRaw.z = -aaRaw.z;
    }
    else if (opt_accel_rot90 == 2) { // 90 around y axis:       x = -z, y = +y, z = +x
        accelerometer.getAcceleration(&aaRaw.z, &aaRaw.y, &aaRaw.x);
        aaRaw.z = -aaRaw.z;
    }
    else if (opt_accel_rot90 == 4) { // 90 around z axis;       x = -y, y = +x, z = +z
        accelerometer.getAcceleration(&aaRaw.y, &aaRaw.x, &aaRaw.z);
        aaRaw.y = -aaRaw.y;
    }
    else if (opt_accel_rot90 == 3) { // 90 around x, y axes:    x = -z, y = -x, z = +y
        accelerometer.getAcceleration(&aaRaw.z, &aaRaw.x, &aaRaw.y);
        aaRaw.x = -aaRaw.x;
        aaRaw.z = -aaRaw.z;
    }
    else if (opt_accel_rot90 == 5) { // 90 around x, z axes:    x = -y, y = -z, z = +x
        accelerometer.getAcceleration(&aaRaw.y, &aaRaw.z, &aaRaw.x);
        aaRaw.y = -aaRaw.y;
        aaRaw.z = -aaRaw.z;
    }
    else if (opt_accel_rot90 == 6) { // 90 around y, z axes:    x = -z, y = +x, z = -y
        accelerometer.getAcceleration(&aaRaw.z, &aaRaw.x, &aaRaw.y);
        aaRaw.y = -aaRaw.y;
        aaRaw.z = -aaRaw.z;
    }
    else if (opt_accel_rot90 == 7) { // 90 around x, y, z axes: x = -z, y = +y, z = +x
        accelerometer.getAcceleration(&aaRaw.z, &aaRaw.y, &aaRaw.x);
        aaRaw.z = -aaRaw.z;
    }

    DEBUG_PRN_ACCELEROMETER("accel\t");
    DEBUG_PRN_ACCELEROMETER(aaRaw.x); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(aaRaw.y); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(aaRaw.z); DEBUG_PRN_ACCELEROMETER("\t");

    // auto zero detection (like a ThinkPad TrackPoint)
    if (accelEnableAutoZero) {
        if (accelZeroTick == 0) {
            accelZeroTick++;
            aaRef.x = aaAvg.x = aaRaw.x;
            aaRef.y = aaAvg.y = aaRaw.y;
            aaRef.z = aaAvg.z = aaRaw.z;
        } else {
            if (abs(aaRaw.x - aaRef.x) > 35 || abs(aaRaw.y - aaRef.y) > 35 || abs(aaRaw.z - aaRef.z) > 35) {
                // too much deviation, reset zero detection
                accelZeroTick = 0;
            } else if (accelZeroTick == 30) {
                // no deviation for 50 iterations, so adjust auto zero offset
                accelZeroTick = 0;  // reset zero detection to continue fine-tuning if necessary
                aaRef.x = aaAvg.x;    // reset the reference point to the new average
                aaRef.y = aaAvg.y;
                aaRef.z = aaAvg.z;
                opt_accel_offset[0] = -aaAvg.x;  // update offsets
                opt_accel_offset[1] = -aaAvg.y;
                opt_accel_offset[2] = (250 - aaAvg.z);
                accelEnableAutoZero = false;
            } else if (accelZeroTick < 30) {
                // no deviation, but not enough data to zero
                // update running average: avg += (current - avg) / iteration
                accelZeroTick++;
                aaAvg.x += (aaRaw.x - aaAvg.x) / accelZeroTick;
                aaAvg.y += (aaRaw.y - aaAvg.y) / accelZeroTick;
                aaAvg.z += (aaRaw.z - aaAvg.z) / accelZeroTick;
            }
        }
    }

    // offset
    aaRaw.x += opt_accel_offset[0];
    aaRaw.y += opt_accel_offset[1];
    aaRaw.z += opt_accel_offset[2];

    // rescale if necessary
    if (opt_accel_calibrate[0] != 1) aaRaw.x = (float)aaRaw.x * opt_accel_calibrate[0];
    if (opt_accel_calibrate[1] != 1) aaRaw.y = (float)aaRaw.y * opt_accel_calibrate[1];
    if (opt_accel_calibrate[2] != 1) aaRaw.z = (float)aaRaw.z * opt_accel_calibrate[2];

    // store previous acceleration values
    aa0.x = aa.x;
    aa0.y = aa.y;
    aa0.z = aa.z;

    // simplified Kalman filtering
    aa.x = aa0.x + (opt_accel_kalman_constant * (aaRaw.x - aa0.x));
    aa.y = aa0.y + (opt_accel_kalman_constant * (aaRaw.y - aa0.y));
    aa.z = aa0.z + (opt_accel_kalman_constant * (aaRaw.z - aa0.z));

    accelPacket[3] = (uint8_t)(aa.x >> 8);
    accelPacket[4] = (uint8_t)aa.x;
    accelPacket[5] = (uint8_t)(aa.y >> 8);
    accelPacket[6] = (uint8_t)aa.y;
    accelPacket[7] = (uint8_t)(aa.z >> 8);
    accelPacket[8] = (uint8_t)aa.z;

    DEBUG_PRN_ACCELEROMETER(aa.x); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(aa.y); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(aa.z); DEBUG_PRN_ACCELEROMETER("\t");

    /*
    // obtain magnitude of acceleration
    // (accelMagnitude is expressed in "g", should always be ~1 if holding still)
    aa.x2 = (int32_t)aa.x * aa.x;
    aa.y2 = (int32_t)aa.y * aa.y;
    aa.z2 = (int32_t)aa.z * aa.z;
    accelMagnitude = (sqrt(aa.x2 + aa.y2 + aa.z2) / 250);

    // obtain tilt angle estimation relative to gravity
    // (all measurements are in radians)
    axTilt = atan((float)aa.x / sqrt(aa.y2 + aa.z2)); // will be 0 on a flat surface
    ayTilt = atan((float)aa.y / sqrt(aa.x2 + aa.z2)); // will be 0 on a flat surface
    azTilt = atan((float)aa.z / sqrt(aa.x2 + aa.y2)); // will be PI/2 (+90 degrees) right side up on a flat surface

    // approximate velocity (time interval is constant @ accel rate)
    av.x += ((aa.x + aa0.x) / 2) >> 5;
    av.y += ((aa.y + aa0.y) / 2) >> 5;
    av.z += ((aa.z + aa0.z) / 2) >> 5;

    accelPacket[6] = (uint8_t)av.x;
    accelPacket[7] = (uint8_t)av.y;
    accelPacket[8] = (uint8_t)av.z;

    DEBUG_PRN_ACCELEROMETER(av.x); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(av.y); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(av.z); DEBUG_PRN_ACCELEROMETER("\t");

    // approximate position (time interval is constant @ accel rate)
    ap.x += ((av.x + av0.x) / 2) >> 2;
    ap.y += ((av.y + av0.y) / 2) >> 2;
    ap.z += ((av.z + av0.z) / 2) >> 2;

    accelPacket[9] = (uint8_t)ap.x;
    accelPacket[10] = (uint8_t)ap.y;
    accelPacket[11] = (uint8_t)ap.z;

    DEBUG_PRN_ACCELEROMETER(ap.x); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(ap.y); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(ap.z); DEBUG_PRN_ACCELEROMETER("\t");
    */

    //Serial.write(accelPacket, 6);

    /*if (opt_enable_calibration) {
        aaMin.x = min(aaMin.x, aa.x);
        aaMin.y = min(aaMin.y, aa.y);
        aaMin.z = min(aaMin.z, aa.z);
        aaMax.x = max(aaMax.x, aa.x);
        aaMax.y = max(aaMax.y, aa.y);
        aaMax.z = max(aaMax.z, aa.z);

        DEBUG_PRN_ACCELEROMETER("calibrateaccel\t");
        DEBUG_PRN_ACCELEROMETER(aaMin.x); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRN_ACCELEROMETER(aaMin.y); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRN_ACCELEROMETER(aaMin.z); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRN_ACCELEROMETER(aaMax.x); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRN_ACCELEROMETER(aaMax.y); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRNL_ACCELEROMETER(aaMax.z);
    }*/

    /*accelGroupTime += (micros() - accelGroupTime0);
    accelTick++;
    if (accelTick % 100 == 0) {
        // 780 us per cycle @ 100 Hz = 7.8% duty cycle when accel is active (w/o Serial.write)
        accelTick = 0;
        Serial.println(millis() - accelGroup);
        Serial.println(accelGroupTime / 100);
        Serial.println("100 accel");
        accelGroup = millis();
        accelGroupTime = 0;
    }*/
}

#endif // _SETUP_MOTION_ADXL345_H_

/* =====
   TEMP CODE THAT IS PROBABLY NOT NECESSARY
   ...but I really don't want to just toss it
   ===== */

    // store previous velocity values
    //av0.x = av.x;
    //av0.y = av.y;
    //av0.z = av.z;

    /*
    // get V and P if we have at least two sets of measurements
    if (accelDiff != 0) {
        // aTD = 1,000,000 us / 50,000 us = 20

        // x = 5, gap = 50000, aTD = 20, xv = (0 + (5 - 0)/2) / 20 = 0 + 2.5 / 20 = 0.125
        // x = 10, gap = 50000, aTD = 20, xv = (5 + (10 - 5)/2) / 20 = 5 + 2.5 / 20 = 0.375

        // at 100hz, each reading is in units of 4mg / (10ms)^2
        // to convert us (s^-6) -> 10ms (s^-2), we divide by 10^4
        //
        // scale factor = 1,000,000 us / (accel measurement time in us)
        //              = 1,000,000 us / (0.01 s @ 100Hz)
        //              = 1,000,000 us / (10 ms @ 100Hz)
        //              = 1,000,000 us / (10,000 us @ 100Hz)
        //              = 100
        //
        // x = 5 (4mg/[10ms]^2), gap = 50,000 us,
        //     xv = (0 (4mg/[10ms]^2) + (5 (4mg/[10ms]^2) - 0 (4mg/[10ms]^2))/2) * 50,000 us * 100
        //        = 0 (4mg/[10ms]^2) + 2.5 (4mg/[10ms]^2) * 50,000 us = 125,000
        // x = 10 (4mg/[10ms]^2), gap = 50,000 us,
        //     xv = (5 (4mg/[10ms]^2) + (10 (4mg/[10ms]^2) - 5 (4mg/[10ms]^2))/2) * 50,000 us * 100
        //        = 5 (4mg/[10ms]^2) + 2.5 (4mg/[10ms]^2) * 50,000 us = 375,000

        // aTD = 1,000,000 us / 50,000 us = 20

        // accelFactor = accelDiff (us) / (1,000,000 (us) / 100 (scale factor))

        // if accelDiff == 10,000 us, which is a perfect interval at 100 Hz, then:
        // accelFactor = 10000 / 10000 = 1 (correct)
        // if accelDiff == 50,000 us, which means 4/5 measurements were missed, then:
        // accelFactor = 50000 / 10000 = 5 (correct)
        
        accelFactor = accelDiff / (1000000L / opt_accel_rate);
        DEBUG_PRNL_ACCELEROMETER(accelFactor);

        // calculate linear velocity (dead reckoning, approximated integral)
        av.x += (ax0 + (ax - ax0)/2) * accelFactor;
        av.y += (ay0 + (ay - ay0)/2) * accelFactor;
        av.z += (az0 + (az - az0)/2) * accelFactor;

        // calculate linear position (dead reckoning, approximated integral)
        ap.x += (av0.x + (av.x - av0.x)/2) * accelFactor;
        ap.y += (av0.y + (av.y - av0.y)/2) * accelFactor;
        ap.z += (av0.z + (av.z - av0.z)/2) * accelFactor;
    }*/

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

    int16_t aa.x = abs(ax), aa.y = abs(ay); //, aa.z = abs(az);
    if (aa.x > 90 || aa.y > 90) az += 180;

    // you can't really get z rotation with just an accelerometer.
    // you need a magnetometer (compass) for absolute heading,
    // or clever use of a gyroscope for relative heading.
    
    // skip angular velocity because it comes from the gyro
    //av.x -= ((ax0 - ax00) - (ax - ax0));
    //av.y -= ((ay0 - ay00) - (ay - ay0));
    //av.z -= ((az0 - az00) - (az - az0));*/
