// Keyglove Controller source code - Arduino sketch definition file for Keyglove firmware
// 2014-11-07 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2014 Jeff Rowberg

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

/**
 * @file keyglove.ino
 * @brief Arduino sketch definition file for Keyglove firmware
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * This file is the entry point for the Arduino sketch, but all real code has
 * been moved to .cpp/.h files for better organization. See the following link:
 * http://www.gammon.com.au/forum/?id=12625 - Thanks, Nick!
 *
 * Normally it is not necessary to edit this file, unless you add new Arduino
 * libraries for certain functionality.
 */

/* ===============================================
 * LIBRARY INCLUDES FOR PROPER BUILD PROCESS
=============================================== */

#include <Wire.h>       // Core Arduino I2C hardware library
#include <I2Cdev.h>     // I2Cdevlib device wrapper library
#include <MPU6050.h>    // MPU-6050 I2C sensor support for I2Cdevlib
#include <iWRAP.h>      // Bluegiga iWRAP parser library
#include <EEPROM.h>     // Core Arduino EEPROM library
