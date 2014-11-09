// Keyglove controller source code - KGAPI protocol core declarations
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
 * @file support_protocol.h
 * @brief KGAPI protocol core declarations
 * @author Jeff Rowberg
 * @date 2014-11-07
 */

#ifndef _SUPPORT_PROTOCOL_H_
#define _SUPPORT_PROTOCOL_H_

#include "support_protocol_system.h"
#include "support_protocol_touch.h"
#include "support_protocol_feedback.h"
#include "support_protocol_motion.h"
#include "support_protocol_flex.h"
#include "support_protocol_pressure.h"
#include "support_protocol_touchset.h"
#include "support_protocol_bluetooth.h"
#include "custom_protocol.h"

#define KG_PROTOCOL_RX_TIMEOUT                  500     ///< Number of milliseconds before KGAPI parser will timeout after an incomplete packet

#define KG_PACKET_TYPE_EVENT                    0x80    ///< First byte in header of an event packet
#define KG_PACKET_TYPE_COMMAND                  0xC0    ///< First byte in header of a command or response packet

#define KG_PACKET_CLASS_PROTOCOL                0x00
#define KG_PACKET_CLASS_SYSTEM                  0x01
#define KG_PACKET_CLASS_TOUCH                   0x02
#define KG_PACKET_CLASS_FEEDBACK                0x03
#define KG_PACKET_CLASS_MOTION                  0x04
#define KG_PACKET_CLASS_FLEX                    0x05
#define KG_PACKET_CLASS_PRESSURE                0x06
#define KG_PACKET_CLASS_TOUCHSET                0x07
#define KG_PACKET_CLASS_BLUETOOTH               0x08

#define KG_LOG_LEVEL_PANIC                      0       ///< Log level for "What a Terrible Failure" problems that will lock the MCU
#define KG_LOG_LEVEL_CRITICAL                   1       ///< Log level for critical issues that will break core functionality
#define KG_LOG_LEVEL_WARNING                    3       ///< Log level for warnings that may impact certain subsystems
#define KG_LOG_LEVEL_NORMAL                     5       ///< Log level for regular status updates
#define KG_LOG_LEVEL_VERBOSE                    9       ///< Log level for extra detailed info

// ------------------------------------------------------------------
// -------- API packets below are built into the core system --------
// ------------------------------------------------------------------

/* =========================== */
/* KGAPI CONSTANT DECLARATIONS */
/* =========================== */

#define KG_PACKET_ID_EVT_PROTOCOL_ERROR                     0x01

/* ================================ */
/* KGAPI COMMAND/EVENT DECLARATIONS */
/* ================================ */

/* 0x01 */ extern uint8_t (*kg_evt_protocol_error)(uint16_t code);

#define KG_PROTOCOL_ERROR_INVALID_COMMAND                   0x0001
#define KG_PROTOCOL_ERROR_PACKET_TIMEOUT                    0x0002
#define KG_PROTOCOL_ERROR_BAD_LENGTH                        0x0003
#define KG_PROTOCOL_ERROR_PARAMETER_LENGTH                  0x0004
#define KG_PROTOCOL_ERROR_PARAMETER_RANGE                   0x0005
#define KG_PROTOCOL_ERROR_NOT_IMPLEMENTED                   0x0006
#define KG_PROTOCOL_ERROR_NULL_POINTER                      0xADDE

// ------------------------------------------------------------------
// ------------------------------------------------------------------
// ------------------------------------------------------------------

#if KG_HOSTIF & KG_HOSTIF_USB_RAWHID
    #define USB_RAWHID_TX_SIZE 64                   ///< Payload byte count of outgoing raw HID report over USB
    #define USB_RAWHID_RX_SIZE 64                   ///< Payload byte count of incoming raw HID report over USB
#endif

// custom protocol function prototypes
uint8_t filter_incoming_keyglove_packet(uint8_t *rxPacket);
uint8_t filter_outgoing_keyglove_packet(uint8_t *packetType, uint8_t *payloadLength, uint8_t *packetClass, uint8_t *packetId, uint8_t *payload);
uint8_t process_protocol_command_custom(uint8_t *rxPacket);

// sends and receives packets via appropriate interface
uint8_t check_incoming_protocol_data();
uint8_t send_keyglove_packet(uint8_t packetType, uint8_t payloadLength, uint8_t packetClass, uint8_t packetId, uint8_t *payload);
uint8_t queue_keyglove_packet(uint8_t packetType, uint8_t payloadLength, uint8_t packetClass, uint8_t packetId, uint8_t *payload);

#if (KG_HOSTIF & HG_HOSTIF_BT2_SPP) || (KG_HOSTIF & KG_HOSTIF_BT2_HID) || (KG_HOSTIF & KG_HOSTIF_BT2_RAWHID) || (KG_HOSTIF & KG_HOSTIF_BT2_IAP)
    // see "support_bluetooth*.h" file(s) for implementation
    uint8_t bluetooth_check_incoming_protocol_data();
    uint8_t bluetooth_send_keyglove_packet_buffer(uint8_t *buffer, uint8_t length, uint8_t specificInterface);
#endif

extern bool inBinPacket;            ///< Indicates whether we have started parsing a binary packet or not
extern uint8_t binDataLength;       ///< Expected size of incoming binary data (should be rxPacketLength - 4)
extern uint8_t skipPacket;          ///< Global var to control whether event packet will be skipped due to custom handler

extern uint8_t lastCommandInterfaceNum; ///< Source interface number of last incoming command packet
extern uint8_t systemResetFlags;    ///< Controls what to reset when we use "system_reset" API command

void setup_protocol();
void protocol_parse(uint8_t inputByte);
uint16_t reset_keyglove_rx_packet();
uint8_t check_incoming_protocol_data();
uint8_t send_keyglove_log(uint8_t level, uint8_t length, const char *message);
uint8_t send_keyglove_log(uint8_t level, uint8_t length, const __FlashStringHelper *message);
uint8_t queue_keyglove_packet(uint8_t packetType, uint8_t payloadLength, uint8_t packetClass, uint8_t packetId, uint8_t *payload);
uint8_t send_keyglove_packet(uint8_t packetType, uint8_t payloadLength, uint8_t packetClass, uint8_t packetId, uint8_t *payload);
uint16_t send_keyglove_queue();

#endif // _SUPPORT_PROTOCOL_H_
