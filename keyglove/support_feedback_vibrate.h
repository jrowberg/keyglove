// Keyglove controller source code - Feedback declarations for vibration motor
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
 * @file support_feedback_vibrate.h
 * @brief Feedback declarations for vibration motor
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * This file defines the feeback functionality for the vibration motor that may
 * be included as an optional module in a Keyglove modular hardware design. This
 * file is meant to contain only behavioral logic and abstracted functions that
 * do not require specific pin knowledge of the selected MCU. Any pins used
 * should be defined in the selected MCU board support file.
 *
 * Normally it is not necessary to edit this file.
 */

#ifndef _SUPPORT_FEEDBACK_VIBRATE_H_
#define _SUPPORT_FEEDBACK_VIBRATE_H_

/**
 * @brief List of possible values for vibration mode
 */
typedef enum {
    KG_VIBRATE_MODE_OFF = 0,    ///< (0) Vibration solid off
    KG_VIBRATE_MODE_SOLID,      ///< (1) Vibration solid on
    KG_VIBRATE_MODE_LONGBUZZ,   ///< (2) Vibration alternating at 1sec period, 500ms pulse
    KG_VIBRATE_MODE_LONGPULSE,  ///< (3) Vibration alternating at 1sec period, 250ms pulse
    KG_VIBRATE_MODE_SHORTBUZZ,  ///< (4) Vibration alternating at 200ms period, 100ms pulse
    KG_VIBRATE_MODE_SHORTPULSE, ///< (5) Vibration alternating at 200ms period, 50ms pulse
    KG_VIBRATE_MODE_TINYBUZZ,   ///< (6) Vibration alternating at 100ms period, 50ms pulse
    KG_VIBRATE_MODE_MAX
} feedback_vibrate_mode_t;

void feedback_set_vibrate_logic(uint8_t logic);
void feedback_set_vibrate_mode(feedback_vibrate_mode_t mode, uint8_t duration);

void setup_feedback_vibrate();
void update_feedback_vibrate();

#endif // _SUPPORT_FEEDBACK_VIBRATE_H_
