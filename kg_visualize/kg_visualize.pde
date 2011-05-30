// Keyglove controller source code - Visualization and testing program
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

import processing.serial.*;
import processing.opengl.*;

// touch sensors
int numTouches = 60;
byte touches[] = new byte[numTouches];
/*String[] touchNames = {
    "AY", "A1", "A2", "A3", "A8",
    "BY", "CY", "MY", "MZ", "NY",
    "NZ", "OY", "OZ", "Y4", "DM",
    "DY", "D1", "D2", "D3", "D4",
    "D6", "D8", "EY", "FY", "PY",
    "PZ", "QY", "QZ", "RY", "RZ",
    "Y5", "GY", "G1", "G2", "G3",
    "G7", "G8", "HY", "IY", "SY",
    "TY", "UY", "Y6", "JY", "J1",
    "J2", "J3", "J8", "KY", "LY",
    "VY", "WY", "XY", "Y7", "Y1",
    "Z4", "Z5", "Z6", "Z7" };*/
String[] touchNames = {
    "Z7", "Y7", "J8", "JY", "KY",
    "LY", "VY", "WY", "XY", "Z6",
    "Y6", "G8", "GY", "HY", "IY",
    "SY", "TY", "UY", "G7", "Z5",
    "Y5", "D8", "DY", "EY", "FY",
    "PZ", "PY", "QZ", "QY", "RZ",
    "RY", "D4", "D6", "D7", "DM",
    "Z4", "Y4", "A8", "AY", "BY",
    "CY", "MZ", "MY", "NZ", "NY",
    "OZ", "OY", "A3", "A2", "A1",
    "D3", "D2", "D1", "G3", "G2",
    "G1", "J3", "J2", "J1", "Y1"
};

// absolute accelerometer x/y/z
float x = 0, y = 0, z = 0;
float px = 0, py = 0, pz = 0;
int accelHistLength = 4;
float xHist[] = new float[accelHistLength];
float yHist[] = new float[accelHistLength];
float zHist[] = new float[accelHistLength];

// angle transformed x/y/z
float xa = 0, ya = 0, za = 0;
int angleHistLength = 4;
float xaHist[] = new float[angleHistLength];
float yaHist[] = new float[angleHistLength];
float zaHist[] = new float[angleHistLength];

// mouse x/y/z
float mx = 0, my = 0;
float mdx = 0, mdy = 0;
int mouseHistLength = 20;
float mxHist[] = new float[mouseHistLength];
float myHist[] = new float[mouseHistLength];

// keypress release
String release = "";
int releaseHistLength = 40;
String releaseHist[] = new String[releaseHistLength];

// viewport size
int vw = 800, vh = 600;

// visualization offsets
int offsetTouchX = 20;
int offsetTouchY = 20;
int offsetAccelX = 170;
int offsetAccelY = 100;
int offsetAngleX = 440;
int offsetAngleY = 20;
int offset3DX = 530;
int offset3DY = 270;

PFont font128, font32, font24, font16, font16b, fixed;
Serial port;
String buff = "";

void setup() {
    size(vw, vh, OPENGL);
    lights();
    smooth();
    noStroke();
    port = new Serial(this, Serial.list()[0], 115200);
    font128 = loadFont("Calibri-128.vlw");
    font32 = loadFont("Calibri-32.vlw");
    font24 = loadFont("Calibri-24.vlw");
    font16 = loadFont("Calibri-16.vlw");
    font16b = loadFont("Calibri-Bold-16.vlw");
    fixed = loadFont("Consolas-Bold-16.vlw");
}

void draw() {
    int which;
    background(51);
    
    // basic project info
    fill(0, 120);
    rectMode(CORNER);
    rect(10, vh - 90, 370, 80);
    fill(255, 255); 
    textAlign(LEFT);
    textFont(font32);
    text("Keyglove Visualization Test", 20, vh - 58);
    fill(255, 185); 
    textFont(font16b);
    textAlign(CENTER);
    text("2011 - Jeff Rowberg", 200, vh - 36);
    text("www.keyglove.net", 200, vh - 20);
    
    // touch sensor combinations
    textFont(fixed);
    textAlign(LEFT);
    int colOffsetX = 0, colOffsetY = 0;
    int touchColSplit = ceil((float)touches.length / 2);
    for (int i = 0; i < touches.length; i++) {
        if (i == touchColSplit) { colOffsetX = 50; colOffsetY = -12*i; }
        if (touches[i] == 1) fill(255, 85, 85, 255);
        else fill(255, 225);
        text(touchNames[i], offsetTouchX + colOffsetX + 20, offsetTouchY + 10 + 12*i + colOffsetY);
        if (touches[i] == 1) fill(255, 55, 55, 255);
        else fill(255, 153);
        ellipse(offsetTouchX + colOffsetX + 48, offsetTouchY + 5 + 12*i + colOffsetY, 8, 8);
    }
    fill(0, 120);
    rectMode(CORNER);
    rect(offsetTouchX, offsetTouchY + 10 + 12*touchColSplit, 130, 30);
    textFont(font16b);
    fill(255, 225);
    text("Sensor Touches", offsetTouchX + 10, offsetTouchY + 30 + 12*touchColSplit);
    
    // update absolute x/y/z from accelerometer
    which = frameCount % accelHistLength;
    xHist[which] = x;
    yHist[which] = y;
    zHist[which] = z;
    rectMode(CORNER);
    fill(255, 153);
    rect(offsetAccelX, offsetAccelY, 225, 4);
    textAlign(LEFT);
    fill(255, 225);
    for (int i = 0; i < accelHistLength; i++) {
        int index = (which + 1 + i) % angleHistLength;
        rect(offsetAccelX + 50, offsetAccelY + 2, 20, -xHist[index]/4);
        rect(offsetAccelX + 120, offsetAccelY + 2, 20, -yHist[index]/4);
        rect(offsetAccelX + 190, offsetAccelY + 2, 20, -zHist[index]/4);
    }
    fill(255, 0, 0, 100);
    rect(offsetAccelX + 50, offsetAccelY + 2, 20, -px/4);
    fill(0, 255, 0, 100);
    rect(offsetAccelX + 120, offsetAccelY + 2, 20, -py/4);
    fill(0, 0, 255, 100);
    rect(offsetAccelX + 190, offsetAccelY + 2, 20, -pz/4);
    fill(255, 255);
    textFont(font24);
    text("X", offsetAccelX + 35, offsetAccelY - 5);
    text("Y", offsetAccelX + 105, offsetAccelY - 5);
    text("Z", offsetAccelX + 175, offsetAccelY - 5);
    fill(0, 120);
    rectMode(CORNER);
    rect(offsetAccelX, offsetAccelY + 100, 225, 30);
    fill(255, 220);
    textFont(font16b);
    text("Accelerometer Measurements", offsetAccelX + 10, offsetAccelY + 120);
    textAlign(RIGHT);
    textFont(font16);
    text(str(x), offsetAccelX + 47, offsetAccelY + 20);
    text(str(y), offsetAccelX + 117, offsetAccelY + 20);
    text(str(z), offsetAccelX + 187, offsetAccelY + 20);
    
    // update angular x/y/z from accelerometer
    which = frameCount % angleHistLength;
    xaHist[which] = xa;
    yaHist[which] = ya;
    zaHist[which] = za;
    rectMode(RADIUS);
    fill(255, 153); 

    stroke(255);
    strokeWeight(4);
    ellipse(offsetAngleX + 50, offsetAngleY + 80, 100, 100);
    ellipse(offsetAngleX + 175, offsetAngleY + 80, 100, 100);
    ellipse(offsetAngleX + 300, offsetAngleY + 80, 100, 100);
    noStroke();
    fill(255, 255); 
    textFont(font24);
    textAlign(RIGHT);
    text(str(xa), offsetAngleX - 28, offsetAngleY + 180, 100, 32);
    text(str(ya), offsetAngleX + 102, offsetAngleY + 180, 100, 32);
    text(str(za), offsetAngleX + 227, offsetAngleY + 180, 100, 32);
    textAlign(LEFT);
    text("X°", offsetAngleX + 45, offsetAngleY + 10);
    text("Y°", offsetAngleX + 170, offsetAngleY + 10);
    text("Z°", offsetAngleX + 295, offsetAngleY + 10);
    for (int i = 0; i < angleHistLength; i++) {
        int index = (which + 1 + i) % angleHistLength;
        fill(30, 30, 255, 255 - (i*255/angleHistLength));

        pushMatrix();
        translate(offsetAngleX + 50, offsetAngleY + 80);
        rotate(radians(xaHist[index]));
        rect(0, 0, 48, 5);
        popMatrix();

        pushMatrix();
        translate(offsetAngleX + 175, offsetAngleY + 80);
        rotate(radians(yaHist[index]));
        rect(0, 0, 48, 5);
        popMatrix();

        pushMatrix();
        translate(offsetAngleX + 300, offsetAngleY + 80);
        rotate(radians(zaHist[index]));
        rect(0, 0, 48, 5);
        popMatrix();
    }
    fill(0, 120);
    rectMode(CORNER);
    rect(offsetAngleX, offsetAngleY + 180, 350, 30);
    fill(255, 225);
    textFont(font16b);
    text("Accelerometer Tilt Calculations", offsetAngleX + 70, offsetAngleY + 200);
    
    // update 3D tilt visualization
    pushMatrix();
    translate(offset3DX + 100, offset3DY + 80, 0);
    fill(255, 0, 0, 200);
    box(200, 5, 5);
    pushMatrix(); translate(100, 0, 0); sphere(7); popMatrix();
    fill(0, 255, 0, 200);
    box(5, 5, 200);
    pushMatrix(); translate(0, 0, 100); sphere(7); popMatrix();
    fill(0, 0, 255, 200);
    box(5, 200, 5);
    pushMatrix(); translate(0, -100, 0); sphere(7); popMatrix();
    pushMatrix();
    rotateX(radians(-ya)); rotateZ(radians(-xa));
    fill(255, 100, 100, 200);
    box(200, 15, 15);
    pushMatrix(); translate(100, 0, 0); sphere(25); popMatrix();
    fill(100, 255, 100, 200);
    box(15, 15, 200);
    pushMatrix(); translate(0, 0, 100); sphere(25); popMatrix();
    fill(100, 100, 255, 200);
    box(15, 200, 15);
    pushMatrix(); translate(0, -100, 0); sphere(25); popMatrix();
    popMatrix();
    popMatrix();
    textFont(font16b);
    fill(255, 100, 100, 200);
    text("X", offset3DX, offset3DY);
    fill(100, 255, 100, 200);
    text("Y", offset3DX + 15, offset3DY);
    fill(100, 100, 255, 200);
    text("Z", offset3DX + 30, offset3DY);
    
    fill(0, 120);
    rectMode(CORNER);
    rect(offset3DX, offset3DY + 220, 200, 30);
    fill(255, 225);
    textFont(font16b);
    textAlign(CENTER);
    text("3D Tilt Representation", offset3DX, offset3DY + 230, 200, 30);

    // update mouse cursor position/trail ellipses
    fill(255, 225);
    which = frameCount % mouseHistLength;
    mxHist[which] = (vw / 2) + mx;
    myHist[which] = (vh / 2) + my;
    for (int i = 0; i < mouseHistLength; i++) {
        // which+1 is the smallest (the oldest in the array)
        int index = (which + 1 + i) % mouseHistLength;
        ellipse(mxHist[index], myHist[index], i/2, i/2);
    }
    
    // update touch release text
    which = frameCount % releaseHistLength;
    releaseHist[which] = release;
    release = "";
    textAlign(CENTER, CENTER);
    textFont(font128);
    int cStep = 255 / releaseHistLength;
    int fStep = 128 / releaseHistLength;
    for (int i = 0; i < releaseHistLength; i++) {
        // which+1 is the smallest (the oldest in the array)
        int index = (which + 1 + i) % releaseHistLength;
        textFont(font128, 128 - fStep * (releaseHistLength - i));
        fill(255, 255 - cStep * (releaseHistLength - i));
        if (releaseHist[index] != null && releaseHist[index].length() > 0) {
            text(releaseHist[index], vw/2, vh/2);
        }
    }
    textAlign(LEFT);
    
    // check for more data availability
    while (port.available() > 0) {
        serialEvent(port.read());
    }
}

void serialEvent(int serial) { 
    if (serial != '\n') { 
        buff += char(serial);
    } else {
        String[] vs = buff.split(" ");
        //println(buff);
        buff = "";
        String cmdType = vs[0].trim();
        try {
            if (cmdType.equals("accel") && vs.length >= 16) {
                px = Integer.parseInt(vs[1].trim());
                py = Integer.parseInt(vs[2].trim());
                pz = Integer.parseInt(vs[3].trim());
                x = Integer.parseInt(vs[4].trim());
                y = Integer.parseInt(vs[5].trim());
                z = Integer.parseInt(vs[6].trim());
                xa = Integer.parseInt(vs[10].trim());
                ya = Integer.parseInt(vs[11].trim());
                za = Integer.parseInt(vs[12].trim());
            } else if (cmdType.equals("touch") && vs.length >= 2 && vs[1].length() >= numTouches) {
                for (int i = 0; i < vs[1].length(); i++) touches[i] = vs[1].charAt(i) == '1' ? (byte)1 : 0;
            } else if (cmdType.equals("mouse") && vs.length >= 5) {
                mx = Integer.parseInt(vs[1].trim());
                my = Integer.parseInt(vs[2].trim());
                mdx = Integer.parseInt(vs[3].trim());
                mdy = Integer.parseInt(vs[4].trim());
            } else if (cmdType.equals("release") && vs.length >= 2) {
                release = vs[1];
            } else if (cmdType.equals("keycode") && vs.length >= 2) {
                if (vs[1].charAt(0) == '+') {
                    // direct key
                } else if (vs[1].charAt(0) == '/') {
                    //
                }
            }
        } catch (NumberFormatException e) {
        }
    }
}
