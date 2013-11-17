// Keyglove controller source code - Special hardware setup file
// 9/9/2013 by Jeff Rowberg <jeff@rowberg.net>

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



#ifndef _SUPPORT_PROTOCOL_H_
#define _SUPPORT_PROTOCOL_H_

#define KG_PROTOCOL_RX_TIMEOUT                  500

#define KG_PACKET_TYPE_EVENT                    0x80
#define KG_PACKET_TYPE_COMMAND                  0xC0

#define KG_PACKET_CLASS_PROTOCOL                0x00
#define KG_PACKET_CLASS_SYSTEM                  0x01
#define KG_PACKET_CLASS_TOUCH                   0x02
#define KG_PACKET_CLASS_FEEDBACK                0x03
#define KG_PACKET_CLASS_MOTION                  0x04
#define KG_PACKET_CLASS_FLEX                    0x05
#define KG_PACKET_CLASS_PRESSURE                0x06
#define KG_PACKET_CLASS_TOUCHSET                0x07

#define KG_LOG_LEVEL_PANIC                      0
#define KG_LOG_LEVEL_CRITICAL                   1
#define KG_LOG_LEVEL_WARNING                    3
#define KG_LOG_LEVEL_NORMAL                     5
#define KG_LOG_LEVEL_VERBOSE                    9

// ------------------------------------------------------------------
// -------- API packets below are built into the core system --------
// ------------------------------------------------------------------

// PROTOCOL PACKETS
#define KG_PACKET_ID_EVT_PROTOCOL_ERROR                 0x01
    #define KG_DEF_PROTOCOL_ERROR_INVALID_COMMAND           0x01
    #define KG_DEF_PROTOCOL_ERROR_PACKET_TIMEOUT            0x02
    #define KG_DEF_PROTOCOL_ERROR_BAD_LENGTH                0x03
    #define KG_DEF_PROTOCOL_ERROR_PARAMETER_LENGTH          0x04
    #define KG_DEF_PROTOCOL_ERROR_NOT_IMPLEMENTED           0x05

// ------------------------------------------------------------------

// SYSTEM PACKETS
#define KG_PACKET_ID_CMD_SYSTEM_PING                    0x01
#define KG_PACKET_ID_CMD_SYSTEM_RESET                   0x02
// -- command/event split --
#define KG_PACKET_ID_EVT_SYSTEM_BOOT                    0x01
#define KG_PACKET_ID_EVT_SYSTEM_READY                   0x02

// ------------------------------------------------------------------

// TOUCH PACKETS
#define KG_PACKET_ID_CMD_TOUCH_GET_MODE                 0x01
#define KG_PACKET_ID_CMD_TOUCH_SET_MODE                 0x02
// -- command/event split --
#define KG_PACKET_ID_EVT_TOUCH_MODE                     0x01
#define KG_PACKET_ID_EVT_TOUCH_STATUS                   0x02

// ------------------------------------------------------------------
// ------- API packets below are optional according to config -------
// ------------------------------------------------------------------

// FEEDBACK PACKETS
#define KG_PACKET_ID_CMD_FEEDBACK_GET_BLINK_MODE        0x01
#define KG_PACKET_ID_CMD_FEEDBACK_SET_BLINK_MODE        0x02
#define KG_PACKET_ID_CMD_FEEDBACK_GET_PIEZO_MODE        0x03
#define KG_PACKET_ID_CMD_FEEDBACK_SET_PIEZO_MODE        0x04
#define KG_PACKET_ID_CMD_FEEDBACK_GET_VIBE_MODE         0x05
#define KG_PACKET_ID_CMD_FEEDBACK_SET_VIBE_MODE         0x06
#define KG_PACKET_ID_CMD_FEEDBACK_GET_RGB_MODE          0x07
#define KG_PACKET_ID_CMD_FEEDBACK_SET_RGB_MODE          0x08
// -- command/event split --
#define KG_PACKET_ID_EVT_FEEDBACK_BLINK_MODE            0x01
#define KG_PACKET_ID_EVT_FEEDBACK_PIEZO_MODE            0x02
#define KG_PACKET_ID_EVT_FEEDBACK_VIBE_MODE             0x03
#define KG_PACKET_ID_EVT_FEEDBACK_RGB_MODE              0x04

// MOTION PACKETS
#define KG_PACKET_ID_CMD_MOTION_GET_MODE                0x01
#define KG_PACKET_ID_CMD_MOTION_SET_MODE                0x02
// -- command/event split --
#define KG_PACKET_ID_EVT_MOTION_MODE                    0x01
#define KG_PACKET_ID_EVT_MOTION_DATA                    0x02

// ------------------------------------------------------------------
// ------------------------------------------------------------------
// ------------------------------------------------------------------

#if KG_HOSTIF & KG_HOSTIF_USB_RAWHID
    #define RAWHID_TX_SIZE 64
    #define RAWHID_RX_SIZE 64
    uint8_t rxRawHIDPacket[RAWHID_RX_SIZE];
    uint8_t txRawHIDPacket[RAWHID_TX_SIZE];
#endif

// custom protocol function prototypes
uint8_t filter_incoming_keyglove_packet(uint8_t *rxPacket);
uint8_t filter_outgoing_keyglove_packet(uint8_t *packetType, uint8_t *payloadLength, uint8_t *packetClass, uint8_t *packetId, uint8_t *payload);
uint8_t process_protocol_command_custom(uint8_t *rxPacket);

uint8_t send_keyglove_packet(uint8_t packetType, uint8_t payloadLength, uint8_t packetClass, uint8_t packetId, uint8_t *payload);

bool interfaceBT2SerialReady = false;
uint8_t interfaceBT2SerialMode = 0;
bool interfaceBT2RawHIDReady = false;
uint8_t interfaceBT2RawHIDMode = 0;
bool interfaceBT2HIDReady = false;
uint8_t interfaceBT2HIDMode = 0;

uint16_t rxByte;            // actually WORD but the high byte is for .read() error checking

uint8_t *rxPacket;             // buffer for incoming data
uint16_t rxPacketSize;      // container size in bytes of received packet buffer
uint16_t rxPacketLength;    // full length in bytes of received packet (may be less than rxPacketSize)
uint32_t packetStartTime; // command packet timeout detection

bool response = false;      // indicates whether we intend to generate a response
bool inBinPacket = false;   // indicates whether we have started a binary packet or not
uint8_t binDataLength;      // expected size of incoming binary data (should be rxPacketLength - 4)

// special string comparison for protocol, match must either be end of string OR followed by space
int strncasecmp2(const char *s1, const char *s2, int len) {
    int r = strncasecmp(s1, s2, len);
    if (r == 0 && s2[len] != 0 && s2[len - 1] != ' ' && s2[len] != ' ') r = 1; // terminate or followed by space
    return r;
}

// pre-declare packet handlers, which must be defined elsewhere if enabled
// (we have to call these functions from this file, but this is compiled before others)

uint8_t process_protocol_command_system(uint8_t *rxPacket);
uint8_t process_protocol_command_touch(uint8_t *rxPacket);
uint8_t process_protocol_command_feedback(uint8_t *rxPacket);
uint8_t process_protocol_command_motion(uint8_t *rxPacket);
uint8_t process_protocol_command_flex(uint8_t *rxPacket);
uint8_t process_protocol_command_pressure(uint8_t *rxPacket);
uint8_t process_protocol_command_touchset(uint8_t *rxPacket);

void setup_protocol() {
    rxPacketSize = 32;
    rxPacket = (uint8_t *)malloc(rxPacketSize);
    rxPacketLength = 0;
}

void protocol_parse(uint8_t inputByte) {
    if (rxPacketLength + 1 == rxPacketSize) {
        rxPacketSize += 32;
        rxPacket = (uint8_t *)realloc(rxPacket, rxPacketSize);
    }
    if (!inBinPacket && inputByte == KG_PACKET_TYPE_COMMAND) { // "bait" byte, 0xC0
        packetStartTime = millis();
        inBinPacket = true;
        rxPacket[0] = inputByte;
        rxPacketLength = 1; // initialize buffer length to include only 1st header byte (so far)
        binDataLength = 0;
    } else if (inBinPacket && rxPacketLength == 1) { // data length byte
        binDataLength = inputByte;
        if (binDataLength > 250) {
            // error (data payload too long)
            uint8_t payload[2] = { KG_DEF_PROTOCOL_ERROR_BAD_LENGTH, 0x00 };
            send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
            inBinPacket = false;
            rxPacketLength = 0;
        } else {
            rxPacket[rxPacketLength++] = inputByte;
        }
    } else {
        rxPacket[rxPacketLength++] = inputByte;
        if (inBinPacket && rxPacketLength - 4 == binDataLength) {
            // process packet that just came in, passing to appropriate main handler
            uint8_t protocol_error = 0;

            // filter incoming packets for custom behavior
            if (filter_incoming_keyglove_packet(rxPacket) == 0) {
                switch (rxPacket[2]) {
                    //case KG_PACKET_CLASS_PROTOCOL:
    
                    case KG_PACKET_CLASS_SYSTEM:
                        protocol_error = process_protocol_command_system(rxPacket);
                        break;
    
                    case KG_PACKET_CLASS_TOUCH:
                        protocol_error = process_protocol_command_touch(rxPacket);
                        break;
    
                    #if KG_MOTION > 0
                        case KG_PACKET_CLASS_MOTION:
                            protocol_error = process_protocol_command_motion(rxPacket);
                            break;
                    #endif
    
                    #if KG_FEEDBACK > 0
                        case KG_PACKET_CLASS_FEEDBACK:
                            protocol_error = process_protocol_command_feedback(rxPacket);
                            break;
                    #endif
    
                    #if KG_FLEX > 0
                        case KG_PACKET_CLASS_FLEX:
                            protocol_error = process_protocol_command_flex(rxPacket);
                            break;
                    #endif
    
                    #if KG_PRESSURE > 0
                        case KG_PACKET_CLASS_PRESSURE:
                            protocol_error = process_protocol_command_pressure(rxPacket);
                            break;
                    #endif
    
                    #if KG_TOUCHSET > 0
                        case KG_PACKET_CLASS_TOUCHSET:
                            protocol_error = process_protocol_command_touchset(rxPacket);
                            break;
                    #endif
    
                    default:
                        // check for custom protocol, will return KG_DEF_PROTOCOL_ERROR_INVALID_COMMAND if no matches
                        protocol_error = KG_DEF_PROTOCOL_ERROR_INVALID_COMMAND;
                        break;
                }

                // check for errors (e.g. unhandled, bad arguments, etc.)
                if (protocol_error) {
                    // check for custom protocol, will return KG_DEF_PROTOCOL_ERROR_INVALID_COMMAND if no matches
                    protocol_error = process_protocol_command_custom(rxPacket);

                    // if we still have an error, then there is no custom functionality implemented for this class/ID combination
                    if (protocol_error) {
                        uint8_t payload[2] = { protocol_error, 0x00 };
                        send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
                    }
                }
            }

            // reset packet status/length
            inBinPacket = false;
            rxPacketLength = 0;
        }
    }
}

uint8_t check_incoming_protocol_data() {
    #if KG_HOSTIF & KG_HOSTIF_USB_SERIAL
        // read available data from USB virtual serial
        if (interfaceUSBSerialReady && (interfaceUSBSerialMode & KG_INTERFACE_MODE_INCOMING_PACKET) != 0) {
            while ((rxByte = USBSerial.read()) < 256) protocol_parse((uint8_t)rxByte);
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_USB_RAWHID
        // read raw HID data over USB
        if (interfaceUSBRawHIDReady && (interfaceUSBRawHIDMode & KG_INTERFACE_MODE_INCOMING_PACKET) != 0) {
            int8_t bytes = RawHID.recv(rxRawHIDPacket, 0);
            if (bytes > 0) {
                for (int8_t i = 0; i < rxRawHIDPacket[0]; i++) protocol_parse(rxRawHIDPacket[i + 1]);
            }
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_BT2_SERIAL
        // smart read data from Bluetooth v2.1 SPP
        if (interfaceBT2SerialReady && (interfaceBT2SerialMode & KG_INTERFACE_MODE_INCOMING_PACKET) != 0 && bluetoothConnectionMap[bluetoothSPPDeviceIndex] -> linkSPP != 0xFF) {
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_BT2_IAP
        // smart read data from Bluetooth v2.1 IAP
        if (interfaceBT2IAPReady && (interfaceBT2IAPMode & KG_INTERFACE_MODE_INCOMING_PACKET) != 0 && bluetoothConnectionMap[bluetoothIAPDeviceIndex] -> linkIAP != 0xFF) {
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_BT2_RAWHID
        // smart read data from Bluetooth v2.1 custom HID
        if (interfaceBT2RawHIDReady && (interfaceBT2RawHIDMode & KG_INTERFACE_MODE_INCOMING_PACKET) != 0 && bluetoothConnectionMap[bluetoothRawHIDDeviceIndex] -> linkHIDInterrupt != 0xFF) {
        }
    #endif

    // check for protocol timeout condition
    if (inBinPacket && (millis() - packetStartTime) > KG_PROTOCOL_RX_TIMEOUT) {
        // error (data payload too long)
        uint8_t payload[2] = { KG_DEF_PROTOCOL_ERROR_PACKET_TIMEOUT, 0x00 };
        send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
        inBinPacket = false;
        rxPacketLength = 0;
    }

    return 0;
}

uint8_t send_keyglove_packet(uint8_t packetType, uint8_t payloadLength, uint8_t packetClass, uint8_t packetId, uint8_t *payload) {
    // validate payload length
    if ((payload == NULL && payloadLength > 0) || payloadLength > 250) {
        // payload specified but not provided, or too long
        return 1;
    }
    
    // filter outgoing packets for custom behavior
    if (filter_outgoing_keyglove_packet(&packetType, &payloadLength, &packetClass, &packetId, payload)) return 255;

    // allocate and check full packet buffer
    uint8_t *buffer = (uint8_t *)malloc(4 + payloadLength);
    if (buffer == 0) {
        // couldn't allocate packet buffer...uh oh
        return 2;
    }
    
    buffer[0] = packetType;
    buffer[1] = payloadLength;
    buffer[2] = packetClass;
    buffer[3] = packetId;
    if (payloadLength) memcpy(buffer + 4, payload, payloadLength);
    uint8_t length = 4 + payloadLength;

    #if KG_HOSTIF & KG_HOSTIF_USB_SERIAL
        // send packet out over wired serial (USB virtual serial)
        if (interfaceUSBSerialReady && (interfaceUSBSerialMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0) {
            USBSerial.write((const uint8_t *)buffer, length); // packet data
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_USB_RAWHID
        // send packet out over wired custom HID interface (USB raw HID)
        // 64-byte packets, formatted where byte 0 is [0-64] and bytes 1-63 are data
        if (interfaceUSBRawHIDReady && (interfaceUSBRawHIDMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0) {
            int8_t bytes;
            for (uint8_t i = 0; i < length; i += (RAWHID_TX_SIZE - 1)) {
                memset(txRawHIDPacket, 0, RAWHID_TX_SIZE);
                txRawHIDPacket[0] = min(RAWHID_TX_SIZE - 1, length - i);
                for (uint8_t j = 0; j < (RAWHID_TX_SIZE - 1); j++) {
                    if (i + j >= length) break;
                    txRawHIDPacket[j + 1] = buffer[i + j];
                }
                bytes = RawHID.send(txRawHIDPacket, 2);
            }
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_BT2_SERIAL
        // send packet out over wireless serial (Bluetooth v2.1 SPP)
        if (interfaceBT2SerialReady && (interfaceBT2SerialMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0 && bluetoothConnectionMap[bluetoothSPPDeviceIndex] -> linkSPP != 0xFF) {
            bluetooth.smartSendData((const uint8_t *)bluetoothTXRawHIDPacket, length, bluetoothConnectionMap[bluetoothSPPDeviceIndex] -> linkSPP);
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_BT2_IAP
        // send packet out over wireless iAP link (Bluetooth v2.1 IAP)
        if (interfaceBT2IAPReady && (interfaceBT2IAPMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0 && bluetoothConnectionMap[bluetoothIAPDeviceIndex] -> linkIAP != 0xFF) {
            bluetooth.smartSendData((const uint8_t *)bluetoothTXRawHIDPacket, length, bluetoothConnectionMap[bluetoothIAPDeviceIndex] -> linkIAP);
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_BT2_RAWHID
        // send packet out over wireless custom HID interface (Bluetooth v2.1 raw HID)
        if (interfaceBT2RawHIDReady && (interfaceBT2RawHIDMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0 && bluetoothConnectionMap[bluetoothRawHIDDeviceIndex] -> linkHIDInterrupt != 0xFF) {
            int8_t bytes;
            for (uint8_t i = 0; i < length; i += (BLUETOOTH_RAWHID_TX_SIZE - 1)) {
                memset(bluetoothTXRawHIDPacket + 4, 0, BLUETOOTH_RAWHID_TX_SIZE);
                bluetoothTXRawHIDPacket[4] = min(BLUETOOTH_RAWHID_TX_SIZE - 1, length - i);
                for (uint8_t j = 0; j < (BLUETOOTH_RAWHID_TX_SIZE - 1); j++) {
                    if (i + j >= length) break;
                    bluetoothTXRawHIDPacket[j + 5] = buffer[i + j];
                }
                bluetooth.smartSendData((const uint8_t *)bluetoothTXRawHIDPacket, BLUETOOTH_RAWHID_TX_SIZE + 4, bluetoothConnectionMap[bluetoothRawHIDDeviceIndex] -> linkHIDInterrupt);
            }
        }
    #endif

    // KG_HID_KEYBOARD and KG_HID_MOUSE are handled elsewhere and deal with other kinds of data
    
    // tidy up!
    free(buffer);

    return 0;
}

#endif // _SUPPORT_PROTOCOL_H_