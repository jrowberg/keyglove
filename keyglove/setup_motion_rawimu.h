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



#ifndef _SETUP_MOTION_RAWIMU_H_
#define _SETUP_MOTION_RAWIMU_H_

// system constants
#define delta_t 0.01f // sampling period in seconds (shown as 10 ms)
#define gyroMeasError 3.14159265358979f * (5.0f / 180.0f) // gyroscope measurement error in rad/s (shown as 5 deg/s)
#define beta sqrt(3.0f / 4.0f) * gyroMeasError // compute beta

// global system variables
float q[4] = { 1.0f, 0.0f, 0.0f, 0.0f }; // estimated orientation quaternion elements with initial conditions
float g[3]; // gravity vector components
float q_qdir[4]; // quaternion derivative from gyroscope elements
float q_gerr[4]; // estimated direction of the gyroscope error
float q_half[4]; // containers to store multiple-use calculated quantities
float q_two[3];  // containers to store multiple-use calculated quantities
float q_objf[3]; // objective function elements
float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33; // objective function Jacobian elements
float q_norm; // vector norm

float angles[3]; // Euler angle container
float ypr[3]; // yaw-pitch-roll container

int16_t axaReal, ayaReal, azaReal; // real acceleration with gravity component removed
int16_t axaGrav, ayaGrav, azaGrav; // real acceleration with respect to gravity plane
float xTilt, yTilt, zTilt; // tilt angles with respect to gravity

int16_t directionRef[3] = { 0, 0, 0 };
uint8_t directionZeroTick = 0;
uint32_t testCount = 0, tb1, tb2;
int8_t direction[3] = { 0, 0, 0 };
uint8_t d[6] = { 0, 0, 0, 0, 0, 0 };

uint8_t quatPacket[11] = { 0xB8, 0x09, 0x11, 0,0, 0,0, 0,0, 0,0 };
uint8_t eulerPacket[9] = { 0xB8, 0x07, 0x12, 0,0, 0,0, 0,0 };
uint8_t yprPacket[9] = { 0xB8, 0x07, 0x13, 0,0, 0,0, 0,0 };
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

void update_imu_filter(float gx, float gy, float gz, float ax, float ay, float az) {
    if (q[0] == 0 && q[1] == 0 && q[2] == 0 && q[3] == 0) q[0] = 1.0f;

    // auxiliary variables to avoid reapeated calculations
    q_half[0] = 0.5f * q[0];
    q_half[1] = 0.5f * q[1];
    q_half[2] = 0.5f * q[2];
    q_half[3] = 0.5f * q[3];
    q_two[0] = 2.0f * q[0];
    q_two[1] = 2.0f * q[1];
    q_two[2] = 2.0f * q[2];

    // normalize the accelerometer measurement
    q_norm = sqrt(ax*ax + ay*ay + az*az);
    ax /= q_norm;
    ay /= q_norm;
    az /= q_norm;

    // compute the objective function and Jacobian
    q_objf[0] = q_two[1] * q[3] - q_two[0] * q[2] - ax;
    q_objf[1] = q_two[0] * q[1] + q_two[2] * q[3] - ay;
    q_objf[2] = 1.0f - q_two[1] * q[1] - q_two[2] * q[2] - az;
    J_11or24 = q_two[2]; // J_11 negated in matrix multiplication
    J_12or23 = 2.0f * q[3];
    J_13or22 = q_two[0]; // J_12 negated in matrix multiplication
    J_14or21 = q_two[1];
    J_32 = 2.0f * J_14or21; // negated in matrix multiplication
    J_33 = 2.0f * J_11or24; // negated in matrix multiplication
    
    // compute the gradient (matrix multiplication)
    q_gerr[0] = J_14or21 * q_objf[1] - J_11or24 * q_objf[0];
    q_gerr[1] = J_12or23 * q_objf[0] + J_13or22 * q_objf[1] - J_32 * q_objf[2];
    q_gerr[2] = J_12or23 * q_objf[1] - J_33 * q_objf[2] - J_13or22 * q_objf[0];
    q_gerr[3] = J_14or21 * q_objf[0] + J_11or24 * q_objf[1];

    // normalize the gradient
    q_norm = sqrt(q_gerr[0] * q_gerr[0] + q_gerr[1] * q_gerr[1] + q_gerr[2] * q_gerr[2] + q_gerr[3] * q_gerr[3]);
    q_gerr[0] /= q_norm;
    q_gerr[1] /= q_norm;
    q_gerr[2] /= q_norm;
    q_gerr[3] /= q_norm;

    // compute the quaternion derivative measured by gyroscopes
    q_qdir[0] = -q_half[1] * gx - q_half[2] * gy - q_half[3] * gz;
    q_qdir[1] = q_half[0] * gx + q_half[2] * gz - q_half[3] * gy;
    q_qdir[2] = q_half[0] * gy - q_half[1] * gz + q_half[3] * gx;
    q_qdir[3] = q_half[0] * gz + q_half[1] * gy - q_half[2] * gx;
    
    // compute, then integrate the estimated quaternion derivative
    q[0] += (q_qdir[0] - (beta * q_gerr[0])) * delta_t;
    q[1] += (q_qdir[1] - (beta * q_gerr[1])) * delta_t;
    q[2] += (q_qdir[2] - (beta * q_gerr[2])) * delta_t;
    q[3] += (q_qdir[3] - (beta * q_gerr[3])) * delta_t;

    // normalize quaternion
    q_norm = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
    q[0] /= q_norm;
    q[1] /= q_norm;
    q[2] /= q_norm;
    q[3] /= q_norm;
}

void setup_motion() {
    q[0] = 1.0f;
    q[1] = 0.0f;
    q[2] = 0.0f;
    q[3] = 0.0f;
}

void update_motion() {
    //tb1 = micros();

    // update IMU filter with current readings
    update_imu_filter(gxv * (PI/(180*PI)), gyv * (PI/(180*PI)), gzv * (PI/(180*PI)), axa, aya, aza);

    /*quatPacket[3] = (uint8_t)(round(q[0] * 256));
    quatPacket[4] = (uint8_t)(round(q[0] * 65536));
    quatPacket[5] = (uint8_t)(round(q[1] * 256));
    quatPacket[6] = (uint8_t)(round(q[1] * 65536));
    quatPacket[7] = (uint8_t)(round(q[2] * 256));
    quatPacket[8] = (uint8_t)(round(q[2] * 65536));
    quatPacket[9] = (uint8_t)(round(q[3] * 256));
    quatPacket[10] = (uint8_t)(round(q[3] * 65536));
    Serial.write(quatPacket, 11); //*/

    teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
    teapotPacket[2] = (uint8_t)(round(q[0] * 16384) >> 8);
    teapotPacket[3] = (uint8_t)(round(q[0] * 16384));
    teapotPacket[4] = (uint8_t)(round(-q[1] * 16384) >> 8);
    teapotPacket[5] = (uint8_t)(round(-q[1] * 16384));
    teapotPacket[6] = (uint8_t)(round(-q[2] * 16384) >> 8);
    teapotPacket[7] = (uint8_t)(round(-q[2] * 16384));
    teapotPacket[8] = (uint8_t)(round(q[3] * 16384) >> 8);
    teapotPacket[9] = (uint8_t)(round(q[3] * 16384));
    Serial.write(teapotPacket, 14); //*/
    return;

    // calculate Euler angles (in degrees, not radians)
    // psi:
    angles[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1) * 180/M_PI;
    // theta:
    angles[1] = -asin(2 * q[1] * q[3] + 2 * q[0] * q[2]) * 180/M_PI;
    // phi:
    angles[2] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1) * 180/M_PI;
    
    /*Serial.print(angles[0]); Serial.print("\t");
    Serial.print(angles[1]); Serial.print("\t");
    Serial.println(angles[2]);*/

    eulerPacket[3] = (uint8_t)(round(angles[0]) >> 8);
    eulerPacket[4] = (uint8_t)round(angles[0]);
    eulerPacket[5] = (uint8_t)(round(angles[1]) >> 8);
    eulerPacket[6] = (uint8_t)round(angles[1]);
    eulerPacket[7] = (uint8_t)(round(angles[2]) >> 8);
    eulerPacket[8] = (uint8_t)round(angles[2]);
    //Serial.write(eulerPacket, 9);

    // get gravity vector components
    g[0] = 2 * (q[1]*q[3] - q[0]*q[2]);
    g[1] = 2 * (q[0]*q[1] + q[2]*q[3]);
    g[2] = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];

    // calculate yaw/pitch/roll with respect to gravity (in degrees, not radians)
    // yaw: (about Z axis)
    ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1) * 180/M_PI;
    // pitch: (nose up/down)
    ypr[1] = atan(g[0] / sqrt(g[1]*g[1] + g[2]*g[2])) * 180/M_PI;
    // roll: (tilt left/right)
    ypr[2] = atan(g[1] / sqrt(g[0]*g[0] + g[2]*g[2])) * 180/M_PI;

    yprPacket[3] = (uint8_t)(round(ypr[0]) >> 8);
    yprPacket[4] = (uint8_t)round(ypr[0]);
    yprPacket[5] = (uint8_t)(round(ypr[1]) >> 8);
    yprPacket[6] = (uint8_t)round(ypr[1]);
    yprPacket[7] = (uint8_t)(round(ypr[2]) >> 8);
    yprPacket[8] = (uint8_t)round(ypr[2]);
    Serial.write(yprPacket, 9);

    // get gravity-free acceleration components
    axaReal = (axa - g[0]*250);
    ayaReal = (aya - g[1]*250);
    azaReal = (aza - g[2]*250);
    
    // calculate tilt angle w.r.t. gravity
    xTilt = atan2(g[0], sqrt(g[1]*g[1] + g[2]*g[2])); // will be 0 on a flat surface
    yTilt = atan2(g[1], sqrt(g[0]*g[0] + g[2]*g[2])); // will be 0 on a flat surface
    zTilt = atan2(g[2], sqrt(g[0]*g[0] + g[1]*g[1])); // will be PI/2 (+90 degrees) right side up on a flat surface

    // calculate best-guess gravity-plane acceleration
    // (true accuracy requires more expressions and a magnetometer)
    axaGrav = axaReal - axaReal*sin(xTilt);
    ayaGrav = ayaReal - ayaReal*sin(yTilt);
    azaGrav = azaReal - azaReal*cos(zTilt);

    /*direction[0] += (axaGrav0 + axaGrav) / 64;
    direction[1] += (ayaGrav0 + ayaGrav) / 64;
    direction[2] += (azaGrav0 + azaGrav) / 64;

    if (directionZeroTick == 0) {
        directionZeroTick++;
        directionRef[0] = direction[0];
        directionRef[1] = direction[1];
        directionRef[2] = direction[2];
    } else {
        if (abs(direction[0] - directionRef[0]) > 3 || abs(direction[1] - directionRef[1]) > 3 || abs(direction[2] - directionRef[2]) > 3) {
            // too much deviation, reset zero detection
            directionZeroTick = 0;
        } else if (directionZeroTick == 10) {
            // no deviation for 10 iterations, so adjust auto zero offset
            directionZeroTick = 0;  // reset zero detection to continue fine-tuning if necessary
            direction[0] = 0;  // update offsets
            direction[1] = 0;
            direction[2] = 0;
            memset(d, 0, 6);
        } else if (directionZeroTick < 10) {
            // no deviation, but not enough data to zero
            directionZeroTick++;
        }
    }
    
    if (direction[0] < -6) d[1]++; // right
    else if (direction[0] > 6) d[0]++; // left
    if (direction[1] < -6) d[3]++; // forward
    else if (direction[1] > 6) d[2]++; // backward
    if (direction[2] < -6) d[4]++; // down
    else if (direction[2] > 6) d[5]++; // up
    
    if (d[0] == 3) Serial.println("LEFT");
    if (d[1] == 3) Serial.println("RIGHT");
    if (d[2] == 3) Serial.println("BACKWARD");
    if (d[3] == 3) Serial.println("FORWARD");
    if (d[4] == 3) Serial.println("DOWN");
    if (d[5] == 3) Serial.println("UP");

    Serial.print(direction[0]); Serial.print("\t");
    Serial.print(direction[1]); Serial.print("\t");
    Serial.println(direction[2]); //*/
}

#endif // _SETUP_MOTION_H_