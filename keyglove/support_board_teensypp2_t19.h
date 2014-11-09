// Keyglove controller source code - Host MCU declarations specific to Teensy++ v2.0 / Touch:19
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
 * @file support_board_teensypp2_t19.h
 * @brief Host MCU declarations specific to Teensy++ v2.0 / Touch:19
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * This file defines all of the hardware-specific parts of the firmware that are
 * unique to the Teensy++ v2.0 platform. Any other hardware-specific code should
 * be added here and abstracted in such a way that other files can use those new
 * features without knowing what is actually happening behind the scenes (here).
 *
 * Normally it is not necessary to edit this file.
 */

#ifndef _SUPPORT_BOARD_TEENSYPP2_T19_H_
#define _SUPPORT_BOARD_TEENSYPP2_T19_H_

// ======================== BEGIN PIN DEFINITIONS ========================

#define USBSerial Serial                    ///< USB serial interface name
#define BT2Serial Serial1                   ///< Bluetooth module UART interface name

#define KG_HOSTIF_USB_SERIAL_BAUD 115200    ///< USB serial interface baud rate
#define KG_HOSTIF_BT2_SERIAL_BAUD 125000    ///< Bluetooth module UART interface baud rate

#define KG_INTERRUPT_PIN_MPU6050_HAND  18   ///< PE6
#define KG_INTERRUPT_NUM_MPU6050_HAND  6    ///< Teensy++ interrupt #6

#define KG_PIN_BT2_CTS              19      ///< PE7
#define KG_INTERRUPT_NUM_BT2_CTS    7       ///< Teensy++ interrupt #7
#define KG_PIN_BT2_RTS              4       ///< PD4

#define KG_PIN_BLINK                6       ///< PD6
#define KG_PIN_PIEZO                24      ///< PB4
#define KG_PIN_VIBRATE              23      ///< PB3
#define KG_PIN_RGB_RED              14      ///< PC4
#define KG_PIN_RGB_GREEN            15      ///< PC5
#define KG_PIN_RGB_BLUE             16      ///< PC6

// ======================== END PIN DEFINITIONS ========================

// sensor count and base combination count
#define KG_TOTAL_SENSORS 19                 ///< Total sensor count with most efficient pin configuration
#define KG_BASE_COMBINATIONS 25             ///< Number of physically reasonable 1-to-1 touch combinations
#define KG_BASE_COMBINATION_BYTES 4         ///< Number of bytes required to store all touch status bits

// NOTE: KG_BASE_COMBINATIONS seems like it would be very high, but there are
// physical and practical limitations that make this number much smaller

#define KGT_AY(test) (test[0] & 0x01)
#define KGT_BY(test) (test[0] & 0x02)
#define KGT_CY(test) (test[0] & 0x04)
#define KGT_DY(test) (test[0] & 0x08)
#define KGT_EY(test) (test[0] & 0x10)
#define KGT_FY(test) (test[0] & 0x20)
#define KGT_GY(test) (test[0] & 0x40)
#define KGT_HY(test) (test[0] & 0x80)
#define KGT_IY(test) (test[1] & 0x01)
#define KGT_JY(test) (test[1] & 0x02)
#define KGT_KY(test) (test[1] & 0x04)
#define KGT_LY(test) (test[1] & 0x08)
#define KGT_A1(test) (test[1] & 0x10)
#define KGT_D1(test) (test[1] & 0x20)
#define KGT_G1(test) (test[1] & 0x40)
#define KGT_J1(test) (test[1] & 0x80)
#define KGT_Y4(test) (test[2] & 0x01)
#define KGT_Y5(test) (test[2] & 0x02)
#define KGT_Y6(test) (test[2] & 0x04)
#define KGT_Y7(test) (test[2] & 0x08)
#define KGT_A8(test) (test[2] & 0x10)
#define KGT_D8(test) (test[2] & 0x20)
#define KGT_G8(test) (test[2] & 0x40)
#define KGT_J8(test) (test[2] & 0x80)
#define KGT_Y1(test) (test[3] & 0x01)

#define KGT_ADY (KGT_AY && KGT_DY)
#define KGT_AJY (KGT_AY && KGT_JY)
#define KGT_DGY (KGT_DY && KGT_GY)
#define KGT_GJY (KGT_GY && KGT_JY)

#define CLR(x, y) (x &= (~(1 << y)))    ///< Bit-clearing macro for port/pin combination
#define SET(x, y) (x |= (1 << y))       ///< Bit-setting macro for port/pin combination
#define _BV(bit) (1 << (bit))           ///< Bit-value calculation macro for lazy people

extern bool interfaceUSBSerialReady;    ///< Status indicator for USB serial interface
extern uint8_t interfaceUSBSerialMode;  ///< USB serial communication mode setting @see KG_INTERFACE_MODE_NONE, @see KG_INTERFACE_MODE_OUTGOING_PACKET, @see KG_INTERFACE_MODE_INCOMING_PACKET
extern bool interfaceUSBRawHIDReady;    ///< Status indicator for USB raw HID interface
extern uint8_t interfaceUSBRawHIDMode;  ///< USB raw HID communication mode setting @see KG_INTERFACE_MODE_NONE, @see KG_INTERFACE_MODE_OUTGOING_PACKET, @see KG_INTERFACE_MODE_INCOMING_PACKET
extern bool interfaceUSBHIDReady;       ///< Status indicator for USB HID interface

void setup_board();
void update_board_touch(uint8_t *touches);

#endif // _SUPPORT_BOARD_TEENSYPP2_T19_H_
