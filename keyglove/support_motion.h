// Keyglove controller source code - General feedback support declarations
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
 * @file support_motion.h
 * @brief General motion support declarations
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * This file a conditional way to include specific types of functional support
 * files for a general category based on Keyglove configuration settings,
 * without needing to have that conditional logic in every implementation file
 * that might need to include the functional support in question. This header
 * deals specifically with feedback support.
 *
 * Normally it is not necessary to edit this file.
 */

#ifndef _SUPPORT_MOTION_H_
#define _SUPPORT_MOTION_H_

#if (KG_MOTION & KG_MOTION_MPU6050_HAND)
 	#include "support_motion_mpu6050_hand.h"
	#define KG_MOTION_SENSOR_COUNT  1
#else
	#define KG_MOTION_SENSOR_COUNT  0
#endif

/**
 * @brief List of possible values for motion sensor mode
 */
typedef enum {
    KG_MOTION_MODE_OFF = 0,		///< (0) Motion sensor disabled
    KG_MOTION_MODE_ON,          ///< (1) Motion sensor enabled
    KG_MOTION_MODE_MAX
} motion_mode_t;

extern motion_mode_t motionMode[KG_MOTION_SENSOR_COUNT];    ///< Motion sensor modes

#endif // _SUPPORT_MOTION_H_
