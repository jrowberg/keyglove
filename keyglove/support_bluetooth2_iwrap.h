// Keyglove controller source code - Declarations for Bluetooth using Bluegiga's WT12 module and iWRAP
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
 * @file support_bluetooth2_iwrap.h
 * @brief Declarations for Bluetooth using Bluegiga's WT12 module and iWRAP
 * @author Jeff Rowberg
 * @date 2014-11-07
 */

#ifndef _SUPPORT_BLUETOOTH2_IWRAP_H_
#define _SUPPORT_BLUETOOTH2_IWRAP_H_

#define KG_BLUETOOTH_ERROR_INTERFACE_NOT_READY  0x0801  ///< Bluetooth interface has not been initialized
#define KG_BLUETOOTH_ERROR_INTERFACE_BUSY       0x0802  ///< Bluetooth module is currently busy and cannot accept this command

#define BLUETOOTH_PROFILE_MASK_HID_CONTROL      0x01    ///< Bitmask indicating HID control link
#define BLUETOOTH_PROFILE_MASK_HID_INTERRUPT    0x02    ///< Bitmask indicating HID interrupt link
#define BLUETOOTH_PROFILE_MASK_IAP              0x04    ///< Bitmask indicating IAP link
#define BLUETOOTH_PROFILE_MASK_SPP              0x08    ///< Bitmask indicating SPP (RFCOMM) link
#define BLUETOOTH_PROFILE_MASK_HFP              0x10    ///< Bitmask indicating HFP link
#define BLUETOOTH_PROFILE_MASK_AVRCP            0x20    ///< Bitmask indicating AVRCP link

#define BT2_RAWHID_RX_SIZE 16                           ///< Raw HID input payload size
#define BT2_RAWHID_TX_SIZE 16                           ///< Raw HID output payload size

// ======== BEGIN DECLARATIONS COPIED/TWEAKED FROM IWRAP ARDUINO DEMO ========

#include <iWRAP.h>

#define IWRAP_STATE_IDLE                0               ///< iWRAP is idle (known state)
#define IWRAP_STATE_UNKNOWN             1               ///< iWRAP state unknown (e.g. fresh boot)
#define IWRAP_STATE_PENDING_AT          2               ///< iWRAP "AT" communication test pending
#define IWRAP_STATE_PENDING_SET         3               ///< iWRAP settings dump request pending
#define IWRAP_STATE_PENDING_LIST        4               ///< iWRAP connection list request pending
#define IWRAP_STATE_PENDING_CALL        5               ///< iWRAP outgoing call pending
#define IWRAP_STATE_PENDING_INQUIRY     6               ///< iWRAP device inquiry pending
#define IWRAP_STATE_PENDING_PAIR        7               ///< iWRAP outgoing pair attempt pending
#define IWRAP_STATE_PENDING_SETBTPAIR   8               ///< iWRAP pairing entry removal pending
#define IWRAP_STATE_COMM_FAILED         255             ///< iWRAP communication attempted and failed (unusable)

#define IWRAP_MAX_PAIRINGS              16              ///< Maximum pairing entries allowed in iWRAP

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

/**
 * @brief Simple wrapper for Teensy-like keyboard behavior and iWRAP raw HID report interface
 */
class BTKeyboardWrapper {
    public:
        BTKeyboardWrapper() { };
        void set_key1(uint8_t code);
        void set_key2(uint8_t code);
        void set_key3(uint8_t code);
        void set_key4(uint8_t code);
        void set_key5(uint8_t code);
        void set_key6(uint8_t code);
        void set_modifier(uint8_t code);
        void send_now();
};

/**
 * @brief Simple wrapper for Teensy-like mouse behavior and iWRAP HID interface
 */
class BTMouseWrapper {
    public:
        BTMouseWrapper() { };
        void move(int8_t x, int8_t y);
        void scroll(int8_t vscroll, int8_t hscroll);
        void move(int8_t x, int8_t y, int8_t vscroll, int8_t hscroll);
};

extern BTKeyboardWrapper BTKeyboard;
extern BTMouseWrapper BTMouse;

// Bluetooth link/interface readiness
extern bool interfaceBT2Ready;
extern bool interfaceBT2SerialReady;
extern uint8_t interfaceBT2SerialMode;
extern bool interfaceBT2IAPReady;
extern uint8_t interfaceBT2IAPMode;
extern bool interfaceBT2RawHIDReady;
extern uint8_t interfaceBT2RawHIDMode;
extern bool interfaceBT2HIDReady;
extern bool interfaceBT2HFPReady;
extern bool interfaceBT2AVRCPReady;

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

// keyglove infrastructure functions
void setup_hostif_bt2();
uint8_t bluetooth_check_incoming_protocol_data();
uint8_t bluetooth_send_keyglove_packet_buffer(uint8_t *buffer, uint8_t length, uint8_t specificInterface);

#endif // _SUPPORT_BLUETOOTH2_IWRAP_H_
