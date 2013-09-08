// Keyglove controller source code - Special hardware setup file
// 7/17/2011 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

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



#ifndef _SUPPORT_HOSTIF_PROTOCOL_DECLARATIONS_H_
#define _SUPPORT_HOSTIF_PROTOCOL_DECLARATIONS_H_

#define KG_PACKET_CLASS_PROTOCOL                0x00
#define KG_PACKET_CLASS_SYSTEM                  0x01
#define KG_PACKET_CLASS_TOUCH                   0x02
#define KG_PACKET_CLASS_MOTION                  0x03
#define KG_PACKET_CLASS_FEEDBACK                0x04
#define KG_PACKET_CLASS_TOUCHSET                0x05

#define KG_LOG_LEVEL_PANIC                      0
#define KG_LOG_LEVEL_CRITICAL                   1
#define KG_LOG_LEVEL_WARNING                    3
#define KG_LOG_LEVEL_NORMAL                     5
#define KG_LOG_LEVEL_VERBOSE                    9

#define KG_INTERFACE_MODE_NONE                  0x00
#define KG_INTERFACE_MODE_OUTGOING_PACKET       0x01
#define KG_INTERFACE_MODE_INCOMING_PACKET       0x02
#define KG_INTERFACE_MODE_OUTGOING_INFO         0x04

// SYSTEM PACKETS
#define KG_PACKET_SYSTEM_BATTERY_STATUS         0x11
#define KG_PACKET_SYSTEM_EEPROM_STATUS          0x21
#define KG_PACKET_SYSTEM_EEPROM_EXPORT          0x22
#define KG_PACKET_SYSTEM_EEPROM_IMPORT          0x23
#define KG_PACKET_SYSTEM_SRAM_STATUS            0x31
#define KG_PACKET_SYSTEM_BT_RAW                 0x81

#define KG_PACKET_SYSTEM_OPTION                 0xF0
#define KG_PACKET_SYSTEM_TICK                   0xF8
#define KG_PACKET_SYSTEM_FEATURES               0xFD
#define KG_PACKET_SYSTEM_VERSION                0xFE
#define KG_PACKET_SYSTEM_RESET                  0xFF

// TOUCH PACKETS
#define KG_PACKET_TOUCH_MODE                    0x01
#define KG_PACKET_TOUCH_STATUS                  0x11
#define KG_PACKET_TOUCH_LOGIC                   0x21

// MOTION SENSOR DATA PACKETS
#define KG_PACKET_MOTION_SENSOR_MODE            0x01
#define KG_PACKET_MOTION_SENSOR_CALIBRATE       0x02

#define KG_PACKET_MOTION_ACCEL_RAW              0x11
#define KG_PACKET_MOTION_ACCELGYRO_RAW          0x12
#define KG_PACKET_MOTION_ACCELMAG_RAW           0x13
#define KG_PACKET_MOTION_ACCELGYROMAG_RAW       0x14
#define KG_PACKET_MOTION_GYRO_RAW               0x15
#define KG_PACKET_MOTION_GYROMAG_RAW            0x16
#define KG_PACKET_MOTION_MAG_RAW                0x17

#define KG_PACKET_MOTION_ACCEL_FILTERED         0x21
#define KG_PACKET_MOTION_ACCELGYRO_FILTERED     0x22
#define KG_PACKET_MOTION_ACCELMAG_FILTERED      0x23
#define KG_PACKET_MOTION_ACCELGYROMAG_FILTERED  0x24
#define KG_PACKET_MOTION_GYRO_FILTERED          0x25
#define KG_PACKET_MOTION_GYROMAG_FILTERED       0x26
#define KG_PACKET_MOTION_MAG_FILTERED           0x27

#define KG_PACKET_MOTION_IMU_MODE               0x41
#define KG_PACKET_MOTION_IMU_ORIENTATION        0x42
#define KG_PACKET_MOTION_IMU_ACCELERATION       0x43
#define KG_PACKET_MOTION_IMU_VELOCITY           0x44
#define KG_PACKET_MOTION_IMU_POSITION           0x45

#define KG_PACKET_MOTION_GESTURE                0x51

// FEEDBACK PACKETS
#define KG_PACKET_FEEDBACK_BLINK                0x01
#define KG_PACKET_FEEDBACK_RGB                  0x02
#define KG_PACKET_FEEDBACK_PIEZO                0x03
#define KG_PACKET_FEEDBACK_VIBE                 0x04

// TOUCHSET PACKETS
#define KG_PACKET_TOUCHSET_INFO                 0x01
#define KG_PACKET_TOUCHSET_LIST                 0x02
#define KG_PACKET_TOUCHSET_SELECT               0x03
#define KG_PACKET_TOUCHSET_EXPORT               0x04
#define KG_PACKET_TOUCHSET_IMPORT               0x05
#define KG_PACKET_TOUCHSET_DELETE               0x06

uint16_t create_packet(char *buffer, uint8_t cmdClass, uint8_t cmdID, uint8_t *paramStr=0, uint8_t paramNum=0xFF);
void hostif_protocol_parse(uint8_t inputByte);
uint8_t send_keyglove_info(uint8_t level, char *buffer);
uint8_t send_keyglove_info(uint8_t level, const __FlashStringHelper *buffer);
uint8_t send_keyglove_packet(char *buffer, uint8_t length, bool autoFree=false);

char *txPacket;             // buffer for outgoing data
uint16_t txPacketLength;    // full length in bytes of outgoing packet

char *rxPacket;             // buffer for incoming data
uint16_t rxPacketSize;      // container size in bytes of received packet buffer
uint16_t rxPacketLength;    // full length in bytes of received packet (may be less than rxPacketSize)
uint16_t protoError;

bool response = false;      // indicates whether we intend to generate a response
bool inBinPacket = false;   // indicates whether we are processing a received binary packet
uint8_t binDataLength;      // expected size of incoming binary data (should be rxPacketLength - 2)

#endif // _SUPPORT_HOSTIF_PROTOCOL_DECLARATIONS_H_