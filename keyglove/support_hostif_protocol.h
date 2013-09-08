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



#ifndef _SUPPORT_HOSTIF_PROTOCOL_H_
#define _SUPPORT_HOSTIF_PROTOCOL_H_

uint16_t create_packet(char *buffer, uint8_t cmdClass, uint8_t cmdID, uint8_t *paramStr, uint8_t paramNum) {
    uint16_t bufLen = 0;
    txPacket = NULL;
    switch (cmdClass) {
        case KG_PACKET_CLASS_PROTOCOL:
            switch (cmdID) {
            }
            break;

        case KG_PACKET_CLASS_SYSTEM:
            switch (cmdID) {
                case KG_PACKET_SYSTEM_TICK:
                    if ((txPacket = (char *)malloc(4 + 4)) != NULL) {
                        bufLen = 4 + 4;
                        txPacket[4] = (uint8_t)(keygloveTock >> 24);
                        txPacket[5] = (uint8_t)(keygloveTock >> 16);
                        txPacket[6] = (uint8_t)(keygloveTock >> 8);
                        txPacket[7] = (uint8_t)keygloveTock;
                    }
                    break;

                case KG_PACKET_SYSTEM_VERSION:
                    /*
                    Serial.println(KG_FIRMWARE_NAME);
                    Serial.println(KG_FIRMWARE_AUTHOR);
                    Serial.println(KG_FIRMWARE_VERSION);
                    */
                    break;

                case KG_PACKET_SYSTEM_FEATURES:
                    /*
                    Serial.print(F("BOARD ")); Serial.println(KG_BOARD);
                    Serial.print(F("DUALGLOVE ")); Serial.println(KG_DUALGLOVE);
                    Serial.print(F("EEPROM ")); Serial.println(KG_EEPROM);
                    Serial.print(F("FEEDBACK ")); Serial.println(KG_FEEDBACK);
                    Serial.print(F("FEEDBACKCONN ")); Serial.println(KG_FEEDBACKCONN);
                    Serial.print(F("FLEX ")); Serial.println(KG_FLEX);
                    Serial.print(F("FUSION ")); Serial.println(KG_FUSION);
                    Serial.print(F("HOSTIF ")); Serial.println(KG_HOSTIF);
                    Serial.print(F("MOTION ")); Serial.println(KG_MOTION);
                    Serial.print(F("PRESSURE ")); Serial.println(KG_PRESSURE);
                    Serial.print(F("SRAM ")); Serial.println(KG_SRAM);
                    Serial.print(F("TOUCHCONN ")); Serial.println(KG_TOUCHCONN);
                    Serial.print(F("DEBUG ")); Serial.println(KG_DEBUG);
                    */
                    break;
            }
            break;

        case KG_PACKET_CLASS_TOUCH:
            switch (cmdID) {
                #if (KG_TOUCHCONN > 0)
                    case KG_PACKET_TOUCH_STATUS:
                        if ((txPacket = (char *)malloc(4 + KG_BASE_COMBINATION_BYTES + 1)) != NULL) {
                            bufLen = 4 + KG_BASE_COMBINATION_BYTES + 1;
                            txPacket[4] = 0; // TODO: support multiple sensor arrays
                            memcpy(txPacket + 5, touches_active, KG_BASE_COMBINATION_BYTES);
                        }
                        break;
                #endif
            }
            break;

        case KG_PACKET_CLASS_MOTION:
            switch (cmdID) {
                #if (defined(ENABLE_ACCELEROMETER) || defined(ENABLE_ACCELGYRO) || defined(ENABLE_ACCELGYROMAG))
                    case KG_PACKET_MOTION_ACCEL_RAW:
                        if ((txPacket = (char *)malloc(4 + 7)) != NULL) {
                            bufLen = 4 + 7;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = aaRaw.x >> 8;
                            txPacket[6] = aaRaw.x;
                            txPacket[7] = aaRaw.y >> 8;
                            txPacket[8] = aaRaw.y;
                            txPacket[9] = aaRaw.z >> 8;
                            txPacket[10] = aaRaw.z;
                        }
                        break;
                    case KG_PACKET_MOTION_ACCEL_FILTERED:
                        if ((txPacket = (char *)malloc(4 + 7)) != NULL) {
                            bufLen = 4 + 7;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = aa.x >> 8;
                            txPacket[6] = aa.x;
                            txPacket[7] = aa.y >> 8;
                            txPacket[8] = aa.y;
                            txPacket[9] = aa.z >> 8;
                            txPacket[10] = aa.z;
                        }
                        break;
                #endif

                #if (defined(ENABLE_ACCELGYRO) || defined(ENABLE_ACCELGYROMAG))
                    case KG_PACKET_MOTION_ACCELGYRO_RAW:
                        if ((txPacket = (char *)malloc(4 + 13)) != NULL) {
                            bufLen = 4 + 13;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = aaRaw.x >> 8;
                            txPacket[6] = aaRaw.x;
                            txPacket[7] = aaRaw.y >> 8;
                            txPacket[8] = aaRaw.y;
                            txPacket[9] = aaRaw.z >> 8;
                            txPacket[10] = aaRaw.z;
                            txPacket[11] = gvRaw.x >> 8;
                            txPacket[12] = gvRaw.x;
                            txPacket[13] = gvRaw.y >> 8;
                            txPacket[14] = gvRaw.y;
                            txPacket[15] = gvRaw.z >> 8;
                            txPacket[16] = gvRaw.z;
                        }
                        break;
                    case KG_PACKET_MOTION_ACCELGYRO_FILTERED:
                        if ((txPacket = (char *)malloc(4 + 13)) != NULL) {
                            bufLen = 4 + 13;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = aa.x >> 8;
                            txPacket[6] = aa.x;
                            txPacket[7] = aa.y >> 8;
                            txPacket[8] = aa.y;
                            txPacket[9] = aa.z >> 8;
                            txPacket[10] = aa.z;
                            txPacket[11] = gv.x >> 8;
                            txPacket[12] = gv.x;
                            txPacket[13] = gv.y >> 8;
                            txPacket[14] = gv.y;
                            txPacket[15] = gv.z >> 8;
                            txPacket[16] = gv.z;
                        }
                        break;
                #endif
        
                #if (defined(ENABLE_ACCELMAG) || defined(ENABLE_ACCELGYROMAG))
                    case KG_PACKET_MOTION_ACCELMAG_RAW:
                        if ((txPacket = (char *)malloc(4 + 13)) != NULL) {
                            bufLen = 4 + 13;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = aaRaw.x >> 8;
                            txPacket[6] = aaRaw.x;
                            txPacket[7] = aaRaw.y >> 8;
                            txPacket[8] = aaRaw.y;
                            txPacket[9] = aaRaw.z >> 8;
                            txPacket[10] = aaRaw.z;
                            txPacket[11] = mhRaw.x >> 8;
                            txPacket[12] = mhRaw.x;
                            txPacket[13] = mhRaw.y >> 8;
                            txPacket[14] = mhRaw.y;
                            txPacket[15] = mhRaw.z >> 8;
                            txPacket[16] = mhRaw.z;
                        }
                        break;
                    case KG_PACKET_MOTION_ACCELMAG_FILTERED:
                        if ((txPacket = (char *)malloc(4 + 13)) != NULL) {
                            bufLen = 4 + 13;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = aa.x >> 8;
                            txPacket[6] = aa.x;
                            txPacket[7] = aa.y >> 8;
                            txPacket[8] = aa.y;
                            txPacket[9] = aa.z >> 8;
                            txPacket[10] = aa.z;
                            txPacket[11] = mh.x >> 8;
                            txPacket[12] = mh.x;
                            txPacket[13] = mh.y >> 8;
                            txPacket[14] = mh.y;
                            txPacket[15] = mh.z >> 8;
                            txPacket[16] = mh.z;
                        }
                        break;
                #endif
        
                #if (defined(ENABLE_ACCELGYROMAG))
                    case KG_PACKET_MOTION_ACCELGYROMAG_RAW:
                        if ((txPacket = (char *)malloc(4 + 19)) != NULL) {
                            bufLen = 4 + 19;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = aaRaw.x >> 8;
                            txPacket[6] = aaRaw.x;
                            txPacket[7] = aaRaw.y >> 8;
                            txPacket[8] = aaRaw.y;
                            txPacket[9] = aaRaw.z >> 8;
                            txPacket[10] = aaRaw.z;
                            txPacket[11] = gvRaw.x >> 8;
                            txPacket[12] = gvRaw.x;
                            txPacket[13] = gvRaw.y >> 8;
                            txPacket[14] = gvRaw.y;
                            txPacket[15] = gvRaw.z >> 8;
                            txPacket[16] = gvRaw.z;
                            txPacket[17] = mhRaw.x >> 8;
                            txPacket[18] = mhRaw.x;
                            txPacket[19] = mhRaw.y >> 8;
                            txPacket[20] = mhRaw.y;
                            txPacket[21] = mhRaw.z >> 8;
                            txPacket[22] = mhRaw.z;
                        }
                        break;
                    case KG_PACKET_MOTION_ACCELGYROMAG_FILTERED:
                        if ((txPacket = (char *)malloc(4 + 19)) != NULL) {
                            bufLen = 4 + 19;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = aa.x >> 8;
                            txPacket[6] = aa.x;
                            txPacket[7] = aa.y >> 8;
                            txPacket[8] = aa.y;
                            txPacket[9] = aa.z >> 8;
                            txPacket[10] = aa.z;
                            txPacket[11] = gv.x >> 8;
                            txPacket[12] = gv.x;
                            txPacket[13] = gv.y >> 8;
                            txPacket[14] = gv.y;
                            txPacket[15] = gv.z >> 8;
                            txPacket[16] = gv.z;
                            txPacket[17] = mh.x >> 8;
                            txPacket[18] = mh.x;
                            txPacket[19] = mh.y >> 8;
                            txPacket[20] = mh.y;
                            txPacket[21] = mh.z >> 8;
                            txPacket[22] = mh.z;
                        }
                        break;
                #endif

                #if (defined(ENABLE_GYROSCOPE) || defined(ENABLE_ACCELGYRO) || defined(ENABLE_ACCELGYROMAG))
                    case KG_PACKET_MOTION_GYRO_RAW:
                        if ((txPacket = (char *)malloc(4 + 7)) != NULL) {
                            bufLen = 4 + 7;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = gvRaw.x >> 8;
                            txPacket[6] = gvRaw.x;
                            txPacket[7] = gvRaw.y >> 8;
                            txPacket[8] = gvRaw.y;
                            txPacket[9] = gvRaw.z >> 8;
                            txPacket[10] = gvRaw.z;
                        }
                        break;
                    case KG_PACKET_MOTION_GYRO_FILTERED:
                        if ((txPacket = (char *)malloc(4 + 7)) != NULL) {
                            bufLen = 4 + 7;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = gv.x >> 8;
                            txPacket[6] = gv.x;
                            txPacket[7] = gv.y >> 8;
                            txPacket[8] = gv.y;
                            txPacket[9] = gv.z >> 8;
                            txPacket[10] = gv.z;
                        }
                        break;
                #endif
            
                #if (defined(ENABLE_GYROMAG) || defined(ENABLE_ACCELGYROMAG))
                    case KG_PACKET_MOTION_GYROMAG_RAW:
                        if ((txPacket = (char *)malloc(4 + 13)) != NULL) {
                            bufLen = 4 + 13;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = gvRaw.x >> 8;
                            txPacket[6] = gvRaw.x;
                            txPacket[7] = gvRaw.y >> 8;
                            txPacket[8] = gvRaw.y;
                            txPacket[9] = gvRaw.z >> 8;
                            txPacket[10] = gvRaw.z;
                            txPacket[11] = mhRaw.x >> 8;
                            txPacket[12] = mhRaw.x;
                            txPacket[13] = mhRaw.y >> 8;
                            txPacket[14] = mhRaw.y;
                            txPacket[15] = mhRaw.z >> 8;
                            txPacket[16] = mhRaw.z;
                        }
                        break;
                    case KG_PACKET_MOTION_GYROMAG_FILTERED:
                        if ((txPacket = (char *)malloc(4 + 13)) != NULL) {
                            bufLen = 4 + 13;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = gv.x >> 8;
                            txPacket[6] = gv.x;
                            txPacket[7] = gv.y >> 8;
                            txPacket[8] = gv.y;
                            txPacket[9] = gv.z >> 8;
                            txPacket[10] = gv.z;
                            txPacket[11] = mh.x >> 8;
                            txPacket[12] = mh.x;
                            txPacket[13] = mh.y >> 8;
                            txPacket[14] = mh.y;
                            txPacket[15] = mh.z >> 8;
                            txPacket[16] = mh.z;
                        }
                        break;
                #endif
        
                #if (defined(ENABLE_MAGNETOMETER) || defined(ENABLE_ACCELGYROMAG))
                    case KG_PACKET_MOTION_MAG_RAW:
                        if ((txPacket = (char *)malloc(4 + 7)) != NULL) {
                            bufLen = 4 + 7;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = mhRaw.x >> 8;
                            txPacket[6] = mhRaw.x;
                            txPacket[7] = mhRaw.y >> 8;
                            txPacket[8] = mhRaw.y;
                            txPacket[9] = mhRaw.z >> 8;
                            txPacket[10] = mhRaw.z;
                        }
                        break;
                    case KG_PACKET_MOTION_MAG_FILTERED:
                        if ((txPacket = (char *)malloc(4 + 7)) != NULL) {
                            bufLen = 4 + 7;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = mh.x >> 8;
                            txPacket[6] = mh.x;
                            txPacket[7] = mh.y >> 8;
                            txPacket[8] = mh.y;
                            txPacket[9] = mh.z >> 8;
                            txPacket[10] = mh.z;
                        }
                        break;
                #endif
            }
            break;

        case KG_PACKET_CLASS_FEEDBACK:
            switch (cmdID) {
                #if (KG_FEEDBACK & KG_FEEDBACK_BLINK)
                    case KG_PACKET_FEEDBACK_BLINK:
                        if ((txPacket = (char *)malloc(4 + 2)) != NULL) {
                            bufLen = 4 + 2;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = blinkMode;
                        }
                        break;
                #endif

                #if (KG_FEEDBACK & KG_FEEDBACK_RGB)
                    case KG_PACKET_FEEDBACK_RGB:
                        if ((txPacket = (char *)malloc(4 + 4)) != NULL) {
                            bufLen = 4 + 4;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = rgbBlinkRed;
                            txPacket[6] = rgbBlinkGreen;
                            txPacket[7] = rgbBlinkBlue;
                        }
                        break;
                #endif

                #if (KG_FEEDBACK & KG_FEEDBACK_PIEZO)
                    case KG_PACKET_FEEDBACK_PIEZO:
                        if ((txPacket = (char *)malloc(4 + 6)) != NULL) {
                            bufLen = 4 + 6;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = piezoMode;
                            txPacket[6] = (uint8_t)(piezoTickLimit >> 8);
                            txPacket[7] = (uint8_t)piezoTickLimit;
                            txPacket[8] = (uint8_t)(piezoFrequency >> 8);
                            txPacket[9] = (uint8_t)piezoFrequency;
                        }
                        break;
                #endif

                #if (KG_FEEDBACK & KG_FEEDBACK_VIBE)
                    case KG_PACKET_FEEDBACK_VIBE:
                        if ((txPacket = (char *)malloc(4 + 4)) != NULL) {
                            bufLen = 4 + 4;
                            txPacket[4] = 0; // TODO: support multiple sensors
                            txPacket[5] = vibrateMode;
                            txPacket[6] = (uint8_t)(vibrateTickLimit >> 8);
                            txPacket[7] = (uint8_t)vibrateTickLimit;
                        }
                        break;
                #endif
            }
            break;

        case KG_PACKET_CLASS_TOUCHSET:
            switch (cmdID) {
            }
            break;
    }

    // finish up packet creation
    if (txPacket != NULL) {
        txPacket[0] = 0xB8;       // "bait" byte
        txPacket[1] = bufLen - 2; // binary packet data size
        txPacket[2] = cmdClass;   // command class byte
        txPacket[3] = cmdID;      // command ID byte
    }

    return bufLen;
}

// special string comparison for protocol, match must either be end of string OR followed by space
int strncasecmp2(const char *s1, const char *s2, int len) {
    int r = strncasecmp(s1, s2, len);
    if (r == 0 && s2[len] != 0 && s2[len - 1] != ' ' && s2[len] != ' ') r = 1; // terminate or followed by space
    return r;
}

void setup_hostif_protocol() {
    rxPacketSize = 32;
    rxPacket = (char *)malloc(rxPacketSize);
    rxPacketLength = 0;
}

void hostif_protocol_parse(uint8_t inputByte) {
    if (rxPacketLength + 1 == rxPacketSize) {
        rxPacketSize += 32;
        rxPacket = (char *)realloc(rxPacket, rxPacketSize);
    }
    if (!inBinPacket && inputByte == 0xB8) { // "bait" byte
        inBinPacket = true;
        rxPacketLength = 0; // reset buffer
        binDataLength = 0;
    } else if (inBinPacket && rxPacketLength == 0 && binDataLength == 0) { // data length byte
        binDataLength = inputByte;
        if (binDataLength == 0) inBinPacket = false; // error
    } else {
        rxPacket[rxPacketLength++] = inputByte;
        if (inBinPacket && rxPacketLength == binDataLength) {
            char *test = (char *)rxPacket;
            response = false;
            //txPacket = NULL;

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Serial protocol binary parser code generated from kgprotocol-0.1.txt @ Wed, 30 May 2012 23:20:40 -0600 //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////

            if ((uint8_t)(*test) == KG_PACKET_SYSTEM_VERSION) {
                // 0x10: VERSION 
                // Get the firmware version installed on the controller board
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_SYSTEM_VERSION);
                }
            } else if ((uint8_t)(*test) == KG_PACKET_SYSTEM_FEATURES) {
                // 0x11: FEATURES 
                // Get enabled features and modules (such as sensors, wireless, feedback, external EEPROM, etc.)
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_SYSTEM_FEATURES);
                }
            } else if ((uint8_t)(*test) == KG_PACKET_SYSTEM_OPTION) {
                // 0x20: OPTION [name [value]]
                // List current module option settings, or get or set single module option value
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: name (optional)
                // >>>: Name of option to get or set
                uint8_t* v_name = (uint8_t *)malloc(*test++ + 1);
                uint8_t v_name_length = 0;
                if (v_name == 0) protoError = 6; // could not allocate memory for parameter
                if (!protoError && test - rxPacket > rxPacketLength - 1) {
                    argCount++;
                    memcpy(v_name, test, sizeof(v_name) - 1);
                    v_name[sizeof(v_name) - 1] = 0; // null terminate
                }
    
                // arg: value (optional)
                // >>>: New option value to set
                int32_t v_value = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 5) {
                    argCount++;
                    v_value = (*test++ << 24) + (*test++ << 16) + (*test++ << 8) + (*test++);
                }
    
                // respond to command if successful
                if (!protoError) {
                    // v_name: Name of option to get or set (string)
                    // v_value: New option value to set (int32)
                    if (argCount == 0) {
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_SYSTEM_OPTION);
                    } else if (argCount == 1) {
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_SYSTEM_OPTION, v_name);
                    } else if (argCount == 2) {
                        // TODO: SET OPTION
                    }
                }
            } else if ((uint8_t)(*test) == KG_PACKET_TOUCHSET_LIST) {
                // 0x40: TOUCHSET LIST 
                // List all programmed touchsets.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_TOUCHSET_LIST);
                }
            } else if ((uint8_t)(*test) == KG_PACKET_TOUCHSET_INFO) {
                // 0x41: TOUCHSET INFO [number]
                // Show information about active or specified touchset.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: number (optional)
                // >>>: Touchset number to describe (leave blank for active touchset)
                uint8_t v_number = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_number = *test++;
                }
    
                // respond to command if successful
                if (!protoError) {
                    // v_number: Touchset number to describe (leave blank for active touchset) (uint8)
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_TOUCHSET_INFO);
                }
            } else if ((uint8_t)(*test) == KG_PACKET_TOUCHSET_SELECT) {
                // 0x42: TOUCHSET SELECT <number>
                // Activate a new touchset.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: number (required)
                // >>>: Touchset number to select
                uint8_t v_number = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_number = *test++;
                } else protoError = 2; // missing parameter(s)
    
                // respond to command if successful
                if (!protoError) {
                    // ---------------------------------------------------------------------
                    // TODO: desired parser actions for 0x42: TOUCHSET SELECT <number>
                    // ---------------------------------------------------------------------
                    // v_number: Touchset number to select (uint8)
                }
            } else if ((uint8_t)(*test) == KG_PACKET_TOUCHSET_IMPORT) {
                // 0x43: TOUCHSET IMPORT <number> <length> <data>
                // Import (store) a new touchset definition.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: number (required)
                // >>>: Touchset number to import (using existing number will overwrite)
                uint8_t v_number = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_number = *test++;
                } else protoError = 2; // missing parameter(s)
    
                // arg: length (required)
                // >>>: Number of bytes of data to import
                uint16_t v_length = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 3) {
                    argCount++;
                    v_length = (*test++ << 8) + (*test++);
                } else protoError = 2; // missing parameter(s)
    
                // arg: data (required)
                // >>>: Touchset definition data (length must match <length> argument)
                uint8_t* v_data = (uint8_t *)malloc(*test++ + 1);
                uint8_t v_data_length = 0;
                if (v_data == 0) protoError = 6; // could not allocate memory for parameter
                if (!protoError && test - rxPacket > rxPacketLength - 1) {
                    argCount++;
                    memcpy(v_data, test, sizeof(v_data) - 1);
                    v_data[sizeof(v_data) - 1] = 0; // null terminate
                } else protoError = 2; // missing parameter(s)
    
                // respond to command if successful
                if (!protoError) {
                    // -------------------------------------------------------------------------------------------------
                    // TODO: desired parser actions for 0x43: TOUCHSET IMPORT <number> <length> <data>
                    // -------------------------------------------------------------------------------------------------
                    // v_number: Touchset number to import (using existing number will overwrite) (uint8)
                    // v_length: Number of bytes of data to import (uint16)
                    // v_data: Touchset definition data (length must match <length> argument) (string)
                }
            } else if ((uint8_t)(*test) == KG_PACKET_TOUCHSET_IMPORT) {
                // 0x44: TOUCHSET EXPORT [number]
                // Export the active or specified touchset definition.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: number (optional)
                // >>>: Touchset number to export (leave blank for active touchset)
                uint8_t v_number = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_number = *test++;
                }
    
                // respond to command if successful
                if (!protoError) {
                    // v_number: Touchset number to export (leave blank for active touchset) (uint8)
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_TOUCHSET_EXPORT);
                }
            } else if ((uint8_t)(*test) == KG_PACKET_TOUCHSET_DELETE) {
                // 0x45: TOUCHSET DELETE [number]
                // Delete the active or specified touchset definition.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: number (optional)
                // >>>: Touchset number to delete (leave blank for active touchset)
                uint8_t v_number = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_number = *test++;
                }
    
                // respond to command if successful
                if (!protoError) {
                    // ---------------------------------------------------------------
                    // TODO: desired parser actions for 0x45: TOUCHSET DELETE [number]
                    // ---------------------------------------------------------------
                    // v_number: Touchset number to delete (leave blank for active touchset) (uint8)
                }
            } else if ((uint8_t)(*test) == KG_PACKET_SYSTEM_EEPROM_STATUS) {
                // 0x50: EEPROM STATUS 
                // Get current EEPROM usage status
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_SYSTEM_EEPROM_STATUS);
                }
            } else if ((uint8_t)(*test) == KG_PACKET_SYSTEM_EEPROM_EXPORT) {
                // 0x51: EEPROM EXPORT [offset [length]]
                // Export current EEPROM data
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: offset (optional)
                // >>>: Byte offset of start of imported data (default 0)
                uint8_t v_offset = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_offset = *test++;
                }
    
                // arg: length (optional)
                // >>>: Length of data to export (default all)
                uint8_t v_length = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_length = *test++;
                }
    
                // respond to command if successful
                if (!protoError) {
                    // v_offset: Byte offset of start of imported data (default 0) (uint32_t)
                    // v_length: Length of data to export (default all) (uint32_t)
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_SYSTEM_EEPROM_EXPORT);
                }
            } else if ((uint8_t)(*test) == KG_PACKET_SYSTEM_EEPROM_IMPORT) {
                // 0x52: EEPROM IMPORT <offset> <length> <data>
                // Import (store) EEPROM data
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: offset (required)
                // >>>: Byte offset of start of imported data
                uint8_t v_offset = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_offset = *test++;
                } else protoError = 2; // missing parameter(s)
    
                // arg: length (required)
                // >>>: Length of data to import
                uint8_t v_length = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_length = *test++;
                } else protoError = 2; // missing parameter(s)
    
                // arg: data (required)
                // >>>: EEPROM data to import (length must match <length> parameter
                uint8_t* v_data = (uint8_t *)malloc(*test++ + 1);
                uint8_t v_data_length = 0;
                if (v_data == 0) protoError = 6; // could not allocate memory for parameter
                if (!protoError && test - rxPacket > rxPacketLength - 1) {
                    argCount++;
                    memcpy(v_data, test, sizeof(v_data) - 1);
                    v_data[sizeof(v_data) - 1] = 0; // null terminate
                } else protoError = 2; // missing parameter(s)
    
                // respond to command if successful
                if (!protoError) {
                    // -----------------------------------------------------------------------------------------------
                    // TODO: desired parser actions for 0x52: EEPROM IMPORT <offset> <length> <data>
                    // -----------------------------------------------------------------------------------------------
                    // v_offset: Byte offset of start of imported data (uint32_t)
                    // v_length: Length of data to import (uint32_t)
                    // v_data: EEPROM data to import (length must match <length> parameter (string)
                }
            } else if ((uint8_t)(*test) == KG_PACKET_SYSTEM_SRAM_STATUS) {
                // 0x58: SRAM STATUS 
                // Get current SRAM usage status
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_SYSTEM_SRAM_STATUS);
                }
            } else if ((uint8_t)(*test) == KG_PACKET_TOUCH_MODE) {
                // 0x60: TOUCH MODE <mode>
                // Controls the touch detection mode.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: mode (required)
                // >>>: Accelerometer mode
                uint8_t v_mode = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_mode = *test++;
                    if (!(v_mode == 0 || v_mode == 1)) protoError = 4; // invalid parameter (not accepted value)
                } else protoError = 2; // missing parameter(s)
    
                // respond to command if successful
                if (!protoError) {
                    // v_mode: Accelerometer mode (uint8)
                    #ifdef ENABLE_TOUCH
                        if (v_mode == 0) disable_touch();
                        else enable_touch();
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_TOUCH_LOGIC) {
                // 0x61: TOUCH LOGIC [logic]
                // Gets current HIGH/LOW logic reading of each sensor. (Supply logic value to emulate sensor logic)
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: logic (optional)
                // >>>: Touch sensor logic status
                uint8_t* v_logic = (uint8_t *)malloc(*test++ + 1);
                uint8_t v_logic_length = 0;
                if (v_logic == 0) protoError = 6; // could not allocate memory for parameter
                if (!protoError && test - rxPacket > rxPacketLength - 1) {
                    argCount++;
                    memcpy(v_logic, test, sizeof(v_logic) - 1);
                    v_logic[sizeof(v_logic) - 1] = 0; // null terminate
                }
    
                // respond to command if successful
                if (!protoError) {
                    // v_logic: Touch sensor logic status (string)
                    #ifdef ENABLE_TOUCH
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_TOUCH_LOGIC, v_logic);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_TOUCH_STATUS) {
                // 0x62: TOUCH STATUS [status]
                // Gets current status of base touch combinations. (Supply status value to emulate touch status)
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: status (optional)
                // >>>: Base touch combination status
                uint8_t* v_status = (uint8_t *)malloc(*test++ + 1);
                uint8_t v_status_length = 0;
                if (v_status == 0) protoError = 6; // could not allocate memory for parameter
                if (!protoError && test - rxPacket > rxPacketLength - 1) {
                    argCount++;
                    memcpy(v_status, test, sizeof(v_status) - 1);
                    v_status[sizeof(v_status) - 1] = 0; // null terminate
                }
    
                // respond to command if successful
                if (!protoError) {
                    // v_status: Base touch combination status (string)
                    #ifdef ENABLE_TOUCH
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_TOUCH_STATUS, v_status);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_SYSTEM_BATTERY_STATUS) {
                // 0x70: BATTERY STATUS 
                // Get current status of battery
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_SYSTEM_BATTERY_STATUS);
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_ACCEL_RAW) {
                // 0x81: ACCEL RAW 
                // Gets last raw accelerometer X/Y/Z measurements
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_ACCELEROMETER
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MOTION_ACCEL_RAW);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_ACCEL_FILTERED) {
                // 0x82: ACCEL FILTERED 
                // Gets last filtered accelerometer X/Y/Z measurements
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_ACCELEROMETER
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MOTION_ACCEL_FILTERED);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_GYRO_RAW) {
                // 0x84: GYRO RAW 
                // Gets last raw gyroscope X/Y/Z measurements
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_GYROSCOPE
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MOTION_GYRO_RAW);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_GYRO_FILTERED) {
                // 0x85: GYRO FILTERED 
                // Gets last filtered gyroscope X/Y/Z measurements
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_GYROSCOPE
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MOTION_GYRO_FILTERED);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_SENSOR_MODE) {
                // 0x86: ACCELGYRO MODE <mode>
                // Controls the accelerometer/gyroscope mode.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: mode (required)
                // >>>: Accel/gyro mode
                uint8_t v_mode = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_mode = *test++;
                    if (!(v_mode == 0 || v_mode == 1)) protoError = 4; // invalid parameter (not accepted value)
                } else protoError = 2; // missing parameter(s)
    
                // respond to command if successful
                if (!protoError) {
                    // v_mode: Accel/gyro mode (uint8)
                    #ifdef ENABLE_ACCELGYRO
                        if (v_mode == 0) disable_motion_accelgyro();
                        else enable_motion_accelgyro();
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_ACCELGYRO_RAW) {
                // 0x87: ACCELGYRO RAW 
                // Gets last raw accelerometer/gyroscope X/Y/Z measurements
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_ACCELGYRO
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MOTION_ACCELGYRO_RAW);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_ACCELGYRO_FILTERED) {
                // 0x88: ACCELGYRO FILTERED 
                // Gets last filtered accelerometer/gyroscope X/Y/Z measurements
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
                
                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_ACCELGYRO
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MOTION_ACCELGYRO_FILTERED);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_MAG_RAW) {
                // 0x90: MAG RAW 
                // Gets last raw magnetometer X/Y/Z measurements
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_MAGNETOMETER
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MOTION_MAG_RAW);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_MAG_FILTERED) {
                // 0x91: MAG FILTERED 
                // Gets last filtered magnetometer X/Y/Z measurements
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_MAGNETOMETER
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MOTION_MAG_FILTERED);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_SYSTEM_BT_RAW) {
                // 0xb7: BT RAW <command>
                // Send a command directly to the Bluetooth module (must be attached, obviously).
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: command (required)
                // >>>: Command to pass through to BT module
                uint8_t* v_command = (uint8_t *)malloc(*test++ + 1);
                uint8_t v_command_length = 0;
                if (v_command == 0) protoError = 6; // could not allocate memory for parameter
                if (!protoError && test - rxPacket > rxPacketLength - 1) {
                    argCount++;
                    memcpy(v_command, test, sizeof(v_command) - 1);
                    v_command[sizeof(v_command) - 1] = 0; // null terminate
                } else protoError = 2; // missing parameter(s)
    
                // respond to command if successful
                if (!protoError) {
                    // v_command: Command to pass through to BT module (string)
                    #ifdef ENABLE_BLUETOOTH
                        BTSerial.write((const uint8_t *)v_command, v_command_length);
                        BTSerial.print("\r\n");
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_IMU_MODE) {
                // 0xc0: IMU MODE <mode>
                // Controls the inertial measurement unit (IMU) mode.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: mode (required)
                // >>>: IMU mode
                uint8_t v_mode = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_mode = *test++;
                    if (!(v_mode == 0 || v_mode == 1)) protoError = 4; // invalid parameter (not accepted value)
                } else protoError = 2; // missing parameter(s)
    
                // respond to command if successful
                if (!protoError) {
                    // ------------------------------------------------------------
                    // TODO: desired parser actions for 0xc0: IMU MODE <mode>
                    // ------------------------------------------------------------
                    // v_mode: IMU mode (uint8)
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_IMU_ORIENTATION) {
                // 0xc1: IMU ORIENTATION [format]
                // Gets last calculated orientation
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: format (optional)
                uint8_t v_format = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_format = *test++;
                    if (!(v_format == 0 || v_format == 1 || v_format == 2)) protoError = 4; // invalid parameter (not accepted value)
                }
    
                // respond to command if successful
                if (!protoError) {
                    // v_format: (uint8)
                    #ifdef ENABLE_FUSION
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MOTION_IMU_ORIENTATION, 0, v_format);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_IMU_ACCELERATION) {
                // 0xc2: IMU ACCELERATION 
                // Gets last calculated X/Y/Z acceleration data from IMU, relative to initial frame of reference and
                // with gravitational effects removed.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_FUSION
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MOTION_IMU_ACCELERATION);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_IMU_VELOCITY) {
                // 0xc3: IMU VELOCITY 
                // Gets last calculated X/Y/Z velocity data from IMU, relative to initial frame of reference and with
                // gravitational effects removed.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_FUSION
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MOTION_IMU_VELOCITY);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_IMU_POSITION) {
                // 0xc4: IMU POSITION 
                // Gets last calculated X/Y/Z position data from IMU, relative to initial frame of reference and with
                // gravitational effects removed.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_FUSION
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MOTION_IMU_POSITION);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_GESTURE) {
                // 0xc8: GESTURE 
                // Gesture detected
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    // ----------------------------------------------
                    // TODO: desired parser actions for 0xc8: GESTURE
                    // ----------------------------------------------
                }
            } else if ((uint8_t)(*test) == KG_PACKET_MOTION_SENSOR_CALIBRATE) {
                // 0xd0: AUTOZERO 
                // Begins motion sensor recalibration to find the zero/still frame of reference.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_ACCELEROMETER
                        accelEnableAutoZero = true;
                    #endif
                    #ifdef ENABLE_GYROSCOPE
                        gyroEnableAutoZero = true;
                    #endif
                    #ifdef ENABLE_ACCELGYRO
                        accelGyroEnableAutoZero = true;
                    #endif
                    #ifdef ENABLE_MAGNETOMETER
                        magEnableAutoZero = true;
                    #endif
                    #if (!defined ENABLE_ACCELEROMETER && !defined ENABLE_GYROSCOPE && !defined ENABLE_ACCELGYRO && !defined ENABLE_MAGNETOMETER)
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_FEEDBACK_BLINK) {
                // 0xf1: BLINK <mode>
                // Controls the single LED for simple feedback.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: mode (required)
                // >>>: Single LED mode
                uint8_t v_mode = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_mode = *test++;
                    if (v_mode < 0 || v_mode > 5) protoError = 3; // invalid parameter (out of range, single)
                } else protoError = 2; // missing parameter(s)
    
                // respond to command if successful
                if (!protoError) {
                    // v_mode: Single LED mode (uint8)
                    #if (KG_FEEDBACK & KG_FEEDBACK_BLINK)
                        set_blink_mode(v_mode);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_FEEDBACK_RGB) {
                // 0xf2: RGB <red> <green> <blue>
                // Controls each channel of the RGB LED for visual feedback.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: red (required)
                // >>>: Red LED mode
                uint8_t v_red = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_red = *test++;
                    if (v_red < 0 || v_red > 6) protoError = 3; // invalid parameter (out of range, single)
                } else protoError = 2; // missing parameter(s)
    
                // arg: green (required)
                // >>>: Green LED mode
                uint8_t v_green = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_green = *test++;
                } else protoError = 2; // missing parameter(s)
    
                // arg: blue (required)
                // >>>: Blue LED mode
                uint8_t v_blue = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_blue = *test++;
                } else protoError = 2; // missing parameter(s)
    
                // respond to command if successful
                if (!protoError) {
                    // v_red: Red LED mode (uint8)
                    // v_green: Green LED mode
                    // v_blue: Blue LED mode
                    #if (KG_FEEDBACK & KG_FEEDBACK_RGB)
                        set_rgb_mode(v_red, v_green, v_blue);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_FEEDBACK_PIEZO) {
                // 0xf3: PIEZO <mode> [duration [frequency]]
                // Controls the piezo buzzer for audible feedback.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: mode (required)
                // >>>: Piezo buzzer mode
                uint8_t v_mode = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_mode = *test++;
                    if (v_mode < 0 || v_mode > 7) protoError = 3; // invalid parameter (out of range, single)
                } else protoError = 2; // missing parameter(s)
    
                // arg: duration (optional)
                // >>>: Duration of tone in 10 ms increments; omit or set to 0 for infinite
                uint8_t v_duration = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_duration = *test++;
                    if (v_duration < 0 || v_duration > 65535) protoError = 3; // invalid parameter (out of range, single)
                }
    
                // arg: frequency (optional)
                // >>>: Frequency of tone in Hz; omit for default 1760 Hz
                uint16_t v_frequency = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 3) {
                    argCount++;
                    v_frequency = (*test++ << 8) + (*test++);
                    if (v_frequency < 60 || v_frequency > 10000) protoError = 3; // invalid parameter (out of range, single)
                }
    
                // respond to command if successful
                if (!protoError) {
                    // v_mode: Piezo buzzer mode (uint8)
                    // v_duration: Duration of tone in 10 ms increments; omit or set to 0 for infinite (uint8)
                    // v_frequency: Frequency of tone in Hz; omit for default 1760 Hz (uint16)
                    #if (KG_FEEDBACK & KG_FEEDBACK_PIEZO)
                        if (argCount == 1) set_piezo_mode(v_mode);
                        else if (argCount == 2) set_piezo_mode(v_mode, v_duration);
                        else if (argCount == 3) set_piezo_mode(v_mode, v_duration, v_frequency);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_FEEDBACK_VIBE) {
                // 0xf4: VIBE <mode> [duration]
                // Controls the vibrating motor for haptic feedback.
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // arg: mode (required)
                // >>>: Vibrating motor mode
                uint8_t v_mode = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 2) {
                    argCount++;
                    v_mode = *test++;
                    if (v_mode < 0 || v_mode > 6) protoError = 3; // invalid parameter (out of range, single)
                } else protoError = 2; // missing parameter(s)
    
                // arg: duration (optional)
                // >>>: Duration of vibration in 10 ms increments; omit or set to 0 for infinite
                uint16_t v_duration = 0;
                if (!protoError && test - rxPacket > rxPacketLength - 3) {
                    argCount++;
                    v_duration = (*test++ << 8) + (*test++);
                    if (v_duration < 0 || v_duration > 65535) protoError = 3; // invalid parameter (out of range, single)
                }
    
                // respond to command if successful
                if (!protoError) {
                    // v_mode: Vibrating motor mode (uint8)
                    // v_duration: Duration of vibration in 10 ms increments; omit or set to 0 for infinite (uint16)
                    #if (KG_FEEDBACK & KG_FEEDBACK_VIBRATE)
                        if (argCount == 1) set_vibrate_mode(v_mode);
                        else if (argCount == 2) set_vibrate_mode(v_mode, v_duration);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if ((uint8_t)(*test) == KG_PACKET_SYSTEM_RESET) {
                // 0xff: RESET 
                // Resets the Keyglove controller board to power-on defaults
                test++;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;
    
                // respond to command if successful
                if (!protoError) {
                    setup();
                }
            }

            if (protoError || !response) {
                // [0] = command byte ACK
                // [1] = protocol error code set above
                char errorPacket[2] = { rxPacket[0], protoError };
                send_keyglove_packet(errorPacket, 2);
            } else if (response) {
                if (txPacket != NULL) {
                    // [0] = command byte ACK (same as RX command code)
                    // [1] = status (0 for success)
                    // [2...] data
                    send_keyglove_packet(txPacket, txPacketLength);
                } else {
                    // [0] = command byte ACP
                    // [1] = 7, OUT OF MEMORY FOR RESPONSE
                    char errorPacket[2] = { rxPacket[0], 0x07 };
                    send_keyglove_packet(errorPacket, 2);
                }
            }

            // reset buffer after processing
            if (txPacket != NULL) free(txPacket);
            inBinPacket = false;
            rxPacketLength = 0;
        //} else if (!inBinPacket) {
        //    Serial.write(inputByte); // local echo
        }
    }
}

uint8_t send_keyglove_info(uint8_t level, char *buffer) {
    Serial.println(buffer);
    return 0;
}

uint8_t send_keyglove_info(uint8_t level, const __FlashStringHelper *buffer) {
    Serial.println(buffer);
    return 0;
}

uint8_t send_keyglove_packet(char *buffer, uint8_t length, bool autoFree) {
    if (buffer == NULL || length == 0) return 1;
    
    #if KG_HOSTIF & KG_HOSTIF_USB_SERIAL
        // send packet out over wired serial (USB virtual serial)
        if (interfaceUSBSerialReady && (interfaceUSBSerialMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0) {
            Serial.write((const uint8_t *)buffer, length);   // packet data
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_USB_RAWHID
        // send packet out over wired custom HID interface (USB raw HID)
        // 64-byte packets, formatted where byte 0 is [0-64] and bytes 1-63 are data
        if (interfaceUSBRawHIDReady && (interfaceUSBRawHIDMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0) {
            int8_t bytes;
            for (uint8_t i = 0; i < length; i += (RAWHID_TX_SIZE - 1)) {
                memset(txRawHIDPacket, 0, RAWHID_TX_SIZE);
                txRawHIDPacket[0] = min(RAWHID_TX_SIZE - 1, length - i);
                for (uint8_t j = 0; j < (RAWHID_TX_SIZE - 1); j++) {
                    if (i + j >= length) break;
                    txRawHIDPacket[j + 1] = buffer[i + j];
                }
                bytes = RawHID.send(txRawHIDPacket, 2);
            }
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_BT2_SERIAL
        // send packet out over wireless serial (Bluetooth v2.1 SPP)
        if (interfaceBT2SerialReady && (interfaceBT2SerialMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0 && bluetoothConnectionMap[bluetoothSPPDeviceIndex] -> linkSPP != 0xFF) {
            bluetooth.smartSendData((const uint8_t *)bluetoothTXRawHIDPacket, length, bluetoothConnectionMap[bluetoothSPPDeviceIndex] -> linkSPP);
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_BT2_IAP
        // send packet out over wireless iAP link (Bluetooth v2.1 IAP)
        if (interfaceBT2IAPReady && (interfaceBT2IAPMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0 && bluetoothConnectionMap[bluetoothIAPDeviceIndex] -> linkIAP != 0xFF) {
            bluetooth.smartSendData((const uint8_t *)bluetoothTXRawHIDPacket, length, bluetoothConnectionMap[bluetoothIAPDeviceIndex] -> linkIAP);
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_BT2_RAWHID
        // send packet out over wireless custom HID interface (Bluetooth v2.1 raw HID)
        if (interfaceBT2RawHIDReady && (interfaceBT2RawHIDMode & KG_INTERFACE_MODE_OUTGOING_PACKET) != 0 && bluetoothConnectionMap[bluetoothRawHIDDeviceIndex] -> linkHIDInterrupt != 0xFF) {
            int8_t bytes;
            for (uint8_t i = 0; i < length; i += (BLUETOOTH_RAWHID_TX_SIZE - 1)) {
                memset(bluetoothTXRawHIDPacket + 4, 0, BLUETOOTH_RAWHID_TX_SIZE);
                bluetoothTXRawHIDPacket[4] = min(BLUETOOTH_RAWHID_TX_SIZE - 1, length - i);
                for (uint8_t j = 0; j < (BLUETOOTH_RAWHID_TX_SIZE - 1); j++) {
                    if (i + j >= length) break;
                    bluetoothTXRawHIDPacket[j + 5] = buffer[i + j];
                }
                bluetooth.smartSendData((const uint8_t *)bluetoothTXRawHIDPacket, BLUETOOTH_RAWHID_TX_SIZE + 4, bluetoothConnectionMap[bluetoothRawHIDDeviceIndex] -> linkHIDInterrupt);
            }
        }
    #endif

    // KG_HOSTIF_USB_HID and KG_HOSTIF_BT2_HID are handled elsewhere
    // (mouse/keyboard/joystick) and deal with other kinds of data

    if (autoFree) {
        free(buffer);
        buffer = NULL;
    }

    return 0;
}

#endif // _SUPPORT_HOSTIF_PROTOCOL_H_