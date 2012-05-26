// Keyglove controller source code - Special hardware setup file
// 11/4/2011 by Jeff Rowberg <jeff@rowberg.net>

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



#ifndef _SETUP_MOTION_RAWIMU_H_
#define _SETUP_MOTION_RAWIMU_H_

// system constants
#define delta_t 0.01f // sampling period in seconds (shown as 10 ms)
#define gyroMeasError 3.14159265358979f * (5.0f / 180.0f) // gyroscope measurement error in rad/s (shown as 5 deg/s)
#define beta sqrt(3.0f / 4.0f) * gyroMeasError // compute beta

// global system variables
Quaternion q; // orientation calculated by IMU
Quaternion q_dir, q_gerr, q_half, q_two;
float q_objf[3]; // objective function elements
float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33; // objective function Jacobian elements

float angles[3]; // Euler angle components
float ypr[3]; // yaw-pitch-roll components

// variables for feeding and post-processing the IMU filter
VectorFloat aaIMU, gvIMU;
VectorFloat gravity;
VectorInt16 aaReal;
VectorInt16 aaFrame, aaFrame0;
VectorInt16 avFrame, avFrame0, avFrameRef;
VectorInt16 apFrame;

uint8_t directionZeroTick = 0;
uint32_t testCount = 0, tb1, tb2;
int8_t d[3] = { 0, 0, 0 };

uint8_t quatPacket[11] = { 0xB8, 0x09, 0x11, 0,0, 0,0, 0,0, 0,0 };
uint8_t eulerPacket[9] = { 0xB8, 0x07, 0x12, 0,0, 0,0, 0,0 };
uint8_t yprPacket[9] = { 0xB8, 0x07, 0x13, 0,0, 0,0, 0,0 };
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

void update_imu_filter(VectorFloat gv, VectorFloat aa) {
    if (q.w == 0.0f && q.x == 0.0f && q.y == 0.0f && q.z == 0.0f) q.w = 1.0f;
    
    // note:
    //  - gv is angular velocity from gyroscope
    //  - aa is linear acceleration from accelerometer

    // auxiliary variables to avoid reapeated calculations
    q_half.w = 0.5f * q.w;
    q_half.x = 0.5f * q.x;
    q_half.y = 0.5f * q.y;
    q_half.z = 0.5f * q.z;
    q_two.w = 2.0f * q.w;
    q_two.x = 2.0f * q.x;
    q_two.y = 2.0f * q.y;

    // normalize the accelerometer measurement
    aa.normalize();

    // compute the objective function and Jacobian
    q_objf[0] = q_two.x * q.z - q_two.w * q.y - aa.x;
    q_objf[1] = q_two.w * q.x + q_two.y * q.z - aa.y;
    q_objf[2] = 1.0f - q_two.x * q.x - q_two.y * q.y - aa.z;
    J_11or24 = q_two.y; // J_11 negated in matrix multiplication
    J_12or23 = 2.0f * q.z;
    J_13or22 = q_two.w; // J_12 negated in matrix multiplication
    J_14or21 = q_two.x;
    J_32 = 2.0f * J_14or21; // negated in matrix multiplication
    J_33 = 2.0f * J_11or24; // negated in matrix multiplication
    
    // compute the gradient (matrix multiplication)
    q_gerr.w = J_14or21 * q_objf[1] - J_11or24 * q_objf[0];
    q_gerr.x = J_12or23 * q_objf[0] + J_13or22 * q_objf[1] - J_32 * q_objf[2];
    q_gerr.y = J_12or23 * q_objf[1] - J_33 * q_objf[2] - J_13or22 * q_objf[0];
    q_gerr.z = J_14or21 * q_objf[0] + J_11or24 * q_objf[1];

    // normalize the gradient
    q_gerr.normalize();

    // compute the quaternion derivative measured by gyroscopes
    q_dir.w = -q_half.x * gv.x - q_half.y * gv.y - q_half.z * gv.z;
    q_dir.x = q_half.w * gv.x + q_half.y * gv.z - q_half.z * gv.y;
    q_dir.y = q_half.w * gv.y - q_half.x * gv.z + q_half.z * gv.x;
    q_dir.z = q_half.w * gv.z + q_half.x * gv.y - q_half.y * gv.x;
    
    // compute, then integrate the estimated quaternion derivative
    q.w += (q_dir.w - (beta * q_gerr.w)) * delta_t;
    q.x += (q_dir.x - (beta * q_gerr.x)) * delta_t;
    q.y += (q_dir.y - (beta * q_gerr.y)) * delta_t;
    q.z += (q_dir.z - (beta * q_gerr.z)) * delta_t;

    // normalize quaternion
    q.normalize();
}

void setup_motion() {
    q.w = 1.0f;
    q.x = 0.0f;
    q.y = 0.0f;
    q.z = 0.0f;
}

void update_motion() {
    //tb1 = micros();

    // gv and aa are Int16 vectors, so we need to copy them in Float vectors
    // (gv needs to be converted to radians/sec and scaled more as well)
    gvIMU.x = gv.x * (PI/(180*4));
    gvIMU.y = gv.y * (PI/(180*4));
    gvIMU.z = gv.z * (PI/(180*4));
    aaIMU.x = aa.x;
    aaIMU.y = aa.y;
    aaIMU.z = aa.z;

    // update IMU filter with current readings
    update_imu_filter(gvIMU, aaIMU);

    /*Serial.print(q.w); Serial.print("\t");
    Serial.print(q.x); Serial.print("\t");
    Serial.print(q.y); Serial.print("\t");
    Serial.println(q.z); //*/

    /*quatPacket[3] = (uint8_t)(round(q.w * 32768) >> 8);
    quatPacket[4] = (uint8_t)(round(q.w * 32768));
    quatPacket[5] = (uint8_t)(round(q.x * 32768) >> 8);
    quatPacket[6] = (uint8_t)(round(q.x * 32768));
    quatPacket[7] = (uint8_t)(round(q.y * 32768) >> 8);
    quatPacket[8] = (uint8_t)(round(q.y * 32768));
    quatPacket[9] = (uint8_t)(round(q.z * 32768) >> 8);
    quatPacket[10] = (uint8_t)(round(q.z * 32768));
    Serial.write(quatPacket, 11); //*/

    /*teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
    teapotPacket[2] = (uint8_t)(round(q.w * 16384) >> 8);
    teapotPacket[3] = (uint8_t)(round(q.w * 16384));
    teapotPacket[4] = (uint8_t)(round(-q.x * 16384) >> 8);
    teapotPacket[5] = (uint8_t)(round(-q.x * 16384));
    teapotPacket[6] = (uint8_t)(round(-q.y * 16384) >> 8);
    teapotPacket[7] = (uint8_t)(round(-q.y * 16384));
    teapotPacket[8] = (uint8_t)(round(q.z * 16384) >> 8);
    teapotPacket[9] = (uint8_t)(round(q.z * 16384));
    Serial.write(teapotPacket, 14); //*/
    //return;

    /*// calculate Euler angles (in degrees, not radians)
    // psi:
    angles[0] = atan2(2 * q.x * q.y - 2 * q.w * q.z, 2 * q.w*q.w + 2 * q.x * q.x - 1) * 180/M_PI;
    // theta:
    angles[1] = -asin(2 * q.x * q.z + 2 * q.w * q.y) * 180/M_PI;
    // phi:
    angles[2] = atan2(2 * q.y * q.z - 2 * q.w * q.x, 2 * q.w * q.w + 2 * q.z * q.z - 1) * 180/M_PI;
    //*/
    
    /*Serial.print(angles[0]); Serial.print("\t");
    Serial.print(angles[1]); Serial.print("\t");
    Serial.println(angles[2]); //*/

    /*eulerPacket[3] = (uint8_t)(round(angles[0]) >> 8);
    eulerPacket[4] = (uint8_t)round(angles[0]);
    eulerPacket[5] = (uint8_t)(round(angles[1]) >> 8);
    eulerPacket[6] = (uint8_t)round(angles[1]);
    eulerPacket[7] = (uint8_t)(round(angles[2]) >> 8);
    eulerPacket[8] = (uint8_t)round(angles[2]);
    Serial.write(eulerPacket, 9); //*/

    // get gravity vector components
    gravity.x = 2 * (q.x*q.z - q.w*q.y);
    gravity.y = 2 * (q.w*q.x + q.y*q.z);
    gravity.z = q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z;

    /*// calculate yaw/pitch/roll with respect to gravity (in degrees, not radians)
    // yaw: (about Z axis)
    ypr[0] = atan2(2 * q.x * q.y - 2 * q.w * q.z, 2 * q.w*q.w + 2 * q.x * q.x - 1) * 180/M_PI;
    // pitch: (nose up/down)
    ypr[1] = atan(gravity.x / sqrt(gravity.y*gravity.y + gravity.z*gravity.z)) * 180/M_PI;
    // roll: (tilt left/right)
    ypr[2] = atan(gravity.y / sqrt(gravity.x*gravity.x + gravity.z*gravity.z)) * 180/M_PI;

    /*Serial.print(ypr[0]); Serial.print("\t");
    Serial.print(ypr[1]); Serial.print("\t");
    Serial.println(ypr[2]); //*/

    /*yprPacket[3] = (uint8_t)(round(ypr[0]) >> 8);
    yprPacket[4] = (uint8_t)round(ypr[0]);
    yprPacket[5] = (uint8_t)(round(ypr[1]) >> 8);
    yprPacket[6] = (uint8_t)round(ypr[1]);
    yprPacket[7] = (uint8_t)(round(ypr[2]) >> 8);
    yprPacket[8] = (uint8_t)round(ypr[2]);
    Serial.write(yprPacket, 9); //*/

    // get gravity-free acceleration components
    // (will be on the scale of +/- 250 = +/- 1g within accelerometer frame)
    aaReal.x = (aa.x - gravity.x*250);
    aaReal.y = (aa.y - gravity.y*250);
    aaReal.z = (aa.z - gravity.z*250);
    
    /*Serial.print(aaReal.x); Serial.print("\t");
    Serial.print(aaReal.y); Serial.print("\t");
    Serial.println(aaReal.z); //*/

    /*// calculate tilt angle w.r.t. gravity
    xTilt = atan2(gravity.x, sqrt(gravity.y*gravity.y + gravity.z*gravity.z)); // will be 0 on a flat surface
    yTilt = atan2(gravity.y, sqrt(gravity.x*gravity.x + gravity.z*gravity.z)); // will be 0 on a flat surface
    zTilt = atan2(gravity.z, sqrt(gravity.x*gravity.x + gravity.y*gravity.y)); // will be PI/2 (+90 degrees) right side up on a flat surface
    //*/
    
    // store last-iteration aaFrame vector for trapezoidal integration estimate later
    aaFrame0.x = aaFrame.x;
    aaFrame0.y = aaFrame.y;
    aaFrame0.z = aaFrame.z;

    // rotate measured 3D acceleration vector into original state
    // frame of reference based on orientation quaternion
    aaFrame = aaReal.getRotated(q);
    
    // simplified Kalman filtering
    aaFrame.x = aaFrame0.x + (0.25 * (aaFrame.x - aaFrame0.x));
    aaFrame.y = aaFrame0.y + (0.25 * (aaFrame.y - aaFrame0.y));
    aaFrame.z = aaFrame0.z + (0.25 * (aaFrame.z - aaFrame0.z));

    /*Serial.print(aaFrame.x); Serial.print("\t");
    Serial.print(aaFrame.y); Serial.print("\t");
    Serial.println(aaFrame.z); //*/
    
    // store last-iteration avFrame vector for trapezoidal integration estimate later
    avFrame0.x = avFrame.x;
    avFrame0.y = avFrame.y;
    avFrame0.z = avFrame.z;

    // integrate acceleration (trapezoidal/average method) to estimate true velocity
    avFrame.x += (aaFrame0.x + aaFrame.x) / 2;
    avFrame.y += (aaFrame0.y + aaFrame.y) / 2;
    avFrame.z += (aaFrame0.z + aaFrame.z) / 2;

    /*Serial.print(avFrame.x); Serial.print("\t");
    Serial.print(avFrame.y); Serial.print("\t");
    Serial.println(avFrame.z); //*/

    // integrate velocity (trapezoidal/average method) to estimate true position
    apFrame.x += (avFrame0.x + avFrame.x) / 64;
    apFrame.y += (avFrame0.y + avFrame.y) / 64;
    apFrame.z += (avFrame0.z + avFrame.z) / 64;

    /*Serial.print(apFrame.x); Serial.print("\t");
    Serial.print(apFrame.y); Serial.print("\t");
    Serial.println(apFrame.z); //*/

    if (directionZeroTick == 0) {
        directionZeroTick++;
        avFrameRef.x = avFrame.x;
        avFrameRef.y = avFrame.y;
        avFrameRef.z = avFrame.z;
    } else {
        if (abs(avFrame.x - avFrameRef.x) > 3 || abs(avFrame.y - avFrameRef.y) > 3 || abs(avFrame.z - avFrameRef.z) > 3) {
            // too much deviation, reset zero detection
            directionZeroTick = 0;
        } else if (directionZeroTick == 10) {
            // no deviation for 10 iterations, so adjust auto zero offset
            directionZeroTick = 0;  // reset zero detection to continue fine-tuning if necessary
            avFrame.x = 0; // clear velocity/position
            avFrame.y = 0;
            avFrame.z = 0;
            apFrame.x = 0;
            apFrame.y = 0;
            apFrame.z = 0;
            memset(d, 0, 3);
        } else if (directionZeroTick < 10) {
            // no deviation, but not enough data to zero
            directionZeroTick++;
        }
    }

    /*
    if (apFrame.x < -100) {
        if (d[1] != 1) Serial.println("RIGHT");
        d[0] = 1; // right
    } else if (apFrame.x > 100) {
        if (d[1] != 1) Serial.println("LEFT");
        d[0] = -1; // left
    } else {
        d[0] = 0; // stopped
    }

    if (apFrame.y < -100) {
        if (d[1] != 1) Serial.println("FORWARD");
        d[1] = 1; // forward
    } else if (apFrame.y > 100) {
        if (d[1] != 1) Serial.println("BACKWARD");
        d[1] = -1; // backward
    } else {
        d[1] = 0; // stopped
    }

    if (apFrame.z < -100) {
        if (d[1] != 1) Serial.println("DOWN");
        d[2] = -1; // down
    } else if (apFrame.z > 100) {
        if (d[1] != 1) Serial.println("UP");
        d[2] = 1; // up
    } else {
        d[2] = 0; // stopped
    } //*/

    /*if (d[0] == -20) Serial.println("LEFT");
    else if (d[0] == 20) Serial.println("RIGHT");
    if (d[1] == -20) Serial.println("BACKWARD");
    else if (d[1] == 20) Serial.println("FORWARD");
    if (d[2] == -20) Serial.println("DOWN");
    else if (d[2] == 20) Serial.println("UP");*/
}

#endif // _SETUP_MOTION_H_