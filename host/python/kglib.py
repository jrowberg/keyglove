#!/usr/bin/env python

"""
================================================================================
Keyglove Protocol Python interface library
2014-12-06 by Jeff Rowberg <jeff@rowberg.net>

Changelog:
    2014-12-06 - Separated communication code from parser/generator code
               - Added transparent support for PySerial/PyUSB/PyWinUSB
    2013-11-20 - Bundled clean serial + HID transport into KGLib code
               - Added send_and_return() method for easier response handling
    2013-11-16 - Initial release

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

import struct, time



# thanks to Masaaki Shibata for Python event handler code
# http://www.emptypage.jp/notes/pyevent.en.html

class KeygloveEvent(object):

    def __init__(self, doc=None):
        self.__doc__ = doc

    def __get__(self, obj, objtype=None):
        if obj is None:
            return self
        return KeygloveEventHandler(self, obj)

    def __set__(self, obj, value):
        pass



class KeygloveEventHandler(object):

    def __init__(self, event, obj):

        self.event = event
        self.obj = obj

    def _getfunctionlist(self):

        """(internal use) """

        try:
            eventhandler = self.obj.__eventhandler__
        except AttributeError:
            eventhandler = self.obj.__eventhandler__ = {}
        return eventhandler.setdefault(self.event, [])

    def add(self, func):

        """Add new event handler function.

        Event handler function must be defined like func(sender, earg).
        You can add handler also by using '+=' operator.
        """

        self._getfunctionlist().append(func)
        return self

    def remove(self, func):

        """Remove existing event handler function.

        You can remove handler also by using '-=' operator.
        """

        self._getfunctionlist().remove(func)
        return self

    def fire(self, earg=None):

        """Fire event and call all handler functions

        You can call EventHandler object itself like e(earg) instead of
        e.fire(earg).
        """

        for func in self._getfunctionlist():
            func(self.obj, earg)

    __iadd__ = add
    __isub__ = remove
    __call__ = fire



class KGLib(object):

    def kg_cmd_system_ping(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x01, 0x01)
    def kg_cmd_system_reset(self, type):
        return struct.pack('<4BB', 0xC0, 0x01, 0x01, 0x02, type)
    def kg_cmd_system_get_info(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x01, 0x03)

    def kg_cmd_touch_get_mode(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x02, 0x01)
    def kg_cmd_touch_set_mode(self, mode):
        return struct.pack('<4BB', 0xC0, 0x01, 0x02, 0x02, mode)

    def kg_cmd_feedback_get_blink_mode(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x03, 0x01)
    def kg_cmd_feedback_set_blink_mode(self, mode):
        return struct.pack('<4BB', 0xC0, 0x01, 0x03, 0x02, mode)
    def kg_cmd_feedback_get_piezo_mode(self, index):
        return struct.pack('<4BB', 0xC0, 0x01, 0x03, 0x03, index)
    def kg_cmd_feedback_set_piezo_mode(self, index, mode, duration, frequency):
        return struct.pack('<4BBBBH', 0xC0, 0x05, 0x03, 0x04, index, mode, duration, frequency)
    def kg_cmd_feedback_get_vibrate_mode(self, index):
        return struct.pack('<4BB', 0xC0, 0x01, 0x03, 0x05, index)
    def kg_cmd_feedback_set_vibrate_mode(self, index, mode, duration):
        return struct.pack('<4BBBB', 0xC0, 0x03, 0x03, 0x06, index, mode, duration)
    def kg_cmd_feedback_get_rgb_mode(self, index):
        return struct.pack('<4BB', 0xC0, 0x01, 0x03, 0x07, index)
    def kg_cmd_feedback_set_rgb_mode(self, index, mode_red, mode_green, mode_blue):
        return struct.pack('<4BBBBB', 0xC0, 0x04, 0x03, 0x08, index, mode_red, mode_green, mode_blue)

    def kg_cmd_motion_get_mode(self, index):
        return struct.pack('<4BB', 0xC0, 0x01, 0x04, 0x01, index)
    def kg_cmd_motion_set_mode(self, index, mode):
        return struct.pack('<4BBB', 0xC0, 0x02, 0x04, 0x02, index, mode)

    def kg_cmd_bluetooth_get_mode(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x08, 0x01)
    def kg_cmd_bluetooth_set_mode(self, mode):
        return struct.pack('<4BB', 0xC0, 0x01, 0x08, 0x02, mode)
    def kg_cmd_bluetooth_reset(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x08, 0x03)
    def kg_cmd_bluetooth_get_mac(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x08, 0x04)
    def kg_cmd_bluetooth_get_pairings(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x08, 0x05)
    def kg_cmd_bluetooth_discover(self, duration):
        return struct.pack('<4BB', 0xC0, 0x01, 0x08, 0x06, duration)
    def kg_cmd_bluetooth_pair(self, address):
        return struct.pack('<4B6s', 0xC0, 0x06, 0x08, 0x07, b''.join(chr(i) for i in address))
    def kg_cmd_bluetooth_delete_pairing(self, index):
        return struct.pack('<4BB', 0xC0, 0x01, 0x08, 0x08, index)
    def kg_cmd_bluetooth_clear_pairings(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x08, 0x09)
    def kg_cmd_bluetooth_get_connections(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x08, 0x0A)
    def kg_cmd_bluetooth_connect(self, index, profile):
        return struct.pack('<4BBB', 0xC0, 0x02, 0x08, 0x0B, index, profile)
    def kg_cmd_bluetooth_disconnect(self, handle):
        return struct.pack('<4BB', 0xC0, 0x01, 0x08, 0x0C, handle)

    kg_rsp_system_ping = KeygloveEvent()
    kg_rsp_system_reset = KeygloveEvent()
    kg_rsp_system_get_info = KeygloveEvent()

    kg_rsp_touch_get_mode = KeygloveEvent()
    kg_rsp_touch_set_mode = KeygloveEvent()

    kg_rsp_feedback_get_blink_mode = KeygloveEvent()
    kg_rsp_feedback_set_blink_mode = KeygloveEvent()
    kg_rsp_feedback_get_piezo_mode = KeygloveEvent()
    kg_rsp_feedback_set_piezo_mode = KeygloveEvent()
    kg_rsp_feedback_get_vibrate_mode = KeygloveEvent()
    kg_rsp_feedback_set_vibrate_mode = KeygloveEvent()
    kg_rsp_feedback_get_rgb_mode = KeygloveEvent()
    kg_rsp_feedback_set_rgb_mode = KeygloveEvent()

    kg_rsp_motion_get_mode = KeygloveEvent()
    kg_rsp_motion_set_mode = KeygloveEvent()

    kg_rsp_bluetooth_get_mode = KeygloveEvent()
    kg_rsp_bluetooth_set_mode = KeygloveEvent()
    kg_rsp_bluetooth_reset = KeygloveEvent()
    kg_rsp_bluetooth_get_mac = KeygloveEvent()
    kg_rsp_bluetooth_get_pairings = KeygloveEvent()
    kg_rsp_bluetooth_discover = KeygloveEvent()
    kg_rsp_bluetooth_pair = KeygloveEvent()
    kg_rsp_bluetooth_delete_pairing = KeygloveEvent()
    kg_rsp_bluetooth_clear_pairings = KeygloveEvent()
    kg_rsp_bluetooth_get_connections = KeygloveEvent()
    kg_rsp_bluetooth_connect = KeygloveEvent()
    kg_rsp_bluetooth_disconnect = KeygloveEvent()

    kg_evt_protocol_error = KeygloveEvent()

    kg_evt_system_boot = KeygloveEvent()
    kg_evt_system_ready = KeygloveEvent()
    kg_evt_system_error = KeygloveEvent()

    kg_evt_touch_mode = KeygloveEvent()
    kg_evt_touch_status = KeygloveEvent()

    kg_evt_feedback_blink_mode = KeygloveEvent()
    kg_evt_feedback_piezo_mode = KeygloveEvent()
    kg_evt_feedback_vibrate_mode = KeygloveEvent()
    kg_evt_feedback_rgb_mode = KeygloveEvent()

    kg_evt_motion_mode = KeygloveEvent()
    kg_evt_motion_data = KeygloveEvent()

    kg_evt_bluetooth_mode = KeygloveEvent()
    kg_evt_bluetooth_ready = KeygloveEvent()
    kg_evt_bluetooth_inquiry_response = KeygloveEvent()
    kg_evt_bluetooth_inquiry_complete = KeygloveEvent()
    kg_evt_bluetooth_pairing_status = KeygloveEvent()
    kg_evt_bluetooth_pairing_failed = KeygloveEvent()
    kg_evt_bluetooth_pairings_cleared = KeygloveEvent()
    kg_evt_bluetooth_connection_status = KeygloveEvent()
    kg_evt_bluetooth_connection_closed = KeygloveEvent()

    kg_log = KeygloveEvent()

    kg_response = KeygloveEvent()
    kg_event = KeygloveEvent()

    on_busy = KeygloveEvent()
    on_idle = KeygloveEvent()
    on_timeout = KeygloveEvent()
    on_before_tx_command = KeygloveEvent()
    on_tx_command_complete = KeygloveEvent()

    kgapi_rx_buffer = []
    kgapi_rx_expected_length = 0
    busy = False
    debug = False

    last_response = None
    last_event = None

    def send_command(self, out_obj, packet):
        if self.debug: print('=>[ ' + ' '.join(['%02X' % ord(b) for b in packet ]) + ' ]')
        self.on_before_tx_command()
        self.busy = True
        self.on_busy()
        if out_obj.__class__.__name__ == "Serial":
            # send packet using PySerial interface
            out_obj.write(packet)
        elif out_obj.__class__.__name__ == "HidReport":
            # send packet via raw HID report (multiple reports if necessary)
            report_size = len(out_obj.values()[0])
            report_i = 1
            out_obj.values()[0][0] = min(len(packet), report_size - 1)
            for i in xrange(len(packet)):
                if report_i >= report_size:
                    # send report and start over with new packet
                    out_obj.send()
                    report_i = 1
                    out_obj.values()[0][0] = min(len(packet) - i, report_size - 1)
                out_obj.values()[0][report_i] = ord(packet[i])
                report_i = report_i + 1

            # finish sending the last (or only) output report
            out_obj.send()
        else:
            print('UNSUPPORTED OUTPUT OBJECT: %s' % out_obj.__class__.__name__)
        self.on_tx_command_complete()

    def send_and_return(self, rxtx_obj, packet, timeout=0):
        self.send_command(rxtx_obj, packet)
        self.check_activity(rxtx_obj, timeout)
        return self.get_last_response()

    def get_last_response(self):
        return self.last_response

    def get_last_event(self):
        return self.last_event

    def check_activity(self, in_obj, timeout=0):
        if in_obj.__class__.__name__ == "Serial":
            # read input data using PySerial interface
            if timeout > 0:
                in_obj.timeout = timeout
                while 1:
                    x = in_obj.read()
                    if len(x) > 0:
                        self.parse(ord(x))
                    else: # timeout
                        self.busy = False
                        self.on_idle()
                        self.on_timeout()
                    if not self.busy: # finished
                        break
            else:
                while in_obj.inWaiting(): self.parse(ord(in_obj.read()))
        elif in_obj.__class__.__name__ == "HidReport":
            # read input using HID interface (already handled via threading so we only need to check for timeouts)
            if timeout > 0:
                t0 = time.time()
                while time.time() - t0 < timeout:
                    if not self.busy: # finished
                        break;
                if self.busy: # timeout
                    self.busy = False
                    self.on_idle()
                    self.on_timeout()
        else:
            print('UNSUPPORTED INPUT OBJECT: %s' % in_obj.__class__.__name__)
        return self.busy

    def parse(self, b):
        if len(self.kgapi_rx_buffer) == 0 and (b == 0xC0 or b == 0x80):
            self.kgapi_rx_buffer.append(b)
        elif len(self.kgapi_rx_buffer) == 1:
            self.kgapi_rx_buffer.append(b)
            self.kgapi_rx_expected_length = 4 + (self.kgapi_rx_buffer[0] & 0x07) + self.kgapi_rx_buffer[1]
        elif len(self.kgapi_rx_buffer) > 1:
            self.kgapi_rx_buffer.append(b)

        """
        Keyglove packet structure (as of 2013-11-16):
            Byte 0:     8 bits, Packet Type             0xC0 = command/response, 0x80 = event
            Byte 1:     8 bits, Length                   Payload length
            Byte 2:     8 bits, Class ID (CID)           Packet class
            Byte 3:     8 bits, Command ID (CMD)         Packet ID
            Bytes 4-n:  0 - 250 Bytes, Payload (PL)      Up to 250 bytes of payload
        """

        #print('%02X: %d, %d' % (b, len(self.kgapi_rx_buffer), self.kgapi_rx_expected_length))
        if self.kgapi_rx_expected_length > 0 and len(self.kgapi_rx_buffer) == self.kgapi_rx_expected_length:
            if self.debug: print('<=[ ' + ' '.join(['%02X' % b for b in self.kgapi_rx_buffer ]) + ' ]')
            packet_type, payload_length, packet_class, packet_command = self.kgapi_rx_buffer[:4]
            self.kgapi_last_rx_packet = self.kgapi_rx_buffer
            self.kgapi_rx_payload = b''.join(chr(i) for i in self.kgapi_rx_buffer[4:])
            self.kgapi_rx_buffer = []
            if packet_type & 0xC0 == 0xC0:
                # 0xC0 = response packet after a command that has just been sent
                if packet_class == 1: # SYSTEM
                    if packet_command == 1: # kg_rsp_system_ping
                        runtime, = struct.unpack('<L', self.kgapi_rx_payload[:4])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'runtime': runtime }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_system_ping(self.last_response['payload'])
                    elif packet_command == 2: # kg_rsp_system_reset
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_system_reset(self.last_response['payload'])
                    elif packet_command == 3: # kg_rsp_system_get_info
                        major, minor, patch, timestamp, = struct.unpack('<BBBL', self.kgapi_rx_payload[:7])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'major': major, 'minor': minor, 'patch': patch, 'timestamp': timestamp }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_system_get_info(self.last_response['payload'])
                elif packet_class == 2: # TOUCH
                    if packet_command == 1: # kg_rsp_touch_get_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_touch_get_mode(self.last_response['payload'])
                    elif packet_command == 2: # kg_rsp_touch_set_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_touch_set_mode(self.last_response['payload'])
                elif packet_class == 3: # FEEDBACK
                    if packet_command == 1: # kg_rsp_feedback_get_blink_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_feedback_get_blink_mode(self.last_response['payload'])
                    elif packet_command == 2: # kg_rsp_feedback_set_blink_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_feedback_set_blink_mode(self.last_response['payload'])
                    elif packet_command == 3: # kg_rsp_feedback_get_piezo_mode
                        mode, duration, frequency, = struct.unpack('<BBH', self.kgapi_rx_payload[:4])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': mode, 'duration': duration, 'frequency': frequency }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_feedback_get_piezo_mode(self.last_response['payload'])
                    elif packet_command == 4: # kg_rsp_feedback_set_piezo_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_feedback_set_piezo_mode(self.last_response['payload'])
                    elif packet_command == 5: # kg_rsp_feedback_get_vibrate_mode
                        mode, duration, = struct.unpack('<BB', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': mode, 'duration': duration }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_feedback_get_vibrate_mode(self.last_response['payload'])
                    elif packet_command == 6: # kg_rsp_feedback_set_vibrate_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_feedback_set_vibrate_mode(self.last_response['payload'])
                    elif packet_command == 7: # kg_rsp_feedback_get_rgb_mode
                        mode_red, mode_green, mode_blue, = struct.unpack('<BBB', self.kgapi_rx_payload[:3])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode_red': mode_red, 'mode_green': mode_green, 'mode_blue': mode_blue }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_feedback_get_rgb_mode(self.last_response['payload'])
                    elif packet_command == 8: # kg_rsp_feedback_set_rgb_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_feedback_set_rgb_mode(self.last_response['payload'])
                elif packet_class == 4: # MOTION
                    if packet_command == 1: # kg_rsp_motion_get_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_motion_get_mode(self.last_response['payload'])
                    elif packet_command == 2: # kg_rsp_motion_set_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_motion_set_mode(self.last_response['payload'])
                elif packet_class == 8: # BLUETOOTH
                    if packet_command == 1: # kg_rsp_bluetooth_get_mode
                        result, mode, = struct.unpack('<HB', self.kgapi_rx_payload[:3])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result, 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_bluetooth_get_mode(self.last_response['payload'])
                    elif packet_command == 2: # kg_rsp_bluetooth_set_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_bluetooth_set_mode(self.last_response['payload'])
                    elif packet_command == 3: # kg_rsp_bluetooth_reset
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_bluetooth_reset(self.last_response['payload'])
                    elif packet_command == 4: # kg_rsp_bluetooth_get_mac
                        result, address, = struct.unpack('<H6s', self.kgapi_rx_payload[:8])
                        address = [ord(b) for b in self.kgapi_rx_payload[2:8]]
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result, 'address': address }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_bluetooth_get_mac(self.last_response['payload'])
                    elif packet_command == 5: # kg_rsp_bluetooth_get_pairings
                        result, count, = struct.unpack('<HB', self.kgapi_rx_payload[:3])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result, 'count': count }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_bluetooth_get_pairings(self.last_response['payload'])
                    elif packet_command == 6: # kg_rsp_bluetooth_discover
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_bluetooth_discover(self.last_response['payload'])
                    elif packet_command == 7: # kg_rsp_bluetooth_pair
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_bluetooth_pair(self.last_response['payload'])
                    elif packet_command == 8: # kg_rsp_bluetooth_delete_pairing
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_bluetooth_delete_pairing(self.last_response['payload'])
                    elif packet_command == 9: # kg_rsp_bluetooth_clear_pairings
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_bluetooth_clear_pairings(self.last_response['payload'])
                    elif packet_command == 10: # kg_rsp_bluetooth_get_connections
                        result, count, = struct.unpack('<HB', self.kgapi_rx_payload[:3])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result, 'count': count }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_bluetooth_get_connections(self.last_response['payload'])
                    elif packet_command == 11: # kg_rsp_bluetooth_connect
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_bluetooth_connect(self.last_response['payload'])
                    elif packet_command == 12: # kg_rsp_bluetooth_disconnect
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_bluetooth_disconnect(self.last_response['payload'])
                self.kg_response(self.last_response)
                self.busy = False
                self.on_idle()
            elif packet_type & 0xC0 == 0x80:
                # 0x80 = event packet
                if packet_class == 0: # PROTOCOL
                    if packet_command == 1: # kg_evt_protocol_error
                        code, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'code': code }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_protocol_error(self.last_event['payload'])
                elif packet_class == 1: # SYSTEM
                    if packet_command == 1: # kg_evt_system_boot
                        major, minor, patch, timestamp, = struct.unpack('<BBBL', self.kgapi_rx_payload[:7])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'major': major, 'minor': minor, 'patch': patch, 'timestamp': timestamp }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_system_boot(self.last_event['payload'])
                    elif packet_command == 2: # kg_evt_system_ready
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': {  }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_system_ready(self.last_event['payload'])
                    elif packet_command == 3: # kg_evt_system_error
                        code, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'code': code }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_system_error(self.last_event['payload'])
                elif packet_class == 2: # TOUCH
                    if packet_command == 1: # kg_evt_touch_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_touch_mode(self.last_event['payload'])
                    elif packet_command == 2: # kg_evt_touch_status
                        status_len, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        status_data = [ord(b) for b in self.kgapi_rx_payload[1:]]
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'status': status_data }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_touch_status(self.last_event['payload'])
                elif packet_class == 3: # FEEDBACK
                    if packet_command == 1: # kg_evt_feedback_blink_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_feedback_blink_mode(self.last_event['payload'])
                    elif packet_command == 2: # kg_evt_feedback_piezo_mode
                        index, mode, duration, frequency, = struct.unpack('<BBBH', self.kgapi_rx_payload[:5])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': index, 'mode': mode, 'duration': duration, 'frequency': frequency }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_feedback_piezo_mode(self.last_event['payload'])
                    elif packet_command == 3: # kg_evt_feedback_vibrate_mode
                        index, mode, duration, = struct.unpack('<BBB', self.kgapi_rx_payload[:3])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': index, 'mode': mode, 'duration': duration }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_feedback_vibrate_mode(self.last_event['payload'])
                    elif packet_command == 4: # kg_evt_feedback_rgb_mode
                        index, mode_red, mode_green, mode_blue, = struct.unpack('<BBBB', self.kgapi_rx_payload[:4])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': index, 'mode_red': mode_red, 'mode_green': mode_green, 'mode_blue': mode_blue }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_feedback_rgb_mode(self.last_event['payload'])
                elif packet_class == 4: # MOTION
                    if packet_command == 1: # kg_evt_motion_mode
                        index, mode, = struct.unpack('<BB', self.kgapi_rx_payload[:2])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': index, 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_motion_mode(self.last_event['payload'])
                    elif packet_command == 2: # kg_evt_motion_data
                        index, flags, data_len, = struct.unpack('<BBB', self.kgapi_rx_payload[:3])
                        data_data = [ord(b) for b in self.kgapi_rx_payload[3:]]
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': index, 'flags': flags, 'data': data_data }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_motion_data(self.last_event['payload'])
                elif packet_class == 8: # BLUETOOTH
                    if packet_command == 1: # kg_evt_bluetooth_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_mode(self.last_event['payload'])
                    elif packet_command == 2: # kg_evt_bluetooth_ready
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': {  }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_ready(self.last_event['payload'])
                    elif packet_command == 3: # kg_evt_bluetooth_inquiry_response
                        address, cod, rssi, status, index, name_len, = struct.unpack('<6s3sbBBB', self.kgapi_rx_payload[:13])
                        address = [ord(b) for b in self.kgapi_rx_payload[0:6]]
                        cod = [ord(b) for b in self.kgapi_rx_payload[6:9]]
                        name_data = [ord(b) for b in self.kgapi_rx_payload[13:]]
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'address': address, 'cod': cod, 'rssi': rssi, 'status': status, 'index': index, 'name': name_data }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_inquiry_response(self.last_event['payload'])
                    elif packet_command == 4: # kg_evt_bluetooth_inquiry_complete
                        count, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'count': count }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_inquiry_complete(self.last_event['payload'])
                    elif packet_command == 5: # kg_evt_bluetooth_pairing_status
                        index, address, priority, profiles_supported, profiles_active, handle_list_len, = struct.unpack('<B6sBBBB', self.kgapi_rx_payload[:11])
                        address = [ord(b) for b in self.kgapi_rx_payload[1:7]]
                        handle_list_data = [ord(b) for b in self.kgapi_rx_payload[11:]]
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': index, 'address': address, 'priority': priority, 'profiles_supported': profiles_supported, 'profiles_active': profiles_active, 'handle_list': handle_list_data }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_pairing_status(self.last_event['payload'])
                    elif packet_command == 6: # kg_evt_bluetooth_pairing_failed
                        address, = struct.unpack('<6s', self.kgapi_rx_payload[:6])
                        address = [ord(b) for b in self.kgapi_rx_payload[0:6]]
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'address': address }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_pairing_failed(self.last_event['payload'])
                    elif packet_command == 7: # kg_evt_bluetooth_pairings_cleared
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': {  }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_pairings_cleared(self.last_event['payload'])
                    elif packet_command == 8: # kg_evt_bluetooth_connection_status
                        handle, address, index, profile, status, = struct.unpack('<B6sBBB', self.kgapi_rx_payload[:10])
                        address = [ord(b) for b in self.kgapi_rx_payload[1:7]]
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'handle': handle, 'address': address, 'index': index, 'profile': profile, 'status': status }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_connection_status(self.last_event['payload'])
                    elif packet_command == 9: # kg_evt_bluetooth_connection_closed
                        handle, reason, = struct.unpack('<BH', self.kgapi_rx_payload[:3])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'handle': handle, 'reason': reason }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_connection_closed(self.last_event['payload'])
                elif packet_class == 0xFF: # LOG
                    if packet_command == 0xFF: # kg_log
                        level, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        message = self.kgapi_rx_payload[1:]
                        payload = { 'level': level, 'message': message }
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': payload, 'raw': self.kgapi_last_rx_packet }
                        self.kg_log(payload)
                self.kg_event(self.last_event)

    def friendly_packet(self, incoming, packet):
        if len(packet) < 4 or len(packet) != ord(packet[1]) + 4:
            return False # invalid packet length
        if packet[0] != 0x80 and (incoming == 0 and ord(packet[0]) != 0xC0):
            return False # invalid packet type

        packet_type = ord(packet[0]) & 0xC0
        payload_length = ord(packet[1])
        packet_class = ord(packet[2])
        packet_command = ord(packet[3])
        payload = packet[4:]

        if incoming == 0:
            if packet_class == 1: # SYSTEM
                if packet_command == 1: # kg_cmd_system_ping
                    return { 'type': 'command', 'name': 'kg_cmd_system_ping', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 2: # kg_cmd_system_reset
                    type, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_system_reset', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'type': ('%02X' % type) }, 'payload_keys': [ 'type' ] }
                elif packet_command == 3: # kg_cmd_system_get_info
                    return { 'type': 'command', 'name': 'kg_cmd_system_get_info', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
            elif packet_class == 2: # TOUCH
                if packet_command == 1: # kg_cmd_touch_get_mode
                    return { 'type': 'command', 'name': 'kg_cmd_touch_get_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 2: # kg_cmd_touch_set_mode
                    mode, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_touch_set_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
            elif packet_class == 3: # FEEDBACK
                if packet_command == 1: # kg_cmd_feedback_get_blink_mode
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_get_blink_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 2: # kg_cmd_feedback_set_blink_mode
                    mode, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_set_blink_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                elif packet_command == 3: # kg_cmd_feedback_get_piezo_mode
                    index, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_get_piezo_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%02X' % index) }, 'payload_keys': [ 'index' ] }
                elif packet_command == 4: # kg_cmd_feedback_set_piezo_mode
                    index, mode, duration, frequency, = struct.unpack('<BBBH', payload[:5])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_set_piezo_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%02X' % index), 'mode': ('%02X' % mode), 'duration': ('%d %s' % (duration * 10, 'ms')), 'frequency': ('%d %s' % (frequency, 'Hz')) }, 'payload_keys': [ 'index', 'mode', 'duration', 'frequency' ] }
                elif packet_command == 5: # kg_cmd_feedback_get_vibrate_mode
                    index, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_get_vibrate_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%02X' % index) }, 'payload_keys': [ 'index' ] }
                elif packet_command == 6: # kg_cmd_feedback_set_vibrate_mode
                    index, mode, duration, = struct.unpack('<BBB', payload[:3])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_set_vibrate_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%02X' % index), 'mode': ('%02X' % mode), 'duration': ('%d %s' % (duration * 10, 'ms')) }, 'payload_keys': [ 'index', 'mode', 'duration' ] }
                elif packet_command == 7: # kg_cmd_feedback_get_rgb_mode
                    index, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_get_rgb_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%02X' % index) }, 'payload_keys': [ 'index' ] }
                elif packet_command == 8: # kg_cmd_feedback_set_rgb_mode
                    index, mode_red, mode_green, mode_blue, = struct.unpack('<BBBB', payload[:4])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_set_rgb_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%02X' % index), 'mode_red': ('%02X' % mode_red), 'mode_green': ('%02X' % mode_green), 'mode_blue': ('%02X' % mode_blue) }, 'payload_keys': [ 'index', 'mode_red', 'mode_green', 'mode_blue' ] }
            elif packet_class == 4: # MOTION
                if packet_command == 1: # kg_cmd_motion_get_mode
                    index, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_motion_get_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%02X' % index) }, 'payload_keys': [ 'index' ] }
                elif packet_command == 2: # kg_cmd_motion_set_mode
                    index, mode, = struct.unpack('<BB', payload[:2])
                    return { 'type': 'command', 'name': 'kg_cmd_motion_set_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%02X' % index), 'mode': ('%02X' % mode) }, 'payload_keys': [ 'index', 'mode' ] }
            elif packet_class == 8: # BLUETOOTH
                if packet_command == 1: # kg_cmd_bluetooth_get_mode
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_get_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 2: # kg_cmd_bluetooth_set_mode
                    mode, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_set_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                elif packet_command == 3: # kg_cmd_bluetooth_reset
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_reset', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 4: # kg_cmd_bluetooth_get_mac
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_get_mac', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 5: # kg_cmd_bluetooth_get_pairings
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_get_pairings', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 6: # kg_cmd_bluetooth_discover
                    duration, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_discover', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'duration': ('%d %s' % (duration, 'second' if (duration == 1) else 'seconds')) }, 'payload_keys': [ 'duration' ] }
                elif packet_command == 7: # kg_cmd_bluetooth_pair
                    address, = struct.unpack('<6s', payload[:6])
                    address = [ord(b) for b in payload[0:6]]
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_pair', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'address': ':'.join(['%02X' % b for b in address][::-1]) }, 'payload_keys': [ 'address' ] }
                elif packet_command == 8: # kg_cmd_bluetooth_delete_pairing
                    index, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_delete_pairing', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%02X' % index) }, 'payload_keys': [ 'index' ] }
                elif packet_command == 9: # kg_cmd_bluetooth_clear_pairings
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_clear_pairings', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 10: # kg_cmd_bluetooth_get_connections
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_get_connections', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 11: # kg_cmd_bluetooth_connect
                    index, profile, = struct.unpack('<BB', payload[:2])
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_connect', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%02X' % index), 'profile': ('%02X' % profile) }, 'payload_keys': [ 'index', 'profile' ] }
                elif packet_command == 12: # kg_cmd_bluetooth_disconnect
                    handle, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_disconnect', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'handle': ('%02X' % handle) }, 'payload_keys': [ 'handle' ] }
        else:
            if packet_type & 0xC0 == 0xC0: # response packet
                if packet_class == 1: # SYSTEM
                    if packet_command == 1: # kg_rsp_system_ping
                        runtime, = struct.unpack('<L', payload[:4])
                        return { 'type': 'response', 'name': 'kg_rsp_system_ping', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'runtime': ('%d %s' % (runtime, 'second' if (runtime == 1) else 'seconds')) }, 'payload_keys': [ 'runtime' ] }
                    elif packet_command == 2: # kg_rsp_system_reset
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_system_reset', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                    elif packet_command == 3: # kg_rsp_system_get_info
                        major, minor, patch, timestamp, = struct.unpack('<BBBL', payload[:7])
                        return { 'type': 'response', 'name': 'kg_rsp_system_get_info', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'major': ('%d' % (major)), 'minor': ('%d' % (minor)), 'patch': ('%d' % (patch)), 'timestamp': ('%d' % (timestamp)) }, 'payload_keys': [ 'major', 'minor', 'patch', 'timestamp' ] }
                elif packet_class == 2: # TOUCH
                    if packet_command == 1: # kg_rsp_touch_get_mode
                        mode, = struct.unpack('<B', payload[:1])
                        return { 'type': 'response', 'name': 'kg_rsp_touch_get_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                    elif packet_command == 2: # kg_rsp_touch_set_mode
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_touch_set_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                elif packet_class == 3: # FEEDBACK
                    if packet_command == 1: # kg_rsp_feedback_get_blink_mode
                        mode, = struct.unpack('<B', payload[:1])
                        return { 'type': 'response', 'name': 'kg_rsp_feedback_get_blink_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                    elif packet_command == 2: # kg_rsp_feedback_set_blink_mode
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_feedback_set_blink_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                    elif packet_command == 3: # kg_rsp_feedback_get_piezo_mode
                        mode, duration, frequency, = struct.unpack('<BBH', payload[:4])
                        return { 'type': 'response', 'name': 'kg_rsp_feedback_get_piezo_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode), 'duration': ('%d %s' % (duration * 10, 'ms')), 'frequency': ('%d %s' % (frequency, 'Hz')) }, 'payload_keys': [ 'mode', 'duration', 'frequency' ] }
                    elif packet_command == 4: # kg_rsp_feedback_set_piezo_mode
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_feedback_set_piezo_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                    elif packet_command == 5: # kg_rsp_feedback_get_vibrate_mode
                        mode, duration, = struct.unpack('<BB', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_feedback_get_vibrate_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode), 'duration': ('%d %s' % (duration * 10, '10')) }, 'payload_keys': [ 'mode', 'duration' ] }
                    elif packet_command == 6: # kg_rsp_feedback_set_vibrate_mode
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_feedback_set_vibrate_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                    elif packet_command == 7: # kg_rsp_feedback_get_rgb_mode
                        mode_red, mode_green, mode_blue, = struct.unpack('<BBB', payload[:3])
                        return { 'type': 'response', 'name': 'kg_rsp_feedback_get_rgb_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode_red': ('%02X' % mode_red), 'mode_green': ('%02X' % mode_green), 'mode_blue': ('%02X' % mode_blue) }, 'payload_keys': [ 'mode_red', 'mode_green', 'mode_blue' ] }
                    elif packet_command == 8: # kg_rsp_feedback_set_rgb_mode
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_feedback_set_rgb_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                elif packet_class == 4: # MOTION
                    if packet_command == 1: # kg_rsp_motion_get_mode
                        mode, = struct.unpack('<B', payload[:1])
                        return { 'type': 'response', 'name': 'kg_rsp_motion_get_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                    elif packet_command == 2: # kg_rsp_motion_set_mode
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_motion_set_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                elif packet_class == 8: # BLUETOOTH
                    if packet_command == 1: # kg_rsp_bluetooth_get_mode
                        result, mode, = struct.unpack('<HB', payload[:3])
                        return { 'type': 'response', 'name': 'kg_rsp_bluetooth_get_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result), 'mode': ('%02X' % mode) }, 'payload_keys': [ 'result', 'mode' ] }
                    elif packet_command == 2: # kg_rsp_bluetooth_set_mode
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_bluetooth_set_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                    elif packet_command == 3: # kg_rsp_bluetooth_reset
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_bluetooth_reset', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                    elif packet_command == 4: # kg_rsp_bluetooth_get_mac
                        result, address, = struct.unpack('<H6s', payload[:8])
                        address = [ord(b) for b in payload[2:8]]
                        return { 'type': 'response', 'name': 'kg_rsp_bluetooth_get_mac', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result), 'address': ':'.join(['%02X' % b for b in address][::-1]) }, 'payload_keys': [ 'result', 'address' ] }
                    elif packet_command == 5: # kg_rsp_bluetooth_get_pairings
                        result, count, = struct.unpack('<HB', payload[:3])
                        return { 'type': 'response', 'name': 'kg_rsp_bluetooth_get_pairings', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result), 'count': ('%02X' % count) }, 'payload_keys': [ 'result', 'count' ] }
                    elif packet_command == 6: # kg_rsp_bluetooth_discover
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_bluetooth_discover', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                    elif packet_command == 7: # kg_rsp_bluetooth_pair
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_bluetooth_pair', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                    elif packet_command == 8: # kg_rsp_bluetooth_delete_pairing
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_bluetooth_delete_pairing', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                    elif packet_command == 9: # kg_rsp_bluetooth_clear_pairings
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_bluetooth_clear_pairings', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                    elif packet_command == 10: # kg_rsp_bluetooth_get_connections
                        result, count, = struct.unpack('<HB', payload[:3])
                        return { 'type': 'response', 'name': 'kg_rsp_bluetooth_get_connections', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result), 'count': ('%02X' % count) }, 'payload_keys': [ 'result', 'count' ] }
                    elif packet_command == 11: # kg_rsp_bluetooth_connect
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_bluetooth_connect', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                    elif packet_command == 12: # kg_rsp_bluetooth_disconnect
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_bluetooth_disconnect', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
            if packet_type & 0xC0 == 0x80: # event packet
                if packet_class == 0: # PROTOCOL
                    if packet_command == 1: # kg_evt_protocol_error
                        code, = struct.unpack('<H', payload[:2])
                        return { 'type': 'event', 'name': 'kg_evt_protocol_error', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'code': ('%04X' % code) }, 'payload_keys': [ 'code' ] }
                elif packet_class == 1: # SYSTEM
                    if packet_command == 1: # kg_evt_system_boot
                        major, minor, patch, timestamp, = struct.unpack('<BBBL', payload[:7])
                        return { 'type': 'event', 'name': 'kg_evt_system_boot', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'major': ('%d' % (major)), 'minor': ('%d' % (minor)), 'patch': ('%d' % (patch)), 'timestamp': ('%d' % (timestamp)) }, 'payload_keys': [ 'major', 'minor', 'patch', 'timestamp' ] }
                    elif packet_command == 2: # kg_evt_system_ready
                        return { 'type': 'event', 'name': 'kg_evt_system_ready', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                    elif packet_command == 3: # kg_evt_system_error
                        code, = struct.unpack('<H', payload[:2])
                        return { 'type': 'event', 'name': 'kg_evt_system_error', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'code': ('%04X' % code) }, 'payload_keys': [ 'code' ] }
                elif packet_class == 2: # TOUCH
                    if packet_command == 1: # kg_evt_touch_mode
                        mode, = struct.unpack('<B', payload[:1])
                        return { 'type': 'event', 'name': 'kg_evt_touch_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                    elif packet_command == 2: # kg_evt_touch_status
                        status_len, = struct.unpack('<B', payload[:1])
                        status_data = [ord(b) for b in payload[1:]]
                        return { 'type': 'event', 'name': 'kg_evt_touch_status', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'status': ' '.join(['%02X' % b for b in status_data]) }, 'payload_keys': [ 'status' ] }
                elif packet_class == 3: # FEEDBACK
                    if packet_command == 1: # kg_evt_feedback_blink_mode
                        mode, = struct.unpack('<B', payload[:1])
                        return { 'type': 'event', 'name': 'kg_evt_feedback_blink_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                    elif packet_command == 2: # kg_evt_feedback_piezo_mode
                        index, mode, duration, frequency, = struct.unpack('<BBBH', payload[:5])
                        return { 'type': 'event', 'name': 'kg_evt_feedback_piezo_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': ('%02X' % index), 'mode': ('%02X' % mode), 'duration': ('%d' % (duration)), 'frequency': ('%d' % (frequency)) }, 'payload_keys': [ 'index', 'mode', 'duration', 'frequency' ] }
                    elif packet_command == 3: # kg_evt_feedback_vibrate_mode
                        index, mode, duration, = struct.unpack('<BBB', payload[:3])
                        return { 'type': 'event', 'name': 'kg_evt_feedback_vibrate_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': ('%02X' % index), 'mode': ('%02X' % mode), 'duration': ('%d' % (duration)) }, 'payload_keys': [ 'index', 'mode', 'duration' ] }
                    elif packet_command == 4: # kg_evt_feedback_rgb_mode
                        index, mode_red, mode_green, mode_blue, = struct.unpack('<BBBB', payload[:4])
                        return { 'type': 'event', 'name': 'kg_evt_feedback_rgb_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': ('%02X' % index), 'mode_red': ('%02X' % mode_red), 'mode_green': ('%02X' % mode_green), 'mode_blue': ('%02X' % mode_blue) }, 'payload_keys': [ 'index', 'mode_red', 'mode_green', 'mode_blue' ] }
                elif packet_class == 4: # MOTION
                    if packet_command == 1: # kg_evt_motion_mode
                        index, mode, = struct.unpack('<BB', payload[:2])
                        return { 'type': 'event', 'name': 'kg_evt_motion_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': ('%02X' % index), 'mode': ('%02X' % mode) }, 'payload_keys': [ 'index', 'mode' ] }
                    elif packet_command == 2: # kg_evt_motion_data
                        index, flags, data_len, = struct.unpack('<BBB', payload[:3])
                        data_data = [ord(b) for b in payload[3:]]
                        return { 'type': 'event', 'name': 'kg_evt_motion_data', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': ('%02X' % index), 'flags': ('%02X' % flags), 'data': ' '.join(['%02X' % b for b in data_data]) }, 'payload_keys': [ 'index', 'flags', 'data' ] }
                elif packet_class == 8: # BLUETOOTH
                    if packet_command == 1: # kg_evt_bluetooth_mode
                        mode, = struct.unpack('<B', payload[:1])
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                    elif packet_command == 2: # kg_evt_bluetooth_ready
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_ready', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                    elif packet_command == 3: # kg_evt_bluetooth_inquiry_response
                        address, cod, rssi, status, index, name_len, = struct.unpack('<6s3sbBBB', payload[:13])
                        address = [ord(b) for b in payload[0:6]]
                        cod = [ord(b) for b in payload[6:9]]
                        name_data = [ord(b) for b in payload[13:]]
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_inquiry_response', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'address': ':'.join(['%02X' % b for b in address][::-1]), 'cod': ''.join(['%02X' % b for b in cod][::-1]), 'rssi': ('%d' % (rssi)), 'status': ('%02X' % status), 'index': ('%02X' % index), 'name': ''.join(['%c' % b for b in name_data]) }, 'payload_keys': [ 'address', 'cod', 'rssi', 'status', 'index', 'name' ] }
                    elif packet_command == 4: # kg_evt_bluetooth_inquiry_complete
                        count, = struct.unpack('<B', payload[:1])
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_inquiry_complete', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'count': ('%d' % (count)) }, 'payload_keys': [ 'count' ] }
                    elif packet_command == 5: # kg_evt_bluetooth_pairing_status
                        index, address, priority, profiles_supported, profiles_active, handle_list_len, = struct.unpack('<B6sBBBB', payload[:11])
                        address = [ord(b) for b in payload[1:7]]
                        handle_list_data = [ord(b) for b in payload[11:]]
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_pairing_status', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': ('%02X' % index), 'address': ':'.join(['%02X' % b for b in address][::-1]), 'priority': ('%02X' % priority), 'profiles_supported': ('%02X' % profiles_supported), 'profiles_active': ('%02X' % profiles_active), 'handle_list': ' '.join(['%02X' % b for b in handle_list_data]) }, 'payload_keys': [ 'index', 'address', 'priority', 'profiles_supported', 'profiles_active', 'handle_list' ] }
                    elif packet_command == 6: # kg_evt_bluetooth_pairing_failed
                        address, = struct.unpack('<6s', payload[:6])
                        address = [ord(b) for b in payload[0:6]]
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_pairing_failed', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'address': ':'.join(['%02X' % b for b in address][::-1]) }, 'payload_keys': [ 'address' ] }
                    elif packet_command == 7: # kg_evt_bluetooth_pairings_cleared
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_pairings_cleared', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                    elif packet_command == 8: # kg_evt_bluetooth_connection_status
                        handle, address, index, profile, status, = struct.unpack('<B6sBBB', payload[:10])
                        address = [ord(b) for b in payload[1:7]]
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_connection_status', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'handle': ('%02X' % handle), 'address': ':'.join(['%02X' % b for b in address][::-1]), 'index': ('%02X' % index), 'profile': ('%02X' % profile), 'status': ('%02X' % status) }, 'payload_keys': [ 'handle', 'address', 'index', 'profile', 'status' ] }
                    elif packet_command == 9: # kg_evt_bluetooth_connection_closed
                        handle, reason, = struct.unpack('<BH', payload[:3])
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_connection_closed', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'handle': ('%02X' % handle), 'reason': ('%04X' % reason) }, 'payload_keys': [ 'handle', 'reason' ] }
                elif packet_class == 0xFF: # LOG
                    if packet_command == 0xFF: # kg_log
                        level, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        message = self.kgapi_rx_payload[1:]
                        return { 'type': 'event', 'name': 'kg_log', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'level': ('%d' % level), 'message': ''.join(['%c' % b for b in message]) }, 'payload_keys': [ 'level', 'message' ] }

# ============================ EOF ===============================
