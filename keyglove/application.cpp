// Keyglove controller source code - Custom application behavior implementaions
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
 * @brief **USER-DEFINED:** Custom application behavior definition
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * Use this file to define any autonomous behavior the firmware should have. By
 * default, the processor will boot into what would be considered "idle" mode,
 * available for KGAPI control from an external host but otherwise not really
 * doing anything. Using custom event handlers, you can trigger actions based on
 * API events, such as boot/reset, Bluetooth ready, touch status changing, etc.
 *
 * Technically these custom event handlers could go anywhere in the codebase,
 * but it makes sense to centralize everything in one place for simplicity. You
 * will most likely not need to modify any other source files (except possibly
 * the "config.h" file which defines the modular hardware configuration), unless
 * you are intentionally changing or adding to the low-level functionality of
 * the Keyglove codebase.
 */

#include "keyglove.h"
#include "support_board.h"
#include "support_protocol.h"
#include "support_feedback.h"
#include "support_motion.h"
#include "support_touch.h"
#include "support_bluetooth.h"
#include "support_hid_keyboard.h"
#include "application.h"

uint8_t touch_data_prev[KG_BASE_COMBINATION_BYTES];     ///< Container for tracking previous touch status bits
uint8_t touch_data_xor[KG_BASE_COMBINATION_BYTES];      ///< Container for XOR'd touch status bits

/**
 * @brief Indicates that Keyglove has completed the boot process
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 *
 * This event is triggered on every power-on or reset, after all internal
 * subsystems have been initialized. For custom application behavior, it is
 * similar to a "main()" entry point in C, or the "setup()" function in an
 * Arduino sketch. Note that the "system_boot" event may seem intuitive for
 * this purpose also, but that runs before the individual subsystem setup
 * functions, which may overwrite some settings (like feedback mode changes).
 */
uint8_t my_kg_evt_system_ready() {
    // start 3-second LED pulse after boot process finishes
    kg_cmd_feedback_set_blink_mode(KG_BLINK_MODE_B3000_100);

    // start RGB (red) LED fade cycle
    //kg_cmd_feedback_set_rgb_mode(0, KG_RGB_MODE_F3000_3000, KG_RGB_MODE_F3000_3000, KG_RGB_MODE_F3000_3000);

    // start 10-second repeating timer
    //kg_cmd_system_set_timer(0, 1000, 0);

    // allow KGAPI event packet transmission
    return 0;
}

uint8_t my_kg_evt_system_timer_tick(uint8_t handle, uint32_t seconds, uint8_t subticks) {
    // read raw battery voltage
    int16_t rawBat = analogRead(0);
    uint8_t payload[2] = { rawBat & 0xFF, (rawBat >> 8) & 0xFF };
    send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, 0xFE, 0xFE, payload);

    // prevent KGAPI event packet transmission
    return 1;
}

/**
 * @brief Indicates that a motion sensor's measurement data has been updated
 * @param[in] index Relevant motion sensor
 * @param[in] flags Flags indicating which measurement data is represented
 * @param[in] data_len Length in bytes of data_data buffer
 * @param[in] data_data New measurement data
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_motion_data(uint8_t index, uint8_t flags, uint8_t data_len, uint8_t *data_data) {
    // TODO: special event handler code here
    // ...

    // prevent KGAPI event packet transmission
    return 1;
}

/**
 * @brief "bluetooth_ready" event handler
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 *
 * This event is triggered once the Bluetooth subsystem has been tested for the
 * expected communication response over UART, and all relevant module settings
 * have been requested and parsed.
 */
uint8_t my_kg_evt_bluetooth_ready() {
    // set the Bluetooth mode to autocall paired devices
    kg_cmd_bluetooth_set_mode(KG_BLUETOOTH_MODE_AUTOCALL);

    // allow KGAPI event packet transmission
    return 0;
}

/**
 * @brief Indicates that the touch sensor status has changed
 * @param[in] status_len Length in bytes of status_data buffer
 * @param[in] status_data New touch status
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 *
 * This event is triggered every time any touch status bits change. This data is
 * detected in the update_board_touch() function in the board definition file,
 * and filtered/debounced in the update_touch() function in the support_touch.h
 * file.
 */
uint8_t my_kg_evt_touch_status(uint8_t status_len, uint8_t *status_data) {
    // XOR current and previous to identify which bits have been changed
    for (uint8_t i = 0; i < KG_BASE_COMBINATION_BYTES; i++) {
        touch_data_xor[i] = touch_data_prev[i] ^ status_data[i];
    }

    // bits set in "touch_data_prev" indicate which touches are on as of last time any touches changed
    // bits set in "touch_data_xor" indicate which touches have just flipped between on and off
    // bits set in "status_data" indicate which touches are on right now

    // check to see whether this is adding or removing
    if (memcmp(status_data, touch_data_prev, KG_BASE_COMBINATION_BYTES) > 0) {
        // touch added
        if      (KGT_AY(touch_data_xor)) kg_cmd_motion_set_mode(0, true);   // enable motion sensor 0 (default MPU-6050 on back of hand)
        else if (KGT_DY(touch_data_xor)) keyboard_key_down(KEY_A);          // send key-down report for 'A' key
    } else {
        // touch removed
        if      (KGT_AY(touch_data_xor)) kg_cmd_motion_set_mode(0, false);  // disable motion sensor 0
        else if (KGT_DY(touch_data_xor)) keyboard_key_up(KEY_A);            // send key-up report for 'A' key
        else if (KGT_GY(touch_data_xor)) keyboard_key_press(KEY_A);         // send key down and key-up report for 'A' key
    }

    // store current touch status for comparing next time
    memcpy(touch_data_prev, status_data, status_len);

    // allow KGAPI event packet transmission
    return 0;
}

/**
 * @brief Custom application setup routine
 *
 * This function is called during the main firmware initialization procedure,
 * inside the setup() function in the keyglove.ino file. It is necessary here to
 * assign the event handler functions to the correct function pointers; without
 * this step, the handlers defined above will not be called when the relevant
 * events are generated.
 *
 * You can also place any other application initialization code in this function
 * to ensure that it is run anytime the system boots or is reset.
 */
void setup_application() {
    // assign any events implemented above here so they will take effect
    kg_evt_system_ready = my_kg_evt_system_ready;
    kg_evt_system_timer_tick = my_kg_evt_system_timer_tick;
    kg_evt_motion_data = my_kg_evt_motion_data;
    kg_evt_bluetooth_ready = my_kg_evt_bluetooth_ready;
    kg_evt_touch_status = my_kg_evt_touch_status;
}
