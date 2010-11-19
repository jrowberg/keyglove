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

// core features
#define ENABLE_BLINK
//#define ENABLE_PS2
//#define ENABLE_USB
//#define ENABLE_BLUETOOTH
#define ENABLE_TOUCH
#define ENABLE_ACCEL

// debug settings (useful for kg_visualize Processing sketch,
// define these and data will be sent over Arduino serial port)
#define SERIAL_DEBUG_ACCEL
#define SERIAL_DEBUG_TOUCH
#define SERIAL_DEBUG_MOUSE
#define SERIAL_DEBUG_KEYBOARD

#ifdef ENABLE_PS2
    #include "ps2keyboard.h"
    #include "ps2mouse.h"
    
    #define KB_PIN_CLOCK    19
    #define KB_PIN_DATA     18
    PS2dev ps2k(KB_PIN_CLOCK, KB_PIN_DATA);
    PS2keyboard keyboard(&ps2k);
    
    #define MOUSE_PIN_CLOCK 3
    #define MOUSE_PIN_DATA  2
    PS2dev ps2m(MOUSE_PIN_CLOCK, MOUSE_PIN_DATA);
    PS2mouse mouse(&ps2m);

    void ps2keyboardInterrupt() {
        keyboard.process_command();
    }
    
    void ps2mouseInterrupt() {
        mouse.process_command();
    }
#endif /* ENABLE_PS2 */

#ifdef ENABLE_USB
#endif /* ENABLE_USB */

#ifdef ENABLE_BLUETOOTH
#endif /* ENABLE_BLUETOOTH */

#ifdef ENABLE_TOUCH
    #include "sensors.h"
    #include "touchset.h"
    unsigned long int d; // detection timestamp
    boolean adding = false;
    boolean removing = false;
    long unsigned int detect1, detect2;
    long unsigned int verify1, verify2;
    long unsigned int sensors1, sensors2;
#endif /* ENABLE_TOUCH */

#ifdef ENABLE_ACCEL
    #include <Wire.h>
    #include "adxl345.h"
    #define MOUSE_MODE_TILT_VELOCITY 1
    #define MOUSE_MODE_TILT_POSITION 2
    #define MOUSE_MODE_MOVEMENT_POSITION 3

    Accelerometer accel;

    // accelerometer measurements
    int xRaw, yRaw, zRaw;             // raw accel values (no calibration, no filtering)
    int x, y, z;                      // immediate accel values
    int x0, y0, z0;                   // last-iteration accel values
    int xBase, yBase, zBase;          // initial accel values
    int xHist[20], yHist[20], zHist[20];
    int xMin, yMin, zMin;             // minimum values (for calibration)
    int xMax, yMax, zMax;             // maximum values (for calibration)
    int ax, ay, az;                   // immediate accel tilt angles
    int ax0, ay0, az0;                // last-iteration accel tilt angles
    int axBase, ayBase, azBase;       // initial accel tilt angles
    boolean aset = false;             // bool to tell whether initial (*Base) values have been set yet
    
    // mouse measurements
    int mx, my;                       // absolute coordinates
    int mx0, my0;                     // last-iteration absolute coordinates
    int mdx, mdy;                     // relative movement
#endif /* ENABLE_ACCEL */

// basic timing and program flow control
int counter = 0;                      // loops at 1000
int ticks = 0;                        // ticks every 1000, never loops
unsigned long int t;                  // time/benchmark
long int i;                           // global loop index
boolean blink_led = false;            // state of

// glove-controlled options
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

int opt_mouse_mode = 2;                         // which mouse mode (0=disable, 1/2/3 as defined)
int opt_mouse_hold = 0;                         // temporarily hold mouse position for hand re-orientation
float opt_mouse_scale_mode1[] = { 1, 1 };       // speed scale [x,y] for mode 1 (tilt-velocity)
float opt_mouse_scale_mode2[] = { 1, 1 };       // speed scale [x,y] for mode 2 (tilt-position)
float opt_mouse_scale_mode3[] = { 1, 1 };       // speed scale [x,y] for mode 3 (movement-position)
float opt_kalman_constant = 0.3;
float opt_smooth_average = 3;

void setup() {
    Serial.begin(57600);
    Serial.println("info Keyglove device activated");

#ifdef ENABLE_TOUCH
    // initialize all I/O pins to INPUT mode and enable internal pullup resistors
    for (i = 0; i < KSP_TOTAL_SENSORS; i++) {
        pinMode(pins[i], INPUT);
        digitalWrite(pins[i], HIGH);
    }
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
#endif

#ifdef ENABLE_PS2
    // initialize PS/2 interfaces
    keyboard.initialize();
    mouse.initialize();
#endif /* ENABLE_PS2 */

#ifdef ENABLE_ACCEL
    // initialize accelerometer
    accel.powerOn();
    accel.setRate(50);
    x = y = z = 0;
    xMin = yMin = zMin = 0;
    xMax = yMax = zMax = 0;
    mx = my = mx0 = my0 = 0;
#endif /* ENABLE_ACCEL */
    
    // initialize benchmark
    t = millis();
}

void loop() {
#ifdef ENABLE_PS2
    if (digitalRead(KB_PIN_CLOCK) == LOW && digitalRead(KB_PIN_DATA) == LOW) {
        Serial.print("PS/2 host interrupt (keyboard)\n");
        ps2keyboardInterrupt();
    }
    if (digitalRead(MOUSE_PIN_CLOCK) == LOW && digitalRead(MOUSE_PIN_DATA) == LOW) {
        Serial.print("PS/2 host interrupt (mouse)\n");
        ps2mouseInterrupt();
    }
#endif /* ENABLE_PS2 */

#ifdef ENABLE_ACCEL
    if (counter % 10 == 0) {
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

        if (abs(x) < 256) {
            ax = abs(x) < 150
                     ? degrees(asin((float)x / 256))
                     : (x < 0)
                         ? (-90 + degrees(acos((float)x / 256)))
                         : (90 - degrees(acos((float)x / 256)));
        } else ax = (x < 0) ? -90 : 90;
        if (abs(y) < 256) {
            ax = abs(x) < 150
                     ? degrees(asin((float)y / 256))
                     : (y < 0)
                         ? (-90 + degrees(acos((float)y / 256)))
                         : (90 - degrees(acos((float)y / 256)));
        } else ay = (y < 0) ? -90 : 90;
        if (abs(z) < 256) {
            az = abs(z) < 150
                     ? degrees(asin((float)z / 256))
                     : (z < 0)
                         ? (-90 + degrees(acos((float)z / 256)))
                         : (90 - degrees(acos((float)z / 256)));
        } else az = (z < 0) ? -90 : 90;
        
#ifdef SERIAL_DEBUG_ACCEL
        Serial.print("accel ");
        Serial.print(xRaw); Serial.print(" ");
        Serial.print(yRaw); Serial.print(" ");
        Serial.print(zRaw); Serial.print(" ");
        Serial.print(x); Serial.print(" ");
        Serial.print(y); Serial.print(" ");
        Serial.print(z); Serial.print(" ");
        Serial.print(ax); Serial.print(" ");
        Serial.print(ay); Serial.print(" ");
        Serial.println(az);
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
                    mx -= (opt_mouse_scale_mode2[0]) * (float)(ax - axBase);
                    my -= (opt_mouse_scale_mode2[1]) * (float)(ay - ayBase);
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

#ifdef SERIAL_DEBUG_MOUSE
        Serial.print("mouse ");
        Serial.print(mx); Serial.print(" ");
        Serial.print(my); Serial.print(" ");
        Serial.print(mdx); Serial.print(" ");
        Serial.println(mdy);
#endif /* SERIAL_DEBUG_MOUSE */
    }
#endif /* ENABLE_ACCEL */

#ifdef ENABLE_TOUCH
    detect1 = 0;
    detect2 = 0;
    removing = false;

    // loop through every possible 1-to-1 sensor combination and record levels
    for (i = 0; i < KSI_TOTAL_BITS; i++) {
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
        d = millis();
#ifdef SERIAL_DEBUG_TOUCH
        Serial.print("touch ");
        for (i = 0; i < KSI_TOTAL_BITS; i++) {
            if (i < 32) Serial.print(bitRead(detect1, i));
            else Serial.print(bitRead(detect2, i - 32));
        }
        Serial.print("\n");
#endif /* SERIAL_DEBUG_TOUCH */
    } else if ((verify1 != sensors1 || verify2 != sensors2) && millis() - d >= opt_touch_detect_threshold) {
        // detection is over threshold and current readings are different from previous readings
        // check to see if they've just initiated or removed any touches
        if (verify1 > sensors1 || verify2 > sensors2) {
            adding = true;
        } else if (adding && (verify1 < sensors1 || verify2 < sensors2)) {
            adding = false;
            removing = true;
            String sdr = "";

            // actual keypress this time around so test for each possible combination

            // pinky finger
            if        (KG_Y7) {
                sdr = "release Y7";
                trigger(KGI_Y7);
            } else if (KG_JY) {
                sdr = "release JY";
                trigger(KGI_JY);
            } else if (KG_KY) {
                sdr = "release KY";
                trigger(KGI_KY);
            } else if (KG_LY) {
                sdr = "release LY";
                trigger(KGI_LY);
            } else if (KG_VY) {
                sdr = "release VY";
                trigger(KGI_VY);
            } else if (KG_WY) {
                sdr = "release WY";
                trigger(KGI_WY);
            } else if (KG_XY) {
                sdr = "release XY";
                trigger(KGI_XY);
            } else if (KG_J8) {
                sdr = "release J8";
                trigger(KGI_J8);

            // ring finger
            } else if (KG_Y6) {
                sdr = "release Y6";
                trigger(KGI_Y6);
            } else if (KG_GY) {
                sdr = "release GY";
                trigger(KGI_GY);
            } else if (KG_HY) {
                sdr = "release HY";
                trigger(KGI_HY);
            } else if (KG_IY) {
                sdr = "release IY";
                trigger(KGI_IY);
            } else if (KG_SY) {
                sdr = "release SY";
                trigger(KGI_SY);
            } else if (KG_TY) {
                sdr = "release TY";
                trigger(KGI_TY);
            } else if (KG_UY) {
                sdr = "release UY";
                trigger(KGI_UY);
            } else if (KG_G7) {
                sdr = "release G7";
                trigger(KGI_G7);
            } else if (KG_G8) {
                sdr = "release G8";
                trigger(KGI_G8);
            
            // middle finger
            } else if (KG_Y5) {
                sdr = "release Y5";
                trigger(KGI_Y5);
            } else if (KG_DY) {
                sdr = "release DY";
                trigger(KGI_DY);
            } else if (KG_EY) {
                sdr = "release EY";
                trigger(KGI_EY);
            } else if (KG_FY) {
                sdr = "release FY";
                trigger(KGI_FY);
            } else if (KG_PZ) {
                sdr = "release PZ";
                trigger(KGI_PZ);
            } else if (KG_PY) {
                sdr = "release PY";
                trigger(KGI_PY);
            } else if (KG_QZ) {
                sdr = "release QZ";
                trigger(KGI_QZ);
            } else if (KG_QY) {
                sdr = "release QY";
                trigger(KGI_QY);
            } else if (KG_RZ) {
                sdr = "release RZ";
                trigger(KGI_RZ);
            } else if (KG_RY) {
                sdr = "release RY";
                trigger(KGI_RY);
            } else if (KG_D4) {
                sdr = "release D4";
                trigger(KGI_D4);
            } else if (KG_D6) {
                sdr = "release D6";
                trigger(KGI_D6);
            } else if (KG_D8) {
                sdr = "release D8";
                trigger(KGI_D8);
            } else if (KG_DM) {
                sdr = "release DM";
                trigger(KGI_DM);
            
            // index finger
            } else if (KG_Y4) {
                sdr = "release Y4";
                trigger(KGI_Y4);
            } else if (KG_AY) {
                sdr = "release AY";
                trigger(KGI_AY);
            } else if (KG_BY) {
                sdr = "release BY";
                trigger(KGI_BY);
            } else if (KG_CY) {
                sdr = "release CY";
                trigger(KGI_CY);
            } else if (KG_MZ) {
                sdr = "release MZ";
                trigger(KGI_MZ);
            } else if (KG_MY) {
                sdr = "release MY";
                trigger(KGI_MY);
            } else if (KG_NZ) {
                sdr = "release NZ";
                trigger(KGI_NZ);
            } else if (KG_NY) {
                sdr = "release NY";
                trigger(KGI_NY);
            } else if (KG_OZ) {
                sdr = "release OZ";
                trigger(KGI_OZ);
            } else if (KG_OY) {
                sdr = "release OY";
                trigger(KGI_OY);
            } else if (KG_A8) {
                sdr = "release A8";
                trigger(KGI_A8);

            // lower thumb pad to fingernails
            } else if (KG_Z4) {
                sdr = "release Z4";
                trigger(KGI_AY);
            } else if (KG_Z4) {
                sdr = "release Z5";
                trigger(KGI_Z5);
            } else if (KG_Z6) {
                sdr = "release Z6";
                trigger(KGI_Z6);
            } else if (KG_Z7) {
                sdr = "release Z7";
                trigger(KGI_Z7);

            // index finger to palm
            } else if (KG_A1) {
                sdr = "release A1";
                trigger(KGI_A1);
            } else if (KG_A2) {
                sdr = "release A2";
                trigger(KGI_A2);
            } else if (KG_A3) {
                sdr = "release A3";
                trigger(KGI_A3);

            // middle finger to palm
            } else if (KG_D1) {
                sdr = "release D1";
                trigger(KGI_D1);
            } else if (KG_D2) {
                sdr = "release D2";
                trigger(KGI_D2);
            } else if (KG_D3) {
                sdr = "release D3";
                trigger(KGI_D3);

            // ring finger to palm
            } else if (KG_G1) {
                sdr = "release G1";
                trigger(KGI_G1);
            } else if (KG_G2) {
                sdr = "release G2";
                trigger(KGI_G2);
            } else if (KG_G3) {
                sdr = "release G3";
                trigger(KGI_G3);

            // pinky finger to palm
            } else if (KG_J1) {
                sdr = "release J1";
                trigger(KGI_J1);
            } else if (KG_J2) {
                sdr = "release J2";
                trigger(KGI_J2);
            } else if (KG_J3) {
                sdr = "release J3";
                trigger(KGI_J3);

            // thumb tip to palm
            } else if (KG_Y1) {
                sdr = "release Y1";
                trigger(KGI_AY);

            } else {
                sdr = "release UNDEFINED";
            }
#ifdef SERIAL_DEBUG_TOUCH
            Serial.println(sdr);
#endif /* SERIAL_DEBUG_TOUCH */
        }
        
        // set official sensor readings to current readings 
        sensors1 = verify1;
        sensors2 = verify2;
    }
    
    verify1 = detect1;
    verify2 = detect2;
#endif /* ENABLE_TOUCH */

    counter++;
    if (counter == 1000) {
#ifdef ENABLE_PS2
        /*keyboard.keydown(KEY_LSHIFT);
        keyboard.keydown(KEY_K);
        keyboard.keyup(KEY_K);
        keyboard.keyup(KEY_LSHIFT);*/
        //mouse.move(1, 1);
        //delay(10);
#endif
        counter = 0;
        ticks++;
#ifdef ENABLE_BLINK
        blink_led = !blink_led;
        if (blink_led) digitalWrite(13, HIGH);
        else digitalWrite(13, LOW);
#endif /* ENABLE_BLINK */
        t = millis();
    }
}

void trigger(unsigned int keycode) {
#ifdef ENABLE_PS2
    //keyboard.keypress(KEY_K);
#endif
    switch (keycode) {
        default: break;
    }
}
