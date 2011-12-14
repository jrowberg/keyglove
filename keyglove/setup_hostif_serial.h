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



#ifndef _SETUP_HOSTIF_SERIAL_H_
#define _SETUP_HOSTIF_SERIAL_H_

#ifndef ENABLE_SERIAL
    #define ENABLE_SERIAL
#endif

char *serialBuffer;
uint16_t serialBufferSize;
uint16_t serialBufferPos;
uint8_t serialByte;
uint16_t serialError;

bool inPacket = false;
bool inData = false;
uint8_t dataLength;

void setup_hostif_serial() {
    serialBuffer = (char *)malloc(32);
    serialBufferSize = 32;
    serialBufferPos = 0;
}

void update_hostif_serial() {
    if (Serial.available()) {
        if (serialBufferPos + 1 == serialBufferSize) {
            serialBufferSize += 32;
            serialBuffer = (char *)realloc(serialBuffer, serialBufferSize);
        }
        serialByte = Serial.read();
        if (!inPacket && (serialByte == '\r' || serialByte == '\n')) {
            // end of a simple text command, so terminate the string
            serialBuffer[serialBufferPos] = 0;
            serialError = 0;

            // echo command back to host
            Serial.println(serialBuffer);

            char *test = (char *)serialBuffer;
            if (strncmp(test, "reset", 5) == 0) {
                // RESET, fully reset Keyglove device
                setup();
            } else if (strncmp(test, "autozero", 9) == 0) {
                // AUTOZERO, auto-zero all sensors
                accelEnableAutoZero = true;
                gyroEnableAutoZero = true;
            } else if (strncmp(test, "set ", 4) == 0 && serialBufferPos > 4) {
                // SET
                test += 4;
                if (strncmp(test, "accel ", 6) == 0 && serialBufferPos > 10) {
                    // SET ACCEL
                    test += 6;
                    if (strncmp(test, "on", 2) == 0 || strncmp(test, "en", 2) == 0 || test[0] == '1') {
                        // SET ACCEL <ON | ENABLED | 1>
                        // enable accelerometer
                        enable_motion_accelerometer();
                    } else if (strncmp(test, "of", 2) == 0 || strncmp(test, "dis", 3) == 0 || test[0] == '0') {
                        // SET ACCEL <OFF | DISABLED | 0>
                        // disable accelerometer
                        disable_motion_accelerometer();
                    } else {
                        serialError = 1;
                    }
                } else if (strncmp(test, "gyro ", 5) == 0 && serialBufferPos > 9) {
                    // SET GYRO
                    test += 5;
                    if (strncmp(test, "on", 2) == 0 || strncmp(test, "en", 2) == 0 || test[0] == '1') {
                        // SET GYRO <ON | ENABLED | 1>
                        // enable gyroscope
                        enable_motion_gyroscope();
                    } else if (strncmp(test, "of", 2) == 0 || strncmp(test, "dis", 3) == 0 || test[0] == '0') {
                        // SET GYRO <OFF | DISABLED | 0>
                        // disable gyroscope
                        disable_motion_gyroscope();
                    } else {
                        serialError = 1;
                    }  
                } else if (strncmp(test, "rgb ", 4) == 0 && serialBufferPos > 8) {
                    // SET RGB
                    test += 4;
                    if (strncmp(test, "red ", 4) == 0 && serialBufferPos > 12) {
                        // SET RGB RED <value>
                        uint8_t r = test[4] - 0x30;
                        if (r <= 5) set_rgb_mode(r, 255, 255);
                    } else if (strncmp(test, "green ", 6) == 0 && serialBufferPos > 12) {
                        // SET RGB GREEN <value>
                        uint8_t g = test[6] - 0x30;
                        if (g <= 5) set_rgb_mode(255, g, 255);
                    } else if (strncmp(test, "blue ", 5) == 0 && serialBufferPos > 12) {
                        // SET RGB BLUE <value>
                        uint8_t b = test[5] - 0x30;
                        if (b <= 5) set_rgb_mode(255, 255, b);
                    } else {
                        // SET RGB <r value> <g value> <b value>
                        uint8_t r = test[0] - 0x30;
                        uint8_t g = test[2] - 0x30;
                        uint8_t b = test[4] - 0x30;
                        if (r > 5) r = 255;
                        if (g > 5) g = 255;
                        if (b > 5) b = 255;
                        set_rgb_mode(r, g, b);
                    }
                } else if (strncmp(test, "piezo ", 6) == 0 && serialBufferPos > 10) {
                    // SET PIEZO <mode> [duration [frequency]]
                    test += 6;
                    uint8_t mode = strtol(test, &test, 10);
                    if (test - serialBuffer < serialBufferPos) {
                        uint16_t duration = strtol(test, &test, 10);
                        if (test - serialBuffer < serialBufferPos) {
                            uint16_t frequency = strtol(test, &test, 10);
                            set_piezo_mode(mode, duration, frequency);
                        } else {
                            set_piezo_mode(mode, duration);
                        }
                    } else {
                        set_piezo_mode(mode);
                    }
                } else if (strncmp(test, "vibe ", 5) == 0 && serialBufferPos > 9) {
                    // SET VIBE <mode> [duration]
                    test += 5;
                    uint8_t mode = strtol(test, &test, 10);
                    if (test - serialBuffer < serialBufferPos) {
                        uint16_t duration = strtol(test, &test, 10);
                        set_vibrate_mode(mode, duration);
                    } else {
                        set_vibrate_mode(mode);
                    }
                } else {
                    serialError = 1;
                }
            #ifdef ENABLE_BLUETOOTH
            } else if (strncmp(test, "bt ", 3) == 0 && serialBufferPos > 3) {
                // Bluetooth passthrough
                test += 3;
                Uart.write((const uint8_t *)test, serialBufferPos - 3);
                Uart.print("\r\n");
            #endif /* ENABLE_BLUETOOTH */
            } else {
                // unknown command
                serialError = 1;
            }

            if (!serialError) {
                Serial.println("OK");
            } else {
                Serial.print("ERR ");
                Serial.println(serialError);
            }

            // reset buffer after processing
            serialBufferPos = 0;
        } else {
            serialBuffer[serialBufferPos++] = serialByte;
        }
    }
}

#endif // _SETUP_HOSTIF_SERIAL_H_


