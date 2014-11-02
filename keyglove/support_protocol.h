// Keyglove controller source code - KGAPI protocol core implementation
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
 * @file support_protocol.h
 * @brief KGAPI protocol core implementation
 * @author Jeff Rowberg
 * @date 2014-07-04
 *
 * This file implements the structure behind the Keyglove binary API protocol.
 * The incoming data stream parsing routines, error detection, timeout handling,
 * packet queuing, and packet sending functionality is all here. A few core API
 * commands and events are also implemented here. Subsystem-specific API packets
 * are defined and implemented elsewhere.
 *
 * Normally it is not necessary to edit this file.
 */

#ifndef _SUPPORT_PROTOCOL_H_
#define _SUPPORT_PROTOCOL_H_

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

/* 0x01 */ uint8_t (*kg_evt_protocol_error)(uint16_t code);

#define KG_PROTOCOL_ERROR_INVALID_COMMAND                   0x0001
#define KG_PROTOCOL_ERROR_PACKET_TIMEOUT                    0x0002
#define KG_PROTOCOL_ERROR_BAD_LENGTH                        0x0003
#define KG_PROTOCOL_ERROR_PARAMETER_LENGTH                  0x0004
#define KG_PROTOCOL_ERROR_PARAMETER_RANGE                   0x0005
#define KG_PROTOCOL_ERROR_NOT_IMPLEMENTED                   0x0006
#define KG_PROTOCOL_ERROR_NULL_POINTER                      0xADDE

// ------------------------------------------------------------------

/* =========================== */
/* KGAPI CONSTANT DECLARATIONS */
/* =========================== */

#define KG_PACKET_ID_CMD_SYSTEM_PING                        0x01
#define KG_PACKET_ID_CMD_SYSTEM_RESET                       0x02
#define KG_PACKET_ID_CMD_SYSTEM_GET_INFO                    0x03
#define KG_PACKET_ID_CMD_SYSTEM_SET_TIMER                   0x04
#define KG_PACKET_ID_CMD_SYSTEM_GET_BATTERY_STATUS          0x05
// -- command/event split --
#define KG_PACKET_ID_EVT_SYSTEM_BOOT                        0x01
#define KG_PACKET_ID_EVT_SYSTEM_READY                       0x02
#define KG_PACKET_ID_EVT_SYSTEM_ERROR                       0x03
#define KG_PACKET_ID_EVT_SYSTEM_TIMER_TICK                  0x04
#define KG_PACKET_ID_EVT_SYSTEM_BATTERY_STATUS              0x05

/* ================================ */
/* KGAPI COMMAND/EVENT DECLARATIONS */
/* ================================ */

/* 0x01 */ uint16_t kg_cmd_system_ping(uint32_t *runtime);
/* 0x02 */ uint16_t kg_cmd_system_reset(uint8_t type);
/* 0x03 */ uint16_t kg_cmd_system_get_info(uint8_t *major, uint8_t *minor, uint8_t *patch, uint32_t *timestamp);
/* 0x04 */ uint16_t kg_cmd_system_set_timer(uint8_t handle, uint16_t interval, uint8_t oneshot);
/* 0x05 */ uint16_t kg_cmd_system_get_battery_status(uint8_t *status, uint8_t *level);
// -- command/event split --
/* 0x01 */ uint8_t (*kg_evt_system_boot)(uint8_t major, uint8_t minor, uint8_t patch, uint32_t timestamp);
/* 0x02 */ uint8_t (*kg_evt_system_ready)();
/* 0x03 */ uint8_t (*kg_evt_system_error)(uint16_t code);
/* 0x04 */ uint8_t (*kg_evt_system_timer_tick)(uint8_t handle, uint32_t seconds, uint8_t subticks);
/* 0x05 */ uint8_t (*kg_evt_system_battery_status)(uint8_t status, uint8_t level);

#define KG_SYSTEM_RESET_TYPE_NORMAL                         0x01
#define KG_SYSTEM_RESET_TYPE_KGONLY                         0x02

// ------------------------------------------------------------------
// ------------------------------------------------------------------
// ------------------------------------------------------------------

#if KG_HOSTIF & KG_HOSTIF_USB_RAWHID
    #define USB_RAWHID_TX_SIZE 64                   ///< Payload byte count of outgoing raw HID report over USB
    #define USB_RAWHID_RX_SIZE 64                   ///< Payload byte count of incoming raw HID report over USB
    uint8_t rxRawHIDPacket[USB_RAWHID_RX_SIZE];     ///< Outgoing raw HID report buffer
    uint8_t txRawHIDPacket[USB_RAWHID_TX_SIZE];     ///< Incoming raw HID report buffer
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

uint16_t rxByte;            ///< Container for incoming byte (actually WORD but the high byte is for .read() error checking)
uint8_t *rxPacket;          ///< Dynamic buffer for incoming KGAPI data
uint16_t rxPacketSize;      ///< Current size of container for received packet buffer
uint16_t rxPacketLength;    ///< Full length in bytes of received packet (may be less than rxPacketSize)
uint32_t packetStartTime;   ///< Incoming command packet timeout detection reference

uint8_t *txQueue;           ///< Dynamic buffer for TX packet queue
uint16_t txQueueSize;       ///< Allocated memory for the TX queue
uint16_t txQueueLength;     ///< Number of bytes used in the TX queue

bool inBinPacket = false;   ///< Indicates whether we have started parsing a binary packet or not
uint8_t binDataLength;      ///< Expected size of incoming binary data (should be rxPacketLength - 4)
uint8_t skipPacket = 0;     ///< Global var to control whether event packet will be skipped due to custom handler

uint8_t lastCommandInterfaceNum;    ///< Source interface number of last incoming command packet

// pre-declare packet handlers, which must be defined elsewhere if enabled
// (we have to call these functions from this file, but this is compiled before others)

uint8_t process_protocol_command_system(uint8_t *rxPacket);
uint8_t process_protocol_command_touch(uint8_t *rxPacket);
uint8_t process_protocol_command_feedback(uint8_t *rxPacket);
uint8_t process_protocol_command_motion(uint8_t *rxPacket);
uint8_t process_protocol_command_flex(uint8_t *rxPacket);
uint8_t process_protocol_command_pressure(uint8_t *rxPacket);
uint8_t process_protocol_command_touchset(uint8_t *rxPacket);
uint8_t process_protocol_command_bluetooth(uint8_t *rxPacket);

uint8_t systemResetFlags = 0; ///< Controls what to reset when we use "system_reset" API command

/**
 * @brief Initialize protocol buffers and BGAPI parser
 */
void setup_protocol() {
    // allocate RX packet buffer on fresh boot
    if (rxPacketSize == 0 || rxPacket == 0) {
        rxPacketSize = 32;
        rxPacket = (uint8_t *)malloc(rxPacketSize);
        rxPacketLength = 0;
    }
}

/**
 * @brief Parse the next incoming KGAPI protocol byte
 * @param[in] inputByte Incoming byte to parse
 */
void protocol_parse(uint8_t inputByte) {
    if (rxPacketLength + 1 == rxPacketSize) {
        rxPacketSize += 32;
        rxPacket = (uint8_t *)realloc(rxPacket, rxPacketSize);
        if (!rxPacket) {
            // failed to reallocate to new chunk of memory
            // this should NEVER happen, but if it does, I want to know
            uint8_t payload[2] = { KG_PROTOCOL_ERROR_NULL_POINTER & 0xFF, KG_PROTOCOL_ERROR_NULL_POINTER >> 8 };
            skipPacket = 0;
            if (kg_evt_protocol_error != 0) { skipPacket = kg_evt_protocol_error(KG_PROTOCOL_ERROR_NULL_POINTER); }
            if (skipPacket == 0) { send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload); }
            return;
        }
    }
    if (!inBinPacket && inputByte == KG_PACKET_TYPE_COMMAND) { // "bait" byte, 0xC0
        packetStartTime = millis();
        inBinPacket = true;
        rxPacket[0] = inputByte;
        rxPacketLength = 1; // initialize buffer length to include only 1st header byte (so far)
        binDataLength = 0;
    } else if (inBinPacket && rxPacketLength == 1) { // data length byte
        binDataLength = inputByte;
        if (binDataLength > 250) {
            // error (data payload too long)
            uint8_t payload[2] = { KG_PROTOCOL_ERROR_BAD_LENGTH, 0x00 };
            skipPacket = 0;
            if (kg_evt_protocol_error != 0) { skipPacket = kg_evt_protocol_error(payload[0]); }
            if (skipPacket == 0) { send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload); }
            inBinPacket = false;
            rxPacketLength = 0;
        } else {
            rxPacket[rxPacketLength++] = inputByte;
        }
    } else {
        rxPacket[rxPacketLength++] = inputByte;
        if (inBinPacket && rxPacketLength - 4 == binDataLength) {
            // process packet that just came in, passing to appropriate main handler
            uint8_t protocol_error = 0;

            // filter incoming packets for custom behavior
            if (filter_incoming_keyglove_packet(rxPacket) == 0) {
                switch (rxPacket[2]) {
                    //case KG_PACKET_CLASS_PROTOCOL:

                    case KG_PACKET_CLASS_SYSTEM:
                        protocol_error = process_protocol_command_system(rxPacket);
                        break;
    
                    case KG_PACKET_CLASS_TOUCH:
                        protocol_error = process_protocol_command_touch(rxPacket);
                        break;
    
                    #if KG_MOTION > 0
                        case KG_PACKET_CLASS_MOTION:
                            protocol_error = process_protocol_command_motion(rxPacket);
                            break;
                    #endif
    
                    #if KG_FEEDBACK > 0
                        case KG_PACKET_CLASS_FEEDBACK:
                            protocol_error = process_protocol_command_feedback(rxPacket);
                            break;
                    #endif
    
                    #if KG_FLEX > 0
                        case KG_PACKET_CLASS_FLEX:
                            protocol_error = process_protocol_command_flex(rxPacket);
                            break;
                    #endif
    
                    #if KG_PRESSURE > 0
                        case KG_PACKET_CLASS_PRESSURE:
                            protocol_error = process_protocol_command_pressure(rxPacket);
                            break;
                    #endif
    
                    #if KG_TOUCHSET > 0
                        case KG_PACKET_CLASS_TOUCHSET:
                            protocol_error = process_protocol_command_touchset(rxPacket);
                            break;
                    #endif
    
                    #if (KG_HOSTIF & HG_HOSTIF_BT2_SPP) || (KG_HOSTIF & KG_HOSTIF_BT2_HID) || (KG_HOSTIF & KG_HOSTIF_BT2_RAWHID) || (KG_HOSTIF & KG_HOSTIF_BT2_IAP)
                        case KG_PACKET_CLASS_BLUETOOTH:
                            protocol_error = process_protocol_command_bluetooth(rxPacket);
                            break;
                    #endif
    
                    default:
                        // check for custom protocol, will return KG_PROTOCOL_ERROR_INVALID_COMMAND if no matches
                        protocol_error = KG_PROTOCOL_ERROR_INVALID_COMMAND;
                        break;
                }

                // check for errors (e.g. unhandled, bad arguments, etc.)
                if (protocol_error) {
                    // check for custom protocol, will return KG_PROTOCOL_ERROR_INVALID_COMMAND if no matches
                    protocol_error = process_protocol_command_custom(rxPacket);

                    // if we still have an error, then there is no custom functionality implemented for this class/ID combination
                    if (protocol_error) {
                        uint8_t payload[2] = { protocol_error, 0x00 };
                        skipPacket = 0;
                        if (kg_evt_protocol_error != 0) { skipPacket = kg_evt_protocol_error(payload[0]); }
                        if (skipPacket == 0) { send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload); }
                    }
                }
            }

            // reset packet status/length
            inBinPacket = false;
            rxPacketLength = 0;
        }
    }
}

/**
 * @brief Check all relevant interfaces for any incoming protocol data
 * @return Result, zero for success or non-zero for error
 */
uint8_t check_incoming_protocol_data() {
    #if KG_HOSTIF & KG_HOSTIF_USB_SERIAL
        // read available data from USB virtual serial
        if (interfaceUSBSerialReady && (interfaceUSBSerialMode & KG_INTERFACE_MODE_INCOMING_PACKET) != 0) {
            while ((rxByte = USBSerial.read()) < 256) {
                lastCommandInterfaceNum = KG_INTERFACENUM_USB_SERIAL;
                protocol_parse((uint8_t)rxByte);
            }
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_USB_RAWHID
        // read raw HID data over USB
        if (interfaceUSBRawHIDReady && (interfaceUSBRawHIDMode & KG_INTERFACE_MODE_INCOMING_PACKET) != 0) {
            int8_t bytes = RawHID.recv(rxRawHIDPacket, 0);
            if (bytes > 0) {
                lastCommandInterfaceNum = KG_INTERFACENUM_USB_RAWHID;
                for (int8_t i = 0; i < rxRawHIDPacket[0]; i++) protocol_parse(rxRawHIDPacket[i + 1]);
            }
        }
    #endif

    #if (KG_HOSTIF & HG_HOSTIF_BT2_SPP) || (KG_HOSTIF & KG_HOSTIF_BT2_HID) || (KG_HOSTIF & KG_HOSTIF_BT2_RAWHID) || (KG_HOSTIF & KG_HOSTIF_BT2_IAP)
        // see if there is any data to deal with from Bluetooth
        bluetooth_check_incoming_protocol_data();
    #endif

    // check for protocol timeout condition
    if (inBinPacket && (millis() - packetStartTime) > KG_PROTOCOL_RX_TIMEOUT) {
        // error (data payload too long)
        uint8_t payload[2] = { KG_PROTOCOL_ERROR_PACKET_TIMEOUT, 0x00 };
        skipPacket = 0;
        if (kg_evt_protocol_error != 0) { skipPacket = kg_evt_protocol_error(payload[0]); }
        if (skipPacket == 0) { send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload); }
        inBinPacket = false;
        rxPacketLength = 0;
    }

    return 0;
}

/**
 * @brief Send a special log message (from RAM) out the main serial interface
 * @param[in] level Log level
 * @param[in] length Number of bytes in message
 * @param[in] message Message to send (normal variable in RAM)
 * @return Result, zero for success or non-zero for error
 */
uint8_t send_keyglove_log(uint8_t level, uint8_t length, const char *message) {
    uint8_t beginning[] = { 0x80, length + 1, 0xFF, 0xFF, level };
    Serial.write(beginning, 5);
    Serial.println((const char *)message);
    return 0;
}

/**
 * @brief Send a special log message (from flash) out the main serial interface
 * @param[in] level Log level
 * @param[in] length Number of bytes in message
 * @param[in] message Message to send (from flash)
 * @return Result, zero for success or non-zero for error
 */
uint8_t send_keyglove_log(uint8_t level, uint8_t length, const __FlashStringHelper *message) {
    uint8_t beginning[] = { 0x80, length + 1, 0xFF, 0xFF, level };
    Serial.write(beginning, 5);
    Serial.println((const __FlashStringHelper *)message);
    return 0;
}

/**
 * @brief Add an outgoing packet (response or event) to the queue to send later
 * @param[in] packetType Type of packet to send
 * @param[in] payloadLength Number of bytes in data payload (0 or more)
 * @param[in] packetClass Packet class ID byte
 * @param[in] packetId Packet command ID byte
 * @param[in] payload Payload data byte array
 * @return Result, zero for success or non-zero for error
 */
uint8_t queue_keyglove_packet(uint8_t packetType, uint8_t payloadLength, uint8_t packetClass, uint8_t packetId, uint8_t *payload) {
    uint8_t packetLength = payloadLength + 4;
    if (txQueueSize == 0) {
        if (!(txQueue = (uint8_t *)malloc(384))) {
            return 1; // couldn't allocate block of memory
        }
        txQueueSize = 384;
    } else if (txQueueSize + packetLength > txQueueSize) {
        if (!(txQueue = (uint8_t *)realloc(txQueue, txQueueSize + packetLength + 64))) {
            return 2; // couldn't re-allocate bigger block of memory
        }
        txQueueSize += packetLength + 64;
    }
    txQueue[txQueueLength++] = packetType;
    txQueue[txQueueLength++] = payloadLength;
    txQueue[txQueueLength++] = packetClass;
    txQueue[txQueueLength++] = packetId;
    if (payloadLength) {
        memcpy(txQueue + txQueueLength, payload, payloadLength);
        txQueueLength += payloadLength;
    }
    return 0;
}

/**
 * @brief Send an outgoing packet (response or event) immediately
 * @param[in] packetType Type of packet to send
 * @param[in] payloadLength Number of bytes in data payload (0 or more)
 * @param[in] packetClass Packet class ID byte
 * @param[in] packetId Packet command ID byte
 * @param[in] payload Payload data byte array
 * @return Result, zero for success or non-zero for error
 */
uint8_t send_keyglove_packet(uint8_t packetType, uint8_t payloadLength, uint8_t packetClass, uint8_t packetId, uint8_t *payload) {
    // validate payload length
    if ((payload == NULL && payloadLength > 0) || payloadLength > 250) {
        // payload specified but not provided, or too long
        return 1;
    }
    
    // filter outgoing packets for custom behavior
    if (filter_outgoing_keyglove_packet(&packetType, &payloadLength, &packetClass, &packetId, payload)) return 255;

    // allocate and check full packet buffer
    uint8_t *buffer = (uint8_t *)malloc(4 + payloadLength);
    if (buffer == 0) {
        // couldn't allocate packet buffer...uh oh
        return 2;
    }

    // certain outgoing packets should only be sent on one specific interface, the last one which was used
    uint8_t specificInterface = (packetType != KG_PACKET_TYPE_EVENT || packetClass == KG_PACKET_CLASS_PROTOCOL);

    buffer[0] = packetType;
    buffer[1] = payloadLength;
    buffer[2] = packetClass;
    buffer[3] = packetId;
    if (payloadLength) memcpy(buffer + 4, payload, payloadLength);
    uint8_t length = 4 + payloadLength;

    #if KG_HOSTIF & KG_HOSTIF_USB_SERIAL
        if (!specificInterface || lastCommandInterfaceNum == KG_INTERFACENUM_USB_SERIAL) {
            // send packet out over wired serial (USB virtual serial)
            if (interfaceUSBSerialReady && (interfaceUSBSerialMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0) {
                USBSerial.write((const uint8_t *)buffer, length); // packet data
            }
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_USB_RAWHID
        if (!specificInterface || lastCommandInterfaceNum == KG_INTERFACENUM_USB_RAWHID) {
            // send packet out over wired custom HID interface (USB raw HID)
            // 64-byte packets, formatted where byte 0 is [0-64] and bytes 1-63 are data
            if (interfaceUSBRawHIDReady && (interfaceUSBRawHIDMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0) {
                int8_t bytes;
                for (uint8_t i = 0; i < length; i += (USB_RAWHID_TX_SIZE - 1)) {
                    memset(txRawHIDPacket, 0, USB_RAWHID_TX_SIZE);
                    txRawHIDPacket[0] = min(USB_RAWHID_TX_SIZE - 1, length - i);
                    for (uint8_t j = 0; j < (USB_RAWHID_TX_SIZE - 1); j++) {
                        if (i + j >= length) break;
                        txRawHIDPacket[j + 1] = buffer[i + j];
                    }
                    bytes = RawHID.send(txRawHIDPacket, 2);
                }
            }
        }
    #endif

    #if (KG_HOSTIF & HG_HOSTIF_BT2_SPP) || (KG_HOSTIF & KG_HOSTIF_BT2_HID) || (KG_HOSTIF & KG_HOSTIF_BT2_RAWHID) || (KG_HOSTIF & KG_HOSTIF_BT2_IAP)
        // send packet via Bluetooth if necessary
        bluetooth_send_keyglove_packet_buffer(buffer, length, specificInterface);
    #endif

    // KG_HID_KEYBOARD and KG_HID_MOUSE are handled elsewhere and deal with other kinds of data
    
    // tidy up!
    free(buffer);

    return 0;
}

/**
 * @brief Check for and send next available queued packet (if any)
 * @return Number of bytes still used in outgoing queue
 * @see queue_keyglove_packet()
 */
uint16_t send_keyglove_queue() {
    // check for queued packets
    if (txQueueLength) {
        send_keyglove_packet(txQueue[0], txQueue[1], txQueue[2], txQueue[3], txQueue + 4);
        if (txQueueLength > txQueue[1] + 4) {
            // queue has more left, so move it into position
            txQueueLength -= (txQueue[1] + 4);
            memmove(txQueue, txQueue + (txQueue[1] + 4), txQueueLength);
        } else {
            // queue is empty, free up the memory
            txQueueLength = 0;
            txQueueSize = 0;
            free(txQueue);
        }
    }
    return txQueueLength;
}

/* ============================= */
/* KGAPI COMMAND IMPLEMENTATIONS */
/* ============================= */

/**
 * @brief Test communication with Keyglove device and get current runtime
 * @param[out] runtime Number of seconds since last boot/reset
 * @return Result code (0=success)
 */
uint16_t kg_cmd_system_ping(uint32_t *runtime) {
    *runtime = keygloveTock;
    return 0; // success
}

/**
 * @brief Reset Keyglove device
 * @param[in] type Type of reset to perform
 * @return Result code (0=success)
 */
uint16_t kg_cmd_system_reset(uint8_t type) {
    // validate type
    if (type < 1 || type > 2) {
        return KG_PROTOCOL_ERROR_PARAMETER_RANGE;
    }

    // send response
    uint8_t payload[] = { 0x00, 0x00 };
    send_keyglove_packet(KG_PACKET_TYPE_COMMAND, sizeof(payload), rxPacket[2], rxPacket[3], payload);

    // reboot Keyglove
    inBinPacket = false;
    rxPacketLength = 0;
    systemResetFlags = 0xFF; // reset everything
    if (type == KG_SYSTEM_RESET_TYPE_KGONLY) {
        systemResetFlags = 0; // don't reset anything except the main core system
    }
    setup();
    return 0xFFFF; // success, but DON'T send response packet (already done)
}

/**
 * @brief Get firmware build info
 * @param[out] major Firmware major version number
 * @param[out] minor Firmware minor version number
 * @param[out] patch Firmware patch version number
 * @param[out] timestamp Firmware build timestamp
 * @return Result code (0=success)
 */
uint16_t kg_cmd_system_get_info(uint8_t *major, uint8_t *minor, uint8_t *patch, uint32_t *timestamp) {
    *major = KG_FIRMWARE_VERSION_MAJOR;
    *minor = KG_FIRMWARE_VERSION_MINOR;
    *patch = KG_FIRMWARE_VERSION_PATCH;
    *timestamp = KG_FIRMWARE_BUILD_TIMESTAMP;
    return 0; // success
}

/**
 * @brief Set a timer interval to trigger future behavior
 * @param[in] handle Timer handle (0-7)
 * @param[in] interval Interval (10ms units)
 * @param[in] oneshot Repeating (0) or one-shot (1)
 * @return Result code (0=success)
 */
uint16_t kg_cmd_system_set_timer(uint8_t handle, uint16_t interval, uint8_t oneshot) {
    if (handle > 7) {
        return KG_PROTOCOL_ERROR_PARAMETER_RANGE;
    }
    if (interval == 0) {
        // stop this timer
        keygloveSoftTimers &= ~(1 << handle);
    } else {
        // schedule/reschedule this timer
        keygloveSoftTimers |= (1 << handle);
        if (oneshot) keygloveSoftTimersRepeat &= ~(1 << handle);
        else keygloveSoftTimersRepeat |= (1 << handle);
        keygloveSoftTimerInterval[handle] = interval;
        keygloveSoftTimerSec[handle] = keygloveTock + (interval / 100);
        keygloveSoftTimer10ms[handle] = keygloveTick + (interval % 100);
        if (keygloveSoftTimer10ms[handle] > 99) {
            keygloveSoftTimer10ms[handle] -= 100;
            keygloveSoftTimerSec[handle]++;
        }
    }
    return 0; // success
}

/**
 * @brief Get battery status
 * @param[out] status Battery status
 * @param[out] level Charge level (0-100)
 * @return Result code (0=success)
 */
uint16_t kg_cmd_system_get_battery_status(uint8_t *status, uint8_t *level) {
    *status = keygloveBatteryStatus;
    *level = keygloveBatteryLevel;
    return 0; // success
}

#endif // _SUPPORT_PROTOCOL_H_