// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps 4.1)
// 6-axis orientation + raw magnetometer is provited by DMP, 9-axis fused with AK8975 compass
// 6/21/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2012-06-21 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

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

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_9Axis_MotionApps41.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;

/* =========================================================================
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
 * ========================================================================= */

/* =========================================================================
   NOTE: Arduino v1.0.1 with the Leonardo board generates a compile error
   when using Serial.write(buf, len). The Teapot output uses this method.
   The solution requires a modification to the Arduino USBAPI.h file, which
   is fortunately simple, but annoying. This will be fixed in the next IDE
   release. For more info, see these links:

   http://arduino.cc/forum/index.php/topic,109987.0.html
   http://code.google.com/p/arduino/issues/detail?id=958
 * ========================================================================= */



// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
//#define OUTPUT_READABLE_QUATERNION

// uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
// (in degrees) calculated from the quaternions coming from the FIFO.
// Note that Euler angles suffer from gimbal lock (for more info, see
// http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_EULER

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
// components with gravity removed. This acceleration reference frame is
// not compensated for orientation, so +X is always +X according to the
// sensor, just without the effects of gravity. If you want acceleration
// compensated for orientation, us OUTPUT_READABLE_WORLDACCEL instead.
//#define OUTPUT_READABLE_REALACCEL

// uncomment "OUTPUT_READABLE_WORLDACCEL" if you want to see acceleration
// components with gravity removed and adjusted for the world frame of
// reference (yaw is relative to initial orientation, since no magnetometer
// is present in this case). Could be quite handy in some cases.
//#define OUTPUT_READABLE_WORLDACCEL

// uncomment "OUTPUT_TEAPOT" if you want output that matches the
// format used for the InvenSense teapot demo
#define OUTPUT_TEAPOT



#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 gv;         // [x, y, z]            gyro sensor measurements
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}



// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // wait for ready
    Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();
    
    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_QUATERNION
            // display quaternion values in easy matrix form: w x y z
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            Serial.print("quat\t");
            Serial.print(q.w);
            Serial.print("\t");
            Serial.print(q.x);
            Serial.print("\t");
            Serial.print(q.y);
            Serial.print("\t");
            Serial.println(q.z);
        #endif

        #ifdef OUTPUT_READABLE_EULER
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetEuler(euler, &q);
            Serial.print("euler\t");
            Serial.print(euler[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(euler[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(euler[2] * 180/M_PI);
        #endif

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
        #endif

        #ifdef OUTPUT_READABLE_REALACCEL
            // display real acceleration, adjusted to remove gravity
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            Serial.print("areal\t");
            Serial.print(aaReal.x);
            Serial.print("\t");
            Serial.print(aaReal.y);
            Serial.print("\t");
            Serial.println(aaReal.z);
        #endif

        #ifdef OUTPUT_READABLE_FRAMEACCEL
            // display initial-frame acceleration, adjusted to remove gravity
            // and rotated based on known orientation from quaternion
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccelInWorld(&aaFrame, &aaReal, &q);
            Serial.print("aframe\t");
            Serial.print(aaFrame.x);
            Serial.print("\t");
            Serial.print(aaFrame.y);
            Serial.print("\t");
            Serial.println(aaFrame.z);
        #endif
    
        #ifdef OUTPUT_TEAPOT
            // display quaternion values in InvenSense Teapot demo format:
            teapotPacket[2] = fifoBuffer[0];
            teapotPacket[3] = fifoBuffer[1];
            teapotPacket[4] = fifoBuffer[4];
            teapotPacket[5] = fifoBuffer[5];
            teapotPacket[6] = fifoBuffer[8];
            teapotPacket[7] = fifoBuffer[9];
            teapotPacket[8] = fifoBuffer[12];
            teapotPacket[9] = fifoBuffer[13];
            Serial.write(teapotPacket, 14);
            teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
        #endif

        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }
}


// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_9Axis_MotionApps41.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file
#include "AK8975.h"
#include "helper_3dmath.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

// MPU-6050 evaluation board has AK8975 connected to MPU AUX lines on address 0x0E
AK8975 mag(0x0E);

// =============================================================
// =============================================================
//                            NOTE!
// =============================================================
// =============================================================
// In addition to connection 3.3v, GND, SDA, and SCL, this sketch
// depends on the MPU-6050's INT pin being connected to the
// Arduino's external interrupt #0 pin. On the Arduino Uno and
// Mega 2560, this is digital I/O pin 2.



// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
//#define OUTPUT_READABLE_QUATERNION

// uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
// (in degrees) calculated from the quaternions coming from the FIFO.
// Note that Euler angles suffer from gimbal lock (for more info, see
// http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_EULER

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires a gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_FRAMEACCEL" if you want to see acceleration
// components with gravity removed and adjusted for the initial "zero
// rotation" frame of reference. Could be quite handy in some cases.
//#define OUTPUT_READABLE_FRAMEACCEL

// uncomment "OUTPUT_TEAPOT" if you want output that matches the
// format used for the InvenSense teapot demo
#define OUTPUT_TEAPOT



// NOTE! Enabling FIFO debug will give you a boatload of extra information
// that you probably don't care about unless you are trying to mess with
// the internal DMP behavior or something. Just saying...
//#define FIFO_DEBUG_SIZE
//#define FIFO_DEBUG_DATA



#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;
bool dmpReady = false;

// FIFO/interrupt vars
uint16_t fifoCount;
uint8_t fifoBuffer[128];
uint8_t mpuIntStatus;

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 gv;         // [x, y, z]            gyro sensor measurements
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 mf;         // [x, y, z]            magnetometer container
VectorFloat mfComp;     // [x, y, z]            tilt-adjusted magnetometer container
float heading;          //                      compass heading in degrees
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaFrame;    // [x, y, z]            initial-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

// boolean to indicate whether the MPU interrupt pin has changed
volatile bool dmpIntChange = false;

void dmpDataReady() {
    dmpIntChange = true;
}



// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    Serial.begin(115200);

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize MPU
    DEBUG_PRINTLN(F("Initializing MPU-6050..."));
    accelgyro.initialize();

    // verify MPU connection
    Serial.println(F("Testing MPU6050 connection..."));
    Serial.println(accelgyro.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // enable MPU AUX I2C bypass mode
    Serial.println(F("Enabling AUX I2C bypass mode..."));
    accelgyro.setI2CBypassEnabled(true);

    // initialize AKM
    DEBUG_PRINTLN(F("Initializing AK8975..."));
    mag.initialize();

    // verify AKM connection
    Serial.println(F("Testing AK8975 connection..."));
    Serial.println(mag.testConnection() ? F("AK8975 connection successful") : F("AK8975 connection failed"));

    // wait for ready
    Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    uint8_t status = accelgyro.dmpInitialize();
    if (status == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        accelgyro.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        dmpReady = true;
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(status);
        Serial.println(F(")"));
    }

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for DMP interrupt
    while (!dmpIntChange);

    // read DMP interrupt status (clears INT flag since LATCH is enabled)
    mpuIntStatus = accelgyro.getIntStatus();
    dmpIntChange = false;

    // wait for correct FIFO packet size (48 bytes for MotionApps 4.1 (MotionFit) DMP output)
    while ((fifoCount = accelgyro.getFIFOCount()) < 48);

    /* ================================================================================================ *
     | Default MotionApps v4.1 48-byte FIFO packet structure:                                           |
     |                                                                                                  |
     | [QUAT W][      ][QUAT X][      ][QUAT Y][      ][QUAT Z][      ][GYRO X][      ][GYRO Y][      ] |
     |   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  |
     |                                                                                                  |
     | [GYRO Z][      ][MAG X ][MAG Y ][MAG Z ][ACC X ][      ][ACC Y ][      ][ACC Z ][      ][      ] |
     |  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  |
     * ================================================================================================ */

    // read FIFO content
    accelgyro.getFIFOBytes(fifoBuffer, min(128, fifoCount));

    #ifdef FIFO_DEBUG_SIZE
        // display FIFO count for debugging
        Serial.print(F("FIFO size:\t"));
        Serial.println(fifoCount);
    #endif

    #ifdef FIFO_DEBUG_DATA
        // display FIFO content for debugging
        Serial.print(F("FIFO data:\t"));
        for (uint8_t k = 0; k < 48; k++) {
            if (fifoBuffer[k] < 0x10) Serial.print("0");
            Serial.print(fifoBuffer[k], HEX);
            Serial.print(" ");
        }
        Serial.println("");
    #endif

    if (fifoCount == 1024) {
        // overflow (this should never happen)
        accelgyro.resetFIFO();
    } else {
        // get quaternion from FIFO packet
        q.w = (float)((fifoBuffer[0] << 8) + fifoBuffer[1]) / 16384;   // w
        q.x = (float)((fifoBuffer[4] << 8) + fifoBuffer[5]) / 16384;   // x
        q.y = (float)((fifoBuffer[8] << 8) + fifoBuffer[9]) / 16384;   // y
        q.z = (float)((fifoBuffer[12] << 8) + fifoBuffer[13]) / 16384; // z

        // get rotational velocity from FIFO packet
        gv.x = (int16_t)((fifoBuffer[16] << 8) + fifoBuffer[17]);
        gv.y = (int16_t)((fifoBuffer[20] << 8) + fifoBuffer[21]);
        gv.z = (int16_t)((fifoBuffer[24] << 8) + fifoBuffer[25]);

        // get magnetic flux from FIFO packet
        mf.x = (int16_t)((fifoBuffer[28] << 8) + fifoBuffer[29]);
        mf.y = (int16_t)((fifoBuffer[30] << 8) + fifoBuffer[31]);
        mf.z = (int16_t)((fifoBuffer[32] << 8) + fifoBuffer[33]);

        // get linear acceleration from FIFO packet
        aa.x = (int16_t)((fifoBuffer[34] << 8) + fifoBuffer[35]);
        aa.y = (int16_t)((fifoBuffer[38] << 8) + fifoBuffer[39]);
        aa.z = (int16_t)((fifoBuffer[42] << 8) + fifoBuffer[43]);
        
        // calculate gravity vector
        gravity.x = 2 * (q.x*q.z - q.w*q.y);
        gravity.y = 2 * (q.w*q.x + q.y*q.z);
        gravity.z = q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z;

        // calculate Euler angles (psi/theta/phi)
        // psi
        euler[0] = atan2(2*q.x*q.y - 2*q.w*q.z, 2*q.w*q.w + 2*q.x*q.x - 1);
        // theta
        euler[1] = -asin(2*q.x*q.z + 2*q.w*q.y);
        // phi
        euler[2] = atan2(2*q.y*q.z - 2*q.w*q.x, 2*q.w*q.w + 2*q.z*q.z - 1);

        // yaw: (about Z axis)
        ypr[0] = euler[0]; //atan2(2*q.x*q.y - 2*q.w*q.z, 2*q.w*q.w + 2*q.x*q.x - 1);
                 // ^^^ yaw is the same as psi above
        // pitch: (nose up/down, about Y axis)
        ypr[1] = atan(gravity.x / sqrt(gravity.y*gravity.y + gravity.z*gravity.z));
        // roll: (tilt left/right, about X axis)
        ypr[2] = atan(gravity.y / sqrt(gravity.x*gravity.x + gravity.z*gravity.z));

        // non-tilt-compensated heading calculation, works if X/Y axes are
        // parallel with the surface of the earth (i.e. compass is level)
        heading = atan2((double)mf.y, (double)mf.x) + M_PI; // radians

        /*
        // tilt-compensated heading calculation (not exactly working yet...)
        float cosRoll = cos(ypr[2]);
        float sinRoll = 1 - (cosRoll * cosRoll);
        float cosPitch = cos(ypr[1]);
        float sinPitch = 1  - (cosPitch * cosPitch);

        // tilt-compensated magnetic field X component:
        mfComp.x = mf.x * cosPitch + mf.y * sinRoll * sinPitch + mf.z * cosRoll * sinPitch;
        // tilt-compensated magnetic field Y component:
        mfComp.y = mf.y * cosRoll - mf.z * sinRoll;
        // compensated magnetic heading
        heading = atan2(-mfComp.y, mfComp.x); // radians

        if (heading < 0) heading += (2*M_PI);
        else if (heading > (2*M_PI)) heading -= (2*M_PI);
        */

        // correct for declination if given
        float declination = 0; // depends on global position
        heading += declination;

        /*Serial.print(gv.x);
        Serial.print("\t");
        Serial.print(gv.y);
        Serial.print("\t");
        Serial.print(gv.z);
        Serial.print("\t\t");

        Serial.print(aa.x);
        Serial.print("\t");
        Serial.print(aa.y);
        Serial.print("\t");
        Serial.print(aa.z);
        Serial.print("\t\t");*/

        Serial.print(mf.x);
        Serial.print("\t");
        Serial.print(mf.y);
        Serial.print("\t");
        Serial.print(mf.z);
        Serial.print("\t");
        Serial.println(heading * 180/M_PI); //*/

        #ifdef OUTPUT_READABLE_QUATERNION
            // display quaternion values in easy matrix form: w x y z
            Serial.print("quat\t");
            Serial.print(q.w);
            Serial.print("\t");
            Serial.print(q.x);
            Serial.print("\t");
            Serial.print(q.y);
            Serial.print("\t");
            Serial.println(q.z);
        #endif

        #ifdef OUTPUT_READABLE_EULER
            // display Euler angles in degrees: x y z
            Serial.print("euler\t");
            Serial.print(euler[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(euler[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(euler[2] * 180/M_PI);
        #endif
    
        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display yaw/pitch/roll angles in degrees:
            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
        #endif

        #ifdef OUTPUT_READABLE_FRAMEACCEL
            // display initial-frame acceleration, adjusted to remove gravity
            // and rotated based on known orientation from quaternion
    
            // get rid of the gravity component (+1g = +16384 in standard DMP)
            aaReal.x = (aa.x - gravity.x*16384);
            aaReal.y = (aa.y - gravity.y*16384);
            aaReal.z = (aa.z - gravity.z*16384);
            
            // rotate measured 3D acceleration vector into original state
            // frame of reference based on orientation quaternion
            aaFrame = aaReal.getRotated(q);

            Serial.print("aframe\t");
            Serial.print(aaFrame.x);
            Serial.print("\t");
            Serial.print(aaFrame.y);
            Serial.print("\t");
            Serial.println(aaFrame.z);
        #endif
    
        #ifdef OUTPUT_TEAPOT
            // display quaternion values in InvenSense Teapot demo format:
            teapotPacket[2] = fifoBuffer[0];
            teapotPacket[3] = fifoBuffer[1];
            teapotPacket[4] = fifoBuffer[4];
            teapotPacket[5] = fifoBuffer[5];
            teapotPacket[6] = fifoBuffer[8];
            teapotPacket[7] = fifoBuffer[9];
            teapotPacket[8] = fifoBuffer[12];
            teapotPacket[9] = fifoBuffer[13];
            Serial.write(teapotPacket, 14);
            teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
        #endif

        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }
}
