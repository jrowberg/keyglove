// Bluegiga iWRAP interface library
// 9/11/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/iwrap

/* ============================================
iWRAP library code is placed under the MIT license
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

#include "iWRAP.h"
//#include "debug.h"

#ifdef round
    #undef round
#endif

void iWRAPAddress::parseHexAddress(const char *str) {
    if (str == NULL) return; // oops
    address[0] = strtol(str, NULL, 16);
    address[1] = strtol(str + 3, NULL, 16);
    address[2] = strtol(str + 6, NULL, 16);
    address[3] = strtol(str + 9, NULL, 16);
    address[4] = strtol(str + 12, NULL, 16);
    address[5] = strtol(str + 15, NULL, 16);
}

void iWRAPAddress::clear() {
    memset(address, 0, 6);
}

iWRAPConfig::iWRAPConfig() {
    muxModeEnabled = false; // default factory setting
    commandModeEnabled = true; // default factory setting
}

void iWRAPConfig::clear() {
    // SET BT AUTH
    authMode = 0;
    if (authPinCode != 0) free(authPinCode);
    authPinCode = 0;

    // SET BT BDADDR
    btAddress.clear();

    // SET BT CLASS
    btClass = 0;

    // SET BT IDENT
    btIdentSource = 0;
    btIdentVendorID = 0;
    btIdentProductID = 0;
    btIdentVersionMajor = 0;
    btIdentVersionMinor = 0;
    btIdentVersionRevision = 0;
    if (btIdentDescription != 0) free(btIdentDescription);
    btIdentDescription = 0;

    // SET BT LAP
    btLAPIAC = 0;

    // SET BT MTU
    btMTU = 0;

    // SET BT NAME
    if (btName != 0) free(btName);
    btName = 0;

    // SET BT PAIRCOUNT
    btMaxPairings = 0;
    
    // SET BT PAGEMODE
    btPageMode = 0;
    btPageTimeout = 0;
    btPageScanMode = 0;

    // SET BT PAIR (multiple entries)
    for (uint8_t i = 0; i < btPairCount && btPairAddress[i] != 0; i++, free(btPairAddress[i]), free(btPairKey[i]), btPairAddress[i] = 0, btPairKey[i] = 0);
    btPairCount = 0;

    // SET BT POWER
    btPowerDefault = 0;
    btPowerMaximum = 0;
    btPowerInquiry = 0;
    
    // SET BT ROLE
    btRoleMSPolicy = 0;
    btRoleLinkPolicy = 0;
    btRoleSupervisionTimeout = 0;
    
    // SET BT SNIFF
    btSniffEnabled = false;
    btSniffMax = 0;
    btSniffMin = 0;
    btSniffAttempt = 0;
    btSniffTimeout = 0;
    
    // SET BT SSP
    btSSPCapabilities = 0;
    btSSPMITMEnabled = false;;

    // ===========================
    // SET CONTROL variables
    // ===========================

    // SET CONTROL AUDIO (WT32 only)
    if (audioSCORouting != 0) free(audioSCORouting);
    audioSCORouting = 0;
    if (audioA2DPRouting != 0) free(audioA2DPRouting);
    audioA2DPRouting = 0;
    audioMultiSCOEnabled = false;
    audioEventsEnabled = false;
    
    // SET CONTROL AUTOCALL
    autocallEnabled = false;
    if (autocallTarget != 0) free(autocallTarget);
    autocallTarget = 0;
    autocallTimeout = 0;
    if (autocallProfile != 0) free(autocallProfile);
    autocallProfile = 0;
    
    // SET CONTROL BATTERY (WT32 only)
    batteryEnabled = false;;
    batteryLow = 0;
    batteryShutdown = 0;
    batteryFull = 0;
    batteryMask = 0;

    // SET CONTROL BAUD
    uartBaudRate = 0;
    uartDataBits = 0;
    uartStopBits = 0;
    uartParity = 0;
    
    // SET CONTROL BIND - multiple entries
    memset(&bindPriority, 0, 8);
    memset(&bindIOMask, 0, 16);
    memset(&bindDirection, 0, 8);
    for (uint8_t i = 0; i < bindEntries && bindCommand[i] != 0; i++, free(bindCommand[i]), bindCommand[i] = 0);
    bindEntries = 0;

    // SET CONTROL CD
    carrierDetectMask = 0;
    carrierDetectDataMode = 0;
    
    // SET CONTROL CODEC
    codec = 0;
    codecChannelMode = 0;
    codecSamplingRate = 0;
    codecPriority = 0;
    
    // SET CONTROL CONFIG
    config_block = 0;
    optional_block_1 = 0;
    optional_block_2 = 0;
    
    // SET CONTROL ECHO
    echoMask = 0;
    
    // SET CONTROL ESCAPE
    escapeCharacter = 0;
    escapeDTRMask = 0;
    escapeDTRMode = 0;
    
    // SET CONTROL GAIN
    gainDefaultInput = 0;
    gainDefaultOutput = 0;
    
    // SET CONTROL INIT
    if (initCommand != 0) free(initCommand);
    initCommand = 0;

    // SET CONTROL MICBIAS
    micBiasVoltage = 0;
    micBiasCurrent = 0;
    
    // SET CONTROL MUX
    //muxModeEnabled = false;
    //commandModeEnabled = true; // default reset condition
    // DON'T CHANGE THESE IN RESET, ONLY IN CONSTRUCTOR BECAUSE THEY PERSIST
    // DESPITE REBOOTING, AND CAN WREAK HAVOC WITH THE ORDER OF EVENTS NEEDED
    // TO TRY AUTO-DETECTING WHICH MODE IS CURRENTLY ACTIVE

    // SET CONTROL MSC
    mscMode = 0;
    mscDSRMask = 0;
    mscDTRMask = 0;
    mscRTSMask = 0;
    mscCTSMask = 0;
    mscRIMask = 0;
    mscDCDMask = 0;

    // SET CONTROL PCM
    pcmConfig = 0;
    pcmData = 0;
    
    // SET CONTROL PREAMP
    preamp20LeftEnabled = false;
    preamp20RightEnabled = false;
    
    // SET CONTROL RINGTONE
    if (ringtone != 0) free(ringtone);
    ringtone = 0;
    
    // SET CONTROL READY
    readyMask = 0;
    
    // SET CONTROL VREGEN (WT32 only)
    vRegMode = 0;
    vRegMask = 0;

    // ===========================
    // SET PROFILE variables
    // ===========================
    profileSPPEnabled = false;
    profileHFPEnabled = false;
    profileHFPAGEnabled = false;
    profileOPPEnabled = false;
    profileA2DPSinkEnabled = false;
    profileA2DPSourceEnabled = false;
    profileAVRCPControllerEnabled = false;
    profileAVRCPTargetEnabled = false;
    profileHIDEnabled = false;
    profileHSPEnabled = false;
    profileHSPAGEnabled = false;
    profileHDPSinkEnabled = false;
    profileHDPSourceEnabled = false;
    profilePBAPEnabled = false;
    profileBGIOEnabled = false;
    profileOTAEnabled = false;
    profileHDPSinkMDEP = 0;
    profileHDPSourceMDEP = 0;
    if (profileSPPName != 0) free(profileSPPName);
    profileSPPName = 0;
    if (profileHFPName != 0) free(profileHFPName);
    profileHFPName = 0;
    if (profileHFPAGName != 0) free(profileHFPAGName);
    profileHFPAGName = 0;
    if (profileHIDName != 0) free(profileHIDName);
    profileHIDName = 0;
}

iWRAP::iWRAP(HardwareSerial *module, HardwareSerial *output) {
    uModule = module;
    uOutput = output;
    echoModuleOutput = false;
    buffer = (char *)malloc(32);
    bufferSize = 32;
    bufferPos = 0;
    busy = false;
    lines = 0;
    lastCommand = 0;
    lastError = false;
    firstDataRead = false;
    muxBlockOffset = 0;
    responseLength = 0;
    responseEnded = false;

    keyboard_keys[0] = 0;
    keyboard_keys[1] = 0;
    keyboard_keys[2] = 0;
    keyboard_keys[3] = 0;
    keyboard_keys[4] = 0;
    keyboard_keys[5] = 0;
    keyboard_modifier_keys = 0;
    mouse_buttons = 0;
}

bool iWRAP::checkActivity(uint16_t timeout) {
    while (uModule -> available() && (timeout == 0 || millis() - timeoutStart < timeout)) {
        parse(uModule -> read());
        if (timeout > 0) timeoutStart = millis();
    }
    if (timeout > 0 && busy && millis() - timeoutStart >= timeout) {
        lastTimeout = true;
        setBusy(false);
    }
    return busy;
}

bool iWRAP::checkError() {
    return lastError;
}
bool iWRAP::checkTimeout() {
    return lastTimeout;
}

void iWRAP::setBusy(bool busyEnabled) {
    busy = busyEnabled;
    if (busy) {
        timeoutStart = millis();
        lastTimeout = false;
        if (funcBusy != 0) funcBusy();
    } else {
        lastError = false;
        if (funcIdle != 0) funcIdle();
    }
}

// set/update UART port objects
void iWRAP::setModuleUART(HardwareSerial *module) {
    uModule = module;
}

void iWRAP::setOutputUART(HardwareSerial *output) {
    uOutput = output;
}

// control module-to-output port echoing
void iWRAP::setEchoModuleOutput(bool enabled) {
    echoModuleOutput = enabled;
}

void iWRAP::parse(uint8_t ch) {
    // echo to output UART if enabled
    if (echoModuleOutput && uOutput) uOutput -> write(ch);

    // automatically detect MUX mode if we need to
    if (!firstDataRead && !config.muxModeEnabled && bufferPos > 1 && (buffer[bufferPos - 2] & 0xFF) == 0xBF && (ch & 0xFC) == 0x00) {
        config.muxModeEnabled = true;
        firstDataRead = true;
        muxBlockOffset = bufferPos - 2;
    }

    // test for end of line or end of MUX sequence
    uint8_t linkID = 0;
    if (config.muxModeEnabled) {
        /*uOutput -> print("-> ");
        uOutput -> print((uint8_t)ch, HEX);
        uOutput -> print(", bufferPos=");
        uOutput -> print(bufferPos);
        uOutput -> print(", mBO=");
        uOutput -> println(muxBlockOffset);*/
        if ((bufferPos - muxBlockOffset > 4) &&        // at least 5 bytes received in block
            (((buffer[muxBlockOffset + 2] & 0x3) << 8) + buffer[muxBlockOffset + 3] == bufferPos - muxBlockOffset - 4) &&
                                                       // real content length matches intended length
            ch == buffer[muxBlockOffset + 1] ^ 0xFF) { // last byte = Link ID XOR 0xFF

            responseEnded = true;
            linkID = buffer[muxBlockOffset + 1];
            responseLength = bufferPos - muxBlockOffset - 5;
            buffer[bufferPos] = 0; // null terminate
            /*uOutput -> print("[BF][");
            uOutput -> print((uint8_t)buffer[muxBlockOffset + 1], HEX);
            uOutput -> print("][");
            uOutput -> print((uint8_t)buffer[muxBlockOffset + 2], HEX);
            uOutput -> print(" ");
            uOutput -> print((uint8_t)buffer[muxBlockOffset + 3], HEX);
            uOutput -> print("][");
            uOutput -> print(buffer + 4 + muxBlockOffset);
            uOutput -> print("][");
            uOutput -> print((uint8_t)ch, HEX);
            uOutput -> println("]");*/
        }
    } else {
        if (ch == '\n' || ch == '\r') {
            responseEnded = true;
            buffer[bufferPos] = 0; // null terminate
            responseLength = bufferPos;
            uOutput -> print("END OF SIMPLE DATA, LENGTH=");
            uOutput -> println(responseLength);
            uOutput -> print("$");
            uOutput -> print(buffer);
            uOutput -> println("$");
        }
    }

    // take appropriate action
    if (!responseEnded) {
        // mid-line, so append to buffer
        buffer[bufferPos++] = ch;
        if (bufferPos + 1 == bufferSize) {
            bufferSize += 32;
            char *tmp = (char *)realloc(buffer, bufferSize);
            if (tmp) buffer = tmp; // safe transfer only if realloc() succeeds
        }
    } else if (responseLength) {
        if ((config.muxModeEnabled && linkID == 0xFF) || config.commandModeEnabled) {
            // end of line/MUX sequence, so parse for events/responses
            if (!busy || lines > 0) {
                char *test = (char *)buffer;
                if (config.muxModeEnabled) test += 4 + muxBlockOffset; // move string pointer to beginning of data
                // ======== SET RESPONSES ========
                if (strncmp(test, "SET ", 4) == 0) {
                    //uOutput -> print(" <-- SET RESPONSE");
                    char *p;
                    if (busy && lastCommand == IWRAP_COMMAND_SET) {
                        if (strncmp(test + 4, "BT ", 3) == 0) {
                            //uOutput -> print(" (BT)");
                            if (strncmp(test + 7, "AUTH ", 5) == 0) {
                                // SET BT AUTH
                                //uint8_t authMode;
                                //char *authPinCode;
                                config.authMode = test[12];
                                if (config.authPinCode != 0) free(config.authPinCode);
                                config.authPinCode = 0;
                                uint8_t len = responseLength - 14; // length of remaining string
                                if (len > 0 && (config.authPinCode = (char *)malloc(len + 1))) {
                                    strncpy(config.authPinCode, test + 14, len);
                                    config.authPinCode[len] = 0;
                                }
                            } else if (strncmp(test + 7, "BDADDR ", 7) == 0) {
                                // SET BT BDADDR
                                //iWRAPAddress btAddress;
                                config.btAddress.parseHexAddress(test + 14);
                            } else if (strncmp(test + 7, "CLASS ", 6) == 0) {
                                // SET BT CLASS
                                //uint16_t btClass;
                                config.btClass = strtol(test + 13, NULL, 16);
                            } else if (strncmp(test + 7, "IDENT ", 6) == 0) {
                                // SET BT IDENT
                                //uint8_t btIdentSource;
                                //uint8_t btIdentVendorID;
                                //uint8_t btIdentProductID;
                                //uint8_t btIdentVersionMajor;
                                //uint8_t btIdentVersionMinor;
                                //uint8_t btIdentVersionRevision;
                                //char *btIdentDescription;
                                if (strncmp(test + 14, "BT", 2) == 0) {
                                    config.btIdentSource = 1; // BT
                                    p = test + 17;
                                } else {
                                    config.btIdentSource = 2; // USB
                                    p = test + 18;
                                }
                                config.btIdentVendorID = strtol(p, &p, 10); // should be 47 for Bluegiga
                                config.btIdentProductID = strtol(p, &p, 16); // should be 0xf000 for WT12
                                config.btIdentVersionMajor = strtol(p, NULL, 10); // should match iWRAP version
                                p += (config.btIdentVersionMajor > 9) ? 3 : 2; // jump decimal point
                                config.btIdentVersionMinor = strtol(p, NULL, 10);
                                p += (config.btIdentVersionMinor > 9) ? 3 : 2; // jump decimal point
                                config.btIdentVersionRevision = strtol(p, &p, 10);
                                p += (config.btIdentVersionRevision > 9) ? 3 : 2; // jump space
                                if (config.btIdentDescription != 0) free(config.btIdentDescription);
                                config.btIdentDescription = 0;
                                uint8_t len = responseLength - (p - test - 1); // length of remaining string
                                if (len > 0 && (config.btIdentDescription = (char *)malloc(len + 1))) {
                                    strncpy(config.btIdentDescription, p + 1, len);
                                    config.btIdentDescription[len] = 0;
                                }
                            } else if (strncmp(test + 7, "LAP ", 4) == 0) {
                                // SET BT LAP
                                //uint32_t btLAPIAC;
                                config.btLAPIAC = strtol(test + 11, NULL, 16);
                            } else if (strncmp(test + 7, "MTU ", 4) == 0) {
                                // SET BT MTU
                                uint16_t btMTU;
                                config.btMTU = strtol(test + 11, NULL, 16);
                            } else if (strncmp(test + 7, "NAME ", 5) == 0) {
                                // SET BT NAME
                                //char *btName; // max length = 256
                                if (config.btName != 0) free(config.btName);
                                config.btName = 0;
                                uint8_t len = responseLength - 12; // length of remaining string
                                if (len > 0 && (config.btName = (char *)malloc(len + 1))) {
                                    strncpy(config.btName, test + 12, len);
                                    config.btName[len] = 0;
                                }
                            } else if (strncmp(test + 7, "PAIRCOUNT ", 10) == 0) {
                                // SET BT PAIRCOUNT
                                //uint8_t btMaxPairings;
                                config.btMaxPairings = strtol(test + 17, NULL, 10);
                            } else if (strncmp(test + 7, "PAGEMODE ", 9) == 0) {
                                // SET BT PAGEMODE
                                //uint8_t btPageMode;
                                //uint16_t btPageTimeout;
                                //uint8_t btPageScanMode;
                                config.btPageMode = strtol(test + 16, &p, 10);
                                config.btPageTimeout = strtol(p, &p, 16);
                                config.btPageScanMode = strtol(p, NULL, 10);
                            } else if (strncmp(test + 7, "PAIR ", 5) == 0) {
                                // SET BT PAIR (multiple entries)
                                //uint8_t btPairCount;
                                //char *btPairAddress[16]; // 6 bytes each
                                //char *btPairKey[16];     // 32 bytes each
                                if (config.btPairCount < 16) {
                                    if (config.btPairAddress[config.btPairCount] != 0) free(config.btPairAddress[config.btPairCount]);
                                    config.btPairAddress[config.btPairCount] = 0;
                                    if (config.btPairKey[config.btPairCount] != 0) free(config.btPairKey[config.btPairCount]);
                                    config.btPairKey[config.btPairCount] = 0;
                                    if ((config.btPairAddress[config.btPairCount] =
                                                (iWRAPAddress *)malloc(sizeof(iWRAPAddress)))) {
                                        config.btPairAddress[config.btPairCount] -> parseHexAddress(test + 12);
                                    }
                                    if ((config.btPairKey[config.btPairCount] = (char *)malloc(33))) {
                                        strncpy(config.btPairKey[config.btPairCount], test + 30, 32);
                                        config.btPairKey[config.btPairCount][32] = 0;
                                    }
                                    config.btPairCount++;
                                }
                            } else if (strncmp(test + 7, "POWER ", 6) == 0) {
                                // SET BT POWER
                                //uint8_t btPowerDefault;
                                //uint8_t btPowerMaximum;
                                //uint8_t btPowerInquiry;
                                config.btPowerDefault = strtol(test + 13, &p, 10);
                                config.btPowerMaximum = strtol(p, &p, 10);
                                config.btPowerInquiry = strtol(p, NULL, 10);
                            } else if (strncmp(test + 7, "ROLE ", 5) == 0) {
                                // SET BT ROLE
                                //uint8_t btRoleMSPolicy;
                                //uint8_t btRoleLinkPolicy;
                                //uint16_t btRoleSupervisionTimeout;
                                config.btRoleMSPolicy = strtol(test + 12, &p, 16);
                                config.btRoleLinkPolicy = strtol(p, &p, 16);
                                config.btRoleSupervisionTimeout = strtol(p, NULL, 16);
                            } else if (strncmp(test + 7, "SNIFF ", 6) == 0) {
                                // SET BT SNIFF
                                //bool btSniffEnabled;
                                //uint16_t btSniffMax;
                                //uint16_t btSniffMin;
                                //uint16_t btSniffAttempt;
                                //uint16_t btSniffTimeout;
                                config.btSniffMax = strtol(test + 13, &p, 16);
                                config.btSniffMin = strtol(p, &p, 16);
                                config.btSniffAttempt = strtol(p, &p, 16);
                                config.btSniffTimeout = strtol(p, NULL, 16);
                                config.btSniffEnabled = config.btSniffMax > 0;
                            } else if (strncmp(test + 7, "SSP ", 4) == 0) {
                                // SET BT SSP
                                //uint8_t btSSPCapabilities;
                                //bool btSSPMITMEnabled;
                                config.btSSPCapabilities = strtol(test + 11, &p, 16);
                                config.btSSPMITMEnabled = strtol(p, NULL, 16) > 0;
                            }
                        } else if (strncmp(test + 4, "CONTROL ", 8) == 0) {
                            //uOutput -> print(" (CONTROL)");
                            if (strncmp(test + 12, "AUDIO ", 6) == 0) {
                                // SET CONTROL AUDIO (WT32 only)
                                //char *audioSCORouting;
                                //char *audioA2DPRouting;
                                //bool audioMultiSCOEnabled;
                                //bool audioEventsEnabled;
                                if (config.audioSCORouting != 0) free(config.audioSCORouting);
                                config.audioSCORouting = 0;
                                if (config.audioA2DPRouting != 0) free(config.audioA2DPRouting);
                                config.audioA2DPRouting = 0;
                                config.audioMultiSCOEnabled = false;
                                config.audioEventsEnabled = false;
                                p = test + 18; // jump to beginning of SCO value
                                uint8_t len = strchr(p, ' ') - p;
                                if (len > 0 && (config.audioSCORouting = (char *)malloc(len + 1))) {
                                    strncpy(config.audioSCORouting, p, len);
                                    config.audioSCORouting[len] = 0;
                                }    
                                p += len + 1; // jump to beginning of A2DP value
                                len = strchr(p, ' ') - p;
                                if (len > 0 && (config.audioA2DPRouting = (char *)malloc(len + 1))) {
                                    strncpy(config.audioA2DPRouting, p, len);
                                    config.audioA2DPRouting[len] = 0;
                                }
                                p += len + 1; // jump to beginning of first optional MULTISCO/EVENT flag
                                if (p < test + responseLength) {
                                    len = strchr(p, ' ') - p;
                                    if (p[0] == 'M') config.audioMultiSCOEnabled = true;
                                    else if (p[0] == 'E') config.audioEventsEnabled = true;
                                    p += len + 1; // jump to beginning of second optional MULTISCO/EVENT flag
                                    if (p < test + responseLength) {
                                        if (p[0] == 'M') config.audioMultiSCOEnabled = true;
                                        else if (p[0] == 'E') config.audioEventsEnabled = true;
                                    }
                                }
                            } else if (strncmp(test + 12, "AUTOCALL ", 9) == 0) {
                                // SET CONTROL AUTOCALL
                                //bool autocallEnabled;
                                //char *autocallTarget;
                                //uint16_t autocallTimeout;
                                //char *autocallProfile;
                                if (config.autocallTarget != 0) free(config.autocallTarget);
                                config.autocallTarget = 0;
                                if (config.autocallProfile != 0) free(config.autocallProfile);
                                config.autocallProfile = 0;
                                config.autocallEnabled = true;
                                p = test + 21; // jump to beginning of {target} value
                                uint8_t len = strchr(p, ' ') - p;
                                if (len > 0 && (config.autocallTarget = (char *)malloc(len + 1))) {
                                    strncpy(config.autocallTarget, p, len);
                                    config.autocallTarget[len] = 0;
                                }
                                p += len + 1; // jump to beginning of {timeout} value
                                len = strchr(p, ' ') - p;
                                config.autocallTimeout = strtol(p, NULL, 10);
                                p += len + 1; // jump to beginning of {profile} value
                                if (len > 0 && (config.autocallProfile = (char *)malloc(len + 1))) {
                                    strncpy(config.autocallProfile, p, len);
                                    config.autocallProfile[len] = 0;
                                }
                            } else if (strncmp(test + 12, "BATTERY ", 8) == 0) {
                                // SET CONTROL BATTERY (WT32 only)
                                //bool batteryEnabled;
                                //uint16_t batteryLow;
                                //uint16_t batteryShutdown;
                                //uint16_t batteryFull;
                                //uint16_t batteryMask;
                                config.batteryEnabled = true;
                                config.batteryLow = strtol(test + 20, &p, 10);
                                config.batteryShutdown = strtol(p, &p, 10);
                                config.batteryFull = strtol(p, &p, 10);
                                config.batteryMask = strtol(p, NULL, 16);
                            } else if (strncmp(test + 12, "BAUD ", 5) == 0) {
                                // SET CONTROL BAUD
                                //uint32_t uartBaudRate;
                                //uint8_t uartDataBits;
                                //uint8_t uartStopBits;
                                //uint8_t uartParity;
                                uint8_t commaPos = strchr(test, ',') - test;
                                test[commaPos] = 0; // false string termination for strtol() parsing
                                config.uartBaudRate = strtol(test + 17, NULL, 10);
                                config.uartDataBits = test[commaPos + 1] - 0x30; // ASCII '0'
                                config.uartParity = test[commaPos + 2]; // n/e/o = none/even/odd
                                config.uartStopBits = test[commaPos + 3] - 0x30; // ASCII '0'
                            } else if (strncmp(test + 12, "BIND ", 5) == 0) {
                                // SET CONTROL BIND - multiple entries
                                //uint8_t bindEntries;
                                //uint8_t bindPriority[8];
                                //uint16_t bindIOMask[8];
                                //uint8_t bindDirection[8];
                                //char *bindCommand[8]; // max length = 31
                                if (config.bindEntries < 16) {
                                    if (config.bindCommand[config.bindEntries] != 0) free(config.bindCommand[config.bindEntries]);
                                    config.bindCommand[config.bindEntries] = 0;
                                    config.bindPriority[config.bindEntries] = strtol(test + 17, &p, 16);
                                    config.bindIOMask[config.bindEntries] = strtol(p, &p, 16);
                                    if (p[1] == 'R') {
                                        config.bindDirection[config.bindEntries] = IWRAP_BIND_DIRECTION_RISE; // RISE
                                        p += 6;
                                    } else if (p[1] == 'F') {
                                        config.bindDirection[config.bindEntries] = IWRAP_BIND_DIRECTION_FALL; // FALL
                                        p += 6;
                                    } else if (p[1] == 'C') {
                                        config.bindDirection[config.bindEntries] = IWRAP_BIND_DIRECTION_CHANGE; // CHANGE
                                        p += 8;
                                    }
                                    uint8_t len = test + responseLength - p;
                                    if ((config.bindCommand[config.bindEntries] = (char *)malloc(len + 1))) {
                                        strncpy(config.bindCommand[config.bindEntries], p, len);
                                        config.bindCommand[config.bindEntries][len] = 0;
                                    }
                                    config.btPairCount++;
                                }
                            } else if (strncmp(test + 12, "CD ", 3) == 0) {
                                // SET CONTROL CD
                                //uint16_t carrierDetectMask;
                                //uint8_t carrierDetectDataMode;
                                config.carrierDetectMask = strtol(test + 15, &p, 16);
                                config.carrierDetectDataMode = strtol(p, NULL, 10);
                            } else if (strncmp(test + 12, "CODEC ", 6) == 0) {
                                // SET CONTROL CODEC
                                //uint8_t codec;
                                //uint8_t codecChannelMode;
                                //uint16_t codecSamplingRate;
                                //uint8_t codecPriority;
                                // TODO: iWRAP docs are confusing here; is {codec} present in list or not?
                                //p = strchr(test + 18, ' ') + 1; // if {codec} is present (syntax indicates)
                                p = test + 18; // if {codec} is not present (example indicates)
                                if (p[0] == 'J') { // JOINT_STEREO
                                    config.codecChannelMode = IWRAP_CODEC_CHANNEL_JOINT_STEREO;
                                } else if (p[0] == 'S') { // STEREO
                                    config.codecChannelMode = IWRAP_CODEC_CHANNEL_STEREO;
                                } else if (p[0] == 'D') { // DUAL_CHANNEL
                                    config.codecChannelMode = IWRAP_CODEC_CHANNEL_DUAL_CHANNEL;
                                } else if (p[0] == 'M') { // MONO
                                    config.codecChannelMode = IWRAP_CODEC_CHANNEL_MONO;
                                }
                                p = strchr(p, ' ');
                                config.codecSamplingRate = strtol(p, &p, 10);
                                config.codecPriority = strtol(p, NULL, 10);
                            } else if (strncmp(test + 12, "CONFIG ", 7) == 0) {
                                // SET CONTROL CONFIG
                                //uint16_t config_block;
                                //uint16_t optional_block_1;
                                //uint16_t optional_block_2;
                                p = test + 19;
                                config.optional_block_2 = strtol(p, &p, 16);
                                config.optional_block_1 = strtol(p, &p, 16);
                                config.config_block = strtol(p, NULL, 16);
                            } else if (strncmp(test + 12, "ECHO ", 5) == 0) {
                                // SET CONTROL ECHO
                                //uint8_t echoMask;
                                config.echoMask = strtol(test + 17, NULL, 16);
                            } else if (strncmp(test + 12, "ESCAPE ", 7) == 0) {
                                // SET CONTROL ESCAPE
                                //uint8_t escapeCharacter;
                                //uint16_t escapeDTRMask;
                                //uint8_t escapeDTRMode;
                                config.escapeCharacter = test[19];
                                config.escapeDTRMask = strtol(test + 21, &p, 16);
                                config.escapeDTRMode = strtol(p, NULL, 16);
                            } else if (strncmp(test + 12, "GAIN ", 5) == 0) {
                                // SET CONTROL GAIN
                                //uint8_t gainDefaultInput;
                                //uint8_t gainDefaultOutput;
                                config.gainDefaultInput = strtol(test + 17, &p, 16);
                                config.gainDefaultOutput = strtol(p, NULL, 16);
                            } else if (strncmp(test + 12, "INIT ", 5) == 0) {
                                // SET CONTROL INIT
                                //char *initCommand;
                                if (config.initCommand != 0) free(config.initCommand);
                                config.initCommand = 0;
                                uint8_t len = responseLength - 17;
                                if (len > 0 && (config.initCommand = (char *)malloc(len + 1))) {
                                    strncpy(config.initCommand, test + 17, len);
                                    config.initCommand[len] = 0;
                                }    
                            } else if (strncmp(test + 12, "MICBIAS ", 8) == 0) {
                                // SET CONTROL MICBIAS
                                //uint8_t micBiasVoltage;
                                //uint8_t micBiasCurrent;
                                config.micBiasVoltage = strtol(test + 20, &p, 16);
                                config.micBiasCurrent = strtol(p, NULL, 16);
                            } else if (strncmp(test + 12, "MUX ", 4) == 0) {
                                // SET CONTROL MUX
                                //bool muxModeEnabled;
                                config.muxModeEnabled = test[16] == '1';
                            } else if (strncmp(test + 12, "MSC ", 4) == 0) {
                                // SET CONTROL MSC
                                //uint8_t mscMode;
                                //uint16_t mscDSRMask;
                                //uint16_t mscDTRMask;
                                //uint16_t mscRTSMask;
                                //uint16_t mscCTSMask;
                                //uint16_t mscRIMask;
                                //uint16_t mscDCDMask;
                                // TODO: add this functionality
                            } else if (strncmp(test + 12, "PCM ", 4) == 0) {
                                // SET CONTROL PCM
                                //uint32_t pcmConfig;
                                //uint16_t pcmData;
                                config.pcmConfig = strtol(test + 16, &p, 16);
                                config.pcmData = strtol(p, NULL, 16);
                            } else if (strncmp(test + 12, "PREAMP ", 7) == 0) {
                                // SET CONTROL PREAMP
                                //bool preamp20LeftEnabled;
                                //bool preamp20RightEnabled;
                                config.preamp20LeftEnabled = test[19] == '1';
                                config.preamp20RightEnabled = test[21] == '1';
                            } else if (strncmp(test + 12, "RINGTONE ", 9) == 0) {
                                // SET CONTROL RINGTONE
                                //char *ringtone;
                                if (config.ringtone != 0) free(config.ringtone);
                                config.ringtone = 0;
                                uint8_t len = responseLength - 21;
                                if (len > 0 && (config.ringtone = (char *)malloc(len + 1))) {
                                    strncpy(config.ringtone, test + 21, len);
                                    config.ringtone[len] = 0;
                                }
                            } else if (strncmp(test + 12, "READY ", 6) == 0) {
                                // SET CONTROL READY
                                //uint16_t readyMask;
                                config.readyMask = strtol(test + 18, NULL, 16);
                            } else if (strncmp(test + 12, "VREGEN ", 7) == 0) {
                                // SET CONTROL VREGEN (WT32 only)
                                //uint8_t vRegMode;
                                //uint16_t vRegMask;
                                config.vRegMode = strtol(test + 19, &p, 16);
                                config.vRegMask = strtol(p, NULL, 16);
                            }
                        } else if (strncmp(test + 4, "PROFILE ", 8) == 0) {
                            //uOutput -> print(" (PROFILE)");
                            // SET PROFILE variables
                            //bool profileSPPEnabled;
                            //bool profileHFPEnabled;
                            //bool profileHFPAGEnabled;
                            //bool profileOPPEnabled;
                            //bool profileA2DPSinkEnabled;
                            //bool profileA2DPSourceEnabled;
                            //bool profileAVRCPControllerEnabled;
                            //bool profileAVRCPTargetEnabled;
                            //bool profileHIDEnabled;
                            //bool profileHSPEnabled;
                            //bool profileHSPAGEnabled;
                            //bool profileHDPSinkEnabled;
                            //bool profileHDPSourceEnabled;
                            //bool profilePBAPEnabled;
                            //bool profileBGIOEnabled;
                            //bool profileOTAEnabled;
                            //uint16_t profileHDPSinkMDEP;
                            //uint16_t profileHDPSourceMDEP;
                            //char *profileSPPName; // names may be up to 49 characters
                            //char *profileHFPName;
                            //char *profileHFPAGName;
                            //char *profileHIDName;
                            if        (strncmp(test + 12, "SPP", 3) == 0) {
                                config.profileSPPEnabled = true;
                                if (config.profileSPPName != 0) free(config.profileSPPName);
                                config.profileSPPName = 0;
                                uint8_t len = responseLength - 16;
                                if (len > 0 && (config.profileSPPName = (char *)malloc(len + 1))) {
                                    strncpy(config.profileSPPName, test + 16, len);
                                    config.profileSPPName[len] = 0;
                                }
                            } else if (strncmp(test + 12, "OPP", 3) == 0) {
                                config.profileOPPEnabled = true;
                            } else if (strncmp(test + 12, "HFP-AG", 6) == 0) {
                                config.profileHFPAGEnabled = true;
                                if (config.profileHFPAGName != 0) free(config.profileHFPAGName);
                                config.profileHFPAGName = 0;
                                uint8_t len = responseLength - 19;
                                if (len > 0 && (config.profileHFPAGName = (char *)malloc(len + 1))) {
                                    strncpy(config.profileHFPAGName, test + 19, len);
                                    config.profileHFPAGName[len] = 0;
                                }
                            } else if (strncmp(test + 12, "HFP", 3) == 0) {
                                config.profileHFPEnabled = true;
                                if (config.profileHFPName != 0) free(config.profileHFPName);
                                config.profileHFPName = 0;
                                uint8_t len = responseLength - 16;
                                if (len > 0 && (config.profileHFPName = (char *)malloc(len + 1))) {
                                    strncpy(config.profileHFPName, test + 16, len);
                                    config.profileHFPName[len] = 0;
                                }
                            } else if (strncmp(test + 12, "A2DP SI", 7) == 0) { // A2DP SINK
                                config.profileA2DPSinkEnabled = true;
                            } else if (strncmp(test + 12, "A2DP SO", 7) == 0) { // A2DP SOURCE
                                config.profileA2DPSourceEnabled = true;
                            } else if (strncmp(test + 12, "AVRCP C", 7) == 0) { // AVRCP CONTROLLER
                                config.profileAVRCPControllerEnabled = true;
                            } else if (strncmp(test + 12, "AVRCP T", 7) == 0) { // AVRCP TARGET
                                config.profileAVRCPTargetEnabled = true;
                            } else if (strncmp(test + 12, "HID", 3) == 0) {
                                config.profileHIDEnabled = true;
                                if (config.profileHIDName != 0) free(config.profileHIDName);
                                config.profileHIDName = 0;
                                uint8_t len = responseLength - 16;
                                if (len > 0 && (config.profileHIDName = (char *)malloc(len + 1))) {
                                    strncpy(config.profileHIDName, test + 16, len);
                                    config.profileHIDName[len] = 0;
                                }
                            } else if (strncmp(test + 12, "HSP-AG", 6) == 0) {
                                config.profileHSPAGEnabled = true;
                            } else if (strncmp(test + 12, "HSP", 3) == 0) {
                                config.profileHSPEnabled = true;
                            } else if (strncmp(test + 12, "HDP SI", 6) == 0) { // HDP SINK
                                config.profileHDPSinkEnabled = true;
                            } else if (strncmp(test + 12, "HDP SO", 6) == 0) { // HDP SOURCE
                                config.profileHDPSourceEnabled = true;
                            } else if (strncmp(test + 12, "PBAP", 4) == 0) {
                                config.profilePBAPEnabled = true;
                            } else if (strncmp(test + 12, "BGIO", 4) == 0) {
                                config.profileBGIOEnabled = true;
                            } else if (strncmp(test + 12, "OTA", 3) == 0) {
                                config.profileOTAEnabled = true;
                            } else {
                                // TODO: user profile support (not really necessary?)
                            }
                        }
                    }
                } else if (strncmp(test, "SET", 3) == 0) {
                    //uOutput -> print(" <-- SET FINISHED");
                    setBusy(false);
    
                // ======== EVENTS ========
                } else if (strncmp(test, "AUTH ", 5) == 0) {
                    //uOutput -> print(" <-- AUTH event found!");
                    if (funcAuth != 0) funcAuth();
                } else if (strncmp(test, "BATTERY F", 9) == 0) {
                    //uOutput -> print(" <-- BATTERY FULL event found!");
                    if (funcBatteryFull != 0) funcBatteryFull();
                } else if (strncmp(test, "BATTERY L", 9) == 0) {
                    //uOutput -> print(" <-- BATTERY LOW event found!");
                    if (funcBatteryLow != 0) funcBatteryLow();
                } else if (strncmp(test, "BATTERY S", 9) == 0) {
                    //uOutput -> print(" <-- BATTERY SHUTDOWN event found!");
                    if (funcBatteryShutdown != 0) funcBatteryShutdown();
                } else if (strncmp(test, "CONNECT ", 8) == 0) {
                    //uOutput -> print(" <-- CONNECT event found!");
                    if (funcConnect != 0) funcConnect();
                } else if (strncmp(test, "CONNAUTH ", 9) == 0) {
                    //uOutput -> print(" <-- CONNAUTH event found!");
                    if (funcConnAuth != 0) funcConnAuth();
                } else if (strncmp(test, "CLOCK ", 6) == 0) {
                    //uOutput -> print(" <-- CLOCK event found!");
                    if (funcClock != 0) funcClock();
                } else if (strncmp(test, "IDENT ERROR ", 12) == 0) {
                    //uOutput -> print(" <-- IDENT ERROR event found!");
                    if (funcIdentError != 0) funcIdentError();
                } else if (strncmp(test, "IDENT ", 6) == 0) {
                    //uOutput -> print(" <-- IDENT event found!");
                    if (funcIdent != 0) funcIdent();
                } else if (strncmp(test, "INQUIRY_PARTIAL ", 16) == 0) {
                    //uOutput -> print(" <-- INQUIRY_PARTIAL event found!");
                    if (funcInquiryPartial != 0) funcInquiryPartial();
                } else if (strncmp(test, "NO CARRIER ", 11) == 0) {
                    //uOutput -> print(" <-- NO CARRIER event found!");
                    if (funcNoCarrier != 0) funcNoCarrier();
                } else if (strncmp(test, "NAME ERROR ", 11) == 0) {
                    //uOutput -> print(" <-- NAME ERROR event found!");
                    if (funcNameError != 0) funcNameError();
                } else if (strncmp(test, "NAME ", 5) == 0) {
                    //uOutput -> print(" <-- NAME event found!");
                    if (funcName != 0) funcName();
                } else if (strncmp(test, "OBEX AUTH ", 10) == 0) {
                    //uOutput -> print(" <-- OBEX AUTH event found!");
                    if (funcOBEXAuth != 0) funcOBEXAuth();
                } else if (strncmp(test, "PAIR ", 5) == 0) {
                    //uOutput -> print(" <-- PAIR event found!");
                    if (funcPair != 0) funcPair();
                } else if (strncmp(test, "READY", 5) == 0) {
                    //uOutput -> print(" <-- READY event found!");
                    setBusy(false);
                    if (funcReady != 0) funcReady();
                } else if (strncmp(test, "RING ", 5) == 0) {
                    //uOutput -> print(" <-- RING event found!");
                    if (funcRing != 0) funcRing();
                } else if (strncmp(test, "SYNTAX ", 7) == 0) {
                    //uOutput -> print(" <-- SYNTAX ERROR event found!");
                    setBusy(false);
                    lastError = true;
                }
            } else {
                // first line of a command, so we can ignore parsing and deal only
                // with the busy flag, which might or might not need to be cleared
                // simply after this one line, since some commands don't generate a
                // response.
                switch (lastCommand) {
                    case IWRAP_COMMAND_SET:
                    case IWRAP_COMMAND_RESET:
                        break; // these commands generate a response parsed above

                    case IWRAP_COMMAND_SET_CONTROL_MUX_EN:
                        config.muxModeEnabled = true;
                        setBusy(false);
                        break;
                    case IWRAP_COMMAND_SET_CONTROL_MUX_DIS:
                        config.muxModeEnabled = false;
                        setBusy(false);
                        break;

                    default:
                        setBusy(false); // all other commands, stop paying attention
                }
            }
        }
        lines++;
        firstDataRead = true;
        memset(buffer, 0, bufferPos); // clean out this line/block
        bufferPos = 0;
        muxBlockOffset = 0;
        responseEnded = false;
        responseLength = 0;
    }
}

// read all settings into local data structures
void iWRAP::readDeviceConfig() {
    config.clear();
    lastCommand = IWRAP_COMMAND_SET;
    smartSend("SET");
}

void iWRAP::readLinkConfig() {
    listConnections();
}

// get/set device mode
uint8_t iWRAP::getDeviceMode() {
    return 0;
}
void iWRAP::setDeviceMode(uint8_t mode) {}

// send plain command or MUX-mode protocol if necessary
void iWRAP::smartSend(const char *data, iWRAPLink *link) {
    if (config.muxModeEnabled) {
        // MUX mode
        // [8:SOF] [8:LINK] [6:FLAGS] [10:LENGTH] [...DATA...] [8:nLINK]
        // SOF = 0xBF
        // LINK = 0xFF-0x08 or 0xFF (control
        // FLAGS = 0x00
        // DATA = 0-1023 bytes
        // nLINK = LINK XOR 0xFF
        uint8_t linkID = 0xFF;
        uint16_t len = strlen(data);
        char *output = (char *)malloc(len + 6);
        if (output == 0) return; // out of memory, uh oh
        output[0] = 0xBF;
        output[1] = linkID;
        output[2] = len >> 8;
        output[3] = len;
        memcpy(output + 4, data, len);
        output[len + 4] = linkID ^ 0xFF;
        for (uint16_t i = 0; i < len + 5; i++) {
            uModule -> write(output[i]);
            uOutput -> write(output[i]);
        }
        free(output);
        setBusy(true);
        lines = 0;
    } else {
        // regular mode
        setBusy(true);
        lines = 0;
        uModule -> print(data);
        uModule -> print("\r\n");
    }
}

// events
void iWRAP::onA2DPCodec(void (*function) ()) { funcA2DPCodec = function; }              // 5.55 SET CONTROL AUDIO
void iWRAP::onAudioRoute(void (*function) ()) { funcAudioRoute = function; }            // 5.55 SET CONTROL AUDIO
void iWRAP::onAuth(void (*function) ()) { funcAuth = function; }                        // 5.13 CALL, 5.28 PAIR
void iWRAP::onBatteryFull(void (*function) ()) { funcBatteryFull = function; }          // 5.57 SET CONTROL BATTERY
void iWRAP::onBatteryLow(void (*function) ()) { funcBatteryLow = function; }            // 5.57 SET CONTROL BATTERY
void iWRAP::onBatteryShutdown(void (*function) ()) { funcBatteryShutdown = function; }  // 5.57 SET CONTROL BATTERY
void iWRAP::onClock(void (*function) ()) { funcClock = function; }                      // 5.13 CALL, 5.14 CLOCK
void iWRAP::onConnAuth(void (*function) ()) { funcConnAuth = function; }                // 5.16 CONNAUTH
void iWRAP::onConnect(void (*function) ()) { funcConnect = function; }                  // 5.13 CALL, 5.36 SCO OPEN
void iWRAP::onErrMaxPaircount(void (*function) ()) { funcErrMaxPaircount = function; }  // 5.48 SET BT PAIRCOUNT
void iWRAP::onIdent(void (*function) ()) { funcIdent = function; }                      // 5.22 IDENT
void iWRAP::onIdentError(void (*function) ()) { funcIdentError = function; }            // 5.22 IDENT
void iWRAP::onInquiryPartial(void (*function) ()) { funcInquiryPartial = function; }    // 5.20 INQUIRY
void iWRAP::onInquiryExtended(void (*function) ()) { funcInquiryExtended = function; }  // 5.20 INQUIRY
void iWRAP::onName(void (*function) ()) { funcName = function; }                        // 5.20 INQUIRY
void iWRAP::onNameError(void (*function) ()) { funcNameError = function; }              // 5.20 INQUIRY
void iWRAP::onNoCarrier(void (*function) ()) { funcNoCarrier = function; }              // 5.13 CALL, 5.24 KILL, 5.36 SCO OPEN
void iWRAP::onOBEXAuth(void (*function) ()) { funcOBEXAuth = function; }                // 5.93 PBAP
void iWRAP::onPair(void (*function) ()) { funcPair = function; }                        // 5.6 AUTH, 5.13 CALL, 5.28 PAIR
void iWRAP::onReady(void (*function) ()) { funcReady = function; }                      // Power-on
void iWRAP::onRing(void (*function) ()) { funcRing = function; }                        // Power-on
void iWRAP::onSyntaxError(void (*function) ()) { funcSyntaxError = function; }          // 5.14 CLOCK, 5.25 L2CAP, 5.28 PAIR, 5.39 SELECT, 5.43 SET BT CLASS, 5.44 SET BT IDENT, 5.45 SET BT LAP, 5.46 SET BT MTU, 5.47 SET BT NAME, 5.48 SET BT PAIRCOUNT
void iWRAP::onVolume(void (*function) ()) { funcVolume = function; }                    // 5.94 VOLUME
void iWRAP::onBusy(void (*function) ()) { funcBusy = function; }                        // special "busy" event from iWRAP class
void iWRAP::onIdle(void (*function) ()) { funcIdle = function; }                        // special "idle" event from iWRAP class
void iWRAP::onTimeout(void (*function) ()) { funcTimeout = function; }                  // special "timeout" event from iWRAP class

// 5.3, @
void iWRAP::sendParserCommand(iWRAPLink *link, const char *command) {}

// 5.4, AIO
uint16_t iWRAP::getAIOValue(uint8_t source) { return 0; }

// 5.5, AT
void iWRAP::testConnection() {}

// 5.6, AUTH
void iWRAP::acceptAuthentication(uint8_t *pin_code) {}
void iWRAP::rejectAuthentication() {}

// 5.7, AVRCP PDU
// TODO: not implemented

// 5.8, BATTERY
uint16_t iWRAP::getBatteryVoltage() { return 0; }

// 5.9, BSCP_ENABLE
void iWRAP::enableBCSP() {}

// 5.10, BER
float iWRAP::getBitErrorRate(iWRAPLink *link) { return 0; }

// 5.11, BOOT
void iWRAP::setBootMode(uint16_t boot_mode) {}

// 5.12, BYPASSUART
void iWRAP::enableBypassUART() {}

// 5.13, CALL
void iWRAP::call(iWRAPAddress *address, uint8_t *target, uint8_t *connect_mode, uint8_t packet_size) {}

// 5.14, CLOCK
uint32_t iWRAP::clock(iWRAPLink *link) { return 0; }

// 5.15, CLOSE
void iWRAP::close(iWRAPLink *link) {}

// 5.16, CONNAUTH
void iWRAP::acceptConnAuth(iWRAPAddress *bd_addr, uint8_t protocol_id, uint8_t channel_id) {}
void iWRAP::rejectConnAuth(iWRAPAddress *bd_addr, uint8_t protocol_id, uint8_t channel_id) {}

// 5.17, CONNECT
void iWRAP::connectLinks(iWRAPLink *link1, iWRAPLink *link2) {}

// 5.18, ECHO
void iWRAP::echo(iWRAPLink *link1, const uint8_t *string) {}

// 5.19, DEFRAG
void iWRAP::defrag() {}

// 5.20, INQUIRY
void iWRAP::discover(uint8_t timeout, bool friendlyNames, uint32_t lap) {}

// 5.21, IC
void iWRAP::cancelDiscovery() {}

// 5.22, IDENT
void iWRAP::identify(iWRAPAddress *bd_addr) {}

// 5.23, INFO
void iWRAP::getInfo(uint8_t extended) {}

// 5.24, KILL
void iWRAP::killConnection(iWRAPAddress *bd_addr, uint16_t reason) {}

// 5.25, L2CAP
void iWRAP::createL2CAP(uint8_t psm) {}

// 5.26, LIST
uint8_t iWRAP::listConnections() {
    lastCommand = IWRAP_COMMAND_LIST;
    smartSend("LIST");
}

// 5.27, NAME
bool iWRAP::getFriendlyName(iWRAPAddress *bd_addr, uint16_t *error, const uint8_t *reason) {}

// 5.28, PAIR
bool iWRAP::pair(iWRAPAddress *bd_addr) {}

// 5.29, PING
uint16_t iWRAP::ping(iWRAPLink *link) {}

// 5.30, PIO
uint8_t iWRAP::pinRead(uint8_t pin) {}
uint16_t iWRAP::readPins() {}
void iWRAP::pinWrite(uint8_t pin, uint8_t value) {}
void iWRAP::writePins(uint16_t values) {}
uint8_t iWRAP::getPinDirection(uint8_t pin) {}
uint16_t iWRAP::getPinDirections() {}
void iWRAP::setPinDirection(uint8_t pin, uint8_t direction) {}
void iWRAP::setPinDirections(uint16_t directions) {}
uint8_t iWRAP::getPinBias(uint8_t pin) {}
uint16_t iWRAP::getPinBiases() {}
void iWRAP::setPinBias(uint8_t pin, uint8_t bias) {}
void iWRAP::setPinBiases(uint16_t biases) {}
void iWRAP::resetPins() {}

// 5.31, PLAY
void iWRAP::play(uint8_t *string) {}

// 5.32, RFCOMM
uint8_t iWRAP::createRFComm() {}

// 5.33, RESET
void iWRAP::resetDevice() {
    lastCommand = IWRAP_COMMAND_RESET;
    smartSend("RESET");
}

// 5.34, RSSI
int8_t iWRAP::getSignalStrength(iWRAPLink *link) {}

// 5.35, SCO ENABLE
void iWRAP::scoEnable() {}

// 5.36, SCO OPEN
void iWRAP::scoOpen(iWRAPLink *link) {}

// 5.37, SDP
void iWRAP::discoverServices(iWRAPAddress *bd_addr, const uint8_t *uuid, bool all) {}

// 5.38, SDP ADD
void iWRAP::addService(const uint8_t *uuid, const uint8_t *name) {}

// 5.39, SELECT
void iWRAP::selectDataMode(iWRAPLink *link) {}

// 5.40, SET
void iWRAP::getSettings() {}

// 5.41, SET BT AUTH
void iWRAP::getPinCode(uint8_t *pin_code) {}
void iWRAP::setPinCode(const uint8_t *pin_code, bool hidden) {}
void iWRAP::clearPinCode() {}

// 5.42, SET BT BDADDR
iWRAPAddress iWRAP::getDeviceAddress() {}

// 5.43, SET BT CLASS
iWRAPClass iWRAP::getDeviceClass() {}
void iWRAP::setDeviceClass(iWRAPClass *class_of_device) {}
void iWRAP::setDeviceClass(uint32_t class_of_device) {}

// 5.44, SET BT IDENT
void iWRAP::setDeviceIdentity(uint8_t *descr) {}

// 5.45, SET BT LAP
void iWRAP::setDeviceLAP(uint32_t iac) {}

// 5.46, SET BT MTU
void iWRAP::setDeviceMTU(uint16_t mtu) {}

// 5.47, SET BT NAME
void iWRAP::setDeviceName(uint8_t *friendly_name) {}

// 5.48, SET BT PAIRCOUNT
void iWRAP::getDevicePairLimit() {}
void iWRAP::setDevicePairLimit(uint8_t max_pairings) {}

// 5.49, SET BT PAGEMODE
void iWRAP::getDevicePageMode() {}
void iWRAP::setDevicePageMode(uint8_t page_mode, uint16_t page_timeout, uint8_t page_scan_mode) {}

// 5.50, SET BT PAIR
void iWRAP::getPairings() {}
void iWRAP::addPairing(iWRAPAddress *bd_addr, const uint8_t *link_key) {}
void iWRAP::clearPairings() {}

// 5.51, SET BT POWER
void iWRAP::getDevicePower(int8_t *def, int8_t *maximum, int8_t *inquiry) {}
void iWRAP::setDevicePower(int8_t def, int8_t maximum, int8_t inquiry) {}
void iWRAP::setDevicePowerClass(uint8_t power_class) {}

// 5.52, SET BT ROLE
void iWRAP::getDeviceRole(int8_t *ms_policy, uint8_t *link_policy, uint16_t *supervision_timeout) {}
void iWRAP::setDeviceRole(int8_t ms_policy, uint8_t link_policy, uint16_t supervision_timeout) {}
void iWRAP::resetDeviceRole() {}

// 5.53, SET BT SNIFF
void iWRAP::getDeviceSniff() {}
void iWRAP::setDeviceSniff(uint16_t max, uint16_t min, uint16_t attempt, uint16_t timeout) {}
void iWRAP::disableDeviceSniff() {}

// 5.54, SET BT SSP
void iWRAP::getDeviceSSP() {}
void iWRAP::setDeviceSSP(uint8_t capabilities, uint8_t mitm) {}

// 5.55, SET CONTROL AUDIO
void iWRAP::getControlAudio() {}
void iWRAP::setControlAudio(const uint8_t *sco_routing, const uint8_t *a2dp_routing, uint8_t multisco, uint8_t event) {}

// 5.56, SET CONTROL AUTOCALL
void iWRAP::getControlAutoCall() {}
void iWRAP::setControlAutoCall(const uint8_t *target, uint16_t timeout, const uint8_t *profile) {}

// 5.57, SET CONTROL BATTERY
void iWRAP::getControlBattery() {}
void iWRAP::setControlBattery(uint16_t low, uint16_t shutdown, uint16_t full, uint16_t mask) {}

// 5.58, SET CONTROL BAUD
void iWRAP::getUARTConfig() {}
void iWRAP::setUARTConfig(uint32_t baud_rate, uint8_t parity, uint8_t stop_bits) {}

// 5.59, SET CONTROL BIND
void iWRAP::bindPIOCommand(uint8_t priority, uint16_t io_mask, uint8_t direction, const uint8_t *command) {}
void iWRAP::clearPIOCommand(uint8_t priority) {}

// 5.60, SET CONTROL CD
void iWRAP::getCarrierDetect() {}
void iWRAP::setCarrierDetect(uint16_t cd_mask, uint8_t datamode) {}

// 5.61, SET CONTROL CODEC
void iWRAP::getAudioCodec() {}
void iWRAP::setAudioCodec(uint8_t codec, uint8_t channel_mode, uint16_t sampling_rate, uint8_t priority) {}

// 5.62, SET CONTROL CONFIG
void iWRAP::getControlConfig(uint16_t *optional_block_2, uint16_t *optional_block_1, uint16_t config_block) {}
void iWRAP::setControlConfig(uint16_t config_block) {}
void iWRAP::setControlConfig(uint16_t optional_block_1, uint16_t config_block) {}
void iWRAP::setControlConfig(uint16_t optional_block_2, uint16_t optional_block_1, uint16_t config_block) {}
void iWRAP::commitConfig() {}

// config_block
bool iWRAP::getConfigRSSI() {}
void iWRAP::setConfigRSSI(bool enabled) {}
bool iWRAP::getConfigClockCaching() {}
void iWRAP::setConfigClockCaching(bool enabled) {}
bool iWRAP::getConfigInterlacedInquiryScan() {}
void iWRAP::setConfigInterlacedInquiryScan(bool enabled) {}
bool iWRAP::getConfigInterlacedPageScan() {}
void iWRAP::setConfigInterlacedPageScan(bool enabled) {}
bool iWRAP::getConfigDeepSleep() {}
void iWRAP::setConfigDeepSleep(bool enabled) {}
bool iWRAP::getConfigAddressInConnect() {}
void iWRAP::setConfigAddressInConnect(bool enabled) {}
bool iWRAP::getConfigPairEvent() {}
void iWRAP::setConfigPairEvent(bool enabled) {}
bool iWRAP::getConfigSCOLinks() {}
void iWRAP::setConfigSCOLinks(bool enabled) {}
bool iWRAP::getConfigInteractivePairing() {}
void iWRAP::setConfigInteractivePairing(bool enabled) {}
bool iWRAP::getConfigRecyclePairings() {}
void iWRAP::setConfigRecyclePairings(bool enabled) {}
bool iWRAP::getConfigClockEvent() {}
void iWRAP::setConfigClockEvent(bool enabled) {}
bool iWRAP::getConfigUARTLowLatency() {}
void iWRAP::setConfigUARTLowLatency(bool enabled) {}
bool iWRAP::getConfigLowInquiryPriority() {}
void iWRAP::setConfigLowInquiryPriority(bool enabled) {}

// optional_block_1
bool iWRAP::getConfigLocked() {}
void iWRAP::setConfigLocked(bool enabled) {} // DANGEROUS! THIS ONLY GOES ONE WAY UNLESS YOU HAVE AN SPI HARDWARE INTERFACE HANDY TO UNDO IT!
bool iWRAP::getConfigEnhancedInquiryResponse() {}
void iWRAP::setConfigEnhancedInquiryResponse(bool enabled) {}
bool iWRAP::getConfigAutoSCOLinks() {}
void iWRAP::setConfigAutoSCOLinks(bool enabled) {}
bool iWRAP::getConfigAutoSupervisionTimeoutAfterSwitch() {}
void iWRAP::setConfigAutoSupervisionTimeoutAfterSwitch(bool enabled) {}
bool iWRAP::getConfigConnAuthEvent() {}
void iWRAP::setConfigConnAuthEvent(bool enabled) {}
bool iWRAP::getConfigAutoRFCOMMDataMode() {}
void iWRAP::setConfigAutoRFCOMMDataMode(bool enabled) {}
bool iWRAP::getConfigOKResponse() {}
void iWRAP::setConfigOKResponse(bool enabled) {}
bool iWRAP::getConfigNoAutoHFPErrors() {}
void iWRAP::setConfigNoAutoHFPErrors(bool enabled) {}

// optional_block_2 has no allowed/unreserved bits in iWRAP4

// 5.63, SET CONTROL ECHO
void iWRAP::getControlEcho() {}
void iWRAP::setControlEcho(uint8_t echo_mask) {}

bool iWRAP::getEchoStartupBanner() {}
void iWRAP::setEchoStartupBanner(bool enabled) {}
bool iWRAP::getEchoLocal() {}
void iWRAP::setEchoLocal(bool enabled) {}
bool iWRAP::getEchoCommandSetEvents() {}
void iWRAP::setEchoCommandSetEvents(bool enabled) {}

// 5.64, SET CONTROL ESCAPE
void iWRAP::getControlEscape() {}
void iWRAP::setControlEscape(uint8_t esc_char, uint16_t dtr_mask, uint8_t dtr_mode) {}
void iWRAP::disableEscapeSequence() {}

// 5.65, SET CONTROL GAIN
void iWRAP::getControlGain() {}
void iWRAP::setControlGain(uint8_t input, uint8_t output, uint8_t def) {}

// 5.66, SET CONTROL INIT
void iWRAP::getControlInit() {}
void iWRAP::setControlInit(const uint8_t *command) {}
void iWRAP::clearControlInit() {}

// 5.67, SET CONTROL MICBIAS
void iWRAP::getControlMicBias() {}
void iWRAP::setControlMicBias(uint8_t voltage, uint8_t current) {}

// 5.68, SET CONTROL MUX
bool iWRAP::getControlMuxEnabled() {
    return config.muxModeEnabled;
}
void iWRAP::setControlMuxEnabled(bool enabled) {
    lastCommand = enabled ? IWRAP_COMMAND_SET_CONTROL_MUX_EN : IWRAP_COMMAND_SET_CONTROL_MUX_DIS;
    smartSend(enabled ? "SET CONTROL MUX 1" : "SET CONTROL MUX 0");
}
void iWRAP::forceControlMuxEnabled(bool enabled) {
    config.muxModeEnabled = enabled;
}

// 5.69, SET CONTROL MSC
void iWRAP::getControlMSC() {}
void iWRAP::setControlMSC(uint8_t mode, uint16_t dsr, uint16_t dtr, uint16_t rts, uint16_t cts, uint16_t ri, uint16_t dcd) {}

// 5.70, SET CONTROL PCM
void iWRAP::getControlPCM() {}
void iWRAP::setControlPCM(const uint8_t *config, uint16_t data) {}

// 5.71, SET CONTROL PREAMP
void iWRAP::getControlPreamp() {}
void iWRAP::setControlPreamp(bool left, bool right) {}

// 5.72, SET CONTROL RINGTONE
void iWRAP::getControlRingtone() {}
void iWRAP::setControlRingtone(const uint8_t *ringtone) {}

// 5.73, SET CONTROL READY
void iWRAP::getControlReady() {}
void iWRAP::setControlReady(uint16_t piomask) {}

// 5.74, SET CONTROL VREGEN
void iWRAP::getControlVRegEnabled() {}
void iWRAP::setControlVRegEnabled(uint8_t mode, uint16_t piomask) {}

// 5.75, SET LINK ACTIVE
void iWRAP::setLinkActive(iWRAPLink *link) {}

// 5.76, SET LINK MASTER
void iWRAP::setLinkMaster(iWRAPLink *link) {}

// 5.77, SET LINK SLAVE
void iWRAP::setLinkSlave(iWRAPLink *link) {}

// 5.78, SET LINK SNIFF
void iWRAP::setLinkSniff(iWRAPLink *link, uint16_t max, uint16_t min, uint16_t attempt, uint16_t timeout) {}
void iWRAP::setLinkSniff(iWRAPLink *link, uint16_t avg) {}

// 5.79, SET LINK SUBRATE
void iWRAP::setLinkSubrate(iWRAPLink *link, uint16_t remote_latency, uint16_t remote_timeout, uint16_t local_latency) {}

// 5.80, SET LINK MSC
void iWRAP::setLinkModemStatus(iWRAPLink *link, uint16_t status) {}

// 5.81, SET LINK SELECT
void iWRAP::setLinkSelect(iWRAPLink *link) {}

// 5.82, SET PROFILE
void iWRAP::setProfile(const char *profile, const char *sdp_name) {
    lastCommand = IWRAP_COMMAND_SET_PROFILE;
    char *command;
    uint8_t hidFeaturesWidth = 0;
    uint8_t commandLen = strlen(profile) + strlen(sdp_name) + 16;
    #if IWRAP_VERSION == 410
        if (strncmp(profile, "HID", 3) == 0) {
            commandLen += 2; // deprecated "features" flag
            hidFeaturesWidth = 2;
        }
    #endif
    if ((command = (char *)malloc(commandLen))) {
        strncpy(command, "SET PROFILE ", 12);
        strncpy(command + 12, profile, strlen(profile));
        if (sdp_name == 0 || strlen(sdp_name) == 0) {
            // this means we're disabling the profile
            command[12 + strlen(profile)] = 0x0D;
            command[13 + strlen(profile)] = 0x0A;
            command[14 + strlen(profile)] = 0;
        } else {
            // this means we're enabling the profile
            // sdp_name must be "ON" or a real profile name
            // for those that allow it (SPP, HFP/AG, HID)
            command[12 + strlen(profile)] = ' ';
            if (hidFeaturesWidth > 0) {
                command[12 + strlen(profile) + 1] = '3';
                command[12 + strlen(profile) + 2] = ' ';
            }
            strncpy(command + 13 + strlen(profile) + hidFeaturesWidth, sdp_name, strlen(sdp_name));
            command[13 + strlen(profile) + strlen(sdp_name) + hidFeaturesWidth] = 0x0D;
            command[14 + strlen(profile) + strlen(sdp_name) + hidFeaturesWidth] = 0x0A;
            command[15 + strlen(profile) + strlen(sdp_name) + hidFeaturesWidth] = 0;
        }
        smartSend(command);
        free(command);
    }
}
void iWRAP::disableProfile(const char *profile) {
    setProfile(profile, 0);
}

// 5.83, SET RESET
void iWRAP::resetSettings() {
    lastCommand = IWRAP_COMMAND_SET_RESET;
    smartSend("SET RESET");
}

// 5.84, SLEEP
void iWRAP::sleep() {}

// 5.85, SSP CONFIRM
void iWRAP::confirmSSP(iWRAPAddress *bd_addr) {}
void iWRAP::rejectSSP(iWRAPAddress *bd_addr) {}

// 5.86, SSP PASSKEY
void iWRAP::confirmSSPPasskey(iWRAPAddress *bd_addr, const char *pass_key) {}
void iWRAP::rejectSSPPasskey(iWRAPAddress *bd_addr) {}

// 5.87, SSP GETOOB
void iWRAP::getSSPOutOfBandPairingKey() {}

// 5.88, SSP GETOOB
void iWRAP::setSSPOutOfBandPairingKey(const char *key1, const char *key2) {}

// 5.89, TEMP
void iWRAP::getTemperature() {}

// 5.90, TEST
void iWRAP::test(const char *mode, const char *mode_specific_parameters) {}

// 5.91, TESTMODE
void iWRAP::enableTestMode() {}

// 5.92, TXPOWER
void iWRAP::getTXPower(iWRAPLink *link) {}

// 5.93, PBAP
void iWRAP::getPhoneBookSize(uint8_t store, uint8_t type) {}
void iWRAP::getPhoneBookEntries(uint8_t store, uint8_t type, uint16_t count, uint16_t offset, uint32_t filter, uint8_t format) {}

// 5.93, VOLUME
void iWRAP::getVolumeLevel() {}
void iWRAP::setVolumeLevel(uint8_t level) {}
void iWRAP::incrementVolumeLevel() {}
void iWRAP::decrementVolumeLevel() {}









void iWRAP::set_modifier(uint8_t c) {
    keyboard_modifier_keys = c;
}

void iWRAP::set_key1(uint8_t c) {
    keyboard_keys[0] = c;
}

void iWRAP::set_key2(uint8_t c) {
    keyboard_keys[1] = c;
}

void iWRAP::set_key3(uint8_t c) {
    keyboard_keys[2] = c;
}

void iWRAP::set_key4(uint8_t c) {
    keyboard_keys[3] = c;
}

void iWRAP::set_key5(uint8_t c) {
    keyboard_keys[4] = c;
}

void iWRAP::set_key6(uint8_t c) {
    keyboard_keys[5] = c;
}

void iWRAP::send_now() {
    uModule -> write(0x9f);
    uModule -> write(0x0a);
    uModule -> write(0xa1);
    uModule -> write(0x01);
    uModule -> write(keyboard_modifier_keys);
    uModule -> write((uint8_t)0x00);
    uModule -> write(keyboard_keys[0]);
    uModule -> write(keyboard_keys[1]);
    uModule -> write(keyboard_keys[2]);
    uModule -> write(keyboard_keys[3]);
    uModule -> write(keyboard_keys[4]);
    uModule -> write(keyboard_keys[5]);
}

void iWRAP::set_buttons(uint8_t left, uint8_t middle, uint8_t right) {
    uint8_t mask=0;
    if (left) mask |= 1;
    if (middle) mask |= 4;
    if (right) mask |= 2;
    mouse_buttons = mask;
    uModule -> write(0x9f);
    uModule -> write(0x05);
    uModule -> write(0xa1);
    uModule -> write(0x02);
    uModule -> write(mouse_buttons);
    uModule -> write((uint8_t)0x00);
    uModule -> write((uint8_t)0x00);
}

void iWRAP::move(uint8_t x, uint8_t y) {
    uModule -> write(0x9f);
    uModule -> write(0x05);
    uModule -> write(0xa1);
    uModule -> write(0x02);
    uModule -> write(mouse_buttons);
    uModule -> write(x);
    uModule -> write(y);
}

void iWRAP::move(uint8_t x, uint8_t y, uint8_t z) {
    move(x, y); // no z functionality yet
}

void iWRAP::scroll(uint8_t s) {
    move(0, 0, s);
}
