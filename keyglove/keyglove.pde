// keyglove controller source code
// 10/1/2010 by Jeff Rowberg <jeff@rowberg.net>

// fix Arduino IDE regex issue with detecting first non-preprocessor directive
// (breaks when #ifdef is false and contains non-preprocessor line, see:
// http://code.google.com/p/arduino/issues/detail?id=156)
unsigned char ide_workaround = 0;

#define SENSORS             25
#define BASE_COMBINATIONS   52
#define DETECT_THRESHOLD    30

//#define ENABLE_PS2
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
#include "combinations.h"
#include "chordset.h"

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
    for (i = 0; i < SENSORS; i++) {
        pinMode(pins[i], INPUT);
        digitalWrite(pins[i], HIGH);
    }
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);

#ifdef ENABLE_PS2
    Serial.print("PS/2 keyboard emulation starting...\n");
    keyboard = new PS2keyboard(KB_PIN_CLOCK, KB_PIN_DATA);
    attachInterrupt(0, ps2interrupt, LOW);
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
    for (i = 0; i < BASE_COMBINATIONS; i++) {
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
            for (i = 0; i < BASE_COMBINATIONS; i++) {
                if (i < 32) Serial.print(bitRead(sensors1, i));
                else Serial.print(bitRead(sensors2, i - 32));
            }
            Serial.print("\n");
            Serial.print("Base combination: ");
            if        (KG_AM) {
                Serial.print("AM");
                if      (KG_ABCDM) trigger(TOUCH_ABCDM);
                else if (KG_ABCM)  trigger(TOUCH_ABCM);
                else if (KG_ABDM)  trigger(TOUCH_ABDM);
                else if (KG_ABM)   trigger(TOUCH_ABM);
                else if (KG_ACDM)  trigger(TOUCH_ACDM);
                else if (KG_ACM)   trigger(TOUCH_ACM);
                else if (KG_ADM)   trigger(TOUCH_ADM);
                else               trigger(TOUCH_AM);
            } else if (KG_AN) {
                Serial.print("AN");
            } else if (KG_AV) {
                Serial.print("AV");
            } else if (KG_A1) {
                Serial.print("A1");
                if      (KG_ABCD1) trigger(TOUCH_ABCD1);
                else if (KG_AB1)   trigger(TOUCH_AB1);
                else if (KG_AB2)   trigger(TOUCH_AB2);
                else if (KG_AB3)   trigger(TOUCH_AB3);
            } else if (KG_A2) {
                Serial.print("A2");
            } else if (KG_A3) {
                Serial.print("A3");
            } else if (KG_BM) {
                Serial.print("BM");
            } else if (KG_BN) {
                Serial.print("BN");
            } else if (KG_BV) {
                Serial.print("BV");
            } else if (KG_BW) {
                Serial.print("BW");
            } else if (KG_BY) {
                Serial.print("BY");
            } else if (KG_BZ) {
                Serial.print("BZ");
            } else if (KG_B1) {
                Serial.print("B1");
            } else if (KG_B2) {
                Serial.print("B2");
            } else if (KG_B3) {
                Serial.print("B3");
            } else if (KG_B4) {
                Serial.print("B4");
            } else if (KG_CM) {
                Serial.print("CM");
            } else if (KG_CN) {
                Serial.print("CN");
            } else if (KG_CV) {
                Serial.print("CV");
            } else if (KG_CZ) {
                Serial.print("CZ");
            } else if (KG_C1) {
                Serial.print("C1");
            } else if (KG_C2) {
                Serial.print("C2");
            } else if (KG_C3) {
                Serial.print("C3");
            } else if (KG_DM) {
                Serial.print("DM");
            } else if (KG_DN) {
                Serial.print("DN");
            } else if (KG_DV) {
                Serial.print("DV");
            } else if (KG_D1) {
                Serial.print("D1");
            } else if (KG_D2) {
                Serial.print("D2");
            } else if (KG_D3) {
                Serial.print("D3");
            } else if (KG_EM) {
                Serial.print("EM");
            } else if (KG_FM) {
                Serial.print("FM");
            } else if (KG_GM) {
                Serial.print("GM");
            } else if (KG_HM) {
                Serial.print("HM");
            } else if (KG_IM) {
                Serial.print("IM");
            } else if (KG_JM) {
                Serial.print("JM");
            } else if (KG_KM) {
                Serial.print("KM");
            } else if (KG_LM) {
                Serial.print("LM");
            } else if (KG_MW) {
                Serial.print("MW");
            } else if (KG_MX) {
                Serial.print("MX");
            } else if (KG_MY) {
                Serial.print("MY");
            } else if (KG_MZ) {
                Serial.print("MZ");
            } else if (KG_M1) {
                Serial.print("M1");
            } else if (KG_M4) {
                Serial.print("M4");
            } else if (KG_M5) {
                Serial.print("M5");
            } else if (KG_M6) {
                Serial.print("M6");
            } else if (KG_NW) {
                Serial.print("NW");
            } else if (KG_NX) {
                Serial.print("NX");
            } else if (KG_NY) {
                Serial.print("NY");
            } else if (KG_NZ) {
                Serial.print("NZ");
            } else if (KG_N4) {
                Serial.print("N4");
            } else if (KG_N5) {
                Serial.print("N5");
            } else if (KG_N6) {
                Serial.print("N6");
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
