// Keyglove controller source code - Feedback implementation for single LED
// 7/4/2014 by Jeff Rowberg <jeff@rowberg.net>

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
 * @file support_feedback_blink.h
 * @brief Feedback implementation for single LED
 * @author Jeff Rowberg
 * @date 2014-07-04
 *
 * This file defines the feeback functionality for the blinking LED that is
 * built into many microcontroller boards, e.g. the Teensy++. This file is meant
 * to contain only behavioral logic and abstracted functions that do not require
 * specific pin knowledge of the selected MCU. Any pins used should be defined
 * in the selected MCU board support file.
 *
 * Normally it is not necessary to edit this file.
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

feedback_blink_mode_t feedbackBlinkMode;    ///< LED blink mode
uint8_t feedbackBlinkTick;                  ///< LED blink tick reference
uint8_t feedbackBlinkLoop;                  ///< Tick loop length for LED blink timing

/**
 * @brief Sets LED feedpack pin logic state
 * @param[in] logic Zero for low (off), non-zero for high (on)
 * @see KG_PIN_BLINK
 */
void feedback_set_blink_logic(uint8_t logic) {
    // simple wrapper for consistency
    digitalWrite(KG_PIN_BLINK, logic);
}

/**
 * @brief Sets LED feedback mode
 * @param[in] mode New LED mode to set
 */
void feedback_set_blink_mode(feedback_blink_mode_t mode) {
    feedbackBlinkMode = mode;
    feedbackBlinkTick = 0;
    feedbackBlinkLoop = 0;

    // logic: mode=0 -> off, mode=5 -> on, else no immediate change
    if (feedbackBlinkMode == KG_BLINK_MODE_OFF) feedback_set_blink_logic(0);
    else if (feedbackBlinkMode == KG_BLINK_MODE_SOLID) feedback_set_blink_logic(1);
    else if (feedbackBlinkMode == KG_BLINK_MODE_B200_100 ||
             feedbackBlinkMode == KG_BLINK_MODE_B200_50
            ) feedbackBlinkLoop = 4;
    else if (feedbackBlinkMode == KG_BLINK_MODE_B1000_500 ||
             feedbackBlinkMode == KG_BLINK_MODE_B1000_100 ||
             feedbackBlinkMode == KG_BLINK_MODE_B1000_100_2X ||
             feedbackBlinkMode == KG_BLINK_MODE_B1000_100_3X
            ) feedbackBlinkLoop = 20;
    else if (feedbackBlinkMode == KG_BLINK_MODE_B3000_1000 ||
             feedbackBlinkMode == KG_BLINK_MODE_B3000_100 ||
             feedbackBlinkMode == KG_BLINK_MODE_B3000_100_2X ||
             feedbackBlinkMode == KG_BLINK_MODE_B3000_100_3X
            ) feedbackBlinkLoop = 60;
}

/**
 * @brief Initialize LED feedback subsystem
 */
void setup_feedback_blink() {
    pinMode(KG_PIN_BLINK, OUTPUT);
    digitalWrite(KG_PIN_BLINK, LOW);
    feedbackBlinkMode = KG_BLINK_MODE_OFF;
    feedbackBlinkTick = 0;

    // SELF-TEST
    //set_blink_mode(KG_BLINK_MODE_3000_100);
}

/**
 * @brief Update status of LED feedback subystem, called at 100Hz from loop()
 */
void update_feedback_blink() {
    // each "keygloveTick" is 10ms, loops at 100 (1 second)
    // each "blinkTick" is 50ms, loops at cycle period (max 12.5 seconds = 250, near 255)
    if (feedbackBlinkLoop && (keygloveTick % 5) == 0) {
        //feedbackBlinkMod = feedbackBlinkTick % feedbackBlinkLoop;

        if (feedbackBlinkMode == KG_BLINK_MODE_B200_100) {
            feedback_set_blink_logic(square_wave(feedbackBlinkTick, 4, 50));
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B200_50) {
            feedback_set_blink_logic(square_wave(feedbackBlinkTick, 2, 25));
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B1000_500) {
            feedback_set_blink_logic(square_wave(feedbackBlinkTick, 20, 50));
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B3000_1000) {
            feedback_set_blink_logic(square_wave(feedbackBlinkTick, 60, 33));
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B1000_100 || feedbackBlinkMode == KG_BLINK_MODE_B3000_100) {
            feedback_set_blink_logic(feedbackBlinkTick < 2 ? square_wave(feedbackBlinkTick, 4, 50) : 0);
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B1000_100_2X || feedbackBlinkMode == KG_BLINK_MODE_B3000_100_2X) {
            feedback_set_blink_logic(feedbackBlinkTick < 6 ? square_wave(feedbackBlinkTick, 4, 50) : 0);
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B1000_100_3X || feedbackBlinkMode == KG_BLINK_MODE_B3000_100_3X) {
            feedback_set_blink_logic(feedbackBlinkTick < 10 ? square_wave(feedbackBlinkTick, 4, 50) : 0);

        // 41354
        /*if (feedbackBlinkMode == KG_BLINK_MODE_B200_100 && feedbackBlinkTick % 2 == 0) {
            feedback_set_blink_logic(feedbackBlinkMod >= 2 ? 0 : 1);
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B200_50) {
            feedback_set_blink_logic(feedbackBlinkMod >= 1 ? 0 : 1);
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B1000_500 && feedbackBlinkTick % 10 == 0) {
            feedback_set_blink_logic(feedbackBlinkMod >= 10 ? 0 : 1);
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B1000_100 && feedbackBlinkTick % 2 == 0) {
            feedback_set_blink_logic(feedbackBlinkMod >= 2 ? 0 : 1);
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B1000_100_2X && feedbackBlinkTick % 2 == 0) {
            feedback_set_blink_logic(((feedbackBlinkMod >= 2 && feedbackBlinkMod < 4) || (feedbackBlinkMod >= 6)) ? 0 : 1);
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B1000_100_3X && feedbackBlinkTick % 2 == 0) {
            feedback_set_blink_logic(((feedbackBlinkMod >= 2 && feedbackBlinkMod < 4) || (feedbackBlinkMod >= 6 && feedbackBlinkMod < 8) || (feedbackBlinkMod >= 10)) ? 0 : 1);
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B3000_1000 && feedbackBlinkTick % 20 == 0) {
            feedback_set_blink_logic(feedbackBlinkMod >= 20 ? 0 : 1);
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B3000_100 && feedbackBlinkTick % 2 == 0) {
            feedback_set_blink_logic(feedbackBlinkMod >= 2 ? 0 : 1);
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B3000_100_2X && feedbackBlinkTick % 2 == 0) {
            feedback_set_blink_logic(((feedbackBlinkMod >= 2 && feedbackBlinkMod < 4) || (feedbackBlinkMod >= 6)) ? 0 : 1);
        } else if (feedbackBlinkMode == KG_BLINK_MODE_B3000_100_3X && feedbackBlinkTick % 2 == 0) {
            feedback_set_blink_logic(((feedbackBlinkMod >= 2 && feedbackBlinkMod < 4) || (feedbackBlinkMod >= 6 && feedbackBlinkMod < 8) || (feedbackBlinkMod >= 10)) ? 0 : 1);*/
        }
        feedbackBlinkTick++;
        if (feedbackBlinkTick == feedbackBlinkLoop) feedbackBlinkTick = 0;
    }
}

/* ============================= */
/* KGAPI COMMAND IMPLEMENTATIONS */
/* ============================= */

/**
 * @brief Get current blink feedback mode
 * @param[out] mode Current blink feedback mode
 * @return Result code (0=success)
 */
uint16_t kg_cmd_feedback_get_blink_mode(uint8_t *mode) {
    *mode = feedbackBlinkMode;
    return 0; // success
}

/**
 * @brief Set new blink feedback mode
 * @param[in] mode New blink feedback mode to set
 * @return Result code (0=success)
 */
uint16_t kg_cmd_feedback_set_blink_mode(uint8_t mode) {
    if (mode >= KG_BLINK_MODE_MAX) {
        return KG_PROTOCOL_ERROR_PARAMETER_RANGE;
    } else {
        feedback_set_blink_mode((feedback_blink_mode_t)mode);

        // send kg_evt_feedback_blink_mode packet (if we aren't setting it from an API command)
        if (!inBinPacket) {
            uint8_t payload[1] = { mode };
            skipPacket = 0;
            if (kg_evt_feedback_blink_mode) skipPacket = kg_evt_feedback_blink_mode(mode);
            if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 1, KG_PACKET_CLASS_FEEDBACK, KG_PACKET_ID_EVT_FEEDBACK_BLINK_MODE, payload);
        }
    }
    return 0; // success
}

#endif // _SUPPORT_FEEDBACK_BLINK_H_
