// Keyglove controller source code - WT12 preparation shortcut sketch
// 7/4/2012 by Jeff Rowberg <jeff@rowberg.net>

// This is a shortcut sketch for making sure your WT12 is correctly configured
// for use with the Keyglove. This will ultimately be rolled into the main
// Keyglove firmware, but for the moment there is a separate sketch to make it
// easier to do.

/* ============================================
Controller code is placed under the MIT license
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



// NOTE: Under normal cirumstances, the host MCU must be running at a minimum of
// 16MHz for reliable serial communication at 115200 baud. The error rate is too
// high at 8MHz or lower. If you're using a Teensy board, make sure the CPU is
// set to 16MHz while running this sketch even if the device has been modified
// for 3.3v operation. 16MHz @ 3.3v is overclocking, but should be safe at least
// for short periods of time.



// use the following line if using Teensy or Teensy++
// if using the Arduino Leonardo or Mega, comment it out
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