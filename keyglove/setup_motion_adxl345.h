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
uint8_t accelPacket[15] = { 0xB8, 0x0A, KG_PACKET_TYPE_REPORT_ACCEL, 0,0,0, 0,0,0, 0,0,0, 0,0,0 };

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
float opt_accel_lsb_scale = 4;
float opt_accel_rate = 100;

// accelerometer measurements
uint32_t accelGroup;
uint32_t accelGroupTime, accelGroupTime0;
uint8_t accelTick;                  // history position counter
float accelMagnitude;               // magnitude of velocity
int16_t axaRaw, ayaRaw, azaRaw;     // raw accel values (no calibration, no filtering)
int16_t axa, aya, aza;              // immediate axial accel values
int16_t axa0, aya0, aza0;           // previous iteration accel values
int32_t axa2, aya2, aza2;           // x/y/z squared calculations
float axTilt, ayTilt, azTilt;       // x/y/z tilt angles for orientation estimate
float axComp, ayComp, azComp;       // x/y/z vector components for direction w.r.t. gravity
int16_t axv, ayv, azv;              // axial velocities
int16_t axv0, ayv0, azv0;           // previous iteration axial velocities
int16_t axp, ayp, azp;              // axial positions
int16_t axaMin, ayaMin, azaMin;     // minimum values (for calibration)
int16_t axaMax, ayaMax, azaMax;     // maximum values (for calibration)

bool accelEnableAutoZero;
int16_t axaAvg, ayaAvg, azaAvg;     // rolling average for auto-zero adjustment
int16_t axaRef, ayaRef, azaRef;     // reference points for auto-zero detection
uint8_t accelZeroTick;

bool axAutoZero, ayAutoZero, azAutoZero;

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
    axa = aya = aza = 0;
    axv = ayv = azv = 0;
    axp = ayp = azp = 0;
    accelGroup = 0;
    accelGroupTime = 0;
    accelTick = 0;
    axaMin = ayaMin = azaMin = 0;
    axaMax = ayaMax = azaMax = 0;
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
        accelerometer.getAcceleration(&axaRaw, &ayaRaw, &azaRaw);
    }
    else if (opt_accel_rot90 == 1) { // 90 around x axis:       x = +x, y = -z, z = +y
        accelerometer.getAcceleration(&axaRaw, &azaRaw, &ayaRaw);
        azaRaw = -azaRaw;
    }
    else if (opt_accel_rot90 == 2) { // 90 around y axis:       x = -z, y = +y, z = +x
        accelerometer.getAcceleration(&azaRaw, &ayaRaw, &axaRaw);
        azaRaw = -azaRaw;
    }
    else if (opt_accel_rot90 == 4) { // 90 around z axis;       x = -y, y = +x, z = +z
        accelerometer.getAcceleration(&ayaRaw, &axaRaw, &azaRaw);
        ayaRaw = -ayaRaw;
    }
    else if (opt_accel_rot90 == 3) { // 90 around x, y axes:    x = -z, y = -x, z = +y
        accelerometer.getAcceleration(&azaRaw, &axaRaw, &ayaRaw);
        axaRaw = -axaRaw;
        azaRaw = -azaRaw;
    }
    else if (opt_accel_rot90 == 5) { // 90 around x, z axes:    x = -y, y = -z, z = +x
        accelerometer.getAcceleration(&ayaRaw, &azaRaw, &axaRaw);
        ayaRaw = -ayaRaw;
        azaRaw = -azaRaw;
    }
    else if (opt_accel_rot90 == 6) { // 90 around y, z axes:    x = -z, y = +x, z = -y
        accelerometer.getAcceleration(&azaRaw, &axaRaw, &ayaRaw);
        ayaRaw = -ayaRaw;
        azaRaw = -azaRaw;
    }
    else if (opt_accel_rot90 == 7) { // 90 around x, y, z axes: x = -z, y = +y, z = +x
        accelerometer.getAcceleration(&azaRaw, &ayaRaw, &axaRaw);
        azaRaw = -azaRaw;
    }

    DEBUG_PRN_ACCELEROMETER("accel\t");
    DEBUG_PRN_ACCELEROMETER(axaRaw); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(ayaRaw); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(azaRaw); DEBUG_PRN_ACCELEROMETER("\t");

    // auto zero detection (like a ThinkPad TrackPoint)
    if (accelEnableAutoZero) {
        if (accelZeroTick == 0) {
            accelZeroTick++;
            axaRef = axaAvg = axaRaw;
            ayaRef = ayaAvg = ayaRaw;
            azaRef = azaAvg = azaRaw;
        } else {
            if (abs(axaRaw - axaRef) > 35 || abs(ayaRaw - ayaRef) > 35 || abs(azaRaw - azaRef) > 35) {
                // too much deviation, reset zero detection
                accelZeroTick = 0;
            } else if (accelZeroTick == 50) {
                // no deviation for 50 iterations, so adjust auto zero offset
                accelZeroTick = 0;  // reset zero detection to continue fine-tuning if necessary
                axaRef = axaAvg;    // reset the reference point to the new average
                ayaRef = ayaAvg;
                azaRef = azaAvg;
                opt_accel_offset[0] = -axaAvg;  // update offsets
                opt_accel_offset[1] = -ayaAvg;
                opt_accel_offset[2] = (250 - azaAvg);
                accelEnableAutoZero = false;
            } else if (accelZeroTick < 50) {
                // no deviation, but not enough data to zero
                // update running average: avg += (current - avg) / iteration
                accelZeroTick++;
                axaAvg += (axaRaw - axaAvg) / accelZeroTick;
                ayaAvg += (ayaRaw - ayaAvg) / accelZeroTick;
                azaAvg += (azaRaw - azaAvg) / accelZeroTick;
            }
        }
    }

    // offset
    axaRaw += opt_accel_offset[0];
    ayaRaw += opt_accel_offset[1];
    azaRaw += opt_accel_offset[2];

    // rescale if necessary
    if (opt_accel_calibrate[0] != 1) axaRaw = (float)axaRaw * opt_accel_calibrate[0];
    if (opt_accel_calibrate[1] != 1) ayaRaw = (float)ayaRaw * opt_accel_calibrate[1];
    if (opt_accel_calibrate[2] != 1) azaRaw = (float)azaRaw * opt_accel_calibrate[2];

    // store previous acceleration values
    axa0 = axa;
    aya0 = aya;
    aza0 = aza;

    // Kalman filtering
    axa = axa0 + (opt_accel_kalman_constant * (axaRaw - axa0));
    aya = aya0 + (opt_accel_kalman_constant * (ayaRaw - aya0));
    aza = aza0 + (opt_accel_kalman_constant * (azaRaw - aza0));
    
    accelPacket[3] = (uint8_t)axa >> 3; // cut range down by 8x for velocity (1 LSB = 32 mg, so 1g ~= 31 LSB)
    accelPacket[4] = (uint8_t)aya >> 3;
    accelPacket[5] = (uint8_t)aza >> 3;

    DEBUG_PRN_ACCELEROMETER(axa); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(aya); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(aza); DEBUG_PRN_ACCELEROMETER("\t");

    /*
    // obtain magnitude of acceleration
    // (accelMagnitude is expressed in "g", should always be ~1 if holding still)
    axa2 = (int32_t)axa * axa;
    aya2 = (int32_t)aya * aya;
    aza2 = (int32_t)aza * aza;
    accelMagnitude = (sqrt(axa2 + aya2 + aza2) / 250);

    // obtain tilt angle estimation relative to gravity
    // (all measurements are in radians)
    axTilt = atan((float)axa / sqrt(aya2 + aza2)); // will be 0 on a flat surface
    ayTilt = atan((float)aya / sqrt(axa2 + aza2)); // will be 0 on a flat surface
    azTilt = atan((float)aza / sqrt(axa2 + aya2)); // will be PI/2 (+90 degrees) right side up on a flat surface

    // approximate velocity (time interval is constant @ accel rate)
    axv += ((axa + axa0) / 2) >> 5;
    ayv += ((aya + aya0) / 2) >> 5;
    azv += ((aza + aza0) / 2) >> 5;

    accelPacket[6] = (uint8_t)axv;
    accelPacket[7] = (uint8_t)ayv;
    accelPacket[8] = (uint8_t)azv;

    DEBUG_PRN_ACCELEROMETER(axv); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(ayv); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(azv); DEBUG_PRN_ACCELEROMETER("\t");

    // approximate position (time interval is constant @ accel rate)
    axp += ((axv + axv0) / 2) >> 2;
    ayp += ((ayv + ayv0) / 2) >> 2;
    azp += ((azv + azv0) / 2) >> 2;

    accelPacket[9] = (uint8_t)axp;
    accelPacket[10] = (uint8_t)ayp;
    accelPacket[11] = (uint8_t)azp;

    DEBUG_PRN_ACCELEROMETER(axp); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(ayp); DEBUG_PRN_ACCELEROMETER("\t");
    DEBUG_PRN_ACCELEROMETER(azp); DEBUG_PRN_ACCELEROMETER("\t");
    */

    //Serial.write(accelPacket, 6);

    if (opt_enable_calibration) {
        axaMin = min(axaMin, axa);
        ayaMin = min(ayaMin, aya);
        azaMin = min(azaMin, aza);
        axaMax = max(axaMax, axa);
        ayaMax = max(ayaMax, aya);
        azaMax = max(azaMax, aza);

        DEBUG_PRN_ACCELEROMETER("calibrateaccel\t");
        DEBUG_PRN_ACCELEROMETER(axaMin); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRN_ACCELEROMETER(ayaMin); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRN_ACCELEROMETER(azaMin); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRN_ACCELEROMETER(axaMax); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRN_ACCELEROMETER(ayaMax); DEBUG_PRN_ACCELEROMETER("\t");
        DEBUG_PRNL_ACCELEROMETER(azaMax);
    }

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
    //axv0 = axv;
    //ayv0 = ayv;
    //azv0 = azv;

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
        axv += (ax0 + (ax - ax0)/2) * accelFactor;
        ayv += (ay0 + (ay - ay0)/2) * accelFactor;
        azv += (az0 + (az - az0)/2) * accelFactor;

        // calculate linear position (dead reckoning, approximated integral)
        axp += (axv0 + (axv - axv0)/2) * accelFactor;
        ayp += (ayv0 + (ayv - ayv0)/2) * accelFactor;
        azp += (azv0 + (azv - azv0)/2) * accelFactor;
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

    int16_t axa = abs(ax), aya = abs(ay); //, aza = abs(az);
    if (axa > 90 || aya > 90) az += 180;

    // you can't really get z rotation with just an accelerometer.
    // you need a magnetometer (compass) for absolute heading,
    // or clever use of a gyroscope for relative heading.
    
    // skip angular velocity because it comes from the gyro
    //axv -= ((ax0 - ax00) - (ax - ax0));
    //ayv -= ((ay0 - ay00) - (ay - ay0));
    //azv -= ((az0 - az00) - (az - az0));*/
