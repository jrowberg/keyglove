// Keyglove controller source code - General touch support implementations
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
 * @file support_touch.cpp
 * @brief General touch support implementations
 * @author Jeff Rowberg
 * @date 2015-07-03
 *
 * This file provides the structural framework for touch implementations in the
 * overall Keyglove architecture.
 *
 * Normally it is not necessary to edit this file.
 */

#include "keyglove.h"
#include "support_board.h"
#include "support_protocol.h"
#include "support_touch.h"

uint8_t touchMode;          ///< Touch mode
//uint32_t touchBench;        ///< Touch benchmark reference end
//uint32_t touchBench0;       ///< Touch benchmark reference start
uint8_t touchTick;          ///< Touch tick reference
uint8_t touchOn;            ///< Indicates whether any touches are active

uint16_t opt_touch_detect_threshold = 10;   ///< OPTION: Milliseconds required for a touch to register as legitimate

uint32_t touchTime;                         ///< Touch detection reference timestamp

uint8_t touches_now[KG_BASE_COMBINATION_BYTES];     ///< Immediate status of all touch combinations
uint8_t touches_verify[KG_BASE_COMBINATION_BYTES];  ///< Previous status of all touch combinations (debouncing in progress)
uint8_t touches_active[KG_BASE_COMBINATION_BYTES];  ///< Registered (debounced) status of all touch combinations

uint8_t touchModeStack[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };    ///< Stackable touch mode tracking info
uint8_t touchModeStackPos = 0;                                  ///< Current position in mode stack

/**
 * @brief Initialize touch detection system
 */
void setup_touch() {
    touchMode = 0; // set to base mode, no alternates
    touch_set_mode(0); // default touchset mode is always 0
}

/**
 * @brief Update status of touch system, called at 100Hz (or constantly while touches active) from loop()
 */
void update_touch() {
    //touchBench0 = micros();

    uint8_t i;
    memset(touches_now, 0x00, KG_BASE_COMBINATION_BYTES);

    // loop through every registered 1-to-1 sensor combination and record levels
    // (moved to hardware-specific code for efficiency, improved iteration time from 2ms to 40us SERIOUSLY OMG)
    update_board_touch(touches_now);

    // check to see if we need to reset detection threshold
    if (memcmp(touches_now, touches_verify, KG_BASE_COMBINATION_BYTES) != 0) {
        // current sensors different from last detection, so reset threshold
        touchTime = millis();
    } else if (memcmp(touches_verify, touches_active, KG_BASE_COMBINATION_BYTES) != 0 && millis() - touchTime >= opt_touch_detect_threshold) {
        // detection is over threshold and current readings are different from previous readings

        // set official sensor readings to current readings
        memcpy(touches_active, touches_verify, KG_BASE_COMBINATION_BYTES);

        // check overall touch state (on or off)
        touchOn = 0;
        for (i = 0; i < KG_BASE_COMBINATION_BYTES && !touchOn; i++) touchOn |= touches_active[i];

        // build event (uint8_t index, uint8_t[] touches)
        uint8_t payload[KG_BASE_COMBINATION_BYTES + 1];
        payload[0] = KG_BASE_COMBINATION_BYTES;
        memcpy(payload + 1, touches_active, KG_BASE_COMBINATION_BYTES);

        // send event
        skipPacket = 0;
        if (kg_evt_touch_status) skipPacket = kg_evt_touch_status(payload[0], payload + 1);
        if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, sizeof(payload), KG_PACKET_CLASS_TOUCH, KG_PACKET_ID_EVT_TOUCH_STATUS, payload);
    }

    // set "verify" readings to match "now" readings (debouncing)
    memcpy(touches_verify, touches_now, KG_BASE_COMBINATION_BYTES);

    /*touchBench += (micros() - touchBench0);
    touchTick++;
    if (touchTick % 100 == 0) {
        // starting point: 1596 us per cycle
        touchTick = 0;
        Serial.println(millis() - touchBench);
        Serial.println(touchBench / 100);
        Serial.println("100 touch");
        touchBench = 0;
    }*/
}

// declare these here so touch_set_mode() etc. have some context
//void activate_mode(uint8_t mode) { }
//void deactivate_mode(uint8_t mode) { }

/**
 * @brief Check whether a certain mode is active in a certain position
 * @param mode Mode to check
 * @param pos Stack position to check
 */
uint8_t touch_check_mode(uint8_t mode, uint8_t pos) {
    if (touchModeStackPos < pos) {
        return false;
    }
    return touchModeStack[touchModeStackPos - pos] == mode;
}

/**
 * @brief Set new active touch mode (replaces everything in mode stack)
 * @param[in] mode New mode to set
 */
void touch_set_mode(uint8_t mode) {
    //for (; touchModeStackPos > 0; touchModeStackPos--) deactivate_mode(touchModeStack[touchModeStackPos - 1]);
    touchModeStackPos = 1;
    touchModeStack[0] = mode;
    //activate_mode(mode);
}

/**
 * @brief Apply new touch mode to top of mode stack
 * @param[in] mode New mode to push
 */
void touch_push_mode(uint8_t mode) {
    if (touchModeStackPos < 10) {
        touchModeStack[touchModeStackPos] = mode;
        touchModeStackPos++;
        //activate_mode(mode);
    }
}

/**
 * @brief Remove last touch mode from top of mode stack
 */
void touch_pop_mode() {
    if (touchModeStackPos > 0) touchModeStackPos--; //deactivate_mode(touchModeStack[--touchModeStackPos]);
    if (touchModeStackPos == 0) touchModeStack[touchModeStackPos++] = 0;
    //activate_mode(touchModeStack[touchModeStackPos - 1]);
}

/**
 * @brief Toggle touch mode in mode stack (push if missing, remove/pop if present)
 * @param[in] mode Mode to toggle
 */
void touch_toggle_mode(uint8_t mode) {
    // find the mode and disable it if it's in the stack
    uint8_t i;
    for (i = 0; i < touchModeStackPos && touchModeStack[i] != mode; i++);
    if (i < touchModeStackPos) {
        // enabled, so turn it off
        //deactivate_mode(mode);
        for (i++; i < touchModeStackPos; i++) touchModeStack[i - 1] = touchModeStack[i];
        touchModeStackPos--;
    } else {
        // not enabled, so turn it on
        touch_push_mode(mode);
    }
}
