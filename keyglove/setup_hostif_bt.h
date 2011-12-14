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



#ifndef _SETUP_HOSTIF_BT_H_
#define _SETUP_HOSTIF_BT_H_

#define ENABLE_BLUETOOTH

#include "iWRAP.h"

#if !defined(ARDUINO) || KG_BOARD == KG_BOARD_TEENSYPP2
    HardwareSerial Uart = HardwareSerial();
#endif /* USE_TEENSY */
#if KG_BOARD == KG_BOARD_ARDUINO_MEGA2560
    HardwareSerial Uart = Serial3;
#endif /* USE_ARDUINO */

iWRAP bluetooth = iWRAP(&Uart, (HardwareSerial *)&Serial);
bool bluetoothLinkActive = false;

void event_onExitDataMode() {
    Serial.println("Exiting data mode...");
    digitalWrite(KG_PIN_BT_DTR, HIGH);
    delayMicroseconds(500);
    digitalWrite(KG_PIN_BT_DTR, LOW);
}
void event_onSelectLink(iWRAPLink *link) {
    Serial.print("Selecting link ");
    Serial.println(link -> link_id);
}

void event_onModuleIdle() {
}

void event_onModuleBusy() {
}

void event_onModuleRing(iWRAPLink *link) {
    bluetoothLinkActive = true;
}

void event_onModuleNoCarrier() {
    bluetoothLinkActive = false;
}

void setup_hostif_bt() {
    // initialize pins
    pinMode(KG_PIN_BT_DTR, OUTPUT);
    pinMode(KG_PIN_BT_LINK, INPUT);
    digitalWrite(KG_PIN_BT_DTR, LOW);

    // initialize iWRAP device
    Uart.begin(38400);

    // assign special busy/idle callbacks because...well...because we can
    bluetooth.onBusy(event_onModuleBusy);
    bluetooth.onIdle(event_onModuleIdle);

    // assign more important RING/NO CARRIER callbacks
    bluetooth.onRing(event_onModuleRing);
    bluetooth.onNoCarrier(event_onModuleNoCarrier);
    
    // assign manual control of exiting data mode (DTR_PIN)
    bluetooth.onExitDataMode(event_onExitDataMode);

    // enable local module -> PC passthrough echo for fun/debugging
    bluetooth.setEchoModuleOutput(true);

    // check for already active link
    if (digitalRead(KG_PIN_BT_LINK)) {
        Serial.println("Device appears to have active data connection!");
        bluetooth.setDeviceMode(IWRAP_MODE_DATA);
        bluetoothLinkActive = true;
    }

    // get all configuration parameters and wait for completion
    Serial.println("Reading iWRAP configuration...");
    bluetooth.readDeviceConfig();
    while (bluetooth.checkActivity(1000));
    if (bluetooth.checkError()) {
        Serial.println("iWRAP config read generated a syntax error, trying again...");
        bluetooth.readDeviceConfig();
        while (bluetooth.checkActivity(1000));
    } else if (bluetooth.checkTimeout()) {
        // assume MUX mode is enabled for testing
        /*Serial.println("iWRAP config read timed out, trying MUX mode...");
        bluetooth.setDeviceMode(IWRAP_MODE_MUX);
        bluetooth.readDeviceConfig();
        while (bluetooth.checkActivity(1000));*/
    }
    if (bluetooth.checkError() || bluetooth.checkTimeout()) {
        Serial.println("iWRAP config could not be read. Something isn't right.");
    } else {
        // disable MUX mode if necessary
        /*if (bluetooth.getControlMuxEnabled()) {
            Serial.println("Disabling MUX mode...");
            bluetooth.setControlMuxEnabled(false);
            while (bluetooth.checkActivity(1000));
            if (bluetooth.checkError()) {
                Serial.println("Darn!");
            }
        }*/

        // get list of active links
        Serial.println("Reading list link configuration...");
        bluetooth.readLinkConfig();
        while (bluetooth.checkActivity(1000));
        
        // enable HID profile if necessary
        if (!bluetooth.config.profileHIDEnabled) {
            Serial.println("Enabling HID profile...");
            bluetooth.setProfile("HID", "iWRAP Demo Device");
            while (bluetooth.checkActivity());
            if (bluetooth.checkError()) {
                Serial.println("Uh oh, that didn't work out so well.");
            } else {
                Serial.println("Rebooting iWRAP device again to activate profile...");
                bluetooth.resetDevice();
                while (bluetooth.checkActivity());
                Serial.println("Re-reading iWRAP configuration...");
                bluetooth.readDeviceConfig();
                while (bluetooth.checkActivity());
            }
        } else {
            Serial.println("Detected HID profile already enabled");
        }

        // set device name for demo
        Serial.println("Setting device name...");
        bluetooth.setDeviceName("Keyglove");

        // set device identity for demo
        Serial.println("Setting device identity...");
        bluetooth.setDeviceIdentity("Keyglove Input Device");

        // set device class to KB/mouse
        Serial.println("Setting device class to keyboard/mouse...");
        bluetooth.setDeviceClass(0x05C0);

        // set SSP mode to 3 0
        Serial.println("Setting Secure Simple Pairing (SSP) mode to 3/0...");
        bluetooth.setDeviceSSP(3, 0);

        // enable CD pair notification on GPIO pin 7
        Serial.println("Enabling GPIO7 toggling on Carrier Detect (link active mode)...");
        bluetooth.setCarrierDetect(0x80, 0);

        // enable DTR escape control on GPIO pin 6
        Serial.println("Enabling GPIO6 DTR detection for entering command mode (ESCAPE char disabled)...");
        bluetooth.setControlEscape('-', 0x40, 1);

        // output some info to prove we read the config correctly
        Serial.print("+ BT address=");
        Serial.print(bluetooth.config.btAddress.address[0], HEX); Serial.print(" ");
        Serial.print(bluetooth.config.btAddress.address[1], HEX); Serial.print(" ");
        Serial.print(bluetooth.config.btAddress.address[2], HEX); Serial.print(" ");
        Serial.print(bluetooth.config.btAddress.address[3], HEX); Serial.print(" ");
        Serial.print(bluetooth.config.btAddress.address[4], HEX); Serial.print(" ");
        Serial.println(bluetooth.config.btAddress.address[5], HEX);
        Serial.print("+ BT name=");
        Serial.println(bluetooth.config.btName);
        Serial.print("+ BT IDENT description=");
        Serial.println(bluetooth.config.btIdentDescription);
        Serial.print("+ UART baud rate=");
        Serial.println(bluetooth.config.uartBaudRate);
        Serial.print("+ Current pairing count=");
        Serial.println(bluetooth.config.btPairCount);
        
        if (digitalRead(KG_PIN_BT_LINK)) {
            Serial.print("Switching back to data mode for previously active link...");
            bluetooth.selectDataMode((uint8_t)0); // use default link since we don't know what it was
            while (bluetooth.checkActivity(1000));
        }
    }
}

void update_hostif_bt() {
    // passthrough from PC to iWRAP for debugging or manual control
    //while (Serial.available()) Uart.write(Serial.read());

    // check for module activity
    // (This method automatically watches for data from the module and parses
    // anything that comes in; you must either call this often or else manage
    // reading the data yourself and send it to the parse() method. This method
    // makes a non-blocking implementation easy though.)
    while (bluetooth.checkActivity()) { Serial.print("*"); }
}

#endif // _SETUP_HOSTIF_BT_H_
