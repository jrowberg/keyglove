#!/usr/bin/env python

""" Keyglove API test/demo console script for serial interface

Changelog:
    2013-11-20 - Bundled clean serial + HID transport into KGLib code
               - Added send_and_return() method for easier response handling
    2013-11-16 - Initial release

============================================
Keyglove Protocol Python interface library
2014-08-02 by Jeff Rowberg <jeff@rowberg.net>
Updates should (hopefully) always be available at https://github.com/jrowberg/keyglove

============================================
Keyglove Protocol Python interface library code is placed under the MIT license
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

"""

__author__ = "Jeff Rowberg"
__license__ = "MIT"
__version__ = "2014-08-02"
__email__ = "jeff@rowberg.net"

import kglib, serial, time, datetime, optparse, signal

keyglove = 0
ser = 0

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

def my_kg_rsp_system_get_info(sender, args):
    print("kg_rsp_system_get_info: { major: %02X, minor: %02X, patch: %02X, timestamp: %08X }" % (args['major'], args['minor'], args['patch'], args['timestamp']))

def my_kg_evt_system_boot(sender, args):
    print("kg_evt_system_boot: { major: %02X, minor: %02X, patch: %02X, timestamp: %08X }" % (args['major'], args['minor'], args['patch'], args['timestamp']))

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

def my_kg_rsp_feedback_get_vibrate_mode(sender, args):
    print("kg_rsp_feedback_get_vibrate_mode: { mode: %02X, duration: %02X }" % (args['mode'], args['duration']))

def my_kg_rsp_feedback_set_vibrate_mode(sender, args):
    print("kg_rsp_feedback_set_vibrate_mode: { result: %04X }" % (args['result']))

def my_kg_rsp_feedback_get_rgb_mode(sender, args):
    print("kg_rsp_feedback_get_rgb_mode: { mode_red: %02X, mode_green: %02X, mode_blue: %02X }" % (args['mode_red'], args['mode_green'], args['mode_blue']))

def my_kg_rsp_feedback_set_rgb_mode(sender, args):
    print("kg_rsp_feedback_set_rgb_mode: { result: %04X }" % (args['result']))

def my_kg_evt_feedback_blink_mode(sender, args):
    print("kg_evt_feedback_blink_mode: { mode: %02X }" % (args['mode']))

def my_kg_evt_feedback_piezo_mode(sender, args):
    print("kg_evt_feedback_piezo_mode: { index: %02X, mode: %02X, duration: %02X, frequency: %04X }" % (args['index'], args['mode'], args['duration'], args['frequency']))

def my_kg_evt_feedback_vibrate_mode(sender, args):
    print("kg_evt_feedback_vibrate_mode: { index: %02X, mode: %02X, duration: %02X }" % (args['index'], args['mode'], args['duration']))

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

def my_kg_rsp_bluetooth_get_mode(sender, args):
    print("kg_rsp_bluetooth_get_mode: { result: %04X, mode: %02X }" % (args['result'], args['mode']))

def my_kg_rsp_bluetooth_set_mode(sender, args):
    print("kg_rsp_bluetooth_set_mode: { result: %04X }" % (args['result']))

def my_kg_rsp_bluetooth_reset(sender, args):
    print("kg_rsp_bluetooth_reset: { result: %04X }" % (args['result']))

def my_kg_rsp_bluetooth_get_mac(sender, args):
    print("kg_rsp_bluetooth_get_mac: { result: %04X, address: %s }" % (args['result'], ' '.join(['%02X' % b for b in args['address']])))

def my_kg_rsp_bluetooth_get_pairings(sender, args):
    print("kg_rsp_bluetooth_get_pairings: { result: %04X, count: %02X }" % (args['result'], args['count']))

def my_kg_rsp_bluetooth_discover(sender, args):
    print("kg_rsp_bluetooth_discover: { result: %04X }" % (args['result']))

def my_kg_rsp_bluetooth_pair(sender, args):
    print("kg_rsp_bluetooth_pair: { result: %04X }" % (args['result']))

def my_kg_rsp_bluetooth_delete_pairing(sender, args):
    print("kg_rsp_bluetooth_delete_pairing: { result: %04X }" % (args['result']))

def my_kg_rsp_bluetooth_clear_pairings(sender, args):
    print("kg_rsp_bluetooth_clear_pairings: { result: %04X }" % (args['result']))

def my_kg_rsp_bluetooth_get_connections(sender, args):
    print("kg_rsp_bluetooth_get_connections: { result: %04X, count: %02X }" % (args['result'], args['count']))

def my_kg_rsp_bluetooth_connect(sender, args):
    print("kg_rsp_bluetooth_connect: { result: %04X }" % (args['result']))

def my_kg_rsp_bluetooth_disconnect(sender, args):
    print("kg_rsp_bluetooth_disconnect: { result: %04X }" % (args['result']))

def my_kg_evt_bluetooth_mode(sender, args):
    print("kg_evt_bluetooth_mode: { mode: %02X }" % (args['mode']))

def my_kg_evt_bluetooth_ready(sender, args):
    print("kg_evt_bluetooth_ready: {  }")

def my_kg_evt_bluetooth_inquiry_response(sender, args):
    print("kg_evt_bluetooth_inquiry_response: { address: %s, cod: %s, rssi: %02X, status: %02X, index: %02X, name: %s }" % (' '.join(['%02X' % b for b in args['address']]), ' '.join(['%02X' % b for b in args['cod']]), args['rssi'], args['status'], args['index'], ' '.join(['%02X' % b for b in args['name']])))

def my_kg_evt_bluetooth_inquiry_complete(sender, args):
    print("kg_evt_bluetooth_inquiry_complete: { count: %02X }" % (args['count']))

def my_kg_evt_bluetooth_pairing_status(sender, args):
    print("kg_evt_bluetooth_pairing_status: { index: %02X, address: %s, priority: %02X, profiles_supported: %02X, profiles_active: %02X, handle_list: %s }" % (args['index'], ' '.join(['%02X' % b for b in args['address']]), args['priority'], args['profiles_supported'], args['profiles_active'], ' '.join(['%02X' % b for b in args['handle_list']])))

def my_kg_evt_bluetooth_pairing_failed(sender, args):
    print("kg_evt_bluetooth_pairing_failed: { address: %s }" % (' '.join(['%02X' % b for b in args['address']])))

def my_kg_evt_bluetooth_pairings_cleared(sender, args):
    print("kg_evt_bluetooth_pairings_cleared: {  }")

def my_kg_evt_bluetooth_connection_status(sender, args):
    print("kg_evt_bluetooth_connection_status: { handle: %02X, address: %s, index: %02X, profile: %02X, status: %02X }" % (args['handle'], ' '.join(['%02X' % b for b in args['address']]), args['index'], args['profile'], args['status']))

def my_kg_evt_bluetooth_connection_closed(sender, args):
    print("kg_evt_bluetooth_connection_closed: { handle: %02X, reason: %04X }" % (args['handle'], args['reason']))

def my_kg_log(sender, args):
    print("*** LOG (%d): %s" % (args['level'], args['message']))

def main():
    global keyglove, ser

    # create option parser
    p = optparse.OptionParser(description='Keyglove API Serial Communication Demo (' + __version__ + ')')

    # set defaults for options
    p.set_defaults(port="/dev/ttyACM0", baud=115200, debug=False)

    # create serial port options argument group
    group = optparse.OptionGroup(p, "Connection Options")
    group.add_option('--port', '-p', type="string", help="Serial port device name (default /dev/ttyACM0)", metavar="PORT")
    group.add_option('--baud', '-b', type="int", help="Serial port baud rate (default 115200)", metavar="BAUD")
    group.add_option('--debug', '-d', action="store_true", help="Debug mode (show raw RX/TX API packets)")
    p.add_option_group(group)

    # actually parse all of the arguments
    options, arguments = p.parse_args()

    # create and setup KGLib object
    keyglove = kglib.KGLib()
    keyglove.debug = options.debug

    # add handler for KGAPI timeout condition (hopefully won't happen)
    keyglove.on_timeout += my_timeout

    # add handlers for KGAPI events
    keyglove.kg_evt_protocol_error += my_kg_evt_protocol_error
    keyglove.kg_rsp_system_ping += my_kg_rsp_system_ping
    keyglove.kg_rsp_system_reset += my_kg_rsp_system_reset
    keyglove.kg_rsp_system_get_info += my_kg_rsp_system_get_info
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
    keyglove.kg_rsp_feedback_get_vibrate_mode += my_kg_rsp_feedback_get_vibrate_mode
    keyglove.kg_rsp_feedback_set_vibrate_mode += my_kg_rsp_feedback_set_vibrate_mode
    keyglove.kg_rsp_feedback_get_rgb_mode += my_kg_rsp_feedback_get_rgb_mode
    keyglove.kg_rsp_feedback_set_rgb_mode += my_kg_rsp_feedback_set_rgb_mode
    keyglove.kg_evt_feedback_blink_mode += my_kg_evt_feedback_blink_mode
    keyglove.kg_evt_feedback_piezo_mode += my_kg_evt_feedback_piezo_mode
    keyglove.kg_evt_feedback_vibrate_mode += my_kg_evt_feedback_vibrate_mode
    keyglove.kg_evt_feedback_rgb_mode += my_kg_evt_feedback_rgb_mode
    keyglove.kg_rsp_motion_get_mode += my_kg_rsp_motion_get_mode
    keyglove.kg_rsp_motion_set_mode += my_kg_rsp_motion_set_mode
    keyglove.kg_evt_motion_mode += my_kg_evt_motion_mode
    keyglove.kg_evt_motion_data += my_kg_evt_motion_data
    keyglove.kg_rsp_bluetooth_get_mode += my_kg_rsp_bluetooth_get_mode
    keyglove.kg_rsp_bluetooth_set_mode += my_kg_rsp_bluetooth_set_mode
    keyglove.kg_rsp_bluetooth_reset += my_kg_rsp_bluetooth_reset
    keyglove.kg_rsp_bluetooth_get_mac += my_kg_rsp_bluetooth_get_mac
    keyglove.kg_rsp_bluetooth_get_pairings += my_kg_rsp_bluetooth_get_pairings
    keyglove.kg_rsp_bluetooth_discover += my_kg_rsp_bluetooth_discover
    keyglove.kg_rsp_bluetooth_pair += my_kg_rsp_bluetooth_pair
    keyglove.kg_rsp_bluetooth_delete_pairing += my_kg_rsp_bluetooth_delete_pairing
    keyglove.kg_rsp_bluetooth_clear_pairings += my_kg_rsp_bluetooth_clear_pairings
    keyglove.kg_rsp_bluetooth_get_connections += my_kg_rsp_bluetooth_get_connections
    keyglove.kg_rsp_bluetooth_connect += my_kg_rsp_bluetooth_connect
    keyglove.kg_rsp_bluetooth_disconnect += my_kg_rsp_bluetooth_disconnect
    keyglove.kg_evt_bluetooth_mode += my_kg_evt_bluetooth_mode
    keyglove.kg_evt_bluetooth_ready += my_kg_evt_bluetooth_ready
    keyglove.kg_evt_bluetooth_inquiry_response += my_kg_evt_bluetooth_inquiry_response
    keyglove.kg_evt_bluetooth_inquiry_complete += my_kg_evt_bluetooth_inquiry_complete
    keyglove.kg_evt_bluetooth_pairing_status += my_kg_evt_bluetooth_pairing_status
    keyglove.kg_evt_bluetooth_pairing_failed += my_kg_evt_bluetooth_pairing_failed
    keyglove.kg_evt_bluetooth_pairings_cleared += my_kg_evt_bluetooth_pairings_cleared
    keyglove.kg_evt_bluetooth_connection_status += my_kg_evt_bluetooth_connection_status
    keyglove.kg_evt_bluetooth_connection_closed += my_kg_evt_bluetooth_connection_closed

    # add handler for KGAPI log packet
    keyglove.kg_log += my_kg_log
    
    # create serial port object
    try:
        ser = serial.Serial(port=options.port, baudrate=options.baud, timeout=1, writeTimeout=1)
    except serial.SerialException as e:
        print "\n================================================================"
        print "Port error (name='%s', baud='%ld'): %s" % (options.port, options.baud, e)
        print "================================================================"
        exit(2)

    # flush buffers
    ser.flushInput()
    ser.flushOutput()

    # reboot Keyglove
    print "Resetting Keyglove..."
    response = keyglove.send_and_return(ser, keyglove.kg_cmd_system_reset(1), 1)
    print "Done!"

    while (1):
        # check for all incoming data (no timeout, non-blocking)
        keyglove.check_activity(ser)

        # don't burden the CPU
        time.sleep(0.01)

# gracefully exit without a big exception message if possible
def ctrl_c_handler(signal, frame):
    print 'Goodbye!'
    exit(0)

signal.signal(signal.SIGINT, ctrl_c_handler)

if __name__ == '__main__':
    main()
