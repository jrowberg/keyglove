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



#ifndef _SUPPORT_HOSTIF_BT2_HID_H_
#define _SUPPORT_HOSTIF_BT2_HID_H_

#include "support_hostif_bt2.h"

uint8_t bluetoothTXHIDKBPacket[12];
uint8_t bluetoothRXHIDKBPacket[12];

uint8_t bluetoothTXHIDCPPacket[7];
//uint8_t bluetoothRXHIDCPPacket[7];

uint8_t bluetoothTXHIDMousePacket[9];
//uint8_t bluetoothRXHIDMousePacket[7];

// wrappers for Arduino/Teensy HID-like objects
class CBTKeyboard {
    private:
        uint8_t modifier_keys;
        uint8_t keys[6];

    public:
        void set_modifier(uint8_t c) { modifier_keys = c; }
        void set_key1(uint8_t c) { keys[0] = c; }
        void set_key2(uint8_t c) { keys[1] = c; }
        void set_key3(uint8_t c) { keys[2] = c; }
        void set_key4(uint8_t c) { keys[3] = c; }
        void set_key5(uint8_t c) { keys[4] = c; }
        void set_key6(uint8_t c) { keys[5] = c; }
        void send_now() {
            bluetoothTXHIDKBPacket[4] = modifier_keys;
            bluetoothTXHIDKBPacket[6] = keys[0];
            bluetoothTXHIDKBPacket[7] = keys[1];
            bluetoothTXHIDKBPacket[8] = keys[2];
            bluetoothTXHIDKBPacket[9] = keys[3];
            bluetoothTXHIDKBPacket[10] = keys[4];
            bluetoothTXHIDKBPacket[11] = keys[5];
            if (interfaceBT2HIDReady && (interfaceBT2HIDMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0 && bluetoothConnectionMap[bluetoothHIDDeviceIndex] -> linkHIDInterrupt != 0xFF) {
                bluetooth.smartSendData((const uint8_t *)bluetoothTXHIDKBPacket, 12, bluetoothConnectionMap[bluetoothHIDDeviceIndex] -> linkHIDInterrupt);
            }
        }
} BTKeyboard;

class CBTMouse {
    private:
        uint8_t mouse_buttons;

    public:
        void set_buttons(uint8_t left, uint8_t middle, uint8_t right) {
            uint8_t mask=0;
            if (left) mask |= 1;
            if (middle) mask |= 4;
            if (right) mask |= 2;
            mouse_buttons = mask;
            memset(bluetoothTXHIDMousePacket + 4, 0, 5); // reset all 0x00 to mouse HID report fields
            bluetoothTXHIDMousePacket[4] = mouse_buttons;
            if (interfaceBT2HIDReady && (interfaceBT2HIDMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0 && bluetoothConnectionMap[bluetoothHIDDeviceIndex] -> linkHIDInterrupt != 0xFF) {
                bluetooth.smartSendData((const uint8_t *)bluetoothTXHIDMousePacket, 9, bluetoothConnectionMap[bluetoothHIDDeviceIndex] -> linkHIDInterrupt);
            }
        }

        void move(uint8_t x, uint8_t y) {
            move(x, y, 0, 0);
        }

        void scroll(int8_t v, int8_t h) {
            move(0, 0, v, h);
        }

        void move(int8_t x, int8_t y, int8_t v, int8_t h) {
            bluetoothTXHIDMousePacket[5] = x;
            bluetoothTXHIDMousePacket[6] = y;
            bluetoothTXHIDMousePacket[7] = v;
            bluetoothTXHIDMousePacket[8] = h;
            if (interfaceBT2HIDReady && (interfaceBT2HIDMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0 && bluetoothConnectionMap[bluetoothHIDDeviceIndex] -> linkHIDInterrupt != 0xFF) {
                bluetooth.smartSendData((const uint8_t *)bluetoothTXHIDMousePacket, 9, bluetoothConnectionMap[bluetoothHIDDeviceIndex] -> linkHIDInterrupt);
            }
        }
} BTMouse;

void setup_hostif_bt2_hid() {
    // pre-build HID reports
    memset(bluetoothTXHIDKBPacket, 0, 12);
    memset(bluetoothTXHIDCPPacket, 0, 7);
    memset(bluetoothTXHIDMousePacket, 0, 9);

    // keyboard
    bluetoothTXHIDKBPacket[0] = 0x9F;
    bluetoothTXHIDKBPacket[1] = 0x0A;
    bluetoothTXHIDKBPacket[2] = 0xA1;
    bluetoothTXHIDKBPacket[3] = 0x01;

    // consumer page
    bluetoothTXHIDCPPacket[0] = 0x9F;
    bluetoothTXHIDCPPacket[1] = 0x05;
    bluetoothTXHIDCPPacket[2] = 0xA1;
    bluetoothTXHIDCPPacket[3] = 0x02;

    // mouse
    bluetoothTXHIDMousePacket[0] = 0x9F;
    bluetoothTXHIDMousePacket[1] = 0x07;
    bluetoothTXHIDMousePacket[2] = 0xA1;
    bluetoothTXHIDMousePacket[3] = 0x03;
}

void update_hostif_bt2_hid() {
}

#endif // _SUPPORT_HOSTIF_BT2_HID_H_
