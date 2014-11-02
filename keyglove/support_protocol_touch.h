// Keyglove controller source code - KGAPI "touch" protocol command parser
// 2014-07-27 by Jeff Rowberg <jeff@rowberg.net>
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
 * @file support_protocol_touch.h
 * @brief KGAPI "touch" protocol command parser
 * @author Jeff Rowberg
 * @date 2014-07-27
 *
 * This file implements subsystem-specific command processing functions for the
 * "touch" part of the KGAPI protocol.
 *
 * Normally it is not necessary to edit this file. (It is also autogenerated.)
 */

#ifndef _SUPPORT_PROTOCOL_TOUCH_H_
#define _SUPPORT_PROTOCOL_TOUCH_H_

/**
 * @brief Command processing routine for "touch" packet class
 * @param[in] rxPacket Incoming KGAPI packet buffer
 * @return Protocol error, if any (0 for success)
 * @see protocol_parse()
 * @see KGAPI command: kg_cmd_touch_get_mode()
 * @see KGAPI command: kg_cmd_touch_set_mode()
 */
uint8_t process_protocol_command_touch(uint8_t *rxPacket) {
    // check for valid command IDs
    uint8_t protocol_error = 0;
    switch (rxPacket[3]) {
        case KG_PACKET_ID_CMD_TOUCH_GET_MODE: // 0x01
            // touch_get_mode()(uint8_t mode)
            // parameters = 0 bytes
            if (rxPacket[1] != 0) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint8_t mode;
                uint16_t result = kg_cmd_touch_get_mode(&mode);
        
                // build response
                uint8_t payload[1] = { mode };
        
                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 1, rxPacket[2], rxPacket[3], payload);
            }
            break;
        
        case KG_PACKET_ID_CMD_TOUCH_SET_MODE: // 0x02
            // touch_set_mode(uint8_t mode)(uint16_t result)
            // parameters = 1 byte
            if (rxPacket[1] != 1) {
                // incorrect parameter length
                protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // run command
                uint16_t result = kg_cmd_touch_set_mode(rxPacket[4]);
        
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

#endif // _SUPPORT_PROTOCOL_TOUCH_H_