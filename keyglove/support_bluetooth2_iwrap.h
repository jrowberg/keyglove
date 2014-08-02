// Keyglove controller source code - Special hardware setup file
// 8/2/2014 by Jeff Rowberg <jeff@rowberg.net>

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
 * @file support_bluetooth2_iwrap.h
 * @brief Support for Bluetooth using Bluegiga's WT12 module and iWRAP
 * @author Jeff Rowberg
 * @date 2014-08-02
 *
 * This file provides an interface between KGAPI and the full-featured iWRAP
 * firmware that runs on Bluegiga's WT12 class 2 Bluetooth module. This code
 * relies on a separate iWRAP parser library for the Arduino platform, and
 * correlates various iWRAP commands and events into API methods which are
 * more specifically useful in the Keyglove environment.
 *
 * Normally it is not necessary to edit this file.

Manual instructions for WT12 setup:

1. Update WT12 firmware to iWRAP5 build 800 or later
2. Reset to default settings ("SET RESET")
3. Clear existing pairings ("SET BT PAIR *")
4. Apply the following settings

```
SET BT NAME Keyglove
SET BT CLASS 00540
SET BT IDENT USB:1d50 6025 1.0.0 Keyglove Input Device
SET BT SSP 3 0
SET CONTROL CD 80 2 20
SET CONTROL ESCAPE - 40 1
SET PROFILE HID 1f 40 100 0 en 0409 Keyglove Input Device
HID SET F2 05010906A1010507850119E029E715002501750195088102950175088101950575010508850119012905910295017503910395067508150025650507190029658100C0050C0901A1018502050C1500250109E909EA09E209CD19B529B87501950881020A8A010A21020A2A021A23022A27027501950881020A83010A96010A92010A9E010A94010A060209B209B4750195088102C005010902A1010901A10085030509190129031500250195037501810295017505810305010930093109381581257F750895038106050C0A380295018106C0C006ABFF0A0002A10185047508150026FF00951009018102951009029102C0
SET CONTROL CONFIG 3400 0048 50A1
SET CONTROL ECHO 5
SET CONTROL BAUD 125000,8n1
```

5. Change to 125000 baud and apply this setting:

```
SET CONTROL MUX 1
```

6. Make SURE you power-cycle or reset the module after this point so that all settings take effect.

================================================================

Basic Bluetooth Functionality:

- Defaults to discoverable/connectable if no pairings
- Defaults to undiscoverable/connectable if 1+ pairings
- Automatically connect to first paired device on startup

================================================================

 */

#ifndef _SUPPORT_BLUETOOTH2_IWRAP_H_
#define _SUPPORT_BLUETOOTH2_IWRAP_H_

/* =========================== */
/* KGAPI CONSTANT DECLARATIONS */
/* =========================== */

#define KG_PACKET_ID_CMD_BLUETOOTH_GET_MODE                 0x01
#define KG_PACKET_ID_CMD_BLUETOOTH_SET_MODE                 0x02
#define KG_PACKET_ID_CMD_BLUETOOTH_RESET                    0x03
#define KG_PACKET_ID_CMD_BLUETOOTH_GET_MAC                  0x04
#define KG_PACKET_ID_CMD_BLUETOOTH_GET_PAIRINGS             0x05
#define KG_PACKET_ID_CMD_BLUETOOTH_DISCOVER                 0x06
#define KG_PACKET_ID_CMD_BLUETOOTH_PAIR                     0x07
#define KG_PACKET_ID_CMD_BLUETOOTH_DELETE_PAIRING           0x08
#define KG_PACKET_ID_CMD_BLUETOOTH_CLEAR_PAIRINGS           0x09
#define KG_PACKET_ID_CMD_BLUETOOTH_GET_CONNECTIONS          0x0A
#define KG_PACKET_ID_CMD_BLUETOOTH_CONNECT                  0x0B
#define KG_PACKET_ID_CMD_BLUETOOTH_DISCONNECT               0x0C
// -- command/event split --
#define KG_PACKET_ID_EVT_BLUETOOTH_MODE                     0x01
#define KG_PACKET_ID_EVT_BLUETOOTH_READY                    0x02
#define KG_PACKET_ID_EVT_BLUETOOTH_INQUIRY_RESPONSE         0x03
#define KG_PACKET_ID_EVT_BLUETOOTH_INQUIRY_COMPLETE         0x04
#define KG_PACKET_ID_EVT_BLUETOOTH_PAIRING_STATUS           0x05
#define KG_PACKET_ID_EVT_BLUETOOTH_PAIRING_FAILED           0x06
#define KG_PACKET_ID_EVT_BLUETOOTH_PAIRINGS_CLEARED         0x07
#define KG_PACKET_ID_EVT_BLUETOOTH_CONNECTION_STATUS        0x08
#define KG_PACKET_ID_EVT_BLUETOOTH_CONNECTION_CLOSED        0x09

/* ================================ */
/* KGAPI COMMAND/EVENT DECLARATIONS */
/* ================================ */

/* 0x01 */ uint16_t kg_cmd_bluetooth_get_mode(uint8_t *mode);
/* 0x02 */ uint16_t kg_cmd_bluetooth_set_mode(uint8_t mode);
/* 0x03 */ uint16_t kg_cmd_bluetooth_reset();
/* 0x04 */ uint16_t kg_cmd_bluetooth_get_mac(uint8_t *address);
/* 0x05 */ uint16_t kg_cmd_bluetooth_get_pairings(uint8_t *count);
/* 0x06 */ uint16_t kg_cmd_bluetooth_discover(uint8_t duration);
/* 0x07 */ uint16_t kg_cmd_bluetooth_pair(uint8_t *address);
/* 0x08 */ uint16_t kg_cmd_bluetooth_delete_pairing(uint8_t index);
/* 0x09 */ uint16_t kg_cmd_bluetooth_clear_pairings();
/* 0x0A */ uint16_t kg_cmd_bluetooth_get_connections(uint8_t *count);
/* 0x0B */ uint16_t kg_cmd_bluetooth_connect(uint8_t index, uint8_t profile);
/* 0x0C */ uint16_t kg_cmd_bluetooth_disconnect(uint8_t handle);
// -- command/event split --
/* 0x01 */ uint8_t (*kg_evt_bluetooth_mode)(uint8_t mode);
/* 0x02 */ uint8_t (*kg_evt_bluetooth_ready)();
/* 0x03 */ uint8_t (*kg_evt_bluetooth_inquiry_response)(uint8_t *address, uint8_t *cod, int8_t rssi, uint8_t status, uint8_t index, uint8_t name_len, uint8_t *name_data);
/* 0x04 */ uint8_t (*kg_evt_bluetooth_inquiry_complete)(uint8_t count);
/* 0x05 */ uint8_t (*kg_evt_bluetooth_pairing_status)(uint8_t index, uint8_t *address, uint8_t priority, uint8_t profiles_supported, uint8_t profiles_active, uint8_t handle_list_len, uint8_t *handle_list_data);
/* 0x06 */ uint8_t (*kg_evt_bluetooth_pairing_failed)(uint8_t *address);
/* 0x07 */ uint8_t (*kg_evt_bluetooth_pairings_cleared)();
/* 0x08 */ uint8_t (*kg_evt_bluetooth_connection_status)(uint8_t handle, uint8_t *address, uint8_t index, uint8_t profile, uint8_t status);
/* 0x09 */ uint8_t (*kg_evt_bluetooth_connection_closed)(uint8_t handle, uint16_t reason);

/* ================================ */
/* ================================ */
/* ================================ */

/**
 * @brief List of possible values for blink mode
 */
typedef enum {
    KG_BLUETOOTH_MODE_DISABLED = 0,     ///< Bluetooth is disabled, undiscoverable and unconnectable (radio off)
    KG_BLUETOOTH_MODE_AUTOCALL,         ///< Bluetooth enabled and in autocall mode if any pairing entries exist
    KG_BLUETOOTH_MODE_MANUAL,           ///< Bluetooth enabled but will not autocall, and will be visible only if no pairings exist
    KG_BLUETOOTH_MODE_VISIBLE,          ///< Bluetooth enabled and will not autocall, but will be visible in inquiries at all times
    KG_BLUETOOTH_MODE_MAX
} bluetooth_mode_t;

#define KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY          0x0801  ///< Bluetooth interface has not been initialized
#define KG_BLUETOOTH_ERROR_INTERFACE_BUSY               0x0802  ///< Bluetooth module is currently busy and cannot accept this command

#define BLUETOOTH_PROFILE_MASK_HID_CONTROL      0x01    ///< Bitmask indicating HID control link
#define BLUETOOTH_PROFILE_MASK_HID_INTERRUPT    0x02    ///< Bitmask indicating HID interrupt link
#define BLUETOOTH_PROFILE_MASK_IAP              0x04    ///< Bitmask indicating IAP link
#define BLUETOOTH_PROFILE_MASK_SPP              0x08    ///< Bitmask indicating SPP (RFCOMM) link
#define BLUETOOTH_PROFILE_MASK_HFP              0x10    ///< Bitmask indicating HFP link
#define BLUETOOTH_PROFILE_MASK_AVRCP            0x20    ///< Bitmask indicating AVRCP link

//#if KG_HOSTIF & KG_HOSTIF_BT2_HID
    uint8_t bluetoothTXHIDKBPacket[12];         ///< Buffer for outgoing HID keyboard report payload
    uint8_t bluetoothTXHIDCPPacket[7];          ///< Buffer for outgoing HID consumer page report payload
    uint8_t bluetoothTXHIDMousePacket[9];       ///< Buffer for outgoing HID mouse report payload
//#endif
//#if KG_HOSTIF & KG_HOSTIF_BT2_RAWHID
    #define BT2_RAWHID_RX_SIZE 16                               ///< Raw HID input payload size
    #define BT2_RAWHID_TX_SIZE 16                               ///< Raw HID output payload size
    uint8_t bluetoothRXRawHIDPacket[BT2_RAWHID_RX_SIZE + 4];    ///< Buffer for incoming raw HID report
    uint8_t bluetoothTXRawHIDPacket[BT2_RAWHID_TX_SIZE + 4];    ///< Buffer for outgoing raw HID report
//#endif

// Bluetooth link/interface readiness
bool interfaceBT2Ready = false;         ///< General Bluetooth ready indicator
bool interfaceBT2SerialReady = false;   ///< Bluetooth SPP connection active indicator
uint8_t interfaceBT2SerialMode = KG_API_BT2_SERIAL; ///< Bluetooth SPP connection KGAPI mode
bool interfaceBT2IAPReady = false;      ///< Bluetooth iAP connection active indicator
uint8_t interfaceBT2IAPMode = KG_API_BT2_IAP;       ///< Bluetooth IAP connection KGAPI mode
bool interfaceBT2RawHIDReady = false;   ///< Bluetooth raw HID connection active indicator
uint8_t interfaceBT2RawHIDMode = KG_API_BT2_RAWHID; ///< Bluetooth raw HID connection KGAPI mode
bool interfaceBT2HIDReady = false;      ///< Bluetooth HID connection active indicator
bool interfaceBT2HFPReady = false;      ///< Bluetooth HFP connection active indicator
bool interfaceBT2AVRCPReady = false;    ///< Bluetooth AVRCP connection active indicator

// ======== BEGIN DECLARATIONS COPIED/TWEAKED FROM IWRAP ARDUINO DEMO ========

//#include <iWRAP.h>

#define IWRAP_STATE_IDLE                0
#define IWRAP_STATE_UNKNOWN             1
#define IWRAP_STATE_PENDING_AT          2
#define IWRAP_STATE_PENDING_SET         3
#define IWRAP_STATE_PENDING_LIST        4
#define IWRAP_STATE_PENDING_CALL        5
#define IWRAP_STATE_PENDING_INQUIRY     6
#define IWRAP_STATE_PENDING_PAIR        7
#define IWRAP_STATE_PENDING_SETBTPAIR   8
#define IWRAP_STATE_COMM_FAILED         255

#define IWRAP_MAX_PAIRINGS              16

/**
 * @brief iWRAP paired device record structure
 */
typedef struct {
    iwrap_address_t mac;                ///< MAC address of paired device
    uint8_t priority;                   ///< Connection priority of this device among others
    uint8_t profiles_supported;         ///< Bitmask for which profiles are supported
    uint8_t profiles_active;            ///< Bitmask for which profiles are actively connected
    uint8_t active_links;               ///< Overall active link count
    //uint8_t link_a2dp1;
    //uint8_t link_a2dp2;
    uint8_t link_avrcp;                 ///< AVRCP link ID
    uint8_t link_hfp;                   ///< HFP link ID
    //uint8_t link_hfpag;
    uint8_t link_hid_control;           ///< HID control channel link ID
    uint8_t link_hid_interrupt;         ///< HID interrupt channel link ID
    //uint8_t link_hsp;
    //uint8_t link_hspag;
    uint8_t link_iap;                   ///< iAP link ID
    //uint8_t link_sco;
    uint8_t link_spp;                   ///< SPP/RFCOMM link ID
    // other profile-specific link IDs may be added here
} iwrap_pairing_t;

iwrap_pairing_t *iwrap_connection_map[IWRAP_MAX_PAIRINGS];   ///< Array of paired devices for connection mapping

// iwrap state tracking info
uint8_t iwrap_mode = IWRAP_MODE_MUX;            ///< iWRAP UART interface mode
uint8_t iwrap_state = IWRAP_STATE_UNKNOWN;      ///< Current iWRAP state
uint8_t iwrap_initialized = 0;                  ///< Flag indicating iWRAP initialization status
//uint32_t iwrap_time_ref = 0;
uint8_t iwrap_page_mode = 0;                    ///< Current Bluetooth page mode (visible/connectable)
uint8_t iwrap_pairings = 0;                     ///< Number of paired devices known
uint8_t iwrap_pending_calls = 0;                ///< Number of pending outgoing calls (should always be 0 or 1)
uint8_t iwrap_pending_call_link_id = 0xFF;      ///< Link ID assigned to pending call, if any
uint8_t iwrap_connected_devices = 0;            ///< Number of unique connected devices
uint8_t iwrap_active_connections = 0;           ///< Total number of active links
uint8_t iwrap_autocall_target = 0;              ///< How many unique devices should autocall attempt to connect to
//uint16_t iwrap_autocall_delay_ms = 10000;
//uint32_t iwrap_autocall_last_time = 0;
uint8_t iwrap_autocall_index = 0;               ///< Current pairing entry index for next autocall attempt
iwrap_address_t iwrap_module_mac;               ///< Local Bluetooth module's MAC address

// iWRAP callbacks necessary for application
void my_iwrap_callback_txcommand(uint16_t length, const uint8_t *data);
void my_iwrap_callback_txdata(uint8_t channel, uint16_t length, const uint8_t *data);
void my_iwrap_callback_rxoutput(uint16_t length, const uint8_t *data);
void my_iwrap_callback_rxdata(uint8_t channel, uint16_t length, const uint8_t *data);
void my_iwrap_rsp_call(uint8_t link_id);
//void my_iwrap_rsp_inquiry_count(uint8_t num_of_devices);
void my_iwrap_rsp_inquiry_result(const iwrap_address_t *bd_addr, uint32_t class_of_device, int8_t rssi);
void my_iwrap_rsp_list_count(uint8_t num_of_connections);
void my_iwrap_rsp_list_result(uint8_t link_id, const char *mode, uint16_t blocksize, uint32_t elapsed_time, uint16_t local_msc, uint16_t remote_msc, const iwrap_address_t *addr, uint16_t channel, uint8_t direction, uint8_t powermode, uint8_t role, uint8_t crypt, uint16_t buffer, uint8_t eretx);
void my_iwrap_rsp_pair(const iwrap_address_t *address, uint8_t result);
void my_iwrap_rsp_set(uint8_t category, const char *option, const char *value);
void my_iwrap_evt_connect(uint8_t link_id, const char *type, uint16_t target, const iwrap_address_t *address);
void my_iwrap_evt_inquiry_extended(const iwrap_address_t *address, uint8_t length, const uint8_t *data);
void my_iwrap_evt_inquiry_partial(const iwrap_address_t *address, uint32_t class_of_device, const char *cached_name, int8_t rssi);
void my_iwrap_evt_name(const iwrap_address_t *address, const char *friendly_name);
//void my_iwrap_evt_name_error(uint16_t error_code, const iwrap_address_t *address, const char *message);
void my_iwrap_evt_no_carrier(uint8_t link_id, uint16_t error_code, const char *message);
void my_iwrap_evt_pair(const iwrap_address_t *address, uint8_t key_type, const uint8_t *link_key);
void my_iwrap_evt_ready();
void my_iwrap_evt_ring(uint8_t link_id, const iwrap_address_t *mac, uint16_t channel, const char *profile);

// general helper functions
uint8_t find_pairing_from_mac(const iwrap_address_t *mac);
uint8_t find_pairing_from_link_id(uint8_t link_id);
void add_mapped_connection(uint8_t link_id, const iwrap_address_t *addr, const char *mode, uint16_t channel);
uint8_t remove_mapped_connection(uint8_t link_id);

// platform-specific helper functions
int serial_out(const char *str);
int serial_out(const __FlashStringHelper *str);
int iwrap_out(int len, unsigned char *data);

// ======== END DECLARATIONS COPIED/TWEAKED FROM IWRAP ARDUINO DEMO ========

// track which connected device is active for any given profile
uint8_t bluetoothSPPDeviceIndex = 0;        ///< Pairing index for device with active/selected SPP connection
uint8_t bluetoothIAPDeviceIndex = 0;        ///< Pairing index for device with active/selected iAP connection
uint8_t bluetoothHIDDeviceIndex = 0;        ///< Pairing index for device with active/selected HID connection
uint8_t bluetoothRawHIDDeviceIndex = 0;     ///< Pairing index for device with active/selected raw HID connection
uint8_t bluetoothHFPDeviceIndex = 0;        ///< Pairing index for device with active/selected HFP connection
uint8_t bluetoothAVRCPDeviceIndex = 0;      ///< Pairing index for device with active/selected AVRCP connection

bluetooth_mode_t bluetoothMode = KG_BLUETOOTH_MODE_DISABLED;    ///< Current Bluetooth subsystem mode
uint32_t bluetoothTock = 0;                                     ///< 1 Hz counter for tracking certain Bluetooth activities

uint16_t bluetoothPendingConnectionStatus = 0xFFFF;     ///< Bitmask for which connections should have status update events triggered (0-15)
uint8_t bluetoothPendingCallPairIndex = 0;              ///< Pairing index for device with pending call active
uint8_t bluetoothPendingCallProfile = 0;                ///< Bitmask for which profile was used for pending call
uint16_t bluetoothActiveLinkMask = 0x0000;              ///< Bitmask for which link IDs are currently allocated to active links (0-15)

/**
 * @brief Simple wrapper for Teensy-like keyboard behavior and iWRAP raw HID report interface
 */
class BTKeyboardWrapper {
    public:
        /**
         * @brief Set the key code for keyboard report position 1 of 6
         * @param[in] code Key code to use
         */
        void set_key1(uint8_t code) { bluetoothTXHIDKBPacket[6] = code; }
        /**
         * @brief Set the key code for keyboard report position 2 of 6
         * @param[in] code Key code to use
         */
        void set_key2(uint8_t code) { bluetoothTXHIDKBPacket[7] = code; }
        /**
         * @brief Set the key code for keyboard report position 3 of 6
         * @param[in] code Key code to use
         */
        void set_key3(uint8_t code) { bluetoothTXHIDKBPacket[8] = code; }
        /**
         * @brief Set the key code for keyboard report position 4 of 6
         * @param[in] code Key code to use
         */
        void set_key4(uint8_t code) { bluetoothTXHIDKBPacket[9] = code; }
        /**
         * @brief Set the key code for keyboard report position 5 of 6
         * @param[in] code Key code to use
         */
        void set_key5(uint8_t code) { bluetoothTXHIDKBPacket[10] = code; }
        /**
         * @brief Set the key code for keyboard report position 6 of 6
         * @param[in] code Key code to use
         */
        void set_key6(uint8_t code) { bluetoothTXHIDKBPacket[11] = code; }
        /**
         * @brief Set the modifier key bitmask in the keyboard report
         * @param[in] code Key code to use
         */
        void set_modifier(uint8_t code) { bluetoothTXHIDKBPacket[4] = code; }
        /**
         * @brief Send the HID report using current data
         */
        void send_now() {
            // send packet out over wireless HID interface (Bluetooth v2.1 HID)
            if (interfaceBT2HIDReady && iwrap_connection_map[bluetoothHIDDeviceIndex] && iwrap_connection_map[bluetoothHIDDeviceIndex] -> link_hid_interrupt != 0xFF) {
                iwrap_send_data(iwrap_connection_map[bluetoothHIDDeviceIndex] -> link_hid_interrupt, 12, (const uint8_t *)bluetoothTXHIDKBPacket, iwrap_mode);
            }
        }
} BTKeyboard; ///< Predefined BTKeyboardWrapper object

/**
 * @brief Simple wrapper for Teensy-like mouse behavior and iWRAP HID interface
 */
class BTMouseWrapper {
    public:
        /**
         * @brief Move the mouse cursor some relative amount
         * @param[in] x Pixels to move cursor horizontally (positive=right)
         * @param[in] y Pixels to move cursor vertically (positive=down)
         */
        void move(int8_t x, int8_t y) {
            move(x, y, 0, 0);
        }
        /**
         * @brief Scroll on either axis
         * @param[in] vscroll Lines to move vertically (positive=down)
         * @param[in] hscroll Characters to move horizontally (positive=right)
         */
        void scroll(int8_t vscroll, int8_t hscroll) {
            move(0, 0, vscroll, hscroll);
        }
        /**
         * @brief Move the mouse cursor and/or scroll on either axis
         * @param[in] x Pixels to move cursor horizontally (positive=right)
         * @param[in] y Pixels to move cursor vertically (positive=down)
         * @param[in] vscroll Lines to move vertically (positive=down)
         * @param[in] hscroll Characters to move horizontally (positive=right)
         */
        void move(int8_t x, int8_t y, int8_t vscroll, int8_t hscroll) {
            // update mouse coordinates
            bluetoothTXHIDMousePacket[4] = x;
            bluetoothTXHIDMousePacket[7] = y;
            bluetoothTXHIDMousePacket[8] = vscroll;
            bluetoothTXHIDMousePacket[9] = hscroll;

            // send packet out over wireless HID interface (Bluetooth v2.1 HID)
            if (interfaceBT2HIDReady && iwrap_connection_map[bluetoothHIDDeviceIndex] && iwrap_connection_map[bluetoothHIDDeviceIndex] -> link_hid_interrupt != 0xFF) {
                iwrap_send_data(iwrap_connection_map[bluetoothHIDDeviceIndex] -> link_hid_interrupt, 9, (const uint8_t *)bluetoothTXHIDMousePacket, iwrap_mode);
            }
        }
} BTMouse;  ///< Predefined BTMouseWrapper object

/**
 * @brief Initialize Bluetooth v2 (iWRAP) subsystem
 */
void setup_hostif_bt2() {
    // initialize pins
    //pinMode(KG_PIN_BT_DTR, OUTPUT);
    //pinMode(KG_PIN_BT_LINK, INPUT);
    //digitalWrite(KG_PIN_BT_DTR, LOW);

    // reset module interface ready state
    interfaceBT2Ready = false;

    // reset host interface ready states
    interfaceBT2SerialReady = false;
    interfaceBT2IAPReady = false;
    interfaceBT2HIDReady = false;
    interfaceBT2RawHIDReady = false;
    interfaceBT2HFPReady = false;
    interfaceBT2AVRCPReady = false;

    // pre-build raw HID report
    bluetoothTXRawHIDPacket[0] = 0x9F;
    bluetoothTXRawHIDPacket[1] = 0x12;
    bluetoothTXRawHIDPacket[2] = 0xA1;
    bluetoothTXRawHIDPacket[3] = 0x04;

    bluetoothMode = KG_BLUETOOTH_MODE_DISABLED;
    bluetoothTock = 0;

    iwrap_state = IWRAP_STATE_UNKNOWN;
    iwrap_initialized = 0;
    iwrap_connected_devices = 0;
    iwrap_pending_calls = 0;
    iwrap_autocall_index = 0;

    // reset connected device/profile indexes
    bluetoothSPPDeviceIndex = 0;
    bluetoothIAPDeviceIndex = 0;
    bluetoothHIDDeviceIndex = 0;
    bluetoothRawHIDDeviceIndex = 0;
    bluetoothHFPDeviceIndex = 0;
    bluetoothAVRCPDeviceIndex = 0;

    // pre-build KB/CP/mouse HID reports
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

    // initialize iWRAP device
    BT2Serial.begin(KG_HOSTIF_BT2_SERIAL_BAUD);

    // assign callbacks
    /* OK */ iwrap_output = iwrap_out;
    /* OK */ //iwrap_callback_txcommand = my_iwrap_callback_txcommand;
    /* OK */ //iwrap_callback_rxoutput = my_iwrap_callback_rxoutput;
    /* OK */ //iwrap_callback_txdata = my_iwrap_callback_txdata;
    /* OK */ iwrap_callback_rxdata = my_iwrap_callback_rxdata;
    /* OK */ iwrap_rsp_call = my_iwrap_rsp_call;
    /* OK */ //iwrap_rsp_inquiry_count = my_iwrap_rsp_inquiry_count;
    /* OK */ iwrap_rsp_inquiry_result = my_iwrap_rsp_inquiry_result;
    /* OK */ iwrap_rsp_list_count = my_iwrap_rsp_list_count;
    /* OK */ iwrap_rsp_list_result = my_iwrap_rsp_list_result;
    /* OK */ iwrap_rsp_pair = my_iwrap_rsp_pair;
    /* OK */ iwrap_rsp_set = my_iwrap_rsp_set;
    /* OK */ iwrap_evt_connect = my_iwrap_evt_connect;
    /* OK */ iwrap_evt_inquiry_extended = my_iwrap_evt_inquiry_extended;
    /* OK */ iwrap_evt_inquiry_partial = my_iwrap_evt_inquiry_partial;
    /* OK */ iwrap_evt_name = my_iwrap_evt_name;
    /* OK */ //iwrap_evt_name_error = my_iwrap_evt_name_error;
    /* OK */ iwrap_evt_no_carrier = my_iwrap_evt_no_carrier;
    /* OK */ iwrap_evt_pair = my_iwrap_evt_pair;
    /* OK */ iwrap_evt_ready = my_iwrap_evt_ready;
    /* OK */ iwrap_evt_ring = my_iwrap_evt_ring;

    // read any available data from the Bluetooth module
    while (BT2Serial.available()) BT2Serial.read();
}

/**
 * @brief Check for new incoming data from iWRAP, called every iteration from check_incoming_protocol_data()
 */
uint8_t bluetooth_check_incoming_protocol_data() {
    uint16_t result;

    // manage iWRAP state machine
    if (!iwrap_pending_commands) {
        // no pending commands, some state transition occurring
        if (iwrap_state) {
            // not idle, in the middle of some process
            if (iwrap_state == IWRAP_STATE_UNKNOWN) {
                // don't try to connect until 1 second after boot
                if (keygloveTock > 1) {
                    // reset all detailed state trackers
                    iwrap_initialized = 0;
                    iwrap_pairings = 0;
                    iwrap_pending_calls = 0;
                    iwrap_pending_call_link_id = 0xFF;
                    iwrap_connected_devices = 0;
                    iwrap_active_connections = 0;
                    
                    // reset host interface ready states
                    interfaceBT2SerialReady = false;
                    interfaceBT2IAPReady = false;
                    interfaceBT2HIDReady = false;
                    interfaceBT2RawHIDReady = false;
                    interfaceBT2HFPReady = false;
                    interfaceBT2AVRCPReady = false;

                    // send command to test module connectivity
                    send_keyglove_log(KG_LOG_LEVEL_NORMAL, 30, F("Testing iWRAP communication..."));
                    iwrap_send_command("AT", iwrap_mode);
                    iwrap_state = IWRAP_STATE_PENDING_AT;

                    // initialize time reference for connectivity test timeout
                    bluetoothTock = keygloveTock;
                }
            } else if (iwrap_state == IWRAP_STATE_PENDING_AT) {
                // send command to dump all module settings and pairings
                send_keyglove_log(KG_LOG_LEVEL_NORMAL, 25, F("Getting iWRAP settings..."));
                iwrap_send_command("SET", iwrap_mode);
                iwrap_state = IWRAP_STATE_PENDING_SET;
            } else if (iwrap_state == IWRAP_STATE_PENDING_SET) {
                // send command to show all current connections
                send_keyglove_log(KG_LOG_LEVEL_NORMAL, 33, F("Getting active connection list..."));
                iwrap_send_command("LIST", iwrap_mode);
                iwrap_state = IWRAP_STATE_PENDING_LIST;
            } else if (iwrap_state == IWRAP_STATE_PENDING_LIST) {
                // all done!
                if (!iwrap_initialized) {
                    iwrap_initialized = 1;
                    interfaceBT2Ready = true; // KGAPI status tracking
                    send_keyglove_log(KG_LOG_LEVEL_NORMAL, 29, F("iWRAP initialization complete"));
                    
                    // send kg_evt_bluetooth_ready()
                    skipPacket = 0;
                    if (kg_evt_bluetooth_ready) skipPacket = kg_evt_bluetooth_ready();
                    if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 0, KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_READY, 0);
                }
                iwrap_state = IWRAP_STATE_IDLE;
            } else if (iwrap_state == IWRAP_STATE_PENDING_CALL && !iwrap_pending_calls) {
                // all done!
                send_keyglove_log(KG_LOG_LEVEL_NORMAL, 22, F("Pending call processed"));
                iwrap_state = IWRAP_STATE_IDLE;
            } else if (iwrap_state == IWRAP_STATE_PENDING_SETBTPAIR) {
                // send kg_evt_bluetooth_pairings_cleared()
                if (!inBinPacket) {
                    skipPacket = 0;
                    if (kg_evt_bluetooth_pairings_cleared) skipPacket = kg_evt_bluetooth_pairings_cleared();
                    if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 0, KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_PAIRINGS_CLEARED, 0);
                }
                iwrap_state = IWRAP_STATE_IDLE;
            }
        } else if (iwrap_initialized) {
            // idle
            if (iwrap_pairings && iwrap_autocall_target > iwrap_connected_devices && !iwrap_pending_calls
                             //&& (!iwrap_autocall_last_time || (millis() - iwrap_autocall_last_time) >= iwrap_autocall_delay_ms)) {
                               && (keygloveTock - bluetoothTock >= 10 || keygloveTock < 3)) {
                char cmd[] = "CALL AA:BB:CC:DD:EE:FF 11 HID";       // HID
                char *cptr = cmd + 5;
                
                // find first unconnected device
                for (; iwrap_connection_map[iwrap_autocall_index] != 0 && iwrap_connection_map[iwrap_autocall_index] -> active_links; iwrap_autocall_index++);
                
                if (!iwrap_connection_map[iwrap_autocall_index]) {
                    // this should NEVER happen, but if it does, I want to know
                    uint8_t payload[2] = { KG_PROTOCOL_ERROR_NULL_POINTER & 0xFF, KG_PROTOCOL_ERROR_NULL_POINTER >> 8 };
                    skipPacket = 0;
                    if (kg_evt_protocol_error) skipPacket = kg_evt_protocol_error(KG_PROTOCOL_ERROR_NULL_POINTER);
                    if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
                    return 0xFF;
                }

                bluetoothPendingCallPairIndex = iwrap_autocall_index;
                bluetoothPendingCallProfile = BLUETOOTH_PROFILE_MASK_HID_CONTROL;

                // write MAC string into call command buffer and send it
                iwrap_bintohexstr((uint8_t *)(iwrap_connection_map[iwrap_autocall_index] -> mac.address), 6, &cptr, ':', 0);
                char s[21];
                sprintf(s, "Calling device #%d\r\n", iwrap_autocall_index);
                send_keyglove_log(KG_LOG_LEVEL_NORMAL, strlen(s), s);
                iwrap_send_command(cmd, iwrap_mode);
                //iwrap_autocall_last_time = millis();
                bluetoothTock = keygloveTock;
            }
        }
    }
    
    // check for incoming iWRAP data
    if ((result = BT2Serial.read()) < 256) iwrap_parse(result & 0xFF, iwrap_mode);

    // check for timeout if still testing communication
    if (!iwrap_initialized && iwrap_state == IWRAP_STATE_PENDING_AT) {
        if (keygloveTock - bluetoothTock > 4) {
            send_keyglove_log(KG_LOG_LEVEL_CRITICAL, 39, F("Could not communicate with iWRAP module"));
            iwrap_state = IWRAP_STATE_COMM_FAILED;
            iwrap_pending_commands = 0; // normally handled by the parser, but comms failed
        }
    }

    return 0;
}

/**
 * @brief Send a KGAPI packet using the Bluetooth v2 (iWRAP) interface
 * @param[in] buffer Outgoing packet buffer
 * @param[in] length Outgoing packet length
 * @param[in] specificInterface Which interface to use, or zero for all available
 * @return Result, zero for success or non-zero for error
 */
uint8_t bluetooth_send_keyglove_packet_buffer(uint8_t *buffer, uint8_t length, uint8_t specificInterface) {
    #if KG_HOSTIF & KG_HOSTIF_BT2_SERIAL
        if (!specificInterface || lastCommandInterfaceNum == KG_INTERFACENUM_BT2_SERIAL) {
            // send packet out over wireless serial (Bluetooth v2.1 SPP)
            if (interfaceBT2SerialReady && (interfaceBT2SerialMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0 && iwrap_connection_map[bluetoothSPPDeviceIndex] && iwrap_connection_map[bluetoothSPPDeviceIndex] -> link_spp != 0xFF) {
                iwrap_send_data(iwrap_connection_map[bluetoothSPPDeviceIndex] -> link_spp, length, (const uint8_t *)buffer, iwrap_mode);
            }
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_BT2_RAWHID
        if (!specificInterface || lastCommandInterfaceNum == KG_INTERFACENUM_BT2_RAWHID) {
            // send packet out over wireless custom HID interface (Bluetooth v2.1 raw HID)
            if (interfaceBT2RawHIDReady && (interfaceBT2RawHIDMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0 && iwrap_connection_map[bluetoothRawHIDDeviceIndex] && iwrap_connection_map[bluetoothRawHIDDeviceIndex] -> link_hid_interrupt != 0xFF) {
                int8_t bytes;
                for (uint8_t i = 0; i < length; i += (BT2_RAWHID_TX_SIZE - 1)) {
                    memset(bluetoothTXRawHIDPacket + 4, 0, BT2_RAWHID_TX_SIZE);
                    bluetoothTXRawHIDPacket[4] = min(BT2_RAWHID_TX_SIZE - 1, length - i);
                    for (uint8_t j = 0; j < (BT2_RAWHID_TX_SIZE - 1); j++) {
                        if (i + j >= length) break;
                        bluetoothTXRawHIDPacket[j + 5] = buffer[i + j];
                    }
                    iwrap_send_data(iwrap_connection_map[bluetoothRawHIDDeviceIndex] -> link_hid_interrupt, BT2_RAWHID_TX_SIZE + 4, (const uint8_t *)bluetoothTXRawHIDPacket, iwrap_mode);
                }
            }
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_BT2_IAP
        if (!specificInterface || lastCommandInterfaceNum == KG_INTERFACENUM_BT2_IAP) {
            // send packet out over wireless iAP link (Bluetooth v2.1 IAP)
            if (interfaceBT2IAPReady && (interfaceBT2IAPMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0 && iwrap_connection_map[bluetoothIAPDeviceIndex] && iwrap_connection_map[bluetoothIAPDeviceIndex] -> link_iap != 0xFF) {
                iwrap_send_data(iwrap_connection_map[bluetoothIAPDeviceIndex] -> link_iap, length, (const uint8_t *)buffer, iwrap_mode);
            }
        }
    #endif
    
    return 0;
}

/* ============================================================================
 * IWRAP RESPONSE AND EVENT HANDLER IMPLEMENTATIONS
 * ========================================================================= */

/**
 * @brief iWRAP TX command callback
 * @param[in] length Length of command sent
 * @param[in] data Buffer of command bytes sent
 *
 * This function is called immediately before a command is sent out via the
 * iWRAP UART interface. No MUX encapsulation is shown even if MUX mode is
 * enabled.
 */
void my_iwrap_callback_txcommand(uint16_t length, const uint8_t *data) {
    char s[length + 18];
    sprintf(s, "=> BT2 (FF, %d): %s", length, data);
    send_keyglove_log(KG_LOG_LEVEL_VERBOSE, strlen(s), s);
}

/**
 * @brief iWRAP TX data callback
 * @param[in] channel Which data channel the data came in up
 * @param[in] length Length of data sent
 * @param[in] data Buffer of data bytes sent
 *
 * This function is called immediately before a data packet is sent out via the
 * iWRAP UART interface. No MUX encapsulation is shown even if MUX mode is
 * enabled.
 */
void my_iwrap_callback_txdata(uint8_t channel, uint16_t length, const uint8_t *data) {
    char s[length + 18];
    sprintf(s, "=> BT2 (%02X, %d): %s", channel, length, data);
    send_keyglove_log(KG_LOG_LEVEL_VERBOSE, strlen(s), s);
}

/**
 * @brief iWRAP RX output callback
 * @param[in] length Length of iWRAP output received
 * @param[in] data Buffer of iWRAP output bytes received
 *
 * This function is called immediately after a response/event line is fully
 * received from the iWRAP command channel, before any relevant response or
 * event handlers are called. No MUX encapsulation is shown even if MUX mode is
 * enabled.
 */
void my_iwrap_callback_rxoutput(uint16_t length, const uint8_t *data) {
    char s[length + 28];
    uint8_t *p = (uint8_t *)data;
    int len = sprintf(s, "<= BT2 (FF, %d): ", length);
    for (; p - data < length && len < length + 24; p++) {
        if (p[0] > 31 && p[0] < 127) {
            s[len++] = p[0];
        } else if (p[0] == 0x09) {
            s[len++] = '\\';
            s[len++] = 't';
        } else if (p[0] == 0x0A) {
            s[len++] = '\\';
            s[len++] = 'n';
        } else if (p[0] == 0x0D) {
            s[len++] = '\\';
            s[len++] = 'r';
        } else {
            s[len++] = '\\';
            s[len++] = 'x';
            s[len++] = 'Z';
            s[len++] = 'Z';
        }
    }
    s[len] = 0; // null terminate
    send_keyglove_log(KG_LOG_LEVEL_VERBOSE, strlen(s), s);
}

/**
 * @brief iWRAP RX data callback
 * @param[in] channel Which data channel the data came in up
 * @param[in] length Length of iWRAP data packet received
 * @param[in] data Buffer of iWRAP data packet bytes received
 *
 * This function is called immediately after a line or frame of user data is
 * received from an open iWRAP data channel. No MUX encapsulation is shown even
 * if MUX mode is enabled.
 */
void my_iwrap_callback_rxdata(uint8_t channel, uint16_t length, const uint8_t *data) {
    ///< TODO: implement incoming data over Bluetooth

    /*
    char s[length + 18];
    sprintf(s, "<= BT2 (%02X, %d): %s", channel, length, data);
    send_keyglove_log(KG_LOG_LEVEL_VERBOSE, strlen(s), s);
    */
}

/**
 * @brief iWRAP "CALL" response handler
 * @param[in] link_id Link ID allocated to new pending outgoing call
 */
void my_iwrap_rsp_call(uint8_t link_id) {
    iwrap_pending_calls++;
    iwrap_pending_call_link_id = link_id;
    iwrap_autocall_index = (iwrap_autocall_index + 1) % iwrap_pairings;
    iwrap_state = IWRAP_STATE_PENDING_CALL;
    
    if (iwrap_connection_map[bluetoothPendingCallPairIndex]) {
        // send bluetooth_connection_status event for pending connection
        uint8_t payload[10] = { link_id, 0, 0, 0, 0, 0, 0, bluetoothPendingCallPairIndex, bluetoothPendingCallProfile, 1 };
        payload[1] = iwrap_connection_map[bluetoothPendingCallPairIndex] -> mac.address[5]; // little-endian byte order
        payload[2] = iwrap_connection_map[bluetoothPendingCallPairIndex] -> mac.address[4];
        payload[3] = iwrap_connection_map[bluetoothPendingCallPairIndex] -> mac.address[3];
        payload[4] = iwrap_connection_map[bluetoothPendingCallPairIndex] -> mac.address[2];
        payload[5] = iwrap_connection_map[bluetoothPendingCallPairIndex] -> mac.address[1];
        payload[6] = iwrap_connection_map[bluetoothPendingCallPairIndex] -> mac.address[0];
        
        // send kg_evt_bluetooth_connection_status(...)
        skipPacket = 0;
        if (kg_evt_bluetooth_connection_status) skipPacket = kg_evt_bluetooth_connection_status(payload[0], payload + 1, payload[7], payload[8], payload[9]);
        if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 10, KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_CONNECTION_STATUS, payload);
        bluetoothPendingConnectionStatus |= (1 << link_id);
    } else {
        // this should NEVER happen, but if it does, I want to know
        uint8_t payload[2] = { KG_PROTOCOL_ERROR_NULL_POINTER & 0xFF, KG_PROTOCOL_ERROR_NULL_POINTER >> 8 };
        skipPacket = 0;
        if (kg_evt_protocol_error) skipPacket = kg_evt_protocol_error(KG_PROTOCOL_ERROR_NULL_POINTER);
        if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
    }
}

/*x*
 * @brief iWRAP "INQUIRY" count response handler
 * @param[in] num_of_devices Total number of devices found in inquiry
 *x/
void my_iwrap_rsp_inquiry_count(uint8_t num_of_devices) {
}*/

/**
 * @brief iWRAP "INQUIRY" result response handler
 * @param[in] mac MAC address of remote device
 * @param[in] class_of_device Bluetooth class of device
 * @param[in] rssi RSSI (signal strength) of device
 */
void my_iwrap_rsp_inquiry_result(const iwrap_address_t *mac, uint32_t class_of_device, int8_t rssi) {
    uint8_t payload[13];
    payload[0] = mac -> address[5];
    payload[1] = mac -> address[4];
    payload[2] = mac -> address[3];
    payload[3] = mac -> address[2];
    payload[4] = mac -> address[1];
    payload[5] = mac -> address[0];
    payload[6] = class_of_device & 0xFF;
    payload[7] = (class_of_device >> 8) & 0xFF;
    payload[8] = (class_of_device >> 16) & 0xFF;
    payload[9] = rssi; // RSSI
    payload[10] = 3; // inquiry status
    payload[11] = find_pairing_from_mac(mac);
    payload[12] = 0; // no NAME yet

    // send kg_evt_bluetooth_inquiry_response(...) event
    skipPacket = 0;
    if (kg_evt_bluetooth_inquiry_response) skipPacket = kg_evt_bluetooth_inquiry_response(payload + 0, payload + 6, payload[9], payload[10], payload[11], payload[12], 0);
    if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 13, KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_INQUIRY_RESPONSE, payload);
}

/**
 * @brief iWRAP "LIST" count response handler
 * @param[in] num_of_connections Total number of active individual connections
 */
void my_iwrap_rsp_list_count(uint8_t num_of_connections) {
    iwrap_active_connections = num_of_connections;
}

/**
 * @brief iWRAP "LIST" result response handler
 * @param[in] link_id Link ID assigned to this connection
 * @param[in] mode Type of connection (profile used, e.g. "HID" or "HFP")
 * @param[in] blocksize MTU for this connection
 * @param[in] elapsed_time Number of seconds this connection has been active
 * @param[in] local_msc Local modem status control bits
 * @param[in] remote_msc Remote modem status control bits
 * @param[in] mac MAC address of remote device
 * @param[in] channel RFCOMM or L2CAP channel
 * @param[in] direction Outgoing (1) out incoming (2) connection
 * @param[in] powermode Active (1), hold (2), sniff (3), or park (4) mode
 * @param[in] role Master (1) or slave (2) role
 * @param[in] crypt Plaintext (1) or encrypted (2) connection
 * @param[in] buffer Number of bytes currently in iWRAP buffer for this link
 * @param[in] eretx Enhanced retransmission mode (0=disabled, 1=enabled)
 */
void my_iwrap_rsp_list_result(uint8_t link_id, const char *mode, uint16_t blocksize, uint32_t elapsed_time, uint16_t local_msc, uint16_t remote_msc, const iwrap_address_t *mac, uint16_t channel, uint8_t direction, uint8_t powermode, uint8_t role, uint8_t crypt, uint16_t buffer, uint8_t eretx) {
    bluetoothActiveLinkMask |= (1 << link_id);
    add_mapped_connection(link_id, mac, mode, channel);
}

/**
 * @brief iWRAP "PAIR" response handler
 * @param[in] mac MAC address of remote device
 * @param[in] result Status of pairing attempt (0=success, 1=failure)
 */
void my_iwrap_rsp_pair(const iwrap_address_t *mac, uint8_t result) {
    if (result) {
        // non-zero = "FAIL" result, send bluetooth_pairing_failed() event
        uint8_t payload[6];
        payload[0] = mac -> address[5];
        payload[1] = mac -> address[4];
        payload[2] = mac -> address[3];
        payload[3] = mac -> address[2];
        payload[4] = mac -> address[1];
        payload[5] = mac -> address[0];

        // send event
        skipPacket = 0;
        if (kg_evt_bluetooth_pairing_failed) skipPacket = kg_evt_bluetooth_pairing_failed(payload);
        if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 6, KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_PAIRING_FAILED, payload);
    } else {
        // zero = "OK" result, send bluetooth_pairing_status() event
        
        // build event (uint8_t index, macaddr_t address, uint8_t priority, uint8_t profiles_supported, uint8_t profiles_active, uint8_t[] handle_list)
        uint8_t i;
        
        // identify pairing by address in case it's been updated
        for (i = 0; i < iwrap_pairings; i++) {
            if (iwrap_connection_map[i]) {
                if (memcmp(mac -> address, iwrap_connection_map[i] -> mac.address, 6) == 0) {
                    // this pairing was already in the list, and we just found it
                    break;
                }
            } else {
                // this should NEVER happen, but if it does, I want to know
                uint8_t payload[2] = { KG_PROTOCOL_ERROR_NULL_POINTER & 0xFF, KG_PROTOCOL_ERROR_NULL_POINTER >> 8 };
                skipPacket = 0;
                if (kg_evt_protocol_error) skipPacket = kg_evt_protocol_error(KG_PROTOCOL_ERROR_NULL_POINTER);
                if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
                return;
            }
        }

        if (i = iwrap_pairings) {
            // this is a new pairing, so increment count
            iwrap_pairings++;
        }

        // make sure we allocate memory if needed
        if (iwrap_connection_map[i] == 0) {
            iwrap_connection_map[i] = (iwrap_pairing_t *)malloc(sizeof(iwrap_pairing_t));
        }
        
        // only continue if allocation succeeded
        if (iwrap_connection_map[i] != 0) {
            memset(iwrap_connection_map[i], 0xFF, sizeof(iwrap_pairing_t)); // 0xFF is "no link ID"
            iwrap_connection_map[i] -> active_links = 0;
            
            uint8_t payload[20];
            payload[0] = i; // pairing index
            payload[1] = iwrap_connection_map[i] -> mac.address[5] = mac -> address[5]; // little-endian MAC address
            payload[2] = iwrap_connection_map[i] -> mac.address[4] = mac -> address[4];
            payload[3] = iwrap_connection_map[i] -> mac.address[3] = mac -> address[3];
            payload[4] = iwrap_connection_map[i] -> mac.address[2] = mac -> address[2];
            payload[5] = iwrap_connection_map[i] -> mac.address[1] = mac -> address[1];
            payload[6] = iwrap_connection_map[i] -> mac.address[0] = mac -> address[0];
            payload[7] = iwrap_connection_map[i] -> priority = 0;
            payload[8] = iwrap_connection_map[i] -> profiles_supported = 0x3F; // assume everything
            payload[9] = iwrap_connection_map[i] -> profiles_active = 0x00; // assume nothing
            payload[10] = 0;
            memset(payload + 11, 0xFF, 6); // all connection handles are 0xFF
            
            // send event
            skipPacket = 0;
            if (kg_evt_bluetooth_pairing_status) skipPacket = kg_evt_bluetooth_pairing_status(payload[0], payload + 1, payload[7], payload[8], payload[9], payload[10], payload + 11);
            if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 11 + payload[10], KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_PAIRING_STATUS, payload);
        } else {
            // this should NEVER happen, but if it does, I want to know
            uint8_t payload[2] = { KG_PROTOCOL_ERROR_NULL_POINTER & 0xFF, KG_PROTOCOL_ERROR_NULL_POINTER >> 8 };
            skipPacket = 0;
            if (kg_evt_protocol_error) skipPacket = kg_evt_protocol_error(KG_PROTOCOL_ERROR_NULL_POINTER);
            if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
            return;
        }
    }
    if (iwrap_state == IWRAP_STATE_PENDING_PAIR) iwrap_state = IWRAP_STATE_IDLE;
}

/**
 * @brief iWRAP "SET" response handler
 * @param[in] category Setting category (1=BT, 2=CONTROL, 3=PROFILE)
 * @param[in] option Option name (e.g. "BDADDR", "PAGEMODE", "MUX", "HID", etc.)
 * @param[in] value Option value (e.g. "00:07:80:AA:BB:CC", 4 2000 1", etc.)
 */
void my_iwrap_rsp_set(uint8_t category, const char *option, const char *value) {
    if (category == IWRAP_SET_CATEGORY_BT) {
        if (strncmp((char *)option, "BDADDR", 6) == 0) {
            iwrap_hexstrtobin((char *)value, 0, iwrap_module_mac.address, 0);
        } else if (strncmp((char *)option, "NAME", 4) == 0) {
            // rename the device if it hasn't been configured yet
            if (strcmp(value, "Keyglove") == 0) {
                // rename device to include BT MAC address for unique name
                char *cmdName = (char *)malloc(30);
                if (cmdName) {
                    strcpy(cmdName, "SET BT NAME Keyglove 000000");
                    cmdName[21] = (iwrap_module_mac.address[3] / 0x10) + 48 + ((iwrap_module_mac.address[3] / 0x10) / 10 * 7);
                    cmdName[22] = (iwrap_module_mac.address[3] & 0x0f) + 48 + ((iwrap_module_mac.address[3] & 0x0f) / 10 * 7);
                    cmdName[23] = (iwrap_module_mac.address[4] / 0x10) + 48 + ((iwrap_module_mac.address[4] / 0x10) / 10 * 7);
                    cmdName[24] = (iwrap_module_mac.address[4] & 0x0f) + 48 + ((iwrap_module_mac.address[4] & 0x0f) / 10 * 7);
                    cmdName[25] = (iwrap_module_mac.address[5] / 0x10) + 48 + ((iwrap_module_mac.address[5] / 0x10) / 10 * 7);
                    cmdName[26] = (iwrap_module_mac.address[5] & 0x0f) + 48 + ((iwrap_module_mac.address[5] & 0x0f) / 10 * 7);
                    iwrap_send_command(cmdName, iwrap_mode);
                    free(cmdName);
                } else {
                    // this should NEVER happen, but if it does, I want to know
                    uint8_t payload[2] = { KG_PROTOCOL_ERROR_NULL_POINTER & 0xFF, KG_PROTOCOL_ERROR_NULL_POINTER >> 8 };
                    skipPacket = 0;
                    if (kg_evt_protocol_error) skipPacket = kg_evt_protocol_error(KG_PROTOCOL_ERROR_NULL_POINTER);
                    if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
                    return;
                }
            }
        } else if (strncmp((char *)option, "PAGE", 4) == 0) {
            iwrap_page_mode = value[0] - 0x30;
        } else if (strncmp((char *)option, "PAIR", 4) == 0) {
            iwrap_address_t remote_mac;
            iwrap_hexstrtobin((char *)value, 0, remote_mac.address, 0);

            // make sure we allocate memory for the connection map entry
            if (iwrap_connection_map[iwrap_pairings] == 0) {
                iwrap_connection_map[iwrap_pairings] = (iwrap_pairing_t *)malloc(sizeof(iwrap_pairing_t));
            }
            if (iwrap_connection_map[iwrap_pairings] != 0) {
                memset(iwrap_connection_map[iwrap_pairings], 0xFF, sizeof(iwrap_pairing_t)); // 0xFF is "no link ID"
                memcpy(&(iwrap_connection_map[iwrap_pairings] -> mac), &remote_mac, sizeof(iwrap_address_t));
                iwrap_connection_map[iwrap_pairings] -> active_links = 0;
                iwrap_connection_map[iwrap_pairings] -> profiles_supported = 0x3F; // assume everything
                iwrap_connection_map[iwrap_pairings] -> profiles_active = 0x00; // assume nothing
                iwrap_pairings++;
            } else {
                // this should NEVER happen, but if it does, I want to know
                uint8_t payload[2] = { KG_PROTOCOL_ERROR_NULL_POINTER & 0xFF, KG_PROTOCOL_ERROR_NULL_POINTER >> 8 };
                skipPacket = 0;
                if (kg_evt_protocol_error) skipPacket = kg_evt_protocol_error(KG_PROTOCOL_ERROR_NULL_POINTER);
                if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
                return;
            }
        }
    }
}

/**
 * @brief iWRAP "CONNECT" event handler
 * @param[in] link_id Link ID assigned to this completed outgoing connection
 * @param[in] type Type of link opened (e.g. "HID", "HFP", etc.)
 * @param[in] target RFCOMM or L2CAP channel for this connection
 * @param[in] mac MAC address of remote device
 */
void my_iwrap_evt_connect(uint8_t link_id, const char *type, uint16_t target, const iwrap_address_t *mac) {
    if (iwrap_pending_call_link_id == link_id) {
        if (iwrap_pending_calls) iwrap_pending_calls--;
        if (iwrap_state == IWRAP_STATE_PENDING_CALL) iwrap_state = IWRAP_STATE_IDLE;
        iwrap_pending_call_link_id = 0xFF;
    }
    iwrap_active_connections++;
    bluetoothPendingConnectionStatus |= (1 << link_id);
    bluetoothActiveLinkMask |= (1 << link_id);
    add_mapped_connection(link_id, mac, type, target);
}

/**
 * @brief iWRAP "INQUIRY_EXTENDED" event handler
 * @param[in] mac MAC address of remote device
 * @param[in] length Length of raw extended inquiry data
 * @param[in] data Raw extended inquiry data
 */
void my_iwrap_evt_inquiry_extended(const iwrap_address_t *mac, uint8_t length, const uint8_t *data) {
    uint8_t payload[13];
    uint32_t bt_class = 0; // filler
    payload[0] = mac -> address[5];
    payload[1] = mac -> address[4];
    payload[2] = mac -> address[3];
    payload[3] = mac -> address[2];
    payload[4] = mac -> address[1];
    payload[5] = mac -> address[0];
    payload[6] = bt_class & 0xFF;
    payload[7] = (bt_class >> 8) & 0xFF;
    payload[8] = (bt_class >> 16) & 0xFF;
    payload[9] = 0; // RSSI
    payload[10] = 2; // inquiry status
    payload[11] = find_pairing_from_mac(mac);
    payload[12] = 0; // no NAME yet

    // send kg_evt_bluetooth_inquiry_response(...) event
    skipPacket = 0;
    if (kg_evt_bluetooth_inquiry_response) skipPacket = kg_evt_bluetooth_inquiry_response(payload + 0, payload + 6, payload[9], payload[10], payload[11], payload[12], 0);
    if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 13, KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_INQUIRY_RESPONSE, payload);
}

/**
 * @brief iWRAP "INQUIRY_PARTIAL" event handler
 * @param[in] mac MAC address of remote device
 * @param[in] class_of_device Bluetooth class of device
 * @param[in] cached_name Null-ternimated device name string
 * @param[in] rssi RSSI (signal strength) of remote device
 */
void my_iwrap_evt_inquiry_partial(const iwrap_address_t *mac, uint32_t class_of_device, const char *cached_name, int8_t rssi) {
    uint8_t payload[13];
    payload[0] = mac -> address[5];
    payload[1] = mac -> address[4];
    payload[2] = mac -> address[3];
    payload[3] = mac -> address[2];
    payload[4] = mac -> address[1];
    payload[5] = mac -> address[0];
    payload[6] = class_of_device & 0xFF;
    payload[7] = (class_of_device >> 8) & 0xFF;
    payload[8] = (class_of_device >> 16) & 0xFF;
    payload[9] = rssi; // RSSI
    payload[10] = 1; // inquiry status
    payload[11] = find_pairing_from_mac(mac);
    payload[12] = 0; // no NAME yet

    // send kg_evt_bluetooth_inquiry_response(...) event
    skipPacket = 0;
    if (kg_evt_bluetooth_inquiry_response) skipPacket = kg_evt_bluetooth_inquiry_response(payload + 0, payload + 6, payload[9], payload[10], payload[11], payload[12], 0);
    if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 13, KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_INQUIRY_RESPONSE, payload);
}

/**
 * @brief iWRAP "NAME" event handler
 * @param[in] mac MAC address of remote device
 * @param[in] friendly_name Null-ternimated device name string
 */
void my_iwrap_evt_name(const iwrap_address_t *mac, const char *friendly_name) {
    uint8_t name_len = 0;
    if (friendly_name) name_len = strlen(friendly_name);
    uint8_t payload[13 + name_len];
    payload[0] = mac -> address[5];
    payload[1] = mac -> address[4];
    payload[2] = mac -> address[3];
    payload[3] = mac -> address[2];
    payload[4] = mac -> address[1];
    payload[5] = mac -> address[0];
    payload[6] = 0;
    payload[7] = 0;
    payload[8] = 0;
    payload[9] = 0; // RSSI
    payload[10] = 4; // inquiry status
    payload[11] = find_pairing_from_mac(mac);
    payload[12] = name_len;
    if (name_len) memcpy(payload + 13, friendly_name, name_len);

    // send kg_evt_bluetooth_inquiry_response(...) event
    skipPacket = 0;
    if (kg_evt_bluetooth_inquiry_response) skipPacket = kg_evt_bluetooth_inquiry_response(payload + 0, payload + 6, payload[9], payload[10], payload[11], payload[12], name_len ? payload + 13 : 0);
    if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 13 + name_len, KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_INQUIRY_RESPONSE, payload);
}

/*x*
 * @brief iWRAP "NAME ERROR" event handler
 * @param[in] error_code Numeric error code
 * @param[in] mac MAC address of remote device
 * @param[in] message Description string for error
 *x/
void my_iwrap_evt_name_error(uint16_t error_code, const iwrap_address_t *mac, const char *message) {
}*/

/**
 * @brief iWRAP "NO CARRIER" event handler
 * @param[in] link_id Link ID of connection which just closed
 * @param[in] error_code Numeric error code
 * @param[in] message Description string for error
 */
void my_iwrap_evt_no_carrier(uint8_t link_id, uint16_t error_code, const char *message) {
    bluetoothPendingConnectionStatus &= ~(1 << link_id);
    bluetoothActiveLinkMask &= ~(1 << link_id);
    
    if (iwrap_pending_call_link_id == link_id) {
        if (iwrap_pending_calls) iwrap_pending_calls--;
        if (iwrap_state == IWRAP_STATE_PENDING_CALL) iwrap_state = IWRAP_STATE_IDLE;
        iwrap_pending_call_link_id = 0xFF;
    }
    if (remove_mapped_connection(link_id) != 0xFF) {
        // only update if the connection was already mapped
        // (i.e. not already closed or a failed outgoing connection attempt)
        if (iwrap_active_connections) iwrap_active_connections--;
    }

    // send kg_evt_bluetooth_connection_closed()
    skipPacket = 0;
    uint8_t payload[3] = { link_id, error_code & 0xFF, error_code >> 8 };
    if (kg_evt_bluetooth_connection_closed) skipPacket = kg_evt_bluetooth_connection_closed(link_id, error_code);
    if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 3, KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_CONNECTION_CLOSED, payload);
}

/**
 * @brief iWRAP "PAIR" event handler
 * @param[in] mac MAC address of remote device
 * @param[in] key_type Key type used during pairing
 * @param[in] link_key Encryption key generated during pairing
 */
void my_iwrap_evt_pair(const iwrap_address_t *mac, uint8_t key_type, const uint8_t *link_key) {
    my_iwrap_rsp_pair(mac, 0);
}

/**
 * @brief iWRAP "READY" event handler
 */
void my_iwrap_evt_ready() {
    iwrap_state = IWRAP_STATE_UNKNOWN;
}

/**
 * @brief iWRAP "RING" event handler
 * @param[in] link_id Link ID assigned to incoming connection
 * @param[in] mac MAC address of remote device
 * @param[in] channel RFCOMM or L2CAP channel for connection
 * @param[in] profile Profile used for connection (e.g. "HID", "AVRCP", etc.)
 */
void my_iwrap_evt_ring(uint8_t link_id, const iwrap_address_t *mac, uint16_t channel, const char *profile) {
    bluetoothPendingConnectionStatus |= (1 << link_id);
    bluetoothActiveLinkMask |= (1 << link_id);
    add_mapped_connection(link_id, mac, profile, channel);
}

/* ============================================================================
 * GENERAL HELPER FUNCTIONS
 * ========================================================================= */

/**
 * @brief Locate pairing index in connection map table from MAC address
 * @param[in] mac MAC address of device to find
 * @return Index if found, -1 (0xFF) otherwise
 */
uint8_t find_pairing_from_mac(const iwrap_address_t *mac) {
    uint8_t i;
    for (i = 0; i < iwrap_pairings; i++) {
        if (iwrap_connection_map[i]) {
            if (memcmp(&(iwrap_connection_map[i] -> mac), mac, sizeof(iwrap_address_t)) == 0) return i;
        } else {
            // this should NEVER happen, but if it does, I want to know
            uint8_t payload[2] = { KG_PROTOCOL_ERROR_NULL_POINTER & 0xFF, KG_PROTOCOL_ERROR_NULL_POINTER >> 8 };
            skipPacket = 0;
            if (kg_evt_protocol_error) skipPacket = kg_evt_protocol_error(KG_PROTOCOL_ERROR_NULL_POINTER);
            if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
            return 0xFF;
        }
    }
    return i >= iwrap_pairings ? 0xFF : i;
}

/**
 * @brief Locate pairing index in connection map table from link ID
 * @param[in] link_id Link ID to use for reference
 * @return Index if found, -1 (0xFF) otherwise
 */
uint8_t find_pairing_from_link_id(uint8_t link_id) {
    // NOTE: This implementation walks through the iwrap_pairing_t struct memory
    // starting at the first link ID (A2DP #1) and going all the way to the end (if
    // necessary). This means it is contingent on the structure keeping this memory
    // arrangement, where the last set of whatever is contained there is always the
    // set of link IDs.
    
    // MODIFIED FOR KEYGLOVE: starts at AVRCP, not A2DP #1
    
    uint8_t i, j, *idptr;
    for (i = 0; i < iwrap_pairings; i++) {
        if (iwrap_connection_map[i]) {
            idptr = &(iwrap_connection_map[i] -> link_avrcp);
            for (j = 6; j < sizeof(iwrap_pairing_t); j++, idptr++) if (idptr[0] == link_id) return i;
        } else {
            // this should NEVER happen, but if it does, I want to know
            uint8_t payload[2] = { KG_PROTOCOL_ERROR_NULL_POINTER & 0xFF, KG_PROTOCOL_ERROR_NULL_POINTER >> 8 };
            skipPacket = 0;
            if (kg_evt_protocol_error) skipPacket = kg_evt_protocol_error(KG_PROTOCOL_ERROR_NULL_POINTER);
            if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
            return 0xFF;
        }
    }
    return 0xFF;
}

/**
 * @brief Add new connection to connection map table
 * @param[in] link_id Link ID of connection
 * @param[in] mac MAC address of connected device
 * @param[in] mode Profile used for connection (e.g. "HID", "RFCOMM", etc.)
 * @param[in] channel RFCOMM or L2CAP channel of connection
 */
void add_mapped_connection(uint8_t link_id, const iwrap_address_t *mac, const char *mode, uint16_t channel) {
    uint8_t pairing_index = find_pairing_from_mac(mac);

    // make sure we found a match (we SHOULD always match something, if we properly parsed the pairing data first)
    if (pairing_index == 0xFF) return; // uh oh
    
    // make sure the pointer exists
    if (!iwrap_connection_map[pairing_index]) {
        // this should NEVER happen, but if it does, I want to know
        uint8_t payload[2] = { KG_PROTOCOL_ERROR_NULL_POINTER & 0xFF, KG_PROTOCOL_ERROR_NULL_POINTER >> 8 };
        skipPacket = 0;
        if (kg_evt_protocol_error) skipPacket = kg_evt_protocol_error(KG_PROTOCOL_ERROR_NULL_POINTER);
        if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
        return;
    }

    // updated connected device count and overall active link count for this device
    if (!iwrap_connection_map[pairing_index] -> active_links) iwrap_connected_devices++;
    iwrap_connection_map[pairing_index] -> active_links++;

    // build connection status event payload in case we need it later
    uint8_t payload[10] = { link_id, 0, 0, 0, 0, 0, 0, pairing_index, 0, 2 };
    payload[1] = iwrap_connection_map[pairing_index] -> mac.address[5]; // little-endian byte order
    payload[2] = iwrap_connection_map[pairing_index] -> mac.address[4];
    payload[3] = iwrap_connection_map[pairing_index] -> mac.address[3];
    payload[4] = iwrap_connection_map[pairing_index] -> mac.address[2];
    payload[5] = iwrap_connection_map[pairing_index] -> mac.address[1];
    payload[6] = iwrap_connection_map[pairing_index] -> mac.address[0];

                // add link ID to connection map
    /*if (strcmp(mode, "A2DP") == 0) {
        if (iwrap_connection_map[pairing_index] -> link_a2dp1 == 0xFF) {
            iwrap_connection_map[pairing_index] -> link_a2dp1 = link_id;
        } else {
            iwrap_connection_map[pairing_index] -> link_a2dp2 = link_id;
        }
    } else*/ if (strcmp(mode, "AVRCP") == 0) {
        iwrap_connection_map[pairing_index] -> link_avrcp = link_id;
        iwrap_connection_map[pairing_index] -> profiles_active |= BLUETOOTH_PROFILE_MASK_AVRCP;
        payload[8] = BLUETOOTH_PROFILE_MASK_AVRCP;
        interfaceBT2AVRCPReady = true;
        bluetoothAVRCPDeviceIndex = pairing_index;
    } else if (strcmp(mode, "HFP") == 0) {
        iwrap_connection_map[pairing_index] -> link_hfp = link_id;
        iwrap_connection_map[pairing_index] -> profiles_active |= BLUETOOTH_PROFILE_MASK_HFP;
        payload[8] = BLUETOOTH_PROFILE_MASK_HFP;
        interfaceBT2HFPReady = false;
        bluetoothHFPDeviceIndex = pairing_index;
    //} else if (strcmp(mode, "HFPAG") == 0) {
    //    iwrap_connection_map[pairing_index] -> link_hfpag = link_id;
    } else if (strcmp(mode, "HID") == 0) {
        if (channel == 0x11) {
            iwrap_connection_map[pairing_index] -> link_hid_control = link_id;
            iwrap_connection_map[pairing_index] -> profiles_active |= BLUETOOTH_PROFILE_MASK_HID_CONTROL;
            payload[8] = BLUETOOTH_PROFILE_MASK_HID_CONTROL;
        } else {
            iwrap_connection_map[pairing_index] -> link_hid_interrupt = link_id;
            iwrap_connection_map[pairing_index] -> profiles_active |= BLUETOOTH_PROFILE_MASK_HID_INTERRUPT;
            payload[8] = BLUETOOTH_PROFILE_MASK_HID_INTERRUPT;
            interfaceBT2HIDReady = true;
            interfaceBT2RawHIDReady = true;
            bluetoothHIDDeviceIndex = pairing_index;
            bluetoothRawHIDDeviceIndex = pairing_index;
        }
    //} else if (strcmp(mode, "HSP") == 0) {
    //    iwrap_connection_map[pairing_index] -> link_hsp = link_id;
    //} else if (strcmp(mode, "HSPAG") == 0) {
    //    iwrap_connection_map[pairing_index] -> link_hspag = link_id;
    } else if (strcmp(mode, "IAP") == 0) {
        iwrap_connection_map[pairing_index] -> link_iap = link_id;
        iwrap_connection_map[pairing_index] -> profiles_active |= BLUETOOTH_PROFILE_MASK_IAP;
        payload[8] = BLUETOOTH_PROFILE_MASK_IAP;
        interfaceBT2IAPReady = true;
        bluetoothIAPDeviceIndex = pairing_index;
    } else if (strcmp(mode, "RFCOMM") == 0) {
        // probably SPP, possibly other RFCOMM-based connections
        if (channel == 1) {
            iwrap_connection_map[pairing_index] -> link_spp = link_id;
            iwrap_connection_map[pairing_index] -> profiles_active |= BLUETOOTH_PROFILE_MASK_SPP;
            payload[8] = BLUETOOTH_PROFILE_MASK_SPP;
            interfaceBT2SerialReady = true;
            bluetoothSPPDeviceIndex = pairing_index;
        //} else {
            //printf("RFCOMM link on channel %d, profile unknown\n", channel);
        }
    }
    
    // check to see if we need to send a connection update event
    if (bluetoothPendingConnectionStatus & (1 << link_id)) {
        // send kg_evt_bluetooth_connection_status(...)
        skipPacket = 0;
        if (kg_evt_bluetooth_connection_status) skipPacket = kg_evt_bluetooth_connection_status(payload[0], payload + 1, payload[7], payload[8], payload[9]);
        if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 10, KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_CONNECTION_STATUS, payload);
        bluetoothPendingConnectionStatus &= ~(1 << link_id);
    }
}

/**
 * @brief Remove connection from connection map table
 * @param[in] link_id Link ID of connection
 * @return Pairing index of device in table if found and removed, -1 (0xFF) otherwise
 */
uint8_t remove_mapped_connection(uint8_t link_id) {
    uint8_t i, j;
    for (i = 0; i < iwrap_pairings; i++) {
        if (iwrap_connection_map[i]) {
            //if (iwrap_connection_map[i] -> link_a2dp1 == link_id) { iwrap_connection_map[i] -> link_a2dp1 = 0xFF; break; }
            //if (iwrap_connection_map[i] -> link_a2dp2 == link_id) { iwrap_connection_map[i] -> link_a2dp2 = 0xFF; break; }
            if (iwrap_connection_map[i] -> link_avrcp == link_id) {
                iwrap_connection_map[i] -> link_avrcp = 0xFF;
                iwrap_connection_map[i] -> profiles_active &= ~(BLUETOOTH_PROFILE_MASK_AVRCP);
                if (bluetoothAVRCPDeviceIndex == i) {
                    interfaceBT2AVRCPReady = false;
                    for (j = 0; j < iwrap_pairings; j++) {
                        if (iwrap_connection_map[j] && iwrap_connection_map[j] -> link_avrcp != 0xFF) {
                            // switch AVRCP device to a different entry
                            interfaceBT2AVRCPReady = true;
                            bluetoothAVRCPDeviceIndex = j;
                            break;
                        }
                    }
                }
                break;
            }
            if (iwrap_connection_map[i] -> link_hfp == link_id) {
                iwrap_connection_map[i] -> link_hfp = 0xFF;
                iwrap_connection_map[i] -> profiles_active &= ~(BLUETOOTH_PROFILE_MASK_HFP);
                if (bluetoothHFPDeviceIndex == i) {
                    interfaceBT2HFPReady = false;
                    for (j = 0; j < iwrap_pairings; j++) {
                        if (iwrap_connection_map[j] && iwrap_connection_map[j] -> link_hfp != 0xFF) {
                            // switch HFP device to a different entry
                            interfaceBT2HFPReady = true;
                            bluetoothHFPDeviceIndex = j;
                            break;
                        }
                    }
                }
                break;
            }
            //if (iwrap_connection_map[i] -> link_hfpag == link_id) { iwrap_connection_map[i] -> link_hfpag = 0xFF; break; }
            if (iwrap_connection_map[i] -> link_hid_control == link_id) {
                iwrap_connection_map[i] -> link_hid_control = 0xFF;
                iwrap_connection_map[i] -> profiles_active &= ~(BLUETOOTH_PROFILE_MASK_HID_CONTROL);
                if (bluetoothHIDDeviceIndex == i) {
                    interfaceBT2HIDReady = false;
                    interfaceBT2RawHIDReady = false;
                    for (j = 0; j < iwrap_pairings; j++) {
                        if (iwrap_connection_map[j] && iwrap_connection_map[j] -> link_hid_control != 0xFF && iwrap_connection_map[j] -> link_hid_interrupt != 0xFF) {
                            // switch HID device to a different entry
                            interfaceBT2HIDReady = true;
                            interfaceBT2RawHIDReady = true;
                            bluetoothHIDDeviceIndex = j;
                            bluetoothRawHIDDeviceIndex = j;
                            break;
                        }
                    }
                }
                break;
            }
            if (iwrap_connection_map[i] -> link_hid_interrupt == link_id) {
                iwrap_connection_map[i] -> link_hid_interrupt = 0xFF;
                iwrap_connection_map[i] -> profiles_active &= ~(BLUETOOTH_PROFILE_MASK_HID_INTERRUPT);
                if (bluetoothHIDDeviceIndex == i) {
                    interfaceBT2HIDReady = false;
                    interfaceBT2RawHIDReady = false;
                    for (j = 0; j < iwrap_pairings; j++) {
                        if (iwrap_connection_map[j] && iwrap_connection_map[j] -> link_hid_control != 0xFF && iwrap_connection_map[j] -> link_hid_interrupt != 0xFF) {
                            // switch HID device to a different entry
                            interfaceBT2HIDReady = true;
                            interfaceBT2RawHIDReady = true;
                            bluetoothHIDDeviceIndex = j;
                            bluetoothRawHIDDeviceIndex = j;
                            break;
                        }
                    }
                }
                break;
            }
            //if (iwrap_connection_map[i] -> link_hsp == link_id) { iwrap_connection_map[i] -> link_hsp = 0xFF; break; }
            //if (iwrap_connection_map[i] -> link_hspag == link_id) { iwrap_connection_map[i] -> link_hspag = 0xFF; break; }
            if (iwrap_connection_map[i] -> link_iap == link_id) {
                iwrap_connection_map[i] -> link_iap = 0xFF;
                iwrap_connection_map[i] -> profiles_active &= ~(BLUETOOTH_PROFILE_MASK_IAP);
                if (bluetoothIAPDeviceIndex == i) {
                    interfaceBT2IAPReady = false;
                    for (j = 0; j < iwrap_pairings; j++) {
                        if (iwrap_connection_map[j] && iwrap_connection_map[j] -> link_iap != 0xFF) {
                            // switch IAP device to a different entry
                            interfaceBT2IAPReady = true;
                            bluetoothIAPDeviceIndex = j;
                            break;
                        }
                    }
                }
                break;
            }
            if (iwrap_connection_map[i] -> link_spp == link_id) {
                iwrap_connection_map[i] -> link_spp = 0xFF;
                iwrap_connection_map[i] -> profiles_active &= ~(BLUETOOTH_PROFILE_MASK_SPP);
                if (bluetoothSPPDeviceIndex == i) {
                    interfaceBT2SerialReady = false;
                    for (j = 0; j < iwrap_pairings; j++) {
                        if (iwrap_connection_map[j] && iwrap_connection_map[j] -> link_spp != 0xFF) {
                            // switch SPP device to a different entry
                            interfaceBT2SerialReady = true;
                            bluetoothSPPDeviceIndex = j;
                            break;
                        }
                    }
                }
                break;
            }
        } else {
            // this should NEVER happen, but if it does, I want to know
            uint8_t payload[2] = { KG_PROTOCOL_ERROR_NULL_POINTER & 0xFF, KG_PROTOCOL_ERROR_NULL_POINTER >> 8 };
            skipPacket = 0;
            if (kg_evt_protocol_error) skipPacket = kg_evt_protocol_error(KG_PROTOCOL_ERROR_NULL_POINTER);
            if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 2, KG_PACKET_CLASS_PROTOCOL, KG_PACKET_ID_EVT_PROTOCOL_ERROR, payload);
            return 0xFF;
        }
    }
    
    // check to make sure we found the link ID in the map
    if (i < iwrap_pairings) {
        // updated connected device count and overall active link count for this device
        if (iwrap_connection_map[i] -> active_links) {
            iwrap_connection_map[i] -> active_links--;
            if (!iwrap_connection_map[i] -> active_links) iwrap_connected_devices--;
        }
        return i;
    }
    
    // not found, return 0xFF
    return 0xFF;
}

/* ============================================================================
 * PLATFORM-SPECIFIC HELPER FUNCTIONS
 * ========================================================================= */

/**
 * @brief iWRAP serial debug output function
 * @param[in] str Null-terminated string to send to host
 * @return Result of output function call
 */
int serial_out(const char *str) {
    // debug output to host
    return Serial.print(str);
}

/**
 * @brief iWRAP serial debug output function
 * @param[in] str Null-terminated flash-stored string to send to host
 * @return Result of output function call
 */
int serial_out(const __FlashStringHelper *str) {
    // debug output to host
    return Serial.print(str);
}

/**
 * @brief iWRAP module output function
 * @param[in] len Length of data to send to module
 * @param[in] data Data to send to module
 * @return Result of output function call
 */
int iwrap_out(int len, unsigned char *data) {
    // iWRAP output to module goes through Serial1 (hardware UART)
    return BT2Serial.write(data, len);
}

/* ============================= */
/* KGAPI COMMAND IMPLEMENTATIONS */
/* ============================= */

/**
 * @brief Get current mode for Bluetooth subsystem
 * @param[out] mode Current Bluetooth mode
 * @return Result code (0=success)
 */
uint16_t kg_cmd_bluetooth_get_mode(uint8_t *mode) {
    if (interfaceBT2Ready) {
        *mode = bluetoothMode;
        return 0; // success
    } else {
        *mode = 0xFF;
        return KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY;
    }
}

/**
 * @brief Set new mode for Bluetooth subsystem
 * @param[in] mode New Bluetooth mode to set
 * @return Result code (0=success)
 */
uint16_t kg_cmd_bluetooth_set_mode(uint8_t mode) {
    uint8_t curId = 0;
    char cmdClose[] = "CLOSE 00";

    // validate mode
    if (mode > KG_BLUETOOTH_MODE_MAX) {
        return KG_PROTOCOL_ERROR_PARAMETER_RANGE;
    } else {
        if (interfaceBT2Ready) {
            // update mode if interface is ready and mode is actually different
            if (mode != bluetoothMode) {
                // handle appropriate reconfiguration due to mode switch if interface is ready
                // (if it isn't ready, this will happen automatically based on current mode setting when it becomes ready)
                iwrap_autocall_target = 0;
                switch (mode) {
                    case KG_BLUETOOTH_MODE_DISABLED:
                        // become invisible
                        iwrap_send_command("SET BT PAGE 0", iwrap_mode);
                        iwrap_page_mode = 0;
                        
                        // close any open links
                        while (bluetoothActiveLinkMask) {
                            if (curId > 9) {
                                cmdClose[6] = '1';
                                cmdClose[7] = curId + 38;
                            } else {
                                cmdClose[6] = curId + 48;
                                cmdClose[7] = 0;
                            }
                            iwrap_send_command(cmdClose, iwrap_mode);
                            bluetoothActiveLinkMask >>= 1;
                            curId++;
                        }
                        break;
                        
                    case KG_BLUETOOTH_MODE_VISIBLE:
                        // set discoverable/connectable regardless
                        if (iwrap_page_mode != 3) {
                            iwrap_send_command("SET BT PAGE 3", iwrap_mode);
                            iwrap_page_mode = 3;
                        }
                        break;
                        
                    case KG_BLUETOOTH_MODE_AUTOCALL:
                        // set autocall target to 1 device
                        iwrap_autocall_target = 1;
                        
                        // try (re)connection instantly if possible
                        bluetoothTock = keygloveTock - 10;
                        
                        // ...fall through ON PURPOSE!

                    case KG_BLUETOOTH_MODE_MANUAL:
                        if (iwrap_pairings > 0) {
                            // not connected, but paired, so default to non-discoverable/connectable
                            if (iwrap_page_mode != 2) {
                                iwrap_send_command("SET BT PAGE 2", iwrap_mode);
                                iwrap_page_mode = 2;
                            }
                        } else {
                            // not connected or paired, so default to discoverable/connectable
                            if (iwrap_page_mode != 4) {
                                iwrap_send_command("SET BT PAGE 4", iwrap_mode);
                                iwrap_page_mode = 4;
                            }
                        }
                        break;
                }
                bluetoothMode = (bluetooth_mode_t)mode;
            }

            // send kg_evt_bluetooth_mode packet (if we aren't setting it from an API command)
            if (!inBinPacket) {
                uint8_t payload[1] = { bluetoothMode };
                skipPacket = 0;
                if (kg_evt_bluetooth_mode) skipPacket = kg_evt_bluetooth_mode(payload[0]);
                if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 1, KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_MODE, payload);
            }
        } else {
            return KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY;
        }
    }

    return 0; // success
}

/**
 * @brief Reset Bluetooth subsystem
 * @return Result code (0=success)
 */
uint16_t kg_cmd_bluetooth_reset() {
    if (interfaceBT2Ready) {
        interfaceBT2Ready = false;
        iwrap_initialized = 0;
        
        // send RESET command
        iwrap_send_command("RESET", iwrap_mode);
        return 0; // success
    } else {
        return KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY;
    }
}

/**
 * @brief Get local Bluetooth MAC address
 * @param[out] address Local six-byte Bluetooth MAC address
 * @return Result code (0=success)
 */
uint16_t kg_cmd_bluetooth_get_mac(uint8_t *address) {
    if (interfaceBT2Ready) {
        address[0] = iwrap_module_mac.address[5]; // little-endian byte order
        address[1] = iwrap_module_mac.address[4];
        address[2] = iwrap_module_mac.address[3];
        address[3] = iwrap_module_mac.address[2];
        address[4] = iwrap_module_mac.address[1];
        address[5] = iwrap_module_mac.address[0];
        return 0; // success
    } else {
        return KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY;
    }
}

/**
 * @brief Get a list of all paired devices
 * @param[out] count Number of paired devices
 * @return Result code (0=success)
 */
uint16_t kg_cmd_bluetooth_get_pairings(uint8_t *count) {
    if (interfaceBT2Ready) {
        *count = iwrap_pairings;
                
        // sending pairing entry events
        uint8_t payload[18];
        for (uint8_t i = 0; i < iwrap_pairings; i++) {
            if (iwrap_connection_map[i]) {
                // build event (uint8_t index, macaddr_t address, uint8_t priority, uint8_t profiles_supported, uint8_t profiles_active, uint8_t[] handle_list)
                payload[0] = i;    // pairing index
                payload[1] = iwrap_connection_map[i] -> mac.address[5];
                payload[2] = iwrap_connection_map[i] -> mac.address[4];
                payload[3] = iwrap_connection_map[i] -> mac.address[3];
                payload[4] = iwrap_connection_map[i] -> mac.address[2];
                payload[5] = iwrap_connection_map[i] -> mac.address[1];
                payload[6] = iwrap_connection_map[i] -> mac.address[0];
                payload[7] = iwrap_connection_map[i] -> priority;
                payload[8] = iwrap_connection_map[i] -> profiles_supported;
                payload[9] = iwrap_connection_map[i] -> profiles_active;
                payload[10] = 0;
                for (uint8_t j = 0; j < 6; j++) {
                    if (payload[9] & (1 << j)) {
                        uint8_t v;
                        payload[10]++;
                        if (j == 0) v = iwrap_connection_map[i] -> link_hid_control;
                        else if (j == 1) v = iwrap_connection_map[i] -> link_hid_interrupt;
                        else if (j == 2) v = iwrap_connection_map[i] -> link_spp;
                        else if (j == 3) v = iwrap_connection_map[i] -> link_iap;
                        else if (j == 4) v = iwrap_connection_map[i] -> link_hfp;
                        else if (j == 5) v = iwrap_connection_map[i] -> link_avrcp;
                        payload[10 + payload[10]] = v;
                    }
                }

                // queue event
                skipPacket = 0;
                if (kg_evt_bluetooth_pairing_status) skipPacket = kg_evt_bluetooth_pairing_status(payload[0], payload + 1, payload[7], payload[8], payload[9], payload[10], payload + 11);
                if (!skipPacket) queue_keyglove_packet(KG_PACKET_TYPE_EVENT, 11 + payload[10], KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_PAIRING_STATUS, payload);
            } else {
                // this should NEVER happen, but if it does, I want to know
                return KG_PROTOCOL_ERROR_NULL_POINTER;
            }
        }
        return 0; // success
    } else {
        return KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY;
    }
}

/**
 * @brief Perform Bluetooth inquiry to locate nearby devices
 * @param[in] duration Number of seconds to run discovery process
 * @return Result code (0=success)
 */
uint16_t kg_cmd_bluetooth_discover(uint8_t duration) {
    if (interfaceBT2Ready) {
        // validate duration
        if (duration < 5 || duration > 30) {
            return KG_PROTOCOL_ERROR_PARAMETER_RANGE;
        }

        // check for other pending operations
        if (iwrap_state == IWRAP_STATE_PENDING_CALL || iwrap_state == IWRAP_STATE_PENDING_INQUIRY || iwrap_state == IWRAP_STATE_PENDING_PAIR) {
            // can't initiate an outgoing inquiry when there is another pending BT radio operation
            return KG_BLUETOOTH_ERROR_INTERFACE_BUSY;
        } else {
            char cmd[] = "INQUIRY 00 NAME";
            if (duration < 10) {
                cmd[9] = duration + 0x30;
            } else {
                cmd[8] = ((duration / 10) % 10) + 0x30;
                cmd[9] = (duration % 10) + 0x30;
            }
            iwrap_send_command(cmd, iwrap_mode);
            return 0; // success
        }
    } else {
        return KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY;
    }
}

/**
 * @brief Initiate pairing request to remote device
 * @param[in] address Six-byte Bluetooth MAC address of remote device to pair with
 * @return Result code (0=success)
 */
uint16_t kg_cmd_bluetooth_pair(uint8_t *address) {
    if (interfaceBT2Ready) {
        // check for other pending operations
        if (iwrap_state == IWRAP_STATE_PENDING_CALL || iwrap_state == IWRAP_STATE_PENDING_INQUIRY || iwrap_state == IWRAP_STATE_PENDING_PAIR) {
            // can't initiate an outgoing pairing request when there is another pending BT radio operation
            return KG_BLUETOOTH_ERROR_INTERFACE_BUSY;
        } else {
            char cmd[] = "PAIR 00:00:00:00:00:00";
            char *cptr = cmd + 5;
            iwrap_bintohexstr((uint8_t *)(address), 6, &cptr, ':', 0);
            iwrap_send_command(cmd, iwrap_mode);
            iwrap_state = IWRAP_STATE_PENDING_PAIR;
            return 0; // success
        }
    } else {
        return KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY;
    }
}

/**
 * @brief Remove a specific pairing entry
 * @param[in] index Index of pairing to delete
 * @return Result code (0=success)
 */
uint16_t kg_cmd_bluetooth_delete_pairing(uint8_t index) {
    if (interfaceBT2Ready) {
        // validate pairing index
        if (index >= iwrap_pairings) {
            return KG_PROTOCOL_ERROR_PARAMETER_RANGE;
        }
        if (iwrap_connection_map[index] == 0) {
            // this should NEVER happen, but if it does, I want to know
            return KG_PROTOCOL_ERROR_NULL_POINTER;
        }
        char cmd[] = "SET BT PAXR 00:00:00:00:00:00";
        char *cptr = cmd + 12;
        iwrap_bintohexstr((uint8_t *)(iwrap_connection_map[index] -> mac.address), 6, &cptr, ':', 0);
        iwrap_send_command(cmd, iwrap_mode);
        iwrap_pairings--;
        free(iwrap_connection_map[index]);
        for (uint8_t i = index; i < iwrap_pairings; i++) {
            iwrap_connection_map[i] = iwrap_connection_map[i + 1];
        }
        iwrap_connection_map[iwrap_pairings] = 0;
        
        // adjust active device indexes if necessary
        if (bluetoothSPPDeviceIndex >= index) bluetoothSPPDeviceIndex--;
        if (bluetoothIAPDeviceIndex >= index) bluetoothIAPDeviceIndex--;
        if (bluetoothHIDDeviceIndex >= index) bluetoothHIDDeviceIndex--;
        if (bluetoothRawHIDDeviceIndex >= index) bluetoothRawHIDDeviceIndex--;
        if (bluetoothHFPDeviceIndex >= index) bluetoothHFPDeviceIndex--;
        if (bluetoothAVRCPDeviceIndex >= index) bluetoothAVRCPDeviceIndex--;
        
        return 0; // success
    } else {
        return KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY;
    }
}

/**
 * @brief Remove all pairing entries
 * @return Result code (0=success)
 */
uint16_t kg_cmd_bluetooth_clear_pairings() {
    if (interfaceBT2Ready) {
        iwrap_pairings = 0;
        iwrap_send_command("SET BT PAIR *", iwrap_mode);
        return 0; // success
    } else {
        return KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY;
    }
}

/**
 * @brief Get a list of all open or pending connections
 * @param[out] count Number of open or pending connections
 * @return Result code (0=success)
 */
uint16_t kg_cmd_bluetooth_get_connections(uint8_t *count) {
    if (interfaceBT2Ready) {
        *count = iwrap_active_connections;
        
        // queue connection status packets
        uint16_t activeLinks = bluetoothActiveLinkMask;
        uint8_t curId = 0, i;
        uint8_t pairing_index;
        while (activeLinks) {
            // build connection status event payload
            uint8_t payload[10] = { curId, 0, 0, 0, 0, 0, 0, 0, 0, 2 };
            for (i = 0; i < iwrap_pairings; i++) {
                if (iwrap_connection_map[i]) {
                    if (iwrap_connection_map[i] -> link_avrcp == curId) {
                        payload[8] = BLUETOOTH_PROFILE_MASK_AVRCP;
                        break;
                    }
                    if (iwrap_connection_map[i] -> link_hfp == curId) {
                        payload[8] = BLUETOOTH_PROFILE_MASK_HFP;
                        break;
                    }
                    if (iwrap_connection_map[i] -> link_hid_control == curId) {
                        payload[8] = BLUETOOTH_PROFILE_MASK_HID_CONTROL;
                        break;
                    }
                    if (iwrap_connection_map[i] -> link_hid_interrupt == curId) {
                        payload[8] = BLUETOOTH_PROFILE_MASK_HID_INTERRUPT;
                        break;
                    }
                    if (iwrap_connection_map[i] -> link_iap == curId) {
                        payload[8] = BLUETOOTH_PROFILE_MASK_IAP;
                        break;
                    }
                    if (iwrap_connection_map[i] -> link_spp == curId) {
                        payload[8] = BLUETOOTH_PROFILE_MASK_SPP;
                        break;
                    }
                } else {
                    // this should NEVER happen, but if it does, I want to know
                    return KG_PROTOCOL_ERROR_NULL_POINTER;
                }
            }
            if (i == iwrap_pairings) continue; // this shouldn't happen, ever! (we didn't find a link match)

            pairing_index = i;
            payload[1] = iwrap_connection_map[pairing_index] -> mac.address[5]; // little-endian byte order
            payload[2] = iwrap_connection_map[pairing_index] -> mac.address[4];
            payload[3] = iwrap_connection_map[pairing_index] -> mac.address[3];
            payload[4] = iwrap_connection_map[pairing_index] -> mac.address[2];
            payload[5] = iwrap_connection_map[pairing_index] -> mac.address[1];
            payload[6] = iwrap_connection_map[pairing_index] -> mac.address[0];
            payload[7] = pairing_index;
            
            // queue kg_evt_bluetooth_connection_status(...)
            skipPacket = 0;
            if (kg_evt_bluetooth_connection_status) skipPacket = kg_evt_bluetooth_connection_status(payload[0], payload + 1, payload[7], payload[8], payload[9]);
            if (!skipPacket) queue_keyglove_packet(KG_PACKET_TYPE_EVENT, 10, KG_PACKET_CLASS_BLUETOOTH, KG_PACKET_ID_EVT_BLUETOOTH_CONNECTION_STATUS, payload);

            activeLinks >>= 1;
            curId++;
        }
        return 0; // success
    } else {
        return KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY;
    }
}

/**
 * @brief Attempt to open a connection to a specific paired device using a specific profile
 * @param[in] index Index of pairing to use
 * @param[in] profile Profile to use for connection
 * @return Result code (0=success)
 */
uint16_t kg_cmd_bluetooth_connect(uint8_t index, uint8_t profile) {
    if (interfaceBT2Ready) {
        // validate pairing index
        if (index >= iwrap_pairings) {
            return KG_PROTOCOL_ERROR_PARAMETER_RANGE;
        }
        
        // check for other pending operations
        if (iwrap_state == IWRAP_STATE_PENDING_CALL || iwrap_state == IWRAP_STATE_PENDING_INQUIRY || iwrap_state == IWRAP_STATE_PENDING_PAIR) {
            // can't initiate an outgoing call when there is another pending BT radio operation
            return KG_BLUETOOTH_ERROR_INTERFACE_BUSY;
        } else if (!iwrap_connection_map[index]) {
            // this should NEVER happen, but if it does, I want to know
            return KG_PROTOCOL_ERROR_NULL_POINTER;
        } else {
            char cmd[] = "CALL 00:00:00:00:00:00 0011 HID\0\0\0";
            char *cptr = cmd + 5;
            iwrap_bintohexstr((uint8_t *)(iwrap_connection_map[index] -> mac.address), 6, &cptr, ':', 0);
            switch (profile) {
                case BLUETOOTH_PROFILE_MASK_AVRCP:
                    cmd[26] = '7'; // 0017
                    cmd[28] = 'A'; // AVRCP
                    cmd[29] = 'V';
                    cmd[30] = 'R';
                    cmd[31] = 'C';
                    cmd[32] = 'P';
                    break;
                case BLUETOOTH_PROFILE_MASK_HFP:
                    cmd[23] = '1'; // 111F
                    cmd[24] = '1';
                    cmd[26] = 'F';
                    cmd[29] = 'F'; // HFP
                    cmd[30] = 'P';
                    break;
                case BLUETOOTH_PROFILE_MASK_IAP:
                    cmd[23] = '*'; // *
                    cmd[24] = ' '; // IAP
                    cmd[25] = 'I'; // IAP
                    cmd[26] = 'A';
                    cmd[27] = 'P';
                    cmd[28] = 0;
                    break;
                case BLUETOOTH_PROFILE_MASK_SPP:
                    cmd[23] = '1'; // 1101
                    cmd[24] = '1';
                    cmd[25] = '0';
                    cmd[28] = 'A'; // RFCOMM
                    cmd[29] = 'V';
                    cmd[30] = 'R';
                    cmd[31] = 'C';
                    cmd[32] = 'P';
                    break;
                //default:
                    // USE HID, LEAVE COMMAND UNCHANGED
                    //break;
            }
            iwrap_send_command(cmd, iwrap_mode);
            return 0; // success
        }
    } else {
        return KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY;
    }
}

/**
 * @brief Close a specific Bluetooth connection
 * @param[in] handle Link ID of connection to close
 * @return Result code (0=success)
 */
uint16_t kg_cmd_bluetooth_disconnect(uint8_t handle) {
    if (interfaceBT2Ready) {
        // validate pairing index
        if (!(bluetoothActiveLinkMask & (1 << handle))) {
            return KG_PROTOCOL_ERROR_PARAMETER_RANGE;
        }

        // send CLOSE command
        char cmdClose[] = "CLOSE 00";
        if (handle > 9) {
            cmdClose[6] = '1';
            cmdClose[7] = handle + 38;
        } else {
            cmdClose[6] = handle + 48;
            cmdClose[7] = 0;
        }
        iwrap_send_command(cmdClose, iwrap_mode);

        return 0; // success
    } else {
        return KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY;
    }
}

#endif // _SUPPORT_BLUETOOTH2_IWRAP_H_
