HardwareSerial Serial1 = HardwareSerial();

void setup() {
    Serial.begin(115200);

    Serial.println("The WT12 Prep sketch will attempt to communicate with the WT12 and set");
    Serial.println("necessary configuration options for Keyglove operation, finishing up by");
    Serial.println("setting the baud rate to 38400 instead of the the default. If successful,");
    Serial.println("this sketch will not run a second time since the baud rate will be");
    Serial.println("mismatched.\r\n");
    Serial.println("Note that you should see responses from the WT12 if the sketch is");
    Serial.println("communicating correctly. If you see binary or gibberish, the baud rate");
    Serial.println("is already mismatched.\r\n");
    Serial.println("Enter '1' to try 115200 baud, or '2' to use 38400 to perform a factory");
    Serial.println("reset only without modifying settings. Entering '2' will require you to");
    Serial.println("run the sketch again and enter '1' to finish the preparation.\r\n");

    while (Serial.available());
    uint8_t ch;
    do { ch = Serial.read(); } while (ch != '1' && ch != '2');

    if (ch == '1') {
        Serial1.begin(115200);
        Serial.println("Resetting iWRAP to factory defaults...");
        Serial1.print("\r\nSET RESET\r\n");
        Serial1.flush();
        delay(3000);
        while (Serial1.available()) Serial.print((char)Serial1.read());
        Serial.println("");
    
        Serial.println("Setting device name...");
        Serial1.print("SET BT NAME Keyglove\r\n");
        Serial1.flush();
        delay(500);
        while (Serial1.available()) Serial.print((char)Serial1.read());
        Serial.println("");
    
        Serial.println("Setting device identity...");
        Serial1.print("SET BT IDENT BT:47 f000 4.0.0 Keyglove Input Device\r\n");
        Serial1.flush();
        delay(500);
        while (Serial1.available()) Serial.print((char)Serial1.read());
        Serial.println("");
    
        Serial.println("Setting device class to keyboard/mouse...");
        Serial1.print("SET BT CLASS 005C0\r\n");
        Serial1.flush();
        delay(500);
        while (Serial1.available()) Serial.print((char)Serial1.read());
        Serial.println("");
    
        Serial.println("Setting Secure Simple Pairing (SSP) mode to 3/0...");
        Serial1.print("SET BT SSP 3 0\r\n");
        Serial1.flush();
        delay(500);
        while (Serial1.available()) Serial.print((char)Serial1.read());
        Serial.println("");
    
        Serial.println("Enabling GPIO7 toggling on Carrier Detect (link active mode)...");
        Serial1.print("SET CONTROL CD 80 0\r\n");
        Serial1.flush();
        delay(500);
        while (Serial1.available()) Serial.print((char)Serial1.read());
        Serial.println("");
    
        Serial.println("Enabling GPIO6 DTR detection for entering command mode (ESCAPE char disabled)...");
        Serial1.print("SET CONTROL ESCAPE - 40 1\r\n");
        Serial1.flush();
        delay(500);
        while (Serial1.available()) Serial.print((char)Serial1.read());
        Serial.println("");
    
        Serial.println("Changing baud rate to 38400...sketch will end after this. Bye!");
        Serial1.print("SET CONTROL BAUD 38400,8n1\r\n");
        Serial1.flush();
        delay(100);
    } else {
        Serial1.begin(38400);
        Serial.println("Resetting iWRAP to factory defaults...sketch will end after this. Bye!");
        Serial1.print("\r\nSET RESET\r\n");
        Serial1.flush();
    }
}

void loop() {
    // nothing to do here
}