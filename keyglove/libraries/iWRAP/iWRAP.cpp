// Bluegiga iWRAP interface library
// 9/11/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/iwrap

/* ============================================
iWRAP library code is placed under the MIT license
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
    mode = IWRAP_MODE_COMMAND; // default factory setting
}

void iWRAPConfig::clear() {
    for (uint8_t i = 0; i < btLinkCount && btLink[i] != 0; i++, free(btLink[i]), btLink[i] = 0);
    btLinkCount = 0;
    btSelectedLink = 0;

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
    for (uint8_t i = 0; i < btPairCount && btPair[i] != 0; i++, free(btPair[i]), btPair[i] = 0);
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
    //mode = IWRAP_MODE_COMMAND; // default boot condition, non-MUX command mode
    // DON'T CHANGE THIS IN RESET, ONLY IN CONSTRUCTOR BECAUSE THEY PERSIST
    // DESPITE RESET, AND CAN WREAK HAVOC WITH THE ORDER OF EVENTS NEEDED
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
        if (funcTimeout != 0) funcTimeout();
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
    //uOutput -> print(ch, HEX);

    // automatically detect MUX mode if we need to
    if (!firstDataRead && config.mode != IWRAP_MODE_MUX && bufferPos > 1 && (buffer[bufferPos - 2] & 0xFF) == 0xBF && (ch & 0xFC) == 0x00) {
        config.mode = IWRAP_MODE_MUX;
        firstDataRead = true;
        muxBlockOffset = bufferPos - 2;
    }

    // test for end of line or end of MUX sequence
    uint8_t linkID = 0;
    if (config.mode == IWRAP_MODE_MUX) {
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
    } else if (config.mode == IWRAP_MODE_COMMAND) {
        if (ch == '\n') {
            // command mode lines from iWRAP end in \r\n (0x0D 0x0A)
            responseEnded = true;
            bufferPos--; // trim last \r off (\n will not be added here anyway)
            buffer[bufferPos] = 0; // null terminate
            responseLength = bufferPos;
            /*uOutput -> print("END OF SIMPLE DATA, LENGTH=");
            uOutput -> println(responseLength);
            uOutput -> print("$");
            uOutput -> print(buffer);
            uOutput -> println("$");*/
        }
    }

    // take appropriate action
    if (!responseEnded) {
        // mid-line, so append to buffer
        buffer[bufferPos++] = ch;
        /*uOutput -> print("-> ");
        uOutput -> print((uint8_t)ch, HEX);
        uOutput -> print(", bufferPos=");
        uOutput -> println(bufferPos);*/
        if (bufferPos + 1 == bufferSize) {
            bufferSize += 32;
            char *tmp = (char *)realloc(buffer, bufferSize);
            if (tmp) buffer = tmp; // safe transfer only if realloc() succeeds
        }
    } else {
        if (responseLength) {
            if (config.mode == IWRAP_MODE_COMMAND || (config.mode == IWRAP_MODE_MUX && linkID == 0xFF)) {
                // end of line/MUX sequence, so parse for events/responses
                if (!busy || lines > 0) {
                    char *test = (char *)buffer;
                    if (config.mode == IWRAP_MODE_MUX) test += 4 + muxBlockOffset; // move string pointer to beginning of data
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
                                    //iWRAPPair *btPair[16];
                                    if (config.btPairCount < 16) {
                                        iWRAPPair *pair;
                                        if (config.btPair[config.btPairCount] != 0) {
                                            // reuse existing pair object and overwrite
                                            pair = config.btPair[config.btPairCount];
                                        } else {
                                            // create new pair object
                                            pair = (iWRAPPair *)malloc(sizeof(iWRAPPair));
                                            config.btPair[config.btPairCount] = pair;
                                        }
                                        pair -> address.parseHexAddress(test + 12);
                                        strncpy(pair -> key, test + 30, 32);
                                        pair -> key[32] = 0;
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
                                    config.mode = (test[16] == '1') ? IWRAP_MODE_MUX : IWRAP_MODE_COMMAND;
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
                                //uint16_t profileHDPSinkMDEP;
                                //uint16_t profileHDPSourceMDEP;
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

                    // ======== DELAYED BUT EXPECTED RESULTS FROM COMMANDS ========
                    } else if (strncmp(test, "LIST ", 5) == 0) {
                        //uOutput -> print(" <-- LIST result found!");
                        if (expectedRows == 0) {
                            // result count, this is the first line returned
                            expectedRows = test[5] - 0x30;
                            config.btLinkCount = expectedRows;
                        } else {
                            // list entry, one per line
                            // LIST {link_id} CONNECTED {mode} {blocksize} 0 0 {elapsed_time} {local_msc} {remote_msc} {addr} {channel} {direction} {powermode} {role} {crypt} {buffer} [ERETX]
                            expectedRows--;
                            uint8_t link_id = test[5] - 0x30;
                            iWRAPLink *link;
                            if (config.btLink[link_id] != 0) {
                                // reuse existing pair object and overwrite
                                link = config.btLink[link_id];
                                if (link -> profile != 0) free(link -> profile);
                                link -> profile = 0;
                            } else {
                                // create new pair object
                                link = (iWRAPLink *)malloc(sizeof(iWRAPLink));
                                link -> link_id = link_id;
                                config.btLink[link_id] = link;
                            }

                            char *p, *p2;
                            p = test + 17; // *p is now at beginning of {mode} (or {profile}?)

                            // DOCS SAY {mode} BUT OBSERVED ITEM IS {profile} ???

                            p2 = strchr(p, ' ');
                            link -> profile = (char *)malloc((p2 - p) + 1);
                            strncpy(link -> profile, p, p2 - p);
                            link -> profile[p2 - p] = 0;
                            p = p2 + 1;

                            /*
                            // {mode} = RFCOMM | L2CAP | SCO
                            if (p[0] == 'R') {
                                link -> mode = IWRAP_LINK_MODE_RFCOMM;
                                p += 7;
                            } else if (p[0] == 'L') {
                                link -> mode = IWRAP_LINK_MODE_L2CAP;
                                p += 6;
                            } else {
                                link -> mode = IWRAP_LINK_MODE_SCO;
                                p += 4;
                            }
                            */

                            // {blocksize} = RFCOMM, L2CAP or SCO data packet size, that is, how many bytes of data can be sent in one packet
                            link -> blocksize = strtol(p, &p, 10);
                            p += 5;

                            // {elapsed_time} = Link life time in seconds
                            link -> elapsed_time = strtol(p, &p, 10);
                            p++;

                            // {local_msc} = Local serial port modem status control (MSC) bits
                            link -> local_msc = strtol(p, &p, 10);
                            p++;

                            // {remote_msc} = Remote serial port modem status control (MSC) bits
                            link -> remote_msc = strtol(p, &p, 10);
                            p++;

                            // {addr} = Bluetooth device address of the remote device
                            link -> addr.parseHexAddress(p);
                            p+= 18;

                            // {channel} = RFCOMM channel or L2CAP psm number at remote device
                            link -> channel = strtol(p, &p, 10);
                            p++;

                            // {direction} = Direction of the link. The possible values are:
                            //   OUTGOING - The connection has been initiated by the local device.
                            //   INCOMING - The connection has been initiated by the remote device
                            if (p[0] == 'O') {
                                link -> direction = IWRAP_LINK_DIRECTION_OUTGOING;
                            } else {
                                link -> direction = IWRAP_LINK_DIRECTION_INCOMING;
                            }
                            p += 9;

                            // {powermode} = mode for the link. The possible values are:
                            //   ACTIVE - Connection is in active mode, no power saving in use
                            //   SNIFF - Connection is in sniff mode
                            //   HOLD - Connection is in hold mode
                            //   PARK - Connection is in park mode
                            if (p[0] == 'A') {
                                link -> powermode = IWRAP_LINK_POWERMODE_ACTIVE;
                                p += 7;
                            } else if (p[0] == 'S') {
                                link -> powermode = IWRAP_LINK_POWERMODE_SNIFF;
                                p += 6;
                            } else if (p[0] == 'H') {
                                link -> powermode = IWRAP_LINK_POWERMODE_HOLD;
                                p += 5;
                            } else if (p[0] == 'P') {
                                link -> powermode = IWRAP_LINK_POWERMODE_PARK;
                                p += 5;
                            }

                            // {role} = Role of the link. The possible values are:
                            //   MASTER - iWRAP is the master device of this connection
                            //   SLAVE - iWRAP is the slave device of this connection
                            if (p[0] == 'M') {
                                link -> role = IWRAP_LINK_ROLE_MASTER;
                                p += 7;
                            } else {
                                link -> role = IWRAP_LINK_ROLE_SLAVE;
                                p += 6;
                            }

                            // {crypt} = Encryption state of the connection. The possible values are:
                            //   PLAIN - Connection is not encrypted
                            //   ENCRYPTED - Connection is encrypted
                            if (p[0] == 'P') {
                                link -> role = IWRAP_LINK_CRYPT_PLAIN;
                                p += 6;
                            } else {
                                link -> role = IWRAP_LINK_CRYPT_ENCRYPTED;
                                p += 10;
                            }

                            // {buffer} = Tells the amount of data (in bytes) that is stored in the incoming data buffer.
                            link -> buffer = strtol(p, &p, 10);
                            p++;

                            // [ERETX] = This flag is visible is enhanced retransmission mode is in use. At the moment only used with HDP connections.
                            if (p - buffer < bufferPos) Serial.println(p);
                            link -> eretx = (p - buffer) < bufferPos;
                        }
                        if (expectedRows == 0) {
                            // this happens when all lines are finished
                            setBusy(false);
                        }

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
                        uint8_t link_id = test[11] - 0x30;
                        iWRAPLink *link;
                        link = config.btLink[link_id];
                        uint8_t error_code = 0;
                        if (funcNoCarrier != 0) funcNoCarrier(link, error_code);
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
                        setDeviceMode(IWRAP_MODE_COMMAND);
                        if (funcReady != 0) funcReady();
                    } else if (strncmp(test, "RING ", 5) == 0) {
                        //uOutput -> print(" <-- RING event found!");
                        // RING 0 00:1b:dc:0f:5e:f4 11 HID
                        uint8_t link_id = test[5] - 0x30;
                        iWRAPLink *link;
                        if (config.btLink[link_id] == 0) {
                            // new link, so add new object to list
                            link = (iWRAPLink *)malloc(sizeof(iWRAPLink));
                            link -> link_id = link_id;
                            config.btLink[link_id] = link;
                            config.btLinkCount++;
                        } else {
                            // link already known, so get existing object
                            link = config.btLink[link_id];
                        }
                        link -> addr.parseHexAddress(test + 7);
                        char *p;
                        link -> channel = strtol(test + 25, &p, 10);
                        link -> profile = (char *)malloc(strlen(p + 1) + 1);
                        strcpy(link -> profile, p + 1);
                        link -> profile[strlen(p + 1)] = 0;
                        if (config.mode != IWRAP_MODE_MUX) setDeviceMode(IWRAP_MODE_DATA);
                        if (funcRing != 0) funcRing(link);
                        if (funcSelectLink != 0) funcSelectLink(link);
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
                        case IWRAP_COMMAND_LIST:
                            break; // these commands generate a response parsed above
    
                        case IWRAP_COMMAND_SET_CONTROL_MUX_EN:
                            config.mode = IWRAP_MODE_MUX;
                            setBusy(false);
                            break;
                        case IWRAP_COMMAND_SET_CONTROL_MUX_DIS:
                            config.mode = IWRAP_MODE_COMMAND;
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
        }
        bufferPos = 0;
        muxBlockOffset = 0;
        responseEnded = false;
        responseLength = 0;
    }
}

// read all settings into local data structures
void iWRAP::readDeviceConfig() {
    // this is the only time we need to issue a pure "SET" command
    config.clear();
    smartSendCommand("SET", IWRAP_COMMAND_SET);
}

void iWRAP::readLinkConfig() {
    // same action as the listConnections() method
    listConnections();
}

// get/set device mode
uint8_t iWRAP::getDeviceMode() {
    return config.mode;
}
void iWRAP::setDeviceMode(uint8_t mode) {
    config.mode = mode;
}
void iWRAP::exitDataMode() {
    // only do the work if we're in data mode, otherwise silently skip it
    if (config.mode == IWRAP_MODE_DATA) {
        lastCommand = IWRAP_COMMAND_RESET; // this will wait for "READY"
        setBusy(true);
        if (funcExitDataMode != 0) {
            funcExitDataMode();
        } else {
            // no user-defined method, so try sending the
            // escape sequence and hope for the best
            uint8_t escape[3];
            escape[0] = escape[1] = escape[2] = config.escapeCharacter;
            uModule -> write(escape, 3);
        }
    }
}

// send plain command or MUX-mode protocol if necessary
void iWRAP::smartSendCommand(const char *command, uint8_t command_id) {
    if (config.mode == IWRAP_MODE_MUX) {
        // MUX mode
        // [8:SOF] [8:LINK] [6:FLAGS] [10:LENGTH] [...DATA...] [8:nLINK]
        // SOF = 0xBF
        // LINK = 0xFF-0x08 or 0xFF (control
        // FLAGS = 0x00
        // DATA = 0-1023 bytes
        // nLINK = LINK XOR 0xFF
        lastCommand = command_id;
        uint16_t length = strlen(command);
        char *output = (char *)malloc(length + 6);
        if (output == 0) return; // out of memory, uh oh
        output[0] = 0xBF;
        output[1] = 0xFF; // 0xFF = command channel
        output[2] = length >> 8;
        output[3] = length;
        memcpy(output + 4, command, length);
        output[length + 4] = 0x00; // 0xFF ^ 0xFF = 0
        for (uint16_t i = 0; i < length + 5; i++) {
            uModule -> write(output[i]);
            uOutput -> write(output[i]);
        }
        free(output);
        setBusy(true);
        lines = 0;
    } else {
        // regular mode
        if (config.mode == IWRAP_MODE_DATA) {
            // currently in data mode, so exit first
            exitDataMode();
        }
        lastCommand = command_id;
        setBusy(true);
        lines = 0;
        uModule -> print(command);
        uModule -> print("\r\n");
    }
}
void iWRAP::smartSendData(const char *data, uint16_t length, iWRAPLink *link) {
    if (link == 0) {
        // sending to default channel (0 if in MUX mode) if invalid link object
        smartSendData(data, length, (uint8_t)0);
    } else {
        // sending to a specified link
        smartSendData(data, length, link -> link_id);
    }
}
void iWRAP::smartSendData(const char *data, uint16_t length, uint8_t link_id) {
    if (config.mode == IWRAP_MODE_MUX) {
        // MUX mode
        // [8:SOF] [8:LINK] [6:FLAGS] [10:LENGTH] [...DATA...] [8:nLINK]
        // SOF = 0xBF
        // LINK = 0xFF-0x08 or 0xFF (control
        // FLAGS = 0x00
        // DATA = 0-1023 bytes
        // nLINK = LINK XOR 0xFF
        char *output = (char *)malloc(length + 6);
        if (output == 0) return; // out of memory, uh oh
        output[0] = 0xBF;
        output[1] = link_id;
        output[2] = length >> 8;
        output[3] = length;
        memcpy(output + 4, data, length);
        output[length + 4] = link_id ^ 0xFF;
        for (uint16_t i = 0; i < length + 5; i++) {
            uModule -> write(output[i]);
            uOutput -> write(output[i]);
        }
        free(output);
        setBusy(true);
        lines = 0;
    } else {
        // regular mode
        if (config.mode == IWRAP_MODE_COMMAND) {
            // currently in command mode, so select link first
            selectDataMode(link_id);
            while (checkActivity());
        } else if (config.btSelectedLink != link_id) {
            // currently selected on a different link, so switch
            exitDataMode();
            while (checkActivity(1000));
            selectDataMode(link_id);
            while (checkActivity(1000));
        }
        uModule -> write((const uint8_t *)data, length);
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
void iWRAP::onNoCarrier(void (*function) (iWRAPLink *link, uint8_t error_code)) { funcNoCarrier = function; }              // 5.13 CALL, 5.24 KILL, 5.36 SCO OPEN
void iWRAP::onOBEXAuth(void (*function) ()) { funcOBEXAuth = function; }                // 5.93 PBAP
void iWRAP::onPair(void (*function) ()) { funcPair = function; }                        // 5.6 AUTH, 5.13 CALL, 5.28 PAIR
void iWRAP::onReady(void (*function) ()) { funcReady = function; }                      // Power-on
void iWRAP::onRing(void (*function) (iWRAPLink *link)) { funcRing = function; }                        // Power-on
void iWRAP::onSyntaxError(void (*function) ()) { funcSyntaxError = function; }          // 5.14 CLOCK, 5.25 L2CAP, 5.28 PAIR, 5.39 SELECT, 5.43 SET BT CLASS, 5.44 SET BT IDENT, 5.45 SET BT LAP, 5.46 SET BT MTU, 5.47 SET BT NAME, 5.48 SET BT PAIRCOUNT
void iWRAP::onVolume(void (*function) ()) { funcVolume = function; }                    // 5.94 VOLUME

void iWRAP::onBusy(void (*function) ()) { funcBusy = function; }                        // special "busy" event from iWRAP class
void iWRAP::onIdle(void (*function) ()) { funcIdle = function; }                        // special "idle" event from iWRAP class
void iWRAP::onTimeout(void (*function) ()) { funcTimeout = function; }                  // special "timeout" event from iWRAP class
void iWRAP::onExitDataMode(void (*function) ()) { funcExitDataMode = function; }        // special event when exiting data mode (GPIO control)
void iWRAP::onSelectLink(void (*function) (iWRAPLink *link)) { funcSelectLink = function; }        // special event when exiting data mode (GPIO control)

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
void iWRAP::echo(iWRAPLink *link1, const char *string) {}

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
    smartSendCommand("LIST", IWRAP_COMMAND_LIST);
}

// 5.27, NAME
bool iWRAP::getFriendlyName(iWRAPAddress *bd_addr, uint16_t *error, const char *reason) {}

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
    smartSendCommand("RESET", IWRAP_COMMAND_RESET);
}

// 5.34, RSSI
int8_t iWRAP::getSignalStrength(iWRAPLink *link) {}

// 5.35, SCO ENABLE
void iWRAP::scoEnable() {}

// 5.36, SCO OPEN
void iWRAP::scoOpen(iWRAPLink *link) {}

// 5.37, SDP
void iWRAP::discoverServices(iWRAPAddress *bd_addr, const char *uuid, bool all) {}

// 5.38, SDP ADD
void iWRAP::addService(const char *uuid, const char *name) {}

// 5.39, SELECT
void iWRAP::selectDataMode(iWRAPLink *link) {
    selectDataMode(link -> link_id);
}
void iWRAP::selectDataMode(uint8_t link_id) {
    char *command;
    command = (char *)malloc(10);
    strncpy(command, "SELECT ", 7);
    config.btSelectedLink = link_id;
    command[7] = link_id + 0x30;
    command[8] = 0; // null terminate
    smartSendCommand(command);
    if (funcSelectLink != 0) funcSelectLink(config.btLink[link_id]);
    free(command);
}

// 5.40, SET
void iWRAP::getSettings() {}

// 5.41, SET BT AUTH
void iWRAP::getPinCode(uint8_t *pin_code) {}
void iWRAP::setPinCode(const char *pin_code, bool hidden) {}
void iWRAP::clearPinCode() {}

// 5.42, SET BT BDADDR
iWRAPAddress iWRAP::getDeviceAddress() {}

// 5.43, SET BT CLASS
iWRAPClass iWRAP::getDeviceClass() {}
void iWRAP::setDeviceClass(iWRAPClass *class_of_device) {}
void iWRAP::setDeviceClass(uint32_t class_of_device) {
    char *command;
    uint8_t length;
    command = (char *)malloc(24);
    strncpy(command, "SET BT CLASS ", 13);
    length = iwsLongToHex(command + 13, class_of_device, 5);
    command[13 + length] = 0; // null terminate
    smartSendCommand(command);
    free(command);
}

// 5.44, SET BT IDENT
void iWRAP::setDeviceIdentity(const char *descr) {
    char *command;
    uint8_t length = strlen(descr);
    command = (char *)malloc(31 + length);
    strncpy(command, "SET BT IDENT BT:47 f000 4.0.0 ", 30);
    strncpy(command + 30, descr, length);
    command[30 + length] = 0; // null terminate
    smartSendCommand(command);
    free(command);
}

// 5.45, SET BT LAP
void iWRAP::setDeviceLAP(uint32_t iac) {}

// 5.46, SET BT MTU
void iWRAP::setDeviceMTU(uint16_t mtu) {}

// 5.47, SET BT NAME
void iWRAP::setDeviceName(const char *friendly_name) {
    char *command;
    uint8_t length = strlen(friendly_name);
    command = (char *)malloc(13 + length);
    strncpy(command, "SET BT NAME ", 12);
    strncpy(command + 12, friendly_name, length);
    command[12 + length] = 0; // null terminate
    smartSendCommand(command);
    free(command);
}

// 5.48, SET BT PAIRCOUNT
void iWRAP::getDevicePairLimit() {}
void iWRAP::setDevicePairLimit(uint8_t max_pairings) {}

// 5.49, SET BT PAGEMODE
void iWRAP::getDevicePageMode() {}
void iWRAP::setDevicePageMode(uint8_t page_mode, uint16_t page_timeout, uint8_t page_scan_mode) {}

// 5.50, SET BT PAIR
void iWRAP::getPairings() {}
void iWRAP::addPairing(iWRAPAddress *bd_addr, const char *link_key) {}
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
void iWRAP::setDeviceSSP(uint8_t capabilities, uint8_t mitm) {
    char *command;
    uint8_t length;
    command = (char *)malloc(15);
    strncpy(command, "SET BT SSP 3 0", 14);
    command[11] = capabilities + 0x30;
    command[13] = mitm + 0x30;
    command[14] = 0; // null terminate
    smartSendCommand(command);
    free(command);
}

// 5.55, SET CONTROL AUDIO
void iWRAP::getControlAudio() {}
void iWRAP::setControlAudio(const char *sco_routing, const char *a2dp_routing, uint8_t multisco, uint8_t event) {}

// 5.56, SET CONTROL AUTOCALL
void iWRAP::getControlAutoCall() {}
void iWRAP::setControlAutoCall(const char *target, uint16_t timeout, const char *profile) {}

// 5.57, SET CONTROL BATTERY
void iWRAP::getControlBattery() {}
void iWRAP::setControlBattery(uint16_t low, uint16_t shutdown, uint16_t full, uint16_t mask) {}

// 5.58, SET CONTROL BAUD
void iWRAP::getUARTConfig() {}
void iWRAP::setUARTConfig(uint32_t baud_rate, uint8_t parity, uint8_t stop_bits) {}

// 5.59, SET CONTROL BIND
void iWRAP::bindPIOCommand(uint8_t priority, uint16_t io_mask, uint8_t direction, const char *command) {}
void iWRAP::clearPIOCommand(uint8_t priority) {}

// 5.60, SET CONTROL CD
void iWRAP::getCarrierDetect() {}
void iWRAP::setCarrierDetect(uint16_t cd_mask, uint8_t datamode) {
    char *command;
    uint8_t length;
    command = (char *)malloc(32);
    strncpy(command, "SET CONTROL CD ", 15);
    length = iwsIntToHex(command + 15, cd_mask, 2);
    command[15 + length] = ' ';
    command[16 + length] = datamode + 0x30; // number to ASCII representation
    command[17 + length] = 0; // null terminate
    smartSendCommand(command);
    free(command);
}

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
void iWRAP::setControlEscape(uint8_t esc_char, uint16_t dtr_mask, uint8_t dtr_mode) {
    char *command;
    uint8_t length;
    command = (char *)malloc(32);
    strncpy(command, "SET CONTROL ESCAPE ", 19);
    command[19] = esc_char;
    command[20] = ' ';
    length = iwsIntToHex(command + 21, dtr_mask, 2);
    command[21 + length] = ' ';
    command[22 + length] = dtr_mode + 0x30; // number to ASCII representation
    command[23 + length] = 0; // null terminate
    smartSendCommand(command);
    free(command);
}
void iWRAP::disableEscapeSequence() {}

// 5.65, SET CONTROL GAIN
void iWRAP::getControlGain() {}
void iWRAP::setControlGain(uint8_t input, uint8_t output, uint8_t def) {}

// 5.66, SET CONTROL INIT
void iWRAP::getControlInit() {}
void iWRAP::setControlInit(const char *command) {}
void iWRAP::clearControlInit() {}

// 5.67, SET CONTROL MICBIAS
void iWRAP::getControlMicBias() {}
void iWRAP::setControlMicBias(uint8_t voltage, uint8_t current) {}

// 5.68, SET CONTROL MUX
bool iWRAP::getControlMuxEnabled() {
    return config.mode == IWRAP_MODE_MUX;
}
void iWRAP::setControlMuxEnabled(bool enabled) {
    char command[] = "SET CONTROL MUX 0\0";
    if (enabled) command[16] = '1';
    smartSendCommand(command, enabled ? IWRAP_COMMAND_SET_CONTROL_MUX_EN : IWRAP_COMMAND_SET_CONTROL_MUX_DIS);
}

// 5.69, SET CONTROL MSC
void iWRAP::getControlMSC() {}
void iWRAP::setControlMSC(uint8_t mode, uint16_t dsr, uint16_t dtr, uint16_t rts, uint16_t cts, uint16_t ri, uint16_t dcd) {}

// 5.70, SET CONTROL PCM
void iWRAP::getControlPCM() {}
void iWRAP::setControlPCM(const char *config, uint16_t data) {}

// 5.71, SET CONTROL PREAMP
void iWRAP::getControlPreamp() {}
void iWRAP::setControlPreamp(bool left, bool right) {}

// 5.72, SET CONTROL RINGTONE
void iWRAP::getControlRingtone() {}
void iWRAP::setControlRingtone(const char *ringtone) {}

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
    char *command;
    uint8_t hidFeaturesWidth = 0;
    uint8_t commandLen = strlen(profile) + strlen(sdp_name) + 14;
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
            command[12 + strlen(profile)] = 0;
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
            command[13 + strlen(profile) + strlen(sdp_name) + hidFeaturesWidth] = 0;
        }
        smartSendCommand(command, IWRAP_COMMAND_SET_PROFILE);
        free(command);
    }
}
void iWRAP::disableProfile(const char *profile) {
    setProfile(profile, 0);
}

// 5.83, SET RESET
void iWRAP::resetSettings() {
    smartSendCommand("SET RESET", IWRAP_COMMAND_SET_RESET);
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

int8_t iWRAP::iwsIntToHex(char *str, uint16_t hex, uint8_t width) {
    uint8_t length = 0;
    uint8_t offset = 0;
    uint8_t hexWidth = 0;
    if (hex > 4095) hexWidth = 4;
    else if (hex > 255) hexWidth = 3;
    else if (hex > 15) hexWidth = 2;
    else hexWidth = 1;
    for (offset = 0; offset < width - hexWidth; offset++) {
        str[offset] = '0';
    }
    uint8_t current = hex % 16;
    do {
        str[offset + hexWidth - length - 1] = current < 10 ? (current + 0x30) : (current + 0x37);
        length++;
        hex /= 16;
        current = hex % 16;
    } while (hex > 0);
    return length;
}

int8_t iWRAP::iwsLongToHex(char *str, uint32_t hex, uint8_t width) {
    uint8_t length = 0;
    uint8_t offset = 0;
    uint8_t hexWidth = 0;
    if (hex > 0xFFFFFFF) hexWidth = 8;
    else if (hex > 0xFFFFFF) hexWidth = 7;
    else if (hex > 0xFFFFF) hexWidth = 6;
    else if (hex > 0xFFFF) hexWidth = 5;
    else if (hex > 0xFFF) hexWidth = 4;
    else if (hex > 0xFF) hexWidth = 3;
    else if (hex > 0xF) hexWidth = 2;
    else hexWidth = 1;
    for (offset = 0; offset < width - hexWidth; offset++) {
        str[offset] = '0';
    }
    uint8_t current = hex % 16;
    do {
        str[offset + hexWidth - length - 1] = current < 10 ? (current + 0x30) : (current + 0x37);
        length++;
        hex /= 16;
        current = hex % 16;
    } while (hex > 0);
    return length + offset;
}










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