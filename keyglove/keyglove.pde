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

#include <Wire.h>
#include "adxl345.h"

#define DETECT_THRESHOLD 20
#define SCALE_MOUSEX 4
#define SCALE_MOUSEY 3
//#define ENABLE_PS2
//#define ENABLE_USB
//#define ENABLE_BLUETOOTH

// trash these? probably
//#define SMOOTH_MOUSE 10
//#define SMOOTH_CURVE 4

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

#endif

#ifdef ENABLE_USB
#endif

#ifdef ENABLE_BLUETOOTH
#endif

#include "sensors.h"
#include "touchset.h"

#define MOUSE_MODE_TILT_VELOCITY 1
#define MOUSE_MODE_TILT_POSITION 2
#define MOUSE_MODE_MOVEMENT_POSITION 3

Accelerometer accel;
int accel_mode = MOUSE_MODE_TILT_POSITION;
int accel_hist_pos = 0;
int accel_hist_pos0 = 0;
#define ACCEL_HIST_SIZE 20
#define ACCEL_CALIBRATE_X (275.0/256.0)
#define ACCEL_CALIBRATE_Y (267.0/256.0)
#define ACCEL_CALIBRATE_Z (241.0/256.0)

// touch sensors
unsigned long sensors1 = 0, sensors2 = 0;
unsigned long detect1 = 0, detect2 = 0;
unsigned long verify1 = 0, verify2 = 0;

// accelerometer measurements
int x = 0, y = 0, z = 0;
int xHist[ACCEL_HIST_SIZE];
int yHist[ACCEL_HIST_SIZE];
int zHist[ACCEL_HIST_SIZE];
int ax, ay, az;
int axBase, ayBase, azBase;
boolean aset = false;

// mouse coordinates
int mx = 0, my = 0;
//int mxHist[SMOOTH_MOUSE];
//int myHist[SMOOTH_MOUSE];
//int mouseHist = 0;

// timing and program flow control
int counter = 0, ticks = 0;
unsigned long int t; // time var
unsigned long int d; // detection timestamp
long int i; // index var
boolean adding = false;
boolean removing = false;
boolean blink_led = false;

void setup() {
    Serial.begin(57600);
    Serial.println("Keyglove device activated");

    // initialize all I/O pins to INPUT mode and enable internal pullup resistors
    for (i = 0; i < KSP_TOTAL_SENSORS; i++) {
        pinMode(pins[i], INPUT);
        digitalWrite(pins[i], HIGH);
    }
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);

#ifdef ENABLE_PS2
    keyboard.initialize();
    mouse.initialize();
#endif

    // initialize accelerometer
    accel.powerOn();
    
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
#endif

    if (counter % 10 == 0) {
        accel.readAccel(&x, &y, &z);
        x = (float)x / ACCEL_CALIBRATE_X;
        y = (float)y / ACCEL_CALIBRATE_Y;
        z = (float)z / ACCEL_CALIBRATE_Z;
        
        xHist[accel_hist_pos] = x;
        yHist[accel_hist_pos] = y;
        zHist[accel_hist_pos] = z;

        // smooth out using very simple averaging
        Serial.print("precision ");
        Serial.print(x); Serial.print(" ");
        for (i = 1; i < ACCEL_HIST_SIZE; i++) x += xHist[(accel_hist_pos + i) % ACCEL_HIST_SIZE];
        x /= ACCEL_HIST_SIZE;
        
        Serial.print(y); Serial.print(" ");
        for (i = 1; i < ACCEL_HIST_SIZE; i++) y += yHist[(accel_hist_pos + i) % ACCEL_HIST_SIZE];
        y /= ACCEL_HIST_SIZE;
        
        Serial.println(z);
        for (i = 1; i < ACCEL_HIST_SIZE; i++) z += zHist[(accel_hist_pos + i) % ACCEL_HIST_SIZE];
        z /= ACCEL_HIST_SIZE;

        ax = (float)x / 256 * 90;
        ay = (float)y / 256 * 90;
        az = (float)z / 256 * 90;
        
        Serial.print("accel ");
        Serial.print(x); Serial.print(" ");
        Serial.print(y); Serial.print(" ");
        Serial.print(z); Serial.print(" ");
        Serial.print(ax); Serial.print(" ");
        Serial.print(ay); Serial.print(" ");
        Serial.println(az);

        accel_hist_pos0 = accel_hist_pos;
        accel_hist_pos = (accel_hist_pos + 1) % ACCEL_HIST_SIZE;
        switch (accel_mode) {
            case MOUSE_MODE_TILT_VELOCITY:
                if (aset) {
                    mx -= ax - axBase;
                    my -= ay - ayBase;
                    Serial.print("mouse ");
                    Serial.print(mx); Serial.print(" ");
                    //Serial.print("\tmy=");
                    Serial.println(my);
                }
                axBase = ax;
                ayBase = ay;
                azBase = az;
                aset = true;
                break;
            case MOUSE_MODE_TILT_POSITION:
                if (aset) {
                    mx -= (SCALE_MOUSEX) * (float)(ax - axBase);
                    my -= (SCALE_MOUSEY) * (float)(ay - ayBase);
                    //mxHist[mouseHist] = mx;
                    //myHist[mouseHist] = my;
                    
                    /*int xavg = 0;
                    int yavg = 0;
                    int used = 0;
                    int mmin = mx + my, mmax = mx + my;
                    int check = (float)SMOOTH_MOUSE / SMOOTH_CURVE;
                    int curveMax = SMOOTH_MOUSE;*/
                    
                    // typical sqrt(mmax - mmin) range is [SCALE, 10*SCALE], with SCALE being the
                    // value when you're holding relatively still.
                    
                    // we want to analyze more smoothing data when slower movement is happening,
                    // since fast movements are generally far less precise. So, for a SMOOTH_MOUSE
                    // setting of 20, we want to average 20 data points when sqrt(mmax - mmin) is
                    // SCALE, and only SMOOTH_MOUSE/SMOOTH_CURVE data points when it is 10*SCALE.
                    // (note that var 'check' = SMOOTH_MOUSE/SMOOTH_CURVE)
                    
                    /*for (int i = mouseHist; i >= 0 && used < curveMax; xavg += mxHist[i], yavg += myHist[i], i--, used++) {
                        mmin = min(mmin, mxHist[i] + myHist[i]);
                        mmax = max(mmax, mxHist[i] + myHist[i]);
                        if (used == check && mmax != mmin) curveMax = max(check, SMOOTH_MOUSE - SMOOTH_MOUSE*0.1*sqrt(mmax - mmin)/(SCALE_MOUSEX + SCALE_MOUSEY));
                    };
                    for (int i = SMOOTH_MOUSE - 1; i > mouseHist && used < curveMax; xavg += mxHist[i], yavg += myHist[i], i--, used++) {
                        mmin = min(mmin, mxHist[i] + myHist[i]);
                        mmax = max(mmax, mxHist[i] + myHist[i]);
                        if (used == check && mmax != mmin) curveMax = max(check, SMOOTH_MOUSE - SMOOTH_MOUSE*0.1*sqrt(mmax - mmin)/(SCALE_MOUSEX + SCALE_MOUSEY));
                    }
                    xavg = (float)xavg / used;
                    yavg = (float)yavg / used;
                    mouseHist = (mouseHist + 1) % SMOOTH_MOUSE;*/
                    
                    Serial.print("mouse ");
                    Serial.print(mx); Serial.print(" ");
                    Serial.println(my);
                }/* else {
                    for (int i = 0; i < mouseHist; mxHist[i] = ax, myHist[i] = ay, i++);
                }*/
                axBase = ax;
                ayBase = ay;
                azBase = az;
                aset = true;
                break;
            case MOUSE_MODE_MOVEMENT_POSITION:
                break;
        }
    }

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
            //Serial.print(bitRead(detect1, i));
        } else {
            // write value to sensors2
            if (digitalRead(p2) == LOW) bitSet(detect2, i - 32);
            //Serial.print(bitRead(detect2, i - 32));
        }
        pinMode(p1, INPUT);     // reset to INPUT mode
        digitalWrite(p1, HIGH); // enable pullup
    }
    //Serial.print("\n");
    
    // check to see if we need to reset detection threshold
    if (verify1 != detect1 || verify2 != detect2) {
        // current sensors different from last detection, so reset threshold
        Serial.print("touch ");
        for (i = 0; i < KSI_TOTAL_BITS; i++) {
            if (i < 32) Serial.print(bitRead(detect1, i));
            else Serial.print(bitRead(detect2, i - 32));
        }
        Serial.print("\n");
        d = millis();
    } else if ((verify1 != sensors1 || verify2 != sensors2) && millis() - d >= DETECT_THRESHOLD) {
        // detection is over threshold and current readings are different from previous readings
        // check to see if they've just initiated or removed any touches
        if (verify1 > sensors1 || verify2 > sensors2) {
            adding = true;
        } else if (adding && (verify1 < sensors1 || verify2 < sensors2)) {
            adding = false;
            removing = true;

            // actual keypress this time around so test for each possible combination
            /*Serial.print("AAAAABCMMNNOOYDDDDDDDDEFPPQQRRYGGGGGGHISTUYJJJJJKLVWXYYZZZZ\n");
            Serial.print("Y1238YYYZYZYZ4MY123468YYYZYZYZ5Y12378YYYYY6Y1238YYYYY714567\n");
            for (i = 0; i < KSI_TOTAL_BITS; i++) {
                if (i < 32) Serial.print(bitRead(sensors1, i));
                else Serial.print(bitRead(sensors2, i - 32));
            }
            Serial.print("\n");
            Serial.print("Base combination: ");*/
            if        (KG_AY) {
                Serial.println("release AY");
                trigger(TOUCH_AY);
            } else if (KG_A1) {
                Serial.println("release A1");
            } else if (KG_A2) {
                Serial.println("release A2");
            } else if (KG_A3) {
                Serial.println("release A3");
            } else if (KG_A8) {
                Serial.println("release A8");
            } else if (KG_BY) {
                Serial.println("release BY");
            } else if (KG_CY) {
                Serial.println("release CY");
            } else if (KG_MY) {
                Serial.println("release MY");
            } else if (KG_MZ) {
                Serial.println("release MZ");
            } else if (KG_NY) {
                Serial.println("release NY");
            } else if (KG_NZ) {
                Serial.println("release NZ");
            } else if (KG_OY) {
                Serial.println("release OY");
            } else if (KG_OZ) {
                Serial.println("release OZ");
            } else if (KG_Y4) {
                Serial.println("release Y4");
            } else if (KG_DM) {
                Serial.println("release DM");
            } else if (KG_DY) {
                Serial.println("release DY");
            } else if (KG_D1) {
                Serial.println("release D1");
            } else if (KG_D2) {
                Serial.println("release D2");
            } else if (KG_D3) {
                Serial.println("release D3");
            } else if (KG_D4) {
                Serial.println("release D4");
            } else if (KG_D6) {
                Serial.println("release D6");
            } else if (KG_D8) {
                Serial.println("release D8");
            } else if (KG_EY) {
                Serial.println("release EY");
            } else if (KG_FY) {
                Serial.println("release FY");
            } else if (KG_PY) {
                Serial.println("release PY");
            } else if (KG_PZ) {
                Serial.println("release PZ");
            } else if (KG_QY) {
                Serial.println("release QY");
            } else if (KG_QZ) {
                Serial.println("release QZ");
            } else if (KG_RY) {
                Serial.println("release RY");
            } else if (KG_RZ) {
                Serial.println("release RZ");
            } else if (KG_Y5) {
                Serial.println("release Y5");
            } else if (KG_GY) {
                Serial.println("release GY");
            } else if (KG_G1) {
                Serial.println("release G1");
            } else if (KG_G2) {
                Serial.println("release G2");
            } else if (KG_G3) {
                Serial.println("release G3");
            } else if (KG_G7) {
                Serial.println("release G7");
            } else if (KG_G8) {
                Serial.println("release G8");
            } else if (KG_HY) {
                Serial.println("release HY");
            } else if (KG_IY) {
                Serial.println("release IY");
            } else if (KG_SY) {
                Serial.println("release SY");
            } else if (KG_TY) {
                Serial.println("release TY");
            } else if (KG_UY) {
                Serial.println("release UY");
            } else if (KG_Y6) {
                Serial.println("release Y6");
            } else if (KG_JY) {
                Serial.println("release JY");
            } else if (KG_J1) {
                Serial.println("release J1");
            } else if (KG_J2) {
                Serial.println("release J2");
            } else if (KG_J3) {
                Serial.println("release J3");
            } else if (KG_J8) {
                Serial.println("release J8");
            } else if (KG_KY) {
                Serial.println("release KY");
            } else if (KG_LY) {
                Serial.println("release LY");
            } else if (KG_VY) {
                Serial.println("release VY");
            } else if (KG_WY) {
                Serial.println("release WY");
            } else if (KG_XY) {
                Serial.println("release XY");
            } else if (KG_Y7) {
                Serial.println("release Y7");
            } else if (KG_Y1) {
                Serial.println("release Y1");
            } else if (KG_Z4) {
                Serial.println("release Z4");
            } else if (KG_Z5) {
                Serial.println("release Z5");
            } else if (KG_Z6) {
                Serial.println("release Z6");
            } else if (KG_Z7) {
                Serial.println("release Z7");
            } else {
                Serial.println("release UNDEFINED");
            }
        }
        
        // set official sensor readings to current readings 
        sensors1 = verify1;
        sensors2 = verify2;
    }
    
    verify1 = detect1;
    verify2 = detect2;

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
        blink_led = !blink_led;
        if (blink_led) digitalWrite(13, HIGH);
        else digitalWrite(13, LOW);
        //Serial.print("1000 iterations: ");
        //Serial.print(millis() - t);
        //Serial.print("\n");
        t = millis();
    }
}

void trigger(unsigned int keycode) {
#ifdef ENABLE_PS2
    //keyboard.keypress(KEY_K);
#endif
    switch (keycode) {
        case TOUCH_AY: break;
        default: break;
    }
}
