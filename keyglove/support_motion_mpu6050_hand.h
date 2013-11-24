// Keyglove controller source code - Special hardware setup file
// 11/3/2013 by Jeff Rowberg <jeff@rowberg.net>

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



#ifndef _SUPPORT_MOTION_MPU6050_HAND_H_
#define _SUPPORT_MOTION_MPU6050_HAND_H_

MPU6050 mpuHand = MPU6050(0x68);
bool mpuHandInterrupt;

VectorInt16 aaRaw;                    // raw linear acceleration
VectorInt16 aa;                       // filtered linear acceleration
VectorInt16 aa0;                      // last-iteration filtered linear acceleration
VectorInt16 gvRaw;                    // raw rotational velocity
VectorInt16 gv;                       // filtered rotational velocity
VectorInt16 gv0;                      // last-iteration filtered rotational velocity

void mpu6050_hand_interrupt() {
    mpuHandInterrupt = true;
}

void set_motion_mpu6050_hand_mode(uint8_t mode) {
    if (mode == 1) {
        aa.x = aa.y = aa.z = 0;
        gv.x = gv.y = gv.z = 0;
        mpuHandInterrupt = true;
        attachInterrupt(4, mpu6050_hand_interrupt, FALLING);
        mpuHand.setSleepEnabled(false);
    } else {
        mpuHand.setSleepEnabled(true);
        detachInterrupt(4);
    }

    // send kg_evt_motion_mode packet (if we aren't just intentionally setting it already)
    if (!inBinPacket) {
        uint8_t payload[2] = { 0x00, mode };
        send_keyglove_packet(KG_PACKET_TYPE_EVENT, sizeof(payload), KG_PACKET_CLASS_MOTION, KG_PACKET_ID_EVT_MOTION_MODE, payload);
    }
}

void setup_motion_mpu6050_hand() {
    // set INT4 pin (Arduino Pin 36) to INPUT/HIGH so MPU can drive interrupt pin as active-low
    pinMode(36, INPUT);
    digitalWrite(36, HIGH);

    // setup MPU-6050
    mpuHandInterrupt = false;
    mpuHand.initialize();
    delay(30);
    mpuHand.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    mpuHand.setDLPFMode(MPU6050_DLPF_BW_42); // 42 Hz DLPF, 1kHz internal sampling
    mpuHand.setRate(9); // 1kHz/(9+1) = 100Hz
    mpuHand.setInterruptMode(1); // active low
    mpuHand.setInterruptDrive(1); // open drain
    mpuHand.setInterruptLatch(0); // latch until read
    mpuHand.setInterruptLatchClear(1); // clear on any read
    mpuHand.setIntDataReadyEnabled(1); // trigger interrupt on data ready
    set_motion_mpu6050_hand_mode(1); // enable motion detection
}

void update_motion_mpu6050_hand() {
    // read raw motion data
    mpuHand.getMotion6(&aaRaw.x, &aaRaw.y, &aaRaw.z, &gvRaw.x, &gvRaw.y, &gvRaw.z);

    // store previous accel/gyro values
    aa0.x = aa.x;
    aa0.y = aa.y;
    aa0.z = aa.z;
    gv0.x = gv.x;
    gv0.y = gv.y;
    gv0.z = gv.z;

    // simple smoothing filter
    aa.x = aa0.x + (0.25 * (aaRaw.x - aa0.x));
    aa.y = aa0.y + (0.25 * (aaRaw.y - aa0.y));
    aa.z = aa0.z + (0.25 * (aaRaw.z - aa0.z));
    gv.x = gv0.x + (0.25 * (gvRaw.x - gv0.x));
    gv.y = gv0.y + (0.25 * (gvRaw.y - gv0.y));
    gv.z = gv0.z + (0.25 * (gvRaw.z - gv0.z));

    // build and send kg_evt_motion_data packet
    uint8_t payload[15];
    payload[0] = 0x00;  // sensor 0
    payload[1] = 0x03;  // 1=accel, 2=gyro, 1|2 = 0x03
    payload[2] = 0x0C;  // 12 bytes of motion data (6 axes, 2 bytes each)
    payload[3] = aa.x & 0xFF;
    payload[4] = aa.x >> 8;
    payload[5] = aa.y & 0xFF;
    payload[6] = aa.y >> 8;
    payload[7] = aa.z & 0xFF;
    payload[8] = aa.z >> 8;
    payload[9] = gv.x & 0xFF;
    payload[10] = gv.x >> 8;
    payload[11] = gv.y & 0xFF;
    payload[12] = gv.y >> 8;
    payload[13] = gv.z & 0xFF;
    payload[14] = gv.z >> 8;
    send_keyglove_packet(KG_PACKET_TYPE_EVENT, sizeof(payload), KG_PACKET_CLASS_MOTION, KG_PACKET_ID_EVT_MOTION_DATA, payload);
}

#endif // _SUPPORT_MOTION_MPU6050_HAND_H_