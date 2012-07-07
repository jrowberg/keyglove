// Keyglove controller source code - Special hardware setup file
// 7/4/2012 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

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



#ifndef _SUPPORT_HOSTIF_BT2_H_
#define _SUPPORT_HOSTIF_BT2_H_

#define ENABLE_BT2

#if KG_BOARD == KG_BOARD_TEENSYPP2
    HardwareSerial BTSerial = HardwareSerial();
#endif /* USE_TEENSY */

bool interfaceBT2Ready = false;

iWRAP bluetooth = iWRAP(&BTSerial, (HardwareSerial *)&Serial);
bool bluetoothLinkActive = false;

void event_onExitDataMode() {
    send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Exiting data mode"));
    digitalWrite(KG_PIN_BT_DTR, HIGH);
    delayMicroseconds(500);
    digitalWrite(KG_PIN_BT_DTR, LOW);
}
void event_onSelectLink(iWRAPLink *link) {
    send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Selecting link"));
    //Serial.println(link -> link_id);
}

void event_onModuleIdle() {
}

void event_onModuleBusy() {
}

void event_onModuleRing(iWRAPLink *link) {
    bluetoothLinkActive = true;
}

void event_onModuleNoCarrier(iWRAPLink*, uint8_t n) {
    bluetoothLinkActive = false;
}

void setup_hostif_bt2() {
    // initialize pins
    pinMode(KG_PIN_BT_DTR, OUTPUT);
    pinMode(KG_PIN_BT_LINK, INPUT);
    digitalWrite(KG_PIN_BT_DTR, LOW);

    // initialize iWRAP device
    BTSerial.begin(38400);
    
    // assign special busy/idle callbacks because...well...because we can
    bluetooth.onBusy(event_onModuleBusy);
    bluetooth.onIdle(event_onModuleIdle);

    // assign more important RING/NO CARRIER callbacks
    bluetooth.onRing(event_onModuleRing);
    bluetooth.onNoCarrier(event_onModuleNoCarrier);
    
    // assign manual control of exiting data mode (DTR_PIN)
    bluetooth.onExitDataMode(event_onExitDataMode);

    // enable local module -> PC passthrough echo for fun/debugging
    //bluetooth.setEchoModuleOutput(true);

    // check for already active link
    if (digitalRead(KG_PIN_BT_LINK)) {
        send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Device appears to have active data connection!"));
        bluetooth.setDeviceMode(IWRAP_MODE_DATA);
        bluetoothLinkActive = true;
    }

    // make sure we can communicate with iWRAP
    while (BTSerial.available()) BTSerial.read();
    BTSerial.println("XYZ");
    BTSerial.flush();
    uint32_t t0 = millis();
    while (!BTSerial.available() && millis() - t0 < 500);
    if (BTSerial.available()) {
        // got something back...yay! make sure it's "SYNTAX ERROR"
        uint8_t inStr[19];
        uint8_t i;
        i = 0;
        t0 = millis();
        while (i < 19 && millis() - t0 < 500) {
            if (BTSerial.available()) { inStr[i] = BTSerial.read(); i++; }
        }
        while (BTSerial.available()) BTSerial.read();
        if (inStr[0] == 'X' && inStr[1] == 'Y' && inStr[2] == 'Z') {
            send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Bluetooth module responded as expected"));
            interfaceBT2Ready = true;
        } else {
            send_keyglove_info(KG_INFO_LEVEL_CRITICAL, F("Bluetooth response unexpected, trying baud rate fix"));
            BTSerial.end();

            // change clock prescaler to 16MHz (overclocking at 3.3v)
            cli();
            (CLKPR = 0x80, CLKPR = 0x00);
            sei();

            // changed prescaler 2x so we use 57600 for 115200 baud
            BTSerial.begin(57600);
            BTSerial.print("\r\nSET CONTROL BAUD 38400,8n1\r\n");
            BTSerial.flush();
            BTSerial.end();

            // change clock prescaler back to 8MHz (max safe speed)
            cli();
            (CLKPR = 0x80, CLKPR = 0x01);
            sei();

            BTSerial.begin(38400);
            while (BTSerial.available()) BTSerial.read();
            BTSerial.println("XYZ");
            BTSerial.flush();
            t0 = millis();
            while (!BTSerial.available() && millis() - t0 < 500);
            for (i = 0; i < 19; i++) inStr[i] = 0;
            i = 0;
            t0 = millis();
            while (i < 19 && millis() - t0 < 500) {
                if (BTSerial.available()) { inStr[i] = BTSerial.read(); i++; }
            }
            while (BTSerial.available()) BTSerial.read();
            if (inStr[0] == 'X' && inStr[1] == 'Y' && inStr[2] == 'Z') {
                send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Bluetooth module responded as expected, fix worked!"));
                interfaceBT2Ready = true;
            } else {
                send_keyglove_info(KG_INFO_LEVEL_CRITICAL, F("Baud rate fix failed; BT interface not available"));
            }
        }
    } else {
        // nothing returned, so BT is not going to work
        send_keyglove_info(KG_INFO_LEVEL_CRITICAL, F("Bluetooth module did not respond, BT interface not available"));
    }

    if (interfaceBT2Ready) {
        // get all configuration parameters and wait for completion
        send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Reading iWRAP configuration..."));
        bluetooth.readDeviceConfig();
        while (bluetooth.checkActivity(1000));
        if (bluetooth.checkError()) {
            send_keyglove_info(KG_INFO_LEVEL_WARNING, F("iWRAP config read generated a syntax error, trying again"));
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
            send_keyglove_info(KG_INFO_LEVEL_CRITICAL, F("iWRAP config could not be read. Something isn't right."));
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
            send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Reading list link configuration"));
            bluetooth.readLinkConfig();
            while (bluetooth.checkActivity(1000));
            
            // enable HID profile if necessary
            if (!bluetooth.config.profileHIDEnabled) {
                send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Enabling HID profile"));
                bluetooth.setProfile("HID", "iWRAP Demo Device");
                while (bluetooth.checkActivity());
                if (bluetooth.checkError()) {
                    send_keyglove_info(KG_INFO_LEVEL_CRITICAL, F("HID profile could not be enabled"));
                } else {
                    send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Rebooting iWRAP device again to activate profile"));
                    bluetooth.resetDevice();
                    while (bluetooth.checkActivity());
                    send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Re-reading iWRAP configuration"));
                    bluetooth.readDeviceConfig();
                    while (bluetooth.checkActivity());
                }
            } else {
                send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Detected HID profile already enabled"));
            }
    
            // set device name for demo
            send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Setting device name"));
            bluetooth.setDeviceName("Keyglove");
    
            // set device identity for demo
            send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Setting device identity"));
            bluetooth.setDeviceIdentity("Keyglove Input Device");
    
            // set device class to KB/mouse
            send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Setting device class to keyboard/mouse"));
            bluetooth.setDeviceClass(0x05C0);
    
            // set SSP mode to 3 0
            send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Setting Secure Simple Pairing (SSP) mode to 3/0"));
            bluetooth.setDeviceSSP(3, 0);
    
            // enable CD pair notification on GPIO pin 7
            send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Enabling GPIO7 toggling on Carrier Detect (link active mode)"));
            bluetooth.setCarrierDetect(0x80, 0);
    
            // enable DTR escape control on GPIO pin 6
            send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Enabling GPIO6 DTR detection for entering command mode (ESCAPE char disabled)"));
            bluetooth.setControlEscape('-', 0x40, 1);
    
            // output some info to prove we read the config correctly
            /*Serial.print("i,5,BT address=");
            Serial.print(bluetooth.config.btAddress.address[0], HEX); Serial.print(" ");
            Serial.print(bluetooth.config.btAddress.address[1], HEX); Serial.print(" ");
            Serial.print(bluetooth.config.btAddress.address[2], HEX); Serial.print(" ");
            Serial.print(bluetooth.config.btAddress.address[3], HEX); Serial.print(" ");
            Serial.print(bluetooth.config.btAddress.address[4], HEX); Serial.print(" ");
            Serial.println(bluetooth.config.btAddress.address[5], HEX);
            Serial.print("i,5,BT name=");
            Serial.println(bluetooth.config.btName);
            Serial.print("i,5,BT IDENT description=");
            Serial.println(bluetooth.config.btIdentDescription);
            Serial.print("i,5,UART baud rate=");
            Serial.println(bluetooth.config.uartBaudRate);
            Serial.print("i,5,Current pair count=");
            Serial.println(bluetooth.config.btPairCount);*/
            
            if (digitalRead(KG_PIN_BT_LINK)) {
                send_keyglove_info(KG_INFO_LEVEL_NORMAL, F("Switching back to data mode for previously active link"));
                bluetooth.selectDataMode((uint8_t)0); // use default link since we don't know what it was
                while (bluetooth.checkActivity(1000));
            }
        }
    }
}

void update_hostif_bt2() {
    // passthrough from PC to iWRAP for debugging or manual control
    //while (Serial.available()) BTSerial.write(Serial.read());

    // check for module activity
    // (This method automatically watches for data from the module and parses
    // anything that comes in; you must either call this often or else manage
    // reading the data yourself and send it to the parse() method. This method
    // makes a non-blocking implementation easy though.)
    if (interfaceBT2Ready) bluetooth.checkActivity();
}

#endif // _SUPPORT_HOSTIF_BT2_H_
