// Keyglove controller source code - Protocol declarations and generic support for motion
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
 * @file support_motion.h
 * @brief Protocol declarations and generic support for motion
 * @author Jeff Rowberg
 * @date 2014-07-04
 *
 * This file provides the structural framework for motion implementations in the
 * overall Keyglove architecture.
 *
 * Normally it is not necessary to edit this file.
 */

#ifndef _SUPPORT_MOTION_H_
#define _SUPPORT_MOTION_H_

#if (KG_MOTION & KG_MOTION_MPU6050_HAND)
	#define KG_MOTION_SENSOR_COUNT  1
#else
	#define KG_MOTION_SENSOR_COUNT  0
#endif

/**
 * @brief List of possible values for motion sensor mode
 */
typedef enum {
    KG_MOTION_MODE_OFF = 0,		///< (0) Motion sensor disabled
    KG_MOTION_MODE_ON,          ///< (1) Motion sensor enabled
    KG_MOTION_MODE_MAX
} motion_mode_t;

motion_mode_t motionMode[KG_MOTION_SENSOR_COUNT];	///< Motion sensor modes

void motion_set_mpu6050_hand_mode(uint8_t mode);

/* =========================== */
/* KGAPI CONSTANT DECLARATIONS */
/* =========================== */

#define KG_PACKET_ID_CMD_MOTION_GET_MODE                    0x01
#define KG_PACKET_ID_CMD_MOTION_SET_MODE                    0x02
// -- command/event split --
#define KG_PACKET_ID_EVT_MOTION_MODE                        0x01
#define KG_PACKET_ID_EVT_MOTION_DATA                        0x02

/* ================================ */
/* KGAPI COMMAND/EVENT DECLARATIONS */
/* ================================ */

/* 0x01 */ uint16_t kg_cmd_motion_get_mode(uint8_t index, uint8_t *mode);
/* 0x02 */ uint16_t kg_cmd_motion_set_mode(uint8_t index, uint8_t mode);
// -- command/event split --
/* 0x01 */ uint8_t (*kg_evt_motion_mode)(uint8_t index, uint8_t mode);
/* 0x02 */ uint8_t (*kg_evt_motion_data)(uint8_t index, uint8_t flags, uint8_t data_len, uint8_t *data_data);

/* ============================= */
/* KGAPI COMMAND IMPLEMENTATIONS */
/* ============================= */

/**
 * @brief Get current mode for specified motion sensor
 * @param[in] index Index of motion sensor for which to get the current mode
 * @param[out] mode Current motion sensor mode
 * @return Result code (0=success)
 */
uint16_t kg_cmd_motion_get_mode(uint8_t index, uint8_t *mode) {
    if (index >= KG_MOTION_SENSOR_COUNT) {
        return KG_PROTOCOL_ERROR_PARAMETER_RANGE;
    } else {
    	*mode = motionMode[index];
    }
    return 0; // success
}

/**
 * @brief Set new mode for specified motion sensor
 * @param[in] index Index of motion sensor for which to get the current mode
 * @param[in] mode New motion sensor mode to set
 * @return Result code (0=success)
 */
uint16_t kg_cmd_motion_set_mode(uint8_t index, uint8_t mode) {
    if (index >= KG_MOTION_SENSOR_COUNT || mode >= KG_MOTION_MODE_MAX) {
        return KG_PROTOCOL_ERROR_PARAMETER_RANGE;
    } else {
        //motion_set_mode((motion_mode_t)mode);
        if (index == 0) {
        	motion_set_mpu6050_hand_mode(mode);
        }

        // send kg_evt_feedback_vibrate_mode packet (if we aren't setting it from an API command)
        if (!inBinPacket) {
            uint8_t payload[2] = { index, mode };
            skipPacket = 0;
            if (kg_evt_motion_mode != 0) { skipPacket = kg_evt_motion_mode(index, mode); }
            if (skipPacket == 0) { send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_MOTION, KG_PACKET_ID_EVT_MOTION_MODE, payload); }
        }
    }
    return 0; // success
}

#endif // _SUPPORT_MOTION_H_