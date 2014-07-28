// Keyglove controller source code - Protocol declarations and generic support for feedback
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
 * @file support_feedback.h
 * @brief Protocol declarations and generic support for feedback
 * @author Jeff Rowberg
 * @date 2014-07-04
 *
 * This file provides the structural framework for feedback implementations in
 * the overall Keyglove architecture.
 *
 * Normally it is not necessary to edit this file.
 */

#ifndef _SUPPORT_FEEDBACK_H_
#define _SUPPORT_FEEDBACK_H_

/* =========================== */
/* KGAPI CONSTANT DECLARATIONS */
/* =========================== */

#define KG_PACKET_ID_CMD_FEEDBACK_GET_BLINK_MODE            0x01
#define KG_PACKET_ID_CMD_FEEDBACK_SET_BLINK_MODE            0x02
#define KG_PACKET_ID_CMD_FEEDBACK_GET_PIEZO_MODE            0x03
#define KG_PACKET_ID_CMD_FEEDBACK_SET_PIEZO_MODE            0x04
#define KG_PACKET_ID_CMD_FEEDBACK_GET_VIBRATE_MODE          0x05
#define KG_PACKET_ID_CMD_FEEDBACK_SET_VIBRATE_MODE          0x06
#define KG_PACKET_ID_CMD_FEEDBACK_GET_RGB_MODE              0x07
#define KG_PACKET_ID_CMD_FEEDBACK_SET_RGB_MODE              0x08
// -- command/event split --
#define KG_PACKET_ID_EVT_FEEDBACK_BLINK_MODE                0x01
#define KG_PACKET_ID_EVT_FEEDBACK_PIEZO_MODE                0x02
#define KG_PACKET_ID_EVT_FEEDBACK_VIBRATE_MODE              0x03
#define KG_PACKET_ID_EVT_FEEDBACK_RGB_MODE                  0x04

/* ================================ */
/* KGAPI COMMAND/EVENT DECLARATIONS */
/* ================================ */

#if KG_FEEDBACK & KG_FEEDBACK_BLINK
/* 0x01 */ uint16_t kg_cmd_feedback_get_blink_mode(uint8_t *mode);
#endif // KG_FEEDBACK & KG_FEEDBACK_BLINK
#if KG_FEEDBACK & KG_FEEDBACK_BLINK
/* 0x02 */ uint16_t kg_cmd_feedback_set_blink_mode(uint8_t mode);
#endif // KG_FEEDBACK & KG_FEEDBACK_BLINK
#if KG_FEEDBACK & KG_FEEDBACK_PIEZO
/* 0x03 */ uint16_t kg_cmd_feedback_get_piezo_mode(uint8_t index, uint8_t *mode, uint8_t *duration, uint16_t *frequency);
#endif // KG_FEEDBACK & KG_FEEDBACK_PIEZO
#if KG_FEEDBACK & KG_FEEDBACK_PIEZO
/* 0x04 */ uint16_t kg_cmd_feedback_set_piezo_mode(uint8_t index, uint8_t mode, uint8_t duration, uint16_t frequency);
#endif // KG_FEEDBACK & KG_FEEDBACK_PIEZO
#if KG_FEEDBACK & KG_FEEDBACK_VIBRATE
/* 0x05 */ uint16_t kg_cmd_feedback_get_vibrate_mode(uint8_t index, uint8_t *mode, uint8_t *duration);
#endif // KG_FEEDBACK & KG_FEEDBACK_VIBRATE
#if KG_FEEDBACK & KG_FEEDBACK_VIBRATE
/* 0x06 */ uint16_t kg_cmd_feedback_set_vibrate_mode(uint8_t index, uint8_t mode, uint8_t duration);
#endif // KG_FEEDBACK & KG_FEEDBACK_VIBRATE
#if KG_FEEDBACK & KG_FEEDBACK_RGB
/* 0x07 */ uint16_t kg_cmd_feedback_get_rgb_mode(uint8_t index, uint8_t *mode_red, uint8_t *mode_green, uint8_t *mode_blue);
#endif // KG_FEEDBACK & KG_FEEDBACK_RGB
#if KG_FEEDBACK & KG_FEEDBACK_RGB
/* 0x08 */ uint16_t kg_cmd_feedback_set_rgb_mode(uint8_t index, uint8_t mode_red, uint8_t mode_green, uint8_t mode_blue);
#endif // KG_FEEDBACK & KG_FEEDBACK_RGB
// -- command/event split --
#if KG_FEEDBACK & KG_FEEDBACK_BLINK
/* 0x01 */ uint8_t (*kg_evt_feedback_blink_mode)(uint8_t mode);
#endif // KG_FEEDBACK & KG_FEEDBACK_BLINK
#if KG_FEEDBACK & KG_FEEDBACK_PIEZO
/* 0x02 */ uint8_t (*kg_evt_feedback_piezo_mode)(uint8_t index, uint8_t mode, uint8_t duration, uint16_t frequency);
#endif // KG_FEEDBACK & KG_FEEDBACK_PIEZO
#if KG_FEEDBACK & KG_FEEDBACK_VIBRATE
/* 0x03 */ uint8_t (*kg_evt_feedback_vibrate_mode)(uint8_t index, uint8_t mode, uint8_t duration);
#endif // KG_FEEDBACK & KG_FEEDBACK_VIBRATE
#if KG_FEEDBACK & KG_FEEDBACK_RGB
/* 0x04 */ uint8_t (*kg_evt_feedback_rgb_mode)(uint8_t index, uint8_t mode_red, uint8_t mode_green, uint8_t mode_blue);
#endif // KG_FEEDBACK & KG_FEEDBACK_RGB

/* ================================ */
/* ================================ */
/* ================================ */

#endif // _SUPPORT_FEEDBACK_H_
