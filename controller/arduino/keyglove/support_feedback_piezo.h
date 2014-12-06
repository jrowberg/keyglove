// Keyglove controller source code - Feedback declarations for piezo buzzer
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
 * @file support_feedback_piezo.h
 * @brief Feedback declarations for piezo buzzer
 * @author Jeff Rowberg
 * @date 2014-11-07
 */

#ifndef _SUPPORT_FEEDBACK_PIEZO_H_
#define _SUPPORT_FEEDBACK_PIEZO_H_

#define KG_PIEZO_DEFAULT_FREQ 1760      ///< Default frequency (high A, 440Hz * 4)

/**
 * @brief List of possible values for piezo mode
 */
typedef enum {
    KG_PIEZO_MODE_OFF = 0,      ///< (0) Buzzer solid off
    KG_PIEZO_MODE_SOLID,        ///< (1) Buzzer solid on
    KG_PIEZO_MODE_LONGBEEP,     ///< (2) Buzzer alternating at 1sec period, 500ms pulse
    KG_PIEZO_MODE_LONGPULSE,    ///< (3) Buzzer alternating at 1sec period, 250ms pulse
    KG_PIEZO_MODE_SHORTBEEP,    ///< (4) Buzzer alternating at 200ms period, 100ms pulse
    KG_PIEZO_MODE_SHORTPULSE,   ///< (5) Buzzer alternating at 200ms period, 50ms pulse
    KG_PIEZO_MODE_TINYBEEP,     ///< (6) Buzzer alternating at 100ms period, 50ms pulse
    KG_PIEZO_MODE_TINYPULSE,    ///< (7) Buzzer alternating at 100ms period, 20ms pulse
    KG_PIEZO_MODE_MAX
} feedback_piezo_mode_t;

void feedback_set_piezo_tone(uint16_t frequency);
void feedback_set_piezo_mode(feedback_piezo_mode_t mode, uint8_t duration, uint16_t frequency);
void setup_feedback_piezo();
void update_feedback_piezo();

#endif // _SUPPORT_FEEDBACK_PIEZO_H_
