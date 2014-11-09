// Keyglove controller source code - Custom application behavior stub/reference implementations
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
 * @file application_stubs.cpp
 * @brief **USER-DEFINED:** Custom application behavior stub/reference implementations
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * This file contains empty "stub" implementations of all existing API events.
 * It is also not included in the compile process. It is intended only that you
 * copy and paste any required functions into the actual "application.h" file,
 * and then add your custom event handler code to the functions there.
 *
 * This file is autogenerated.
 */

#if false

//////////////////////////////// PROTOCOL ////////////////////////////////

/**
 * @brief Occurs when a problem exists with a command you have sent
 * @param[in] code Error code describing what went wrong with the protocol communication
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_protocol_error(uint16_t code) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}


//////////////////////////////// SYSTEM ////////////////////////////////

/**
 * @brief Indicates that Keyglove has started the boot process
 * @param[in] major Firmware major version number
 * @param[in] minor Firmware minor version number
 * @param[in] patch Firmware patch version number
 * @param[in] timestamp Firmware build timestamp
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_system_boot(uint8_t major, uint8_t minor, uint8_t patch, uint32_t timestamp) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that Keyglove has completed the boot process
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_system_ready() {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that Keyglove has encountered an error
 * @param[in] code Error code describing what went wrong with the system
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_system_error(uint16_t code) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that a previously scheduled software timer has elapsed
 * @param[in] handle Timer handle which triggered this event
 * @param[in] seconds Seconds elapsed since boot
 * @param[in] subticks 10ms subticks above whole second
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_system_timer_tick(uint8_t handle, uint32_t seconds, uint8_t subticks) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that battery status has changed
 * @param[in] status Battery status
 * @param[in] level Charge level (0-100)
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_system_battery_status(uint8_t status, uint8_t level) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}


//////////////////////////////// TOUCH ////////////////////////////////

/**
 * @brief Indicates that the touch mode has changed
 * @param[in] mode New touch mode
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_touch_mode(uint8_t mode) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that the touch sensor status has changed
 * @param[in] status_len Length in bytes of status_data buffer
 * @param[in] status_data New touch status
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_touch_status(uint8_t status_len, uint8_t *status_data) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}


//////////////////////////////// FEEDBACK ////////////////////////////////

/**
 * @brief Indicates that the blink feedback mode has changed
 * @param[in] mode New blink feedback mode
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_feedback_blink_mode(uint8_t mode) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that a piezo buzzer feedback mode has changed
 * @param[in] index Piezo feedback device index
 * @param[in] mode New piezo feedback mode for indicated piezo device
 * @param[in] duration Duration to maintain tone
 * @param[in] frequency Frequency of tone to generate
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_feedback_piezo_mode(uint8_t index, uint8_t mode, uint8_t duration, uint16_t frequency) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that a vibration feedback mode has changed
 * @param[in] index Vibration feedback device index
 * @param[in] mode New vibration feedback mode for indicated vibration device
 * @param[in] duration Duration to maintain vibration
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_feedback_vibrate_mode(uint8_t index, uint8_t mode, uint8_t duration) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that an RGB LED feedback mode has changed
 * @param[in] index RGB feedback device index
 * @param[in] mode_red New feedback mode for indicated RGB device red LED
 * @param[in] mode_green New feedback mode for indicated RGB device green LED
 * @param[in] mode_blue New feedback mode for indicated RGB device blue LED
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_feedback_rgb_mode(uint8_t index, uint8_t mode_red, uint8_t mode_green, uint8_t mode_blue) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}


//////////////////////////////// MOTION ////////////////////////////////

/**
 * @brief Indicates that a motion sensor's mode has changed
 * @param[in] index Affected motion sensor
 * @param[in] mode New motion sensor mode
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_motion_mode(uint8_t index, uint8_t mode) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
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

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Motion state change detected, such as 'still' or 'moving'
 * @param[in] index Relevant motion sensor
 * @param[in] state Type of motion state detected
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_motion_state(uint8_t index, uint8_t state) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}


//////////////////////////////// BLUETOOTH ////////////////////////////////

/**
 * @brief Indicates that the Bluetooth mode has been changed
 * @param[in] mode New Bluetooth connectivity mode
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_bluetooth_mode(uint8_t mode) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that the Bluetooth subsystem is ready for use
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_bluetooth_ready() {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that a new device has been paired
 * @param[in] address Six-byte Bluetooth MAC address of remote device
 * @param[in] cod Three-byte Bluetooth Class-of-Device value
 * @param[in] rssi RSSI value from discovered device
 * @param[in] status Status within inquiry process
 * @param[in] index Index of device in pairing list (0xFF if not paired)
 * @param[in] name_len Length in bytes of name_data buffer
 * @param[in] name_data Friendly name of remote device (if available)
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_bluetooth_inquiry_response(uint8_t *address, uint8_t *cod, int8_t rssi, uint8_t status, uint8_t index, uint8_t name_len, uint8_t *name_data) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that an ongoing Bluetooth discovery process has finished
 * @param[in] count Total number of devices found during discovery
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_bluetooth_inquiry_complete(uint8_t count) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Provides a single pairing entry detailed status record
 * @param[in] index Index of device in pairing list
 * @param[in] address Six-byte Bluetooth MAC address of remote device
 * @param[in] priority Auto-connection priority
 * @param[in] profiles_supported Bitmask of supported profiles
 * @param[in] profiles_active Bitmask of active profiles
 * @param[in] handle_list_len Length in bytes of handle_list_data buffer
 * @param[in] handle_list_data Handles for all active Bluetooth profile connections
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_bluetooth_pairing_status(uint8_t index, uint8_t *address, uint8_t priority, uint8_t profiles_supported, uint8_t profiles_active, uint8_t handle_list_len, uint8_t *handle_list_data) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that a pending pair attempt has failed
 * @param[in] address Six-byte Bluetooth MAC address which failed pairing attempt
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_bluetooth_pairing_failed(uint8_t *address) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that all pairings have been removed
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_bluetooth_pairings_cleared() {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that a paired device has connected
 * @param[in] handle Connection handle
 * @param[in] address Six-byte Bluetooth MAC address of remote device
 * @param[in] index Index of device in pairing list
 * @param[in] profile Bluetooth profile used for this connection
 * @param[in] status Status
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_bluetooth_connection_status(uint8_t handle, uint8_t *address, uint8_t index, uint8_t profile, uint8_t status) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

/**
 * @brief Indicates that an active connection has been closed
 * @param[in] handle Connection handle
 * @param[in] reason Reason for connection closure
 * @return KGAPI event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t my_kg_evt_bluetooth_connection_closed(uint8_t handle, uint16_t reason) {
    // TODO: special event handler code here
    // ...

    return 0; // 0=send event API packet, otherwise skip sending
}

#endif
