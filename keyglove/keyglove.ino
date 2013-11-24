// Keyglove Controller source code - Main setup/loop controller
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



/* ===============================================
 * LIBRARY INCLUDES FOR PROPER BUILD PROCESS
=============================================== */

#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <iWRAP.h>



/* ===============================================
 * UNIVERSAL CONTROLLER DECLARATIONS
=============================================== */

volatile uint8_t keyglove100Hz = 0;
uint8_t keygloveTick = 0;   // increments every ~10ms (100hz), loops at 100
uint32_t keygloveTock = 0;  // increments every 100 ticks, loops at 2^32 (~4 billion)
uint32_t keygloveTickTime = 0, keygloveTickTime0 = 0;



/* ===============================================
 * SUPPORT INCLUDES
 =============================================== */

#include "version.h"
#include "hardware.h"
#include "config.h"

// BOARD
#if KG_BOARD == KG_BOARD_TEENSYPP2
    #include "support_board_teensypp2.h"
#elif #KG_BOARD == KG_BOARD_ARDUINO_DUE
    #include "support_board_arduino_due.h"
#else
    #error Unsupported platform selected in KG_BOARD
#endif

// COMMUNICATION PROTOCOL
#include "support_protocol.h"

// CORE TOUCH SENSOR LOGIC
#include "support_touch.h"

// FEEDBACK
#if (KG_FEEDBACK & KG_FEEDBACK_BLINK)
    #include "support_feedback_blink.h"
#endif
#if (KG_FEEDBACK & KG_FEEDBACK_PIEZO)
    #include "support_feedback_piezo.h"
#endif
#if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE_HAND)
    #include "support_feedback_vibrate_hand.h"
#endif
#if (KG_FEEDBACK & KG_FEEDBACK_RGB)
    #include "support_feedback_rgb.h"
#endif

// MOTION
#include "support_helper_3dmath.h"
#if (KG_MOTION & KG_MOTION_MPU6050_HAND)
    #include "support_motion_mpu6050_hand.h"
#endif

// HOST INTERFACE
#if (KG_HOSTIF & KG_HOSTIF_USB)
    #include "support_hostif_usb.h"
#endif
#if (KG_HOSTIF & KG_HOSTIF_BT2)
    #include "support_hostif_bt2.h"
#endif

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

// USE THIS FILE TO MODIFY/CANCEL BUILT-IN PROTOCOL PACKETS AND ADD YOUR OWN FOR SPECIAL FUNCTIONALITY
#include "custom_protocol.h"

/* ===============================================
 * MAIN SETUP ROUTINE
=============================================== */

void setup() {
    // reset runtime counters
    keygloveTick = 0;
    keygloveTock = 0;

    // BOARD
    #if KG_BOARD == KG_BOARD_TEENSYPP2
        setup_board_teensypp2();
    #elif #KG_BOARD == KG_BOARD_ARDUINO_DUE
        setup_board_arduino_due();
    #endif

    // COMMUNICATION PROTOCOL
    setup_protocol();

    // send system_boot event
    send_keyglove_packet(KG_PACKET_TYPE_EVENT, 0, KG_PACKET_CLASS_SYSTEM, KG_PACKET_ID_EVT_SYSTEM_BOOT, 0);

    // CORE TOUCH SENSOR LOGIC
    //setup_touch();

    // FEEDBACK
    #if (KG_FEEDBACK & KG_FEEDBACK_BLINK)
        setup_feedback_blink();
    #endif
    #if (KG_FEEDBACK & KG_FEEDBACK_PIEZO)
        setup_feedback_piezo();
    #endif
    #if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE_HAND)
        setup_feedback_vibrate_hand();
    #endif
    #if (KG_FEEDBACK & KG_FEEDBACK_RGB)
        setup_feedback_rgb();
    #endif

    // MOTION
    #if (KG_MOTION & KG_MOTION_MPU6050_HAND)
        setup_motion_mpu6050_hand();
    #endif

    // HOST INTERFACE
    #if (KG_HOSTIF & KG_HOSTIF_USB)
        setup_hostif_usb();
    #endif
    #if (KG_HOSTIF & KG_HOSTIF_BT2)
        setup_hostif_bt2();
    #endif

    // HUMAN INPUT DEVICE
    #if (KG_HID & KG_HID_KEYBOARD)
        setup_hid_keyboard();
    #endif
    #if (KG_HID & KG_HID_MOUSE)
        setup_hid_mouse();
    #endif

    // send system_boot event
    send_keyglove_packet(KG_PACKET_TYPE_EVENT, 0, KG_PACKET_CLASS_SYSTEM, KG_PACKET_ID_EVT_SYSTEM_READY, 0);
}



/* ===============================================
 * MAIN LOOP ROUTINE
=============================================== */

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
    
    #if (KG_MOTION & KG_MOTION_MPU6050_HAND)
        // check for available motion data from MPU-6050 on back of hand
        if (mpuHandInterrupt) {
            mpuHandInterrupt = false; // clear the flag so we don't read again until the next interrupt
            update_motion_mpu6050_hand();
        }
    #endif
}