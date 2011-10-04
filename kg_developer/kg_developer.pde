import processing.serial.*;

int ACCEL_Y = 125;
int GYRO_Y = 350;

Serial port;
String buff = "";
PFont font128, font32, font24, font16, font16b, fixed;

int axRaw = 0, ayRaw = 0, azRaw = 0;
int ax = 0, ay = 0, az = 0;
int axv = 0, ayv = 0, azv = 0;
int axp = 0, ayp = 0, azp = 0;

int gxRaw = 0, gyRaw = 0, gzRaw = 0;
int gx = 0, gy = 0, gz = 0;
int gxv = 0, gyv = 0, gzv = 0;
int gxp = 0, gyp = 0, gzp = 0;

int mxRaw = 0, myRaw = 0, mzRaw = 0;
int mx = 0, my = 0, mz = 0;
int mxv = 0, myv = 0, mzv = 0;
int mxp = 0, myp = 0, mzp = 0;

int mouse_x = 0, mouse_y = 0, mouse_z = 0;
int mouse_dx = 0, mouse_dy = 0, mouse_dz = 0;
int scrollx = 0, scrolly = 0;
int scrolldx = 0, scrolldy = 0;

void setup() {
    size(800, 600);
    smooth();

    font128 = loadFont("Calibri-128.vlw");
    font32 = loadFont("Calibri-32.vlw");
    font24 = loadFont("Calibri-24.vlw");
    font16 = loadFont("Calibri-16.vlw");
    font16b = loadFont("Calibri-Bold-16.vlw");
    fixed = loadFont("Consolas-Bold-16.vlw");

    // List all the available serial ports:
    println(Serial.list());
    
    if (Serial.list().length == 0) {
        println("ERROR: No serial ports available. Is the Keyglove plugged in and configured with the serial port interface enabled?");
        noLoop();
    } else {
        port = new Serial(this, Serial.list()[0], 38400);
    }
}

void draw() {
    background(32);

    noStroke();
    fill(255, 225);

    // accelerometer info
    fill(0);
    rect(25, ACCEL_Y - 100, 570, 200);
    fill(255, 255);
    rect(25, ACCEL_Y - 1, 570, 3);

    fill(255, 0, 0, 100);
    rect(50, ACCEL_Y, 20, max(-100, min(100, axRaw)));
    fill(255, 0, 0, 255);
    rect(50, ACCEL_Y, 20, max(-100, min(100, ax)));
    rect(250, ACCEL_Y, 20, max(-100, min(100, axv)));
    rect(450, ACCEL_Y, 20, max(-100, min(100, axp)));

    fill(0, 255, 0, 100);
    rect(100, ACCEL_Y, 20, max(-100, min(100, ayRaw)));
    fill(0, 255, 0, 255);
    rect(100, ACCEL_Y, 20, max(-100, min(100, ay)));
    rect(300, ACCEL_Y, 20, max(-100, min(100, ayv)));
    rect(500, ACCEL_Y, 20, max(-100, min(100, ayp)));

    fill(0, 0, 255, 100);
    rect(150, ACCEL_Y, 20, max(-100, min(100, azRaw)));
    fill(0, 0, 255, 255);
    rect(150, ACCEL_Y, 20, max(-100, min(100, az)));
    rect(350, ACCEL_Y, 20, max(-100, min(100, azv)));
    rect(550, ACCEL_Y, 20, max(-100, min(100, azp)));

    fill(255);
    textAlign(LEFT);
    textFont(font16b);
    text("Accelerometer", 25, ACCEL_Y - 102);
    textFont(font16);
    text("x", 32, ACCEL_Y - 4);
    text("y", 82, ACCEL_Y - 4);
    text("z", 132, ACCEL_Y - 4);
    text("xV", 232, ACCEL_Y - 4);
    text("yV", 282, ACCEL_Y - 4);
    text("zV", 332, ACCEL_Y - 4);
    text("xP", 432, ACCEL_Y - 4);
    text("yP", 482, ACCEL_Y - 4);
    text("zP", 532, ACCEL_Y - 4);

    // gyroscope info
    fill(0);
    rect(25, GYRO_Y - 100, 570, 200);
    fill(255, 255);
    rect(25, GYRO_Y - 1, 570, 3);

    fill(255, 0, 0, 100);
    rect(50, GYRO_Y, 20, max(-100, min(100, gxRaw)));
    fill(255, 0, 0, 255);
    rect(50, GYRO_Y, 20, max(-100, min(100, gx)));
    rect(250, GYRO_Y, 20, max(-100, min(100, gxv)));
    rect(450, GYRO_Y, 20, max(-100, min(100, gxp)));

    fill(0, 255, 0, 100);
    rect(100, GYRO_Y, 20, max(-100, min(100, gyRaw)));
    fill(0, 255, 0, 255);
    rect(100, GYRO_Y, 20, max(-100, min(100, gy)));
    rect(300, GYRO_Y, 20, max(-100, min(100, gyv)));
    rect(500, GYRO_Y, 20, max(-100, min(100, gyp)));

    fill(0, 0, 255, 100);
    rect(150, GYRO_Y, 20, max(-100, min(100, gzRaw)));
    fill(0, 0, 255, 255);
    rect(150, GYRO_Y, 20, max(-100, min(100, gz)));
    rect(350, GYRO_Y, 20, max(-100, min(100, gzv)));
    rect(550, GYRO_Y, 20, max(-100, min(100, gzp)));

    fill(255);
    textAlign(LEFT);
    textFont(font16b);
    text("Gyroscope", 25, GYRO_Y - 102);
    textFont(font16);
    text("x", 32, GYRO_Y - 4);
    text("y", 82, GYRO_Y - 4);
    text("z", 132, GYRO_Y - 4);
    text("xV", 232, GYRO_Y - 4);
    text("yV", 282, GYRO_Y - 4);
    text("zV", 332, GYRO_Y - 4);
    text("xP", 432, GYRO_Y - 4);
    text("yP", 482, GYRO_Y - 4);
    text("zP", 532, GYRO_Y - 4);

    // check for serial data availability
    while (port.available() > 0) {
        serialEvent(port.read());
    }
}

void serialEvent(int ch) {
    if (ch != '\n' && ch != '\r') {
        buff += char(ch);
    } else if (buff.length() > 0) {
        String[] vs = buff.split("\t");
        println(buff);
        buff = "";
        String cmdType = vs[0].trim();
        try {
            if (cmdType.equals("accel") && vs.length > 12) {
                axRaw = Integer.parseInt(vs[1].trim());
                ayRaw = Integer.parseInt(vs[2].trim());
                azRaw = Integer.parseInt(vs[3].trim());
                ax = Integer.parseInt(vs[4].trim());
                ay = Integer.parseInt(vs[5].trim());
                az = Integer.parseInt(vs[6].trim());
                axv = Integer.parseInt(vs[7].trim());
                ayv = Integer.parseInt(vs[8].trim());
                azv = Integer.parseInt(vs[9].trim());
                axp = Integer.parseInt(vs[10].trim());
                ayp = Integer.parseInt(vs[11].trim());
                azp = Integer.parseInt(vs[12].trim());
            } else if (cmdType.equals("gyro") && vs.length > 12) {
                gxRaw = Integer.parseInt(vs[1].trim());
                gyRaw = Integer.parseInt(vs[2].trim());
                gzRaw = Integer.parseInt(vs[3].trim());
                gx = Integer.parseInt(vs[4].trim());
                gy = Integer.parseInt(vs[5].trim());
                gz = Integer.parseInt(vs[6].trim());
                gxv = Integer.parseInt(vs[7].trim());
                gyv = Integer.parseInt(vs[8].trim());
                gzv = Integer.parseInt(vs[9].trim());
                gxp = Integer.parseInt(vs[10].trim());
                gyp = Integer.parseInt(vs[11].trim());
                gzp = Integer.parseInt(vs[12].trim());
            } else if (cmdType.equals("mag") && vs.length > 6) {
                mxRaw = Integer.parseInt(vs[1].trim());
                myRaw = Integer.parseInt(vs[2].trim());
                mzRaw = Integer.parseInt(vs[3].trim());
                mx = Integer.parseInt(vs[4].trim());
                my = Integer.parseInt(vs[5].trim());
                mz = Integer.parseInt(vs[6].trim());
            } else if (cmdType.equals("mouse") && vs.length > 6) {
                mouse_x = Integer.parseInt(vs[1].trim());
                mouse_y = Integer.parseInt(vs[2].trim());
                mouse_z = Integer.parseInt(vs[3].trim());
                mouse_dx = Integer.parseInt(vs[4].trim());
                mouse_dy = Integer.parseInt(vs[5].trim());
                mouse_dx = Integer.parseInt(vs[6].trim());
            } else if (cmdType.equals("scroll") && vs.length > 4) {
                scrollx = Integer.parseInt(vs[1].trim());
                scrolly = Integer.parseInt(vs[2].trim());
                scrolldx = Integer.parseInt(vs[3].trim());
                scrolldy = Integer.parseInt(vs[4].trim());
            }
        } catch (NumberFormatException e) {
        }
    }
}
