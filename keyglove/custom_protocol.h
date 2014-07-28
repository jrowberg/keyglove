// Keyglove controller source code - Custom protocol filtering and functional addition
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
 * @file custom_protocol.h
 * @brief **USER-DEFINED:** Custom protocol filtering and functional addition
 * @author Jeff Rowberg
 * @date 2014-07-04
 *
 * This file can be used to alter the behavior of predefined API packets without
 * actually modifying the portions of code where those packets are defined. It
 * can also be used to add support for new KGAPI commands which you may want for
 * your custom functionality.
 */

#ifndef _CUSTOM_PROTOCOL_H_
#define _CUSTOM_PROTOCOL_H_

/**
 * @brief Custom filter for incoming command packets
 * @param[in,out] rxPacket Incoming packet byte array (guaranteed to be well-formed)
 * @return KGAPI command packet fallthrough, zero allows and non-zero prevents
 *
 * This function should not be used to add support for new commands; it is only
 * for filtering/modifying packets on-the-fly if desired. To add new commands,
 * refer to the process_protocol_command_custom() function instead.
 *
 * @see protocol_parse()
 * @see process_protocol_command_custom()
 */
uint8_t filter_incoming_keyglove_packet(uint8_t *rxPacket) {
    // GUIDELINES:
    //  - We know that the packet is well-formed (correct length/payload match), but that's all.
    //  - Packet byte array argument is passed by reference, so you can modify it. BE CAREFUL IF YOU DO THIS.
    //  - Return 0 to let the packet go through (normal processing).
    //  - Return non-zero to prevent the packet from being processed.

    // EXAMPLE: block "system_ping" command
    //if (rxPacket[0] == KG_PACKET_TYPE_COMMAND && rxPacket[2] == KG_PACKET_CLASS_SYSTEM && rxPacket[3] == KG_PACKET_ID_CMD_SYSTEM_PING) return 1;

    // allow packet to go through if we reach this point
    return 0;
}

/**
 * @brief Custom filter for outgoing response/event packets
 * @param[in,out] packetType Packet type (0x80 = event, 0xC0 = response)
 * @param[in,out] payloadLength Length of packet payload data
 * @param[in,out] packetClass Packet class byte
 * @param[in,out] packetId Packet ID byte
 * @param[in,out] payload Packet payload data (may be 0 if payloadLength is 0)
 * @return KGAPI response/event packet fallthrough, zero allows and non-zero prevents
 */
uint8_t filter_outgoing_keyglove_packet(uint8_t *packetType, uint8_t *payloadLength, uint8_t *packetClass, uint8_t *packetId, uint8_t *payload) {
    // GUIDELINES:
    //  - We know that the packet is well-formed (correct length/payload match), but that's all.
    //  - All arguments are passed by reference, so you can modify them. BE CAREFUL IF YOU DO THIS.
    //  - Return 0 to let the packet go through.
    //  - Return non-zero to prevent the packet from being sent.
    
    // EXAMPLE: block "system_ready" event
    //if (*packetType == KG_PACKET_TYPE_EVENT && *packetClass == KG_PACKET_CLASS_SYSTEM && *padketId == KG_PACKET_ID_EVT_SYSTEM_READY) return 1;

    // allow packet to go through if we reach this point
    return 0;
}

/**
 * @brief Custom command identification and execution
 * @param[in] rxPacket Incoming command packet byte array (guaranteed to be well-formed)
 * @return KGAPI protocol error code after parsing/detecting command data
 * @see protocol_parse()
 */
uint8_t process_protocol_command_custom(uint8_t *rxPacket) {
    uint8_t protocol_error = 0;

    // check for valid custom command class/ID values
    switch (rxPacket[2]) {
        /* // EXAMPLE CLASS/COMMAND ENTRY FOR CUSTOM PROTOCOL
        case 0x80:
            // custom 0x80 command class
            switch (rxPacket[3]) {
                case 0x01:
                    // custom 0x80:0x01 class/ID
                    // TODO: implement custom functionality - validate data length, validate parameter(s), send_keyglove_packet()

                    // custom_addition(uint8_t x, uint8_t y)(uint16_t sum)
                    // parameters = 2 bytes
                    if (rxPacket[1] != 2) {
                        // incorrect parameter length
                        protocol_error = KG_PROTOCOL_ERROR_PARAMETER_LENGTH;
                    } else {
                        // run command via your own new custom function
                        {
                            uint16_t sum;
                            uint16_t result = kg_cmd_custom_addition(x, y, &sum);
                        }

                        // *** OR handle functionality locally if desired ***
                        {
                            uint16_t sum = x + y;
                            uint16_t result = 0x0000; // <-- optional, if "result" is part of your response packet
                        }

                        // build response (note little-endian byte order for multi-byte numbers)
                        uint8_t payload[4] = { result & 0xFF, (result >> 8) & 0xFF, sum & 0xFF, (sum >> 8) & 0xFF };

                        // send response
                        send_keyglove_packet(KG_PACKET_TYPE_COMMAND, 4, rxPacket[2], rxPacket[3], payload);
                    }
                    break;

                default:
                    // valid class, but invalid ID (not defined in custom protocol)
                    protocol_error = KG_PROTOCOL_ERROR_INVALID_COMMAND;

            }
            break;
            // */

        default:
            // invalid class (not defined in custom protocol)
            protocol_error = KG_PROTOCOL_ERROR_INVALID_COMMAND;
    }

    return protocol_error;
}

#endif // _CUSTOM_PROTOCOL_H_