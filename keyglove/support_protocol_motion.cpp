// Keyglove controller source code - KGAPI "motion" protocol command parser implementation
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
 * @file support_protocol_motion.cpp
 * @brief KGAPI "motion" protocol command parser implementation
 * @author Jeff Rowberg
 * @date 2014-11-03
 *
 * This file implements subsystem-specific command processing functions for the
 * "motion" part of the KGAPI protocol.
 *
 * Normally it is not necessary to edit this file. (It is also autogenerated.)
 */

#include "keyglove.h"
#include "support_protocol.h"
//#include "support_protocol_motion.h"

/**
 * @brief Command processing routine for "motion" packet class
 * @param[in] rxPacket Incoming KGAPI packet buffer
 * @return Protocol error, if any (0 for success)
 * @see protocol_parse()
 * @see KGAPI command: kg_cmd_motion_get_mode()
 * @see KGAPI command: kg_cmd_motion_set_mode()
 */
uint8_t process_protocol_command_motion(uint8_t *rxPacket) {
    // check for valid command IDs
    uint8_t protocol_error = 0;
    switch (rxPacket[3]) {
        case KG_PACKET_ID_CMD_MOTION_GET_MODE: // 0x01
            // motion_get_mode(uint8_t index)(uint8_t mode)
            // parameters = 1 byte
            if (rxPacket[1] != 1) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint8_t mode;
                uint16_t result = kg_cmd_motion_get_mode(rxPacket[4], &mode);
        
                // build response
                uint8_t payload[1] = { mode };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 1, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_MOTION_SET_MODE: // 0x02
            // motion_set_mode(uint8_t index, uint8_t mode)(uint16_t result)
            // parameters = 2 bytes
            if (rxPacket[1] != 2) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint16_t result = kg_cmd_motion_set_mode(rxPacket[4], rxPacket[5]);
        
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

/* 0x01 */ uint8_t (*kg_evt_motion_mode)(uint8_t index, uint8_t mode) = 0;
/* 0x02 */ uint8_t (*kg_evt_motion_data)(uint8_t index, uint8_t flags, uint8_t data_len, uint8_t *data_data) = 0;
