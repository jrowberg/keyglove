// Keyglove controller source code - Custom protocol filtering declarations
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
 * @file custom_protocol.h
 * @brief **USER-DEFINED:** Custom protocol filtering declarations
 * @author Jeff Rowberg
 * @date 2014-11-07
 */

#ifndef _CUSTOM_PROTOCOL_H_
#define _CUSTOM_PROTOCOL_H_

uint8_t filter_incoming_keyglove_packet(uint8_t *rxPacket);
uint8_t filter_outgoing_keyglove_packet(uint8_t *packetType, uint8_t *payloadLength, uint8_t *packetClass, uint8_t *packetId, uint8_t *payload);
uint8_t process_protocol_command_custom(uint8_t *rxPacket);

#endif // _CUSTOM_PROTOCOL_H_
