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



#ifndef _SUPPORT_HOSTIF_BT2_RAWHID_H_
#define _SUPPORT_HOSTIF_BT2_RAWHID_H_

#include "support_hostif_bt2.h"

#define BLUETOOTH_RAWHID_TX_SIZE          16    // transmit packet size
#define BLUETOOTH_RAWHID_RX_SIZE          16    // receive packet size

uint8_t bluetoothTXRawHIDPacket[BLUETOOTH_RAWHID_TX_SIZE + 5];
uint8_t bluetoothRXRawHIDPacket[BLUETOOTH_RAWHID_RX_SIZE + 5];

void setup_hostif_bt2_rawhid() {
    // pre-build raw HID report
    bluetoothTXRawHIDPacket[0] = 0x9F;
    bluetoothTXRawHIDPacket[1] = 0x12;
    bluetoothTXRawHIDPacket[2] = 0xA1;
    bluetoothTXRawHIDPacket[3] = 0x04;
}

void update_hostif_bt2_rawhid() {
}

#endif // _SUPPORT_HOSTIF_BT2_RAWHID_H_


