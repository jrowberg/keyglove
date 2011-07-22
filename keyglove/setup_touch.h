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



#ifndef _SETUP_TOUCH_H_
#define _SETUP_TOUCH_H_

#include "touchset_helpers.h"
#include "touchset.h"

uint32_t touchTime; // detection timestamp
uint8_t adding = false;
uint8_t removing = false;
uint32_t detect1, detect2;
uint32_t verify1, verify2;
uint32_t sensors1, sensors2;
uint8_t mode;

void setup_touch() {
    detect1 = detect2 = 0;
    verify1 = verify2 = 0;
    sensors1 = sensors2 = 0;

    // make sure we enable internal pullup resistors
    for (i = 0; i < KG_TOTAL_SENSORS; i++) {
        pinMode(pins[i], INPUT);
        digitalWrite(pins[i], HIGH);
    }

    mode = 0; // set to base mode, no alt/shift/mouse/prog
    setmode(0); // default touchset mode is always 0
}

void update_touch() {
    detect1 = 0;
    detect2 = 0;
    removing = false;
    
    uint8_t p1, p2;

    // loop through every possible 1-to-1 sensor combination and record levels
    for (i = 0; i < KG_BASE_COMBINATIONS; i++) {
        p1 = combinations[i][0];
        p2 = combinations[i][1];

        pinMode(p1, OUTPUT);    // change to OUTPUT mode
        digitalWrite(p1, LOW);  // bring LOW (default input level is HIGH)
        if (i < 32) {
            // write value to sensors1
            if (digitalRead(p2) == LOW) bitSet(detect1, i);
        } else {
            // write value to sensors2
            if (digitalRead(p2) == LOW) bitSet(detect2, i - 32);
        }
        pinMode(p1, INPUT);     // reset to INPUT mode
        digitalWrite(p1, HIGH); // enable pullup
    }
    
    // check to see if we need to reset detection threshold
    if (verify1 != detect1 || verify2 != detect2) {
        // current sensors different from last detection, so reset threshold
        touchTime = millis();
    } else if ((verify1 != sensors1 || verify2 != sensors2) && millis() - touchTime >= opt_touch_detect_threshold) {
        // detection is over threshold and current readings are different from previous readings

        #ifdef DEBUG_TOUCH
            DEBUG_PRN_TOUCH("touch ");
            for (i = 0; i < KG_BASE_COMBINATIONS; i++) {
                if (i < 32) DEBUG_PRN_TOUCH(bitRead(detect1, i));
                else DEBUG_PRN_TOUCH(bitRead(detect2, i - 32));
            }
            DEBUG_PRN_TOUCH("\n");
        #endif /* DEBUG_TOUCH */

        // check to see if they've just initiated or removed any touches
        if (verify1 > sensors1 || verify2 > sensors2) {
            adding = true;

            // touch initiation, check for actions
            check_sensors_touch(verify1, verify2, 1);
        } else if (adding && (verify1 < sensors1 || verify2 < sensors2)) {
            adding = false;
            removing = true;

            // touch release, check for actions
            check_sensors_release(sensors1, sensors2, 1);
        }
        
        // set official sensor readings to current readings 
        sensors1 = verify1;
        sensors2 = verify2;
    }
    
    verify1 = detect1;
    verify2 = detect2;
}  

#endif // _SETUP_TOUCH_H_