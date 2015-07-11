// Keyglove controller source code - Feedback declarations for single LED
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

/**
 * @file support_feedback_blink.h
 * @brief Feedback declarations for single LED
 * @author Jeff Rowberg
 * @date 2015-07-03
 */

#ifndef _SUPPORT_FEEDBACK_BLINK_H_
#define _SUPPORT_FEEDBACK_BLINK_H_

/**
 * @brief List of possible values for blink mode
 */
typedef enum {
    KG_BLINK_MODE_OFF = 0,      ///< (0) LED solid off
    KG_BLINK_MODE_SOLID,        ///< (1) LED solid on
    KG_BLINK_MODE_B200_100,     ///< (2) LED blinking with 200ms period, 100ms pulse
    KG_BLINK_MODE_B200_50,      ///< (3) LED blinking with 200ms period, 50ms pulse
    KG_BLINK_MODE_B1000_500,    ///< (4) LED blinking with 1sec period, 500ms pulse
    KG_BLINK_MODE_B1000_100,    ///< (5) LED blinking with 1sec period, 100ms pulse
    KG_BLINK_MODE_B1000_100_2X, ///< (6) LED blinking with 1sec period, 100ms pulse 2x
    KG_BLINK_MODE_B1000_100_3X, ///< (7) LED blinking with 1sec period, 100ms pulse 3x
    KG_BLINK_MODE_B3000_1000,   ///< (8) LED blinking with 3sec period, 1sec pulse
    KG_BLINK_MODE_B3000_100,    ///< (9) LED blinking with 3sec period, 100ms pulse
    KG_BLINK_MODE_B3000_100_2X, ///< (10) LED blinking with 3sec period, 100ms pulse 2x
    KG_BLINK_MODE_B3000_100_3X, ///< (11) LED blinking with 3sec period, 100ms pulse 3x
    KG_BLINK_MODE_MAX
} feedback_blink_mode_t;

extern feedback_blink_mode_t feedbackBlinkMode;

void feedback_set_blink_logic(uint8_t logic);
void feedback_set_blink_mode(feedback_blink_mode_t mode);

void setup_feedback_blink();
void update_feedback_blink();

#endif // _SUPPORT_FEEDBACK_BLINK_H_
