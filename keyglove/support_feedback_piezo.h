// Keyglove controller source code - Feedback implementation for piezo buzzer
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
 * @file support_feedback_piezo.h
 * @brief Feedback implementation for piezo buzzer
 * @author Jeff Rowberg
 * @date 2014-07-04
 *
 * This file defines the feeback functionality for the piezo buzzer that may be
 * included as an optional module in a Keyglove modular hardware design. This
 * file is meant to contain only behavioral logic and abstracted functions that
 * do not require specific pin knowledge of the selected MCU. Any pins used
 * should be defined in the selected MCU board support file.
 *
 * Normally it is not necessary to edit this file.
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

feedback_piezo_mode_t feedbackPiezoMode;    ///< Piezo sound mode
uint16_t feedbackPiezoTick;                 ///< Piezo tick reference
uint16_t feedbackPiezoDuration;             ///< Piezo pattern duration
uint16_t feedbackPiezoFrequency;            ///< Piezo frequency to use for tone generation

/**
 * @brief Enables or disables the piezo buzzer using a certain frequency
 * @param[in] frequency Frequency (Hz) of tone to generate, or zero to disable
 * @see KG_PIN_PIEZO
 */
void feedback_set_piezo_tone(uint16_t frequency) {
    if (frequency == 0) {
        noTone(KG_PIN_PIEZO);
    } else {
        tone(KG_PIN_PIEZO, frequency);
    }
}

/**
 * @brief Sets the operational mode of the piezo buzzer
 * @param[in] mode Piezo mode to use
 * @param[in] duration Duration in 10ms units to run pattern before ending (0 to run forever)
 * @param[in] frequency Frequency (Hz) of tone to generate
 */
void feedback_set_piezo_mode(feedback_piezo_mode_t mode, uint8_t duration, uint16_t frequency) {
    feedbackPiezoMode = mode;
    feedbackPiezoFrequency = frequency;
    if (mode == KG_PIEZO_MODE_OFF) feedback_set_piezo_tone(0);
    else if (mode == KG_PIEZO_MODE_SOLID) feedback_set_piezo_tone(frequency);
    feedbackPiezoTick = 0xFFFF;
    feedbackPiezoDuration = duration;
}

/**
 * @brief Initialize piezo feedback subsystem
 */
void setup_feedback_piezo() {
    pinMode(KG_PIN_PIEZO, OUTPUT);
    digitalWrite(KG_PIN_PIEZO, LOW);
    noTone(KG_PIN_PIEZO);

    // SELF-TEST
    //feedback_set_piezo_mode(KG_PIEZO_TINYPULSE, 20);
}

/**
 * @brief Update status of piezo feedback subystem, called at 100Hz from loop()
 */
void update_feedback_piezo() {
     if (feedbackPiezoMode > 0) {
         if      (feedbackPiezoMode == KG_PIEZO_MODE_LONGBEEP   && feedbackPiezoTick % 50 == 0) { feedback_set_piezo_tone(feedbackPiezoTick % 100 >= 50 ? 0 : feedbackPiezoFrequency); }
         else if (feedbackPiezoMode == KG_PIEZO_MODE_LONGPULSE  && feedbackPiezoTick % 25 == 0) { feedback_set_piezo_tone(feedbackPiezoTick % 100 >= 25 ? 0 : feedbackPiezoFrequency); }
         else if (feedbackPiezoMode == KG_PIEZO_MODE_SHORTBEEP  && feedbackPiezoTick % 10 == 0) { feedback_set_piezo_tone(feedbackPiezoTick %  20 >= 10 ? 0 : feedbackPiezoFrequency); }
         else if (feedbackPiezoMode == KG_PIEZO_MODE_SHORTPULSE && feedbackPiezoTick %  5 == 0) { feedback_set_piezo_tone(feedbackPiezoTick %  20 >=  5 ? 0 : feedbackPiezoFrequency); }
         else if (feedbackPiezoMode == KG_PIEZO_MODE_TINYBEEP   && feedbackPiezoTick %  5 == 0) { feedback_set_piezo_tone(feedbackPiezoTick %  10 >=  5 ? 0 : feedbackPiezoFrequency); }
         else if (feedbackPiezoMode == KG_PIEZO_MODE_TINYPULSE  && feedbackPiezoTick %  2 == 0) { feedback_set_piezo_tone(feedbackPiezoTick %  10 >=  2 ? 0 : feedbackPiezoFrequency); }
         if (++feedbackPiezoTick >= feedbackPiezoDuration && feedbackPiezoDuration > 0) feedback_set_piezo_mode(KG_PIEZO_MODE_OFF, 0, 0);
     }
}

/* ============================= */
/* KGAPI COMMAND IMPLEMENTATIONS */
/* ============================= */

/**
 * @brief Get current feedback mode for a piezo buzzer
 * @param[in] index Index of piezo device for which to get the current mode
 * @param[out] mode Current feedback mode for specified piezo device
 * @param[out] duration Duration to maintain tone
 * @param[out] frequency Frequency of tone to generate
 * @return Result code (0=success)
 */
uint16_t kg_cmd_feedback_get_piezo_mode(uint8_t index, uint8_t *mode, uint8_t *duration, uint16_t *frequency) {
    // "index" is currently ignored, as there is only one piezo device in the design
    *mode = feedbackPiezoMode;
    *duration = feedbackPiezoDuration;
    *frequency = feedbackPiezoFrequency;
    return 0; // success
}

/**
 * @brief Set a new piezo feedback mode for a piezo buzzer
 * @param[in] index Index of piezo device for which to set a new mode
 * @param[in] mode New feedback mode to set for specified piezo device
 * @param[in] duration Duration to maintain tone
 * @param[in] frequency Frequency of tone to generate
 * @return Result code (0=success)
 */
uint16_t kg_cmd_feedback_set_piezo_mode(uint8_t index, uint8_t mode, uint8_t duration, uint16_t frequency) {
    if (mode >= KG_PIEZO_MODE_MAX) {
        return KG_PROTOCOL_ERROR_PARAMETER_RANGE;
    } else {
        // "index" is currently ignored, as there is only one piezo device in the design
        feedback_set_piezo_mode((feedback_piezo_mode_t)mode, duration, frequency);

        // send kg_evt_feedback_piezo_mode packet (if we aren't setting it from an API command)
        if (!inBinPacket) {
            uint8_t payload[5] = { index, mode, duration, frequency & 0xFF, frequency >> 8 };
            skipPacket = 0;
            if (kg_evt_feedback_piezo_mode != 0) { skipPacket = kg_evt_feedback_piezo_mode(index, mode, duration, frequency); }
            if (skipPacket == 0) { send_keyglove_packet(KG_PACKET_TYPE_EVENT, 5, KG_PACKET_CLASS_FEEDBACK, KG_PACKET_ID_EVT_FEEDBACK_PIEZO_MODE, payload); }
        }
    }
    return 0; // success
}

#endif // _SUPPORT_FEEDBACK_PIEZO_H_
