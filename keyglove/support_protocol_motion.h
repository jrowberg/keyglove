// Keyglove controller source code - Special hardware setup file
// 11/3/2013 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2013 Jeff Rowberg

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



#ifndef _SUPPORT_PROTOCOL_MOTION_H_
#define _SUPPORT_PROTOCOL_MOTION_H_

uint8_t process_protocol_command_motion(uint8_t *rxPacket) {
    // check for valid command IDs
    uint8_t protocol_error = 0;
    switch (rxPacket[3]) {
        case KG_PACKET_ID_CMD_MOTION_GET_MODE:
            // parameters = 0 bytes
            if (rxPacket[1] != 0) {
                // incorrect parameter length
                protocol_error = KG_DEF_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // build response (uint16_t result, uint8_t mode)
                uint8_t payload[3] = { 0x00, 0x00, blinkMode };

                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, sizeof(payload), rxPacket[2], rxPacket[3], payload);
            }
            break;

        case KG_PACKET_ID_CMD_MOTION_SET_MODE:
            // parameters = 1 byte (uint8_t mode)
            if (rxPacket[1] != 1) {
                // incorrect parameter length
                protocol_error = KG_DEF_PROTOCOL_ERROR_PARAMETER_LENGTH;
            } else {
                // build response (uint16_t result)
                uint8_t payload[2] = { 0x00, 0x00 };

                // validate mode
                if (rxPacket[4] > 11) {
                    payload[0] = 0x01; // 0x0001 result code = parameter(s) out of range
                } else {
                    //set_blink_mode(rxPacket[4]);
                }

                // send response
                send_keyglove_packet(KG_PACKET_TYPE_COMMAND, sizeof(payload), rxPacket[2], rxPacket[3], payload);
            }
            break;

        default:
            protocol_error = KG_DEF_PROTOCOL_ERROR_INVALID_COMMAND;
    }
    return protocol_error;
}

#endif // _SUPPORT_PROTOCOL_MOTION_H_