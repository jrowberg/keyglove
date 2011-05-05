// Keyglove controller source code - Main setup/loop controller
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
unsigned char ide_workaround = 0;

/* ===============================================
 * CORE CONTROLLER SETTINGS
=============================================== */
#define USE_ARDUINO
//#define USE_TEENSY

/* ===============================================
 * CORE FEATURE SETTINGS
=============================================== */
//#define ENABLE_BLINK
#define ENABLE_TRICOLOR
#define ENABLE_BEEP
#define ENABLE_VIBRATE
//#define ENABLE_PS2
#define ENABLE_BLUETOOTH
#define ENABLE_TOUCH
#define ENABLE_ACCEL
#define ENABLE_GYRO
#define ENABLE_RX400
#ifdef USE_TEENSY
    //#define ENABLE_USB
#endif /* USE_TEENSY */


/* ===============================================
 * DEBUG COMPILER SETTINGS
=============================================== */
// useful for kg_visualize Processing sketch and other serial debugging
// define these and data will be sent over controller serial port
#define SERIAL_DEBUG
#define SERIAL_DEBUG_ACCEL
//#define SERIAL_DEBUG_GYRO
#define SERIAL_DEBUG_TOUCH
#define SERIAL_DEBUG_MOUSE
#define SERIAL_DEBUG_KEYBOARD
#define SERIAL_DEBUG_TOUCHSET
//#define SERIAL_DEBUG_BENCHMARK

// set correct pins based on host device
#include "pins.h"

/* ===============================================
 * TOUCHSET CONTROL CONSTANT DEFINITIONS
=============================================== */
#define KBEEP_OFF 0
#define KBEEP_LONGBEEP 1
#define KBEEP_LONGPULSE 2
#define KBEEP_SHORTBEEP 3
#define KBEEP_SHORTPULSE 4
#define KBEEP_SOLID 5

#define KLED_OFF 0
#define KLED_LONGBLINK 1
#define KLED_LONGPULSE 2
#define KLED_SHORTBLINK 3
#define KLED_SHORTPULSE 4
#define KLED_SOLID 5

#define KMOUSE_LEFT 1
#define KMOUSE_MIDDLE 2
#define KMOUSE_RIGHT 4
#define KMOUSE_MOVE 0
#define KMOUSE_SCROLL 1

#define STR_PRODUCT L"Keyglove"
#define KEYPAD_ASTERISK 85
#define KEY_BACKSLASH2 100
#define KEY_APP        101
#define KEY_POWER      102
#define KEYPAD_EQUAL   103
#define KEY_F13        104
#define KEY_F14        105
#define KEY_F15        106
#define KEY_F16        107
#define KEY_F17        108
#define KEY_F18        109
#define KEY_F19        110
#define KEY_F20        111
#define KEY_F21        112
#define KEY_F22        113
#define KEY_F23        114
#define KEY_F24        115
#define KEY_EXECUTE    116
#define KEY_HELP       117
#define KEY_MENU       118
#define KEY_SELECT     119
#define KEY_STOP       120
#define KEY_AGAIN      121
#define KEY_UNDO       122
#define KEY_CUT        123
#define KEY_COPY       124
#define KEY_PASTE      125
#define KEY_FIND       126
#define KEY_MUTE       127
#define KEY_VOLUP      128
#define KEY_VOLDOWN    129

#ifdef USE_ARDUINO
    #define MODIFIERKEY_CTRL        0x01
    #define MODIFIERKEY_SHIFT       0x02
    #define MODIFIERKEY_ALT         0x04
    #define MODIFIERKEY_GUI         0x08
    
    #define KEY_A           4
    #define KEY_B           5
    #define KEY_C           6
    #define KEY_D           7
    #define KEY_E           8
    #define KEY_F           9
    #define KEY_G           10
    #define KEY_H           11
    #define KEY_I           12
    #define KEY_J           13
    #define KEY_K           14
    #define KEY_L           15
    #define KEY_M           16
    #define KEY_N           17
    #define KEY_O           18
    #define KEY_P           19
    #define KEY_Q           20
    #define KEY_R           21
    #define KEY_S           22
    #define KEY_T           23
    #define KEY_U           24
    #define KEY_V           25
    #define KEY_W           26
    #define KEY_X           27
    #define KEY_Y           28
    #define KEY_Z           29
    #define KEY_1           30
    #define KEY_2           31
    #define KEY_3           32
    #define KEY_4           33
    #define KEY_5           34
    #define KEY_6           35
    #define KEY_7           36
    #define KEY_8           37
    #define KEY_9           38
    #define KEY_0           39
    #define KEY_ENTER       40
    #define KEY_ESC         41
    #define KEY_BACKSPACE   42
    #define KEY_TAB         43
    #define KEY_SPACE       44
    #define KEY_MINUS       45
    #define KEY_EQUAL       46
    #define KEY_LEFT_BRACE  47
    #define KEY_RIGHT_BRACE 48
    #define KEY_BACKSLASH   49
    #define KEY_NON_US_NUM  50
    #define KEY_SEMICOLON   51
    #define KEY_QUOTE       52
    #define KEY_TILDE       53
    #define KEY_COMMA       54
    #define KEY_PERIOD      55
    #define KEY_SLASH       56
    #define KEY_CAPS_LOCK   57
    #define KEY_F1          58
    #define KEY_F2          59
    #define KEY_F3          60
    #define KEY_F4          61
    #define KEY_F5          62
    #define KEY_F6          63
    #define KEY_F7          64
    #define KEY_F8          65
    #define KEY_F9          66
    #define KEY_F10         67
    #define KEY_F11         68
    #define KEY_F12         69
    #define KEY_PRINTSCREEN 70
    #define KEY_SCROLL_LOCK 71
    #define KEY_PAUSE       72
    #define KEY_INSERT      73
    #define KEY_HOME        74
    #define KEY_PAGE_UP     75
    #define KEY_DELETE      76
    #define KEY_END         77
    #define KEY_PAGE_DOWN   78
    #define KEY_RIGHT       79
    #define KEY_LEFT        80
    #define KEY_DOWN        81
    #define KEY_UP          82
    #define KEY_NUM_LOCK    83
    #define KEYPAD_SLASH    84
    #define KEYPAD_ASTERIX  85
    #define KEYPAD_MINUS    86
    #define KEYPAD_PLUS     87
    #define KEYPAD_ENTER    88
    #define KEYPAD_1        89
    #define KEYPAD_2        90
    #define KEYPAD_3        91
    #define KEYPAD_4        92
    #define KEYPAD_5        93
    #define KEYPAD_6        94
    #define KEYPAD_7        95
    #define KEYPAD_8        96
    #define KEYPAD_9        97
    #define KEYPAD_0        98
    #define KEYPAD_PERIOD   99
#endif /* USE_ARDUINO */

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
#endif /* ENABLE_USB */

/* ===============================================
 * BLUETOOTH DECLARATIONS
=============================================== */
#ifdef ENABLE_BLUETOOTH
    #include "iwrap.h"
#ifdef USE_TEENSY    
    HardwareSerial Uart = HardwareSerial();    
#endif /* USE_TEENSY */
#ifdef USE_ARDUINO 
    HardwareSerial Uart = Serial3;    
#endif /* USE_ARDUINO */
    iWRAP bluetooth = iWRAP(&Uart);
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
    #include "Adxl345.h"

    Accelerometer accel;

    // accelerometer measurements
    int accelTick;                    // history position counter
    int xRaw, yRaw, zRaw;             // raw accel values (no calibration, no filtering)
    int x, y, z;                      // immediate accel values
    int x0, y0, z0;                   // last-iteration accel values
    int x00, y00, z00;                // 2x-last-iteration accel values (for speed)
    int xv, yv, zv;                   // axial vectors
    long cx, cy, cz;                  // calibration offsets
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
    int calAccel = 0;
    boolean accelCalibrated = false;
    
    #define ENABLE_MOUSE
#endif /* ENABLE_ACCEL */

/* ===============================================
 * GYROSCOPE DECLARATIONS
=============================================== */
#ifdef ENABLE_GYRO
    #include <Wire.h>
    #include "ITG3200.h"

    ITG3200 gyro = ITG3200();

    // gyroscope measurements
    int gyroTick;                    // history position counter
    int gxRaw, gyRaw, gzRaw;         // raw accel values (no calibration, no filtering)
    int gx, gy, gz;                  // immediate gyro values
    int gx0, gy0, gz0;               // last-iteration gyro values
    int gx00, gy00, gz00;            // 2x-last-iteration gyro values (for speed)
    long cgx, cgy, cgz;              // calibration offsets
    int gxBase, gyBase, gzBase;      // initial gyro values
    int gxHist[20], gyHist[20], gzHist[20];  // history for averaging (smoothing)
    int gxMin, gyMin, gzMin;         // minimum values (for calibration)
    int gxMax, gyMax, gzMax;         // maximum values (for calibration)
    boolean gset = false;            // bool to tell whether initial (*Base) values have been set yet
    int calGyro = 0;
    boolean gyroCalibrated = false;
    
    #ifndef ENABLE_MOUSE
        #define ENABLE_MOUSE
    #endif
#endif /* ENABLE_GYRO */

/* ===============================================
 * LOGITECH HACK DECLARATIONS - Emulate an RX400 presenter
=============================================== */
#ifdef ENABLE_RX400
    #include "logitech.h"
    
    logitech RX400 = logitech();
    #define ENABLE_MOUSE
#endif /* ENABLE_RX400 */

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

#define MOUSE_MODE_TILT_VELOCITY 1
#define MOUSE_MODE_TILT_POSITION 2
#define MOUSE_MODE_MOVEMENT_POSITION 3
#define MOUSE_MODE_3D 4

#define SCROLL_MODE_TILT_VELOCITY 1
#define SCROLL_MODE_TILT_POSITION 2
#define SCROLL_MODE_MOVEMENT_POSITION 3

// basic timing and program flow control
int counter;                          // loops at 1000
int ticks;                            // ticks every 1000, never loops
unsigned long int t;                  // time/benchmark
long int i;                           // global loop index
boolean moveMouse;                    // whether any mouse movement should possibly occur
boolean scrollMouse;                  // whether any mouse scrolling should possibly occur

// mouse measurements
int mx, my, mz, sy;                   // absolute coordinates
int mx0, my0, mz0, sy0;               // last-iteration absolute coordinates
int mdx, mdy, mdz, sdy;               // relative movement
// (note z is for 3D movement, sy is for scrolling)

/* ===============================================
 * DEVICE-CONTROLLED OPTION DECLARATIONS
=============================================== */
int opt_touch_detect_threshold = 20;            // number of milliseconds required for a touch to register as real
int opt_enable_calibration = 0;                 // temporarily enable calibration mode (resets accel/gyro offset/calibrate)
//int opt_accel_offset[] = { 0, 0, 0 };           // amount to offset raw accelerometer readings [x,y,z]
//float opt_accel_calibrate[] = { 1, 1, 1 };      // amount to scale raw accelerometer readings [x,y,z] (post-offset)

int opt_accel_offset[] = { -12, 5, 2 };
float opt_accel_calibrate[] = { 0.952, 0.945, 1.04 };
// mine @ room temp:
// x={-257,281}, y={-276,267}, z={-258,254}
// x offset = -12, x calibrate = 0.952
// y offset = 5, y calibrate = 0.945
// z offset = 2, z calibrate = 1

//int opt_gyro_offset[] = { 0, 0, 0 };            // amount to offset raw gyroscope readings [x,y,z]
//float opt_gyro_calibrate[] = { 1, 1, 1 };       // amount to scale raw gyroscope readings [x,y,z] (post-offset)

int opt_gyro_offset[] = { -22, -115, -7 };
float opt_gyro_calibrate[] = { 1, 1, 1 };

int opt_motion_sampling_div = 15;                // how many ticks in between each accel/gyro reading
float opt_accel_kalman_constant = 0.6;
float opt_gyro_kalman_constant = 0.3;
int opt_accel_smooth_average = 4;
int opt_gyro_smooth_average = 0;
int opt_mouse_mode = 0;                         // which mouse move mode (0=disable, 1/2/3 as defined)
int opt_scroll_mode = 0;                        // which mouse scroll mode (0=disable, 1/2/3 as defined), uses mouse Y axis values
int opt_mouse_hold = 0;                         // temporarily hold mouse position for hand re-orientation
int opt_mouse_invert_x = 1;                     // invert x axis control
int opt_mouse_invert_y = 1;                     // invert y axis control
int opt_mouse_invert_z = 0;                     // invert z axis control
float opt_mouse_scale_mode1[] = { 1, 1 };       // speed scale [x,y] for mode 1 (tilt-velocity)
float opt_mouse_scale_mode2[] = { 1, 1 };       // speed scale [x,y] for mode 2 (tilt-position)
float opt_mouse_scale_mode3[] = { 1, 1 };       // speed scale [x,y] for mode 3 (movement-position)
float opt_mouse_scale_mode4[] = { 1, 1, 1 };    // speed scale [x,y,z] for mode 4 (3D)

int modifiersDown = 0;
int keysDown[] = { 0, 0, 0, 0, 0, 0 };
int mouseDown = 0;
int modeStack[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int modeStackPos = 0;

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
        Uart.begin(38400);
    #endif /* ENABLE_BLUETOOTH */

    /* ===============================================
     * INITIALIZE LOGITECH RX400 INTERFACE
    =============================================== */
    #ifdef ENABLE_RX400
        RX400.init();
    #endif /* ENABLE_RX400 */
        
    /* ===============================================
     * INITIALIZE ACCELEROMETER INTERFACE
    =============================================== */
    #ifdef ENABLE_ACCEL
        // initialize accelerometer
        accel.powerOn();
        accel.setRate(100);
        x = y = z = 0;
        cx = cy = cz = 0;
        accelTick = 0;
        accelCalibrated = false;
        xMin = yMin = zMin = 0;
        xMax = yMax = zMax = 0;
    #endif /* ENABLE_ACCEL */

    /* ===============================================
     * INITIALIZE GYROSCOPE INTERFACE
    =============================================== */
    #ifdef ENABLE_GYRO
        // initialize gyroscope
        gyro.init(ITG3200_ADDR_AD0_LOW); 
        gx = gy = gz = 0;
        gyroTick = 0;
        gyroCalibrated = false;
        gxMin = gyMin = gzMin = 0;
        gxMax = gyMax = gzMax = 0;
    #endif /* ENABLE_GYRO */

    mx = my = mz = mx0 = my0 = mz0 = 0;
    sy = sy0 = 0;

    /* ===============================================
     * INITIALIZE VIBRATION FEEDBACK
    =============================================== */
    #ifdef ENABLE_VIBRATE
        pinMode(VIBRATE_PIN, OUTPUT);
        digitalWrite(VIBRATE_PIN, LOW);
        delay(100);
        digitalWrite(VIBRATE_PIN, HIGH);
    #endif /* ENABLE_VIBRATE */
    
    /* ===============================================
     * INITIALIZE AUDIO FEEDBACK
    =============================================== */
    #ifdef ENABLE_BEEP    
        tone(SOUND_PIN, 1760, 25);
        delay(100);
        tone(SOUND_PIN, 1760, 25);
    #endif /* ENABLE_BEEP */
    
    #ifdef ENABLE_TOUCH
        setmode(0); // default touchset mode is always 0
    #endif /* ENABLE_TOUCH */

    // initialize benchmark
    t = millis();
}

void loop() {
    moveMouse = false;
    scrollMouse = false;
    if (counter % opt_motion_sampling_div == 0) {
        if (opt_mouse_mode > 0 || opt_scroll_mode > 0) {
            /* ===============================================
             * READ AND PROCESS ACCELEROMETER DATA
            =============================================== */
            #ifdef ENABLE_ACCEL
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
                x = x0 + (opt_accel_kalman_constant * (xRaw - x0));
                y = y0 + (opt_accel_kalman_constant * (yRaw - y0));
                z = z0 + (opt_accel_kalman_constant * (zRaw - z0));
                
                // averaging
                if (opt_accel_smooth_average > 0) {
                    for (i = 1; i < opt_accel_smooth_average; i++) {
                        x += xHist[(accelTick + i) % opt_accel_smooth_average];
                        y += yHist[(accelTick + i) % opt_accel_smooth_average];
                        z += zHist[(accelTick + i) % opt_accel_smooth_average];
                    }
                    x /= opt_accel_smooth_average;
                    y /= opt_accel_smooth_average;
                    z /= opt_accel_smooth_average;
                    xHist[accelTick % opt_accel_smooth_average] = x;
                    yHist[accelTick % opt_accel_smooth_average] = y;
                    zHist[accelTick % opt_accel_smooth_average] = z;
                }
                
                // calculate linear velocity
                xv += (x - cx); //((x - x0) - (x0 - x00));
                yv += (y - cy); //((y - y0) - (y0 - y00));
                zv += (z - cz); //((z - z0) - (z0 - z00));
    
                if (opt_enable_calibration) {
                    xMin = min(xMin, x);
                    yMin = min(yMin, y);
                    zMin = min(zMin, z);
                    xMax = max(xMax, x);
                    yMax = max(yMax, y);
                    zMax = max(zMax, z);
                    #ifdef SERIAL_DEBUG_ACCEL
                        Serial.print("calibrateaccel ");
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
                
                float xc = (float)x/256, yc = (float)y/256, zc = (float)z/256;
                ax = degrees(atan(xc / sqrt(yc*yc + zc*zc)));
                ay = degrees(atan(yc / sqrt(xc*xc + zc*zc)));
                az = degrees(atan(sqrt(xc*xc + yc*yc) / zc));
                if (z >= 0) {
                    // right side up
                    if (x >= 0 && y >= 0) {
                        // Q1, x+ / y+ / z+
                    } else if (x < 0 && y >= 0) {
                        // Q2, x- / y+ / z+
                    } else if (x < 0 && y < 0) {
                        // Q3, x- / y- / z+
                    } else {
                        // Q4, x+ / y- / z+
                    }
                } else {
                    // upside down
                    if (x >= 0 && y >= 0) {
                        // Q5, x+ / y+ / z-
                    } else if (x < 0 && y >= 0) {
                        // Q6, x- / y+ / z-
                    } else if (x < 0 && y < 0) {
                        // Q7, x- / y- / z-
                    } else {
                        // Q8, x+ / y- / z-
                    }
                }
                
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
        
                accelTick++;
            #endif /* ENABLE_ACCEL */
        
            /* ===============================================
             * READ AND PROCESS GYROSCOPE DATA
            =============================================== */
            #ifdef ENABLE_GYRO
                gx00 = gx0;
                gy00 = gy0;
                gz00 = gz0;
                gx0 = gx;
                gy0 = gy;
                gz0 = gz;
                gyro.readGyroRaw(&gxRaw, &gyRaw, &gzRaw);
                
                // offset
                gxRaw += opt_gyro_offset[0];
                gyRaw += opt_gyro_offset[1];
                gzRaw += opt_gyro_offset[2];
                
                // calibrate
                gxRaw = (float)gxRaw * opt_gyro_calibrate[0] / 15; //14.375;
                gyRaw = (float)gyRaw * opt_gyro_calibrate[1] / 15; //14.375;
                gzRaw = (float)gzRaw * opt_gyro_calibrate[2] / 15; //14.375;
                
                // Kalman filtering
                gx = gx0 + (opt_gyro_kalman_constant * (gxRaw - gx0));
                gy = gy0 + (opt_gyro_kalman_constant * (gyRaw - gy0));
                gz = gz0 + (opt_gyro_kalman_constant * (gzRaw - gz0));
                
                // averaging
                if (opt_gyro_smooth_average > 0) {
                    for (i = 1; i < opt_gyro_smooth_average; i++) {
                        gx += gxHist[(gyroTick + i) % opt_gyro_smooth_average];
                        gy += gyHist[(gyroTick + i) % opt_gyro_smooth_average];
                        gz += gzHist[(gyroTick + i) % opt_gyro_smooth_average];
                    }
                    gx /= opt_gyro_smooth_average;
                    gy /= opt_gyro_smooth_average;
                    gz /= opt_gyro_smooth_average;
                    gxHist[gyroTick % opt_gyro_smooth_average] = gx;
                    gyHist[gyroTick % opt_gyro_smooth_average] = gy;
                    gzHist[gyroTick % opt_gyro_smooth_average] = gz;
                }
                
                if (opt_enable_calibration) {
                    gxMin = min(gxMin, gx);
                    gyMin = min(gyMin, gy);
                    gzMin = min(gzMin, gz);
                    gxMax = max(gxMax, gx);
                    gyMax = max(gyMax, gy);
                    gzMax = max(gzMax, gz);
                    #ifdef SERIAL_DEBUG_GYRO
                        Serial.print("calibrategyro ");
                        Serial.print(gxMin); Serial.print(" ");
                        Serial.print(gyMin); Serial.print(" ");
                        Serial.print(gzMin); Serial.print(" ");
                        Serial.print(gxMax); Serial.print(" ");
                        Serial.print(gyMax); Serial.print(" ");
                        Serial.println(gzMax);
                    #endif
                }
        
                #ifdef SERIAL_DEBUG_GYRO
                    Serial.print("gyro ");
                    Serial.print(gxRaw); Serial.print(" ");
                    Serial.print(gyRaw); Serial.print(" ");
                    Serial.print(gzRaw); Serial.print(" ");
                    Serial.print(gx); Serial.print(" ");
                    Serial.print(gy); Serial.print(" ");
                    Serial.println(gz);
                #endif /* SERIAL_DEBUG_GYRO */
        
                gyroTick++;
            #endif /* ENABLE_GYRO */
        }
    
        #ifdef ENABLE_MOUSE
            switch (opt_mouse_mode) {
                case MOUSE_MODE_TILT_VELOCITY:
                    if (aset) {
                        mx0 = mx;
                        my0 = my;
                        mx -= (float)(ax - axBase) / opt_mouse_scale_mode1[0];
                        my -= (float)(ay - ayBase) / opt_mouse_scale_mode1[1];
                        moveMouse = true;
                    } else {
                        axBase = ax;
                        ayBase = ay;
                        azBase = az;
                        aset = true;
                    }
                    break;
                /*case MOUSE_MODE_TILT_POSITION:
                    if (aset) {
                        // progressive scaling
                        mx -= (pow(2, abs(axv)) * opt_mouse_scale_mode2[0]) * (float)(ax - axBase);
                        my -= (pow(2, abs(ayv)) * opt_mouse_scale_mode2[1]) * (float)(ay - ayBase);
                    }
                    axBase = ax;
                    ayBase = ay;
                    azBase = az;
                    aset = true;
                    break;*/
                case MOUSE_MODE_TILT_POSITION:
                    mx0 = mx;
                    my0 = my;
                    mx += (gy < 0) ? -sqrt(-gy) : sqrt(gy);
                    my -= (gx < 0) ? -sqrt(-gx) : sqrt(gx);
                    //mx += gy;
                    //my -= gx;
                    moveMouse = true;
                    break;
                case MOUSE_MODE_MOVEMENT_POSITION:
                    if (aset) {
                        mx0 = mx;
                        my0 = my;
                        mx -= (float)(x - xBase) / opt_mouse_scale_mode3[0];
                        my -= (float)((z - 256) - zBase) / opt_mouse_scale_mode3[1];
                        moveMouse = true;
                    } else {
                        xBase = x;
                        yBase = y;
                        zBase = z - 256;
                        aset = true;
                    }
                    break;
                case MOUSE_MODE_3D:
                    mx0 = mx;
                    my0 = my;
                    mz0 = mz;
                    mx -= xv / opt_mouse_scale_mode4[0];
                    my -= zv / opt_mouse_scale_mode4[1];
                    mz -= yv / opt_mouse_scale_mode4[2];
                    moveMouse = true;
                    break;
            }
            switch (opt_scroll_mode) {
                case SCROLL_MODE_TILT_VELOCITY: // gyro
                    if (aset) {
                        mx0 = mx;
                        my0 = my;
                        mx -= (float)(ax - axBase) / opt_mouse_scale_mode1[0];
                        my -= (float)(ay - ayBase) / opt_mouse_scale_mode1[1];
                        scrollMouse = true;
                    } else {
                        axBase = ax;
                        ayBase = ay;
                        azBase = az;
                        aset = true;
                    }
                    break;
                case SCROLL_MODE_TILT_POSITION: // gyro
                    sy0 = sy;
                    sy -= (gx < 0) ? -sqrt(-gx / 30) : sqrt(gx / 30);
                    scrollMouse = true;
                    break;
                case SCROLL_MODE_MOVEMENT_POSITION: // accel
                    if (aset) {
                        mx0 = mx;
                        my0 = my;
                        mx -= (float)(x - xBase) / opt_mouse_scale_mode3[0];
                        my -= (float)((z - 256) - zBase) / opt_mouse_scale_mode3[1];
                        scrollMouse = true;
                    } else {
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
        mdz = mz - mz0;
    
        if (opt_mouse_invert_x == 1) mdx = -mdx;
        if (opt_mouse_invert_y == 1) mdy = -mdy;
        if (opt_mouse_invert_z == 1) mdz = -mdz;
        if (moveMouse && opt_mouse_mode > 0 && (mdx != 0 || mdy != 0 || mdz != 0)) {
            #ifdef ENABLE_USB
                if (mdz > 0) Mouse.move(mdx, mdy, mdz);
                else Mouse.move(mdx, mdy);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                if (mdz > 0) bluetooth.move(mdx, mdy, mdz);
                else bluetooth.move(mdx, mdy);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                if (mdz > 0) RX400.move(mdx, mdy, mdz);
                else RX400.move(mdx, mdy);
            #endif /* ENABLE_RX400 */
            #ifdef SERIAL_DEBUG_MOUSE
                Serial.print("mouse ");
                Serial.print(mx); Serial.print(" ");
                Serial.print(my); Serial.print(" ");
                Serial.print(mdx); Serial.print(" ");
                Serial.print(mdy); Serial.print(" ");
                Serial.print(mdz); Serial.print(" ");
                Serial.println(mdz);
            #endif /* SERIAL_DEBUG_MOUSE */
        }
        if (scrollMouse && opt_scroll_mode > 0 && sdy != 0) {
            #ifdef ENABLE_USB
                Mouse.scroll(sdy);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_BLUETOOTH
                bluetooth.scroll(sdy);
            #endif /* ENABLE_BLUETOOTH */
            #ifdef ENABLE_RX400
                RX400.scroll(sdy);
            #endif /* ENABLE_RX400 */
            #ifdef SERIAL_DEBUG_MOUSE
                Serial.print("scroll ");
                Serial.print(sy); Serial.print(" ");
                Serial.println(sdy);
            #endif /* SERIAL_DEBUG_MOUSE */
        }
        #endif /* ENABLE_MOUSE */
    }
    
    #ifdef ENABLE_BLUETOOTH
        while (Uart.available() > 0) {
            bluetooth.parse(Uart.read());
        }
        while (Serial.available() > 0) {
            byte bs = Serial.read();
            Serial.print(bs, BYTE);
            Uart.print(bs, BYTE);
        }
    #endif /* ENABLE_BLUETOOTH */

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
        } else if ((verify1 != sensors1 || verify2 != sensors2) && millis() - touchTime >= opt_touch_detect_threshold) {
            // detection is over threshold and current readings are different from previous readings

            #ifdef SERIAL_DEBUG_TOUCH
                Serial.print("touch ");
                for (i = 0; i < KG_BASE_COMBINATIONS; i++) {
                    if (i < 32) Serial.print(bitRead(detect1, i));
                    else Serial.print(bitRead(detect2, i - 32));
                }
                Serial.print("\n");
            #endif /* SERIAL_DEBUG_TOUCH */

            // check to see if they've just initiated or removed any touches
            if (verify1 > sensors1 || verify2 > sensors2) {
                adding = true;

                // touch initiation, check for actions
                check_sensors_touch(verify1, verify2);
            } else if (adding && (verify1 < sensors1 || verify2 < sensors2)) {
                adding = false;
                removing = true;
    
                // touch release, check for actions
                check_sensors_release(sensors1, sensors2);
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
        #ifdef SERIAL_DEBUG_BENCHMARK
            Serial.print("benchmark 1000 ");
            Serial.print(millis() - t);
            Serial.print(" ");
            Serial.println(ticks);
        #endif /* SERIAL_DEBUG_BENCHMARK */
        t = millis();
    }
}

// touchset control functions
#ifdef ENABLE_TOUCH
    
    boolean modeCheck(int mode) {
        return modeStack[modeStackPos - 1] == mode;
    }
    
    void setmode(int mode) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset setmode ");
            Serial.println(mode);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        for (; modeStackPos > 0; modeStackPos--) deactivate_mode(modeStack[modeStackPos - 1]);
        modeStackPos = 1;
        modeStack[0] = mode;
        activate_mode(mode);
    }
    
    void togglemode(int mode) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset togglemode ");
            Serial.println(mode);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        // find the mode and disable it if it's in the stack
        int i = 0;
        for (i = 0; i < modeStackPos && modeStack[i] != mode; i++);
        if (i < modeStackPos) {
            // enabled, so turn it off
            deactivate_mode(mode);
            for (; i < modeStackPos; i++) modeStack[i - 1] = modeStack[i];
            modeStackPos--;
        } else {
            // not enabled, so turn it on
            pushmode(mode);
        }
    }
    
    void pushmode(int mode) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset pushmode ");
            Serial.println(mode);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        if (modeStackPos < 10) {
            modeStack[modeStackPos] = mode;
            modeStackPos++;
            activate_mode(mode);
        }
    }
    
    void popmode() {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.println("touchset popmode");
        #endif /* SERIAL_DEBUG_TOUCHSET */
        if (modeStackPos > 0) deactivate_mode(modeStack[--modeStackPos]);
        if (modeStackPos == 0) modeStack[modeStackPos++] = 0;
        activate_mode(modeStack[modeStackPos - 1]);
    }
    
    void mouseon(int mode) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset mouseon ");
            Serial.println(mode);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        if (mode == KMOUSE_MOVE) {
            opt_mouse_mode = MOUSE_MODE_TILT_VELOCITY;
        } else if (mode == KMOUSE_SCROLL) {
            opt_scroll_mode = SCROLL_MODE_TILT_POSITION;
        }
    }
    
    void mouseoff(int mode) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset mouseoff ");
            Serial.println(mode);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        if (mode == KMOUSE_MOVE) {
            opt_mouse_mode = 0;
        } else if (mode == KMOUSE_SCROLL) {
            opt_scroll_mode = 0;
        }
    }
    
    void mousedown(int button) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset mousedown ");
            Serial.println(button);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        mouseDown = mouseDown | button;
        #ifdef ENABLE_USB
            Mouse.set_buttons((mouseDown & 1) > 0 ? 1 : 0, (mouseDown & 2) > 0 ? 1 : 0, (mouseDown & 4) > 0 ? 1 : 0);
        #endif /* ENABLE_USB */
        #ifdef ENABLE_RX400
            RX400.set_buttons((mouseDown & 1) > 0 ? 1 : 0, (mouseDown & 2) > 0 ? 1 : 0, (mouseDown & 4) > 0 ? 1 : 0);
        #endif /* ENABLE_RX400 */
    }
    
    void mouseup(int button) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset mouseup ");
            Serial.println(button);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        if ((mouseDown & button) > 0) {
            mouseDown -= button;
            #ifdef ENABLE_USB
                Mouse.set_buttons((mouseDown & 1) > 0 ? 1 : 0, (mouseDown & 2) > 0 ? 1 : 0, (mouseDown & 4) > 0 ? 1 : 0);
            #endif /* ENABLE_USB */
            #ifdef ENABLE_RX400
                RX400.set_buttons((mouseDown & 1) > 0 ? 1 : 0, (mouseDown & 2) > 0 ? 1 : 0, (mouseDown & 4) > 0 ? 1 : 0);
            #endif /* ENABLE_RX400 */
        }
    }
    
    void mouseclick(int button) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset mouseclick ");
            Serial.println(button);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        mousedown(button);
        delay(5);
        mouseup(button);
    }
    
    void keydown(int code) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset keydown ");
            Serial.println(code);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        int usePos = 0;
        for (usePos = 0; usePos < 6 && keysDown[usePos] != 0; usePos++);
        if (usePos == 6) return; // out of HID keyboard buffer space, REALLY weird for the Keyglove!
        keysDown[usePos] = code;
        switch (usePos) {
            case 0:
                #ifdef ENABLE_USB
                    Keyboard.set_key1(code);
                #endif /* ENABLE_USB */
                #ifdef ENABLE_BLUETOOTH
                    bluetooth.set_key1(code);
                #endif /* ENABLE_BLUETOOTH */
                #ifdef ENABLE_RX400
                    RX400.set_key1(code);
                #endif /* ENABLE_RX400 */
                break;
            case 1:
                #ifdef ENABLE_USB
                    Keyboard.set_key2(code);
                #endif /* ENABLE_USB */
                #ifdef ENABLE_BLUETOOTH
                    bluetooth.set_key2(code);
                #endif /* ENABLE_BLUETOOTH */
                #ifdef ENABLE_RX400
                    RX400.set_key2(code);
                #endif /* ENABLE_RX400 */
                break;
            case 2:
                #ifdef ENABLE_USB
                    Keyboard.set_key3(code);
                #endif /* ENABLE_USB */
                #ifdef ENABLE_BLUETOOTH
                    bluetooth.set_key3(code);
                #endif /* ENABLE_BLUETOOTH */
                #ifdef ENABLE_RX400
                    RX400.set_key3(code);
                #endif /* ENABLE_RX400 */
                break;
            case 3:
                #ifdef ENABLE_USB
                    Keyboard.set_key4(code);
                #endif /* ENABLE_USB */
                #ifdef ENABLE_BLUETOOTH
                    bluetooth.set_key4(code);
                #endif /* ENABLE_BLUETOOTH */
                #ifdef ENABLE_RX400
                    RX400.set_key4(code);
                #endif /* ENABLE_RX400 */
                break;
            case 4:
                #ifdef ENABLE_USB
                    Keyboard.set_key5(code);
                #endif /* ENABLE_USB */
                #ifdef ENABLE_BLUETOOTH
                    bluetooth.set_key5(code);
                #endif /* ENABLE_BLUETOOTH */
                #ifdef ENABLE_RX400
                    RX400.set_key5(code);
                #endif /* ENABLE_RX400 */
                break;
            case 5:
                #ifdef ENABLE_USB
                    Keyboard.set_key6(code);
                #endif /* ENABLE_USB */
                #ifdef ENABLE_BLUETOOTH
                    bluetooth.set_key6(code);
                #endif /* ENABLE_BLUETOOTH */
                #ifdef ENABLE_RX400
                    RX400.set_key6(code);
                #endif /* ENABLE_RX400 */
                break;
        }
        #ifdef ENABLE_USB
            Keyboard.send_now();
        #endif /* ENABLE_USB */
        #ifdef ENABLE_BLUETOOTH
            bluetooth.send_now();
        #endif /* ENABLE_BLUETOOTH */
        #ifdef ENABLE_RX400
            RX400.send_now();
        #endif /* ENABLE_RX400 */
    }
    
    void keyup(int code) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset keyup ");
            Serial.println(code);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        int usePos = 0;
        for (usePos = 0; usePos < 6 && keysDown[usePos] != code; usePos++);
        if (usePos == 6) return; // key not currently down...oops.
        keysDown[usePos] = 0;
        switch (usePos) {
            case 0:
                #ifdef ENABLE_USB
                    Keyboard.set_key1(0);
                #endif /* ENABLE_USB */
                #ifdef ENABLE_BLUETOOTH
                    bluetooth.set_key1(0);
                #endif /* ENABLE_BLUETOOTH */
                #ifdef ENABLE_RX400
                    RX400.set_key1(0);
                #endif /* ENABLE_RX400 */
                break;
            case 1:
                #ifdef ENABLE_USB
                    Keyboard.set_key2(0);
                #endif /* ENABLE_USB */
                #ifdef ENABLE_BLUETOOTH
                    bluetooth.set_key2(0);
                #endif /* ENABLE_BLUETOOTH */
                #ifdef ENABLE_RX400
                    RX400.set_key2(0);
                #endif /* ENABLE_RX400 */
                break;
            case 2:
                #ifdef ENABLE_USB
                    Keyboard.set_key3(0);
                #endif /* ENABLE_USB */
                #ifdef ENABLE_BLUETOOTH
                    bluetooth.set_key3(0);
                #endif /* ENABLE_BLUETOOTH */
                #ifdef ENABLE_RX400
                    RX400.set_key3(0);
                #endif /* ENABLE_RX400 */
                break;
            case 3:
                #ifdef ENABLE_USB
                    Keyboard.set_key4(0);
                #endif /* ENABLE_USB */
                #ifdef ENABLE_BLUETOOTH
                    bluetooth.set_key4(0);
                #endif /* ENABLE_BLUETOOTH */
                #ifdef ENABLE_RX400
                    RX400.set_key4(0);
                #endif /* ENABLE_RX400 */
                break;
            case 4:
                #ifdef ENABLE_USB
                    Keyboard.set_key5(0);
                #endif /* ENABLE_USB */
                #ifdef ENABLE_BLUETOOTH
                    bluetooth.set_key5(0);
                #endif /* ENABLE_BLUETOOTH */
                #ifdef ENABLE_RX400
                    RX400.set_key5(0);
                #endif /* ENABLE_RX400 */
                break;
            case 5:
                #ifdef ENABLE_USB
                    Keyboard.set_key6(0);
                #endif /* ENABLE_USB */
                #ifdef ENABLE_BLUETOOTH
                    bluetooth.set_key6(0);
                #endif /* ENABLE_BLUETOOTH */
                #ifdef ENABLE_RX400
                    RX400.set_key6(0);
                #endif /* ENABLE_RX400 */
                break;
        }
        #ifdef ENABLE_USB
            Keyboard.send_now();
        #endif /* ENABLE_USB */
        #ifdef ENABLE_BLUETOOTH
            bluetooth.send_now();
        #endif /* ENABLE_BLUETOOTH */
        #ifdef ENABLE_RX400
            RX400.send_now();
        #endif /* ENABLE_RX400 */
    }
    
    void keypress(int code) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset keypress ");
            Serial.println(code);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        keydown(code);
        delay(5);
        keyup(code);
    }
    
    void modifierdown(int code) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset modifierdown ");
            Serial.println(code);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        modifiersDown = modifiersDown | code;
        #ifdef ENABLE_USB
            Keyboard.set_modifier(modifiersDown);
            Keyboard.send_now();
        #endif /* ENABLE_USB */
        #ifdef ENABLE_RX400       
            RX400.set_modifier(modifiersDown);
            RX400.send_now();
        #endif /* ENABLE_RX400 */
    }
    
    void modifierup(int code) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset modifierup ");
            Serial.println(code);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        if ((modifiersDown & code) > 0) {
            modifiersDown -= code;
            #ifdef ENABLE_USB
                Keyboard.set_modifier(modifiersDown);
                Keyboard.send_now();
            #endif /* ENABLE_USB */
            #ifdef ENABLE_RX400
                RX400.set_modifier(modifiersDown);
                RX400.send_now();
            #endif /* ENABLE_RX400 */
        }
    }
    
    void modifierpress(int code) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset modifierpress ");
            Serial.println(code);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        modifierdown(code);
        delay(5);
        modifierup(code);
    }
    
    void redled(int mode, int duration) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset redled ");
            Serial.print(mode);
            Serial.print(" ");
            Serial.println(duration);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        switch (mode) {
            case KLED_OFF: tricolorBlinkRed = 0; tricolor(0, -1, -1); break;
            case KLED_LONGBLINK: tricolorBlinkRed = 1; tricolor(1, -1, -1); break;
            case KLED_LONGPULSE: tricolorBlinkRed = 2; tricolor(1, -1, -1); break;
            case KLED_SHORTBLINK: tricolorBlinkRed = 3; tricolor(1, -1, -1); break;
            case KLED_SHORTPULSE: tricolorBlinkRed = 4; tricolor(1, -1, -1); break;
            case KLED_SOLID: tricolorBlinkRed = 0; tricolor(1, -1, -1); break;
        }
    }
    
    void greenled(int mode, int duration) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset greenled ");
            Serial.print(mode);
            Serial.print(" ");
            Serial.println(duration);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        switch (mode) {
            case KLED_OFF: tricolorBlinkGreen = 0; tricolor(-1, 0, -1); break;
            case KLED_LONGBLINK: tricolorBlinkGreen = 1; tricolor(-1, 1, -1); break;
            case KLED_LONGPULSE: tricolorBlinkGreen = 2; tricolor(-1, 1, -1); break;
            case KLED_SHORTBLINK: tricolorBlinkGreen = 3; tricolor(-1, 1, -1); break;
            case KLED_SHORTPULSE: tricolorBlinkGreen = 4; tricolor(-1, 1, -1); break;
            case KLED_SOLID: tricolorBlinkGreen = 0; tricolor(-1, 1, -1); break;
        }
    }
    
    void blueled(int mode, int duration) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset blueled ");
            Serial.print(mode);
            Serial.print(" ");
            Serial.println(duration);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        switch (mode) {
            case KLED_OFF: tricolorBlinkBlue = 0; tricolor(-1, -1, 0); break;
            case KLED_LONGBLINK: tricolorBlinkBlue = 1; tricolor(-1, -1, 1); break;
            case KLED_LONGPULSE: tricolorBlinkBlue = 2; tricolor(-1, -1, 1); break;
            case KLED_SHORTBLINK: tricolorBlinkBlue = 3; tricolor(-1, -1, 1); break;
            case KLED_SHORTPULSE: tricolorBlinkBlue = 4; tricolor(-1, -1, 1); break;
            case KLED_SOLID: tricolorBlinkBlue = 0; tricolor(-1, -1, 1); break;
        }
    }
    
    void beep(int pitch, int mode, int duration) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset beep ");
            Serial.print(pitch);
            Serial.print(" ");
            Serial.print(mode);
            Serial.print(" ");
            Serial.println(duration);
        #endif /* SERIAL_DEBUG_TOUCHSET */
        #ifdef ENABLE_BEEP
            if (mode == KBEEP_OFF) {
                noTone(SOUND_PIN);
                return;
            } 
            
            int duration_ms = 1000;
            if (mode == KBEEP_SHORTPULSE) {
                duration_ms = 100;
            } else if (mode == KBEEP_SHORTBEEP) {
                duration_ms = 25;
            }
            
            for (int i = 0; i < duration; i++) {
                if (i > 0) {
                    delay(80);
                }
                tone(SOUND_PIN, pitch, duration_ms);
            }
        #endif /* ENABLE_BEEP */
    }
    
    void vibrate(int mode, int duration) {
        #ifdef SERIAL_DEBUG_TOUCHSET
            Serial.print("touchset vibrate ");
            Serial.print(mode);
            Serial.print(" ");
            Serial.println(duration);
        #endif /* SERIAL_DEBUG_TOUCHSET */
    }
    
#endif /* ENABLE_TOUCH */

#ifdef ENABLE_ACCEL

class Quat {
    public:
        double heading;
        double attitude;
        double bank;

        double w;
        double x;
        double y;
        double z;
        
        double sqw;
        double sqx;
        double sqy;
        double sqz;
        
        Quat(double nw, double nx, double ny, double nz) {
            w = nw;
            x = nx;
            y = ny;
            z = nz;
            fix();
        }
        
        // thanks to http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
        void fix() {
            double sqw = w * w;
            double sqx = x * x;
            double sqy = y * y;
            double sqz = z * z;
            double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
            double test = x*y + z*w;
            if (test > 0.499*unit) { // singularity at north pole
                heading = 2 * atan2(x, w);
                attitude = PI/2;
                bank = 0;
                return;
            }
            if (test < -0.499*unit) { // singularity at south pole
                heading = -2 * atan2(x, w);
                attitude = -PI/2;
                bank = 0;
                return;
            }
            heading = atan2(2*y*w - 2*x*z, sqx - sqy - sqz + sqw);
            attitude = asin(2*test/unit);
            bank = atan2(2*x*w - 2*y*z , -sqx + sqy - sqz + sqw);
        }
};

#endif /* ENABLE_ACCEL */
