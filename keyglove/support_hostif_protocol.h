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

uint16_t create_packet(char *buffer, bool binary, uint8_t type, uint8_t *paramStr, uint8_t paramNum) {
    uint16_t bufLen = 0;
    txPacket = NULL;
    switch (type) {
            case KG_PACKET_TICK:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 2)) != NULL) {
                        bufLen = 4 + 2;
                        txPacket[4] = (uint8_t)(keygloveTock >> 8);
                        txPacket[5] = (uint8_t)keygloveTock;
                    }
                } else {
                    if ((txPacket = (char *)malloc(13)) != NULL) {
                        bufLen = sprintf(txPacket, "tick\t%d\r\n", keygloveTock & 0xFFFF);
                    }
                }
                break;

            case KG_PACKET_VERSION:
                if (binary) {
                } else {
                    Serial.println(KG_FIRMWARE_NAME);
                    Serial.println(KG_FIRMWARE_AUTHOR);
                    Serial.println(KG_FIRMWARE_VERSION);
                }
                break;

            case KG_PACKET_FEATURES:
                if (binary) {
                } else {
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
                }
                break;

        #if (defined(ENABLE_ACCELEROMETER) || defined(ENABLE_ACCELGYRO) || defined(ENABLE_ACCELGYROMAG))
            case KG_PACKET_ACCEL_RAW16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 6)) != NULL) {
                        bufLen = 4 + 6;
                        txPacket[4] = aaRaw.x >> 8;
                        txPacket[5] = aaRaw.x;
                        txPacket[6] = aaRaw.y >> 8;
                        txPacket[7] = aaRaw.y;
                        txPacket[8] = aaRaw.z >> 8;
                        txPacket[9] = aaRaw.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(30)) != NULL) {
                        bufLen = sprintf(txPacket, "a_r1\t%d\t%d\t%d\r\n", aaRaw.x, aaRaw.y, aaRaw.z);
                    }
                }
                break;
            case KG_PACKET_ACCEL_RAW8:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 3)) != NULL) {
                        bufLen = 4 + 3;
                        txPacket[4] = aaRaw.x >> 8;
                        txPacket[5] = aaRaw.y >> 8;
                        txPacket[6] = aaRaw.z >> 8;
                    }
                } else {
                    if ((txPacket = (char *)malloc(23)) != NULL) {
                        bufLen = sprintf(txPacket, "a_r0\t%d\t%d\t%d\r\n", aaRaw.x >> 8, aaRaw.y >> 8, aaRaw.z >> 8);
                    }
                }
                break;
            case KG_PACKET_ACCEL_FILTERED16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 6)) != NULL) {
                        bufLen = 4 + 6;
                        txPacket[4] = aa.x >> 8;
                        txPacket[5] = aa.x;
                        txPacket[6] = aa.y >> 8;
                        txPacket[7] = aa.y;
                        txPacket[8] = aa.z >> 8;
                        txPacket[9] = aa.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(30)) != NULL) {
                        bufLen = sprintf(txPacket, "a_f1\t%d\t%dt%d\r\n", aa.x, aa.y, aa.z);
                    }
                }
                break;
            case KG_PACKET_ACCEL_FILTERED8:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 3)) != NULL) {
                        bufLen = 4 + 3;
                        txPacket[4] = aa.x >> 8;
                        txPacket[5] = aa.y >> 8;
                        txPacket[6] = aa.z >> 8;
                    }
                } else {
                    if ((txPacket = (char *)malloc(23)) != NULL) {
                        bufLen = sprintf(txPacket, "a_f0\t%d\t%d\t%d\r\n", aa.x >> 8, aa.y >> 8, aa.z >> 8);
                    }
                }
                break;
        #endif

        #if ((defined(ENABLE_ACCELEROMETER) && defined(ENABLE_GYROSCOPE)) || defined(ENABLE_ACCELGYRO) || defined(ENABLE_ACCELGYROMAG))
            case KG_PACKET_ACCELGYRO_RAW16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 12)) != NULL) {
                        bufLen = 4 + 12;
                        txPacket[4] = aaRaw.x >> 8;
                        txPacket[5] = aaRaw.x;
                        txPacket[6] = aaRaw.y >> 8;
                        txPacket[7] = aaRaw.y;
                        txPacket[8] = aaRaw.z >> 8;
                        txPacket[9] = aaRaw.z;
                        txPacket[10] = gvRaw.x >> 8;
                        txPacket[11] = gvRaw.x;
                        txPacket[12] = gvRaw.y >> 8;
                        txPacket[13] = gvRaw.y;
                        txPacket[14] = gvRaw.z >> 8;
                        txPacket[15] = gvRaw.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(51)) != NULL) {
                        bufLen = sprintf(txPacket, "ag_r1\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
                            aaRaw.x, aaRaw.y, aaRaw.z,
                            gvRaw.x, gvRaw.y, gvRaw.z);
                    }
                }
                break;
            case KG_PACKET_ACCELGYRO_RAW8:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 6)) != NULL) {
                        bufLen = 4 + 6;
                        txPacket[4] = aaRaw.x >> 8;
                        txPacket[5] = aaRaw.y >> 8;
                        txPacket[6] = aaRaw.z >> 8;
                        txPacket[7] = gvRaw.x >> 8;
                        txPacket[8] = gvRaw.y >> 8;
                        txPacket[9] = gvRaw.z >> 8;
                    }
                } else {
                    if ((txPacket = (char *)malloc(41)) != NULL) {
                        bufLen = sprintf(txPacket, "ag_r0\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
                            aaRaw.x >> 8, aaRaw.y >> 8, aaRaw.z >> 8,
                            gvRaw.x >> 8, gvRaw.y >> 8, gvRaw.z >> 8);
                    }
                }
                break;
            case KG_PACKET_ACCELGYRO_FILTERED16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 12)) != NULL) {
                        bufLen = 4 + 12;
                        txPacket[4] = aa.x >> 8;
                        txPacket[5] = aa.x;
                        txPacket[6] = aa.y >> 8;
                        txPacket[7] = aa.y;
                        txPacket[8] = aa.z >> 8;
                        txPacket[9] = aa.z;
                        txPacket[10] = gv.x >> 8;
                        txPacket[11] = gv.x;
                        txPacket[12] = gv.y >> 8;
                        txPacket[13] = gv.y;
                        txPacket[14] = gv.z >> 8;
                        txPacket[15] = gv.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(51)) != NULL) {
                        bufLen = sprintf(txPacket, "ag_f1\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
                            aa.x, aa.y, aa.z,
                            gv.x, gv.y, gv.z);
                    }
                }
                break;
            case KG_PACKET_ACCELGYRO_FILTERED8:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 6)) != NULL) {
                        bufLen = 4 + 6;
                        txPacket[4] = aa.x >> 8;
                        txPacket[5] = aa.y >> 8;
                        txPacket[6] = aa.z >> 8;
                        txPacket[7] = gv.x >> 8;
                        txPacket[8] = gv.y >> 8;
                        txPacket[9] = gv.z >> 8;
                    }
                } else {
                    if ((txPacket = (char *)malloc(41)) != NULL) {
                        bufLen = sprintf(txPacket, "ag_f0\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
                            aa.x >> 8, aa.y >> 8, aa.z >> 8,
                            gv.x >> 8, gv.y >> 8, gv.z >> 8);
                    }
                }
                break;
        #endif

        #if ((defined(ENABLE_ACCELEROMETER) && defined(ENABLE_MAGNETOMETER)) || \
             (defined(ENABLE_ACCELGYRO) && defined(ENABLE_MAGNETOMETER)) || \
             (defined(ENABLE_ACCELGYROMAG)))
            case KG_PACKET_ACCELMAG_RAW16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 12)) != NULL) {
                        bufLen = 4 + 12;
                        txPacket[4] = aaRaw.x >> 8;
                        txPacket[5] = aaRaw.x;
                        txPacket[6] = aaRaw.y >> 8;
                        txPacket[7] = aaRaw.y;
                        txPacket[8] = aaRaw.z >> 8;
                        txPacket[9] = aaRaw.z;
                        txPacket[10] = mhRaw.x >> 8;
                        txPacket[11] = mhRaw.x;
                        txPacket[12] = mhRaw.y >> 8;
                        txPacket[13] = mhRaw.y;
                        txPacket[14] = mhRaw.z >> 8;
                        txPacket[15] = mhRaw.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(51)) != NULL) {
                        bufLen = sprintf(txPacket, "am_r1\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
                            aaRaw.x, aaRaw.y, aaRaw.z,
                            mhRaw.x, mhRaw.y, mhRaw.z);
                    }
                }
                break;
            case KG_PACKET_ACCELMAG_FILTERED16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 12)) != NULL) {
                        bufLen = 4 + 12;
                        txPacket[4] = aa.x >> 8;
                        txPacket[5] = aa.x;
                        txPacket[6] = aa.y >> 8;
                        txPacket[7] = aa.y;
                        txPacket[8] = aa.z >> 8;
                        txPacket[9] = aa.z;
                        txPacket[10] = mh.x >> 8;
                        txPacket[11] = mh.x;
                        txPacket[12] = mh.y >> 8;
                        txPacket[13] = mh.y;
                        txPacket[14] = mh.z >> 8;
                        txPacket[15] = mh.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(51)) != NULL) {
                        bufLen = sprintf(txPacket, "am_f1\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
                            aa.x, aa.y, aa.z,
                            mh.x, mh.y, mh.z);
                    }
                }
                break;
        #endif

        #if ((defined(ENABLE_ACCELEROMETER) && defined(ENABLE_GYROSCOPE) && defined(ENABLE_MAGNETOMETER)) || \
             (defined(ENABLE_ACCELGYRO) && defined(ENABLE_MAGNETOMETER)) || \
             (defined(ENABLE_ACCELGYROMAG)))
            case KG_PACKET_ACCELGYROMAG_RAW16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 18)) != NULL) {
                        bufLen = 4 + 18;
                        txPacket[4] = aaRaw.x >> 8;
                        txPacket[5] = aaRaw.x;
                        txPacket[6] = aaRaw.y >> 8;
                        txPacket[7] = aaRaw.y;
                        txPacket[8] = aaRaw.z >> 8;
                        txPacket[9] = aaRaw.z;
                        txPacket[10] = gvRaw.x >> 8;
                        txPacket[11] = gvRaw.x;
                        txPacket[12] = gvRaw.y >> 8;
                        txPacket[13] = gvRaw.y;
                        txPacket[14] = gvRaw.z >> 8;
                        txPacket[15] = gvRaw.z;
                        txPacket[16] = mhRaw.x >> 8;
                        txPacket[17] = mhRaw.x;
                        txPacket[18] = mhRaw.y >> 8;
                        txPacket[19] = mhRaw.y;
                        txPacket[20] = mhRaw.z >> 8;
                        txPacket[21] = mhRaw.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(72)) != NULL) {
                        bufLen = sprintf(txPacket, "amg_r1\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
                            aaRaw.x, aaRaw.y, aaRaw.z,
                            gvRaw.x, gvRaw.y, gvRaw.z,
                            mhRaw.x, mhRaw.y, mhRaw.z);
                    }
                }
                break;
            case KG_PACKET_ACCELGYROMAG_FILTERED16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 18)) != NULL) {
                        bufLen = 4 + 18;
                        txPacket[4] = aa.x >> 8;
                        txPacket[5] = aa.x;
                        txPacket[6] = aa.y >> 8;
                        txPacket[7] = aa.y;
                        txPacket[8] = aa.z >> 8;
                        txPacket[9] = aa.z;
                        txPacket[10] = gv.x >> 8;
                        txPacket[11] = gv.x;
                        txPacket[12] = gv.y >> 8;
                        txPacket[13] = gv.y;
                        txPacket[14] = gv.z >> 8;
                        txPacket[15] = gv.z;
                        txPacket[16] = mh.x >> 8;
                        txPacket[17] = mh.x;
                        txPacket[18] = mh.y >> 8;
                        txPacket[19] = mh.y;
                        txPacket[20] = mh.z >> 8;
                        txPacket[21] = mh.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(72)) != NULL) {
                        bufLen = sprintf(txPacket, "agm_f1\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
                            aa.x, aa.y, aa.z,
                            gv.x, gv.y, gv.z,
                            mh.x, mh.y, mh.z);
                    }
                }
                break;
        #endif

        #if (defined(ENABLE_GYROSCOPE) || defined(ENABLE_ACCELGYRO) || defined(ENABLE_ACCELGYROMAG))
            case KG_PACKET_GYRO_RAW16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 6)) != NULL) {
                        bufLen = 4 + 6;
                        txPacket[4] = gvRaw.x >> 8;
                        txPacket[5] = gvRaw.x;
                        txPacket[6] = gvRaw.y >> 8;
                        txPacket[7] = gvRaw.y;
                        txPacket[8] = gvRaw.z >> 8;
                        txPacket[9] = gvRaw.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(30)) != NULL) {
                        bufLen = sprintf(txPacket, "g_r1\t%d\t%d\t%d\r\n", gvRaw.x, gvRaw.y, gvRaw.z);
                    }
                }
                break;
            case KG_PACKET_GYRO_FILTERED16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 6)) != NULL) {
                        bufLen = 4 + 6;
                        txPacket[4] = gv.x >> 8;
                        txPacket[5] = gv.x;
                        txPacket[6] = gv.y >> 8;
                        txPacket[7] = gv.y;
                        txPacket[8] = gv.z >> 8;
                        txPacket[9] = gv.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(30)) != NULL) {
                        bufLen = sprintf(txPacket, "g_f1\t%d\t%d\t%d\r\n", gv.x, gv.y, gv.z);
                    }
                }
                break;
        #endif
    
        #if ((defined(ENABLE_GYRO) && defined(ENABLE_MAGNETOMETER)) || \
             (defined(ENABLE_ACCELGYRO) && defined(ENABLE_MAGNETOMETER)) || \
             (defined(ENABLE_ACCELGYROMAG)))
            case KG_PACKET_GYROMAG_RAW16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 12)) != NULL) {
                        bufLen = 4 + 12;
                        txPacket[4] = gvRaw.x >> 8;
                        txPacket[5] = gvRaw.x;
                        txPacket[6] = gvRaw.y >> 8;
                        txPacket[7] = gvRaw.y;
                        txPacket[8] = gvRaw.z >> 8;
                        txPacket[9] = gvRaw.z;
                        txPacket[10] = mhRaw.x >> 8;
                        txPacket[11] = mhRaw.x;
                        txPacket[12] = mhRaw.y >> 8;
                        txPacket[13] = mhRaw.y;
                        txPacket[14] = mhRaw.z >> 8;
                        txPacket[15] = mhRaw.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(52)) != NULL) {
                        bufLen = sprintf(txPacket, "gm_r1\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
                            gvRaw.x, gvRaw.y, gvRaw.z,
                            mhRaw.x, mhRaw.y, mhRaw.z);
                    }
                }
                break;
            case KG_PACKET_GYROMAG_FILTERED16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 12)) != NULL) {
                        bufLen = 4 + 12;
                        txPacket[4] = gv.x >> 8;
                        txPacket[5] = gv.x;
                        txPacket[6] = gv.y >> 8;
                        txPacket[7] = gv.y;
                        txPacket[8] = gv.z >> 8;
                        txPacket[9] = gv.z;
                        txPacket[10] = mh.x >> 8;
                        txPacket[11] = mh.x;
                        txPacket[12] = mh.y >> 8;
                        txPacket[13] = mh.y;
                        txPacket[14] = mh.z >> 8;
                        txPacket[15] = mh.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(52)) != NULL) {
                        bufLen = sprintf(txPacket, "gm_f1\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
                            gv.x, gv.y, gv.z,
                            mh.x, mh.y, mh.z);
                    }
                }
                break;
        #endif

        #if (defined(ENABLE_MAGNETOMETER) || defined(ENABLE_ACCELGYROMAG))
            case KG_PACKET_MAG_RAW16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 6)) != NULL) {
                        bufLen = 4 + 6;
                        txPacket[4] = mhRaw.x >> 8;
                        txPacket[5] = mhRaw.x;
                        txPacket[6] = mhRaw.y >> 8;
                        txPacket[7] = mhRaw.y;
                        txPacket[8] = mhRaw.z >> 8;
                        txPacket[9] = mhRaw.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(30)) != NULL) {
                        bufLen = sprintf(txPacket, "m_r1\t%d\t%d\t%d\r\n", mhRaw.x, mhRaw.y, mhRaw.z);
                    }
                }
                break;
            case KG_PACKET_MAG_FILTERED16:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 6)) != NULL) {
                        bufLen = 4 + 6;
                        txPacket[4] = mh.x >> 8;
                        txPacket[5] = mh.x;
                        txPacket[6] = mh.y >> 8;
                        txPacket[7] = mh.y;
                        txPacket[8] = mh.z >> 8;
                        txPacket[9] = mh.z;
                    }
                } else {
                    if ((txPacket = (char *)malloc(30)) != NULL) {
                        bufLen = sprintf(txPacket, "m_f1\t%d\t%d\t%d\r\n", mh.x, mh.y, mh.z);
                    }
                }
                break;
        #endif

        #if (KG_FEEDBACK & KG_FEEDBACK_BLINK)
            case KG_PACKET_BLINK:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 1)) != NULL) {
                        bufLen = 4 + 1;
                        txPacket[4] = blinkMode;
                    }
                } else {
                    if ((txPacket = (char *)malloc(10)) != NULL) {
                        bufLen = sprintf(txPacket, "blink\t%d\r\n", blinkMode);
                    }
                }
                break;
        #endif

        #if (KG_FEEDBACK & KG_FEEDBACK_RGB)
            case KG_PACKET_RGB:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 3)) != NULL) {
                        bufLen = 4 + 3;
                        txPacket[4] = rgbBlinkRed;
                        txPacket[5] = rgbBlinkGreen;
                        txPacket[6] = rgbBlinkBlue;
                    }
                } else {
                    if ((txPacket = (char *)malloc(14)) != NULL) {
                        bufLen = sprintf(txPacket, "rgb\t%d\t%d\t%d\r\n", rgbBlinkRed, rgbBlinkGreen, rgbBlinkBlue);
                    }
                }
                break;
        #endif

        #if (KG_FEEDBACK & KG_FEEDBACK_PIEZO)
            case KG_PACKET_PIEZO:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 5)) != NULL) {
                        bufLen = 4 + 5;
                        txPacket[4] = piezoMode;
                        txPacket[5] = (uint8_t)(piezoTickLimit >> 8);
                        txPacket[6] = (uint8_t)piezoTickLimit;
                        txPacket[7] = (uint8_t)(piezoFrequency >> 8);
                        txPacket[8] = (uint8_t)piezoFrequency;
                    }
                } else {
                    if ((txPacket = (char *)malloc(20)) != NULL) {
                        bufLen = sprintf(txPacket, "piezo\t%d\t%ld\t%ld\r\n", piezoMode, piezoTickLimit, piezoFrequency);
                    }
                }
                break;
        #endif

        #if (KG_FEEDBACK & KG_FEEDBACK_VIBE)
            case KG_PACKET_VIBE:
                if (binary) {
                    if ((txPacket = (char *)malloc(4 + 3)) != NULL) {
                        bufLen = 4 + 3;
                        txPacket[4] = vibrateMode;
                        txPacket[5] = (uint8_t)(vibrateTickLimit >> 8);
                        txPacket[6] = (uint8_t)vibrateTickLimit;
                    }
                } else {
                    if ((txPacket = (char *)malloc(14)) != NULL) {
                        bufLen = sprintf(txPacket, "vibe\t%d\t%ld\r\n", vibrateMode, vibrateTickLimit);
                    }
                }
                break;
        #endif

        #if (KG_TOUCHCONN > 0)
            case KG_PACKET_TOUCH_STATUS:
                if (binary) {
                    uint8_t touchBytes = KG_BASE_COMBINATIONS / 8 + 1;
                    if ((txPacket = (char *)malloc(4 + touchBytes)) != NULL) {
                        bufLen = 4 + touchBytes;
                        txPacket[4] = (uint8_t)(sensors2 >> 24);
                        txPacket[5] = (uint8_t)(sensors2 >> 16);
                        txPacket[6] = (uint8_t)(sensors2 >> 8);
                        txPacket[7] = (uint8_t)sensors2;
                        txPacket[8] = (uint8_t)(sensors1 >> 24);
                        txPacket[9] = (uint8_t)(sensors1 >> 16);
                        txPacket[10] = (uint8_t)(sensors1 >> 8);
                        txPacket[11] = (uint8_t)sensors1;
                    }
                } else {
                    if ((txPacket = (char *)malloc(25)) != NULL) {
                        bufLen = sprintf(txPacket, "touch\t%08lX%08lX\r\n", sensors2, sensors1);
                    }
                }
                break;
        #endif
    }

    // finish up binary packet creation
    if (binary && txPacket != NULL) {
        txPacket[0] = 0xB8;       // "bait" byte
        txPacket[1] = bufLen - 2; // binary packet data size
        txPacket[2] = type;       // command byte ACK, or event type
        txPacket[3] = 0x00;       // status byte (0 = success)
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
    if (!inBinPacket && (inputByte == '\r' || inputByte == '\n')) {
        // end of a simple text command, so terminate the string
        rxPacket[rxPacketLength] = 0;
        protoError = 1; // invalid command (assumed)
        response = false;
        //txPacket = NULL;

        char *test = (char *)rxPacket;
        Serial.print("\r\n"); // local echo

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Serial protocol text parser code generated from kgprotocol-0.1.txt @ Wed, 30 May 2012 23:20:40 -0600 //
        //////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (strncasecmp2("ACCEL ", test, 6) == 0) {
            // ACCEL parent level
            test += 6;
            if (strncasecmp2("FILTERED", test, 8) == 0) {
                // 0x82: ACCEL FILTERED 
                // Gets last filtered accelerometer X/Y/Z measurements
                test += 9;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_ACCELEROMETER
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_ACCEL_FILTERED16);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if (strncasecmp2("MODE", test, 4) == 0) {
                // 0x80: ACCEL MODE <mode>
                // Controls the accelerometer mode.
                test += 5;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: mode (required)
                // >>>: Accelerometer mode
                uint8_t v_mode = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_mode = strtoul(test, &test, 10);
                    test++;
                    if (!(v_mode == 0 || v_mode == 1)) protoError = 4; // invalid parameter (not accepted value)
                } else protoError = 2; // missing parameter(s)

                // respond to command if successful
                if (!protoError) {
                    // v_mode: Accelerometer mode (uint8)
                    #ifdef ENABLE_ACCELEROMETER
                        if (v_mode == 0) disable_motion_accelerometer();
                        else enable_motion_accelerometer();
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if (strncasecmp2("RAW", test, 3) == 0) {
                // 0x81: ACCEL RAW 
                // Gets last raw accelerometer X/Y/Z measurements
                test += 4;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_ACCELEROMETER
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_ACCEL_RAW16);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            }
        } else if (strncasecmp2("ACCELGYRO ", test, 10) == 0) {
            // ACCELGYRO parent level
            test += 10;
            if (strncasecmp2("FILTERED", test, 8) == 0) {
                // 0x88: ACCELGYRO FILTERED 
                // Gets last filtered accelerometer/gyroscope X/Y/Z measurements
                test += 9;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_ACCELGYRO
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_ACCELGYRO_FILTERED16);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if (strncasecmp2("MODE", test, 4) == 0) {
                // 0x86: ACCELGYRO MODE <mode>
                // Controls the accelerometer/gyroscope mode.
                test += 5;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: mode (required)
                // >>>: Accel/gyro mode
                uint8_t v_mode = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_mode = strtoul(test, &test, 10);
                    test++;
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
            } else if (strncasecmp2("RAW", test, 3) == 0) {
                // 0x87: ACCELGYRO RAW 
                // Gets last raw accelerometer/gyroscope X/Y/Z measurements
                test += 4;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_ACCELGYRO
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_ACCELGYRO_RAW16);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            }
        } else if (strncasecmp2("AUTOZERO", test, 8) == 0) {
            // 0xD0: AUTOZERO 
            // Begins motion sensor recalibration to find the zero/still frame of reference.
            test += 9;
            protoError = 0; // valid command, no error (assumed)

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
        } else if (strncasecmp2("BATTERY ", test, 8) == 0) {
            // BATTERY parent level
            test += 8;
            if (strncasecmp2("STATUS", test, 6) == 0) {
                // 0x70: BATTERY STATUS 
                // Get current status of battery
                test += 7;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_EEPROM_STATUS);
                }
            }
        } else if (strncasecmp2("BLINK", test, 5) == 0) {
            // 0xF1: BLINK <mode>
            // Controls the single LED for simple feedback.
            test += 6;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // arg: mode (required)
            // >>>: Single LED mode
            uint8_t v_mode = 0;
            if (!protoError && test - rxPacket < rxPacketLength) {
                argCount++;
                if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                v_mode = strtoul(test, &test, 10);
                test++;
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
        } else if (strncasecmp2("BT ", test, 3) == 0) {
            // BT parent level
            test += 3;
            if (strncasecmp2("RAW", test, 3) == 0) {
                // 0xB7: BT RAW <command>
                // Send a command directly to the Bluetooth module (must be attached, obviously).
                test += 4;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: command (required)
                // >>>: Command to pass through to BT module
                uint8_t *v_command = 0;
                uint8_t v_command_length = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    v_command_length = rxPacketLength - (test - rxPacket);
                    v_command = (uint8_t *)malloc(v_command_length + 1);
                    if (v_command == 0) protoError = 6; // could not allocate memory for parameter
                    else {
                        memcpy(v_command, test, v_command_length);
                        v_command[v_command_length] = 0; // null terminate
                    }
                    argCount++;
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
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: offset (optional)
                // >>>: Byte offset of start of imported data (default 0)
                uint8_t v_offset = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_offset = (test, &test, 10);
                    test++;
                }

                // arg: length (optional)
                // >>>: Length of data to export (default all)
                uint8_t v_length = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_length = (test, &test, 10);
                    test++;
                }

                // respond to command if successful
                if (!protoError) {
                    // v_offset: Byte offset of start of imported data (default 0) (uint32_t)
                    // v_length: Length of data to export (default all) (uint32_t)
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_EEPROM_EXPORT);
                }
            } else if (strncasecmp2("IMPORT", test, 6) == 0) {
                // 0x52: EEPROM IMPORT <offset> <length> <data>
                // Import (store) EEPROM data
                test += 7;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: offset (required)
                // >>>: Byte offset of start of imported data
                uint8_t v_offset = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_offset = (test, &test, 10);
                    test++;
                } else protoError = 2; // missing parameter(s)

                // arg: length (required)
                // >>>: Length of data to import
                uint8_t v_length = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_length = (test, &test, 10);
                    test++;
                } else protoError = 2; // missing parameter(s)

                // arg: data (required)
                // >>>: EEPROM data to import (length must match <length> parameter
                uint8_t *v_data = 0;
                uint8_t v_data_length = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    v_data_length = rxPacketLength - (test - rxPacket);
                    v_data = (uint8_t *)malloc(v_data_length + 1);
                    if (v_data == 0) protoError = 6; // could not allocate memory for parameter
                    else {
                        memcpy(v_data, test, v_data_length);
                        v_data[v_data_length] = 0; // null terminate
                    }
                    argCount++;
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

                // free any dynamically allocated memory
                if (v_data != 0) free(v_data);
            } else if (strncasecmp2("STATUS", test, 6) == 0) {
                // 0x50: EEPROM STATUS 
                // Get current EEPROM usage status
                test += 7;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_EEPROM_STATUS);
                }
            }
        } else if (strncasecmp2("FEATURES", test, 8) == 0) {
            // 0x11: FEATURES 
            // Get enabled features and modules (such as sensors, wireless, feedback, external EEPROM, etc.)
            test += 9;
            protoError = 0; // valid command, no error (assumed)

            // respond to command if successful
            if (!protoError) {
                response = true;
                txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_FEATURES);
            }
        } else if (strncasecmp2("GESTURE", test, 7) == 0) {
            // 0xC8: GESTURE 
            // Gesture detected
            test += 8;
            protoError = 0; // valid command, no error (assumed)

            // respond to command if successful
            if (!protoError) {
                // ----------------------------------------------
                // TODO: desired parser actions for 0xC8: GESTURE
                // ----------------------------------------------
            }
        } else if (strncasecmp2("GYRO ", test, 5) == 0) {
            // GYRO parent level
            test += 5;
            if (strncasecmp2("FILTERED", test, 8) == 0) {
                // 0x85: GYRO FILTERED 
                // Gets last filtered gyroscope X/Y/Z measurements
                test += 9;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_GYROSCOPE
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_GYRO_FILTERED16);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if (strncasecmp2("MODE", test, 4) == 0) {
                // 0x83: GYRO MODE <mode>
                // Controls the gyroscope mode.
                test += 5;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: mode (required)
                // >>>: Gyroscope mode
                uint8_t v_mode = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_mode = strtoul(test, &test, 10);
                    test++;
                    if (!(v_mode == 0 || v_mode == 1)) protoError = 4; // invalid parameter (not accepted value)
                } else protoError = 2; // missing parameter(s)

                // respond to command if successful
                if (!protoError) {
                    // v_mode: Gyroscope mode (uint8)
                    #ifdef ENABLE_GYROSCOPE
                        if (v_mode == 0) disable_motion_gyroscope();
                        else enable_motion_gyroscope();
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if (strncasecmp2("RAW", test, 3) == 0) {
                // 0x84: GYRO RAW 
                // Gets last raw gyroscope X/Y/Z measurements
                test += 4;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_GYROSCOPE
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_GYRO_RAW16);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            }
        } else if (strncasecmp2("IMU ", test, 4) == 0) {
            // IMU parent level
            test += 4;
            if (strncasecmp2("ACCELERATION", test, 12) == 0) {
                // 0xC2: IMU ACCELERATION 
                // Gets last calculated X/Y/Z acceleration data from IMU, relative to initial frame of reference and
                // with gravitational effects removed.
                test += 13;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_FUSION
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_IMU_ACCELERATION16);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if (strncasecmp2("MODE", test, 4) == 0) {
                // 0xC0: IMU MODE <mode>
                // Controls the inertial measurement unit (IMU) mode.
                test += 5;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: mode (required)
                // >>>: IMU mode
                uint8_t v_mode = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_mode = strtoul(test, &test, 10);
                    test++;
                    if (!(v_mode == 0 || v_mode == 1)) protoError = 4; // invalid parameter (not accepted value)
                } else protoError = 2; // missing parameter(s)

                // respond to command if successful
                if (!protoError) {
                    // ------------------------------------------------------------
                    // TODO: desired parser actions for 0xC0: IMU MODE <mode>
                    // ------------------------------------------------------------
                    // v_mode: IMU mode (uint8)
                }
            } else if (strncasecmp2("ORIENTATION", test, 11) == 0) {
                // 0xC1: IMU ORIENTATION [format]
                // Gets last calculated orientation
                test += 12;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: format (optional)
                uint8_t v_format = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_format = strtoul(test, &test, 10);
                    test++;
                    if (!(v_format == 0 || v_format == 1 || v_format == 2)) protoError = 4; // invalid parameter (not accepted value)
                }

                // respond to command if successful
                if (!protoError) {
                    // v_format: (uint8)
                    #ifdef ENABLE_FUSION
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_IMU_ORIENTATION16, 0, v_format);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if (strncasecmp2("POSITION", test, 8) == 0) {
                // 0xC4: IMU POSITION 
                // Gets last calculated X/Y/Z position data from IMU, relative to initial frame of reference and
                // with gravitational effects removed.
                test += 9;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_FUSION
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_IMU_POSITION16);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if (strncasecmp2("VELOCITY", test, 8) == 0) {
                // 0xC3: IMU VELOCITY 
                // Gets last calculated X/Y/Z velocity data from IMU, relative to initial frame of reference and
                // with gravitational effects removed.
                test += 9;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_FUSION
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_IMU_VELOCITY16);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            }
        } else if (strncasecmp2("MAG ", test, 4) == 0) {
            // MAG parent level
            test += 4;
            if (strncasecmp2("FILTERED", test, 8) == 0) {
                // 0x91: MAG FILTERED 
                // Gets last filtered magnetometer X/Y/Z measurements
                test += 9;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_MAGNETOMETER
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MAG_FILTERED16);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if (strncasecmp2("MODE", test, 4) == 0) {
                // 0x89: MAG MODE <mode>
                // Controls the magnetometer mode.
                test += 5;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: mode (required)
                // >>>: Magnetometer mode
                uint8_t v_mode = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_mode = strtoul(test, &test, 10);
                    test++;
                    if (!(v_mode == 0 || v_mode == 1)) protoError = 4; // invalid parameter (not accepted value)
                } else protoError = 2; // missing parameter(s)

                // respond to command if successful
                if (!protoError) {
                    // v_mode: Magnetometer mode (uint8)
                    #ifdef ENABLE_MAGNETOMETER
                        if (v_mode == 1) disable_motion_magnetometer();
                        else enable_motion_magnetometer();
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            } else if (strncasecmp2("RAW", test, 3) == 0) {
                // 0x90: MAG RAW 
                // Gets last raw magnetometer X/Y/Z measurements
                test += 4;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    #ifdef ENABLE_MAGNETOMETER
                        response = true;
                        txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MAG_RAW16);
                    #else
                        protoError = 255; // not supported by hardware/firmware
                    #endif
                }
            }
        } else if (strncasecmp2("OPTION", test, 6) == 0) {
            // 0x20: OPTION [name [value]]
            // List current module option settings, or get or set single module option value
            test += 7;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // arg: name (optional)
            // >>>: Name of option to get or set
            uint8_t *v_name = 0;
            uint8_t v_name_length = 0;
            if (!protoError && test - rxPacket < rxPacketLength) {
                v_name_length = rxPacketLength - (test - rxPacket);
                v_name = (uint8_t *)malloc(v_name_length + 1);
                if (v_name == 0) protoError = 6; // could not allocate memory for parameter
                else {
                    memcpy(v_name, test, v_name_length);
                    v_name[v_name_length] = 0; // null terminate
                }
                argCount++;
            }

            // arg: value (optional)
            // >>>: New option value to set
            int32_t v_value = 0;
            if (!protoError && test - rxPacket < rxPacketLength) {
                argCount++;
                if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                v_value = strtol(test, &test, 10);
                test++;
            }

            // respond to command if successful
            if (!protoError) {
                // v_name: Name of option to get or set (string)
                // v_value: New option value to set (int32)
                if (argCount == 0) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_OPTION);
                } else if (argCount == 1) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_OPTION, v_name);
                } else if (argCount == 2) {
                    // TODO: SET OPTION
                }
            }

            // free any dynamically allocated memory
            if (v_name != 0) free(v_name);
        } else if (strncasecmp2("PIEZO", test, 5) == 0) {
            // 0xF3: PIEZO <mode> [duration [frequency]]
            // Controls the piezo buzzer for audible feedback.
            test += 6;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // arg: mode (required)
            // >>>: Piezo buzzer mode
            uint8_t v_mode = 0;
            if (!protoError && test - rxPacket < rxPacketLength) {
                argCount++;
                if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                v_mode = strtoul(test, &test, 10);
                test++;
                if (v_mode < 0 || v_mode > 7) protoError = 3; // invalid parameter (out of range, single)
            } else protoError = 2; // missing parameter(s)

            // arg: duration (optional)
            // >>>: Duration of tone in 10 ms increments; omit or set to 0 for infinite
            uint8_t v_duration = 0;
            if (!protoError && test - rxPacket < rxPacketLength) {
                argCount++;
                if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                v_duration = strtoul(test, &test, 10);
                test++;
                if (v_duration < 0 || v_duration > 65535) protoError = 3; // invalid parameter (out of range, single)
            }

            // arg: frequency (optional)
            // >>>: Frequency of tone in Hz; omit for default 1760 Hz
            uint16_t v_frequency = 0;
            if (!protoError && test - rxPacket < rxPacketLength) {
                argCount++;
                if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                v_frequency = strtoul(test, &test, 10);
                test++;
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
        } else if (strncasecmp2("RESET", test, 5) == 0) {
            // 0xFF: RESET 
            // Resets the Keyglove controller board to power-on defaults
            test += 6;
            protoError = 0; // valid command, no error (assumed)

            // respond to command if successful
            if (!protoError) {
                setup();
            }
        } else if (strncasecmp2("RGB", test, 3) == 0) {
            // 0xF2: RGB <red> <green> <blue>
            // Controls each channel of the RGB LED for visual feedback.
            test += 4;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // arg: red (required)
            // >>>: Red LED mode
            uint8_t v_red = 0;
            if (!protoError && test - rxPacket < rxPacketLength) {
                argCount++;
                if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                v_red = strtoul(test, &test, 10);
                test++;
                if (v_red < 0 || v_red > 6) protoError = 3; // invalid parameter (out of range, single)
            } else protoError = 2; // missing parameter(s)

            // arg: green (required)
            // >>>: Green LED mode
            uint8_t v_green = 0;
            if (!protoError && test - rxPacket < rxPacketLength) {
                argCount++;
                if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                v_green = strtoul(test, &test, 10);
                test++;
            } else protoError = 2; // missing parameter(s)

            // arg: blue (required)
            // >>>: Blue LED mode
            uint8_t v_blue = 0;
            if (!protoError && test - rxPacket < rxPacketLength) {
                argCount++;
                if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                v_blue = strtoul(test, &test, 10);
                test++;
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
        } else if (strncasecmp2("SRAM ", test, 5) == 0) {
            // SRAM parent level
            test += 5;
            if (strncasecmp2("STATUS", test, 6) == 0) {
                // 0x58: SRAM STATUS 
                // Get current SRAM usage status
                test += 7;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_SRAM_STATUS);
                }
            }
        } else if (strncasecmp2("TOUCH ", test, 6) == 0) {
            // TOUCH parent level
            test += 6;
            if (strncasecmp2("LOGIC", test, 5) == 0) {
                // 0x61: TOUCH LOGIC [logic]
                // Gets current HIGH/LOW logic reading of each sensor. (Supply logic value to emulate sensor logic)
                test += 6;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: logic (optional)
                // >>>: Touch sensor logic status
                uint8_t *v_logic = 0;
                uint8_t v_logic_length = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    v_logic_length = rxPacketLength - (test - rxPacket);
                    v_logic = (uint8_t *)malloc(v_logic_length + 1);
                    if (v_logic == 0) protoError = 6; // could not allocate memory for parameter
                    else {
                        memcpy(v_logic, test, v_logic_length);
                        v_logic[v_logic_length] = 0; // null terminate
                    }
                    argCount++;
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

                // free any dynamically allocated memory
                if (v_logic != 0) free(v_logic);
            } else if (strncasecmp2("MODE", test, 4) == 0) {
                // 0x60: TOUCH MODE <mode>
                // Controls the touch detection mode.
                test += 5;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: mode (required)
                // >>>: Accelerometer mode
                uint8_t v_mode = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_mode = strtoul(test, &test, 10);
                    test++;
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
            } else if (strncasecmp2("STATUS", test, 6) == 0) {
                // 0x62: TOUCH STATUS [status]
                // Gets current status of base touch combinations. (Supply status value to emulate touch status)
                test += 7;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: status (optional)
                // >>>: Base touch combination status
                uint8_t *v_status = 0;
                uint8_t v_status_length = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    v_status_length = rxPacketLength - (test - rxPacket);
                    v_status = (uint8_t *)malloc(v_status_length + 1);
                    if (v_status == 0) protoError = 6; // could not allocate memory for parameter
                    else {
                        memcpy(v_status, test, v_status_length);
                        v_status[v_status_length] = 0; // null terminate
                    }
                    argCount++;
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

                // free any dynamically allocated memory
                if (v_status != 0) free(v_status);
            }
        } else if (strncasecmp2("TOUCHSET ", test, 9) == 0) {
            // TOUCHSET parent level
            test += 9;
            if (strncasecmp2("DELETE", test, 6) == 0) {
                // 0x45: TOUCHSET DELETE [number]
                // Delete the active or specified touchset definition.
                test += 7;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: number (optional)
                // >>>: Touchset number to delete (leave blank for active touchset)
                uint8_t v_number = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_number = strtoul(test, &test, 10);
                    test++;
                }

                // respond to command if successful
                if (!protoError) {
                    // ---------------------------------------------------------------
                    // TODO: desired parser actions for 0x45: TOUCHSET DELETE [number]
                    // ---------------------------------------------------------------
                    // v_number: Touchset number to delete (leave blank for active touchset) (uint8)
                }
            } else if (strncasecmp2("EXPORT", test, 6) == 0) {
                // 0x44: TOUCHSET EXPORT [number]
                // Export the active or specified touchset definition.
                test += 7;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: number (optional)
                // >>>: Touchset number to export (leave blank for active touchset)
                uint8_t v_number = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_number = strtoul(test, &test, 10);
                    test++;
                }

                // respond to command if successful
                if (!protoError) {
                    // v_number: Touchset number to export (leave blank for active touchset) (uint8)
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_TOUCHSET_EXPORT);
                }
            } else if (strncasecmp2("IMPORT", test, 6) == 0) {
                // 0x43: TOUCHSET IMPORT <number> <length> <data>
                // Import (store) a new touchset definition.
                test += 7;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: number (required)
                // >>>: Touchset number to import (using existing number will overwrite)
                uint8_t v_number = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_number = strtoul(test, &test, 10);
                    test++;
                } else protoError = 2; // missing parameter(s)

                // arg: length (required)
                // >>>: Number of bytes of data to import
                uint16_t v_length = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_length = strtoul(test, &test, 10);
                    test++;
                } else protoError = 2; // missing parameter(s)

                // arg: data (required)
                // >>>: Touchset definition data (length must match <length> argument)
                uint8_t *v_data = 0;
                uint8_t v_data_length = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    v_data_length = rxPacketLength - (test - rxPacket);
                    v_data = (uint8_t *)malloc(v_data_length + 1);
                    if (v_data == 0) protoError = 6; // could not allocate memory for parameter
                    else {
                        memcpy(v_data, test, v_data_length);
                        v_data[v_data_length] = 0; // null terminate
                    }
                    argCount++;
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

                // free any dynamically allocated memory
                if (v_data != 0) free(v_data);
            } else if (strncasecmp2("INFO", test, 4) == 0) {
                // 0x41: TOUCHSET INFO [number]
                // Show information about active or specified touchset.
                test += 5;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: number (optional)
                // >>>: Touchset number to describe (leave blank for active touchset)
                uint8_t v_number = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_number = strtoul(test, &test, 10);
                    test++;
                }

                // respond to command if successful
                if (!protoError) {
                    // v_number: Touchset number to describe (leave blank for active touchset) (uint8)
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_TOUCHSET_INFO);
                }
            } else if (strncasecmp2("LIST", test, 4) == 0) {
                // 0x40: TOUCHSET LIST
                // List all programmed touchsets.
                test += 5;
                protoError = 0; // valid command, no error (assumed)

                // respond to command if successful
                if (!protoError) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_TOUCHSET_LIST);
                }
            } else if (strncasecmp2("SELECT", test, 6) == 0) {
                // 0x42: TOUCHSET SELECT <number>
                // Activate a new touchset.
                test += 7;
                protoError = 0; // valid command, no error (assumed)
                uint8_t argCount = 0;

                // arg: number (required)
                // >>>: Touchset number to select
                uint8_t v_number = 0;
                if (!protoError && test - rxPacket < rxPacketLength) {
                    argCount++;
                    if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                    v_number = strtoul(test, &test, 10);
                    test++;
                } else protoError = 2; // missing parameter(s)

                // respond to command if successful
                if (!protoError) {
                    // ---------------------------------------------------------------------
                    // TODO: desired parser actions for 0x42: TOUCHSET SELECT <number>
                    // ---------------------------------------------------------------------
                    // v_number: Touchset number to select (uint8)
                }
            }
        } else if (strncasecmp2("VERSION", test, 7) == 0) {
            // 0x10: VERSION 
            // Get the firmware version installed on the controller board
            test += 8;
            protoError = 0; // valid command, no error (assumed)

            // respond to command if successful
            if (!protoError) {
                response = true;
                txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_VERSION);
            }
        } else if (strncasecmp2("VIBE", test, 4) == 0) {
            // 0xF4: VIBE <mode> [duration]
            // Controls the vibrating motor for haptic feedback.
            test += 5;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // arg: mode (required)
            // >>>: Vibrating motor mode
            uint8_t v_mode = 0;
            if (!protoError && test - rxPacket < rxPacketLength) {
                argCount++;
                if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                v_mode = strtoul(test, &test, 10);
                test++;
                if (v_mode < 0 || v_mode > 6) protoError = 3; // invalid parameter (out of range, single)
            } else protoError = 2; // missing parameter(s)

            // arg: duration (optional)
            // >>>: Duration of vibration in 10 ms increments; omit or set to 0 for infinite
            uint16_t v_duration = 0;
            if (!protoError && test - rxPacket < rxPacketLength) {
                argCount++;
                if (test[0] < '0' || test[0] > '9') protoError = 5; // invalid parameter (bad format)
                v_duration = strtoul(test, &test, 10);
                test++;
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
        }

        if (response && txPacket == NULL) protoError = 7;

        if (!protoError) {
            send_keyglove_packet(txPacket, txPacketLength);
            Serial.println(F("OK"));
        } else {
            Serial.print(F("ERR "));
            Serial.print(protoError);
            switch (protoError) {
                case 1:
                    Serial.println(F(" UNKNOWN COMMAND"));
                    break;
                case 2:
                    Serial.println(F(" MISSING PARAMETER(S)"));
                    break;
                case 3:
                    Serial.println(F(" PARAMETER OUT OF RANGE (SINGLE)"));
                    break;
                case 4:
                    Serial.println(F(" PARAMETER OUT OF RANGE (MULTIPLE)"));
                    break;
                case 5:
                    Serial.println(F(" INVALID PARAMETER FORMAT"));
                    break;
                case 6:
                    Serial.println(F(" OUT OF MEMORY FOR PARAMETER"));
                    break;
                case 7:
                    Serial.println(F(" OUT OF MEMORY FOR RESPONSE"));
                    break;
                case 255:
                    Serial.println(F(" NO HARDWARE/FIRMWARE SUPPORT"));
                    break;
            }
        }

        // reset buffer after processing
        rxPacketLength = 0;
        if (txPacket != NULL) free(txPacket);
    } else {
        if (!inBinPacket && inputByte == 0xB8) { // "bait" byte
            inBinPacket = true;
            rxPacketLength = 0; // reset buffer
            binDataLength = 0;
        } else if (!inBinPacket && inputByte == 0x08) { // backspace
            if (rxPacketLength > 0) {
                rxPacketLength--;
                Serial.write(inputByte); // local echo backspace
                Serial.write(' '); // overwrite w/space to clear
                Serial.write(inputByte); // backspace again
            }
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

        if ((uint8_t)(*test) == KG_PACKET_VERSION) {
            // 0x10: VERSION 
            // Get the firmware version installed on the controller board
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // respond to command if successful
            if (!protoError) {
                response = true;
                txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_VERSION);
            }
        } else if ((uint8_t)(*test) == KG_PACKET_FEATURES) {
            // 0x11: FEATURES 
            // Get enabled features and modules (such as sensors, wireless, feedback, external EEPROM, etc.)
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // respond to command if successful
            if (!protoError) {
                response = true;
                txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_FEATURES);
            }
        } else if ((uint8_t)(*test) == KG_PACKET_OPTION) {
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
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_OPTION);
                } else if (argCount == 1) {
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_OPTION, v_name);
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
        } else if ((uint8_t)(*test) == KG_PACKET_EEPROM_STATUS) {
            // 0x50: EEPROM STATUS 
            // Get current EEPROM usage status
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // respond to command if successful
            if (!protoError) {
                response = true;
                txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_EEPROM_STATUS);
            }
        } else if ((uint8_t)(*test) == KG_PACKET_EEPROM_EXPORT) {
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
                txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_EEPROM_EXPORT);
            }
        } else if ((uint8_t)(*test) == KG_PACKET_EEPROM_IMPORT) {
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
        } else if ((uint8_t)(*test) == KG_PACKET_SRAM_STATUS) {
            // 0x58: SRAM STATUS 
            // Get current SRAM usage status
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // respond to command if successful
            if (!protoError) {
                response = true;
                txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_SRAM_STATUS);
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
        } else if ((uint8_t)(*test) == KG_PACKET_BATTERY_STATUS) {
            // 0x70: BATTERY STATUS 
            // Get current status of battery
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // respond to command if successful
            if (!protoError) {
                response = true;
                txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_EEPROM_STATUS);
            }
        } else if ((uint8_t)(*test) == KG_PACKET_ACCEL_MODE) {
            // 0x80: ACCEL MODE <mode>
            // Controls the accelerometer mode.
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
                #ifdef ENABLE_ACCELEROMETER
                    if (v_mode == 0) disable_motion_accelerometer();
                    else enable_motion_accelerometer();
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_ACCEL_RAW16) {
            // 0x81: ACCEL RAW 
            // Gets last raw accelerometer X/Y/Z measurements
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // respond to command if successful
            if (!protoError) {
                #ifdef ENABLE_ACCELEROMETER
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_ACCEL_RAW16);
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_ACCEL_FILTERED16) {
            // 0x82: ACCEL FILTERED 
            // Gets last filtered accelerometer X/Y/Z measurements
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // respond to command if successful
            if (!protoError) {
                #ifdef ENABLE_ACCELEROMETER
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_ACCEL_FILTERED16);
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_GYRO_MODE) {
            // 0x83: GYRO MODE <mode>
            // Controls the gyroscope mode.
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // arg: mode (required)
            // >>>: Gyroscope mode
            uint8_t v_mode = 0;
            if (!protoError && test - rxPacket > rxPacketLength - 2) {
                argCount++;
                v_mode = *test++;
                if (!(v_mode == 0 || v_mode == 1)) protoError = 4; // invalid parameter (not accepted value)
            } else protoError = 2; // missing parameter(s)

            // respond to command if successful
            if (!protoError) {
                // v_mode: Gyroscope mode (uint8)
                #ifdef ENABLE_GYROSCOPE
                    if (v_mode == 0) disable_motion_gyroscope();
                    else enable_motion_gyroscope();
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_GYRO_RAW16) {
            // 0x84: GYRO RAW 
            // Gets last raw gyroscope X/Y/Z measurements
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // respond to command if successful
            if (!protoError) {
                #ifdef ENABLE_GYROSCOPE
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_GYRO_RAW16);
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_GYRO_FILTERED16) {
            // 0x85: GYRO FILTERED 
            // Gets last filtered gyroscope X/Y/Z measurements
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // respond to command if successful
            if (!protoError) {
                #ifdef ENABLE_GYROSCOPE
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_GYRO_FILTERED16);
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_ACCELGYRO_MODE) {
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
        } else if ((uint8_t)(*test) == KG_PACKET_ACCELGYRO_RAW16) {
            // 0x87: ACCELGYRO RAW 
            // Gets last raw accelerometer/gyroscope X/Y/Z measurements
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // respond to command if successful
            if (!protoError) {
                #ifdef ENABLE_ACCELGYRO
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_ACCELGYRO_RAW16);
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_ACCELGYRO_FILTERED16) {
            // 0x88: ACCELGYRO FILTERED 
            // Gets last filtered accelerometer/gyroscope X/Y/Z measurements
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;
            
            // respond to command if successful
            if (!protoError) {
                #ifdef ENABLE_ACCELGYRO
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_ACCELGYRO_FILTERED16);
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_MAG_MODE) {
            // 0x89: MAG MODE <mode>
            // Controls the magnetometer mode.
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // arg: mode (required)
            // >>>: Magnetometer mode
            uint8_t v_mode = 0;
            if (!protoError && test - rxPacket > rxPacketLength - 2) {
                argCount++;
                v_mode = *test++;
                if (!(v_mode == 0 || v_mode == 1)) protoError = 4; // invalid parameter (not accepted value)
            } else protoError = 2; // missing parameter(s)

            // respond to command if successful
            if (!protoError) {
                // v_mode: Magnetometer mode (uint8)
                #ifdef ENABLE_MAGNETOMETER
                    if (v_mode == 1) disable_motion_magnetometer();
                    else enable_motion_magnetometer();
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_MAG_RAW16) {
            // 0x90: MAG RAW 
            // Gets last raw magnetometer X/Y/Z measurements
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // respond to command if successful
            if (!protoError) {
                #ifdef ENABLE_MAGNETOMETER
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MAG_RAW16);
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_MAG_FILTERED16) {
            // 0x91: MAG FILTERED 
            // Gets last filtered magnetometer X/Y/Z measurements
            test++;
            protoError = 0; // valid command, no error (assumed)
            uint8_t argCount = 0;

            // respond to command if successful
            if (!protoError) {
                #ifdef ENABLE_MAGNETOMETER
                    response = true;
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_MAG_FILTERED16);
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_BT_RAW) {
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
        } else if ((uint8_t)(*test) == KG_PACKET_IMU_MODE) {
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
        } else if ((uint8_t)(*test) == KG_PACKET_IMU_ORIENTATION16) {
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
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_IMU_ORIENTATION16, 0, v_format);
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_IMU_ACCELERATION16) {
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
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_IMU_ACCELERATION16);
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_IMU_VELOCITY16) {
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
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_IMU_VELOCITY16);
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_IMU_POSITION16) {
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
                    txPacketLength = create_packet(txPacket, inBinPacket, KG_PACKET_IMU_POSITION16);
                #else
                    protoError = 255; // not supported by hardware/firmware
                #endif
            }
        } else if ((uint8_t)(*test) == KG_PACKET_GESTURE) {
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
        } else if ((uint8_t)(*test) == KG_PACKET_AUTOZERO) {
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
        } else if ((uint8_t)(*test) == KG_PACKET_BLINK) {
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
        } else if ((uint8_t)(*test) == KG_PACKET_RGB) {
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
        } else if ((uint8_t)(*test) == KG_PACKET_PIEZO) {
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
        } else if ((uint8_t)(*test) == KG_PACKET_VIBE) {
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
        } else if ((uint8_t)(*test) == KG_PACKET_RESET) {
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
                    // [1] = protocl error code set above
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
}

uint8_t send_keyglove_info(uint8_t level, char *buffer) {
    return 0;
}

uint8_t send_keyglove_info(uint8_t level, const __FlashStringHelper *buffer) {
    return 0;
}

uint8_t send_keyglove_packet(char *buffer, uint8_t length, bool autoFree) {
    if (buffer == NULL || length == 0) return 1;

    #if KG_HOSTIF & KG_HOSTIF_USB_SERIAL
        // send packet out over wired serial (USB virtual serial)
        if (interfaceUSBSerialReady) {
            Serial.write((const uint8_t *)buffer, length);   // packet data
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_USB_RAWHID
        // send packet out over wired custom HID interface (USB raw HID)
        // 64-byte packets, formatted where byte 0 is [0-64] and bytes 1-63 are data
        if (interfaceUSBRawHIDReady) {
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
        // send packet out over wireless serial (Bluetooth v2 SPP)
        if (interfaceBT2SerialReady) {
            // TODO: not yet supported
            //BTSerial.write((const uint8_t *)buffer, length);   // packet data
        }
    #endif

    #if KG_HOSTIF & KG_HOSTIF_BT2_RAWHID
        // send packet out over wireless custom HID interface (Bluetooth v2 raw HID)
        if (interfaceBT2RawHIDReady) {
            // TODO: not yet supported
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