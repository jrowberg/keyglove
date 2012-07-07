// I2C device class (I2Cdev) demonstration Arduino sketch for ADXL345/ITG3200 classes
// 10/29/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2011-10-29 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
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

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev, ITG3200, and ADXL345 must be installed as libraries, or else the
// .cpp/.h files for all three classes must be in the include path of your project
#include "I2Cdev.h"
#include "ADXL345.h"
#include "ITG3200.h"

// ADXL345 class default I2C address is 0x53
// specific I2C addresses may be passed as a parameter here
// ALT low = 0x53 (default for SparkFun 6DOF board)
// ALT high = 0x1D
ADXL345 accel;

// ITG3200 class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun 6DOF board)
// AD0 high = 0x69 (default for SparkFun ITG-3200 standalone board)
ITG3200 gyro;

// 4D quaternion representation
class Quaternion {
    public:
        float w;
        float x;
        float y;
        float z;
        
        Quaternion() {
            w = 1.0f;
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
        }
        
        Quaternion(float nw, float nx, float ny, float nz) {
            w = nw;
            x = nx;
            y = ny;
            z = nz;
        }

        Quaternion getProduct(Quaternion q) {
            return Quaternion(
                w*q.w - x*q.x - y*q.y - z*q.z,  // new w
                w*q.x + x*q.w + y*q.z - z*q.y,  // new x
                w*q.y - x*q.z + y*q.w + z*q.x,  // new y
                w*q.z + x*q.y - y*q.x + z*q.w); // new z
        }

        Quaternion getConjugate() {
            return Quaternion(w, -x, -y, -z);
        }
        
        float getMagnitude() {
            return sqrt(w*w + x*x + y*y + z*z);
        }
        
        void normalize() {
            float m = getMagnitude();
            w /= m;
            x /= m;
            y /= m;
            z /= m;
        }
        
        Quaternion getNormalized() {
            Quaternion r(w, x, y, z);
            r.normalize();
            return r;
        }
};

// 3D vector using "int16_t" data types (for sensor readings)
class VectorInt16 {
    public:
        int16_t x;
        int16_t y;
        int16_t z;

        VectorInt16() {
            x = 0;
            y = 0;
            z = 0;
        }
        
        VectorInt16(int16_t nx, int16_t ny, int16_t nz) {
            x = nx;
            y = ny;
            z = nz;
        }

        float getMagnitude() {
            return sqrt(x*x + y*y + z*z);
        }

        void normalize() {
            float m = getMagnitude();
            x /= m;
            y /= m;
            z /= m;
        }
        
        VectorInt16 getNormalized() {
            VectorInt16 r(x, y, z);
            r.normalize();
            return r;
        }
        
        void rotate(Quaternion q) {
            // http://www.cprogramming.com/tutorial/3d/quaternions.html
            // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/index.htm
            // http://content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
            // ^ or: http://webcache.googleusercontent.com/search?q=cache:xgJAp3bDNhQJ:content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation&hl=en&gl=us&strip=1

            // P_out = q * P_in * conj(q)
            // - P_out is the output vector
            // - q is the orientation quaternion
            // - P_in is the input vector (a*aReal)
            // - conj(q) is the conjugate of the orientation quaternion (q=[w,x,y,z], q*=[w,-x,-y,-z])
            Quaternion p(0, x, y, z);

            // quaternion multiplication: q * p, stored back in p
            p = q.getProduct(p);

            // quaternion multiplication: p * conj(q), stored back in p
            p = p.getProduct(q.getConjugate());

            // p quaternion is now [0, x', y', z']
            x = p.x;
            y = p.y;
            z = p.z;
        }

        VectorInt16 getRotated(Quaternion q) {
            VectorInt16 r(x, y, z);
            r.rotate(q);
            return r;
        }
};

// 3D vector using "float" data types (for gravity, IMU inputs)
class VectorFloat {
    public:
        float x;
        float y;
        float z;

        VectorFloat() {
            x = 0;
            y = 0;
            z = 0;
        }
        
        VectorFloat(float nx, float ny, float nz) {
            x = nx;
            y = ny;
            z = nz;
        }

        float getMagnitude() {
            return sqrt(x*x + y*y + z*z);
        }

        void normalize() {
            float m = getMagnitude();
            x /= m;
            y /= m;
            z /= m;
        }
        
        VectorFloat getNormalized() {
            VectorFloat r(x, y, z);
            r.normalize();
            return r;
        }
        
        void rotate(Quaternion q) {
            Quaternion p(0, x, y, z);

            // quaternion multiplication: q * p, stored back in p
            p = q.getProduct(p);

            // quaternion multiplication: p * conj(q), stored back in p
            p = p.getProduct(q.getConjugate());

            // p quaternion is now [0, x', y', z']
            x = p.x;
            y = p.y;
            z = p.z;
        }

        VectorFloat getRotated(Quaternion q) {
            VectorFloat r(x, y, z);
            r.rotate(q);
            return r;
        }
};

// =============================================================================
// 3D DATA VARIABLES AND OBJECTS
// =============================================================================
VectorInt16 aa;         // accelerometer's "acceleration" measurements
VectorInt16 aaOffset;   // offset for accelerometer zero-calibration
VectorInt16 gv;         // gyroscope's "angular velocity" measurements
VectorInt16 gvOffset;   // offset for gyroscope zero-calibration
VectorFloat aaIMU;      // float-based acceleration vector (sensor gives int16, IMU filter needs floats)
VectorFloat gvIMU;      // float-based rotation vector (sensor gives int16, IMU filter needs floats)
VectorFloat gravity;    // calculated gravity vector to remove from accelerometer measurements
VectorInt16 aaReal;     // gravity-free acceleration vector in device's rotated frame of reference
VectorInt16 aaFrame;    // gravity-free acceleration vector in original non-rotated frame of reference
VectorInt16 aaFrame0;   // ^ last-iteration value of above, for approximating the integral to get velocity
VectorInt16 avFrame;    // velocity vector in original non-rotated frame of reference
VectorInt16 avFrame0;   // ^ last-iteration value of above, for approximating the integral to get position
VectorInt16 avFrameRef; // ^ reference values for guessing whether we've stopped moving or not
VectorInt16 apFrame;    // position vector in original non-rotated frame of reference

float angles[3];
float ypr[3];

// NOTE: the integrations done here to get velocity from acceleration and
// position from velocity are mathematically sound from theoretically, but the
// reality is that there is quite a bit of error because even small amounts of
// noise or jitter in the raw acceleration readings get exponentially bigger as
// we integrate. This means you shouldn't count on this for very accurate
// results. You can, however, get excellent orientation info (although the yaw,
// or heading, is estimated from the gyroscope since there not absolute value
// coming from a magnetometer). You can also get good enough info for rough
// velocity measurements, gesture detection, etc.

// system constants
#define delta_t 0.01f // sampling period in seconds (shown as 10 ms, or 100Hz)
#define gyroMeasError 3.14159265358979f * (5.0f / 180.0f) // gyroscope measurement error in rad/s (shown as 5 deg/s)
#define beta sqrt(3.0f / 4.0f) * gyroMeasError // compute beta

// global system variables
Quaternion q;           // orientation calculated by IMU
Quaternion q_dir;       // derivative of orientation quaternion
Quaternion q_gerr;      // gyroscope error
Quaternion q_half;      // clock-cycle-saving storage for 0.5x calculations
Quaternion q_two;       // clock-cycle-saving storage for 2x calculations

float q_objf[3];        // objective function elements
float J_11or24;         // |
float J_12or23;         // |
float J_13or22;         // + objective function Jacobian elements
float J_14or21;         // |
float J_32, J_33;       // |

// main IMU filter (thanks to Madgwick report)
void update_imu_filter() {
    // note:
    //  - gvIMU is angular velocity from gyroscope, float datatype elements
    //  - aaIMU is linear acceleration from accelerometer, float datatype elements

    // auxiliary variables to avoid repeated calculations
    q_half.w = 0.5f * q.w;
    q_half.x = 0.5f * q.x;
    q_half.y = 0.5f * q.y;
    q_half.z = 0.5f * q.z;
    q_two.w = 2.0f * q.w;
    q_two.x = 2.0f * q.x;
    q_two.y = 2.0f * q.y;

    // normalize the accelerometer measurement
    aaIMU.normalize();

    // compute the objective function and Jacobian
    q_objf[0] = q_two.x * q.z - q_two.w * q.y - aaIMU.x;
    q_objf[1] = q_two.w * q.x + q_two.y * q.z - aaIMU.y;
    q_objf[2] = 1.0f - q_two.x * q.x - q_two.y * q.y - aaIMU.z;
    J_11or24 = q_two.y; // J_11 negated in matrix multiplication
    J_12or23 = 2.0f * q.z;
    J_13or22 = q_two.w; // J_12 negated in matrix multiplication
    J_14or21 = q_two.x;
    J_32 = 2.0f * J_14or21; // negated in matrix multiplication
    J_33 = 2.0f * J_11or24; // negated in matrix multiplication
    
    // compute the gradient (matrix multiplication)
    q_gerr.w = J_14or21 * q_objf[1] - J_11or24 * q_objf[0];
    q_gerr.x = J_12or23 * q_objf[0] + J_13or22 * q_objf[1] - J_32 * q_objf[2];
    q_gerr.y = J_12or23 * q_objf[1] - J_33 * q_objf[2] - J_13or22 * q_objf[0];
    q_gerr.z = J_14or21 * q_objf[0] + J_11or24 * q_objf[1];

    // normalize the gradient
    q_gerr.normalize();

    // compute the quaternion derivative measured by gyroscopes
    q_dir.w = -q_half.x * gvIMU.x - q_half.y * gvIMU.y - q_half.z * gvIMU.z;
    q_dir.x = q_half.w * gvIMU.x + q_half.y * gvIMU.z - q_half.z * gvIMU.y;
    q_dir.y = q_half.w * gvIMU.y - q_half.x * gvIMU.z + q_half.z * gvIMU.x;
    q_dir.z = q_half.w * gvIMU.z + q_half.x * gvIMU.y - q_half.y * gvIMU.x;
    
    // compute, then integrate the estimated quaternion derivative
    q.w += (q_dir.w - (beta * q_gerr.w)) * delta_t;
    q.x += (q_dir.x - (beta * q_gerr.x)) * delta_t;
    q.y += (q_dir.y - (beta * q_gerr.y)) * delta_t;
    q.z += (q_dir.z - (beta * q_gerr.z)) * delta_t;

    // normalize quaternion
    q.normalize();

    /*Serial.print(q.w); Serial.print("\t");
    Serial.print(q.x); Serial.print("\t");
    Serial.print(q.y); Serial.print("\t");
    Serial.println(q.z); //*/

    // calculate Euler angles (in degrees, not radians)
    // psi:
    angles[0] = atan2(2 * q.x * q.y - 2 * q.w * q.z, 2 * q.w*q.w + 2 * q.x * q.x - 1) * 180/M_PI;
    // theta:
    angles[1] = -asin(2 * q.x * q.z + 2 * q.w * q.y) * 180/M_PI;
    // phi:
    angles[2] = atan2(2 * q.y * q.z - 2 * q.w * q.x, 2 * q.w * q.w + 2 * q.z * q.z - 1) * 180/M_PI;
    //*/
    
    Serial.print(angles[0]); Serial.print("\t");
    Serial.print(angles[1]); Serial.print("\t");
    Serial.println(angles[2]); //*/

    /*eulerPacket[3] = (uint8_t)(round(angles[0]) >> 8);
    eulerPacket[4] = (uint8_t)round(angles[0]);
    eulerPacket[5] = (uint8_t)(round(angles[1]) >> 8);
    eulerPacket[6] = (uint8_t)round(angles[1]);
    eulerPacket[7] = (uint8_t)(round(angles[2]) >> 8);
    eulerPacket[8] = (uint8_t)round(angles[2]);
    Serial.write(eulerPacket, 9); //*/

    // get gravity vector components
    gravity.x = 2 * (q.x*q.z - q.w*q.y);
    gravity.y = 2 * (q.w*q.x + q.y*q.z);
    gravity.z = q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z;

    /*// calculate yaw/pitch/roll with respect to gravity (in degrees, not radians)
    // yaw: (about Z axis)
    ypr[0] = atan2(2 * q.x * q.y - 2 * q.w * q.z, 2 * q.w*q.w + 2 * q.x * q.x - 1) * 180/M_PI;
    // pitch: (nose up/down)
    ypr[1] = atan(gravity.x / sqrt(gravity.y*gravity.y + gravity.z*gravity.z)) * 180/M_PI;
    // roll: (tilt left/right)
    ypr[2] = atan(gravity.y / sqrt(gravity.x*gravity.x + gravity.z*gravity.z)) * 180/M_PI;

    /*Serial.print(ypr[0]); Serial.print("\t");
    Serial.print(ypr[1]); Serial.print("\t");
    Serial.println(ypr[2]); //*/

    // get gravity-free acceleration components
    // (will be on the scale of +/- 250 = +/- 1g within accelerometer frame)
    aaReal.x = (aa.x - gravity.x*250);
    aaReal.y = (aa.y - gravity.y*250);
    aaReal.z = (aa.z - gravity.z*250);
    
    /*Serial.print(aaReal.x); Serial.print("\t");
    Serial.print(aaReal.y); Serial.print("\t");
    Serial.println(aaReal.z); //*/

    /*// calculate tilt angle w.r.t. gravity
    xTilt = atan2(gravity.x, sqrt(gravity.y*gravity.y + gravity.z*gravity.z)); // will be 0 on a flat surface
    yTilt = atan2(gravity.y, sqrt(gravity.x*gravity.x + gravity.z*gravity.z)); // will be 0 on a flat surface
    zTilt = atan2(gravity.z, sqrt(gravity.x*gravity.x + gravity.y*gravity.y)); // will be PI/2 (+90 degrees) right side up on a flat surface
    //*/
    
    // store last-iteration aaFrame vector for trapezoidal integration estimate later
    aaFrame0.x = aaFrame.x;
    aaFrame0.y = aaFrame.y;
    aaFrame0.z = aaFrame.z;

    // rotate measured 3D acceleration vector into original state
    // frame of reference based on orientation quaternion
    aaFrame = aaReal.getRotated(q);

    /*Serial.print(aaFrame.x); Serial.print("\t");
    Serial.print(aaFrame.y); Serial.print("\t");
    Serial.println(aaFrame.z); //*/

    // integrate to estimate true velocity
    avFrame.x += (aaFrame0.x + aaFrame.x) >> 1;
    avFrame.y += (aaFrame0.y + aaFrame.y) >> 1;
    avFrame.z += (aaFrame0.z + aaFrame.z) >> 1;
}

void autozero() {
    int32_t gvxOffset = 0, gvyOffset = 0, gvzOffset = 0;
    int32_t aaxOffset = 0, aayOffset = 0, aazOffset = 0;
    for (uint8_t i = 0; i < 50; i++) {
        gyro.getRotation(&gv.x, &gv.y, &gv.z);
        accel.getAcceleration(&aa.x, &aa.y, &aa.z);
        gvxOffset += gv.x;
        gvyOffset += gv.y;
        gvzOffset += gv.z;
        aaxOffset += aa.x;
        aayOffset += aa.y;
        aazOffset += aa.z;
        delay(10);
    }
    aaOffset.x = aaxOffset / 50;
    aaOffset.y = aayOffset / 50;
    aaOffset.z = (aazOffset / 50) - 250; // accel Z axis should be +1g on a level surface
    gvOffset.x = gvxOffset / 50;
    gvOffset.y = gvyOffset / 50;
    gvOffset.z = gvzOffset / 50;

    /*Serial.print(aaOffset.x); Serial.print("\t");
    Serial.print(aaOffset.y); Serial.print("\t");
    Serial.print(aaOffset.z); Serial.print("\t");
    Serial.print(gvOffset.y); Serial.print("\t");
    Serial.print(gvOffset.x); Serial.print("\t");
    Serial.println(gvOffset.z); //*/
}

uint8_t imuTick = 0;   // increments every ~10ms (100hz), loops at 100
uint32_t imuTock = 0;  // increments every 100 ticks, loops as 2^32 (~4 billion)
uint32_t imuTickTime = 0, imuTickTime0 = 0;

volatile uint8_t timer1Overflow = 0;
ISR(TIMER1_OVF_vect) {
    timer1Overflow++;
}

#define LED_PIN 11 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accel.initialize();
    gyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accel.testConnection() ? "ADXL345 connection successful" : "ADXL345 connection failed");
    Serial.println(gyro.testConnection() ? "ITG3200 connection successful" : "ITG3200 connection failed");
    
    // set accelerometer to 100Hz, enable active-low interrupt for data ready
    accel.setRate(ADXL345_RATE_100);
    /*DDRE &= 0b11101111; // E4 = input
    PORTE |= 0b00010000; // E4 = pullup*/
    accel.setInterruptMode(1);
    accel.setIntDataReadyPin(0);
    accel.setIntDataReadyEnabled(1);

    // set gyroscope to 100Hz, enable active-low interrupt for data ready
    gyro.setFullScaleRange(ITG3200_FULLSCALE_2000);
    gyro.setDLPFBandwidth(ITG3200_DLPF_BW_42); // 42 Hz DLPF, 1kHz internal sampling
    gyro.setRate(9); // 1kHz/(9+1) = 100Hz
    /*DDRE &= 0b11011111; // E5 = input
    PORTE |= 0b00100000; // E5 = pullup*/
    gyro.setInterruptMode(1); // active low
    gyro.setInterruptDrive(1); // open drain
    gyro.setInterruptLatchClear(1); // clear on any read
    gyro.setIntDataReadyEnabled(1); // trigger interrupt on data ready

    // auto-detect zero state (usually takes ~500ms once)
    // NOTE: for best results, start sketch with 6DOF board on a FLAT SURFACE!
    Serial.print("Calibrating...");
    autozero();
    Serial.println("done!");

    // setup internal 100Hz "tick" interrupt
    // thanks to http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1212098919 (and 'bens')
    // also, lots of timer info here and here:
    //    http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=50106
    //    http://www.avrbeginners.net/architecture/timers/timers.html
    TCCR1A = 1; // set TIMER1 overflow at 8-bit max (0xFF)
    TCCR1B = 1; // no prescaler
    TCNT1 = 0; // clear TIMER1 counter
    TIFR1 |= (1 << TOV1); // clear the TIMER1 overflow flag
    TIMSK1 |= (1 << TOIE1); // enable TIMER1 overflow interrupts

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // check for TIMER1 overflow limit and increment tick (should be every 10 ms)
    // 156 results in 9.937 ms, 157 results in 10.001 ms
    if (timer1Overflow >= 157) {
        timer1Overflow = 0;
        imuTick++;
        //imuTickTime += micros() - imuTickTime0;
        //imuTickTime0 = micros();

        // check for 100 ticks and reset counter (should be every 1 second)
        if (imuTick == 100) {
            //Serial.println(imuTickTime / 100);
            //imuTickTime = 0;
            imuTick = 0;
            imuTock++;
        }

        // read raw accel/gyro measurements from devices
        accel.getAcceleration(&aa.x, &aa.y, &aa.z);
        gyro.getRotation(&gv.x, &gv.y, &gv.z);
        
        // adjust for calibrated zero position
        gv.x -= gvOffset.x;
        gv.y -= gvOffset.y;
        gv.z -= gvOffset.z;
        aa.x -= aaOffset.x;
        aa.y -= aaOffset.y;
        aa.z -= aaOffset.z;

        /*Serial.print(aa.x); Serial.print("\t");
        Serial.print(aa.y); Serial.print("\t");
        Serial.print(aa.z); Serial.print("\t");
        Serial.print(gv.x); Serial.print("\t");
        Serial.print(gv.y); Serial.print("\t");
        Serial.println(gv.z); //*/

        gvIMU.x = gv.x * (PI/(180 * 4));
        gvIMU.y = gv.y * (PI/(180 * 4));
        gvIMU.z = gv.z * (PI/(180 * 4));
        aaIMU.x = aa.x;
        aaIMU.y = aa.y;
        aaIMU.z = aa.z;

        /*Serial.print(aaIMU.x); Serial.print("\t");
        Serial.print(aaIMU.y); Serial.print("\t");
        Serial.print(aaIMU.z); Serial.print("\t");
        Serial.print(gvIMU.x); Serial.print("\t");
        Serial.print(gvIMU.y); Serial.print("\t");
        Serial.println(gvIMU.z); //*/

        // update orientation and everything else
        update_imu_filter();

        // check for blink condition (should be every 1/2 second)
        if (imuTick % 50 == 0) {
            // update simple blinking LED
            blinkState = !blinkState;
            digitalWrite(LED_PIN, blinkState);
        }
    }
}