#!/usr/bin/env python

""" Keyglove API test/demo script for raw HID interface (Windows)

Changelog:
    2013-11-20 - Bundled clean serial + HID transport into KGLib code
               - Added send_and_return() method for easier response handling
    2013-11-16 - Initial release

============================================
Keyglove Protocol Python interface library
2013-11-20 by Jeff Rowberg <jeff@rowberg.net>
Updates should (hopefully) always be available at https://github.com/jrowberg/keyglove

============================================
Keyglove Protocol Python interface library code is placed under the MIT license
Copyright (c) 2013 Jeff Rowberg

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

"""

__author__ = "Jeff Rowberg"
__license__ = "MIT"
__version__ = "2013-11-20"
__email__ = "jeff@rowberg.net"

import kglib, pywinusb.hid as hid, time, datetime, optparse, signal

keyglove = 0
hidobj = 0

# handler for reading incoming raw HID packets
def keyglove_rawhid_handler(data):
    global keyglove
    if data[0] == 0x00 and data[1] < len(data) - 1:
        for b in data[2:data[1] + 2]:
            keyglove.parse(b)
    #print("Raw data ({0}): {1}".format(len(data), data))

# handler to notify of an API parser timeout condition
def my_timeout(sender, args):
    # might want to try the following lines to reset, though it probably
    # wouldn't work at this point if it's already timed out:
    #keyglove.send_command(ser, keyglove.kg_cmd_system_reset())
    #keyglove.check_activity(ser, 1)
    print "Communication timed out. Make sure the Keyglove device is in a known/idle state."

def my_kg_evt_protocol_error(sender, args):
    print("kg_evt_protocol_error: { code: %04X }" % (args['code']))

def my_kg_rsp_system_ping(sender, args):
    print("kg_rsp_system_ping: { runtime: %08X }" % (args['runtime']))

def my_kg_rsp_system_reset(sender, args):
    print("kg_rsp_system_reset: { result: %04X }" % (args['result']))

def my_kg_evt_system_boot(sender, args):
    print("kg_evt_system_boot: {  }")

def my_kg_evt_system_ready(sender, args):
    print("kg_evt_system_ready: {  }")

def my_kg_evt_system_error(sender, args):
    print("kg_evt_system_error: { code: %04X }" % (args['code']))

def my_kg_rsp_touch_get_mode(sender, args):
    print("kg_rsp_touch_get_mode: { mode: %02X }" % (args['mode']))

def my_kg_rsp_touch_set_mode(sender, args):
    print("kg_rsp_touch_set_mode: { result: %04X }" % (args['result']))

def my_kg_evt_touch_mode(sender, args):
    print("kg_evt_touch_mode: { mode: %02X }" % (args['mode']))

def my_kg_evt_touch_status(sender, args):
    print("kg_evt_touch_status: { status: %s }" % (' '.join(['%02X' % b for b in args['status']])))

def my_kg_rsp_feedback_get_blink_mode(sender, args):
    print("kg_rsp_feedback_get_blink_mode: { mode: %02X }" % (args['mode']))

def my_kg_rsp_feedback_set_blink_mode(sender, args):
    print("kg_rsp_feedback_set_blink_mode: { result: %04X }" % (args['result']))

def my_kg_rsp_feedback_get_piezo_mode(sender, args):
    print("kg_rsp_feedback_get_piezo_mode: { mode: %02X, duration: %02X, frequency: %04X }" % (args['mode'], args['duration'], args['frequency']))

def my_kg_rsp_feedback_set_piezo_mode(sender, args):
    print("kg_rsp_feedback_set_piezo_mode: { result: %04X }" % (args['result']))

def my_kg_rsp_feedback_get_vibe_mode(sender, args):
    print("kg_rsp_feedback_get_vibe_mode: { mode: %02X, duration: %02X }" % (args['mode'], args['duration']))

def my_kg_rsp_feedback_set_vibe_mode(sender, args):
    print("kg_rsp_feedback_set_vibe_mode: { result: %04X }" % (args['result']))

def my_kg_rsp_feedback_get_rgb_mode(sender, args):
    print("kg_rsp_feedback_get_rgb_mode: { mode_red: %02X, mode_green: %02X, mode_blue: %02X }" % (args['mode_red'], args['mode_green'], args['mode_blue']))

def my_kg_rsp_feedback_set_rgb_mode(sender, args):
    print("kg_rsp_feedback_set_rgb_mode: { result: %04X }" % (args['result']))

def my_kg_evt_feedback_blink_mode(sender, args):
    print("kg_evt_feedback_blink_mode: { mode: %02X }" % (args['mode']))

def my_kg_evt_feedback_piezo_mode(sender, args):
    print("kg_evt_feedback_piezo_mode: { index: %02X, mode: %02X, duration: %02X, frequency: %04X }" % (args['index'], args['mode'], args['duration'], args['frequency']))

def my_kg_evt_feedback_vibe_mode(sender, args):
    print("kg_evt_feedback_vibe_mode: { index: %02X, mode: %02X, duration: %02X }" % (args['index'], args['mode'], args['duration']))

def my_kg_evt_feedback_rgb_mode(sender, args):
    print("kg_evt_feedback_rgb_mode: { index: %02X, mode_red: %02X, mode_green: %02X, mode_blue: %02X }" % (args['index'], args['mode_red'], args['mode_green'], args['mode_blue']))

def my_kg_rsp_motion_get_mode(sender, args):
    print("kg_rsp_motion_get_mode: { mode: %02X }" % (args['mode']))

def my_kg_rsp_motion_set_mode(sender, args):
    print("kg_rsp_motion_set_mode: { result: %04X }" % (args['result']))

def my_kg_evt_motion_mode(sender, args):
    print("kg_evt_motion_mode: { index: %02X, mode: %02X }" % (args['index'], args['mode']))

def my_kg_evt_motion_data(sender, args):
    print("kg_evt_motion_data: { index: %02X, flags: %02X, data: %s }" % (args['index'], args['flags'], ' '.join(['%02X' % b for b in args['data']])))

def main():
    global keyglove, hidobj

    # create option parser
    p = optparse.OptionParser(description='Keyglove API Raw HID Communication Demo for Teensy/Windows (' + __version__ + ')')

    # set defaults for options
    p.set_defaults(debug=False)

    # create serial port options argument group
    group = optparse.OptionGroup(p, "Connection Options")
    group.add_option('--debug', '-d', action="store_true", help="Debug mode (show raw RX/TX API packets)")
    p.add_option_group(group)

    # actually parse all of the arguments
    options, arguments = p.parse_args()

    # create and setup KGLib object
    keyglove = kglib.KGLib()
    keyglove.debug = options.debug

    # add handler for KGAPI timeout condition (hopefully won't happen)
    keyglove.on_timeout += my_timeout

    # add handlers for BGAPI events
    keyglove.kg_evt_protocol_error += my_kg_evt_protocol_error
    keyglove.kg_rsp_system_ping += my_kg_rsp_system_ping
    keyglove.kg_rsp_system_reset += my_kg_rsp_system_reset
    keyglove.kg_evt_system_boot += my_kg_evt_system_boot
    keyglove.kg_evt_system_ready += my_kg_evt_system_ready
    keyglove.kg_evt_system_error += my_kg_evt_system_error
    keyglove.kg_rsp_touch_get_mode += my_kg_rsp_touch_get_mode
    keyglove.kg_rsp_touch_set_mode += my_kg_rsp_touch_set_mode
    keyglove.kg_evt_touch_mode += my_kg_evt_touch_mode
    keyglove.kg_evt_touch_status += my_kg_evt_touch_status
    keyglove.kg_rsp_feedback_get_blink_mode += my_kg_rsp_feedback_get_blink_mode
    keyglove.kg_rsp_feedback_set_blink_mode += my_kg_rsp_feedback_set_blink_mode
    keyglove.kg_rsp_feedback_get_piezo_mode += my_kg_rsp_feedback_get_piezo_mode
    keyglove.kg_rsp_feedback_set_piezo_mode += my_kg_rsp_feedback_set_piezo_mode
    keyglove.kg_rsp_feedback_get_vibe_mode += my_kg_rsp_feedback_get_vibe_mode
    keyglove.kg_rsp_feedback_set_vibe_mode += my_kg_rsp_feedback_set_vibe_mode
    keyglove.kg_rsp_feedback_get_rgb_mode += my_kg_rsp_feedback_get_rgb_mode
    keyglove.kg_rsp_feedback_set_rgb_mode += my_kg_rsp_feedback_set_rgb_mode
    keyglove.kg_evt_feedback_blink_mode += my_kg_evt_feedback_blink_mode
    keyglove.kg_evt_feedback_piezo_mode += my_kg_evt_feedback_piezo_mode
    keyglove.kg_evt_feedback_vibe_mode += my_kg_evt_feedback_vibe_mode
    keyglove.kg_evt_feedback_rgb_mode += my_kg_evt_feedback_rgb_mode
    keyglove.kg_rsp_motion_get_mode += my_kg_rsp_motion_get_mode
    keyglove.kg_rsp_motion_set_mode += my_kg_rsp_motion_set_mode
    keyglove.kg_evt_motion_mode += my_kg_evt_motion_mode
    keyglove.kg_evt_motion_data += my_kg_evt_motion_data

    # create hid object (Teensy in Raw HID mode is 16C0:0486)
    all_hidobjs = hid.HidDeviceFilter(vendor_id=0x16C0, product_id=0x0486).get_devices()
    if all_hidobjs:
        print("Listing HID entries matching 16C0:0486...\n")
        use_id = -1
        for index, device in enumerate(all_hidobjs):
            device_name = unicode("{0.vendor_name}, {0.product_name}" \
                    " (vID=0x{1:04x}, pID=0x{2:04x})" \
                    "".format(device, device.vendor_id, device.product_id))
            print("{0} => {1}".format(index, device_name))
            if device.product_name == "Teensyduino RawHID":
                use_id = index
                print "^^^ using this device"
                break

        if use_id >= 0:
            hidobj = all_hidobjs[use_id]
            try:
                # open HID device
                hidobj.open();
                hidobj.set_raw_data_handler(keyglove_rawhid_handler)

                # find correct output report entry to use for sending data
                target_usage = hid.get_full_usage_id(0xffab, 0x02)
                output_report = None
                for report in hidobj.find_output_reports():
                    if target_usage in report:
                        output_report = report

                # reboot Keyglove
                print "Resetting Keyglove..."
                response = keyglove.send_and_return(output_report, keyglove.kg_cmd_system_reset(), 1)
                print "Done!"

                # loop forever until Ctrl+C or device is unplugged
                while device.is_plugged():
                    time.sleep(0.01)

            finally:
                hidobj.close();

        else:
            print("Keyglove raw HID interface not available!")
    else:
        print("There are no HID-class devices available!")

# gracefully exit without a big exception message if possible
def ctrl_c_handler(signal, frame):
    print 'Goodbye!'
    exit(0)

signal.signal(signal.SIGINT, ctrl_c_handler)

if __name__ == '__main__':
    main()
