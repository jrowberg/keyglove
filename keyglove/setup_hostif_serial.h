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

// special string comparison for protocol, match must either be end of string OR followed by space
int strncasecmp2(const char *s1, const char *s2, int len) {
    int r = strncasecmp(s1, s2, len);
    if (r == 0 && s2[len] != 0 && s2[len - 1] != ' ' && s2[len] != ' ') r = 1; // terminate or followed by space
    return r;
}

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
            serialError = 1; // invalid command (assumed)

            // echo command back to host
            Serial.println(serialBuffer);

            char *test = (char *)serialBuffer;

#ifdef ORIGINAL_SERIAL
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
#endif


            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Serial protocol text parser code generated from kgprotocol-0.1.txt @ Fri, 25 May 2012 08:37:56 -0600 //
            //////////////////////////////////////////////////////////////////////////////////////////////////////////

            if (strncasecmp2("ACCEL ", test, 6) == 0) {
                // ACCEL parent level
                test += 6;
                if (strncasecmp2("FILTERED", test, 8) == 0) {
                    // 0xA2: ACCEL FILTERED 
                    // Gets last filtered accelerometer X/Y/Z measurements
                    test += 9;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // -----------------------------------------------------
                        // TODO: desired parser actions for 0xA2: ACCEL FILTERED
                        // -----------------------------------------------------
                    }
                } else if (strncasecmp2("MODE", test, 4) == 0) {
                    // 0xA0: ACCEL MODE <mode>
                    // Controls the accelerometer mode.
                    test += 5;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: mode (required)
                    // >>>: Accelerometer mode
                    uint8_t v_mode = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_mode = strtoul(test, &test, 10);
                        if (!(v_mode == 0 || v_mode == 1)) serialError = 4; // invalid parameter (not accepted value)
                    } else serialError = 2; // missing parameter(s)

                    // respond to command if successful
                    if (!serialError) {
                        // v_mode: Accelerometer mode (uint8)
                        if (mode == 1) enable_motion_accelerometer();
                        else disable_motion_accelerometer();
                    }
                } else if (strncasecmp2("RAW", test, 3) == 0) {
                    // 0xA1: ACCEL RAW 
                    // Gets last raw accelerometer X/Y/Z measurements
                    test += 4;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // ------------------------------------------------
                        // TODO: desired parser actions for 0xA1: ACCEL RAW
                        // ------------------------------------------------
                    }
                }
            } else if (strncasecmp2("AUTOZERO", test, 8) == 0) {
                // 0x30: AUTOZERO 
                // Begins motion sensor recalibration to find the zero/still frame of reference.
                test += 9;
                serialError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!serialError) {
                    accelEnableAutoZero = true;
                    gyroEnableAutoZero = true;
                }
            } else if (strncasecmp2("BATTERY ", test, 8) == 0) {
                // BATTERY parent level
                test += 8;
                if (strncasecmp2("STATUS", test, 6) == 0) {
                    // BATTERY STATUS 
                    // Get current status of battery
                    test += 7;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // -----------------------------------------------
                        // TODO: desired parser actions for BATTERY STATUS
                        // -----------------------------------------------
                    }
                }
            } else if (strncasecmp2("BLINK", test, 5) == 0) {
                // 0xF1: BLINK <mode>
                // Controls the single LED for simple feedback.
                test += 6;
                serialError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: mode (required)
                // >>>: Single LED mode
                uint8_t v_mode = 0;
                if (!serialError && test - serialBuffer < serialBufferPos) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                    v_mode = strtoul(test, &test, 10);
                    if (v_mode < 0 || v_mode > 1) serialError = 4; // invalid parameter (out of range)
                } else serialError = 2; // missing parameter(s)

                // respond to command if successful
                if (!serialError) {
                    // ---------------------------------------------------------
                    // TODO: desired parser actions for 0xF1: BLINK <mode>
                    // ---------------------------------------------------------
                    // v_mode: Single LED mode (uint8)
                }
            } else if (strncasecmp2("BT ", test, 3) == 0) {
                // BT parent level
                test += 3;
                if (strncasecmp2("RAW", test, 3) == 0) {
                    // 0xB7: BT RAW <command>
                    // Send a command directly to the Bluetooth module (must be attached, obviously).
                    test += 4;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: command (required)
                    // >>>: Command to pass through to BT module
                    uint8_t *v_command = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        v_command = (uint8_t *)malloc(serialBufferPos - (test - serialBuffer));
                        if (v_command == 0) serialError = 6; // could not allocate memory for parameter
                        else {
                            memcpy(v_command, test, sizeof(v_command) - 1);
                            v_command[sizeof(v_command) - 1] = 0; // null terminate
                        }
                        argCount++;
                    } else serialError = 2; // missing parameter(s)

                    // respond to command if successful
                    if (!serialError) {
                        // v_command: Command to pass through to BT module (string)
                        Serial.print("*");
                        Serial.print(command);
                        Serial.print("*");
                        Uart.write((const uint8_t *)command, serialBufferPos - 7);
                        Uart.print("\r\n");
                    }

                    // free any dynamically allocated memory
                    if (v_command != 0) free(v_command);
                }
            } else if (strncasecmp2("EEPROM ", test, 7) == 0) {
                // EEPROM parent level
                test += 7;
                if (strncasecmp2("EXPORT", test, 6) == 0) {
                    // 0x51: EEPROM EXPORT [offset [length]]
                    // Export current EEPROM data
                    test += 7;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: offset (optional)
                    // >>>: Byte offset of start of imported data (default 0)
                    uint8_t v_offset = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_offset = (test, &test, 10);
                    }

                    // arg: length (optional)
                    // >>>: Length of data to export (default all)
                    uint8_t v_length = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_length = (test, &test, 10);
                    }

                    // respond to command if successful
                    if (!serialError) {
                        // ----------------------------------------------------------------------
                        // TODO: desired parser actions for 0x51: EEPROM EXPORT [offset [length]]
                        // ----------------------------------------------------------------------
                        // v_offset: Byte offset of start of imported data (default 0) (uint32_t)
                        // v_length: Length of data to export (default all) (uint32_t)
                    }
                } else if (strncasecmp2("IMPORT", test, 6) == 0) {
                    // 0x52: EEPROM IMPORT <offset> <length> <data>
                    // Import (store) EEPROM data
                    test += 7;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: offset (required)
                    // >>>: Byte offset of start of imported data
                    uint8_t v_offset = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_offset = (test, &test, 10);
                    } else serialError = 2; // missing parameter(s)

                    // arg: length (required)
                    // >>>: Length of data to import
                    uint8_t v_length = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_length = (test, &test, 10);
                    } else serialError = 2; // missing parameter(s)

                    // arg: data (required)
                    // >>>: EEPROM data to import (length must match <length> parameter
                    uint8_t *v_data = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        v_data = (uint8_t *)malloc(serialBufferPos - (test - serialBuffer));
                        if (v_data == 0) serialError = 6; // could not allocate memory for parameter
                        else {
                            memcpy(v_data, test, sizeof(v_data) - 1);
                            v_data[sizeof(v_data) - 1] = 0; // null terminate
                        }
                        argCount++;
                    } else serialError = 2; // missing parameter(s)

                    // respond to command if successful
                    if (!serialError) {
                        // -----------------------------------------------------------------------------------------------
                        // TODO: desired parser actions for 0x52: EEPROM IMPORT <offset> <length> <data>
                        // -----------------------------------------------------------------------------------------------
                        // v_offset: Byte offset of start of imported data (uint32_t)
                        // v_length: Length of data to import (uint32_t)
                        // v_data: EEPROM data to import (length must match <length> parameter (string)
                    }

                    // free any dynamically allocated memory
                    if (v_data != 0) free(v_data);
                } else if (strncasecmp2("STATUS", test, 6) == 0) {
                    // 0x50: EEPROM STATUS 
                    // Get current EEPROM usage status
                    test += 7;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // ----------------------------------------------------
                        // TODO: desired parser actions for 0x50: EEPROM STATUS
                        // ----------------------------------------------------
                    }
                }
            } else if (strncasecmp2("FEATURES", test, 8) == 0) {
                // 0x11: FEATURES 
                // Get enabled features and modules (such as sensors, wireless, feedback, external EEPROM, etc.)
                test += 9;
                serialError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!serialError) {
                    // -----------------------------------------------
                    // TODO: desired parser actions for 0x11: FEATURES
                    // -----------------------------------------------
                }
            } else if (strncasecmp2("GESTURE", test, 7) == 0) {
                // GESTURE 
                // Gesture detected
                test += 8;
                serialError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!serialError) {
                    // ----------------------------------------
                    // TODO: desired parser actions for GESTURE
                    // ----------------------------------------
                }
            } else if (strncasecmp2("GYRO ", test, 5) == 0) {
                // GYRO parent level
                test += 5;
                if (strncasecmp2("FILTERED", test, 8) == 0) {
                    // 0xA6: GYRO FILTERED 
                    // Gets last filtered gyroscope X/Y/Z measurements
                    test += 9;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // ----------------------------------------------------
                        // TODO: desired parser actions for 0xA6: GYRO FILTERED
                        // ----------------------------------------------------
                    }
                } else if (strncasecmp2("MODE", test, 4) == 0) {
                    // 0xA4: GYRO MODE <mode>
                    // Controls the gyroscope mode.
                    test += 5;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: mode (required)
                    // >>>: Gyroscope mode
                    uint8_t v_mode = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_mode = strtoul(test, &test, 10);
                        if (!(v_mode == 0 || v_mode == 1)) serialError = 4; // invalid parameter (not accepted value)
                    } else serialError = 2; // missing parameter(s)

                    // respond to command if successful
                    if (!serialError) {
                        // v_mode: Gyroscope mode (uint8)
                        if (mode == 1) enable_motion_gyroscope();
                        else disable_motion_gyroscope();
                    }
                } else if (strncasecmp2("RAW", test, 3) == 0) {
                    // 0xA5: GYRO RAW 
                    // Gets last raw gyroscope X/Y/Z measurements
                    test += 4;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // -----------------------------------------------
                        // TODO: desired parser actions for 0xA5: GYRO RAW
                        // -----------------------------------------------
                    }
                }
            } else if (strncasecmp2("IMU ", test, 4) == 0) {
                // IMU parent level
                test += 4;
                if (strncasecmp2("ACCELERATION", test, 12) == 0) {
                    // 0xB2: IMU ACCELERATION 
                    // Gets last calculated X/Y/Z acceleration data from IMU, relative to initial frame of reference and
                    // with gravitational effects removed.
                    test += 13;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // -------------------------------------------------------
                        // TODO: desired parser actions for 0xB2: IMU ACCELERATION
                        // -------------------------------------------------------
                    }
                } else if (strncasecmp2("MODE", test, 4) == 0) {
                    // 0xB0: IMU MODE <mode>
                    // Controls the inertial measurement unit (IMU) mode.
                    test += 5;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: mode (required)
                    // >>>: IMU mode
                    uint8_t v_mode = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_mode = strtoul(test, &test, 10);
                        if (!(v_mode == 0 || v_mode == 1)) serialError = 4; // invalid parameter (not accepted value)
                    } else serialError = 2; // missing parameter(s)

                    // respond to command if successful
                    if (!serialError) {
                        // ------------------------------------------------------------
                        // TODO: desired parser actions for 0xB0: IMU MODE <mode>
                        // ------------------------------------------------------------
                        // v_mode: IMU mode (uint8)
                    }
                } else if (strncasecmp2("ORIENTATION", test, 11) == 0) {
                    // 0xB1: IMU ORIENTATION [format]
                    // Gets last calculated orientation
                    test += 12;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: format (optional)
                    uint8_t v_format = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_format = strtoul(test, &test, 10);
                        if (!(v_format == 0 || v_format == 1 || v_format == 2)) serialError = 4; // invalid parameter (not accepted value)
                    }

                    // respond to command if successful
                    if (!serialError) {
                        // ---------------------------------------------------------------
                        // TODO: desired parser actions for 0xB1: IMU ORIENTATION [format]
                        // ---------------------------------------------------------------
                        // v_format: (uint8)
                    }
                } else if (strncasecmp2("POSITION", test, 8) == 0) {
                    // 0xB4: IMU POSITION 
                    // Gets last calculated X/Y/Z position data from IMU, relative to initial frame of reference and
                    // with gravitational effects removed.
                    test += 9;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // ---------------------------------------------------
                        // TODO: desired parser actions for 0xB4: IMU POSITION
                        // ---------------------------------------------------
                    }
                } else if (strncasecmp2("VELOCITY", test, 8) == 0) {
                    // 0xB3: IMU VELOCITY 
                    // Gets last calculated X/Y/Z velocity data from IMU, relative to initial frame of reference and
                    // with gravitational effects removed.
                    test += 9;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // ---------------------------------------------------
                        // TODO: desired parser actions for 0xB3: IMU VELOCITY
                        // ---------------------------------------------------
                    }
                }
            } else if (strncasecmp2("MAG ", test, 4) == 0) {
                // MAG parent level
                test += 4;
                if (strncasecmp2("FILTERED", test, 8) == 0) {
                    // 0xAA: MAG FILTERED 
                    // Gets last filtered magnetometer X/Y/Z measurements
                    test += 9;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // ---------------------------------------------------
                        // TODO: desired parser actions for 0xAA: MAG FILTERED
                        // ---------------------------------------------------
                    }
                } else if (strncasecmp2("MODE", test, 4) == 0) {
                    // 0xA8: MAG MODE <mode>
                    // Controls the magnetometer mode.
                    test += 5;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: mode (required)
                    // >>>: Magnetometer mode
                    uint8_t v_mode = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_mode = strtoul(test, &test, 10);
                        if (!(v_mode == 0 || v_mode == 1)) serialError = 4; // invalid parameter (not accepted value)
                    } else serialError = 2; // missing parameter(s)

                    // respond to command if successful
                    if (!serialError) {
                        // v_mode: Magnetometer mode (uint8)
                        if (mode == 1) enable_motion_magnetometer();
                        else disable_motion_magnetometer();
                    }
                } else if (strncasecmp2("RAW", test, 3) == 0) {
                    // 0xA9: MAG RAW 
                    // Gets last raw magnetometer X/Y/Z measurements
                    test += 4;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // ----------------------------------------------
                        // TODO: desired parser actions for 0xA9: MAG RAW
                        // ----------------------------------------------
                    }
                }
            } else if (strncasecmp2("OPTION", test, 6) == 0) {
                // 0x20: OPTION [name [value]]
                // List current module option settings, or get or set single module option value
                test += 7;
                serialError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: name (optional)
                // >>>: Name of option to get or set
                uint8_t *v_name = 0;
                if (!serialError && test - serialBuffer < serialBufferPos) {
                    v_name = (uint8_t *)malloc(serialBufferPos - (test - serialBuffer));
                    if (v_name == 0) serialError = 6; // could not allocate memory for parameter
                    else {
                        memcpy(v_name, test, sizeof(v_name) - 1);
                        v_name[sizeof(v_name) - 1] = 0; // null terminate
                    }
                    argCount++;
                }

                // arg: value (optional)
                // >>>: New option value to set
                int32_t v_value = 0;
                if (!serialError && test - serialBuffer < serialBufferPos) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                    v_value = strtol(test, &test, 10);
                }

                // respond to command if successful
                if (!serialError) {
                    // ------------------------------------------------------------
                    // TODO: desired parser actions for 0x20: OPTION [name [value]]
                    // ------------------------------------------------------------
                    // v_name: Name of option to get or set (string)
                    // v_value: New option value to set (int32)
                }

                // free any dynamically allocated memory
                if (v_name != 0) free(v_name);
            } else if (strncasecmp2("PIEZO", test, 5) == 0) {
                // 0xF3: PIEZO <mode> [duration [frequency]]
                // Controls the piezo buzzer for audible feedback.
                test += 6;
                serialError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: mode (required)
                // >>>: Piezo buzzer mode
                uint8_t v_mode = 0;
                if (!serialError && test - serialBuffer < serialBufferPos) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                    v_mode = strtoul(test, &test, 10);
                    if (v_mode < 0 || v_mode > 7) serialError = 4; // invalid parameter (out of range)
                } else serialError = 2; // missing parameter(s)

                // arg: duration (optional)
                // >>>: Duration of tone in 10 ms increments; omit or set to 0 for infinite
                uint8_t v_duration = 0;
                if (!serialError && test - serialBuffer < serialBufferPos) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                    v_duration = strtoul(test, &test, 10);
                    if (v_duration < 0 || v_duration > 65535) serialError = 4; // invalid parameter (out of range)
                }

                // arg: frequency (optional)
                // >>>: Frequency of tone in Hz; omit for default 1760 Hz
                uint16_t v_frequency = 0;
                if (!serialError && test - serialBuffer < serialBufferPos) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                    v_frequency = strtoul(test, &test, 10);
                    if (v_frequency < 60 || v_frequency > 10000) serialError = 4; // invalid parameter (out of range)
                }

                // respond to command if successful
                if (!serialError) {
                    // v_mode: Piezo buzzer mode (uint8)
                    // v_duration: Duration of tone in 10 ms increments; omit or set to 0 for infinite (uint8)
                    // v_frequency: Frequency of tone in Hz; omit for default 1760 Hz (uint16)
                    if (argCount == 1) set_piezo_mode(mode);
                    else if (argCount == 2) set_piezo_mode(mode, duration);
                    else if (argCount == 3) set_piezo_mode(mode, duration, frequency);
                }
            } else if (strncasecmp2("RESET", test, 5) == 0) {
                // 0xFF: RESET 
                // Resets the Keyglove controller board to power-on defaults
                test += 6;
                serialError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!serialError) {
                    setup();
                }
            } else if (strncasecmp2("RGB", test, 3) == 0) {
                // 0xF2: RGB <red> <green> <blue>
                // Controls each channel of the RGB LED for visual feedback.
                test += 4;
                serialError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: red (required)
                // >>>: Red LED mode
                uint8_t v_red = 0;
                if (!serialError && test - serialBuffer < serialBufferPos) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                    v_red = strtoul(test, &test, 10);
                    if (v_red < 0 || v_red > 6) serialError = 4; // invalid parameter (out of range)
                } else serialError = 2; // missing parameter(s)

                // arg: green (required)
                // >>>: Green LED mode
                uint8_t v_green = 0;
                if (!serialError && test - serialBuffer < serialBufferPos) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                    v_green = strtoul(test, &test, 10);
                } else serialError = 2; // missing parameter(s)

                // arg: blue (required)
                // >>>: Blue LED mode
                uint8_t v_blue = 0;
                if (!serialError && test - serialBuffer < serialBufferPos) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                    v_blue = strtoul(test, &test, 10);
                } else serialError = 2; // missing parameter(s)

                // respond to command if successful
                if (!serialError) {
                    // v_red: Red LED mode (uint8)
                    // v_green: Green LED mode
                    // v_blue: Blue LED mode
                    set_rgb_mode(red, green, blue);
                }
            } else if (strncasecmp2("SRAM ", test, 5) == 0) {
                // SRAM parent level
                test += 5;
                if (strncasecmp2("STATUS", test, 6) == 0) {
                    // 0x60: SRAM STATUS 
                    // Get current SRAM usage status
                    test += 7;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // --------------------------------------------------
                        // TODO: desired parser actions for 0x60: SRAM STATUS
                        // --------------------------------------------------
                    }
                }
            } else if (strncasecmp2("TOUCH ", test, 6) == 0) {
                // TOUCH parent level
                test += 6;
                if (strncasecmp2("LOGIC", test, 5) == 0) {
                    // 0x91: TOUCH LOGIC [logic]
                    // Gets current HIGH/LOW logic reading of each sensor. (Supply logic value to emulate touch event)
                    test += 6;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: logic (optional)
                    // >>>: Touch sensor logic status
                    uint8_t *v_logic = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        v_logic = (uint8_t *)malloc(serialBufferPos - (test - serialBuffer));
                        if (v_logic == 0) serialError = 6; // could not allocate memory for parameter
                        else {
                            memcpy(v_logic, test, sizeof(v_logic) - 1);
                            v_logic[sizeof(v_logic) - 1] = 0; // null terminate
                        }
                        argCount++;
                    }

                    // respond to command if successful
                    if (!serialError) {
                        // ----------------------------------------------------------
                        // TODO: desired parser actions for 0x91: TOUCH LOGIC [logic]
                        // ----------------------------------------------------------
                        // v_logic: Touch sensor logic status (string)
                    }

                    // free any dynamically allocated memory
                    if (v_logic != 0) free(v_logic);
                } else if (strncasecmp2("MODE", test, 4) == 0) {
                    // 0x90: TOUCH MODE <mode>
                    // Controls the touch detection mode.
                    test += 5;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: mode (required)
                    // >>>: Accelerometer mode
                    uint8_t v_mode = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_mode = strtoul(test, &test, 10);
                        if (!(v_mode == 0 || v_mode == 1)) serialError = 4; // invalid parameter (not accepted value)
                    } else serialError = 2; // missing parameter(s)

                    // respond to command if successful
                    if (!serialError) {
                        // --------------------------------------------------------------
                        // TODO: desired parser actions for 0x90: TOUCH MODE <mode>
                        // --------------------------------------------------------------
                        // v_mode: Accelerometer mode (uint8)
                    }
                } else if (strncasecmp2("STATUS", test, 6) == 0) {
                    // 0x92: TOUCH STATUS [status]
                    // Gets current status of base touch combinations. (Supply status value to emulate touch event)
                    test += 7;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: status (optional)
                    // >>>: Base touch combination status
                    uint8_t *v_status = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        v_status = (uint8_t *)malloc(serialBufferPos - (test - serialBuffer));
                        if (v_status == 0) serialError = 6; // could not allocate memory for parameter
                        else {
                            memcpy(v_status, test, sizeof(v_status) - 1);
                            v_status[sizeof(v_status) - 1] = 0; // null terminate
                        }
                        argCount++;
                    }

                    // respond to command if successful
                    if (!serialError) {
                        // ------------------------------------------------------------
                        // TODO: desired parser actions for 0x92: TOUCH STATUS [status]
                        // ------------------------------------------------------------
                        // v_status: Base touch combination status (string)
                    }

                    // free any dynamically allocated memory
                    if (v_status != 0) free(v_status);
                }
            } else if (strncasecmp2("TOUCHSET ", test, 9) == 0) {
                // TOUCHSET parent level
                test += 9;
                if (strncasecmp2("DELETE", test, 6) == 0) {
                    // 0xC5: TOUCHSET DELETE [number]
                    // Delete the active or specified touchset definition.
                    test += 7;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: number (optional)
                    // >>>: Touchset number to delete (leave blank for active touchset)
                    uint8_t v_number = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_number = strtoul(test, &test, 10);
                    }

                    // respond to command if successful
                    if (!serialError) {
                        // ---------------------------------------------------------------
                        // TODO: desired parser actions for 0xC5: TOUCHSET DELETE [number]
                        // ---------------------------------------------------------------
                        // v_number: Touchset number to delete (leave blank for active touchset) (uint8)
                    }
                } else if (strncasecmp2("EXPORT", test, 6) == 0) {
                    // 0xC4: TOUCHSET EXPORT [number]
                    // Export the active or specified touchset definition.
                    test += 7;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: number (optional)
                    // >>>: Touchset number to export (leave blank for active touchset)
                    uint8_t v_number = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_number = strtoul(test, &test, 10);
                    }

                    // respond to command if successful
                    if (!serialError) {
                        // ---------------------------------------------------------------
                        // TODO: desired parser actions for 0xC4: TOUCHSET EXPORT [number]
                        // ---------------------------------------------------------------
                        // v_number: Touchset number to export (leave blank for active touchset) (uint8)
                    }
                } else if (strncasecmp2("IMPORT", test, 6) == 0) {
                    // 0xC3: TOUCHSET IMPORT <number> <length> <data>
                    // Import (store) a new touchset definition.
                    test += 7;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: number (required)
                    // >>>: Touchset number to import (using existing number will overwrite)
                    uint8_t v_number = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_number = strtoul(test, &test, 10);
                    } else serialError = 2; // missing parameter(s)

                    // arg: length (required)
                    // >>>: Number of bytes of data to import
                    uint16_t v_length = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_length = strtoul(test, &test, 10);
                    } else serialError = 2; // missing parameter(s)

                    // arg: data (required)
                    // >>>: Touchset definition data (length must match <length> argument)
                    uint8_t *v_data = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        v_data = (uint8_t *)malloc(serialBufferPos - (test - serialBuffer));
                        if (v_data == 0) serialError = 6; // could not allocate memory for parameter
                        else {
                            memcpy(v_data, test, sizeof(v_data) - 1);
                            v_data[sizeof(v_data) - 1] = 0; // null terminate
                        }
                        argCount++;
                    } else serialError = 2; // missing parameter(s)

                    // respond to command if successful
                    if (!serialError) {
                        // -------------------------------------------------------------------------------------------------
                        // TODO: desired parser actions for 0xC3: TOUCHSET IMPORT <number> <length> <data>
                        // -------------------------------------------------------------------------------------------------
                        // v_number: Touchset number to import (using existing number will overwrite) (uint8)
                        // v_length: Number of bytes of data to import (uint16)
                        // v_data: Touchset definition data (length must match <length> argument) (string)
                    }

                    // free any dynamically allocated memory
                    if (v_data != 0) free(v_data);
                } else if (strncasecmp2("INFO", test, 4) == 0) {
                    // 0xC1: TOUCHSET INFO [number]
                    // Show information about active or specified touchset.
                    test += 5;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: number (optional)
                    // >>>: Touchset number to describe (leave blank for active touchset)
                    uint8_t v_number = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_number = strtoul(test, &test, 10);
                    }

                    // respond to command if successful
                    if (!serialError) {
                        // -------------------------------------------------------------
                        // TODO: desired parser actions for 0xC1: TOUCHSET INFO [number]
                        // -------------------------------------------------------------
                        // v_number: Touchset number to describe (leave blank for active touchset) (uint8)
                    }
                } else if (strncasecmp2("LIST", test, 4) == 0) {
                    // 0xC0: TOUCHSET LIST 
                    // List all programmed touchsets.
                    test += 5;
                    serialError = 0; // valid command, no error (assumed)

                    // respond to command if successful
                    if (!serialError) {
                        // ----------------------------------------------------
                        // TODO: desired parser actions for 0xC0: TOUCHSET LIST
                        // ----------------------------------------------------
                    }
                } else if (strncasecmp2("SELECT", test, 6) == 0) {
                    // 0xC2: TOUCHSET SELECT <number>
                    // Activate a new touchset.
                    test += 7;
                    serialError = 0; // valid command, no error (assumed)
                    uint8_t argCount = 0;

                    // arg: number (required)
                    // >>>: Touchset number to select
                    uint8_t v_number = 0;
                    if (!serialError && test - serialBuffer < serialBufferPos) {
                        argCount++;
                        if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                        v_number = strtoul(test, &test, 10);
                    } else serialError = 2; // missing parameter(s)

                    // respond to command if successful
                    if (!serialError) {
                        // ---------------------------------------------------------------------
                        // TODO: desired parser actions for 0xC2: TOUCHSET SELECT <number>
                        // ---------------------------------------------------------------------
                        // v_number: Touchset number to select (uint8)
                    }
                }
            } else if (strncasecmp2("VERSION", test, 7) == 0) {
                // 0x10: VERSION 
                // Get the firmware version installed on the controller board
                test += 8;
                serialError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!serialError) {
                    // ----------------------------------------------
                    // TODO: desired parser actions for 0x10: VERSION
                    // ----------------------------------------------
                }
            } else if (strncasecmp2("VIBE", test, 4) == 0) {
                // 0xF4: VIBE <mode> [duration]
                // Controls the vibrating motor for haptic feedback.
                test += 5;
                serialError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: mode (required)
                // >>>: Vibrating motor mode
                uint8_t v_mode = 0;
                if (!serialError && test - serialBuffer < serialBufferPos) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                    v_mode = strtoul(test, &test, 10);
                    if (v_mode < 0 || v_mode > 6) serialError = 4; // invalid parameter (out of range)
                } else serialError = 2; // missing parameter(s)

                // arg: duration (optional)
                // >>>: Duration of vibration in 10 ms increments; omit or set to 0 for infinite
                uint16_t v_duration = 0;
                if (!serialError && test - serialBuffer < serialBufferPos) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') serialError = 5; // invalid parameter (bad format)
                    v_duration = strtoul(test, &test, 10);
                    if (v_duration < 0 || v_duration > 65535) serialError = 4; // invalid parameter (out of range)
                }

                // respond to command if successful
                if (!serialError) {
                    // v_mode: Vibrating motor mode (uint8)
                    // v_duration: Duration of vibration in 10 ms increments; omit or set to 0 for infinite (uint16)
                    if (argCount == 1) set_vibrate_mode(mode);
                    else if (argCount == 2) set_vibrate_mode(mode, duration);
                }
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


