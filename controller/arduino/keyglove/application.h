// Keyglove controller source code - Custom application behavior declarations
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
 * @file application.h
 * @brief **USER-DEFINED:** Custom application behavior declarations
 * @author Jeff Rowberg
 * @date 2014-11-07
 */

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

uint8_t my_kg_evt_system_ready();
uint8_t my_kg_evt_system_timer_tick(uint8_t handle, uint32_t seconds, uint8_t subticks);
uint8_t my_kg_evt_motion_data(uint8_t index, uint8_t flags, uint8_t data_len, uint8_t *data_data);
uint8_t my_kg_evt_bluetooth_ready();
uint8_t my_kg_evt_touch_status(uint8_t status_len, uint8_t *status_data);

void setup_application();

#endif // _APPLICATION_H_
