// Keyglove Controller source code - Main setup/loop controller declarations
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
 * @file keyglove.h
 * @brief Main setup/loop controller declarations
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * Essentially every .cpp (implementation) file in the Keyglove project should
 * include this header file.
 */

#ifndef _KEYGLOVE_H_
#define _KEYGLOVE_H_

// UNIVERSAL INCLUDES
#include <Arduino.h>
#include "version.h"
#include "hardware.h"
#include "config.h"

extern volatile uint8_t keyglove100Hz;              ///< Flag for 100Hz hardware timer interrupt
extern uint8_t keygloveTick;                        ///< Fast 100Hz counter, increments every ~10ms and loops at 100
extern uint32_t keygloveTock;                       ///< Slow 1Hz counter (a.k.a. "uptime"), increments every 100 ticks and loops at 2^32 (~4 billion)
//extern uint32_t keygloveTickTime;                   ///< Benchmark testing "end" reference timestamp
//extern uint32_t keygloveTickTime0;                  ///< Benchmark testing "start" reference timestamp

extern uint8_t keygloveSoftTimers;
extern uint8_t keygloveSoftTimersRepeat;
extern uint16_t keygloveSoftTimerInterval[8];
extern uint32_t keygloveSoftTimerSec[8];
extern uint8_t keygloveSoftTimer10ms[8];

extern volatile uint8_t keygloveBatteryInterrupt;   ///< Flag for battery status change interrupt
extern volatile uint8_t keygloveBatteryStatus;      ///< Battery status signal container for post-interrupt processing
extern uint8_t keygloveBatteryLevel;                ///< Battery charge level (0-100)

void keyglove_setup();
void keyglove_loop();

#endif // _KEYGLOVE_H_
