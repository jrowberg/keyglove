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



#ifndef _SUPPORT_MOTION_ITG3200_H_
#define _SUPPORT_MOTION_ITG3200_H_

#ifdef NO_I2C_SUPPORT
    #error Selected board has no I2C support. I2C devices cannot be enabled under these circumstances.
#endif

#include "ITG3200.h"

ITG3200 gyroscope = ITG3200();

bool activeGyroscope;
volatile bool readyGyroscopeData;
uint8_t gyroPacket[9] = { 0xB8, 0x07, KG_PACKET_TYPE_REPORT_GYRO, 0,0, 0,0, 0,0 };

//int16_t opt_gyro_offset[] = { 0, 0, 0 };        // amount to offset raw gyroscope readings [x,y,z]
//float opt_gyro_calibrate[] = { 1, 1, 1 };       // amount to scale raw gyroscope readings [x,y,z] (post-offset)

int16_t opt_gyro_offset[] = { 0, 0, 0 }; //-590, 210, 50 };
float opt_gyro_calibrate[] = { 1, 1, 1 };

float opt_gyro_kalman_constant = 0.25;
uint8_t opt_gyro_autozero_samples = 50;
uint8_t opt_gyro_autozero_threshold = 25;
uint8_t opt_gyro_smooth_average = 0;
uint8_t opt_gyro_rot90 = 0;
//float opt_gyro_lsb_scale = 14.375; // ITG-3200 sensitivity is 14.375 LSBs per degree/sec

// gyroscope measurements
uint32_t gyroGroup;
uint32_t gyroGroupTime, gyroGroupTime0;
uint8_t gyroTick;                     // history position counter
VectorInt16 gvRaw;                    // raw rotational velocity
VectorInt16 gv;                       // filtered rotational velocity
VectorInt16 gv0;                      // last-iteration filtered rotational velocity
//VectorInt16 gp;                       // gyro position values
//VectorInt16 gvMin;                    // minimum values (for calibration)
//VectorInt16 gvMax;                    // maximum values (for calibration)

bool gyroEnableAutoZero;
VectorInt16 gvAvg;                    // rolling average for auto-zero adjustment
VectorInt16 gvRef;                    // reference points for auto-zero detection
uint8_t gyroZeroTick;

//uint8_t calGyro = 0;
//uint8_t gyroCalibrated = false;
//bool gxAutoZero, gyAutoZero, gzAutoZero;

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
    gyroscope.setInterruptLatch(1); // latch until read
    gyroscope.setInterruptLatchClear(1); // clear on any read
    gyroscope.setIntDataReadyEnabled(1); // trigger interrupt on data ready
    activeGyroscope = false;
    readyGyroscopeData = false;
    gyroEnableAutoZero = true;
}

void enable_motion_gyroscope() {
    gv.x = gv.y = gv.z = 0;
    //gp.x = gp.y = gp.z = 0;
    gyroGroup = 0;
    gyroGroupTime = 0;
    gyroTick = 0;
    gyroZeroTick = 0;
    //gyroCalibrated = false;
    //gvMin.x = gvMin.y = gvMin.z = 0;
    //gvMax.x = gvMax.y = gvMax.z = 0;
    activeGyroscope = true;
    attachInterrupt(5, itg3200_interrupt, FALLING);
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
        gyroscope.getRotation(&gvRaw.x, &gvRaw.y, &gvRaw.z);
    }
    else if (opt_gyro_rot90 == 1) { // 90 around x axis:       x = +x, y = -z, z = +y
        gyroscope.getRotation(&gvRaw.x, &gvRaw.z, &gvRaw.y);
        gvRaw.z = -gvRaw.z;
    }
    else if (opt_gyro_rot90 == 2) { // 90 around y axis:       x = -z, y = +y, z = +x
        gyroscope.getRotation(&gvRaw.z, &gvRaw.y, &gvRaw.x);
        gvRaw.z = -gvRaw.z;
    }
    else if (opt_gyro_rot90 == 4) { // 90 around z axis;       x = -y, y = +x, z = +z
        gyroscope.getRotation(&gvRaw.y, &gvRaw.x, &gvRaw.z);
        gvRaw.y = -gvRaw.y;
    }
    else if (opt_gyro_rot90 == 3) { // 90 around x, y axes:    x = -z, y = -x, z = +y
        gyroscope.getRotation(&gvRaw.z, &gvRaw.x, &gvRaw.y);
        gvRaw.x = -gvRaw.x;
        gvRaw.z = -gvRaw.z;
    }
    else if (opt_gyro_rot90 == 5) { // 90 around x, z axes:    x = -y, y = -z, z = +x
        gyroscope.getRotation(&gvRaw.y, &gvRaw.z, &gvRaw.x);
        gvRaw.y = -gvRaw.y;
        gvRaw.z = -gvRaw.z;
    }
    else if (opt_gyro_rot90 == 6) { // 90 around y, z axes:    x = -z, y = +x, z = -y
        gyroscope.getRotation(&gvRaw.z, &gvRaw.x, &gvRaw.y);
        gvRaw.y = -gvRaw.y;
        gvRaw.z = -gvRaw.z;
    }
    else if (opt_gyro_rot90 == 7) { // 90 around x, y, z axes: x = -z, y = +y, z = +x
        gyroscope.getRotation(&gvRaw.z, &gvRaw.y, &gvRaw.x);
        gvRaw.z = -gvRaw.z;
    }
    
    // convert to degrees/sec
    gvRaw.x /= 14.375;
    gvRaw.y /= 14.375;
    gvRaw.z /= 14.375;

    DEBUG_GYROSCOPE(Serial.print("gyro\t"));
    DEBUG_GYROSCOPE(Serial.print(gvRaw.x)); DEBUG_GYROSCOPE(Serial.print("\t"));
    DEBUG_GYROSCOPE(Serial.print(gvRaw.y)); DEBUG_GYROSCOPE(Serial.print("\t"));
    DEBUG_GYROSCOPE(Serial.print(gvRaw.z)); DEBUG_GYROSCOPE(Serial.print("\t"));

    // auto zero detection (like a ThinkPad TrackPoint)
    if (gyroEnableAutoZero) {
        if (gyroZeroTick == 0) {
            gyroZeroTick++;
            gvRef.x = gvAvg.x = gvRaw.x;
            gvRef.y = gvAvg.y = gvRaw.y;
            gvRef.z = gvAvg.z = gvRaw.z;
        } else {
            if (abs(gvRaw.x - gvRef.x) > 20 || abs(gvRaw.y - gvRef.y) > 20 || abs(gvRaw.z - gvRef.z) > 20) {
                // too much deviation, reset zero detection
                gyroZeroTick = 0;
            } else if (gyroZeroTick == 30) {
                // no deviation for 50 iterations, so adjust auto zero offset
                gyroZeroTick = 0;   // reset zero detection to continue fine-tuning if necessary
                gvRef.x = gvAvg.x;    // reset the reference point to the new average
                gvRef.y = gvAvg.y;
                gvRef.z = gvAvg.z;
                opt_gyro_offset[0] = -gvAvg.x;   // update offsets
                opt_gyro_offset[1] = -gvAvg.y;
                opt_gyro_offset[2] = -gvAvg.z;
                gyroEnableAutoZero = false;
            } else if (gyroZeroTick < 30) {
                // no deviation, but not enough data to zero
                // update running average: avg += (current - avg) / iteration
                gyroZeroTick++;
                gvAvg.x += (gvRaw.x - gvAvg.x) / gyroZeroTick;
                gvAvg.y += (gvRaw.y - gvAvg.y) / gyroZeroTick;
                gvAvg.z += (gvRaw.z - gvAvg.z) / gyroZeroTick;
            }
        }
    }

    // offset
    gvRaw.x += opt_gyro_offset[0];
    gvRaw.y += opt_gyro_offset[1];
    gvRaw.z += opt_gyro_offset[2];

    // rescale if necessary
    if (opt_gyro_calibrate[0] != 1) gvRaw.x = (float)gvRaw.x * opt_gyro_calibrate[0];
    if (opt_gyro_calibrate[1] != 1) gvRaw.y = (float)gvRaw.y * opt_gyro_calibrate[1];
    if (opt_gyro_calibrate[2] != 1) gvRaw.z = (float)gvRaw.z * opt_gyro_calibrate[2];

    // store previous acceleration values
    gv0.x = gv.x;
    gv0.y = gv.y;
    gv0.z = gv.z;

    // simplified Kalman filtering
    gv.x = gv0.x + (opt_gyro_kalman_constant * (gvRaw.x - gv0.x));
    gv.y = gv0.y + (opt_gyro_kalman_constant * (gvRaw.y - gv0.y));
    gv.z = gv0.z + (opt_gyro_kalman_constant * (gvRaw.z - gv0.z));
    
    gyroPacket[3] = (uint8_t)(gv.x >> 8);
    gyroPacket[4] = (uint8_t)gv.x;
    gyroPacket[5] = (uint8_t)(gv.y >> 8);
    gyroPacket[6] = (uint8_t)gv.y;
    gyroPacket[7] = (uint8_t)(gv.z >> 8);
    gyroPacket[8] = (uint8_t)gv.z;

    DEBUG_GYROSCOPE(Serial.print(gv.x)); DEBUG_GYROSCOPE(Serial.print("\t"));
    DEBUG_GYROSCOPE(Serial.print(gv.y)); DEBUG_GYROSCOPE(Serial.print("\t"));
    DEBUG_GYROSCOPE(Serial.println(gv.z));

    #if (KG_HOSTIF > 0)
        txPacketLength = create_packet(txPacket, packetFormatBinary, KG_PACKET_GYRO_FILTERED16);
        if (txPacket != NULL) send_keyglove_packet(txPacket, txPacketLength, true);
    #endif

    /*
    // approximate position (time interval is constant @ gyro rate)
    gp.x += ((gv.x + gv0.x) / 2);
    gp.y += ((gv.y + gv0.x) / 2);
    gp.z += ((gv.z + gv0.x) / 2);

    gyroPacket[6] = (uint8_t)gp.x >> 2;
    gyroPacket[7] = (uint8_t)gp.y >> 2;
    gyroPacket[8] = (uint8_t)gp.z >> 2;

    /*
    DEBUG_GYROSCOPE(Serial.print(gp.x)); DEBUG_GYROSCOPE(Serial.print("\t"));
    DEBUG_GYROSCOPE(Serial.print(gp.y)); DEBUG_GYROSCOPE(Serial.print("\t"));
    DEBUG_GYROSCOPE(Serial.print(gp.z)); DEBUG_GYROSCOPE(Serial.print("\t"));

    //Serial.write(gyroPacket, 9);

    if (opt_enable_calibration) {
        gvMin.x = min(gvMin.x, gv.x);
        gvMin.y = min(gvMin.y, gv.y);
        gvMin.z = min(gvMin.z, gv.z);
        gvMax.x = max(gvMax.x, gv.x);
        gvMax.y = max(gvMax.y, gv.y);
        gvMax.z = max(gvMax.z, gv.z);

        DEBUG_GYROSCOPE(Serial.print("calibrategyro\t"));
        DEBUG_GYROSCOPE(Serial.print(gvMin.x)); DEBUG_GYROSCOPE(Serial.print("\t"));
        DEBUG_GYROSCOPE(Serial.print(gvMin.y)); DEBUG_GYROSCOPE(Serial.print("\t"));
        DEBUG_GYROSCOPE(Serial.print(gvMin.z)); DEBUG_GYROSCOPE(Serial.print("\t"));
        DEBUG_GYROSCOPE(Serial.print(gvMax.x)); DEBUG_GYROSCOPE(Serial.print("\t"));
        DEBUG_GYROSCOPE(Serial.print(gvMax.y)); DEBUG_GYROSCOPE(Serial.print("\t"));
        DEBUG_GYROSCOPE(Serial.println(gvMax.z));
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

#endif // _SUPPORT_MOTION_ITG3200_H_