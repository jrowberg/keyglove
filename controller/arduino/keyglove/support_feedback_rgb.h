// Keyglove controller source code - Feedback declarations for RGB LED
// 8/9/2014 by Jeff Rowberg <jeff@rowberg.net>

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
 * @file support_feedback_rgb.h
 * @brief Feedback declarations for RGB LED
 * @author Jeff Rowberg
 * @date 2014-08-09
 *
 * This file defines the feeback functionality for the RGB LED that may be
 * included as an optional module in a Keyglove modular hardware design. This
 * file is meant to contain only behavioral logic and abstracted functions that
 * do not require specific pin knowledge of the selected MCU. Any pins used
 * should be defined in the selected MCU board support file.
 *
 * Normally it is not necessary to edit this file.
 */

#ifndef _SUPPORT_FEEDBACK_RGB_H_
#define _SUPPORT_FEEDBACK_RGB_H_

/**
 * @brief List of possible values for RGB mode
 */
typedef enum {
    KG_RGB_MODE_OFF = 0,      ///< (0) LED solid off
    KG_RGB_MODE_SOLID,        ///< (1) LED solid on
    KG_RGB_MODE_B200_100,     ///< (2) LED blinking with 200ms period, 100ms pulse
    KG_RGB_MODE_B200_50,      ///< (3) LED blinking with 200ms period, 50ms pulse
    KG_RGB_MODE_B1000_500,    ///< (4) LED blinking with 1sec period, 500ms pulse
    KG_RGB_MODE_B1000_100,    ///< (5) LED blinking with 1sec period, 100ms pulse
    KG_RGB_MODE_B1000_100_2X, ///< (6) LED blinking with 1sec period, 100ms pulse 2x
    KG_RGB_MODE_B1000_100_3X, ///< (7) LED blinking with 1sec period, 100ms pulse 3x
    KG_RGB_MODE_B3000_1000,   ///< (8) LED blinking with 3sec period, 1sec pulse
    KG_RGB_MODE_B3000_100,    ///< (9) LED blinking with 3sec period, 100ms pulse
    KG_RGB_MODE_B3000_100_2X, ///< (10) LED blinking with 3sec period, 100ms pulse 2x
    KG_RGB_MODE_B3000_100_3X, ///< (11) LED blinking with 3sec period, 100ms pulse 3x
    KG_RGB_MODE_F200_100,     ///< (12) LED fading with 200ms period, 100ms pulse
    KG_RGB_MODE_F200_50,      ///< (13) LED fading with 200ms period, 50ms pulse
    KG_RGB_MODE_F1000_1000,   ///< (14) LED fading with 1sec period, 500ms pulse
    KG_RGB_MODE_F1000_500,    ///< (15) LED fading with 1sec period, 500ms pulse
    KG_RGB_MODE_F1000_100,    ///< (16) LED fading with 1sec period, 100ms pulse
    KG_RGB_MODE_F1000_100_2X, ///< (17) LED fading with 1sec period, 100ms pulse 2x
    KG_RGB_MODE_F1000_100_3X, ///< (18) LED fading with 1sec period, 100ms pulse 3x
    KG_RGB_MODE_F3000_3000,   ///< (19) LED fading with 3sec period, 3sec pulse
    KG_RGB_MODE_F3000_1000,   ///< (20) LED fading with 3sec period, 1sec pulse
    KG_RGB_MODE_F3000_100,    ///< (21) LED fading with 3sec period, 100ms pulse
    KG_RGB_MODE_F3000_100_2X, ///< (22) LED fading with 3sec period, 100ms pulse 2x
    KG_RGB_MODE_F3000_100_3X, ///< (23) LED fading with 3sec period, 100ms pulse 3x
    KG_RGB_MODE_MAX
} feedback_rgb_mode_t;

void feedback_set_rgb_digital(uint8_t r, uint8_t g, uint8_t b);
void feedback_set_rgb_digital_channel(uint8_t channel, uint8_t value);
void feedback_set_rgb_analog(uint8_t r, uint8_t g, uint8_t b);
void feedback_set_rgb_analog_channel(uint8_t channel, uint8_t value);
void feedback_set_rgb_mode(feedback_rgb_mode_t r, feedback_rgb_mode_t g, feedback_rgb_mode_t b);
void setup_feedback_rgb();
void update_feedback_rgb();

#endif // _SUPPORT_FEEDBACK_RGB_H_
