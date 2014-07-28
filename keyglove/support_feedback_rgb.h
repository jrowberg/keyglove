// Keyglove controller source code - Feedback implementation for RGB LED
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
 * @file support_feedback_rgb.h
 * @brief Feedback implementation for RGB LED
 * @author Jeff Rowberg
 * @date 2014-07-04
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
    KG_RGB_MODE_OFF = 0,    ///< (0) LED solid off
    KG_RGB_MODE_SOLID,      ///< (1) LED solid on
    KG_RGB_MODE_LONGBLINK,  ///< (2) LED blinking with 1sec period, 500ms pulse
    KG_RGB_MODE_LONGPULSE,  ///< (3) LED blinking with 1sec period, 250ms pulse
    KG_RGB_MODE_SHORTBLINK, ///< (4) LED blinking with 200ms period, 100ms pulse
    KG_RGB_MODE_SHORTPULSE, ///< (5) LED blinking with 200ms period, 50ms pulse
    KG_RGB_MODE_MAX
} feedback_rgb_mode_t;

feedback_rgb_mode_t feedbackRGBModeRed;     ///< RGB feedback red component mode
feedback_rgb_mode_t feedbackRGBModeGreen;   ///< RGB feedback green component mode
feedback_rgb_mode_t feedbackRGBModeBlue;    ///< RGB feedback blue component mode

/**
 * @brief Sets RGB feedpack pin logic states (red/green/blue)
 * @param[in] r Red: zero for low (off), non-zero for high (on), 255 for no change
 * @param[in] g Green: zero for low (off), non-zero for high (on), 255 for no change
 * @param[in] b Blue: zero for low (off), non-zero for high (on), 255 for no change
 * @see KG_PIN_RGB_RED
 * @see KG_PIN_RGB_GREEN
 * @see KG_PIN_RGB_BLUE
 */
void feedback_set_rgb_logic(uint8_t r, uint8_t g, uint8_t b) {
    if (r != 255) digitalWrite(KG_PIN_RGB_RED, r);
    if (g != 255) digitalWrite(KG_PIN_RGB_GREEN, g);
    if (b != 255) digitalWrite(KG_PIN_RGB_BLUE, b);
}

/**
 * @brief Sets RGB feedback modes (red/green/blue)
 * @param[in] r Red mode
 * @param[in] g Green mode
 * @param[in] b Blue mode
 */
void feedback_set_rgb_mode(feedback_rgb_mode_t r, feedback_rgb_mode_t g, feedback_rgb_mode_t b) {
    feedbackRGBModeRed = r;
    feedbackRGBModeGreen = g;
    feedbackRGBModeBlue = b;
    // logic: r=0 -> off, r=5 -> on, else no immediate change
    feedback_set_rgb_logic(
        r == 0 ? 0 : (r == 5 ? 1 : 255),
        g == 0 ? 0 : (g == 5 ? 1 : 255),
        b == 0 ? 0 : (b == 5 ? 1 : 255)
    );
}

/**
 * @brief Initialize RGB feedback subsystem
 */
void setup_feedback_rgb() {
    pinMode(KG_PIN_RGB_RED, OUTPUT);
    pinMode(KG_PIN_RGB_GREEN, OUTPUT);
    pinMode(KG_PIN_RGB_BLUE, OUTPUT);
    digitalWrite(KG_PIN_RGB_RED, LOW);
    digitalWrite(KG_PIN_RGB_GREEN, LOW);
    digitalWrite(KG_PIN_RGB_BLUE, LOW);

    // SELF-TEST
    //feedback_set_rgb_logic(1, 1, 1); // turn everything on and wait 1/20 sec
    //delay(50);
    //feedback_set_rgb_logic(0, 0, 0); // turn everything off again
}

/**
 * @brief Update status of RGB feedback subystem, called at 100Hz from loop()
 */
void update_feedback_rgb() {
    if      (feedbackRGBModeRed   == KG_RGB_MODE_LONGBLINK  && keygloveTick % 50 == 0) feedback_set_rgb_logic(keygloveTick % 100 >= 50 ? 0 : 1, 255, 255);
    else if (feedbackRGBModeRed   == KG_RGB_MODE_LONGPULSE  && keygloveTick % 25 == 0) feedback_set_rgb_logic(keygloveTick % 100 >= 25 ? 0 : 1, 255, 255);
    else if (feedbackRGBModeRed   == KG_RGB_MODE_SHORTBLINK && keygloveTick % 10 == 0) feedback_set_rgb_logic(keygloveTick %  20 >= 10 ? 0 : 1, 255, 255);
    else if (feedbackRGBModeRed   == KG_RGB_MODE_SHORTPULSE && keygloveTick %  5 == 0) feedback_set_rgb_logic(keygloveTick %  20 >=  5 ? 0 : 1, 255, 255);
    if      (feedbackRGBModeGreen == KG_RGB_MODE_LONGBLINK  && keygloveTick % 50 == 0) feedback_set_rgb_logic(255, keygloveTick % 100 >= 50 ? 0 : 1, 255);
    else if (feedbackRGBModeGreen == KG_RGB_MODE_LONGPULSE  && keygloveTick % 25 == 0) feedback_set_rgb_logic(255, keygloveTick % 100 >= 25 ? 0 : 1, 255);
    else if (feedbackRGBModeGreen == KG_RGB_MODE_SHORTBLINK && keygloveTick % 10 == 0) feedback_set_rgb_logic(255, keygloveTick %  20 >= 10 ? 0 : 1, 255);
    else if (feedbackRGBModeGreen == KG_RGB_MODE_SHORTPULSE && keygloveTick %  5 == 0) feedback_set_rgb_logic(255, keygloveTick %  20 >=  5 ? 0 : 1, 255);
    if      (feedbackRGBModeBlue  == KG_RGB_MODE_LONGBLINK  && keygloveTick % 50 == 0) feedback_set_rgb_logic(255, 255, keygloveTick % 100 >= 50 ? 0 : 1);
    else if (feedbackRGBModeBlue  == KG_RGB_MODE_LONGPULSE  && keygloveTick % 25 == 0) feedback_set_rgb_logic(255, 255, keygloveTick % 100 >= 25 ? 0 : 1);
    else if (feedbackRGBModeBlue  == KG_RGB_MODE_SHORTBLINK && keygloveTick % 10 == 0) feedback_set_rgb_logic(255, 255, keygloveTick %  20 >= 10 ? 0 : 1);
    else if (feedbackRGBModeBlue  == KG_RGB_MODE_SHORTPULSE && keygloveTick %  5 == 0) feedback_set_rgb_logic(255, 255, keygloveTick %  20 >=  5 ? 0 : 1);
}

/* ============================= */
/* KGAPI COMMAND IMPLEMENTATIONS */
/* ============================= */

/**
 * @brief Get current feedback mode for an RGB LED
 * @param[in] index Index of RGB device for which to get the current mode
 * @param[out] mode_red Current feedback mode for specified RGB device red LED
 * @param[out] mode_green Current feedback mode for specified RGB device green LED
 * @param[out] mode_blue Current feedback mode for specified RGB device blue LED
 * @return Result code (0=success)
 */
uint16_t kg_cmd_feedback_get_rgb_mode(uint8_t index, uint8_t *mode_red, uint8_t *mode_green, uint8_t *mode_blue) {
    // "index" is currently ignored, as there is only one RGB device in the design
    *mode_red = feedbackRGBModeRed;
    *mode_green = feedbackRGBModeGreen;
    *mode_blue = feedbackRGBModeBlue;
    return 0; // success
}

/**
 * @brief Set a new RGB LED feedback mode
 * @param[in] index Index of RGB device for which to set a new mode
 * @param[in] mode_red New feedback mode to set for specified RGB device red LED
 * @param[in] mode_green New feedback mode to set for specified RGB device green LED
 * @param[in] mode_blue New feedback mode to set for specified RGB device blue LED
 * @return Result code (0=success)
 */
uint16_t kg_cmd_feedback_set_rgb_mode(uint8_t index, uint8_t mode_red, uint8_t mode_green, uint8_t mode_blue) {
    if (mode_red >= KG_RGB_MODE_MAX || mode_green >= KG_RGB_MODE_MAX || mode_blue >= KG_RGB_MODE_MAX) {
        return KG_PROTOCOL_ERROR_PARAMETER_RANGE;
    } else {
        // "index" is currently ignored, as there is only one RGB device in the design
        feedback_set_rgb_mode((feedback_rgb_mode_t)mode_red, (feedback_rgb_mode_t)mode_green, (feedback_rgb_mode_t)mode_blue);

        // send kg_evt_feedback_rgb_mode packet (if we aren't setting it from an API command)
        if (!inBinPacket) {
            uint8_t payload[4] = { index, mode_red, mode_green, mode_blue };
            skipPacket = 0;
            if (kg_evt_feedback_rgb_mode) skipPacket = kg_evt_feedback_rgb_mode(index, mode_red, mode_green, mode_blue);
            if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 4, KG_PACKET_CLASS_FEEDBACK, KG_PACKET_ID_EVT_FEEDBACK_RGB_MODE, payload);
        }
    }
    return 0; // success
}

#endif // _SUPPORT_FEEDBACK_RGB_H_
