// keyglove controller source code
// 10/1/2010 by Jeff Rowberg <jeff@rowberg.net>

// fix Arduino IDE regex issue with detecting first non-preprocessor directive
// (breaks when #ifdef is false and contains non-preprocessor line, see:
// http://code.google.com/p/arduino/issues/detail?id=156)
unsigned char ide_workaround = 0;

#define DETECT_THRESHOLD    30

#define ENABLE_PS2
//#define ENABLE_USB
//#define ENABLE_BLUETOOTH

#ifdef ENABLE_PS2
    #include "ps2keyboard.h"
    #define KB_PIN_CLOCK    3
    #define KB_PIN_DATA     2
    PS2keyboard *keyboard;
#endif

#ifdef ENABLE_USB
#endif

#ifdef ENABLE_BLUETOOTH
#endif

#include "sensors.h"
#include "touchset.h"

unsigned long sensors1 = 0, sensors2 = 0;
unsigned long detect1 = 0, detect2 = 0;
unsigned long verify1 = 0, verify2 = 0;

int counter = 0;
unsigned long int t; // time var
unsigned long int d; // detection timestamp
long int i; // index var
boolean adding = false;
boolean removing = false;
boolean blink_led = false;

#ifdef ENABLE_PS2
void ps2interrupt() {
    keyboard -> process_command();
}
#endif

void setup() {
    Serial.begin(115200);
    Serial.print("Keyglove device activated\n");

    // initialize all 25 I/O pins to INPUT mode and enable internal pullup resistors
    for (i = 0; i < KSP_TOTAL_SENSORS; i++) {
        pinMode(pins[i], INPUT);
        digitalWrite(pins[i], HIGH);
    }
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);

#ifdef ENABLE_PS2
    Serial.print("PS/2 keyboard emulation starting...\n");
    keyboard = new PS2keyboard(KB_PIN_CLOCK, KB_PIN_DATA);
    //attachInterrupt(0, ps2interrupt, LOW);
    keyboard -> initialize();
    Serial.print("PS/2 keyboard initialized\n");
#endif

    t = millis();
}

void loop() {
#ifdef ENABLE_PS2
    if (digitalRead(KB_PIN_CLOCK) == LOW && digitalRead(KB_PIN_DATA) == LOW) {
        Serial.print("PS/2 host interrupt\n");
        ps2interrupt();
    }
#endif

    detect1 = 0;
    detect2 = 0;
    removing = false;
    
    // loop through every possible 1-to-1 sensor combination and record levels
    for (i = 0; i < KSI_TOTAL_BITS; i++) {
        int s1 = combinations[i][0];
        int s2 = combinations[i][1];

        pinMode(pins[s1], OUTPUT);    // change to OUTPUT mode
        digitalWrite(pins[s1], LOW);  // bring LOW (default input level is HIGH)
        if (i < 32) {
            // write value to sensors1
            if (digitalRead(pins[s2]) == LOW) bitSet(detect1, i);
            //Serial.print(bitRead(detect1, i));
        } else {
            // write value to sensors2
            if (digitalRead(pins[s2]) == LOW) bitSet(detect2, i - 32);
            //Serial.print(bitRead(detect2, i - 32));
        }
        pinMode(pins[s1], INPUT);     // reset to INPUT mode
        digitalWrite(pins[s1], HIGH); // enable pullup
    }
    //Serial.print("\n");
    
    // check to see if we need to reset detection threshold
    if (verify1 != detect1 || verify2 != detect2) {
        // current sensors different from last detection, so reset threshold
        d = millis();
    } else if ((verify1 != sensors1 || verify2 != sensors2) && millis() - d >= DETECT_THRESHOLD) {
        // detection is over threshold and current readings are different from previous readings
        // check to see if they've just initiated or removed any touches
        if (verify1 > sensors1 || verify2 > sensors2) {
            adding = true;
        } else if (adding && (verify1 < sensors1 || verify2 < sensors2)) {
            adding = false;
            removing = true;

            // actual keypress this time around so test for each possible combination
            Serial.print("AAAAAABBBBBBBBBBCCCCCCCDDDDDDEFGHIJKLMMMMMMMMNNNNNNN\n");
            Serial.print("MNV123MNVWYZ1234MNVZ123MNV123MMMMMMMMWXYZ1456WXYZ456\n");
            for (i = 0; i < KSI_TOTAL_BITS; i++) {
                if (i < 32) Serial.print(bitRead(sensors1, i));
                else Serial.print(bitRead(sensors2, i - 32));
            }
            Serial.print("\n");
            Serial.print("Base combination: ");
            if        (KG_AY) {
                Serial.print("AY");
            } else if (KG_A1) {
                Serial.print("A1");
            } else if (KG_A2) {
                Serial.print("A2");
            } else if (KG_A3) {
                Serial.print("A3");
            } else if (KG_A8) {
                Serial.print("A8");
            } else if (KG_BY) {
                Serial.print("BY");
            } else if (KG_CY) {
                Serial.print("CY");
            } else if (KG_MY) {
                Serial.print("MY");
            } else if (KG_MZ) {
                Serial.print("MZ");
            } else if (KG_NY) {
                Serial.print("NY");
            } else if (KG_NZ) {
                Serial.print("NZ");
            } else if (KG_OY) {
                Serial.print("OY");
            } else if (KG_OZ) {
                Serial.print("OZ");
            } else if (KG_Y4) {
                Serial.print("Y4");
            } else if (KG_DM) {
                Serial.print("DM");
            } else if (KG_DY) {
                Serial.print("DY");
            } else if (KG_D1) {
                Serial.print("D1");
            } else if (KG_D2) {
                Serial.print("D2");
            } else if (KG_D3) {
                Serial.print("D3");
            } else if (KG_D4) {
                Serial.print("D4");
            } else if (KG_D6) {
                Serial.print("D6");
            } else if (KG_D8) {
                Serial.print("D8");
            } else if (KG_EY) {
                Serial.print("EY");
            } else if (KG_FY) {
                Serial.print("FY");
            } else if (KG_PY) {
                Serial.print("PY");
            } else if (KG_PZ) {
                Serial.print("PZ");
            } else if (KG_QY) {
                Serial.print("QY");
            } else if (KG_QZ) {
                Serial.print("QZ");
            } else if (KG_RY) {
                Serial.print("RY");
            } else if (KG_RZ) {
                Serial.print("RZ");
            } else if (KG_Y5) {
                Serial.print("Y5");
            } else if (KG_GY) {
                Serial.print("GY");
            } else if (KG_G1) {
                Serial.print("G1");
            } else if (KG_G2) {
                Serial.print("G2");
            } else if (KG_G3) {
                Serial.print("G3");
            } else if (KG_G7) {
                Serial.print("G7");
            } else if (KG_G8) {
                Serial.print("G8");
            } else if (KG_HY) {
                Serial.print("HY");
            } else if (KG_IY) {
                Serial.print("IY");
            } else if (KG_SY) {
                Serial.print("SY");
            } else if (KG_TY) {
                Serial.print("TY");
            } else if (KG_UY) {
                Serial.print("UY");
            } else if (KG_Y6) {
                Serial.print("Y6");
            } else if (KG_JY) {
                Serial.print("JY");
            } else if (KG_J1) {
                Serial.print("J1");
            } else if (KG_J2) {
                Serial.print("J2");
            } else if (KG_J3) {
                Serial.print("J3");
            } else if (KG_J8) {
                Serial.print("J8");
            } else if (KG_KY) {
                Serial.print("KY");
            } else if (KG_LY) {
                Serial.print("LY");
            } else if (KG_VY) {
                Serial.print("VY");
            } else if (KG_WY) {
                Serial.print("WY");
            } else if (KG_XY) {
                Serial.print("XY");
            } else if (KG_Y7) {
                Serial.print("Y7");
            } else if (KG_Y1) {
                Serial.print("Y1");
            } else if (KG_Z4) {
                Serial.print("Z4");
            } else if (KG_Z5) {
                Serial.print("Z5");
            } else if (KG_Z6) {
                Serial.print("Z6");
            } else if (KG_Z7) {
                Serial.print("Z7");
            } else {
                Serial.print("UNDEFINED");
            }
            Serial.print("\n");
        }
        
        // set official sensor readings to current readings 
        sensors1 = verify1;
        sensors2 = verify2;
    }
    
    verify1 = detect1;
    verify2 = detect2;

    counter++;
    if (counter == 1000) {
#ifdef ENABLE_PS2
    keyboard -> keypress(KEY_K);
#endif
        counter = 0;
        blink_led = !blink_led;
        if (blink_led) digitalWrite(13, HIGH);
        else digitalWrite(13, LOW);
        //Serial.print("1000 iterations: ");
        //Serial.print(millis() - t);
        //Serial.print("\n");
        //t = millis();
    }
}

void trigger(unsigned int keycode) {
#ifdef ENABLE_PS2
    keyboard -> keypress(KEY_K);
#endif
    switch (keycode) {
        case TOUCH_AM: Serial.print(" (AM)"); break;
        case TOUCH_AB1: Serial.print(" (AB1)"); break;
        case TOUCH_AB2: Serial.print(" (AB2)"); break;
        case TOUCH_AB3: Serial.print(" (AB3)"); break;
        default: Serial.print(" ("); Serial.print(keycode); Serial.print(")"); break;
    }
}
