// NAMED KEYGLOVE SENSOR CONSTANTS (pins[] array indexes, these are NOT pin numbers)
#define KGS_A 0
#define KGS_B 1
#define KGS_C 2
#define KGS_D 3
#define KGS_E 4
#define KGS_F 5
#define KGS_G 6
#define KGS_H 7
#define KGS_I 8
#define KGS_J 9
#define KGS_K 10
#define KGS_L 11
#define KGS_M 12
#define KGS_N 13
#define KGS_V 14
#define KGS_W 15
#define KGS_X 16
#define KGS_Y 17
#define KGS_Z 18
#define KGS_1 19
#define KGS_2 20
#define KGS_3 21
#define KGS_4 22
#define KGS_5 23
#define KGS_6 24

// NAMED KEYGLOVE 1-TO-1 SENSOR TOUCH CONSTANTS (bits correspond to combinations[] array indexes)
#define KG_AM    bitRead(sensors1,  0)
#define KG_AN    bitRead(sensors1,  1)
#define KG_AV    bitRead(sensors1,  2)
#define KG_A1    bitRead(sensors1,  3)
#define KG_A2    bitRead(sensors1,  4)
#define KG_A3    bitRead(sensors1,  5)
#define KG_BM    bitRead(sensors1,  6)
#define KG_BN    bitRead(sensors1,  7)
#define KG_BV    bitRead(sensors1,  8)
#define KG_BW    bitRead(sensors1,  9)
#define KG_BY    bitRead(sensors1, 10)
#define KG_BZ    bitRead(sensors1, 11)
#define KG_B1    bitRead(sensors1, 12)
#define KG_B2    bitRead(sensors1, 13)
#define KG_B3    bitRead(sensors1, 14)
#define KG_B4    bitRead(sensors1, 15)
#define KG_CM    bitRead(sensors1, 16)
#define KG_CN    bitRead(sensors1, 17)
#define KG_CV    bitRead(sensors1, 18)
#define KG_CZ    bitRead(sensors1, 19)
#define KG_C1    bitRead(sensors1, 20)
#define KG_C2    bitRead(sensors1, 21)
#define KG_C3    bitRead(sensors1, 22)
#define KG_DM    bitRead(sensors1, 23)
#define KG_DN    bitRead(sensors1, 24)
#define KG_DV    bitRead(sensors1, 25)
#define KG_D1    bitRead(sensors1, 26)
#define KG_D2    bitRead(sensors1, 27)
#define KG_D3    bitRead(sensors1, 28)
#define KG_EM    bitRead(sensors1, 29)
#define KG_FM    bitRead(sensors1, 30)
#define KG_GM    bitRead(sensors1, 31)
#define KG_HM    bitRead(sensors2,  0)
#define KG_IM    bitRead(sensors2,  1)
#define KG_JM    bitRead(sensors2,  2)
#define KG_KM    bitRead(sensors2,  3)
#define KG_LM    bitRead(sensors2,  4)
#define KG_MW    bitRead(sensors2,  5)
#define KG_MX    bitRead(sensors2,  6)
#define KG_MY    bitRead(sensors2,  7)
#define KG_MZ    bitRead(sensors2,  8)
#define KG_M1    bitRead(sensors2,  9)
#define KG_M4    bitRead(sensors2, 10)
#define KG_M5    bitRead(sensors2, 11)
#define KG_M6    bitRead(sensors2, 12)
#define KG_NW    bitRead(sensors2, 13)
#define KG_NX    bitRead(sensors2, 14)
#define KG_NY    bitRead(sensors2, 15)
#define KG_NZ    bitRead(sensors2, 16)
#define KG_N4    bitRead(sensors2, 17)
#define KG_N5    bitRead(sensors2, 18)
#define KG_N6    bitRead(sensors2, 19)

// -----------------> glove sensors should be wired to the following I/O pins:
//                     A  B  C  D  E  F  G  H  I  J  K  L  M  N  V  W  X  Y  Z  1  2  3  4  5  6
int pins[SENSORS] = { 22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46 };

int combinations[BASE_COMBINATIONS][2] = {
  
    // index fingertip
    { KGS_A, KGS_M },
    { KGS_A, KGS_N },
    { KGS_A, KGS_V },
    { KGS_A, KGS_1 },
    { KGS_A, KGS_2 },
    { KGS_A, KGS_3 },
    
    // middle fingertip
    { KGS_B, KGS_M },
    { KGS_B, KGS_N },
    { KGS_B, KGS_V },
    { KGS_B, KGS_W },
    { KGS_B, KGS_Y },
    { KGS_B, KGS_Z },
    { KGS_B, KGS_1 },
    { KGS_B, KGS_2 },
    { KGS_B, KGS_3 },
    { KGS_B, KGS_4 },
    
    // ring fingertip
    { KGS_C, KGS_M },
    { KGS_C, KGS_N },
    { KGS_C, KGS_V },
    { KGS_C, KGS_Z },
    { KGS_C, KGS_1 },
    { KGS_C, KGS_2 },
    { KGS_C, KGS_3 },
    
    // pinky fingertip
    { KGS_D, KGS_M },
    { KGS_D, KGS_N },
    { KGS_D, KGS_V },
    { KGS_D, KGS_1 },
    { KGS_D, KGS_2 },
    { KGS_D, KGS_3 },
    
    // finger segment 1 / thumbtip
    { KGS_E, KGS_M },
    { KGS_F, KGS_M },
    { KGS_G, KGS_M },
    { KGS_H, KGS_M },
    
    // finger segment 2 / thumbtip
    { KGS_I, KGS_M },
    { KGS_J, KGS_M },
    { KGS_K, KGS_M },
    { KGS_L, KGS_M },
    
    // thumbtip / fingernail
    { KGS_M, KGS_W },
    { KGS_M, KGS_X },
    { KGS_M, KGS_Y },
    { KGS_M, KGS_Z },
    
    // thumbtip / top palm
    { KGS_M, KGS_1 },
    
    // thumbtip / index side
    { KGS_M, KGS_4 },
    { KGS_M, KGS_5 },
    { KGS_M, KGS_6 },
    
    // thumb segment / fingernail
    { KGS_N, KGS_W },
    { KGS_N, KGS_X },
    { KGS_N, KGS_Y },
    { KGS_N, KGS_Z },
    
    // thumb segment / index side
    { KGS_N, KGS_4 },
    { KGS_N, KGS_5 },
    { KGS_N, KGS_6 },
    
};

