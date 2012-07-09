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

#define KG_INFO_LEVEL_PANIC                 0
#define KG_INFO_LEVEL_CRITICAL              1
#define KG_INFO_LEVEL_WARNING               3
#define KG_INFO_LEVEL_NORMAL                5
#define KG_INFO_LEVEL_VERBOSE               9

// MOTION SENSOR DATA PACKETS (8-BIT)
#define KG_PACKET_ACCEL_RAW8                0xA1
#define KG_PACKET_ACCELGYRO_RAW8            0xA2
#define KG_PACKET_ACCELMAG_RAW8             0xA3
#define KG_PACKET_ACCELGYROMAG_RAW8         0xA4
#define KG_PACKET_GYRO_RAW8                 0xA5
#define KG_PACKET_GYROMAG_RAW8              0xA6
#define KG_PACKET_MAG_RAW8                  0xA7
#define KG_PACKET_ACCEL_FILTERED8           0xA9
#define KG_PACKET_ACCELGYRO_FILTERED8       0xAA
#define KG_PACKET_ACCELMAG_FILTERED8        0xAB
#define KG_PACKET_ACCELGYROMAG_FILTERED8    0xAC
#define KG_PACKET_GYRO_FILTERED8            0xAD
#define KG_PACKET_GYROMAG_FILTERED8         0xAE
#define KG_PACKET_MAG_FILTERED8             0xAF

// MOTION SENSOR DATA PACKETS (16-BIT)
#define KG_PACKET_ACCEL_RAW16               0xB1
#define KG_PACKET_ACCELGYRO_RAW16           0xB2
#define KG_PACKET_ACCELMAG_RAW16            0xB3
#define KG_PACKET_ACCELGYROMAG_RAW16        0xB4
#define KG_PACKET_GYRO_RAW16                0xB5
#define KG_PACKET_GYROMAG_RAW16             0xB6
#define KG_PACKET_MAG_RAW16                 0xB7
#define KG_PACKET_ACCEL_FILTERED16          0xB9
#define KG_PACKET_ACCELGYRO_FILTERED16      0xBA
#define KG_PACKET_ACCELMAG_FILTERED16       0xBB
#define KG_PACKET_ACCELGYROMAG_FILTERED16   0xBC
#define KG_PACKET_GYRO_FILTERED16           0xBD
#define KG_PACKET_GYROMAG_FILTERED16        0xBE
#define KG_PACKET_MAG_FILTERED16            0xBF

#define KG_PACKET_ACCEL_MODE                0x90
#define KG_PACKET_ACCELGYRO_MODE            0x91
#define KG_PACKET_ACCELGYROMAG_MODE         0x92
#define KG_PACKET_GYRO_MODE                 0x93
#define KG_PACKET_MAG_MODE                  0x94

#define KG_PACKET_BATTERY_STATUS            0xE0

#define KG_PACKET_BT_RAW                    0x00

#define KG_PACKET_EEPROM_STATUS             0x00
#define KG_PACKET_EEPROM_EXPORT             0x00
#define KG_PACKET_EEPROM_IMPORT             0x00

// MOTION FUSION PACKETS
#define KG_PACKET_IMU_MODE                  0x00
#define KG_PACKET_IMU_ORIENTATION8          0x00
#define KG_PACKET_IMU_ACCELERATION8         0x00
#define KG_PACKET_IMU_VELOCITY8             0x00
#define KG_PACKET_IMU_POSITION8             0x00
#define KG_PACKET_IMU_ORIENTATION16         0x00
#define KG_PACKET_IMU_ACCELERATION16        0x00
#define KG_PACKET_IMU_VELOCITY16            0x00
#define KG_PACKET_IMU_POSITION16            0x00

#define KG_PACKET_GESTURE                   0x00
#define KG_PACKET_AUTOZERO                  0x00

#define KG_PACKET_SRAM_STATUS               0x00

// TOUCH PACKETS
#define KG_PACKET_TOUCH_MODE                0xC0
#define KG_PACKET_TOUCH_LOGIC               0xC1
#define KG_PACKET_TOUCH_STATUS              0xC2

// TOUCHSET PACKETS
#define KG_PACKET_TOUCHSET_EXPORT           0xC8
#define KG_PACKET_TOUCHSET_INFO             0xC9
#define KG_PACKET_TOUCHSET_LIST             0xCA
#define KG_PACKET_TOUCHSET_SELECT           0xCB
#define KG_PACKET_TOUCHSET_IMPORT           0xCC
#define KG_PACKET_TOUCHSET_DELETE           0xCD

#define KG_PACKET_BLINK                     0x80
#define KG_PACKET_RGB                       0x81
#define KG_PACKET_PIEZO                     0x82
#define KG_PACKET_VIBE                      0x83

#define KG_PACKET_OPTION                    0xF0
#define KG_PACKET_TICK                      0xF8
#define KG_PACKET_FEATURES                  0xFD
#define KG_PACKET_VERSION                   0xFE
#define KG_PACKET_RESET                     0xFF

uint16_t create_packet(char *buffer, bool binary, uint8_t type, uint8_t *paramStr=0, uint8_t paramNum=0xFF);
void hostif_protocol_parse(uint8_t inputByte);
uint8_t send_keyglove_info(uint8_t level, char *buffer);
uint8_t send_keyglove_info(uint8_t level, const __FlashStringHelper *buffer);
uint8_t send_keyglove_packet(char *buffer, uint8_t length, bool autoFree=false);

char *txPacket;             // buffer for outgoing data
uint16_t txPacketLength;    // full length in bytes of outgoing packet

bool packetFormatBinary;    // whether to use binary or readable format (binary is MUCH more efficient)

char *rxPacket;             // buffer for incoming data
uint16_t rxPacketSize;      // container size in bytes of received packet buffer
uint16_t rxPacketLength;    // full length in bytes of received packet (may be less than rxPacketSize)
uint16_t protoError;

bool response = false;      // indicates whether we intend to generate a response
bool inBinPacket = false;   // indicates whether we are processing a received binary packet
uint8_t binDataLength;      // expected size of incoming binary data (should be rxPacketLength - 2)

#endif // _SUPPORT_HOSTIF_PROTOCOL_DECLARATIONS_H_