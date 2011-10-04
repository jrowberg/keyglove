// Keyglove Controller source code - Main setup/loop controller
// 10/1/2010 by Jeff Rowberg <jeff@rowberg.net>

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

// =============================================================================
// specific build hardware/debug options (EDIT THIS FILE FOR YOUR HARDWARE)
// =============================================================================

#include "config.h"

//#include <Wire.h>

// =============================================================================
// !!! NO EDITING SHOULD BE NECESSARY BEYOND THIS POINT !!!
// =============================================================================

/* ===============================================
 * UNIVERSAL CONTROLLER DECLARATIONS
=============================================== */

uint16_t counter;           // loops at 1000
uint32_t ticks;             // ticks every 1000, never loops
uint32_t t;                 // time/benchmark
uint32_t i, j;              // global loop indexes for convenience and to avoid dynamic reallocation

bool activeAccelerometer;
bool activeGyroscope;
bool activeMagnetometer;
bool activeGestures;
bool activeTouch;
bool activeKeyboard;
bool activeMouse;
bool activeJoystick;
bool activeSerial;

uint16_t opt_touch_detect_threshold = 20;   // number of milliseconds required for a touch to register as real
uint8_t  opt_motion_sampling_div = 12;      // how many ticks in between each accel/gyro reading
uint8_t  opt_enable_calibration = 0;        // whether calibration should be enabled

// debug constant definitions
#include "debug.h"

// hardware constant option definitions
#include "hardware.h"

// minimal Arduino support wrapper if we're outside of the Arduino environment
#ifndef ARDUINO
    #include "ArduinoWrapper.h"
#endif

// pin assigment constants
#include "pins.h"

// pre-processor controlled global variable initializations and setup() calls
#include "setup.h"

/* ===============================================
 * MAIN SETUP ROUTINE
=============================================== */
void setup() {
    // call main setup function (see setup.h for details)
    keyglove_setup();

    // initialize benchmark
    t = millis();
}

void loop() {
    // read all defined and active motion sensors
    #ifdef ENABLE_ACCELEROMETER
        if (activeAccelerometer && counter % 10 == 0) update_motion_accelerometer();
    #endif
    #ifdef ENABLE_GYROSCOPE
        if (activeGyroscope && counter % 10 == 0) update_motion_gyroscope();
    #endif
    #ifdef ENABLE_MAGNETOMETER
        if (activeMagnetometer && counter % 10 == 0) update_motion_magnetometer();
    #endif
    #ifdef ENABLE_FUSION
        if (activeFusion && counter % 10 == 0) update_motion_fusion();
    #endif

    // process motion into gestures
    //if (activeGestures) update_gestures();

    // process touch sensor status
    if (activeTouch) update_touch();

    // keyboard is almost always done in real time, this is usually not necessary
    //if (activeKeyboard) update_keyboard();

    // send any unsent mouse control data
    if (activeMouse && counter % 10 == 0) update_mouse();

    // send any unsent joystick control data
    if (activeJoystick) update_joystick();

    // increment loop tick counter
    counter++;

    // update counter-based feedback (e.g. blinking LEDs)
    #if (KG_FEEDBACK & KG_FEEDBACK_RGB) > 0
        update_feedback_rgb();
    #endif /* KG_FEEDBACK_RGB */

    // test counter and reset if necessary
    if (counter == 1000) {
        counter = 0;
        ticks++;
        #if (KG_FEEDBACK & KG_FEEDBACK_BLINK) > 0
            update_feedback_blink();
        #endif /* KG_FEEDBACK_BLINK */

        DEBUG_PRN_BENCHMARK("benchmark 1000 ");
        DEBUG_PRN_BENCHMARK(millis() - t);
        DEBUG_PRN_BENCHMARK(" ");
        DEBUG_PRNL_BENCHMARK(ticks);
        t = millis();
    }
}


