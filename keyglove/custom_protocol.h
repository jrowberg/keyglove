// Keyglove controller source code - Special hardware setup file
// 9/9/2013 by Jeff Rowberg <jeff@rowberg.net>

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



#ifndef _CUSTOM_PROTOCOL_H_
#define _CUSTOM_PROTOCOL_H_

uint8_t filter_incoming_keyglove_packet(uint8_t *rxPacket) {
    // GUIDELINES:
    //  - We know that the packet is well-formed (correct length/payload match), but that's all.
    //  - Packet byte array argument is passed by reference, so you can modify it. BE CAREFUL IF YOU DO THIS.
    //  - Return 0 to let the packet go through (normal processing).
    //  - Return non-zero to prevent the packet from being processed.

    // EXAMPLE: block "system_ping" command
    //if (rxPacket[0] == 0xC0 && rxPacket[2] == 0x01 && rxPacket[3] == 0x01) return 1;

    // allow packet to go through if we reach this point
    return 0;
}

uint8_t filter_outgoing_keyglove_packet(uint8_t *packetType, uint8_t *payloadLength, uint8_t *packetClass, uint8_t *packetId, uint8_t *payload) {
    // GUIDELINES:
    //  - We know that the packet is well-formed (correct length/payload match), but that's all.
    //  - All arguments are passed by reference, so you can modify them. BE CAREFUL IF YOU DO THIS.
    //  - Return 0 to let the packet go through.
    //  - Return non-zero to prevent the packet from being sent.
    
    // EXAMPLE: block "system_ready" event
    //if (rxPacket[0] == 0x80 && rxPacket[2] == 0x01 && rxPacket[3] == 0x02) return 1;

    // allow packet to go through if we reach this point
    return 0;
}

uint8_t process_protocol_command_custom(uint8_t *rxPacket) {
    uint8_t protocol_error = 0;

    // check for valid custom command class/ID values
    switch (rxPacket[2]) {
        /*// EXAMPLE CLASS/COMMAND ENTRY FOR CUSTOM PROTOCOL
        case 0x80:
            // custom 0x80 command class
            switch (rxPacket[3]) {
                case 0x01:
                    // custom 0x80:0x01 class/ID
                    // TODO: implement custom functionality - validate data length, validate parameter(s), send_keyglove_packet()

                    // parameters = 0 bytes
                    if (rxPacket[1] != 0) {
                        // incorrect parameter length
                        protocol_error = KG_DEF_PROTOCOL_ERROR_PARAMETER_LENGTH;
                    } else {
                        // build response (uint32_t runtime)
                        uint8_t payload[4];
                        payload[0] = keygloveTock & 0xFF;
                        payload[1] = (keygloveTock >> 8) & 0xFF;
                        payload[2] = (keygloveTock >> 16) & 0xFF;
                        payload[3] = (keygloveTock >> 24) & 0xFF;

                        // send response
                        send_keyglove_packet(KG_PACKET_TYPE_COMMAND, sizeof(payload), rxPacket[2], rxPacket[3], payload);
                    }
                    break;

                default:
                    // valid class, but invalid ID (not defined in custom protocol)
                    protocol_error = KG_DEF_PROTOCOL_ERROR_INVALID_COMMAND;

            }
            break;
            //*/

        default:
            // invalid class (not defined in custom protocol)
            protocol_error = KG_DEF_PROTOCOL_ERROR_INVALID_COMMAND;
    }

    return protocol_error;
}

#endif // _CUSTOM_PROTOCOL_H_