// Keyglove controller source code - Main setup/loop controller
// 10/1/2010 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2010 Jeff Rowberg

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
unsigned char ide_workaround = 0;

/* ===============================================
 * CORE CONTROLLER SETTINGS
=============================================== */
//#define USE_ARDUINO
#define USE_TEENSY

/* ===============================================
 * CORE FEATURE SETTINGS
=============================================== */
#define ENABLE_BLINK
#define ENABLE_TRICOLOR
//#define ENABLE_PS2
#define ENABLE_USB
//#define ENABLE_BLUETOOTH
#define ENABLE_TOUCH
#define ENABLE_ACCEL

/* ===============================================
 * DEBUG COMPILER SETTINGS
=============================================== */
// useful for kg_visualize Processing sketch and other serial debugging
// define these and data will be sent over controller serial port
//#define SERIAL_DEBUG
//#define SERIAL_DEBUG_ACCEL
//#define SERIAL_DEBUG_TOUCH
//#define SERIAL_DEBUG_MOUSE
//#define SERIAL_DEBUG_KEYBOARD

// set correct pins based on host device
#include "pins.h"

/* ===============================================
 * PS/2 DECLARATIONS
=============================================== */
#ifdef ENABLE_PS2
    #include "ps2keyboard.h"
    #include "ps2mouse.h"
    
    PS2dev ps2k(KB_PIN_CLOCK, KB_PIN_DATA);
    PS2keyboard keyboard(&ps2k);
    
    PS2dev ps2m(MOUSE_PIN_CLOCK, MOUSE_PIN_DATA);
    PS2mouse mouse(&ps2m);

    void ps2keyboardInterrupt() {
        keyboard.process_command();
    }
    
    void ps2mouseInterrupt() {
        mouse.process_command();
    }
#endif /* ENABLE_PS2 */

/* ===============================================
 * USB DECLARATIONS
=============================================== */
#ifdef ENABLE_USB
    #include "usbkeyproc.h"
#endif /* ENABLE_USB */

/* ===============================================
 * BLUETOOTH DECLARATIONS
=============================================== */
#ifdef ENABLE_BLUETOOTH
#endif /* ENABLE_BLUETOOTH */

/* ===============================================
 * TOUCH SENSOR DECLARATIONS
=============================================== */
#ifdef ENABLE_TOUCH
    #include "touchset.h"
    unsigned long int touchTime; // detection timestamp
    boolean adding = false;
    boolean removing = false;
    long unsigned int detect1, detect2;
    long unsigned int verify1, verify2;
    long unsigned int sensors1, sensors2;
    byte mode;
#endif /* ENABLE_TOUCH */

/* ===============================================
 * ACCELEROMETER DECLARATIONS
=============================================== */
#ifdef ENABLE_ACCEL
    #include <Wire.h>
    #include "adxl345.h"
    #define MOUSE_MODE_TILT_VELOCITY 1
    #define MOUSE_MODE_TILT_POSITION 2
    #define MOUSE_MODE_MOVEMENT_POSITION 3

    Accelerometer accel;

    // accelerometer measurements
    int accelTick;                    // history position counter
    int xRaw, yRaw, zRaw;             // raw accel values (no calibration, no filtering)
    int x, y, z;                      // immediate accel values
    int x0, y0, z0;                   // last-iteration accel values
    int x00, y00, z00;                // 2x-last-iteration accel values (for speed)
    int xv, yv, zv;                   // axial vectors
    int xBase, yBase, zBase;          // initial accel values
    int xHist[20], yHist[20], zHist[20];  // history for averaging (smoothing)
    int xMin, yMin, zMin;             // minimum values (for calibration)
    int xMax, yMax, zMax;             // maximum values (for calibration)
    int ax, ay, az;                   // immediate accel tilt angles
    int ax0, ay0, az0;                // last-iteration accel tilt angles
    int ax00, ay00, az00;             // 2x-last-iteration accel tilt angles (for speed)
    int axv, ayv, azv;                // angular rotational vectors
    int axBase, ayBase, azBase;       // initial accel tilt angles
    boolean aset = false;             // bool to tell whether initial (*Base) values have been set yet
    
    // mouse measurements
    int mx, my;                       // absolute coordinates
    int mx0, my0;                     // last-iteration absolute coordinates
    int mdx, mdy;                     // relative movement
#endif /* ENABLE_ACCEL */

/* ===============================================
 * TRICOLOR STATUS LED DECLARATIONS
=============================================== */
#ifdef ENABLE_TRICOLOR
    int tricolorBlinkRed;
    int tricolorBlinkGreen;
    int tricolorBlinkBlue;
    void tricolor(byte r, byte g, byte b) {
        if (r != 255) digitalWrite(TRICOLOR_RED_PIN, r);
        if (g != 255) digitalWrite(TRICOLOR_GREEN_PIN, g);
        if (b != 255) digitalWrite(TRICOLOR_BLUE_PIN, b);
    }
#endif /* ENABLE_TRICOLOR */

/* ===============================================
 * BLINK LED DECLARATIONS
=============================================== */
boolean blink_led;                    // state of blink LED

/* ===============================================
 * UNIVERSAL CONTROLLER DECLARATIONS
=============================================== */
// basic timing and program flow control
int counter;                          // loops at 1000
int ticks;                            // ticks every 1000, never loops
unsigned long int t;                  // time/benchmark
long int i;                           // global loop index

/* ===============================================
 * DEVICE-CONTROLLED OPTION DECLARATIONS
=============================================== */
int opt_touch_detect_threshold = 20;            // number of milliseconds required for a touch to register as real
int opt_enable_calibration = 0;                 // temporarily enable calibration mode (resets accel offset/calibrate)
//int opt_accel_offset[] = { 0, 0, 0 };           // amount to offset raw accelerometer readings [x,y,z]
//float opt_accel_calibrate[] = { 1, 1, 1 };      // amount to scale raw accelerometer readings [x,y,z] (post-offset)

int opt_accel_offset[] = { -12, 5, 2 };
float opt_accel_calibrate[] = { 0.952, 0.945, 1.04 };
// mine @ room temp:
// x={-257,281}, y={-276,267}, z={-258,254}
// x offset = -12, x calibrate = 0.952
// y offset = 5, y calibrate = 0.945
// z offset = 2, z calibrate = 1

int opt_accel_sampling_div = 4;                 // how many ticks in between each accel reading
float opt_kalman_constant = 0.4;
int opt_smooth_average = 3;
int opt_mouse_mode = 2;                         // which mouse mode (0=disable, 1/2/3 as defined)
int opt_mouse_hold = 0;                         // temporarily hold mouse position for hand re-orientation
float opt_mouse_scale_mode1[] = { 1, 1 };       // speed scale [x,y] for mode 1 (tilt-velocity)
float opt_mouse_scale_mode2[] = { 1, 1 };       // speed scale [x,y] for mode 2 (tilt-position)
float opt_mouse_scale_mode3[] = { 1, 1 };       // speed scale [x,y] for mode 3 (movement-position)

/* ===============================================
 * MAIN SETUP ROUTINE
=============================================== */
void setup() {
    /* ===============================================
     * INITIALIZE SERIAL DEBUGGING
    =============================================== */
    #ifdef SERIAL_DEBUG
        Serial.begin(57600);
        Serial.println("info Keyglove device activated");
    #endif /* SERIAL_DEBUG */

    /* ===============================================
     * INITIALIZE TRICOLOR STATUS LED CONTROL
    =============================================== */
    #ifdef ENABLE_TRICOLOR
        pinMode(TRICOLOR_RED_PIN, OUTPUT);
        pinMode(TRICOLOR_GREEN_PIN, OUTPUT);
        pinMode(TRICOLOR_BLUE_PIN, OUTPUT);
        tricolor(1, 1, 1); // turn everything on and wait 1/10 sec
        delay(100);
        tricolor(0, 0, 0); // turn everything off again
    #endif /* ENABLE_TRICOLOR */

    /* ===============================================
     * INITIALIZE BLINK LED STATE
    =============================================== */
    #ifdef ENABLE_BLINK
        pinMode(BLINK_LED_PIN, OUTPUT);
        digitalWrite(BLINK_LED_PIN, LOW);
        blink_led = false;
    #endif /* ENABLE_BLINK */

    /* ===============================================
     * INITIALIZE TOUCH SENSOR DIGITAL I/O PINS
    =============================================== */
    #ifdef ENABLE_TOUCH
        // make sure we enable internal pullup resistors
        for (i = 0; i < KG_TOTAL_SENSORS; i++) {
            pinMode(pins[i], INPUT);
            digitalWrite(pins[i], HIGH);
        }
        mode = 0; // set to base mode, no alt/shift/mouse/prog
    #endif /* ENABLE_TOUCH */

    /* ===============================================
     * INITIALIZE PS/2 INTERFACE
    =============================================== */
    #ifdef ENABLE_PS2
        keyboard.initialize();
        mouse.initialize();
        attachInterrupt(KB_INT1, ps2keyboardInterrupt, LOW);
        attachInterrupt(KB_INT2, ps2keyboardInterrupt, LOW);
        attachInterrupt(MOUSE_INT1, ps2mouseInterrupt, LOW);
        attachInterrupt(MOUSE_INT2, ps2mouseInterrupt, LOW);
    #endif /* ENABLE_PS2 */

    /* ===============================================
     * INITIALIZE USB INTERFACE
    =============================================== */
    #ifdef ENABLE_USB
    #endif /* ENABLE_USB */
    
    /* ===============================================
     * INITIALIZE BLUETOOTH INTERFACE
    =============================================== */
    #ifdef ENABLE_BLUETOOTH
    #endif /* ENABLE_BLUETOOTH */
    
    /* ===============================================
     * INITIALIZE ACCELEROMETER INTERFACE
    =============================================== */
    #ifdef ENABLE_ACCEL
        // initialize accelerometer
        accel.powerOn();
        accel.setRate(100);
        x = y = z = 0;
        accelTick = 0;
        xMin = yMin = zMin = 0;
        xMax = yMax = zMax = 0;
        mx = my = mx0 = my0 = 0;
    #endif /* ENABLE_ACCEL */
    
    // initialize benchmark
    t = millis();
}

void loop() {
    /* ===============================================
     * READ AND PROCESS ACCELEROMETER DATA
    =============================================== */
    #ifdef ENABLE_ACCEL
        if (counter % opt_accel_sampling_div == 0) {
            x00 = x0;
            y00 = y0;
            z00 = z0;
            x0 = x;
            y0 = y;
            z0 = z;
            accel.readAccel(&xRaw, &yRaw, &zRaw);
            
            // offset
            xRaw += opt_accel_offset[0];
            yRaw += opt_accel_offset[1];
            zRaw += opt_accel_offset[2];
            
            // calibrate
            xRaw = (float)xRaw * opt_accel_calibrate[0];
            yRaw = (float)yRaw * opt_accel_calibrate[1];
            zRaw = (float)zRaw * opt_accel_calibrate[2];
            
            // Kalman filtering
            x = x0 + (opt_kalman_constant * (xRaw - x0));
            y = y0 + (opt_kalman_constant * (yRaw - y0));
            z = z0 + (opt_kalman_constant * (zRaw - z0));
            
            // averaging
            if (opt_smooth_average > 0) {
                for (i = 1; i < opt_smooth_average; i++) {
                    x += xHist[(accelTick + i) % opt_smooth_average];
                    y += yHist[(accelTick + i) % opt_smooth_average];
                    z += zHist[(accelTick + i) % opt_smooth_average];
                }
                x /= opt_smooth_average;
                y /= opt_smooth_average;
                z /= opt_smooth_average;
                xHist[accelTick % opt_smooth_average] = x;
                yHist[accelTick % opt_smooth_average] = y;
                zHist[accelTick % opt_smooth_average] = z;
            }
            
            // calculate linear velocity
            xv -= ((x0 - x00) - (x - x0));
            yv -= ((y0 - y00) - (y - y0));
            zv -= ((z0 - z00) - (z - z0));

            if (opt_enable_calibration) {
                xMin = min(xMin, x);
                yMin = min(yMin, y);
                zMin = min(zMin, z);
                xMax = max(xMax, x);
                yMax = max(yMax, y);
                zMax = max(zMax, z);
                #ifdef SERIAL_DEBUG_ACCEL
                    Serial.print("calibrate ");
                    Serial.print(xMin); Serial.print(" ");
                    Serial.print(yMin); Serial.print(" ");
                    Serial.print(zMin); Serial.print(" ");
                    Serial.print(xMax); Serial.print(" ");
                    Serial.print(yMax); Serial.print(" ");
                    Serial.println(zMax);
                #endif
            }
    
            // calculate tilt angle and angular velocity
            ax00 = ax0; // 2x-last-iteration
            ay00 = ay0;
            az00 = az0;
            ax0 = ax; // last-iteration
            ay0 = ay;
            az0 = az;
            ax = abs(x) < 256 ? degrees(asin((float)x / 256)) : (x < 0 ? -90 : 90);
            ay = abs(y) < 256 ? degrees(asin((float)y / 256)) : (y < 0 ? -90 : 90);
            az = abs(z) < 256 ? degrees(asin((float)z / 256)) : (z < 0 ? -90 : 90);
            axv -= ((ax0 - ax00) - (ax - ax0));
            ayv -= ((ay0 - ay00) - (ay - ay0));
            azv -= ((az0 - az00) - (az - az0));
            
            #ifdef SERIAL_DEBUG_ACCEL
                Serial.print("accel ");
                Serial.print(xRaw); Serial.print(" ");
                Serial.print(yRaw); Serial.print(" ");
                Serial.print(zRaw); Serial.print(" ");
                Serial.print(x); Serial.print(" ");
                Serial.print(y); Serial.print(" ");
                Serial.print(z); Serial.print(" ");
                Serial.print(xv); Serial.print(" ");
                Serial.print(yv); Serial.print(" ");
                Serial.print(zv); Serial.print(" ");
                Serial.print(ax); Serial.print(" ");
                Serial.print(ay); Serial.print(" ");
                Serial.print(az); Serial.print(" ");
                Serial.print(axv); Serial.print(" ");
                Serial.print(ayv); Serial.print(" ");
                Serial.println(azv);
            #endif /* SERIAL_DEBUG_ACCEL */
    
            mx0 = mx;
            my0 = my;
            switch (opt_mouse_mode) {
                case MOUSE_MODE_TILT_VELOCITY:
                    if (aset) {
                        mx -= (float)(ax - axBase) / opt_mouse_scale_mode1[0];
                        my -= (float)(ay - ayBase) / opt_mouse_scale_mode1[1];
                    } else if (ticks == 0 && counter == 10) {
                        axBase = ax;
                        ayBase = ay;
                        azBase = az;
                        aset = true;
                    }
                    break;
                case MOUSE_MODE_TILT_POSITION:
                    if (aset) {
                        // progressive scaling
                        mx -= (pow(2, abs(axv)) * opt_mouse_scale_mode2[0]) * (float)(ax - axBase);
                        my -= (pow(2, abs(ayv)) * opt_mouse_scale_mode2[1]) * (float)(ay - ayBase);
                    }
                    axBase = ax;
                    ayBase = ay;
                    azBase = az;
                    aset = true;
                    break;
                case MOUSE_MODE_MOVEMENT_POSITION:
                    if (aset) {
                        mx -= (float)(x - xBase) / opt_mouse_scale_mode3[0];
                        my -= (float)((z - 256) - zBase) / opt_mouse_scale_mode3[1];
                    } else if (ticks == 0 && counter == 10) {
                        xBase = x;
                        yBase = y;
                        zBase = z - 256;
                        aset = true;
                    }
                    break;
            }
            //if (BOUND_MOUSEX > 0 && abs(mx) > BOUND_MOUSEX) mx = mx < 0 ? -BOUND_MOUSEX : BOUND_MOUSEX;
            //if (BOUND_MOUSEY > 0 && abs(my) > BOUND_MOUSEY) my = my < 0 ? -BOUND_MOUSEY : BOUND_MOUSEY;
    
            // get relative movement amounts
            mdx = mx - mx0;
            mdy = my - my0;
    
            #ifdef ENABLE_USB
                Mouse.move(mdx, mdy);
            #endif /* ENABLE_USB */
            
            #ifdef SERIAL_DEBUG_MOUSE
                Serial.print("mouse ");
                Serial.print(mx); Serial.print(" ");
                Serial.print(my); Serial.print(" ");
                Serial.print(mdx); Serial.print(" ");
                Serial.println(mdy);
            #endif /* SERIAL_DEBUG_MOUSE */
            accelTick++;
        }
    #endif /* ENABLE_ACCEL */
    
    /* ===============================================
     * READ AND PROCESS TOUCH DATA
    =============================================== */
    #ifdef ENABLE_TOUCH
        detect1 = 0;
        detect2 = 0;
        removing = false;
    
        // loop through every possible 1-to-1 sensor combination and record levels
        for (i = 0; i < KG_BASE_COMBINATIONS; i++) {
            int p1 = combinations[i][0];
            int p2 = combinations[i][1];
    
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
            #ifdef SERIAL_DEBUG_TOUCH
                Serial.print("touch ");
                for (i = 0; i < KG_BASE_COMBINATIONS; i++) {
                    if (i < 32) Serial.print(bitRead(detect1, i));
                    else Serial.print(bitRead(detect2, i - 32));
                }
                Serial.print("\n");
            #endif /* SERIAL_DEBUG_TOUCH */
        } else if ((verify1 != sensors1 || verify2 != sensors2) && millis() - touchTime >= opt_touch_detect_threshold) {
            // detection is over threshold and current readings are different from previous readings
            // check to see if they've just initiated or removed any touches
            if (verify1 > sensors1 || verify2 > sensors2) {
                adding = true;
            } else if (adding && (verify1 < sensors1 || verify2 < sensors2)) {
                adding = false;
                removing = true;
    
                // actual keypress this time around so test for each possible combination
                test_sensors(sensors1, sensors2);
            }
            
            // set official sensor readings to current readings 
            sensors1 = verify1;
            sensors2 = verify2;
        }
        
        verify1 = detect1;
        verify2 = detect2;
    #endif /* ENABLE_TOUCH */

    // increment loop tick counter
    counter++;

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
        t = millis();
    }
}
