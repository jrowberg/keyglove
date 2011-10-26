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



#ifndef _SETUP_MOTION_H_
#define _SETUP_MOTION_H_

// System constants
#define deltat 0.01f // sampling period in seconds (shown as 10 ms)
#define gyroMeasError 3.14159265358979f * (5.0f / 180.0f) // gyroscope measurement error in rad/s (shown as 5 deg/s)
#define beta sqrt(3.0f / 4.0f) * gyroMeasError // compute beta

// Global system variables
//float ax, ay, az; // accelerometer measurements
//float gx, gy, gz; // gyroscope measurements in rad/s
float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f; // estimated orientation quaternion elements with initial conditions

void setup_motion() {
    q0 = 1.0f;
    q1 = 0.0f;
    q2 = 0.0f;
    q3 = 0.0f;
}

void filterUpdate(float gx, float gy, float gz, float ax, float ay, float az) {
    // local system variables
    float norm; // vector norm
    float q0_qdir, q1_qdir, q2_qdir, q3_qdir; // quaternion derivative from gyroscopes elements
    float f_1, f_2, f_3; // objective function elements
    float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33; // objective function Jacobian elements
    float q0_gerr, q1_gerr, q2_gerr, q3_gerr; // estimated direction of the gyroscope error

    // auxiliary variables to avoid reapeated calcualtions
    float q0_half = 0.5f * q0;
    float q1_half = 0.5f * q1;
    float q2_half = 0.5f * q2;
    float q3_half = 0.5f * q3;
    float q0_two = 2.0f * q0;
    float q1_two = 2.0f * q1;
    float q2_two = 2.0f * q2;

    // normalise the accelerometer measurement
    norm = sqrt(ax * ax + ay * ay + az * az);
    ax /= norm;
    ay /= norm;
    az /= norm;

    // compute the objective function and Jacobian
    f_1 = q1_two * q3 - q0_two * q2 - ax;
    f_2 = q0_two * q1 + q2_two * q3 - ay;
    f_3 = 1.0f - q1_two * q1 - q2_two * q2 - az;
    J_11or24 = q2_two; // J_11 negated in matrix multiplication
    J_12or23 = 2.0f * q3;
    J_13or22 = q0_two; // J_12 negated in matrix multiplication
    J_14or21 = q1_two;
    J_32 = 2.0f * J_14or21; // negated in matrix multiplication
    J_33 = 2.0f * J_11or24; // negated in matrix multiplication
    
    // Compute the gradient (matrix multiplication)
    q0_gerr = J_14or21 * f_2 - J_11or24 * f_1;
    q1_gerr = J_12or23 * f_1 + J_13or22 * f_2 - J_32 * f_3;
    q2_gerr = J_12or23 * f_2 - J_33 * f_3 - J_13or22 * f_1;
    q3_gerr = J_14or21 * f_1 + J_11or24 * f_2;

    // Normalise the gradient
    norm = sqrt(q0_gerr * q0_gerr + q1_gerr * q1_gerr + q2_gerr * q2_gerr + q3_gerr * q3_gerr);
    q0_gerr /= norm;
    q1_gerr /= norm;
    q2_gerr /= norm;
    q3_gerr /= norm;

    // Compute the quaternion derivative measured by gyroscopes
    q0_qdir = -q1_half * gx - q2_half * gy - q3_half * gz;
    q1_qdir = q0_half * gx + q2_half * gz - q3_half * gy;
    q2_qdir = q0_half * gy - q1_half * gz + q3_half * gx;
    q3_qdir = q0_half * gz + q1_half * gy - q2_half * gx;
    
    // Compute then integrate the estimated quaternion derivative
    q0 += (q0_qdir - (beta * q0_gerr)) * deltat;
    q1 += (q1_qdir - (beta * q1_gerr)) * deltat;
    q2 += (q2_qdir - (beta * q2_gerr)) * deltat;
    q3 += (q3_qdir - (beta * q3_gerr)) * deltat;

    // Normalise quaternion
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 /= norm;
    q1 /= norm;
    q2 /= norm;
    q3 /= norm;
}

#endif // _SETUP_MOTION_H_