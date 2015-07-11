// Keyglove controller source code - Feedback implementations for RGB LED
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
 * @file support_feedback_rgb.cpp
 * @brief Feedback implementations for RGB LED
 * @author Jeff Rowberg
 * @date 2015-07-03
 *
 * This file defines the feeback functionality for the RGB LED that may be
 * included as an optional module in a Keyglove modular hardware design. This
 * file is meant to contain only behavioral logic and abstracted functions that
 * do not require specific pin knowledge of the selected MCU. Any pins used
 * should be defined in the selected MCU board support file.
 *
 * Normally it is not necessary to edit this file.
 */

#include "keyglove.h"
#include "support_board.h"
#include "support_protocol.h"
#include "support_feedback.h"
#include "support_feedback_rgb.h"

feedback_rgb_mode_t feedbackRGBMode[3];     ///< RGB feedback red component mode
int16_t feedbackRGBTick[3];                 ///< RGB fade tick reference (signed because of abs equations)
uint16_t feedbackRGBLoop[3];                ///< Tick loop length for RGB fade timing

/**
 * @brief Sets RGB feedpack pin digital logic states (red/green/blue)
 * @param[in] r Red: zero for low (off), non-zero for high (on), 255 for no change
 * @param[in] g Green: zero for low (off), non-zero for high (on), 255 for no change
 * @param[in] b Blue: zero for low (off), non-zero for high (on), 255 for no change
 */
void feedback_set_rgb_digital(uint8_t r, uint8_t g, uint8_t b) {
    if (r != 255) digitalWrite(KG_PIN_RGB_RED, r);
    if (g != 255) digitalWrite(KG_PIN_RGB_GREEN, g);
    if (b != 255) digitalWrite(KG_PIN_RGB_BLUE, b);
}

/**
 * @brief Sets RGB feedpack pin digital logic state on one channel (red/green/blue)
 * @param[in] channel Channel number (0=RED, 1=GREEN, 2=BLUE)
 * @param[in] value Logic state (zero for low/off, non-zero for high/on)
 */
void feedback_set_rgb_digital_channel(uint8_t channel, uint8_t value) {
    if (channel == 0) digitalWrite(KG_PIN_RGB_RED, value);
    else if (channel == 1) digitalWrite(KG_PIN_RGB_GREEN, value);
    else if (channel == 2) digitalWrite(KG_PIN_RGB_BLUE, value);
}

/**
 * @brief Sets RGB feedpack pin analog PWM states (red/green/blue)
 * @param[in] r Red: zero for off, non-zero for PWM (on), 255 for no change
 * @param[in] g Green: zero for off, non-zero for PWM (on), 255 for no change
 * @param[in] b Blue: zero for off, non-zero for PWM (on), 255 for no change
 */
void feedback_set_rgb_analog(uint8_t r, uint8_t g, uint8_t b) {
    if (r != 255) analogWrite(KG_PIN_RGB_RED, r);
    if (g != 255) analogWrite(KG_PIN_RGB_GREEN, g);
    if (b != 255) analogWrite(KG_PIN_RGB_BLUE, b);
}

/**
 * @brief Sets RGB feedpack pin analog PWM state on one channel (red/green/blue)
 * @param[in] channel Channel number (0=RED, 1=GREEN, 2=BLUE)
 * @param[in] value PWM duty cycle (zero for off, non-zero for PWM/on)
 */
void feedback_set_rgb_analog_channel(uint8_t channel, uint8_t value) {
    if (channel == 0) analogWrite(KG_PIN_RGB_RED, value);
    else if (channel == 1) analogWrite(KG_PIN_RGB_GREEN, value);
    else if (channel == 2) analogWrite(KG_PIN_RGB_BLUE, value);
}

/**
 * @brief Sets RGB feedback modes (red/green/blue)
 * @param[in] r Red mode
 * @param[in] g Green mode
 * @param[in] b Blue mode
 */
void feedback_set_rgb_mode(feedback_rgb_mode_t r, feedback_rgb_mode_t g, feedback_rgb_mode_t b) {
    feedbackRGBMode[0] = r;
    feedbackRGBMode[1] = g;
    feedbackRGBMode[2] = b;
    feedbackRGBTick[0] = feedbackRGBTick[1] = feedbackRGBTick[2] = 0;
    feedbackRGBLoop[0] = feedbackRGBLoop[1] = feedbackRGBLoop[2] = 0;

    for (uint8_t i = 0; i < 3; i++) {
             if (feedbackRGBMode[i] == KG_RGB_MODE_B200_100     || feedbackRGBMode[i] == KG_RGB_MODE_F200_100 ||
                 feedbackRGBMode[i] == KG_RGB_MODE_B200_50      || feedbackRGBMode[i] == KG_RGB_MODE_F200_50
                ) feedbackRGBLoop[i] = 20;
        else if (feedbackRGBMode[i] == KG_RGB_MODE_B1000_500    || feedbackRGBMode[i] == KG_RGB_MODE_F1000_500 ||
                 feedbackRGBMode[i] == KG_RGB_MODE_B1000_100    || feedbackRGBMode[i] == KG_RGB_MODE_F1000_100 ||
                 feedbackRGBMode[i] == KG_RGB_MODE_B1000_100_2X || feedbackRGBMode[i] == KG_RGB_MODE_F1000_100_2X ||
                 feedbackRGBMode[i] == KG_RGB_MODE_B1000_100_3X || feedbackRGBMode[i] == KG_RGB_MODE_F1000_100_3X ||
                 feedbackRGBMode[i] == KG_RGB_MODE_F1000_1000
                ) feedbackRGBLoop[i] = 100;
        else if (feedbackRGBMode[i] == KG_RGB_MODE_B3000_1000   || feedbackRGBMode[i] == KG_RGB_MODE_F3000_1000 ||
                 feedbackRGBMode[i] == KG_RGB_MODE_B3000_100    || feedbackRGBMode[i] == KG_RGB_MODE_F3000_100 ||
                 feedbackRGBMode[i] == KG_RGB_MODE_B3000_100_2X || feedbackRGBMode[i] == KG_RGB_MODE_F3000_100_2X ||
                 feedbackRGBMode[i] == KG_RGB_MODE_B3000_100_3X || feedbackRGBMode[i] == KG_RGB_MODE_F3000_100_3X ||
                 feedbackRGBMode[i] == KG_RGB_MODE_F3000_3000
                ) feedbackRGBLoop[i] = 300;
    }

    // logic: r=OFF -> off, r=ON -> on, else no immediate change, update_feedback_rgb() will handling it
    feedback_set_rgb_digital(
        r == KG_RGB_MODE_OFF ? 0 : (r == KG_RGB_MODE_SOLID ? 1 : 255),
        g == KG_RGB_MODE_OFF ? 0 : (g == KG_RGB_MODE_SOLID ? 1 : 255),
        b == KG_RGB_MODE_OFF ? 0 : (b == KG_RGB_MODE_SOLID ? 1 : 255)
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
    //feedback_set_rgb_digital(1, 1, 1); // turn everything on and wait 1/20 sec
    //delay(50);
    //feedback_set_rgb_digital(0, 0, 0); // turn everything off again
}

/**
 * @brief Update status of RGB feedback subystem, called at 100Hz from loop()
 */
void update_feedback_rgb() {
    // each "keygloveTick" is 10ms, loops at 100 (1 second)
    // each "RGBTick" is also 10ms, loops at cycle period (can be greater than 1 second, actually 327.67 seconds)
    for (uint8_t i = 0; i < 3; i++) {
        if (feedbackRGBMode[i] == KG_RGB_MODE_B200_100) {
            feedback_set_rgb_digital_channel(i, square_wave(feedbackRGBTick[i], 20, 50));
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_B200_50) {
            feedback_set_rgb_digital_channel(i, square_wave(feedbackRGBTick[i], 10, 25));
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_B1000_500) {
            feedback_set_rgb_digital_channel(i, square_wave(feedbackRGBTick[i], 100, 50));
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_B3000_1000) {
            feedback_set_rgb_digital_channel(i, square_wave(feedbackRGBTick[i], 300, 33));
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_B1000_100 || feedbackRGBMode[i] == KG_RGB_MODE_B3000_100) {
            feedback_set_rgb_digital_channel(i, feedbackRGBTick[i] < 10 ? square_wave(feedbackRGBTick[i], 20, 50) : 0);
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_B1000_100_2X || feedbackRGBMode[i] == KG_RGB_MODE_B3000_100_2X) {
            feedback_set_rgb_digital_channel(i, feedbackRGBTick[i] < 30 ? square_wave(feedbackRGBTick[i], 20, 50) : 0);
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_B1000_100_3X || feedbackRGBMode[i] == KG_RGB_MODE_B3000_100_3X) {
            feedback_set_rgb_digital_channel(i, feedbackRGBTick[i] < 50 ? square_wave(feedbackRGBTick[i], 20, 50) : 0);

        } else if (feedbackRGBMode[i] == KG_RGB_MODE_F200_50) {
            feedback_set_rgb_analog_channel(i, feedbackRGBTick[i] < 5 ? triangle_wave(feedbackRGBTick[i], 10, 400) : 0);
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_F1000_500) {
            feedback_set_rgb_analog_channel(i, feedbackRGBTick[i] < 50 ? triangle_wave(feedbackRGBTick[i], 100, 400) : 0);
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_F1000_1000) {
            feedback_set_rgb_analog_channel(i, triangle_wave(feedbackRGBTick[i], 200, 400));
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_F3000_1000) {
            feedback_set_rgb_analog_channel(i, feedbackRGBTick[i] < 100 ? triangle_wave(feedbackRGBTick[i], 200, 400) : 0);
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_F3000_3000) {
            feedback_set_rgb_analog_channel(i, triangle_wave(feedbackRGBTick[i], 600, 400));
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_F200_100 || feedbackRGBMode[i] == KG_RGB_MODE_F1000_100 || feedbackRGBMode[i] == KG_RGB_MODE_F3000_100) {
            feedback_set_rgb_analog_channel(i, feedbackRGBTick[i] < 10 ? triangle_wave(feedbackRGBTick[i], 20, 400) : 0);
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_F1000_100_2X || feedbackRGBMode[i] == KG_RGB_MODE_F3000_100_2X) {
            feedback_set_rgb_analog_channel(i, feedbackRGBTick[i] < 30 ? triangle_wave(feedbackRGBTick[i], 20, 400) : 0);
        } else if (feedbackRGBMode[i] == KG_RGB_MODE_F1000_100_3X || feedbackRGBMode[i] == KG_RGB_MODE_F3000_100_3X) {
            feedback_set_rgb_analog_channel(i, feedbackRGBTick[i] < 50 ? triangle_wave(feedbackRGBTick[i], 20, 400) : 0);
        }

        feedbackRGBTick[i]++;
        if (feedbackRGBTick[i] == (int16_t)feedbackRGBLoop[i]) feedbackRGBTick[i] = 0;
    }
}
