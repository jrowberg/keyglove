// Keyglove controller source code - Motion support declarations for hand-mounted MPU-6050 sensor
// 2014-11-07 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2014 Jeff Rowberg

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

/**
 * @file support_motion_mpu6050_hand.h
 * @brief Motion support declarations for hand-mounted MPU-6050 sensor
 * @author Jeff Rowberg
 * @date 2014-11-07
 */

#ifndef _SUPPORT_MOTION_MPU6050_HAND_H_
#define _SUPPORT_MOTION_MPU6050_HAND_H_

#include <I2Cdev.h>
#include <MPU6050.h>

#include "support_helper_3dmath.h"

extern bool mpuHandInterrupt;

extern VectorInt16 aa;
extern VectorInt16 gv;

void motion_mpu6050_hand_interrupt();
void motion_set_mpu6050_hand_mode(uint8_t mode);
void setup_motion_mpu6050_hand();
void update_motion_mpu6050_hand();

#endif // _SUPPORT_MOTION_MPU6050_HAND_H_
