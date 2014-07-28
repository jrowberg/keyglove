// Keyglove Controller source code - Main setup/loop controller
// 7/4/2014 by Jeff Rowberg <jeff@rowberg.net>

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
 * @file keyglove.ino
 * @brief Master control point for Keyglove firmware
 * @author Jeff Rowberg
 * @date 2014-07-04
 *
 * This is the main entry point and program flow control code for the entire
 * Keyglove Arduino firmware project. All of the relevant `#include` files for
 * library support and other conditional support files are listed here. The main
 * setup() and loop() functions run from here as well.
 *
 * Normally it is not necessary to edit this file.
 */



/* ===============================================
 * LIBRARY INCLUDES FOR PROPER BUILD PROCESS
=============================================== */

#include <Wire.h>       // Core Arduino I2C hardware library
#include <I2Cdev.h>     // I2Cdevlib device wrapper library
#include <MPU6050.h>    // MPU-6050 I2C sensor support for I2Cdevlib
#include <iWRAP.h>      // Bluegiga iWRAP parser library
#include <EEPROM.h>     // Core Arduino EEPROM library



/* ===============================================
 * UNIVERSAL CONTROLLER DECLARATIONS
=============================================== */

volatile uint8_t keyglove100Hz = 0;         ///< Counter inside 100Hz hardware timer interrupt
uint8_t keygloveTick = 0;                   ///< Fast 100Hz counter, increments every ~10ms and loops at 100
uint32_t keygloveTock = 0;                  ///< Slow 1Hz counter (a.k.a. "uptime"), increments every 100 ticks and loops at 2^32 (~4 billion)
//uint32_t keygloveTickTime = 0;              ///< Benchmark testing "end" reference timestamp
//uint32_t keygloveTickTime0 = 0;             ///< Benchmark testing "start" reference timestamp



/* ===============================================
 * SUPPORT INCLUDES
 =============================================== */

#include "version.h"
#include "hardware.h"
#include "config.h"



// BOARD
#if KG_BOARD == KG_BOARD_TEENSYPP2
    #include "support_board_teensypp2.h"
//#elif #KG_BOARD == KG_BOARD_ARDUINO_DUE
    //#include "support_board_arduino_due.h"
    // TODO: Arduino Due ARM chip support
#else
    #error Unsupported platform selected in KG_BOARD
#endif

// EEPROM ADDRESS CONSTANTS (best to keep these centralized)
//#include "support_eeprom.h"

// COMMUNICATION PROTOCOL
#include "support_protocol.h"

// BLUETOOTH SUPPORT
//#if (KG_HOSTIF & HG_HOSTIF_BT2_SPP) || (KG_HOSTIF & KG_HOSTIF_BT2_HID) || (KG_HOSTIF & KG_HOSTIF_BT2_RAWHID) || (KG_HOSTIF & KG_HOSTIF_BT2_IAP)
    #include "support_bluetooth2_iwrap.h"
//#endif

// CORE TOUCH SENSOR LOGIC
#include "support_touch.h"

// FEEDBACK
//#if (KG_FEEDBACK > 0)
    #include "support_feedback.h"
    #if (KG_FEEDBACK & KG_FEEDBACK_BLINK)
        #include "support_feedback_blink.h"
    #endif
    #if (KG_FEEDBACK & KG_FEEDBACK_PIEZO)
        #include "support_feedback_piezo.h"
    #endif
    #if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE)
        #include "support_feedback_vibrate.h"
    #endif
    #if (KG_FEEDBACK & KG_FEEDBACK_RGB)
        #include "support_feedback_rgb.h"
    #endif
//#endif

// MOTION
//#if (KG_MOTION > 0)
    #include "support_motion.h"
    #include "support_helper_3dmath.h"
    #if (KG_MOTION & KG_MOTION_MPU6050_HAND)
        #include "support_motion_mpu6050_hand.h"
    #endif
//#endif

// HUMAN INPUT DEVICE
#if (KG_HID & KG_HID_KEYBOARD)
    #include "support_hid_keyboard.h"
#endif
#if (KG_HID & KG_HID_MOUSE)
    #include "support_hid_mouse.h"
#endif

// PROTOCOL DEFINITIONS (DECLARATIONS IN "support_protocol.h")
#include "support_protocol_system.h"
#include "support_protocol_touch.h"
#include "support_protocol_feedback.h"
#include "support_protocol_motion.h"
//#include "support_protocol_flex.h"
//#include "support_protocol_pressure.h"
//#include "support_protocol_touchset.h"
#include "support_protocol_bluetooth.h"

// USE THIS FILE TO MODIFY/CANCEL BUILT-IN PROTOCOL PACKETS AND ADD YOUR OWN FOR SPECIAL FUNCTIONALITY
#include "custom_protocol.h"

// USE THIS FILE TO IMPLEMENT ANY AUTONOMOUS BEHAVIOUR, SUCH AS ENABLING BLUETOOTH ON BOOT
#include "application.h"

/* ===============================================
 * MAIN SETUP ROUTINE
=============================================== */

/**
 * @brief Microcontroller initial setup routine
 *
 * Runs on power-on or reset, and again if `system_reset` BGAPI command is
 * received.
 */
void setup() {
    #if (KG_HOSTIF & HG_HOSTIF_BT2_SPP) || (KG_HOSTIF & KG_HOSTIF_BT2_HID) || (KG_HOSTIF & KG_HOSTIF_BT2_RAWHID) || (KG_HOSTIF & KG_HOSTIF_BT2_IAP)
        if (systemResetFlags & 0x01) {
            // reset Bluetooth module
            kg_cmd_bluetooth_reset();
            systemResetFlags &= ~(0x01);
        }
    #endif

    // reset runtime counters
    keygloveTick = 0;
    keygloveTock = 0;

    // BOARD
    #if KG_BOARD == KG_BOARD_TEENSYPP2
        setup_board_teensypp2();
    //#elif KG_BOARD == KG_BOARD_ARDUINO_DUE
        //setup_board_arduino_due();
    #endif

    // COMMUNICATION PROTOCOL
    setup_protocol();

    // CUSTOM APPLICATION
    setup_application();

    // send system_boot event
    uint8_t payload[7] = {
        KG_FIRMWARE_VERSION_MAJOR,
        KG_FIRMWARE_VERSION_MINOR,
        KG_FIRMWARE_VERSION_PATCH,
        KG_FIRMWARE_BUILD_TIMESTAMP & 0xFF,
        (KG_FIRMWARE_BUILD_TIMESTAMP >> 8) & 0xFF,
        (KG_FIRMWARE_BUILD_TIMESTAMP >> 16) & 0xFF,
        (KG_FIRMWARE_BUILD_TIMESTAMP >> 24) & 0xFF
    };
    skipPacket = 0;
    if (kg_evt_system_boot) skipPacket = kg_evt_system_boot(KG_FIRMWARE_VERSION_MAJOR, KG_FIRMWARE_VERSION_MINOR, KG_FIRMWARE_VERSION_PATCH, KG_FIRMWARE_BUILD_TIMESTAMP);
    if (!skipPacket) send_keyglove_packet(KG_PACKET_TYPE_EVENT, 7, KG_PACKET_CLASS_SYSTEM, KG_PACKET_ID_EVT_SYSTEM_BOOT, payload);

    // CORE TOUCH SENSOR LOGIC
    setup_touch();

    // FEEDBACK
    #if (KG_FEEDBACK & KG_FEEDBACK_BLINK)
        setup_feedback_blink();
    #endif
    #if (KG_FEEDBACK & KG_FEEDBACK_PIEZO)
        setup_feedback_piezo();
    #endif
    #if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE)
        setup_feedback_vibrate();
    #endif
    #if (KG_FEEDBACK & KG_FEEDBACK_RGB)
        setup_feedback_rgb();
    #endif

    // MOTION
    #if (KG_MOTION & KG_MOTION_MPU6050_HAND)
        setup_motion_mpu6050_hand();
    #endif

    // HOST INTERFACE
    #if (KG_HOSTIF & HG_HOSTIF_BT2_SPP) || (KG_HOSTIF & KG_HOSTIF_BT2_HID) || (KG_HOSTIF & KG_HOSTIF_BT2_RAWHID) || (KG_HOSTIF & KG_HOSTIF_BT2_IAP)
        setup_hostif_bt2();
    #endif

    // HUMAN INPUT DEVICE
    #if (KG_HID & KG_HID_KEYBOARD)
        setup_hid_keyboard();
    #endif
    #if (KG_HID & KG_HID_MOUSE)
        setup_hid_mouse();
    #endif

    // send system_ready event
    skipPacket = 0;
    if (kg_evt_system_ready) skipPacket = kg_evt_system_ready();
    send_keyglove_packet(KG_PACKET_TYPE_EVENT, 0, KG_PACKET_CLASS_SYSTEM, KG_PACKET_ID_EVT_SYSTEM_READY, 0);
}



/* ===============================================
 * MAIN LOOP ROUTINE
=============================================== */

/**
 * @brief Microcontroller infinite loop routine
 *
 * This routine loops forever while the microcontroller is running. It is
 * responsible for checking on touch status, motion sensor status, and generally
 * everything else which does not rely strictly on hardware interrupts to
 * operate. Even some interrupt-related code is found here, since the interrupt
 * handlers typically just set a flag which causes this code to process the
 * event, so that the actual longer-running execution does not block any other
 * interrupts from occuring.
 */

void loop() {
    // check for incoming protocol data
    check_incoming_protocol_data();

    // check for 100Hz tick (i.e. every 10ms)
    if (keyglove100Hz) {
        keyglove100Hz = 0;
        //keygloveTickTime += micros() - keygloveTickTime0;
        //keygloveTickTime0 = micros();
        
        // update touch status
        update_touch();

        // update feedback settings
        #if (KG_FEEDBACK & KG_FEEDBACK_BLINK)
            update_feedback_blink();
        #endif // KG_FEEDBACK_BLINK
        #if (KG_FEEDBACK & KG_FEEDBACK_RGB)
            update_feedback_rgb();
        #endif // KG_FEEDBACK_RGB
        #if (KG_FEEDBACK & KG_FEEDBACK_PIEZO)
            update_feedback_piezo();
        #endif // KG_FEEDBACK_PIEZO
        #if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE)
            update_feedback_vibrate();
        #endif // KG_FEEDBACK_VIBRATE

        // check for 100 ticks and reset counter (should be every 1 second)
        keygloveTick++;
        if (keygloveTick == 100) {
            //keygloveTickTime = 0;
            keygloveTick = 0;
            keygloveTock++;
        }
    }
    
    // MOTION
    #if (KG_MOTION & KG_MOTION_MPU6050_HAND)
        // check for available motion data from MPU-6050 on back of hand
        if (mpuHandInterrupt) {
            mpuHandInterrupt = false; // clear the flag so we don't read again until the next interrupt
            update_motion_mpu6050_hand();
        }
    #endif
    
    // send any queued packets
    send_keyglove_queue();
}