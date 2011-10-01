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

#ifndef _IWRAP_H_
#define _IWRAP_H_

#ifdef LUFA_ARDUINO_WRAPPER
    #include "ArduinoWrapper.h"
#endif

#ifndef LUFA_ARDUINO_WRAPPER
    #include "Arduino.h"
#endif

#define IWRAP_VERSION 400
//#define IWRAP_VERSION 410
//#define IWRAP_VERSION 500

#define IWRAP_MODE_COMMAND          0 // iWRAP control via UART commands (also HFP, HSP, OBEX, A2DP, AVRCP, PBAP)
#define IWRAP_MODE_DATA             1 // passthrough (SPP, HID
#define IWRAP_MODE_MUX              2 // simultaneous COMMAND and DATA

// iWRAP4 User Guide Section 5.4, AIO
#define IWRAP_AIO_SOURCE_0          0 // AIO0 on WT32
#define IWRAP_AIO_SOURCE_1          1 // AIO1 on WT32
#define IWRAP_AIO_SOURCE_INTERNAL   2 // Interval voltage reference on WT32

// iWRAP4 User Guide Section 5.7, BOOT
#define IWRAP_BOOT_MODE_IWRAP       0x0000
#define IWRAP_BOOT_MODE_HCI_BCSP    0x0001
#define IWRAP_BOOT_MODE_HCI_USB     0x0003
#define IWRAP_BOOT_MODE_HCI_H4      0x0004

// iWRAP4 User Guide Section 5.20, INQUIRY
#define IWRAP_INQUIRY_LAP_GIAC      0x9E8B33
#define IWRAP_INQUIRY_LAP_LIAC      0x9E8B00

// iWRAP4 User Guide Section 5.23, INFO
#define IWRAP_INFO_CONFIG           1
#define IWRAP_INFO_BOOTMODE         2

// iWRAP4 User Guide Section 5.24, KILL (see Chapter 9)
#define IWRAP_KILL_REASON_POWEROFF  0x115
#define IWRAP_INFO_BOOTMODE         2

// iWRAP4 User Guide Section 5.43, SET BT CLASS
// see also http://bluetooth-pentest.narod.ru/software/bluetooth_class_of_device-service_generator.html

// major service class values (logically OR together)
#define IWRAP_CLASS_SERVICE_LIMITED      0x2000
#define IWRAP_CLASS_SERVICE_POSITIONING  0x10000
#define IWRAP_CLASS_SERVICE_NETWORKING   0x20000
#define IWRAP_CLASS_SERVICE_RENDERING    0x40000
#define IWRAP_CLASS_SERVICE_CAPTURING    0x80000
#define IWRAP_CLASS_SERVICE_OBJTRANSFER  0x100000
#define IWRAP_CLASS_SERVICE_AUDIO        0x200000
#define IWRAP_CLASS_SERVICE_TELEPHONY    0x400000
#define IWRAP_CLASS_SERVICE_INFORMATION  0x800000

// major device class values (single value selected per device)
#define IWRAP_CLASS_DEVICE_COMPUTER      0x100
#define IWRAP_CLASS_DEVICE_PHONE         0x200
#define IWRAP_CLASS_DEVICE_NETWORK       0x300
#define IWRAP_CLASS_DEVICE_AV            0x400
#define IWRAP_CLASS_DEVICE_PERIPHERAL    0x500
#define IWRAP_CLASS_DEVICE_IMAGING       0x600
#define IWRAP_CLASS_DEVICE_WEARABLE      0x700
#define IWRAP_CLASS_DEVICE_TOY           0x800
#define IWRAP_CLASS_DEVICE_MISC          0x0
#define IWRAP_CLASS_DEVICE_UNCATEGORIZED 0x1F00

// minor device class values for COMPUTER major device class (mathematically add together)
#define IWRAP_DCLASS_COMPUTER_DESKTOP    0x04
#define IWRAP_DCLASS_COMPUTER_SERVER     0x08
#define IWRAP_DCLASS_COMPUTER_LAPTOP     0x0C
#define IWRAP_DCLASS_COMPUTER_CLAMSHELL  0x10
#define IWRAP_DCLASS_COMPUTER_PALMTOP    0x14
#define IWRAP_DCLASS_COMPUTER_WEARABLE   0x18

// minor device class values for PHONE major device class (mathematically add together)
#define IWRAP_DCLASS_PHONE_CELLULAR      0x04
#define IWRAP_DCLASS_PHONE_CORDLESS      0x08
#define IWRAP_DCLASS_PHONE_SMARTPHONE    0x0C
#define IWRAP_DCLASS_PHONE_MODEM         0x10
#define IWRAP_DCLASS_PHONE_ISDN          0x14

// minor device class values for NETWORK major device class (mathematically add together)
#define IWRAP_DCLASS_NETWORK_AVAILABLE   0x00
#define IWRAP_DCLASS_NETWORK_17PCT       0x02
#define IWRAP_DCLASS_NETWORK_33PCT       0x04
#define IWRAP_DCLASS_NETWORK_50PCT       0x06
#define IWRAP_DCLASS_NETWORK_67PCT       0x08
#define IWRAP_DCLASS_NETWORK_83PCT       0x0A
#define IWRAP_DCLASS_NETWORK_99PCT       0x0C
#define IWRAP_DCLASS_NETWORK_UNAVAILABLE 0x0E

// minor device class values for AV major device class (mathematically add together)
#define IWRAP_DCLASS_AV_HEADSET          0x04
#define IWRAP_DCLASS_AV_HANDSFREE        0x08
#define IWRAP_DCLASS_AV_MICROPHONE       0x10
#define IWRAP_DCLASS_AV_LOUDSPEAKER      0x14
#define IWRAP_DCLASS_AV_HEADPHONES       0x18
#define IWRAP_DCLASS_AV_PORTABLEAUDIO    0x1C
#define IWRAP_DCLASS_AV_CARAUDIO         0x20
#define IWRAP_DCLASS_AV_SETTOPBOX        0x24
#define IWRAP_DCLASS_AV_HIFIAUDIO        0x28
#define IWRAP_DCLASS_AV_VCR              0x2C
#define IWRAP_DCLASS_AV_VIDEOCAMERA      0x30
#define IWRAP_DCLASS_AV_CAMCORDER        0x34
#define IWRAP_DCLASS_AV_VIDEOMONITOR     0x38
#define IWRAP_DCLASS_AV_VIDEOAUDIO       0x3C
#define IWRAP_DCLASS_AV_VIDEOCONFERENCE  0x40
#define IWRAP_DCLASS_AV_GAME             0x48

// minor device class values for PERIPHERAL major device class (logically OR together)
#define IWRAP_DCLASS_PERIPHERAL_SIMPLE   0x00
#define IWRAP_DCLASS_PERIPHERAL_KEYBOARD 0x40
#define IWRAP_DCLASS_PERIPHERAL_MOUSE    0x80
#define IWRAP_DCLASS_PERIPHERAL_KBMOUSE  0xC0
// minor device subclass values for PERIPHERAL major device class (mathematically add together)
#define IWRAP_DCLASS2_PERIPHERAL_JOYSTICK    0x04
#define IWRAP_DCLASS2_PERIPHERAL_GAMEPAD     0x08
#define IWRAP_DCLASS2_PERIPHERAL_REMOTE      0x0C
#define IWRAP_DCLASS2_PERIPHERAL_SENSING     0x10
#define IWRAP_DCLASS2_PERIPHERAL_TABLET      0x14
#define IWRAP_DCLASS2_PERIPHERAL_CARDREADER  0x18

// minor device class values for IMAGING major device class (mathematically add together)
#define IWRAP_DCLASS_IMAGING_DISPLAY     0x10
#define IWRAP_DCLASS_IMAGING_CAMERA      0x20
#define IWRAP_DCLASS_IMAGING_SCANNER     0x40
#define IWRAP_DCLASS_IMAGING_PRINTER     0x80

// minor device class values for WEARABLE major device class (mathematically add together)
#define IWRAP_DCLASS_WEARABLE_WRISTWATCH 0x04
#define IWRAP_DCLASS_WEARABLE_PAGER      0x08
#define IWRAP_DCLASS_WEARABLE_JACKET     0x0C
#define IWRAP_DCLASS_WEARABLE_HELMET     0x10
#define IWRAP_DCLASS_WEARABLE_GLASSES    0x14

// minor device class values for TOY major device class (mathematically add together)
#define IWRAP_DCLASS_TOY_ROBOT           0x04
#define IWRAP_DCLASS_TOY_VEHICLE         0x08
#define IWRAP_DCLASS_TOY_ACTIONFIGURE    0x0C
#define IWRAP_DCLASS_TOY_CONTROLLER      0x10
#define IWRAP_DCLASS_TOY_GAME            0x14

// iWRAP4 User Guide Section 5.45, SET BT LAP
#define IWRAP_BT_LAP_GIAC                0x9E8B33
#define IWRAP_BT_LAP_LIAC                0x9E8B00

// iWRAP4 User Guide Section 5.49, SET BT PAGEMODE
#define IWRAP_BT_PAGE_MODE_NOVIS_NOANS   0x0
#define IWRAP_BT_PAGE_MODE_VIS_NOANS     0x1
#define IWRAP_BT_PAGE_MODE_NOVIS_ANS     0x2
#define IWRAP_BT_PAGE_MODE_VIS_ANS       0x3
#define IWRAP_BT_PAGE_MODE_VIS_ANS_CONN  0x4

#define IWRAP_BT_PAGE_SCAN_CONN_ALWAYS   0x0
#define IWRAP_BT_PAGE_SCAN_CONN_128      0x1
#define IWRAP_BT_PAGE_SCAN_CONN_256      0x2

// iWRAP4 User Guide Section 5.52, SET BT ROLE
#define IWRAP_BT_ROLE_MS_SWITCH_NOREQ    0x0
#define IWRAP_BT_ROLE_MS_SWITCH_REQ      0x1
#define IWRAP_BT_ROLE_MS_NOSWITCH_REQ    0x2

#define IWRAP_BT_ROLE_LINK_ROLESWITCH    0x1
#define IWRAP_BT_ROLE_LINK_HOLDMODE      0x2
#define IWRAP_BT_ROLE_LINK_SNIFFMODE     0x4
#define IWRAP_BT_ROLE_LINK_PARKSTATE     0x8
#define IWRAP_BT_ROLE_LINK_ALL           0xF
#define IWRAP_BT_ROLE_LINK_NONE          0x0

// iWRAP4 User Guide section 5.54, SET BT SSP
#define IWRAP_BT_SSP_DISPLAY_ONLY        0x0
#define IWRAP_BT_SSP_DISPLAY_YN          0x1
#define IWRAP_BT_SSP_KEYBOARD_ONLY       0x2
#define IWRAP_BT_SSP_NONE                0x3

#define IWRAP_BT_SSP_MITM_DISABLED       0x0
#define IWRAP_BT_SSP_MITM_ENABLED        0x1

// iWRAP4 User Guide section 5.59, SET CONTROL BIND
#define IWRAP_BIND_DIRECTION_RISE        1
#define IWRAP_BIND_DIRECTION_FALL        2
#define IWRAP_BIND_DIRECTION_CHANGE      3

// iWRAP4 User Guide section 5.60, SET CONTROL CD
#define IWRAP_CARRIER_DETECT_CONNECTIONS 0x0
#define IWRAP_CARRIER_DETECT_DATAMODE    0x1

// iWRAP4 User Guide section 5.61, SET CONTROL CODEC
#define IWRAP_CODEC_SBC                  1
#define IWRAP_CODEC_APTX                 2
#define IWRAP_CODEC_FASTSTREAM           3

#define IWRAP_CODEC_CHANNEL_JOINT_STEREO 1
#define IWRAP_CODEC_CHANNEL_STEREO       2
#define IWRAP_CODEC_CHANNEL_DUAL_CHANNEL 3
#define IWRAP_CODEC_CHANNEL_MONO         4

// iWRAP4 User Guide section 5.64, SET CONTROL ESCAPE
#define IWRAP_DTR_MODE_DISABLED          0x0
#define IWRAP_DTR_MODE_COMMAND_MODE      0x1
#define IWRAP_DTR_MODE_CLOSE_ACTIVE      0x2
#define IWRAP_DTR_MODE_SOFT_RESET        0x3

// iWRAP4 User Guide section 5.65, SET CONTROL GAIN
#define IWRAP_GAIN_N24                   0x00
#define IWRAP_GAIN_N21                   0x01
#define IWRAP_GAIN_N18                   0x02
#define IWRAP_GAIN_N15                   0x03
#define IWRAP_GAIN_N12                   0x04
#define IWRAP_GAIN_N9                    0x05
#define IWRAP_GAIN_N6                    0x06
#define IWRAP_GAIN_N3                    0x07
#define IWRAP_GAIN_0                     0x08
#define IWRAP_GAIN_P3                    0x09
#define IWRAP_GAIN_P6                    0x0A
#define IWRAP_GAIN_P9                    0x0B
#define IWRAP_GAIN_P12                   0x0C
#define IWRAP_GAIN_P15                   0x0D
#define IWRAP_GAIN_P18                   0x0E
#define IWRAP_GAIN_P21                   0x0F
#define IWRAP_GAIN_P24                   0x10
#define IWRAP_GAIN_P27                   0x11
#define IWRAP_GAIN_P30                   0x12
#define IWRAP_GAIN_P33                   0x13
#define IWRAP_GAIN_P36                   0x14
#define IWRAP_GAIN_P39                   0x15
#define IWRAP_GAIN_P42                   0x16

// iWRAP4 User Guide section 5.66, SET CONTROL MICBIAS
#define IWRAP_MIC_BIAS_V_161_180         0x0
#define IWRAP_MIC_BIAS_V_165_186         0x1
#define IWRAP_MIC_BIAS_V_171_183         0x2
#define IWRAP_MIC_BIAS_V_176_188         0x3
#define IWRAP_MIC_BIAS_V_184_206         0x4
#define IWRAP_MIC_BIAS_V_189_214         0x5
#define IWRAP_MIC_BIAS_V_197_223         0x6
#define IWRAP_MIC_BIAS_V_204_232         0x7
#define IWRAP_MIC_BIAS_V_218_246         0x8
#define IWRAP_MIC_BIAS_V_227_258         0x9
#define IWRAP_MIC_BIAS_V_239_272         0xA
#define IWRAP_MIC_BIAS_V_250_287         0xB
#define IWRAP_MIC_BIAS_V_270_309         0xC
#define IWRAP_MIC_BIAS_V_285_329         0xD
#define IWRAP_MIC_BIAS_V_307_356         0xE
#define IWRAP_MIC_BIAS_V_328_384         0xF

#define IWRAP_MIC_BIAS_C_0237_0394       0x0
#define IWRAP_MIC_BIAS_C_0296_0492       0x1
#define IWRAP_MIC_BIAS_C_0354_0589       0x2
#define IWRAP_MIC_BIAS_C_0412_0687       0x3
#define IWRAP_MIC_BIAS_C_0471_0785       0x4
#define IWRAP_MIC_BIAS_C_0530_0883       0x5
#define IWRAP_MIC_BIAS_C_0589_0980       0x6
#define IWRAP_MIC_BIAS_C_0647_1078       0x7
#define IWRAP_MIC_BIAS_C_0706_1176       0x8
#define IWRAP_MIC_BIAS_C_0764_1273       0x9
#define IWRAP_MIC_BIAS_C_0823_1371       0xA
#define IWRAP_MIC_BIAS_C_0882_1469       0xB
#define IWRAP_MIC_BIAS_C_0940_1566       0xC
#define IWRAP_MIC_BIAS_C_0998_1664       0xD
#define IWRAP_MIC_BIAS_C_1057_1762       0xE
#define IWRAP_MIC_BIAS_C_1167_1859       0xF

// iWRAP4 User Guide section 5.70, SET CONTROL PCM
#define IWRAP_PCM_INPUT_CODING_LINEAR    0b0000000000
#define IWRAP_PCM_INPUT_CODING_ULAW      0b0100000000
#define IWRAP_PCM_INPUT_CODING_ALAW      0b1000000000
#define IWRAP_PCM_INPUT_FORMAT_1SCOMP    0b0000000000
#define IWRAP_PCM_INPUT_FORMAT_2SCOMP    0b0000000000
#define IWRAP_PCM_INPUT_FORMAT_SIGNMAG   0b0001000000
#define IWRAP_PCM_INPUT_FORMAT_UNSIGNED  0b0011000000
#define IWRAP_PCM_INPUT_SAMPLE_SIZE_8    0b0000000000
#define IWRAP_PCM_INPUT_SAMPLE_SIZE_16   0b0000100000
#define IWRAP_PCM_INPUT_LINEAR_BIT_POS_0 0b0000000000
#define IWRAP_PCM_INPUT_LINEAR_BIT_POS_1 0b0000000100
#define IWRAP_PCM_INPUT_LINEAR_BIT_POS_2 0b0000001000
#define IWRAP_PCM_INPUT_LINEAR_BIT_POS_3 0b0000001100
#define IWRAP_PCM_INPUT_LINEAR_BIT_POS_4 0b0000010000
#define IWRAP_PCM_INPUT_LINEAR_BIT_POS_5 0b0000010100
#define IWRAP_PCM_INPUT_LINEAR_BIT_POS_6 0b0000010000
#define IWRAP_PCM_INPUT_LINEAR_BIT_POS_7 0b0000011100
#define IWRAP_PCM_AIR_CODING_CVSD        0b0000000000
#define IWRAP_PCM_AIR_CODING_ULAW        0b0000000001
#define IWRAP_PCM_AIR_CODING_ALAW        0b0000000010
#define IWRAP_PCM_AIR_CODING_TRANS       0b0000000011

// iWRAP4 User Guide section 5.74, SET CONTROL VREGEN
#define IWRAP_VREG_MODE_HOLD             0x0
#define IWRAP_VREG_MODE_FALLING          0x1
#define IWRAP_VREG_MODE_RISING           0x2

// iWRAP4 User Guide section 7.1, HCI Errors
#define IWRAP_HCI_SUCCESS                            0x100
#define IWRAP_HCI_ERROR_ILLEGAL_COMMAND              0x101
#define IWRAP_HCI_ERROR_NO_CONNECTION                0x102
#define IWRAP_HCI_ERROR_HARDWARE_FAIL                0x103
#define IWRAP_HCI_ERROR_PAGE_TIMEOUT                 0x104
#define IWRAP_HCI_ERROR_AUTH_FAIL                    0x105
#define IWRAP_HCI_ERROR_KEY_MISSING                  0x106
#define IWRAP_HCI_ERROR_MEMORY_FULL                  0x107
#define IWRAP_HCI_ERROR_CONN_TIMEOUT                 0x108
#define IWRAP_HCI_ERROR_MAX_NR_OF_CONNS              0x109
#define IWRAP_HCI_ERROR_MAX_NR_OF_SCO                0x10A
#define IWRAP_HCI_ERROR_MAX_NR_OF_ACL                0x10B
#define IWRAP_HCI_ERROR_COMMAND_DISALLOWED           0x10C
#define IWRAP_HCI_ERROR_REJ_BY_REMOTE_NO_RES         0x10D
#define IWRAP_HCI_ERROR_REJ_BY_REMOTE_SEC            0x10E
#define IWRAP_HCI_ERROR_REJ_BY_REMOTE_PERS           0x10F
#define IWRAP_HCI_ERROR_HOST_TIMEOUT                 0x110
#define IWRAP_HCI_ERROR_UNSUPPORTED_FEATURE          0x111
#define IWRAP_HCI_ERROR_ILLEGAL_FORMAT               0x112
#define IWRAP_HCI_ERROR_OETC_USER                    0x113
#define IWRAP_HCI_ERROR_OETC_LOW_RESOURCE            0x114
#define IWRAP_HCI_ERROR_OETC_POWERING_OFF            0x115
#define IWRAP_HCI_ERROR_CONN_TERM_LOCAL_HOST         0x116
#define IWRAP_HCI_ERROR_AUTH_REPEATED                0x117
#define IWRAP_HCI_ERROR_PAIRING_NOT_ALLOWED          0x118
#define IWRAP_HCI_ERROR_UNKNOWN_LMP_PDU              0x119
#define IWRAP_HCI_ERROR_UNSUPPORTED_REM_FEATURE      0x11A
#define IWRAP_HCI_ERROR_SCO_OFFSET_REJECTED          0x11B
#define IWRAP_HCI_ERROR_SCO_INTERVAL_REJECTED        0x11C
#define IWRAP_HCI_ERROR_SCO_AIR_MODE_REJECTED        0x11D
#define IWRAP_HCI_ERROR_INVALID_LMP_PARAMETERS       0x11E
#define IWRAP_HCI_ERROR_UNSPECIFIED                  0x11F
#define IWRAP_HCI_ERROR_UNSUPP_LMP_PARAM             0x120
#define IWRAP_HCI_ERROR_ROLE_CHANGE_NOT_ALLOWED      0x121
#define IWRAP_HCI_ERROR_LMP_RESPONSE_TIMEOUT         0x122
#define IWRAP_HCI_ERROR_LMP_TRANSACTION_COLLISION    0x123
#define IWRAP_HCI_ERROR_LMP_PDU_NOT_ALLOWED          0x124
#define IWRAP_HCI_ERROR_ENC_MODE_NOT_ACCEPTABLE      0x125
#define IWRAP_HCI_ERROR_UNIT_KEY_USED                0x126
#define IWRAP_HCI_ERROR_QOS_NOT_SUPPORTED            0x127
#define IWRAP_HCI_ERROR_INSTANT_PASSED               0x128
#define IWRAP_HCI_ERROR_PAIR_UNIT_KEY_NO_SUPPORT     0x129

// iWRAP4 User Guide section 7.2, SDP Errors
#define IWRAP_SDC_OK                                 0x300
#define IWRAP_SDC_OPEN_SEARCH_BUSY                   0x301
#define IWRAP_SDC_OPEN_SEARCH_FAILED                 0x302
#define IWRAP_SDC_OPEN_SEARCH_OPEN                   0x303
#define IWRAP_SDC_OPEN_DISCONNECTED                  0x304
#define IWRAP_SDC_OPEN_SEARCH_FAILED_PAGE_TIMEOUT    0x305
#define IWRAP_SDC_OPEN_SEARCH_FAILED_REJ_PS          0x306
#define IWRAP_SDC_OPEN_SEARCH_FAILED_REJ_SECURITY    0x307
#define IWRAP_SDC_OPEN_SEARCH_FAILED_REJ_RESOURCES   0x308
#define IWRAP_SDC_OPEN_SEARCH_FAILED_SIGNAL_TIMEOUT  0x309
#define IWRAP_SDC_ERROR_RESPONSE_PDU                 0x310
#define IWRAP_SDC_NO_RESPONSE_DATA                   0x311
#define IWRAP_SDC_CON_DISCONNECTED                   0x312
#define IWRAP_SDC_CONNECTION_ERROR                   0x313
#define IWRAP_SDC_CONFIGURE_ERROR                    0x314
#define IWRAP_SDC_SEARCH_DATA_ERROR                  0x315
#define IWRAP_SDC_DATA_CFM_ERROR                     0x316
#define IWRAP_SDC_SEARCH_BUSY                        0x317
#define IWRAP_SDC_RESPONSE_PDU_HEADER_ERROR          0x318
#define IWRAP_SDC_RESPONSE_PDU_SIZE_ERROR            0x319
#define IWRAP_SDC_RESPONSE_TIMEOUT_ERROR             0x31A
#define IWRAP_SDC_SEARCH_SIZE_TOO_BIG                0x31B
#define IWRAP_SDC_RESPONSE_OUT_OF_MEMORY             0x31C
#define IWRAP_SDC_RESPONSE_TERMINATED                0x31D
//#define IWRAP_SDC_OPEN_SEARCH_FAILED_PAGE_TIMEOUT    0x305
//#define IWRAP_SDC_OPEN_SEARCH_FAILED_REJ_TIMEOUT     0x305
// last two are duplicates in the datasheed...typos?

// iWRAP4 User Guide section 7.3, RFCOMM Errors
#define IWRAP_RFC_OK                                   0x400
#define IWRAP_RFC_CONNECTION_PENDING                   0x401
#define IWRAP_RFC_CONNECTION_REJ_PSM                   0x402
#define IWRAP_RFC_CONNECTION_REJ_SECURITY              0x403
#define IWRAP_RFC_CONNECTION_REJ_RESOURCES             0x404
#define IWRAP_RFC_CONNECTION_REJ_NOT_READY             0x405
#define IWRAP_RFC_CONNECTION_FAILED                    0x406
#define IWRAP_RFC_CONNECTION_TIMEOUT                   0x407
#define IWRAP_RFC_NORMAL_DISCONNECT                    0x408
#define IWRAP_RFC_ABNORMAL_DISCONNECT                  0x409
#define IWRAP_RFC_CONFIG_UNACCEPTABLE                  0x40A
#define IWRAP_RFC_CONFIG_REJECTED                      0x40B
#define IWRAP_RFC_CONFIG_INVALID_CID                   0x40C
#define IWRAP_RFC_CONFIG_UNKNOWN                       0x40D
#define IWRAP_RFC_CONFIG_REJECTED_LOCALLY              0x40E
#define IWRAP_RFC_CONFIG_TIMEOUT                       0x40F
#define IWRAP_RFC_REMOTE_REFUSAL                       0x411
#define IWRAP_RFC_RACE_CONDITION_DETECTED              0x412
#define IWRAP_RFC_INSUFFICIENT_RESOURCES               0x413
#define IWRAP_RFC_CANNOT_CHANGE_FLOW_CONTROL_MECHANISM 0x414
#define IWRAP_RFC_DLC_ALREADY_EXISTS                   0x415
#define IWRAP_RFC_DLC_REJ_SECURITY                     0x416
#define IWRAP_RFC_DLS_GENERIC_REFUSAL                  0x41F
#define IWRAP_RFC_UNEXPECTED_PRIMITIVE                 0x420
#define IWRAP_RFC_INVALID_SERVER_CHANNEL               0x421
#define IWRAP_RFC_UNKNOWN_MUX_ID                       0x422
#define IWRAP_RFC_LOCAL_ENTITY_TERMINATED_CONNECTION   0x423
#define IWRAP_RFC_UNKNOWN_PRIMITIVE                    0x424
#define IWRAP_RFC_MAX_PAYLOAD_EXCEEDED                 0x425
#define IWRAP_RFC_INCONSISTENT_PARAMETERS              0x426
#define IWRAP_RFC_INSUFFICIENT_CREDITS                 0x427
#define IWRAP_RFC_CREDIT_FLOW_CONTROL_PROTOCOL_VIOLATION   0x428
#define IWRAP_RFC_RES_ACK_TIMEOUT                      0x430
//#define IWRAP_RFC_CONNECTION_REJ_SSP_AUTH_FAIL         0x0
//#define IWRAP_L2CAP_CONNECTION_SSP_AUTH_FAIL           0x0
// last two declared but not defined in the datasheet...typos?

// iWRAP "last command" tracking for this library's internal parsing
#define IWRAP_COMMAND_LIST          1
#define IWRAP_COMMAND_RESET         2
#define IWRAP_COMMAND_SET           3
#define IWRAP_COMMAND_SET_CONTROL_MUX_EN  10
#define IWRAP_COMMAND_SET_CONTROL_MUX_DIS 11
#define IWRAP_COMMAND_SET_PROFILE   4
#define IWRAP_COMMAND_SET_RESET     5

class iWRAPAddress {
    public:
        void parseHexAddress(const char *str);
        void clear();
        uint8_t address[6];
};

class iWRAPClass {
    public:
        uint8_t device_class[3];
};

class iWRAPLink {
    public:
        uint8_t link_id;
        uint8_t mode;
        uint8_t blocksize;
        uint32_t elapse_time;
        uint8_t local_msc;
        uint8_t remote_msc;
        iWRAPAddress addr;
        uint8_t channel;
        uint8_t direction;
        uint8_t powermode;
        uint8_t role;
        uint8_t crypt;
        uint16_t buffer;
        bool eretx;
};

class iWRAPConfig {
    public:
        iWRAPConfig();
        void clear();

        // ===========================
        // SET BT variables
        // ===========================

        // SET BT AUTH
        uint8_t authMode;
        char *authPinCode;
        
        // SET BT BDADDR
        iWRAPAddress btAddress;
        
        // SET BT CLASS
        uint16_t btClass;
        
        // SET BT IDENT
        uint8_t btIdentSource;
        uint8_t btIdentVendorID;
        uint8_t btIdentProductID;
        uint8_t btIdentVersionMajor;
        uint8_t btIdentVersionMinor;
        uint8_t btIdentVersionRevision;
        char *btIdentDescription;
        
        // SET BT LAP
        uint32_t btLAPIAC;
        
        // SET BT MTU
        uint16_t btMTU;
        
        // SET BT NAME
        char *btName; // max length = 256

        // SET BT PAIRCOUNT
        uint8_t btMaxPairings;
        
        // SET BT PAGEMODE
        uint8_t btPageMode;
        uint16_t btPageTimeout;
        uint8_t btPageScanMode;
        
        // SET BT PAIR (multiple entries)
        uint8_t btPairCount;
        iWRAPAddress *btPairAddress[16];
        char *btPairKey[16]; // 32 bytes each
        
        // SET BT POWER
        uint8_t btPowerDefault;
        uint8_t btPowerMaximum;
        uint8_t btPowerInquiry;
        
        // SET BT ROLE
        uint8_t btRoleMSPolicy;
        uint8_t btRoleLinkPolicy;
        uint16_t btRoleSupervisionTimeout;
        
        // SET BT SNIFF
        bool btSniffEnabled;
        uint16_t btSniffMax;
        uint16_t btSniffMin;
        uint16_t btSniffAttempt;
        uint16_t btSniffTimeout;
        
        // SET BT SSP
        uint8_t btSSPCapabilities;
        bool btSSPMITMEnabled;

        // ===========================
        // SET CONTROL variables
        // ===========================

        // SET CONTROL AUDIO (WT32 only)
        char *audioSCORouting;
        char *audioA2DPRouting;
        bool audioMultiSCOEnabled;
        bool audioEventsEnabled;
        
        // SET CONTROL AUTOCALL
        bool autocallEnabled;
        char *autocallTarget;
        uint16_t autocallTimeout;
        char *autocallProfile;
        
        // SET CONTROL BATTERY (WT32 only)
        bool batteryEnabled;
        uint16_t batteryLow;
        uint16_t batteryShutdown;
        uint16_t batteryFull;
        uint16_t batteryMask;

        // SET CONTROL BAUD
        uint32_t uartBaudRate;
        uint8_t uartDataBits;
        uint8_t uartStopBits;
        uint8_t uartParity;
        
        // SET CONTROL BIND - multiple entries
        uint8_t bindEntries;
        uint8_t bindPriority[8];
        uint16_t bindIOMask[8];
        uint8_t bindDirection[8];
        char *bindCommand[8]; // max length = 31
        
        // SET CONTROL CD
        uint16_t carrierDetectMask;
        uint8_t carrierDetectDataMode;
        
        // SET CONTROL CODEC
        uint8_t codec;
        uint8_t codecChannelMode;
        uint16_t codecSamplingRate;
        uint8_t codecPriority;
        
        // SET CONTROL CONFIG
        uint16_t config_block;
        uint16_t optional_block_1;
        uint16_t optional_block_2;
        
        // SET CONTROL ECHO
        uint8_t echoMask;
        
        // SET CONTROL ESCAPE
        uint8_t escapeCharacter;
        uint16_t escapeDTRMask;
        uint8_t escapeDTRMode;
        
        // SET CONTROL GAIN
        uint8_t gainDefaultInput;
        uint8_t gainDefaultOutput;
        
        // SET CONTROL INIT
        char *initCommand;
        
        // SET CONTROL MICBIAS
        uint8_t micBiasVoltage;
        uint8_t micBiasCurrent;
        
        // SET CONTROL MUX
        bool muxModeEnabled;
        bool commandModeEnabled;

        // SET CONTROL MSC
        uint8_t mscMode;
        uint16_t mscDSRMask;
        uint16_t mscDTRMask;
        uint16_t mscRTSMask;
        uint16_t mscCTSMask;
        uint16_t mscRIMask;
        uint16_t mscDCDMask;
        
        // SET CONTROL PCM
        uint32_t pcmConfig;
        uint16_t pcmData;
        
        // SET CONTROL PREAMP
        bool preamp20LeftEnabled;
        bool preamp20RightEnabled;
        
        // SET CONTROL RINGTONE
        char *ringtone;
        
        // SET CONTROL READY
        uint16_t readyMask;
        
        // SET CONTROL VREGEN (WT32 only)
        uint8_t vRegMode;
        uint16_t vRegMask;

        // ===========================
        // SET PROFILE variables
        // ===========================
        bool profileSPPEnabled;
        bool profileHFPEnabled;
        bool profileHFPAGEnabled;
        bool profileOPPEnabled;
        bool profileA2DPSinkEnabled;
        bool profileA2DPSourceEnabled;
        bool profileAVRCPControllerEnabled;
        bool profileAVRCPTargetEnabled;
        bool profileHIDEnabled;
        bool profileHSPEnabled;
        bool profileHSPAGEnabled;
        bool profileHDPSinkEnabled;
        bool profileHDPSourceEnabled;
        bool profilePBAPEnabled;
        bool profileBGIOEnabled;
        bool profileOTAEnabled;
        uint16_t profileHDPSinkMDEP;
        uint16_t profileHDPSourceMDEP;
        char *profileSPPName; // names may be up to 49 characters
        char *profileHFPName;
        char *profileHFPAGName;
        char *profileHIDName;
};

class iWRAP {
    public:
        iWRAP(HardwareSerial *module=0, HardwareSerial *output=0);

        bool checkActivity(uint16_t timeout=0);
        bool checkError();
        bool checkTimeout();
        void setBusy(bool busyEnabled);

        // set/update UART port objects
        void setModuleUART(HardwareSerial *module);
        void setOutputUART(HardwareSerial *debug);
        
        // control module-to-output port echoing
        void setEchoModuleOutput(bool enabled);

        // read all settings into local data structures
        void readDeviceConfig();
        void readLinkConfig();

        // get/set device mode
        uint8_t getDeviceMode();
        void setDeviceMode(uint8_t mode);

        // send plain command/data or use MUX-mode protocol if necessary
        void smartSend(const char *command, iWRAPLink *link=0);

        // events
        void onA2DPCodec(void (*function) ());         // 5.55 SET CONTROL AUDIO
        void onAudioRoute(void (*function) ());        // 5.55 SET CONTROL AUDIO
        void onAuth(void (*function) ());              // 5.13 CALL, 5.28 PAIR
        void onBatteryFull(void (*function) ());       // 5.57 SET CONTROL BATTERY
        void onBatteryLow(void (*function) ());        // 5.57 SET CONTROL BATTERY
        void onBatteryShutdown(void (*function) ());   // 5.57 SET CONTROL BATTERY
        void onClock(void (*function) ());             // 5.13 CALL, 5.14 CLOCK
        void onConnAuth(void (*function) ());          // 5.16 CONNAUTH
        void onConnect(void (*function) ());           // 5.13 CALL, 5.36 SCO OPEN
        void onErrMaxPaircount(void (*function) ());   // 5.48 SET BT PAIRCOUNT
        void onIdent(void (*function) ());             // 5.22 IDENT
        void onIdentError(void (*function) ());        // 5.22 IDENT
        void onInquiryPartial(void (*function) ());    // 5.20 INQUIRY
        void onInquiryExtended(void (*function) ());   // 5.20 INQUIRY
        void onName(void (*function) ());              // 5.20 INQUIRY
        void onNameError(void (*function) ());         // 5.20 INQUIRY
        void onNoCarrier(void (*function) ());         // 5.13 CALL, 5.24 KILL, 5.36 SCO OPEN
        void onOBEXAuth(void (*function) ());          // 5.93 PBAP
        void onPair(void (*function) ());              // 5.6 AUTH, 5.13 CALL, 5.28 PAIR
        void onReady(void (*function) ());             // Power-on
        void onRing(void (*function) ());              // Power-on
        void onSyntaxError(void (*function) ());       // 5.14 CLOCK, 5.25 L2CAP, 5.28 PAIR, 5.39 SELECT, 5.43 SET BT CLASS, 5.44 SET BT IDENT, 5.45 SET BT LAP, 5.46 SET BT MTU, 5.47 SET BT NAME, 5.48 SET BT PAIRCOUNT
        void onVolume(void (*function) ());            // 5.94 VOLUME
        void onBusy(void (*function) ());              // special "busy" event from iWRAP class
        void onIdle(void (*function) ());              // special "idle" event from iWRAP class
        void onTimeout(void (*function) ());           // special "timeout" event from iWRAP class

        // 5.3, @
        void sendParserCommand(iWRAPLink *link, const char *command);

        // 5.4, AIO
        uint16_t getAIOValue(uint8_t source);

        // 5.5, AT
        void testConnection();

        // 5.6, AUTH
        void acceptAuthentication(uint8_t *pin_code);
        void rejectAuthentication();

        // 5.7, AVRCP PDU
        // TODO: not implemented

        // 5.8, BATTERY
        uint16_t getBatteryVoltage();

        // 5.9, BSCP_ENABLE
        void enableBCSP();

        // 5.10, BER
        float getBitErrorRate(iWRAPLink *link);

        // 5.11, BOOT
        void setBootMode(uint16_t boot_mode);

        // 5.12, BYPASSUART
        void enableBypassUART();

        // 5.13, CALL
        void call(iWRAPAddress *address, uint8_t *target, uint8_t *connect_mode, uint8_t packet_size=0);

        // 5.14, CLOCK
        uint32_t clock(iWRAPLink *link);
        
        // 5.15, CLOSE
        void close(iWRAPLink *link);

        // 5.16, CONNAUTH
        void acceptConnAuth(iWRAPAddress *bd_addr, uint8_t protocol_id, uint8_t channel_id);
        void rejectConnAuth(iWRAPAddress *bd_addr, uint8_t protocol_id, uint8_t channel_id);

        // 5.17, CONNECT
        void connectLinks(iWRAPLink *link1, iWRAPLink *link2);

        // 5.18, ECHO
        void echo(iWRAPLink *link1, const uint8_t *string);

        // 5.19, DEFRAG
        void defrag();
        
        // 5.20, INQUIRY
        void discover(uint8_t timeout, bool friendlyNames=false, uint32_t lap=0);
        
        // 5.21, IC
        void cancelDiscovery();
        
        // 5.22, IDENT
        void identify(iWRAPAddress *bd_addr);
        
        // 5.23, INFO
        void getInfo(uint8_t extended=0);

        // 5.24, KILL
        void killConnection(iWRAPAddress *bd_addr, uint16_t reason=0);
        
        // 5.25, L2CAP
        void createL2CAP(uint8_t psm);
        
        // 5.26, LIST
        uint8_t listConnections();
        
        // 5.27, NAME
        bool getFriendlyName(iWRAPAddress *bd_addr, uint16_t *error=0, const uint8_t *reason=0);

        // 5.28, PAIR
        bool pair(iWRAPAddress *bd_addr);
        
        // 5.29, PING
        uint16_t ping(iWRAPLink *link);
        
        // 5.30, PIO
        uint8_t pinRead(uint8_t pin);
        uint16_t readPins();
        void pinWrite(uint8_t pin, uint8_t value);
        void writePins(uint16_t values);
        uint8_t getPinDirection(uint8_t pin);
        uint16_t getPinDirections();
        void setPinDirection(uint8_t pin, uint8_t direction);
        void setPinDirections(uint16_t directions);
        uint8_t getPinBias(uint8_t pin);
        uint16_t getPinBiases();
        void setPinBias(uint8_t pin, uint8_t bias);
        void setPinBiases(uint16_t biases);
        void resetPins();
        
        // 5.31, PLAY
        void play(uint8_t *string);

        // 5.32, RFCOMM
        uint8_t createRFComm();

        // 5.33, RESET
        void resetDevice();

        // 5.34, RSSI
        int8_t getSignalStrength(iWRAPLink *link);
        
        // 5.35, SCO ENABLE
        void scoEnable();

        // 5.36, SCO OPEN
        void scoOpen(iWRAPLink *link);

        // 5.37, SDP
        void discoverServices(iWRAPAddress *bd_addr, const uint8_t *uuid, bool all=false);

        // 5.38, SDP ADD
        void addService(const uint8_t *uuid, const uint8_t *name);
        
        // 5.39, SELECT
        void selectDataMode(iWRAPLink *link);
        
        // 5.40, SET
        void getSettings();
        
        // 5.41, SET BT AUTH
        void getPinCode(uint8_t *pin_code);
        void setPinCode(const uint8_t *pin_code, bool hidden=false);
        void clearPinCode();

        // 5.42, SET BT BDADDR
        iWRAPAddress getDeviceAddress();

        // 5.43, SET BT CLASS
        iWRAPClass getDeviceClass();
        void setDeviceClass(iWRAPClass *class_of_device);
        void setDeviceClass(uint32_t class_of_device);
        
        // 5.44, SET BT IDENT
        void setDeviceIdentity(uint8_t *descr);

        // 5.45, SET BT LAP
        void setDeviceLAP(uint32_t iac);
        
        // 5.46, SET BT MTU
        void setDeviceMTU(uint16_t mtu);

        // 5.47, SET BT NAME
        void setDeviceName(uint8_t *friendly_name);
        
        // 5.48, SET BT PAIRCOUNT
        void getDevicePairLimit();
        void setDevicePairLimit(uint8_t max_pairings);

        // 5.49, SET BT PAGEMODE
        void getDevicePageMode();
        void setDevicePageMode(uint8_t page_mode, uint16_t page_timeout, uint8_t page_scan_mode);

        // 5.50, SET BT PAIR
        void getPairings();
        void addPairing(iWRAPAddress *bd_addr, const uint8_t *link_key);
        void clearPairings();

        // 5.51, SET BT POWER
        void getDevicePower(int8_t *def, int8_t *maximum, int8_t *inquiry);
        void setDevicePower(int8_t def, int8_t maximum, int8_t inquiry=0);
        void setDevicePowerClass(uint8_t power_class);

        // 5.52, SET BT ROLE
        void getDeviceRole(int8_t *ms_policy, uint8_t *link_policy, uint16_t *supervision_timeout);
        void setDeviceRole(int8_t ms_policy, uint8_t link_policy, uint16_t supervision_timeout);
        void resetDeviceRole();

        // 5.53, SET BT SNIFF
        void getDeviceSniff();
        void setDeviceSniff(uint16_t max, uint16_t min, uint16_t attempt=0, uint16_t timeout=8);
        void disableDeviceSniff();

        // 5.54, SET BT SSP
        void getDeviceSSP();
        void setDeviceSSP(uint8_t capabilities, uint8_t mitm);
        
        // 5.55, SET CONTROL AUDIO
        void getControlAudio();
        void setControlAudio(const uint8_t *sco_routing, const uint8_t *a2dp_routing, uint8_t multisco=0, uint8_t event=0);

        // 5.56, SET CONTROL AUTOCALL
        void getControlAutoCall();
        void setControlAutoCall(const uint8_t *target, uint16_t timeout, const uint8_t *profile);
        
        // 5.57, SET CONTROL BATTERY
        void getControlBattery();
        void setControlBattery(uint16_t low, uint16_t shutdown, uint16_t full, uint16_t mask);
        
        // 5.58, SET CONTROL BAUD
        void getUARTConfig();
        void setUARTConfig(uint32_t baud_rate, uint8_t parity, uint8_t stop_bits);
        
        // 5.59, SET CONTROL BIND
        void bindPIOCommand(uint8_t priority, uint16_t io_mask, uint8_t direction, const uint8_t *command);
        void clearPIOCommand(uint8_t priority);
        
        // 5.60, SET CONTROL CD
        void getCarrierDetect();
        void setCarrierDetect(uint16_t cd_mask, uint8_t datamode);

        // 5.61, SET CONTROL CODEC
        void getAudioCodec();
        void setAudioCodec(uint8_t codec, uint8_t channel_mode, uint16_t sampling_rate, uint8_t priority);
        
        // 5.62, SET CONTROL CONFIG
        void getControlConfig(uint16_t *optional_block_2, uint16_t *optional_block_1, uint16_t config_block);
        void setControlConfig(uint16_t config_block);
        void setControlConfig(uint16_t optional_block_1, uint16_t config_block);
        void setControlConfig(uint16_t optional_block_2, uint16_t optional_block_1, uint16_t config_block);
        void commitConfig();

        // config_block
        bool getConfigRSSI();
        void setConfigRSSI(bool enabled);
        bool getConfigClockCaching();
        void setConfigClockCaching(bool enabled);
        bool getConfigInterlacedInquiryScan();
        void setConfigInterlacedInquiryScan(bool enabled);
        bool getConfigInterlacedPageScan();
        void setConfigInterlacedPageScan(bool enabled);
        bool getConfigDeepSleep();
        void setConfigDeepSleep(bool enabled);
        bool getConfigAddressInConnect();
        void setConfigAddressInConnect(bool enabled);
        bool getConfigPairEvent();
        void setConfigPairEvent(bool enabled);
        bool getConfigSCOLinks();
        void setConfigSCOLinks(bool enabled);
        bool getConfigInteractivePairing();
        void setConfigInteractivePairing(bool enabled);
        bool getConfigRecyclePairings();
        void setConfigRecyclePairings(bool enabled);
        bool getConfigClockEvent();
        void setConfigClockEvent(bool enabled);
        bool getConfigUARTLowLatency();
        void setConfigUARTLowLatency(bool enabled);
        bool getConfigLowInquiryPriority();
        void setConfigLowInquiryPriority(bool enabled);

        // optional_block_1
        bool getConfigLocked();
        void setConfigLocked(bool enabled); // DANGEROUS! THIS ONLY GOES ONE WAY UNLESS YOU HAVE AN SPI HARDWARE INTERFACE HANDY TO UNDO IT!
        bool getConfigEnhancedInquiryResponse();
        void setConfigEnhancedInquiryResponse(bool enabled);
        bool getConfigAutoSCOLinks();
        void setConfigAutoSCOLinks(bool enabled);
        bool getConfigAutoSupervisionTimeoutAfterSwitch();
        void setConfigAutoSupervisionTimeoutAfterSwitch(bool enabled);
        bool getConfigConnAuthEvent();
        void setConfigConnAuthEvent(bool enabled);
        bool getConfigAutoRFCOMMDataMode();
        void setConfigAutoRFCOMMDataMode(bool enabled);
        bool getConfigOKResponse();
        void setConfigOKResponse(bool enabled);
        bool getConfigNoAutoHFPErrors();
        void setConfigNoAutoHFPErrors(bool enabled);

        // optional_block_2 has no allowed/unreserved bits in iWRAP4
        
        // 5.63, SET CONTROL ECHO
        void getControlEcho();
        void setControlEcho(uint8_t echo_mask);

        bool getEchoStartupBanner();
        void setEchoStartupBanner(bool enabled);
        bool getEchoLocal();
        void setEchoLocal(bool enabled);
        bool getEchoCommandSetEvents();
        void setEchoCommandSetEvents(bool enabled);
        
        // 5.64, SET CONTROL ESCAPE
        void getControlEscape();
        void setControlEscape(uint8_t esc_char, uint16_t dtr_mask, uint8_t dtr_mode);
        void disableEscapeSequence();
        
        // 5.65, SET CONTROL GAIN
        void getControlGain();
        void setControlGain(uint8_t input, uint8_t output, uint8_t def=0);
        
        // 5.66, SET CONTROL INIT
        void getControlInit();
        void setControlInit(const uint8_t *command);
        void clearControlInit();

        // 5.67, SET CONTROL MICBIAS
        void getControlMicBias();
        void setControlMicBias(uint8_t voltage, uint8_t current);

        // 5.68, SET CONTROL MUX
        bool getControlMuxEnabled();
        void setControlMuxEnabled(bool enabled);
        void forceControlMuxEnabled(bool enabled);

        // 5.69, SET CONTROL MSC
        void getControlMSC();
        void setControlMSC(uint8_t mode, uint16_t dsr=0xFFFF, uint16_t dtr=0xFFFF, uint16_t rts=0xFFFF, uint16_t cts=0xFFFF, uint16_t ri=0xFFFF, uint16_t dcd=0xFFFF);

        // 5.70, SET CONTROL PCM
        void getControlPCM();
        void setControlPCM(const uint8_t *config, uint16_t data);

        // 5.71, SET CONTROL PREAMP
        void getControlPreamp();
        void setControlPreamp(bool left, bool right);

        // 5.72, SET CONTROL RINGTONE
        void getControlRingtone();
        void setControlRingtone(const uint8_t *ringtone);

        // 5.73, SET CONTROL READY
        void getControlReady();
        void setControlReady(uint16_t piomask);

        // 5.74, SET CONTROL VREGEN
        void getControlVRegEnabled();
        void setControlVRegEnabled(uint8_t mode, uint16_t piomask);
        
        // 5.75, SET LINK ACTIVE
        void setLinkActive(iWRAPLink *link);

        // 5.76, SET LINK MASTER
        void setLinkMaster(iWRAPLink *link);

        // 5.77, SET LINK SLAVE
        void setLinkSlave(iWRAPLink *link);

        // 5.78, SET LINK SNIFF
        void setLinkSniff(iWRAPLink *link, uint16_t max, uint16_t min, uint16_t attempt=0, uint16_t timeout=8);
        void setLinkSniff(iWRAPLink *link, uint16_t avg);

        // 5.79, SET LINK SUBRATE
        void setLinkSubrate(iWRAPLink *link, uint16_t remote_latency, uint16_t remote_timeout, uint16_t local_latency);

        // 5.80, SET LINK MSC
        void setLinkModemStatus(iWRAPLink *link, uint16_t status);

        // 5.81, SET LINK SELECT
        void setLinkSelect(iWRAPLink *link);

        // 5.82, SET PROFILE
        void setProfile(const char *profile, const char *sdp_name=0);
        void disableProfile(const char *profile);

        // 5.83, SET RESET
        void resetSettings();

        // 5.84, SLEEP
        void sleep();

        // 5.85, SSP CONFIRM
        void confirmSSP(iWRAPAddress *bd_addr);
        void rejectSSP(iWRAPAddress *bd_addr);

        // 5.86, SSP PASSKEY
        void confirmSSPPasskey(iWRAPAddress *bd_addr, const char *pass_key);
        void rejectSSPPasskey(iWRAPAddress *bd_addr);

        // 5.87, SSP GETOOB
        void getSSPOutOfBandPairingKey();

        // 5.88, SSP GETOOB
        void setSSPOutOfBandPairingKey(const char *key1, const char *key2);
        
        // 5.89, TEMP
        void getTemperature();

        // 5.90, TEST
        void test(const char *mode, const char *mode_specific_parameters=0);

        // 5.91, TESTMODE
        void enableTestMode();
        
        // 5.92, TXPOWER
        void getTXPower(iWRAPLink *link);
        
        // 5.93, PBAP
        void getPhoneBookSize(uint8_t store, uint8_t type);
        void getPhoneBookEntries(uint8_t store, uint8_t type, uint16_t count, uint16_t offset=0, uint32_t filter=0, uint8_t format=0xFF);
        
        // 5.93, VOLUME
        void getVolumeLevel();
        void setVolumeLevel(uint8_t level);
        void incrementVolumeLevel();
        void decrementVolumeLevel();

        // methods for imitating Teensy++ simple HID kb/mouse methods
        void parse(uint8_t ch);
        void set_modifier(uint8_t c);
        void set_key1(uint8_t c);
        void set_key2(uint8_t c);
        void set_key3(uint8_t c);
        void set_key4(uint8_t c);
        void set_key5(uint8_t c);
        void set_key6(uint8_t c);
        void send_now();
        void set_buttons(uint8_t left, uint8_t middle, uint8_t right);
        void move(uint8_t x, uint8_t y);
        void move(uint8_t x, uint8_t y, uint8_t z);
        void scroll(uint8_t s);

        iWRAPConfig config;

    private:

        bool busy;
        uint8_t lines;
        uint8_t lastCommand;
        uint32_t timeoutStart;
        bool lastError;
        bool lastTimeout;
        bool firstDataRead;
        bool responseEnded;
        uint16_t responseLength;
        uint16_t muxBlockOffset;

        // variables for imitating Teensy simple HID kb/mouse methods
        uint8_t mouse_buttons;
        uint8_t keyboard_keys[6];
        uint8_t keyboard_modifier_keys;

        HardwareSerial *uModule; // required UART object with module connection
        HardwareSerial *uOutput; // optional UART object for host/debug connection
        bool echoModuleOutput;
        char *buffer;
        uint16_t bufferSize;
        uint16_t bufferPos;

        // event function pointers
        void (*funcA2DPCodec) ();         // 5.55 SET CONTROL AUDIO
        void (*funcAudioRoute) ();        // 5.55 SET CONTROL AUDIO
        void (*funcAuth) ();              // 5.13 CALL, 5.28 PAIR
        void (*funcBatteryFull) ();       // 5.57 SET CONTROL BATTERY
        void (*funcBatteryLow) ();        // 5.57 SET CONTROL BATTERY
        void (*funcBatteryShutdown) ();   // 5.57 SET CONTROL BATTERY
        void (*funcClock) ();             // 5.13 CALL, 5.14 CLOCK
        void (*funcConnAuth) ();          // 5.16 CONNAUTH
        void (*funcConnect) ();           // 5.13 CALL, 5.36 SCO OPEN
        void (*funcErrMaxPaircount) ();   // 5.48 SET BT PAIRCOUNT
        void (*funcIdent) ();             // 5.22 IDENT
        void (*funcIdentError) ();        // 5.22 IDENT
        void (*funcInquiryPartial) ();    // 5.20 INQUIRY
        void (*funcInquiryExtended) ();   // 5.20 INQUIRY
        void (*funcName) ();              // 5.20 INQUIRY
        void (*funcNameError) ();         // 5.20 INQUIRY
        void (*funcNoCarrier) ();         // 5.13 CALL, 5.24 KILL, 5.36 SCO OPEN
        void (*funcOBEXAuth) ();          // 5.93 PBAP
        void (*funcPair) ();              // 5.6 AUTH, 5.13 CALL, 5.28 PAIR
        void (*funcReady) ();             // Power-on
        void (*funcRing) ();              // Power-on
        void (*funcSyntaxError) ();       // 5.14 CLOCK, 5.25 L2CAP, 5.28 PAIR, 5.39 SELECT, 5.43 SET BT CLASS, 5.44 SET BT IDENT, 5.45 SET BT LAP, 5.46 SET BT MTU, 5.47 SET BT NAME, 5.48 SET BT PAIRCOUNT
        void (*funcVolume) ();            // 5.94 VOLUME
        void (*funcBusy) ();              // special "busy" event from iWRAP class
        void (*funcIdle) ();              // special "idle" event from iWRAP class
        void (*funcTimeout) ();           // special "timeout" event from iWRAP class
};

#endif /* _IWRAP_H_ */
