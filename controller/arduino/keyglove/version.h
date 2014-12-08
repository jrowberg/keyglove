// Keyglove controller source code - Firmware version info definitions
// 2014-12-07 by Jeff Rowberg <jeff@rowberg.net>

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
 * @file version.h
 * @brief **USER-DEFINED:** Firmware version info definitions
 * @author Jeff Rowberg
 * @date 2014-12-07
 *
 * This short file holds firmware version data in both string format and numeric
 * format. This is reported in the `system_boot` KGAPI event, and in the
 * response sent when the `system_get_info` KGAPI command is received.
 */

#ifndef _VERSION_H_
#define _VERSION_H_

// info included in system_boot() event
#define KG_FIRMWARE_VERSION_MAJOR 0                         ///< Firmware major version number
#define KG_FIRMWARE_VERSION_MINOR 5                         ///< Firmware minor version number
#define KG_FIRMWARE_VERSION_PATCH 0                         ///< Firmware patch version number
#define KG_PROTOCOL_VERSION 1                               ///< API protocol version
#define KG_BUILD_TIMESTAMP 1415420841                       ///< UNIX timestamp for current build

// info available for reference, not in system_boot() event
#define KG_FIRMWARE_NAME_STRING "Keyglove Official v0.5.0"  ///< Firmware name string
#define KG_FIRMWARE_AUTHOR_STRING "Jeff Rowberg"            ///< Firmware author string
#define KG_FIRMWARE_DATE_STRING "2014-12-08"                ///< Firmware date string

#endif // _VERSION_H_
 