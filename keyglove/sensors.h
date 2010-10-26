// Keyglove controller source code - sensor constant definition file
// 10/1/2010 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2010 Jeff Rowberg

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

// === BASE SENSOR PIN NUMBERS (MICROPROCESSOR I/O PIN ASSIGNMENTS)
// These designate which sensors are plugged into which I/O pins

// KSP is Keyglove Sensor Pin
#define KSP_TOTAL_SENSORS 34

#define KSP_A 18
#define KSP_B 19
#define KSP_C 22
#define KSP_D 23
#define KSP_E 24
#define KSP_F 25
#define KSP_G 26
#define KSP_H 27
#define KSP_I 28
#define KSP_J 29
#define KSP_K 30
#define KSP_L 31
#define KSP_M 32
#define KSP_N 33
#define KSP_O 34
#define KSP_P 35
#define KSP_Q 36
#define KSP_R 37
#define KSP_S 38
#define KSP_T 39
#define KSP_U 40
#define KSP_V 41
#define KSP_W 42
#define KSP_X 43
#define KSP_Y 44
#define KSP_Z 45
#define KSP_1 46
#define KSP_2 47
#define KSP_3 48
#define KSP_4 49
#define KSP_5 50
#define KSP_6 51
#define KSP_7 52
#define KSP_8 53

// === BASE 2-SENSOR COMBINATION ARRAY INDEXES
// These are used to reference to correct base combination bit position
// in the two 32-bit integers that record each sensor combination status

// KSI is Keyglove Sensor Index
#define KSI_TOTAL_BITS 59

#define KSI_AY 0
#define KSI_A1 1
#define KSI_A2 2
#define KSI_A3 3
#define KSI_A8 4
#define KSI_BY 5
#define KSI_CY 6
#define KSI_MY 7
#define KSI_MZ 8
#define KSI_NY 9
#define KSI_NZ 10
#define KSI_OY 11
#define KSI_OZ 12
#define KSI_Y4 13

// middle finger
#define KSI_DM 14
#define KSI_DY 15
#define KSI_D1 16
#define KSI_D2 17
#define KSI_D3 18
#define KSI_D4 19
#define KSI_D6 20
#define KSI_D8 21
#define KSI_EY 22
#define KSI_FY 23
#define KSI_PY 24
#define KSI_PZ 25
#define KSI_QY 26
#define KSI_QZ 27
#define KSI_RY 28
#define KSI_RZ 29
#define KSI_Y5 30

// ring finger
#define KSI_GY 31
#define KSI_G1 0
#define KSI_G2 1
#define KSI_G3 2
#define KSI_G7 3
#define KSI_G8 4
#define KSI_HY 5
#define KSI_IY 6
#define KSI_SY 7
#define KSI_TY 8
#define KSI_UY 9
#define KSI_Y6 10

// little finger
#define KSI_JY 11
#define KSI_J1 12
#define KSI_J2 13
#define KSI_J3 14
#define KSI_J8 15
#define KSI_KY 16
#define KSI_LY 17
#define KSI_VY 18
#define KSI_WY 19
#define KSI_XY 20
#define KSI_Y7 21

// thumbtip / top palm
#define KSI_Y1 22

// thumb segment / fingernail
#define KSI_Z4 23
#define KSI_Z5 24
#define KSI_Z6 25
#define KSI_Z7 26

// === 2-SENSOR COMBINATIONS
// These are used to provide the base combination bitRead() method
// calls which are used directly for 1-to-1 combinations and indirectly
// for multi combinations

// index finger
#define KG_AY bitRead(sensors1, KSI_AY)
#define KG_A1 bitRead(sensors1, KSI_A1)
#define KG_A2 bitRead(sensors1, KSI_A2)
#define KG_A3 bitRead(sensors1, KSI_A3)
#define KG_A8 bitRead(sensors1, KSI_A8)
#define KG_BY bitRead(sensors1, KSI_BY)
#define KG_CY bitRead(sensors1, KSI_CY)
#define KG_MY bitRead(sensors1, KSI_MY)
#define KG_MZ bitRead(sensors1, KSI_MY)
#define KG_NY bitRead(sensors1, KSI_NY)
#define KG_NZ bitRead(sensors1, KSI_NY)
#define KG_OY bitRead(sensors1, KSI_OY)
#define KG_OZ bitRead(sensors1, KSI_OY)
#define KG_Y4 bitRead(sensors1, KSI_Y4)

// middle finger
#define KG_DM bitRead(sensors1, KSI_DM)
#define KG_DY bitRead(sensors1, KSI_DY)
#define KG_D1 bitRead(sensors1, KSI_D1)
#define KG_D2 bitRead(sensors1, KSI_D2)
#define KG_D3 bitRead(sensors1, KSI_D3)
#define KG_D4 bitRead(sensors1, KSI_D4)
#define KG_D6 bitRead(sensors1, KSI_D6)
#define KG_D8 bitRead(sensors1, KSI_D8)
#define KG_EY bitRead(sensors1, KSI_EY)
#define KG_FY bitRead(sensors1, KSI_FY)
#define KG_PY bitRead(sensors1, KSI_PY)
#define KG_PZ bitRead(sensors1, KSI_PZ)
#define KG_QY bitRead(sensors1, KSI_QY)
#define KG_QZ bitRead(sensors1, KSI_QZ)
#define KG_RY bitRead(sensors1, KSI_RY)
#define KG_RZ bitRead(sensors1, KSI_RZ)
#define KG_Y5 bitRead(sensors1, KSI_Y5)

// ring finger
#define KG_GY bitRead(sensors1, KSI_GY)
// (we run out of the first 32 bits at this point)
#define KG_G1 bitRead(sensors2, KSI_G1)
#define KG_G2 bitRead(sensors2, KSI_G2)
#define KG_G3 bitRead(sensors2, KSI_G3)
#define KG_G7 bitRead(sensors2, KSI_G7)
#define KG_G8 bitRead(sensors2, KSI_G8)
#define KG_HY bitRead(sensors2, KSI_HY)
#define KG_IY bitRead(sensors2, KSI_IY)
#define KG_SY bitRead(sensors2, KSI_SY)
#define KG_TY bitRead(sensors2, KSI_TY)
#define KG_UY bitRead(sensors2, KSI_UY)
#define KG_Y6 bitRead(sensors2, KSI_Y6)

// little finger
#define KG_JY bitRead(sensors2, KSI_JY)
#define KG_J1 bitRead(sensors2, KSI_JY)
#define KG_J2 bitRead(sensors2, KSI_JY)
#define KG_J3 bitRead(sensors2, KSI_JY)
#define KG_J8 bitRead(sensors2, KSI_J8)
#define KG_KY bitRead(sensors2, KSI_KY)
#define KG_LY bitRead(sensors2, KSI_LY)
#define KG_VY bitRead(sensors2, KSI_VY)
#define KG_WY bitRead(sensors2, KSI_WY)
#define KG_XY bitRead(sensors2, KSI_XY)
#define KG_Y7 bitRead(sensors2, KSI_Y7)

// thumbtip / top palm
#define KG_Y1 bitRead(sensors2, KSI_Y1)

// thumb segment / fingernail
#define KG_Z4 bitRead(sensors2, KSI_Z4)
#define KG_Z5 bitRead(sensors2, KSI_Z5)
#define KG_Z6 bitRead(sensors2, KSI_Z6)
#define KG_Z7 bitRead(sensors2, KSI_Z7)

// === 3-SENSOR COMBINATIONS

// fingertip / thumb tip
#define KG_ADY       KG_AY && KG_DY
#define KG_AGY       KG_AY && KG_GY
#define KG_AJY       KG_AY && KG_JY
#define KG_AY1       KG_AY && KG_Y1
#define KG_DGY       KG_DY && KG_GY
#define KG_DJY       KG_DY && KG_JY
#define KG_DY1       KG_DY && KG_Y1
#define KG_GJY       KG_GY && KG_JY
#define KG_GY1       KG_GY && KG_Y1
#define KG_JY1       KG_JY && KG_Y1

// thumbtip / fingernail
#define KG_Y45       KG_Y4 && KG_Y5
#define KG_Y46       KG_Y4 && KG_Y6
#define KG_Y47       KG_Y4 && KG_Y7
#define KG_Y56       KG_Y5 && KG_Y6
#define KG_Y57       KG_Y5 && KG_Y7
#define KG_Y67       KG_Y6 && KG_Y7

// thumb segment / fingernail
#define KG_Z45       KG_Z4 && KG_Z5
#define KG_Z46       KG_Z4 && KG_Z6
#define KG_Z47       KG_Z4 && KG_Z7
#define KG_Z56       KG_Z5 && KG_Z6
#define KG_Z57       KG_Z5 && KG_Z7
#define KG_Z67       KG_Z6 && KG_Z7

// fingertip / thumb nail
#define KG_AD8       KG_A8 && KG_DY
#define KG_AG8       KG_A8 && KG_G8
#define KG_AJ8       KG_A8 && KG_J8
#define KG_DG8       KG_D8 && KG_G8
#define KG_DJ8       KG_D8 && KG_J8
#define KG_GJ8       KG_G8 && KG_J8

// fingertip / top pad
#define KG_AD1       KG_A1 && KG_D1
#define KG_AG1       KG_A1 && KG_G1
#define KG_AJ1       KG_A1 && KG_J1
#define KG_DG1       KG_D1 && KG_G1
#define KG_DJ1       KG_D1 && KG_J1
#define KG_GJ1       KG_G1 && KG_J1

// fingertip / mid pad
#define KG_AD2       KG_A2 && KG_D2
#define KG_AG2       KG_A2 && KG_G2
#define KG_AJ2       KG_A2 && KG_J2
#define KG_DG2       KG_D2 && KG_G2
#define KG_DJ2       KG_D2 && KG_J2
#define KG_GJ2       KG_G2 && KG_J2

// fingertip / bottom pad
#define KG_AD3       KG_A3 && KG_D3
#define KG_AG3       KG_A3 && KG_G3
#define KG_AJ3       KG_A3 && KG_J3
#define KG_DG3       KG_D3 && KG_G3
#define KG_DJ3       KG_D3 && KG_J3
#define KG_GJ3       KG_G3 && KG_J3

// === 4-SENSOR COMBINATIONS

// fingertips / thumb tip
#define KG_ADGY      KG_AY && KG_DY && KG_GY
#define KG_ADJY      KG_AY && KG_DY && KG_JY
#define KG_AGJY      KG_AY && KG_GY && KG_JY
#define KG_DGJY      KG_DY && KG_GY && KG_JY

// thumbtip / fingernail
#define KG_Y456      KG_Y4 && KG_Y5 && KG_Y6
#define KG_Y457      KG_Y4 && KG_Y5 && KG_Y7
#define KG_Y467      KG_Y4 && KG_Y6 && KG_Y7
#define KG_Y567      KG_Y5 && KG_Y6 && KG_Y7

// thumb segment / fingernail
#define KG_Z456      KG_Z4 && KG_Z5 && KG_Z6
#define KG_Z457      KG_Z4 && KG_Z5 && KG_Z7
#define KG_Z467      KG_Z4 && KG_Z6 && KG_Z7
#define KG_Z567      KG_Z5 && KG_Z6 && KG_Z7

// fingertips / top pad
#define KG_ADG1      KG_A1 && KG_D1 && KG_G1
#define KG_ADJ1      KG_A1 && KG_D1 && KG_J1
#define KG_AGJ1      KG_A1 && KG_G1 && KG_J1
#define KG_DGJ1      KG_D1 && KG_G1 && KG_J1

// fingertips / mid pad
#define KG_ADG2      KG_A2 && KG_D2 && KG_G2
#define KG_ADJ2      KG_A2 && KG_D2 && KG_J2
#define KG_AGJ2      KG_A2 && KG_G2 && KG_J2
#define KG_DGJ2      KG_D2 && KG_G2 && KG_J2

// fingertips / bottom pad
#define KG_ADG3      KG_A3 && KG_D3 && KG_J3
#define KG_ADJ3      KG_A3 && KG_D3 && KG_J3
#define KG_AGJ3      KG_A3 && KG_G3 && KG_J3
#define KG_DGJ3      KG_D3 && KG_G3 && KG_J3

// === 5-SENSOR COMBINATIONS

// fingertips / thumb tip
#define KG_ADGJY     AY && KG_DY && KG_GY && KG_JY

// fingertips / top pad
#define KG_ADGJ1     A2 && KG_D1 && KG_G1 && KG_J1

// fingertips / mid pad
#define KG_ADGJ2     A2 && KG_D2 && KG_G2 && KG_J2

// fingertips / bottom pad
#define KG_ADGJ3     A3 && KG_D3 && KG_G3 && KG_J3

// === 2/2-SENSOR COMBINATIONS

// fingertip / thumbtip
#define KG_AY_D4 // index tip / thumbtip + middle tip / index nail
#define KG_AY_D6 // index tip / thumbtip + middle tip / ring nail
#define KG_AY_D1 // index tip / thumbtip + middle tip / top palm
#define KG_AY_D2 // index tip / thumbtip + middle tip / middle palm
#define KG_AY_D3 // index tip / thumbtip + middle tip / bottom palm
#define KG_AY_G7 // index tip / thumbtip + ring tip / pinky nail
#define KG_AY_G1 // index tip / thumbtip + ring tip / top palm
#define KG_AY_G2 // index tip / thumbtip + ring tip / middle palm
#define KG_AY_G3 // index tip / thumbtip + ring tip / bottom palm
#define KG_AY_J1 // index tip / thumbtip + pinky tip / top palm
#define KG_AY_J2 // index tip / thumbtip + pinky tip / middle palm
#define KG_AY_J3 // index tip / thumbtip + pinky tip / bottom palm
#define KG_AY_Z5 // index tip / thumbtip + thumb segment / middle tip

#define KG_DY_A1 // middle tip / thumbtip + index tip / top palm
#define KG_DY_G5 // middle tip / thumbtip + ring tip / middle nail
#define KG_DY_G7 // middle tip / thumbtip + ring tip / pinky nail
#define KG_DY_G1 // middle tip / thumbtip + ring tip / top palm
#define KG_DY_G2 // middle tip / thumbtip + ring tip / middle palm
#define KG_DY_G3 // middle tip / thumbtip + ring tip / bottom palm
#define KG_DY_J1 // middle tip / thumbtip + pinky tip / top palm
#define KG_DY_J2 // middle tip / thumbtip + pinky tip / middle palm
#define KG_DY_J3 // middle tip / thumbtip + pinky tip / bottom palm
#define KG_DY_Z4 // middle tip / thumbtip + thumb segment / index nail

#define KG_GY_A1 // ring tip / thumbtip + index tip / top palm
#define KG_GY_D1 // ring tip / thumbtip + middle tip / top palm
#define KG_GY_D4 // ring tip / thumbtip + middle tip / index nail
#define KG_GY_D6 // ring tip / thumbtip + middle tip / ring nail
#define KG_GY_DM // ring tip / thumbtip + middle tip / index side
#define KG_GY_J6 // ring tip / thumbtip + pinky tip / ring nail
#define KG_GY_J1 // ring tip / thumbtip + pinky tip / top palm
#define KG_GY_J2 // ring tip / thumbtip + pinky tip / middle palm
#define KG_GY_J3 // ring tip / thumbtip + pinky tip / bottom palm

#define KG_JY_A1 // pinky tip / thumbtip + index tip / top palm
#define KG_JY_D4 // pinky tip / thumbtip + middle tip / index nail
#define KG_JY_D1 // pinky tip / thumbtip + middle tip / top palm
#define KG_JY_DM // pinky tip / thumbtip + middle tip / index side
#define KG_JY_G1 // pinky tip / thumbtip + ring tip / top palm
#define KG_JY_G7 // pinky tip / thumbtip + ring tip / pinky nail

// thumbtip / fingernail
#define KG_Y4_D1 // thumbtip / index nail + middle tip / top palm
#define KG_Y4_D2 // thumbtip / index nail + middle tip / middle palm
#define KG_Y4_D3 // thumbtip / index nail + middle tip / bottom palm
#define KG_Y4_G1 // thumbtip / index nail + ring tip / top palm
#define KG_Y4_G2 // thumbtip / index nail + ring tip / middle palm
#define KG_Y4_G3 // thumbtip / index nail + ring tip / bottom palm
#define KG_Y4_J1 // thumbtip / index nail + pinky tip / top palm
#define KG_Y4_J2 // thumbtip / index nail + pinky tip / middle palm
#define KG_Y4_J3 // thumbtip / index nail + pinky tip / bottom palm

#define KG_Y5_A1 // thumbtip / middle nail + index tip / top palm
#define KG_Y5_G7 // thumbtip / middle nail + ring tip / pinky nail
#define KG_Y5_G1 // thumbtip / middle nail + ring tip / top palm
#define KG_Y5_G2 // thumbtip / middle nail + ring tip / middle palm
#define KG_Y5_G3 // thumbtip / middle nail + ring tip / bottom palm
#define KG_Y5_J1 // thumbtip / middle nail + pinky tip / top palm
#define KG_Y5_J2 // thumbtip / middle nail + pinky tip / middle palm
#define KG_Y5_J3 // thumbtip / middle nail + pinky tip / bottom palm
#define KG_Y5_Z4 // thumbtip / middle nail + index nail / thumb segment

#define KG_Y6_A1 // thumbtip / ring nail + index tip / top palm
#define KG_Y6_D4 // thumbtip / ring nail + middle tip / index nail
#define KG_Y6_D1 // thumbtip / ring nail + middle tip / top palm
#define KG_Y6_DM // thumbtip / ring nail + middle tip / index side
#define KG_Y6_J1 // thumbtip / ring nail + pinky tip / top palm
#define KG_Y6_J2 // thumbtip / ring nail + pinky tip / middle palm
#define KG_Y6_J3 // thumbtip / ring nail + pinky tip / bottom palm
#define KG_Y6_Z4 // thumbtip / ring nail + index nail / thumb segment

#define KG_Y7_A1 // thumbtip / pinky nail + index tip / top palm
#define KG_Y7_D4 // thumbtip / pinky nail + middle tip / index nail
#define KG_Y7_D6 // thumbtip / pinky nail + middle tip / ring nail
#define KG_Y7_DM // thumbtip / pinky nail + middle tip / index side
#define KG_Y7_D1 // thumbtip / pinky nail + middle tip / top palm
#define KG_Y7_G1 // thumbtip / pinky nail + ring tip / top palm
#define KG_Y7_Z4 // thumbtip / pinky nail + index nail / thumb segment

// thumbtip / top palm
#define KG_Y1_D4
#define KG_Y1_D6
#define KG_Y1_DM
#define KG_Y1_G7
#define KG_Y1_J8

// thumbtip / index side
#define KG_MY_A1
#define KG_MY_A2
#define KG_MY_A3
#define KG_MY_D4
#define KG_MY_D6
#define KG_MY_D1
#define KG_MY_D2
#define KG_MY_D3
#define KG_MY_G7
#define KG_MY_G1
#define KG_MY_G2
#define KG_MY_G3
#define KG_MY_J1
#define KG_MY_J2
#define KG_MY_J3

#define KG_NY_A1
#define KG_NY_A2
#define KG_NY_A3
#define KG_NY_D4
#define KG_NY_D6
#define KG_NY_D1
#define KG_NY_D2
#define KG_NY_D3
#define KG_NY_G7
#define KG_NY_G1
#define KG_NY_G2
#define KG_NY_G3
#define KG_NY_J1
#define KG_NY_J2
#define KG_NY_J3

#define KG_OY_A1
#define KG_OY_A2
#define KG_OY_A3
#define KG_OY_D4
#define KG_OY_D6
#define KG_OY_D1
#define KG_OY_D2
#define KG_OY_D3
#define KG_OY_G7
#define KG_OY_G1
#define KG_OY_G2
#define KG_OY_G3
#define KG_OY_J1
#define KG_OY_J2
#define KG_OY_J3

// thumb segment / index side
#define KG_MZ_D1
#define KG_MZ_D2
#define KG_MZ_D3
#define KG_MZ_G1
#define KG_MZ_G2
#define KG_MZ_G3
#define KG_MZ_J1
#define KG_MZ_J2
#define KG_MZ_J3

#define KG_NZ_D1
#define KG_NZ_D2
#define KG_NZ_D3
#define KG_NZ_G1
#define KG_NZ_G2
#define KG_NZ_G3
#define KG_NZ_J1
#define KG_NZ_J2
#define KG_NZ_J3

#define KG_OZ_D1
#define KG_OZ_D2
#define KG_OZ_D3
#define KG_OZ_G1
#define KG_OZ_G2
#define KG_OZ_G3
#define KG_OZ_J1
#define KG_OZ_J2
#define KG_OZ_J3

// fingertip / thumb nail
#define KG_A8_D1
#define KG_A8_D2
#define KG_A8_D3
#define KG_A8_G1
#define KG_A8_G2
#define KG_A8_G3
#define KG_A8_J1
#define KG_A8_J2
#define KG_A8_J3

#define KG_D8_A1
#define KG_D8_G1
#define KG_D8_G2
#define KG_D8_G3
#define KG_D8_J1
#define KG_D8_J2
#define KG_D8_J3

#define KG_G8_A1
#define KG_G8_D1
#define KG_G8_J1
#define KG_G8_J2
#define KG_G8_J3

#define KG_J8_A1
#define KG_J8_D1
#define KG_J8_G1

// fingertip / top pad
#define KG_A1_D2
#define KG_A1_D3
#define KG_A1_G2
#define KG_A1_G3
#define KG_A1_J2
#define KG_A1_J3

#define KG_D1_A2
#define KG_D1_A3
#define KG_D1_G2
#define KG_D1_G3
#define KG_D1_J2
#define KG_D1_J3

#define KG_G1_A2
#define KG_G1_A3
#define KG_G1_D2
#define KG_G1_D3
#define KG_G1_J2
#define KG_G1_J3

#define KG_J1_A2
#define KG_J1_A3
#define KG_J1_D2
#define KG_J1_D3
#define KG_J1_G2
#define KG_J1_G3

// fingertip / mid pad
#define KG_A2_D1
#define KG_A2_D3
#define KG_A2_G1
#define KG_A2_G3
#define KG_A2_J1
#define KG_A2_J3

#define KG_D2_A1
#define KG_D2_A3
#define KG_D2_G1
#define KG_D2_G3
#define KG_D2_J1
#define KG_D2_J3

#define KG_G2_A1
#define KG_G2_A3
#define KG_G2_D1
#define KG_G2_D3
#define KG_G2_J1
#define KG_G2_J3

#define KG_J2_A1
#define KG_J2_A3
#define KG_J2_D1
#define KG_J2_D3
#define KG_J2_G1
#define KG_J2_G3

// fingertip / bottom pad
#define KG_A3_D1
#define KG_A3_D2
#define KG_A3_G1
#define KG_A3_G2
#define KG_A3_J1
#define KG_A3_J2

#define KG_D3_A1
#define KG_D3_A2
#define KG_D3_G1
#define KG_D3_G2
#define KG_D3_J1
#define KG_D3_J2

#define KG_G3_A1
#define KG_G3_A2
#define KG_G3_D1
#define KG_G3_D2
#define KG_G3_J1
#define KG_G3_J2

#define KG_J3_A1
#define KG_J3_A2
#define KG_J3_D1
#define KG_J3_D2
#define KG_J3_G1
#define KG_J3_G2

// === 2/3-SENSOR COMBINATIONS

// fingertip / thumbtip
#define KG_AY_DG1 // index tip / thumbtip + middle tip / top palm
#define KG_AY_DJ1
#define KG_AY_DG2 // index tip / thumbtip + middle tip / middle palm
#define KG_AY_DJ2
#define KG_AY_DG3 // index tip / thumbtip + middle tip / bottom palm
#define KG_AY_DJ3
#define KG_AY_GJ1 // index tip / thumbtip + ring tip / top palm
#define KG_AY_GJ2 // index tip / thumbtip + ring tip / middle palm
#define KG_AY_GJ3 // index tip / thumbtip + ring tip / bottom palm

#define KG_DY_AG1 // middle tip / thumbtip + index tip / top palm
#define KG_DY_AJ1
#define KG_DY_GJ1 // middle tip / thumbtip + ring tip / top palm
#define KG_DY_GJ2 // middle tip / thumbtip + ring tip / middle palm
#define KG_DY_GJ3 // middle tip / thumbtip + ring tip / bottom palm

#define KG_GY_AD1 // ring tip / thumbtip + index tip / top palm
#define KG_GY_AJ1
#define KG_GY_DG1 // ring tip / thumbtip + middle tip / top palm
#define KG_GY_DJ6 // ring tip / thumbtip + middle tip / ring nail

#define KG_JY_AD1 // pinky tip / thumbtip + index tip / top palm
#define KG_JY_AG1
#define KG_JY_DG7
#define KG_JY_DG1 // pinky tip / thumbtip + middle tip / top palm

// thumbtip / fingernail
#define KG_Y4_DG1 // thumbtip / index nail + middle tip / top palm
#define KG_Y4_DJ1
#define KG_Y4_DG2 // thumbtip / index nail + middle tip / middle palm
#define KG_Y4_DJ2
#define KG_Y4_DG3 // thumbtip / index nail + middle tip / bottom palm
#define KG_Y4_DJ3
#define KG_Y4_GJ1 // thumbtip / index nail + ring tip / top palm
#define KG_Y4_GJ2 // thumbtip / index nail + ring tip / middle palm
#define KG_Y4_GJ3 // thumbtip / index nail + ring tip / bottom palm

#define KG_Y5_AG1 // thumbtip / middle nail + index tip / top palm
#define KG_Y5_AJ1
#define KG_Y5_GJ1 // thumbtip / middle nail + ring tip / top palm
#define KG_Y5_GJ2 // thumbtip / middle nail + ring tip / middle palm
#define KG_Y5_GJ3 // thumbtip / middle nail + ring tip / bottom palm

#define KG_Y6_AD1 // thumbtip / ring nail + index tip / top palm
#define KG_Y6_AJ1
#define KG_Y6_DJ1 // thumbtip / ring nail + middle tip / top palm

#define KG_Y7_AD1 // thumbtip / pinky nail + index tip / top palm
#define KG_Y7_AG1
#define KG_Y7_DG1 // thumbtip / pinky nail + middle tip / top palm

// thumbtip / top palm
#define KG_Y1_D46
#define KG_Y1_DG7

// thumbtip / index side
#define KG_MY_AD1
#define KG_MY_AG1
#define KG_MY_AJ1
#define KG_MY_AD2
#define KG_MY_AG2
#define KG_MY_AJ2
#define KG_MY_AD3
#define KG_MY_AG3
#define KG_MY_AJ3

#define KG_MY_DG7
#define KG_MY_DG1
#define KG_MY_DJ1
#define KG_MY_DG2
#define KG_MY_DJ2
#define KG_MY_DG3
#define KG_MY_DJ3

#define KG_MY_GJ1
#define KG_MY_GJ2
#define KG_MY_GJ3

#define KG_NY_AD1
#define KG_NY_AG1
#define KG_NY_AJ1
#define KG_NY_AD2
#define KG_NY_AG2
#define KG_NY_AJ2
#define KG_NY_AD3
#define KG_NY_AG3
#define KG_NY_AJ3

#define KG_NY_DG7
#define KG_NY_DG1
#define KG_NY_DJ1
#define KG_NY_DG2
#define KG_NY_DJ2
#define KG_NY_DG3
#define KG_NY_DJ3

#define KG_NY_GJ1
#define KG_NY_GJ2
#define KG_NY_GJ3

#define KG_OY_AD1
#define KG_OY_AG1
#define KG_OY_AJ1
#define KG_OY_AD2
#define KG_OY_AG2
#define KG_OY_AJ2
#define KG_OY_AD3
#define KG_OY_AG3
#define KG_OY_AJ3

#define KG_OY_DG7
#define KG_OY_DG1
#define KG_OY_DJ1
#define KG_OY_DG2
#define KG_OY_DJ2
#define KG_OY_DG3
#define KG_OY_DJ3

#define KG_OY_GJ1
#define KG_OY_GJ2
#define KG_OY_GJ3

// thumb segment / index side
#define KG_MZ_AD1
#define KG_MZ_AG1
#define KG_MZ_AJ1
#define KG_MZ_AD2
#define KG_MZ_AG2
#define KG_MZ_AJ2
#define KG_MZ_AD3
#define KG_MZ_AG3
#define KG_MZ_AJ3

#define KG_MZ_DG7
#define KG_MZ_DG1
#define KG_MZ_DJ1
#define KG_MZ_DG2
#define KG_MZ_DJ2
#define KG_MZ_DG3
#define KG_MZ_DJ3

#define KG_MZ_GJ1
#define KG_MZ_GJ2
#define KG_MZ_GJ3

#define KG_NZ_AD1
#define KG_NZ_AG1
#define KG_NZ_AJ1
#define KG_NZ_AD2
#define KG_NZ_AG2
#define KG_NZ_AJ2
#define KG_NZ_AD3
#define KG_NZ_AG3
#define KG_NZ_AJ3

#define KG_NZ_DG7
#define KG_NZ_DG1
#define KG_NZ_DJ1
#define KG_NZ_DG2
#define KG_NZ_DJ2
#define KG_NZ_DG3
#define KG_NZ_DJ3

#define KG_NZ_GJ1
#define KG_NZ_GJ2
#define KG_NZ_GJ3

#define KG_OZ_AD1
#define KG_OZ_AG1
#define KG_OZ_AJ1
#define KG_OZ_AD2
#define KG_OZ_AG2
#define KG_OZ_AJ2
#define KG_OZ_AD3
#define KG_OZ_AG3
#define KG_OZ_AJ3

#define KG_OZ_DG7
#define KG_OZ_DG1
#define KG_OZ_DJ1
#define KG_OZ_DG2
#define KG_OZ_DJ2
#define KG_OZ_DG3
#define KG_OZ_DJ3

#define KG_OZ_GJ1
#define KG_OZ_GJ2
#define KG_OZ_GJ3

// fingertip / thumb nail
#define KG_A8_DG1
#define KG_A8_DJ1
#define KG_A8_DG2
#define KG_A8_DJ2
#define KG_A8_DG3
#define KG_A8_DJ3
#define KG_A8_GJ1
#define KG_A8_GJ2
#define KG_A8_GJ3

#define KG_D8_AG1
#define KG_D8_AJ1
#define KG_D8_GJ1
#define KG_D8_GJ2
#define KG_D8_GJ3

#define KG_G8_AD1

#define KG_J8_AD1
#define KG_J8_AG1
#define KG_J8_DG1

// fingertip / top pad
#define KG_A1_DG2
#define KG_A1_DJ2
#define KG_A1_DG3
#define KG_A1_DJ3
#define KG_A1_GJ2
#define KG_A1_GJ3

#define KG_D1_AG2
#define KG_D1_AJ2
#define KG_D1_AG3
#define KG_D1_AJ3
#define KG_D1_GJ2
#define KG_D1_GJ3

#define KG_G1_AD2
#define KG_G1_AJ2
#define KG_G1_AD3
#define KG_G1_AJ3
#define KG_G1_DJ2
#define KG_G1_DJ3

#define KG_J1_AD2
#define KG_J1_AG2
#define KG_J1_AD3
#define KG_J1_AG3
#define KG_J1_DG2
#define KG_J1_DG3

// fingertip / mid pad
#define KG_A2_DG1
#define KG_A2_DJ1
#define KG_A2_DG3
#define KG_A2_DJ3
#define KG_A2_GJ1
#define KG_A2_GJ3

#define KG_D2_AG1
#define KG_D2_AJ1
#define KG_D2_AG3
#define KG_D2_AJ3
#define KG_D2_GJ1
#define KG_D2_GJ3

#define KG_G2_AD1
#define KG_G2_AJ1
#define KG_G2_AD3
#define KG_G2_AJ3
#define KG_G2_DJ1
#define KG_G2_DJ3

#define KG_J2_AD1
#define KG_J2_AG1
#define KG_J2_AD3
#define KG_J2_AG3
#define KG_J2_DG1
#define KG_J2_DG3

// fingertip / bottom pad
#define KG_A3_DG1
#define KG_A3_DJ1
#define KG_A3_DG2
#define KG_A3_DJ2
#define KG_A3_GJ1
#define KG_A3_GJ2

#define KG_D3_AG1
#define KG_D3_AJ1
#define KG_D3_AG2
#define KG_D3_AJ2
#define KG_D3_GJ1
#define KG_D3_GJ2

#define KG_G3_AD1
#define KG_G3_AJ1
#define KG_G3_AD2
#define KG_G3_AJ2
#define KG_G3_DJ1
#define KG_G3_DJ2

#define KG_J3_AD1
#define KG_J3_AG1
#define KG_J3_AD2
#define KG_J3_AG2
#define KG_J3_DG1
#define KG_J3_DG2

// === 2/4-SENSOR COMBINATIONS

// fingertip / thumbtip
#define KG_AY_DGJ1 // index tip / thumbtip + middle tip / top palm
#define KG_AY_DGJ2 // index tip / thumbtip + middle tip / middle palm
#define KG_AY_DGJ3 // index tip / thumbtip + middle tip / bottom palm
#define KG_DY_AGJ1 // middle tip / thumbtip + index tip / top palm
#define KG_GY_ADJ1 // ring tip / thumbtip + index tip / top palm
#define KG_JY_ADG1 // pinky tip / thumbtip + index tip / top palm

// thumbtip / fingernail
#define KG_Y4_DGJ1 // thumbtip / index nail + middle tip / top palm
#define KG_Y4_DGJ2 // thumbtip / index nail + middle tip / middle palm
#define KG_Y4_DGJ3 // thumbtip / index nail + middle tip / bottom palm
#define KG_Y5_AGJ1 // thumbtip / middle nail + index tip / top palm
#define KG_Y6_ADJ1 // thumbtip / ring nail + index tip / top palm
#define KG_Y7_ADG1 // thumbtip / pinky nail + index tip / top palm

// thumbtip / index side
#define KG_MY_ADG1
#define KG_MY_ADJ1
#define KG_MY_AGJ1
#define KG_MY_DGJ1
#define KG_MY_ADG2
#define KG_MY_ADJ2
#define KG_MY_AGJ2
#define KG_MY_DGJ2
#define KG_MY_ADG3
#define KG_MY_ADJ3
#define KG_MY_AGJ3
#define KG_MY_DGJ3

#define KG_NY_ADG1
#define KG_NY_ADJ1
#define KG_NY_AGJ1
#define KG_NY_DGJ1
#define KG_NY_ADG2
#define KG_NY_ADJ2
#define KG_NY_AGJ2
#define KG_NY_DGJ2
#define KG_NY_ADG3
#define KG_NY_ADJ3
#define KG_NY_AGJ3
#define KG_NY_DGJ3

#define KG_OY_ADG1
#define KG_OY_ADJ1
#define KG_OY_AGJ1
#define KG_OY_DGJ1
#define KG_OY_ADG2
#define KG_OY_ADJ2
#define KG_OY_AGJ2
#define KG_OY_DGJ2
#define KG_OY_ADG3
#define KG_OY_ADJ3
#define KG_OY_AGJ3
#define KG_OY_DGJ3

// thumb segment / index side
#define KG_MZ_ADG1
#define KG_MZ_ADJ1
#define KG_MZ_AGJ1
#define KG_MZ_DGJ1
#define KG_MZ_ADG2
#define KG_MZ_ADJ2
#define KG_MZ_AGJ2
#define KG_MZ_DGJ2
#define KG_MZ_ADG3
#define KG_MZ_ADJ3
#define KG_MZ_AGJ3
#define KG_MZ_DGJ3

#define KG_NZ_ADG1
#define KG_NZ_ADJ1
#define KG_NZ_AGJ1
#define KG_NZ_DGJ1
#define KG_NZ_ADG2
#define KG_NZ_ADJ2
#define KG_NZ_AGJ2
#define KG_NZ_DGJ2
#define KG_NZ_ADG3
#define KG_NZ_ADJ3
#define KG_NZ_AGJ3
#define KG_NZ_DGJ3

#define KG_OZ_ADG1
#define KG_OZ_ADJ1
#define KG_OZ_AGJ1
#define KG_OZ_DGJ1
#define KG_OZ_ADG2
#define KG_OZ_ADJ2
#define KG_OZ_AGJ2
#define KG_OZ_DGJ2
#define KG_OZ_ADG3
#define KG_OZ_ADJ3
#define KG_OZ_AGJ3
#define KG_OZ_DGJ3

// fingertip / thumb nail
#define KG_A8_DGJ1
#define KG_A8_DGJ2
#define KG_A8_DGJ3
#define KG_D8_AGJ1
#define KG_G8_ADJ1
#define KG_J8_ADJ1

// fingertip / top pad
#define KG_A1_DGJ2
#define KG_A1_DGJ3
#define KG_D1_AGJ2
#define KG_D1_AGJ3
#define KG_G1_ADJ2
#define KG_G1_ADJ3
#define KG_J1_ADG2
#define KG_J1_ADG3

// fingertip / mid pad
#define KG_A2_DGJ1
#define KG_A2_DGJ3
#define KG_D2_AGJ1
#define KG_D2_AGJ3
#define KG_G2_ADJ1
#define KG_G2_ADJ3
#define KG_J2_ADG1
#define KG_J2_ADG3

// fingertip / bottom pad
#define KG_A3_DGJ1
#define KG_A3_DGJ2
#define KG_D3_AGJ1
#define KG_D3_AGJ2
#define KG_G3_ADJ1
#define KG_G3_ADJ2
#define KG_J3_ADG1
#define KG_J3_ADG2

// === 2/5-SENSOR COMBINATIONS

// thumbtip / index side
#define KG_MY_ADGJ1
#define KG_MY_ADGJ2
#define KG_MY_ADGJ3

#define KG_NY_ADGJ1
#define KG_NY_ADGJ2
#define KG_NY_ADGJ3

#define KG_OY_ADGJ1
#define KG_OY_ADGJ2
#define KG_OY_ADGJ3

// thumb segment / index side
#define KG_MZ_ADGJ1
#define KG_MZ_ADGJ2
#define KG_MZ_ADGJ3

#define KG_NZ_ADGJ1
#define KG_NZ_ADGJ2
#define KG_NZ_ADGJ3

#define KG_OZ_ADGJ1
#define KG_OZ_ADGJ2
#define KG_OZ_ADGJ3

// === 2/2/2-SENSOR COMBINATIONS
#define KG_DM_J1_Y7

int pins[KSP_TOTAL_SENSORS] = {
    KSP_A, KSP_B, KSP_C, KSP_D, KSP_E,
    KSP_F, KSP_G, KSP_H, KSP_I, KSP_J,
    KSP_K, KSP_L, KSP_M, KSP_N, KSP_O,
    KSP_P, KSP_Q, KSP_R, KSP_S, KSP_T,
    KSP_U, KSP_V, KSP_W, KSP_X, KSP_Y,
    KSP_Z, KSP_1, KSP_2, KSP_3, KSP_4,
    KSP_5, KSP_6, KSP_7, KSP_8 };

int combinations[KSI_TOTAL_BITS][2] = {
    // index finger
    { KSP_A, KSP_Y },
    { KSP_A, KSP_1 },
    { KSP_A, KSP_2 },
    { KSP_A, KSP_3 },
    { KSP_A, KSP_8 },
    { KSP_B, KSP_Y },
    { KSP_C, KSP_Y },
    { KSP_M, KSP_Y },
    { KSP_M, KSP_Z },
    { KSP_N, KSP_Y },
    { KSP_N, KSP_Z },
    { KSP_O, KSP_Y },
    { KSP_O, KSP_Z },
    { KSP_Y, KSP_4 },

    // middle finger
    { KSP_D, KSP_M },
    { KSP_D, KSP_Y },
    { KSP_D, KSP_1 },
    { KSP_D, KSP_2 },
    { KSP_D, KSP_3 },
    { KSP_D, KSP_4 },
    { KSP_D, KSP_6 },
    { KSP_D, KSP_8 },
    { KSP_E, KSP_Y },
    { KSP_F, KSP_Y },
    { KSP_P, KSP_Y },
    { KSP_P, KSP_Z },
    { KSP_Q, KSP_Y },
    { KSP_Q, KSP_Z },
    { KSP_R, KSP_Y },
    { KSP_R, KSP_Z },
    { KSP_Y, KSP_5 },

    // ring finger
    { KSP_G, KSP_Y },
    { KSP_G, KSP_1 },
    { KSP_G, KSP_2 },
    { KSP_G, KSP_3 },
    { KSP_G, KSP_7 },
    { KSP_G, KSP_8 },
    { KSP_H, KSP_Y },
    { KSP_I, KSP_Y },
    { KSP_S, KSP_Y },
    { KSP_T, KSP_Y },
    { KSP_U, KSP_Y },
    { KSP_Y, KSP_6 },

    // little finger
    { KSP_J, KSP_Y },
    { KSP_J, KSP_1 },
    { KSP_J, KSP_2 },
    { KSP_J, KSP_3 },
    { KSP_J, KSP_8 },
    { KSP_K, KSP_Y },
    { KSP_L, KSP_Y },
    { KSP_V, KSP_Y },
    { KSP_W, KSP_Y },
    { KSP_X, KSP_Y },
    { KSP_Y, KSP_7 },

    // thumbtip / top palm
    { KSP_Y, KSP_1 },

    // thumb segment / fingernail
    { KSP_Z, KSP_4 },
    { KSP_Z, KSP_5 },
    { KSP_Z, KSP_6 },
    { KSP_Z, KSP_7 }
};

