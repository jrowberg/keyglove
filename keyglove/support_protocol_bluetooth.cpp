// Keyglove controller source code - KGAPI "bluetooth" protocol command parser implementation
// 2014-11-03 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/keyglove

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
 * @file support_protocol_bluetooth.cpp
 * @brief KGAPI "bluetooth" protocol command parser implementation
 * @author Jeff Rowberg
 * @date 2014-11-03
 *
 * This file implements subsystem-specific command processing functions for the
 * "bluetooth" part of the KGAPI protocol.
 *
 * Normally it is not necessary to edit this file. (It is also autogenerated.)
 */

#include "keyglove.h"
#include "support_protocol.h"
//#include "support_protocol_bluetooth.h"

/**
 * @brief Command processing routine for "bluetooth" packet class
 * @param[in] rxPacket Incoming KGAPI packet buffer
 * @return Protocol error, if any (0 for success)
 * @see protocol_parse()
 * @see KGAPI command: kg_cmd_bluetooth_get_mode()
 * @see KGAPI command: kg_cmd_bluetooth_set_mode()
 * @see KGAPI command: kg_cmd_bluetooth_reset()
 * @see KGAPI command: kg_cmd_bluetooth_get_mac()
 * @see KGAPI command: kg_cmd_bluetooth_get_pairings()
 * @see KGAPI command: kg_cmd_bluetooth_discover()
 * @see KGAPI command: kg_cmd_bluetooth_pair()
 * @see KGAPI command: kg_cmd_bluetooth_delete_pairing()
 * @see KGAPI command: kg_cmd_bluetooth_clear_pairings()
 * @see KGAPI command: kg_cmd_bluetooth_get_connections()
 * @see KGAPI command: kg_cmd_bluetooth_connect()
 * @see KGAPI command: kg_cmd_bluetooth_disconnect()
 */
uint8_t process_protocol_command_bluetooth(uint8_t *rxPacket) {
    // check for valid command IDs
    uint8_t protocol_error = 0;
    switch (rxPacket[3]) {
        case KG_PACKET_ID_CMD_BLUETOOTH_GET_MODE: // 0x01
            // bluetooth_get_mode()(uint16_t result, uint8_t mode)
            // parameters = 0 bytes
            if (rxPacket[1] != 0) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint8_t mode;
                uint16_t result = kg_cmd_bluetooth_get_mode(&mode);
        
                // build response
                uint8_t payload[3] = { result & 0xFF, (result >> 8) & 0xFF, mode };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 3, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_BLUETOOTH_SET_MODE: // 0x02
            // bluetooth_set_mode(uint8_t mode)(uint16_t result)
            // parameters = 1 byte
            if (rxPacket[1] != 1) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint16_t result = kg_cmd_bluetooth_set_mode(rxPacket[4]);
        
                // build response
                uint8_t payload[2] = { result & 0xFF, (result >> 8) & 0xFF };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 2, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_BLUETOOTH_RESET: // 0x03
            // bluetooth_reset()(uint16_t result)
            // parameters = 0 bytes
            if (rxPacket[1] != 0) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint16_t result = kg_cmd_bluetooth_reset();
        
                // build response
                uint8_t payload[2] = { result & 0xFF, (result >> 8) & 0xFF };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 2, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_BLUETOOTH_GET_MAC: // 0x04
            // bluetooth_get_mac()(uint16_t result, macaddr_t address)
            // parameters = 0 bytes
            if (rxPacket[1] != 0) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint8_t address[6];
                uint16_t result = kg_cmd_bluetooth_get_mac(address);
        
                // build response
                uint8_t payload[8] = { result & 0xFF, (result >> 8) & 0xFF, 0,0,0,0,0,0 };
                memcpy(payload + 2, address, 6);
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 8, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_BLUETOOTH_GET_PAIRINGS: // 0x05
            // bluetooth_get_pairings()(uint16_t result, uint8_t count)
            // parameters = 0 bytes
            if (rxPacket[1] != 0) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint8_t count;
                uint16_t result = kg_cmd_bluetooth_get_pairings(&count);
        
                // build response
                uint8_t payload[3] = { result & 0xFF, (result >> 8) & 0xFF, count };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 3, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_BLUETOOTH_DISCOVER: // 0x06
            // bluetooth_discover(uint8_t duration)(uint16_t result)
            // parameters = 1 byte
            if (rxPacket[1] != 1) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint16_t result = kg_cmd_bluetooth_discover(rxPacket[4]);
        
                // build response
                uint8_t payload[2] = { result & 0xFF, (result >> 8) & 0xFF };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 2, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_BLUETOOTH_PAIR: // 0x07
            // bluetooth_pair(macaddr_t address)(uint16_t result)
            // parameters = 6 bytes
            if (rxPacket[1] != 6) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint16_t result = kg_cmd_bluetooth_pair(rxPacket + 4);
        
                // build response
                uint8_t payload[2] = { result & 0xFF, (result >> 8) & 0xFF };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 2, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_BLUETOOTH_DELETE_PAIRING: // 0x08
            // bluetooth_delete_pairing(uint8_t index)(uint16_t result)
            // parameters = 1 byte
            if (rxPacket[1] != 1) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint16_t result = kg_cmd_bluetooth_delete_pairing(rxPacket[4]);
        
                // build response
                uint8_t payload[2] = { result & 0xFF, (result >> 8) & 0xFF };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 2, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_BLUETOOTH_CLEAR_PAIRINGS: // 0x09
            // bluetooth_clear_pairings()(uint16_t result)
            // parameters = 0 bytes
            if (rxPacket[1] != 0) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint16_t result = kg_cmd_bluetooth_clear_pairings();
        
                // build response
                uint8_t payload[2] = { result & 0xFF, (result >> 8) & 0xFF };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 2, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_BLUETOOTH_GET_CONNECTIONS: // 0x0A
            // bluetooth_get_connections()(uint16_t result, uint8_t count)
            // parameters = 0 bytes
            if (rxPacket[1] != 0) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint8_t count;
                uint16_t result = kg_cmd_bluetooth_get_connections(&count);
        
                // build response
                uint8_t payload[3] = { result & 0xFF, (result >> 8) & 0xFF, count };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 3, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_BLUETOOTH_CONNECT: // 0x0B
            // bluetooth_connect(uint8_t index, uint8_t profile)(uint16_t result)
            // parameters = 2 bytes
            if (rxPacket[1] != 2) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint16_t result = kg_cmd_bluetooth_connect(rxPacket[4], rxPacket[5]);
        
                // build response
                uint8_t payload[2] = { result & 0xFF, (result >> 8) & 0xFF };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 2, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_BLUETOOTH_DISCONNECT: // 0x0C
            // bluetooth_disconnect(uint8_t handle)(uint16_t result)
            // parameters = 1 byte
            if (rxPacket[1] != 1) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint16_t result = kg_cmd_bluetooth_disconnect(rxPacket[4]);
        
                // build response
                uint8_t payload[2] = { result & 0xFF, (result >> 8) & 0xFF };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 2, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        default:
            protocol_error = KG_PROTOCOL_ERROR_INVALID_COMMAND;
    }
    return protocol_error;
}

/* 0x01 */ uint8_t (*kg_evt_bluetooth_mode)(uint8_t mode) = 0;
/* 0x02 */ uint8_t (*kg_evt_bluetooth_ready)() = 0;
/* 0x03 */ uint8_t (*kg_evt_bluetooth_inquiry_response)(uint8_t *address, uint8_t *cod, int8_t rssi, uint8_t status, uint8_t index, uint8_t name_len, uint8_t *name_data) = 0;
/* 0x04 */ uint8_t (*kg_evt_bluetooth_inquiry_complete)(uint8_t count) = 0;
/* 0x05 */ uint8_t (*kg_evt_bluetooth_pairing_status)(uint8_t index, uint8_t *address, uint8_t priority, uint8_t profiles_supported, uint8_t profiles_active, uint8_t handle_list_len, uint8_t *handle_list_data) = 0;
/* 0x06 */ uint8_t (*kg_evt_bluetooth_pairing_failed)(uint8_t *address) = 0;
/* 0x07 */ uint8_t (*kg_evt_bluetooth_pairings_cleared)() = 0;
/* 0x08 */ uint8_t (*kg_evt_bluetooth_connection_status)(uint8_t handle, uint8_t *address, uint8_t index, uint8_t profile, uint8_t status) = 0;
/* 0x09 */ uint8_t (*kg_evt_bluetooth_connection_closed)(uint8_t handle, uint16_t reason) = 0;
