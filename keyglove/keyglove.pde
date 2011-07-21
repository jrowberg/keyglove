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

// fix Arduino IDE regex issue with detecting first non-preprocessor directive
// (breaks when #ifdef is false and contains non-preprocessor line, see:
// http://code.google.com/p/arduino/issues/detail?id=156)
uint8_t ide_workaround = 0;

// =============================================================================
// specific build hardware/debug options (EDIT THIS FILE FOR YOUR HARDWARE)
// =============================================================================

#include "config.h"

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

// LUFA framework Arduino wrapper
#ifdef LUFA
    // the Keyglove LUFA code release makefile defines the LUFA constant for all
    // compiled source files, including this one...LUFA is not automatically
    // Arduino-friendly, so a wrapper is required (like LUFAduino, but not)
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
        if (activeAccelerometer) update_motion_accelerometer();
    #endif
    #ifdef ENABLE_GYROSCOPE
        if (activeGyroscope) update_motion_gyroscope();
    #endif
    #ifdef ENABLE_MAGNETOMETER
        if (activeMagnetometer) update_motion_magnetometer();
    #endif
    #ifdef ENABLE_FUSION
        if (activeFusion) update_motion_fusion();
    #endif
    
    // process motion into gestures
    //if (activeGestures) update_gestures();
    
    // process touch sensor status
    if (activeTouch) update_touch();

    // keyboard is almost always done in real time, this is usually not necessary
    if (activeKeyboard) update_keyboard();

    // send any unsent mouse control data
    if (activeMouse) update_mouse();

    // send any unsent joystick control data
    if (activeJoystick) update_joystick();

    // increment loop tick counter
    counter++;

    // update counter-based feedback (e.g. blinking LEDs)
    #ifdef ENABLE_TRICOLOR
        if      (tricolorBlinkRed   == 1 && counter % 500 == 0) tricolor(counter % 1000 >= 500 ? 0 : 1, 255, 255);
        else if (tricolorBlinkRed   == 2 && counter % 100 == 0) tricolor(counter % 1000 >= 100 ? 0 : 1, 255, 255);
        else if (tricolorBlinkRed   == 3 && counter %  50 == 0) tricolor(counter %  100 >=  50 ? 0 : 1, 255, 255);
        else if (tricolorBlinkRed   == 4 && counter %  10 == 0) tricolor(counter %  100 >=  10 ? 0 : 1, 255, 255);
        if      (tricolorBlinkGreen == 1 && counter % 500 == 0) tricolor(255, counter % 1000 >= 500 ? 0 : 1, 255);
        else if (tricolorBlinkGreen == 3 && counter % 100 == 0) tricolor(255, counter % 1000 >= 100 ? 0 : 1, 255);
        else if (tricolorBlinkGreen == 2 && counter %  50 == 0) tricolor(255, counter %  100 >=  50 ? 0 : 1, 255);
        else if (tricolorBlinkGreen == 4 && counter %  10 == 0) tricolor(255, counter %  100 >=  10 ? 0 : 1, 255);
        if      (tricolorBlinkBlue  == 1 && counter % 500 == 0) tricolor(255, 255, counter % 1000 >= 500 ? 0 : 1);
        else if (tricolorBlinkBlue  == 2 && counter % 100 == 0) tricolor(255, 255, counter % 1000 >= 100 ? 0 : 1);
        else if (tricolorBlinkBlue  == 3 && counter %  50 == 0) tricolor(255, 255, counter %  100 >=  50 ? 0 : 1);
        else if (tricolorBlinkBlue  == 4 && counter %  10 == 0) tricolor(255, 255, counter %  100 >=  10 ? 0 : 1);
    #endif /* ENABLE_TRICOLOR */

    // test counter and reset if necessary
    if (counter == 1000) {
        counter = 0;
        ticks++;
        #ifdef ENABLE_BLINK
            blink_led = !blink_led;
            if (blink_led) digitalWrite(BLINK_LED_PIN, HIGH);
            else digitalWrite(BLINK_LED_PIN, LOW);
        #endif /* ENABLE_BLINK */

        DEBUG_PRN_BENCHMARK("benchmark 1000 ");
        DEBUG_PRN_BENCHMARK(millis() - t);
        DEBUG_PRN_BENCHMARK(" ");
        DEBUG_PRNL_BENCHMARK(ticks);
    }

    t = millis();
}