#!/usr/bin/env python

"""
================================================================================
Keyglove host control console demo application
2014-12-06 by Jeff Rowberg <jeff@rowberg.net>

Changelog:
    2014-12-06 - Initial release

================================================================================
Keyglove source code is placed under the MIT license
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

================================================================================

"""

__author__ = "Jeff Rowberg"
__license__ = "MIT"
__version__ = "2014-12-06"
__email__ = "jeff@rowberg.net"

import sys, signal, time
import kglib

keyglove = None         # Keyglove device instance (see 'kglib.KeygloveDevice')
kgapi = kglib.KGAPI()   # API protocol parser instance (see 'kglib.KGAPI')

# ==============================================================================
# DEVICE CONNECTION MANAGEMENT EVENTS (triggered from KeygloveDevice object)
# ==============================================================================

def my_on_connected(sender, args):
    print("+++ Keyglove connected")

def my_on_disconnected(sender, args):
    print("--- Keyglove disconnected")

def my_on_unplugged(sender, args):
    print("!!! Keyglove device unplugged, no communication possible at this time")

def my_on_api_timeout(sender, args):
    print("??? API command timed out waiting for response, device may be disconnected")

# ==============================================================================
# API COMMUNICATION CALLBACKS (assigned in main(), triggered from KGAPI object)
# ==============================================================================

def my_kg_response(sender, args):
    friendly = kgapi.friendly_packet(args["raw"], True)
    if friendly:
        print("%.2f <-- %s { %s }" % (time.time(), friendly["name"], ', '.join([ ("%s: %s" % (x, friendly["payload"][x])) for x in friendly["payload"]])))
    else:
        print("%.2f <-- UNKNOWN RESPONSE: [ %s ]" % (time.time(), ' '.join(['%02X' % b for b in args['raw']])))

def my_kg_event(sender, args):
    friendly = kgapi.friendly_packet(args["raw"], True)
    if friendly:
        print("%.2f <-- %s { %s }" % (time.time(), friendly["name"], ', '.join([ ("%s: %s" % (x, friendly["payload"][x])) for x in friendly["payload"]])))
    else:
        print("%.2f <-- UNKNOWN EVENT: [ %s ]" % (time.time(), ' '.join(['%02X' % b for b in args['raw']])))

def my_kg_log(sender, args):
    print("%.2f <-- LOG (%d): %s" % (time.time(), args["level"], args["log"]))

def my_kg_evt_touch_status(sender, args):
    print("%.2f *** Touch update: %s" % (time.time(), ' '.join(['%02X' % b for b in args['status']])))

# ==============================================================================
# MAIN APPLICATION LOGIC
# ==============================================================================

def main():
    global keyglove

    print("=================================")
    print("Keyglove Console Application Stub")
    print("=================================\n")

    # show available transports/backends
    print("USB options: %s" % kglib.usb_options)
    print("USB selected: %s" % kglib.usb_selected)
    print("Serial options: %s" % kglib.serial_options)
    print("Serial selected: %s\n" % kglib.serial_selected)

    # list all available devices
    print("Keyglove devices:")
    index = 0
    devices = list(kglib.get_devices())
    if len(devices) > 0:
        for device in devices:
            print("    #%d: %s @ %s" % (index, device.description, device.port))
            index = index + 1
        print("")
    else:
        # no devices available (no backends or nothing connected)
        print("    No devices found!")
        print("    Please make sure Keyglove is connected and")
        print("    PySerial, PyWinUSB, and/or PyUSB is available")
        exit(1)

    # attempt to connect and use Keyglove
    print("Connecting to first device in list...")
    keyglove = devices[0]
    try:
        # assign custom event handlers for connection/disconnection/unplug
        keyglove.on_connected += my_on_connected
        keyglove.on_disconnected += my_on_disconnected
        keyglove.on_unplugged += my_on_unplugged
        keyglove.on_api_timeout += my_on_api_timeout

        # assign custom generic handlers for responses, events, and log entries
        kgapi.kg_response += my_kg_response
        kgapi.kg_event += my_kg_event
        kgapi.kg_log += my_kg_log

        # assign custom event handler for kg_evt_touch_status() API event
        kgapi.kg_evt_touch_status += my_kg_evt_touch_status

        if keyglove.connect(kgapi):
            # repeatedly ping Keyglove device (SIGINT/Ctrl+C will break)
            while keyglove.connected:
                # build outgoing command packet
                cmd_packet = kgapi.kg_cmd_system_ping()

                # pass raw command to "friendly" interpreter in KGAPI object
                # (this is totally optional and mostly just helps debug output)
                friendly = kgapi.friendly_packet(cmd_packet, False)
                if friendly:
                    print("%.2f --> %s { %s }" % (time.time(), friendly["name"], ', '.join([ ("%s: %s" % (x, friendly["payload"][x])) for x in friendly["payload"]])))
                else:
                    print("%.2f --> UNKNOWN COMMAND: [ %s ]" % (time.time(), ' '.join(['%02X' % b for b in cmd_packet])))

                # send command and wait up to 1 second for response
                response = keyglove.send_and_return(cmd_packet, 1)

                # wait 5 seconds before repeating
                if keyglove.connected: time.sleep(5)

    except kglib.KeygloveError as e:
        print("Keyglove error (%s): %s" % (type(e), e))

# ==============================================================================
# PYTHON "__main__" ENTRY POINT DEFINITION
# ==============================================================================

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        if keyglove != None and keyglove.connected:
            print("Disconnecting from Keyglove...")
            if not keyglove.disconnect():
                print("Could not disconnect!")

        print("Goodbye!")
        sys.exit(0)
