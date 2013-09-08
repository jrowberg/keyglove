// Keyglove controller source code - Special hardware setup file
// 8/3/2013 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2013 Jeff Rowberg

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

/*

1. Update WT12 firmware to iWRAP5 build
2. Reset to default settings ("SET RESET")
3. Clear existing pairings ("SET BT PAIR *")
4. Apply the following settings

SET BT NAME Keyglove
SET BT CLASS 00540
SET BT IDENT USB:1d50 6025 1.0.0 Keyglove Input Device
SET BT SSP 3 0
SET CONTROL CD 80 2 20
SET CONTROL ESCAPE - 40 1
SET PROFILE HID 1f 40 100 0 en 0409 Keyglove Input Device
HID SET F2 05010906A1010507850119E029E715002501750195088102950175088101950575010508850119012905910295017503910395067508150025650507190029658100C0050C0901A1018502050C1500250109E909EA09E209CD19B529B87501950881020A8A010A21020A2A021A23022A27027501950881020A83010A96010A92010A9E010A94010A060209B209B4750195088102C005010902A1010901A10085030509190129031500250195037501810295017505810305010930093109381581257F750895038106050C0A380295018106C0C006ABFF0A0002A10185047508150026FF00951009018102951009029102C0
SET CONTROL CONFIG 0080 1080
SET CONTROL ECHO 5
SET CONTROL BAUD 62500,8n1

5. Change to 62500 baud and apply this setting:

SET CONTROL MUX 1

6. Make SURE you power-cycle or reset the module after this point so that all settings take effect.

================================================================

Bluetooth Functionality:

- Defaults to discoverable/connectable if no pairings
- Defaults to undiscoverable/connectable if 1+ pairings
- Automatically connect to first paired device on startup
- Automatically connect to next paired device if desired and available
- Switch between which connection is primary
- Special combinations go only to matching (full/partial) MAC addresses

================================================================

*/

#define ENABLE_BT2

// keep track of connection attempts/status so we don't double-up on anything
#define BLUETOOTH_CONNECTION_STATE_DISCONNECTED 0
#define BLUETOOTH_CONNECTION_STATE_CALLING      1
#define BLUETOOTH_CONNECTION_STATE_CONNECTED    2

// track certain commands that take a while to complete, to remain non-blocking
#define BLUETOOTH_IWRAP_PENDING_NONE            0
#define BLUETOOTH_IWRAP_PENDING_FIRST_SET       1
#define BLUETOOTH_IWRAP_PENDING_FIRST_LINK_LIST 2
#define BLUETOOTH_IWRAP_PENDING_PAIR_LIST       3
#define BLUETOOTH_IWRAP_PENDING_LINK_LIST       4

bool interfaceBT2Ready = false;

iWRAP bluetooth = iWRAP(&BTSerial, (HardwareSerial *)&Serial);

uint8_t bluetoothConnectionState = BLUETOOTH_CONNECTION_STATE_DISCONNECTED;
uint8_t bluetoothIWRAPState = BLUETOOTH_IWRAP_PENDING_NONE;
uint8_t bluetoothPendingLinkCount = 0;
uint8_t bluetoothAutocallIndex = 0;
uint32_t bluetoothTock = 0;

// map for Bluetooth connection tracking (16 pairings max):
class bluetoothConnection {
    public:
        iWRAPAddress address;
        uint8_t flags;
        uint8_t priority;
        uint8_t linkHIDControl;
        uint8_t linkHIDInterrupt;
        uint8_t linkSPP;
        uint8_t linkIAP;
};
bluetoothConnection *bluetoothConnectionMap[16];

uint8_t bluetoothSPPDeviceIndex = 0;
uint8_t bluetoothIAPDeviceIndex = 0;
uint8_t bluetoothHIDDeviceIndex = 0;
uint8_t bluetoothRawHIDDeviceIndex = 0;

bool interfaceBT2SerialReady = false;
uint8_t interfaceBT2SerialMode = KG_INTERFACE_MODE_OUTGOING_PACKET | KG_INTERFACE_MODE_INCOMING_PACKET;

bool interfaceBT2IAPReady = false;
uint8_t interfaceBT2IAPMode = KG_INTERFACE_MODE_OUTGOING_PACKET | KG_INTERFACE_MODE_INCOMING_PACKET;

bool interfaceBT2HIDReady = false;
uint8_t interfaceBT2HIDMode = KG_INTERFACE_MODE_OUTGOING_PACKET | KG_INTERFACE_MODE_INCOMING_PACKET;

bool interfaceBT2RawHIDReady = false;
uint8_t interfaceBT2RawHIDMode = KG_INTERFACE_MODE_OUTGOING_PACKET | KG_INTERFACE_MODE_INCOMING_PACKET;

void rebuild_bluetooth_connection_map(uint8_t linkOnly) {
    // reset host interface ready states
    interfaceBT2SerialReady = false;
    interfaceBT2IAPReady = false;
    interfaceBT2HIDReady = false;
    interfaceBT2RawHIDReady = false;

    // build Bluetooth connection map
    for (uint8_t i = 0; i < bluetooth.config.btPairCount; i++) {
        if (linkOnly == 0) {
            // free existing entry if necessary (extra careful here to avoid memory leaks)
            if (bluetoothConnectionMap[i] != 0) free(bluetoothConnectionMap[i]);

            // create new pairing map object
            bluetoothConnectionMap[i] = (bluetoothConnection *)malloc(sizeof(bluetoothConnection));

            // fill it with the correct pairing info
            memcpy(bluetoothConnectionMap[i] -> address.address, bluetooth.config.btPair[i] -> address.address, 6);
            bluetoothConnectionMap[i] -> priority = i; // TODO: add connection ordering
            /*
            char *mac = (char *)malloc(18);
            mac[17] = 0; // null terminate
            bluetoothConnectionMap[i] -> address.buildHexStringFromAddress(mac);
            Serial.print("Added pairing to map: MAC=");
            Serial.print(mac);
            free(mac);
            Serial.print(", priority=");
            Serial.println(bluetoothConnectionMap[i] -> priority);
            */
        }

        // failsafe in case pairing hasn't been defined (wtf?)
        if (bluetoothConnectionMap[i] == 0) continue;

        // reset flags and connection IDs for this mac address
        bluetoothConnectionMap[i] -> flags = 0x00;
        bluetoothConnectionMap[i] -> linkHIDControl = 0xff; // "0" is a valid link ID, so "none" is 0xFF
        bluetoothConnectionMap[i] -> linkHIDInterrupt = 0xff;
        bluetoothConnectionMap[i] -> linkSPP = 0xff;
        bluetoothConnectionMap[i] -> linkIAP = 0xff;

        // map any open links properly
        for (uint8_t j = 0; j < bluetooth.config.btLinkCount; j++) {
            if (memcmp(bluetooth.config.btLink[j] -> address.address, bluetoothConnectionMap[i] -> address.address, 6) == 0) {
                if (bluetooth.config.btLink[j] -> channel == 11) { // HID control link
                    bluetoothConnectionMap[i] -> linkHIDControl = j;
                    bluetoothConnectionMap[i] -> flags |= 0x01;
                } else if (bluetooth.config.btLink[j] -> channel == 13) { // HID interrupt link
                    bluetoothConnectionMap[i] -> linkHIDInterrupt = j;
                    bluetoothConnectionMap[i] -> flags |= 0x02;
                    bluetoothHIDDeviceIndex = i;
                    bluetoothRawHIDDeviceIndex = i;
                    interfaceBT2HIDReady = true;
                    interfaceBT2RawHIDReady = true;
                } else if (strncmp(bluetooth.config.btLink[j] -> profile, "RF", 2) == 0) { // SPP (RFCOMM)
                    bluetoothConnectionMap[i] -> linkSPP = j;
                    bluetoothConnectionMap[i] -> flags |= 0x04;
                    bluetoothSPPDeviceIndex = i;
                    interfaceBT2SerialReady = true;
                } else if (strncmp(bluetooth.config.btLink[j] -> profile, "IA", 2) == 0) { // IAP
                    bluetoothConnectionMap[i] -> linkIAP = j;
                    bluetoothConnectionMap[i] -> flags |= 0x08;
                    bluetoothIAPDeviceIndex = i;
                    interfaceBT2IAPReady = true;
                }
            }
        }
        /*
        Serial.print("Pairing #"); Serial.print(i);
        Serial.print(": flags="); Serial.print(bluetoothConnectionMap[i] -> flags, HEX);
        Serial.print(", HIDC="); Serial.print(bluetoothConnectionMap[i] -> linkHIDControl);
        Serial.print(", HIDI="); Serial.print(bluetoothConnectionMap[i] -> linkHIDInterrupt);
        Serial.print(", SPP="); Serial.print(bluetoothConnectionMap[i] -> linkSPP);
        Serial.print(", IAP="); Serial.println(bluetoothConnectionMap[i] -> linkIAP);
        */
    }
    
    // free up remaining entries if they were previously defined
    for (uint8_t i = bluetooth.config.btPairCount; i < 16; i++) {
        if (bluetoothConnectionMap[i] != 0) free(bluetoothConnectionMap[i]);
    }

    // wrap up with correct state
    if (bluetooth.config.btLinkCount == 0) {
        bluetoothConnectionState = BLUETOOTH_CONNECTION_STATE_DISCONNECTED;

        // start sending control packets out USB if enabled AND if we have lost all BT connections
        #if (KG_HOSTIF & KG_HOSTIF_USB_SERIAL)
            interfaceUSBSerialMode |= KG_INTERFACE_MODE_OUTGOING_PACKET;
        #endif
        #if (KG_HOSTIF & KG_HOSTIF_USB_RAWHID)
            interfaceUSBRawHIDMode |= KG_INTERFACE_MODE_OUTGOING_PACKET;
        #endif
        #if (KG_HOSTIF & KG_HOSTIF_USB_HID)
            interfaceUSBHIDMode |= KG_INTERFACE_MODE_OUTGOING_PACKET;
        #endif
    } else {
        // stop sending control packets out USB serial if enabled AND if we have a BT connection
        #if (KG_HOSTIF & KG_HOSTIF_USB_SERIAL)
            interfaceUSBSerialMode &= ~(KG_INTERFACE_MODE_OUTGOING_PACKET);
        #endif
        #if (KG_HOSTIF & KG_HOSTIF_USB_RAWHID)
            interfaceUSBRawHIDMode &= ~(KG_INTERFACE_MODE_OUTGOING_PACKET);
        #endif
        #if (KG_HOSTIF & KG_HOSTIF_USB_HID)
            interfaceUSBHIDMode &= ~(KG_INTERFACE_MODE_OUTGOING_PACKET);
        #endif
    }
}

void event_onModuleIdle() {
    if (!bluetooth.checkError() && !bluetooth.checkTimeout()) {
        switch (bluetoothIWRAPState) {
            case BLUETOOTH_IWRAP_PENDING_FIRST_SET:
                // rename the device if it hasn't been configured yet
                if (strcmp(bluetooth.config.btName, "Keyglove") == 0) {
                    // rename device to include BT MAC address for unique name
                    char *newName = (char *)malloc(30);
                    strcpy(newName, "SET BT NAME Keyglove 000000");
                    newName[21] = (bluetooth.config.btAddress.address[3] / 0x10) + 48 + ((bluetooth.config.btAddress.address[3] / 0x10) / 10 * 7);
                    newName[22] = (bluetooth.config.btAddress.address[3] & 0x0f) + 48 + ((bluetooth.config.btAddress.address[3] & 0x0f) / 10 * 7);
                    newName[23] = (bluetooth.config.btAddress.address[4] / 0x10) + 48 + ((bluetooth.config.btAddress.address[4] / 0x10) / 10 * 7);
                    newName[24] = (bluetooth.config.btAddress.address[4] & 0x0f) + 48 + ((bluetooth.config.btAddress.address[4] & 0x0f) / 10 * 7);
                    newName[25] = (bluetooth.config.btAddress.address[5] / 0x10) + 48 + ((bluetooth.config.btAddress.address[5] / 0x10) / 10 * 7);
                    newName[26] = (bluetooth.config.btAddress.address[5] & 0x0f) + 48 + ((bluetooth.config.btAddress.address[5] & 0x0f) / 10 * 7);
                    bluetooth.smartSendCommand(newName);
                    free(newName);
                }

                send_keyglove_info(KG_LOG_LEVEL_NORMAL, F("Reading list link configuration"));
                bluetoothIWRAPState = BLUETOOTH_IWRAP_PENDING_FIRST_LINK_LIST;
                bluetooth.readLinkConfig();
                break;
            case BLUETOOTH_IWRAP_PENDING_FIRST_LINK_LIST:
                // set correct state and update PAGEMODE setting (discoverable/connectable) if necessary
                if (bluetooth.config.btLinkCount > 0) {
                    // connected (by paired by definition), so default to non-discoverable/connectable
                    if (bluetooth.config.btPageMode != 2) bluetooth.smartSendCommand("SET BT PAGE 2");
                    bluetoothConnectionState = BLUETOOTH_CONNECTION_STATE_CONNECTED;
                } else if (bluetooth.config.btPairCount > 0) {
                    // not connected, but paired, so default to non-discoverable/connectable
                    if (bluetooth.config.btPageMode != 2) bluetooth.smartSendCommand("SET BT PAGE 2");
                } else {
                    // not connected or paired, so default to discoverable/connectable
                    if (bluetooth.config.btPageMode != 4) bluetooth.smartSendCommand("SET BT PAGE 4");
                }
            case BLUETOOTH_IWRAP_PENDING_PAIR_LIST:
                bluetoothIWRAPState = BLUETOOTH_IWRAP_PENDING_NONE;
                send_keyglove_info(KG_LOG_LEVEL_NORMAL, F("Building full connection map"));
                rebuild_bluetooth_connection_map(0);    // pairings + links
                break;
            case BLUETOOTH_IWRAP_PENDING_LINK_LIST:
                bluetoothIWRAPState = BLUETOOTH_IWRAP_PENDING_NONE;
                send_keyglove_info(KG_LOG_LEVEL_NORMAL, F("Building link-only connection map"));
                rebuild_bluetooth_connection_map(1);    // links only, no pairings
                break;
            default:
                bluetoothIWRAPState = BLUETOOTH_IWRAP_PENDING_NONE;
                break;
        }
    } else {
        // error, oops
        bluetoothIWRAPState = BLUETOOTH_IWRAP_PENDING_NONE;
    }
}

/*
void event_onModuleBusy() {
}
*/

void event_onModuleRing(iWRAPLink *link) {
    bluetoothConnectionState = BLUETOOTH_CONNECTION_STATE_CONNECTED;
    bluetoothIWRAPState = BLUETOOTH_IWRAP_PENDING_LINK_LIST;
    bluetooth.readLinkConfig(); // new link to parse
}

void event_onModuleConnect() {
    if (bluetoothPendingLinkCount > 0) bluetoothPendingLinkCount--;
    bluetoothConnectionState = BLUETOOTH_CONNECTION_STATE_CONNECTED;
    bluetoothIWRAPState = BLUETOOTH_IWRAP_PENDING_LINK_LIST;
    bluetooth.readLinkConfig(); // new link to parse
}

void event_onModuleCall(uint8_t link_id) {
    bluetoothPendingLinkCount++;
    bluetoothConnectionState = BLUETOOTH_CONNECTION_STATE_CALLING;
}

void event_onModulePair() {
    bluetoothIWRAPState = BLUETOOTH_IWRAP_PENDING_PAIR_LIST;
    bluetooth.readPairConfig(); // new pairing to parse
}

void event_onModuleNoCarrier(iWRAPLink *link, uint16_t n) {
    switch (n) {
        case 9801:  // L2CAP_CONNECTION_FAILED (tried CALLing but couldn't connect)
            bluetoothConnectionState = bluetooth.config.btLinkCount > 0 ? BLUETOOTH_CONNECTION_STATE_CONNECTED : BLUETOOTH_CONNECTION_STATE_DISCONNECTED;
            if (bluetoothPendingLinkCount > 0) bluetoothPendingLinkCount--;
            break;
        default:
            // disconnected for some other reason
            bluetoothIWRAPState = BLUETOOTH_IWRAP_PENDING_LINK_LIST;
            bluetooth.readLinkConfig(); // new link to parse
    }
}

void setup_hostif_bt2() {
    // initialize pins
    //pinMode(KG_PIN_BT_DTR, OUTPUT);
    //pinMode(KG_PIN_BT_LINK, INPUT);
    //digitalWrite(KG_PIN_BT_DTR, LOW);

    // initialize iWRAP device
    BTSerial.begin(115200);
    #if KG_BOARD == KG_BOARD_ARDUINO_DUE
        // disable USART1
        US1_CR = 0;

        // enable RTS/CTS handshaking (flow control) on UART1 (Serial2 in Arduino)
        // uses digital pins 23 and 24
        US1_MR = US_MR_USART_MODE_NORMAL | US_MR_USART_MODE_HW_HANDSHAKING | US_MR_USCLKS_MCK |
                 US_MR_CHRL_8_BIT | US_MR_PAR_NO | US_MR_NBSTOP_1_BIT | US_MR_CHMODE_NORMAL;
                 
        // enable USART1
        US1_CR = US_CR_RXEN | US_CR_TXEN;
    #endif

    // make sure we're intentionally using MUX mode
    bluetooth.setDeviceMode(IWRAP_MODE_MUX);
    bluetooth.setDebugOutput(true);

    // assign special busy/idle callbacks because...well...because we can
    //bluetooth.onBusy(event_onModuleBusy);
    bluetooth.onIdle(event_onModuleIdle);

    // assign more important RING/NO CARRIER callbacks
    bluetooth.onRing(event_onModuleRing);
    bluetooth.onCall(event_onModuleCall);
    bluetooth.onConnect(event_onModuleConnect);
    bluetooth.onNoCarrier(event_onModuleNoCarrier);

    //bluetooth.smartSendCommand("SET CONTROL BAUD 115200,8n1");

    // assign manual control of exiting data mode (DTR_PIN)
    //bluetooth.onExitDataMode(event_onExitDataMode);

    // enable local module -> PC passthrough echo for fun/debugging
    //bluetooth.setEchoModuleOutput(true);

    // check for already active link
    //if (digitalRead(KG_PIN_BT_LINK)) {
    //    send_keyglove_info(KG_LOG_LEVEL_NORMAL, F("Device appears to have active BT connection!"));
    //}

    // make sure we can communicate with iWRAP
    while (BTSerial.available()) BTSerial.read();
    bluetooth.smartSendCommand("XYZ", 0);
    while (bluetooth.checkActivity(1000));
    if (bluetooth.checkError() && !bluetooth.checkTimeout()) {
        // no timeout, syntax error (exactly what should happen)
        interfaceBT2Ready = true;
        send_keyglove_info(KG_LOG_LEVEL_NORMAL, F("Bluetooth module responded as expected"));
    } else {
        // some other condition, this is not supposed to happen
        send_keyglove_info(KG_LOG_LEVEL_CRITICAL, F("Bluetooth module did not respond, BT interface not available"));
    }

    if (interfaceBT2Ready) {
        // get all configuration parameters and wait for completion
        send_keyglove_info(KG_LOG_LEVEL_NORMAL, F("Reading iWRAP configuration..."));
        bluetoothIWRAPState = BLUETOOTH_IWRAP_PENDING_FIRST_SET;
        bluetooth.readDeviceConfig();
        // ...states detected and further steps taken in the onIdle() callback (non-blocking implementation)
    }
}

void update_hostif_bt2() {
    // passthrough from PC to iWRAP for debugging or manual control
    //while (Serial.available()) BTSerial.write(Serial.read());

    if (interfaceBT2Ready) {
        // autocall algorithm: round robin through paired devices if not connected and not already calling
        if (bluetooth.config.btLinkCount == 0 &&
            bluetooth.config.btPairCount > 0 &&
            bluetoothConnectionState != BLUETOOTH_CONNECTION_STATE_CALLING &&
            bluetoothIWRAPState == BLUETOOTH_IWRAP_PENDING_NONE) {

            // only try autocall every 10 seconds
            if (keygloveTock - bluetoothTock >= 10 || keygloveTock < 2) {
                bluetoothConnectionState = BLUETOOTH_CONNECTION_STATE_CALLING;
                char *callCmd = (char *)malloc(30);
                strcpy(callCmd, "CALL 00:00:00:00:00:00 11 HID");
                bluetooth.config.btPair[bluetoothAutocallIndex] -> address.buildHexStringFromAddress(callCmd + 5);
                bluetooth.smartSendCommand(callCmd);
                free(callCmd);
                bluetoothAutocallIndex = (bluetoothAutocallIndex + 1) % bluetooth.config.btPairCount;
                bluetoothTock = keygloveTock;
            }
        } else if (bluetoothConnectionState == BLUETOOTH_CONNECTION_STATE_CALLING && keygloveTock - bluetoothTock >= 10) {
            // fix in case we get into an odd state internally
            bluetoothConnectionState = bluetooth.config.btLinkCount > 0 ? BLUETOOTH_CONNECTION_STATE_CONNECTED : BLUETOOTH_CONNECTION_STATE_DISCONNECTED;
        /*} else {
            // only try autocall every 10 seconds
            if (keygloveTock - bluetoothTock >= 10) {
                Serial.print("btLinkCount="); Serial.println(bluetooth.config.btLinkCount);
                Serial.print("btPairCount="); Serial.println(bluetooth.config.btPairCount);
                Serial.print("connState="); Serial.println(bluetoothConnectionState);
                Serial.print("iWRAPState="); Serial.println(bluetoothIWRAPState);
                bluetoothTock = keygloveTock;
            }*/
        }

        // check for module activity
        // (This method automatically watches for data from the module and parses
        // anything that comes in; you must either call this often or else manage
        // reading the data yourself and send it to the parse() method. This method
        // makes a non-blocking implementation easy though.)
        bluetooth.checkActivity();
    }
}

#endif // _SUPPORT_HOSTIF_BT2_H_
