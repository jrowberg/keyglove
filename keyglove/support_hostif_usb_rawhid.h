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



#ifndef _SUPPORT_HOSTIF_USB_RAWHID_H_
#define _SUPPORT_HOSTIF_USB_RAWHID_H_

#define STR_MANUFACTURER        L"Keyglove"
#define STR_PRODUCT             L"Keyglove Input Device"

#define VENDOR_ID               0x1D50
#define PRODUCT_ID              0x6025
#define RAWHID_USAGE_PAGE       0xFFB8  // recommended: 0xFF00 to 0xFFFF
#define RAWHID_USAGE            0x0200  // recommended: 0x0100 to 0xFFFF

#define RAWHID_TX_SIZE          64      // transmit packet size
#define RAWHID_TX_INTERVAL      2       // max # of ms between transmit packets
#define RAWHID_RX_SIZE          64      // receive packet size
#define RAWHID_RX_INTERVAL      10      // max # of ms between receive packets

#include "support_hostif_usb.h"

bool interfaceUSBRawHIDReady = false;
uint8_t interfaceUSBRawHIDMode = KG_INTERFACE_MODE_OUTGOING_PACKET | KG_INTERFACE_MODE_INCOMING_PACKET;

uint8_t txRawHIDPacket[RAWHID_TX_SIZE];
uint8_t rxRawHIDPacket[RAWHID_RX_SIZE];

void setup_hostif_usb_rawhid() {
}

void update_hostif_usb_rawhid() {
    interfaceUSBRawHIDReady = interfaceUSBReady;
    if ((interfaceUSBRawHIDMode & KG_INTERFACE_MODE_INCOMING_PACKET) != 0) {
        int8_t bytes = RawHID.recv(rxRawHIDPacket, 0);
        for (int8_t i = 0; i < bytes; i++) hostif_protocol_parse(rxRawHIDPacket[i]);
    }    
}

#endif // _SUPPORT_HOSTIF_USB_RAWHID_H_


