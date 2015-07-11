// Keyglove Controller source code - Main setup/loop controller declarations
// 2015-07-03 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2015 Jeff Rowberg

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

/* X *
 * @defgroup system System Core
 * @defgroup config Configuration
 * @defgroup protocol KGAPI Protocol
 * @defgroup feedback Feedback
 * @defgroup touch Touch Sensors
 * @defgroup motion Motion Sensors
 * @defgroup flex Flex Sensors
 * @defgroup pressure Pressure Sensors
 * @defgroup bluetooth Bluetooth
 */

/**
 * @file keyglove.h
 * @brief Main setup/loop controller declarations
 * @author Jeff Rowberg
 * @date 2015-07-03
 *
 * Essentially every .cpp (implementation) file in the Keyglove project should
 * include this header file.
 */

#ifndef _KEYGLOVE_H_
#define _KEYGLOVE_H_

// UNIVERSAL INCLUDES
#include <I2Cdev.h>
#include "version.h"
#include "hardware.h"
#include "config.h"

extern volatile uint8_t keyglove100Hz;
extern uint8_t keygloveTick;
extern uint32_t keygloveTock;
//extern uint32_t keygloveTickTime;
//extern uint32_t keygloveTickTime0;

extern uint8_t keygloveSoftTimers;
extern uint8_t keygloveSoftTimersRepeat;
extern uint16_t keygloveSoftTimerInterval[8];
extern uint32_t keygloveSoftTimerSec[8];
extern uint8_t keygloveSoftTimer10ms[8];

extern volatile uint8_t keygloveBatteryInterrupt;
extern volatile uint8_t keygloveBatteryStatus;
extern uint8_t keygloveBatteryLevel;

#endif // _KEYGLOVE_H_
