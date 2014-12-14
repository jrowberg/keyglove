#!/usr/bin/env python

"""
================================================================================
Keyglove Protocol Python interface library
2014-12-06 by Jeff Rowberg <jeff@rowberg.net>

Changelog:
    2014-12-06 - Separated communication code from parser/generator code
               - Added transparent support for PySerial/PyUSB/PyWinUSB
    2014-09-01 - Added unknown last response/event case for stability
    2014-05-20 - Added friendly output for GUI tool and general logging
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
__version__ = "2014-12-13"
__email__ = "jeff@rowberg.net"

import struct, platform, sys, threading, time



# custom cases for cleaner exception handling
class KeygloveError(Exception):
    pass

class KeygloveDeviceError(KeygloveError):
    pass

class KeygloveSerialError(KeygloveError):
    pass

class KeygloveHIDError(KeygloveError):
    pass



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



class KeygloveDevice(object):

    on_connected = KeygloveEvent()
    on_disconnected = KeygloveEvent()
    on_unplugged = KeygloveEvent()

    on_api_busy = KeygloveEvent()
    on_api_idle = KeygloveEvent()
    on_api_timeout = KeygloveEvent()
    on_before_tx_command = KeygloveEvent()
    on_tx_command_complete = KeygloveEvent()

    def __init__(self, devobj=None, type=None, backend=None, transport=None, hardware=None, port=None, description=None, mac=None, debug=False):
        self.devobj = devobj
        self.type = type
        self.backend = backend
        self.transport = transport
        self.hardware = hardware
        self.port = port
        self.description = description
        self.mac = mac
        self.debug = debug

        self.connected = False
        self.responses_pending = 0
        self.serial_port = None
        self.serial_read_thread = None
        self.pywinusb_output = None
        self.pyusb_endpoint_in = None
        self.pyusb_endpoint_out = None
        self.pyusb_read_thread = None

    def connect(self, kgapi):
        # validate device settings
        if self.devobj == None or self.type == None or self.backend == None:
            raise KeygloveDeviceError("Unable to connect, device must at least have properly defined 'devobj', 'type', and 'backend' members")
        if self.type not in ['hid', 'serial']:
            raise KeygloveDeviceError("Unable to connect, device type invalid (must be in list %s)" % ['hid', 'serial'])
        if self.type == 'hid' and self.backend not in usb_options:
            raise KeygloveDeviceError("Unable to connect, device backend invalid for HID type (must be in list %s)" % usb_options)
        if self.type == 'serial' and self.backend not in serial_options:
            raise KeygloveDeviceError("Unable to connect, device backend invalid for serial type (must be in list %s)" % serial_options)

        # attach KGAPI instance
        self.kgapi = kgapi

        # skip the rest if we're already connected
        if self.connected:
            return True

        if self.type == 'hid':
            if self.backend == 'pywinusb':
                # open the device and set handler
                self.devobj.open();
                self.devobj.set_raw_data_handler(self.pywinusb_read_handler)

                # find correct output report entry to use for sending data
                target_usage = pywinusb.hid.get_full_usage_id(0xffab, 0x02)
                output_report = None
                for report in self.devobj.find_output_reports():
                    if target_usage in report:
                        output_report = report
                if output_report == None:
                    self.devobj.close();
                    self.connected = False
                    raise KeygloveHIDError("Unable to locate correct output report (0xffab:0x02) using PyWinUSB")

                self.pywinusb_output = output_report
                self.connected = True
                self.on_connected()
                return True

            elif self.backend == 'pyusb':
                try:
                    self.devobj.detach_kernel_driver(0)
                except usb.core.USBError as e:
                    if e.errno == 2:
                        # already detached kernel driver
                        sys.exc_clear()

                try:
                    cfg = self.devobj.get_active_configuration()
                    if cfg == None: self.devobj.set_configuration()
                    intf = cfg[(0, 0)]
                    self.pyusb_endpoint_in = usb.util.find_descriptor(intf, custom_match=lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_IN)
                    self.pyusb_endpoint_out = usb.util.find_descriptor(intf, custom_match=lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_OUT)
                    self.connected = True

                    # start read thread
                    self.pyusb_read_thread = threading.Thread(target=self.pyusb_read_handler)
                    self.pyusb_read_thread.daemon = True
                    self.pyusb_read_thread.start()

                    self.on_connected()
                    return True
                except usb.core.USBError as e:
                    raise KeygloveHIDError("Unable to open device with PyUSB: %s" % e)

        elif self.type == 'serial':
            if self.backend == 'pyserial':
                try:
                    # create and open the serial port
                    self.serial_port = serial.Serial(port=self.port, baudrate=115200, timeout=1, writeTimeout=1)

                    # flush buffers
                    self.serial_port.flushInput()
                    self.serial_port.flushOutput()

                    self.connected = True

                    # start read thread
                    self.serial_read_thread = threading.Thread(target=self.pyserial_read_handler)
                    self.serial_read_thread.daemon = True
                    self.serial_read_thread.start()

                    self.on_connected()
                    return True
                except serial.serialutil.SerialException as e:
                    raise KeygloveSerialError("Unable to open serial port: %s" % e)

    def disconnect(self):
        # skip the rest if we're NOT already connected
        if not self.connected:
            return True

        if self.type == 'hid':
            if self.backend == 'pywinusb':
                # close the device (PyWinUSB spawns separate read thread while open)
                self.connected = False
                self.devobj.close()
                self.pywinusb_output = None
                self.kgapi = None
                self.on_disconnected()
                return True

            elif self.backend == 'pyusb':
                #try:
                #self.devobj.attach_kernel_driver(0)
                self.connected = False
                self.pyusb_endpoint_in = None
                self.pyusb_endpoint_out = None
                self.kgapi = None
                self.on_disconnected()
                return True
                #except usb.core.USBError as e:
                #    if e.errno == 2:
                #        # already detached kernel driver
                #        sys.exc_clear()

        elif self.type == 'serial':
            # close serial port cleanly
            self.connected = False
            self.serial_port.close()
            self.serial_port = None
            self.kgapi = None
            self.on_disconnected()
            return True

    def send(self, packet):
        if not self.connected:
            raise KeygloveError('Cannot send packet, Keyglove not connected')

        if type(packet) == type(list()):
            packet = b''.join(chr(x) for x in packet)
        if self.debug:
            print('=>[ ' + ' '.join(['%02X' % ord(b) for b in packet ]) + ' ]')
        self.on_before_tx_command()
        self.responses_pending = self.responses_pending + 1
        self.on_api_busy()

        if self.type == 'hid':
            if self.backend == 'pywinusb':
                # send packet via PyWinUSB raw HID report (multiple reports if necessary)
                if self.pywinusb_output == None:
                    raise KeygloveHIDError('Cannot send packet, PyWinUSB output report not valid')

                try:
                    report_size = len(self.pywinusb_output.values()[0])
                    report_i = 1
                    self.pywinusb_output.values()[0][0] = min(len(packet), report_size - 1)
                    for i in xrange(len(packet)):
                        if report_i >= report_size:
                            # send report and start over with new packet
                            self.pywinusb_output.send()
                            report_i = 1
                            self.pywinusb_output.values()[0][0] = min(len(packet) - i, report_size - 1)
                        self.pywinusb_output.values()[0][report_i] = ord(packet[i])
                        report_i = report_i + 1

                    # finish sending the last (or only) output report
                    self.pywinusb_output.send()
                except pywinusb.hid.helpers.HIDError as e:
                    # HID output endpoint cannot be written to, most likely unplugged/disconnected
                    self.on_unplugged()
                    self.disconnect()

            elif self.backend == 'pyusb':
                # send packet via PyUSB raw HID report (multiple reports if necessary)
                report_size = self.pyusb_endpoint_out.wMaxPacketSize
                report_i = 1
                raw_packet = [ min(len(packet), report_size - 1) ]

                try:
                    for i in xrange(len(packet)):
                        if report_i >= report_size:
                            # send report and start over with new packet
                            self.pyusb_endpoint_out.write(raw_packet)
                            report_i = 1
                            raw_packet = [ min(len(packet) - i, report_size - 1) ]
                        raw_packet.append(ord(packet[i]))
                        report_i = report_i + 1

                    # finish sending the last (or only) output report
                    raw_packet += [0] * (report_size - len(raw_packet)) # pad array to full size
                    self.pyusb_endpoint_out.write(raw_packet)

                except usb.core.USBError as e:
                    # error on write means the device is no longer available
                    self.on_unplugged()
                    self.disconnect()

        elif self.type == 'serial':
            # send packet via PySerial
            if self.serial_port == None:
                raise KeygloveHIDError('Cannot send packet, PySerial port object not valid')
            elif not self.serial_port.isOpen():
                raise KeygloveHIDError('Cannot send packet, PySerial port is not open')

            try:
                self.serial_port.write(packet)
            except (serial.serialutil.SerialException, serial.serialutil.SerialTimeoutException) as e:
                # serial port cannot be written to, most likely unplugged/disconnected
                self.on_unplugged()
                self.disconnect()

        self.on_tx_command_complete()

    def send_and_return(self, packet, timeout=0):
        if self.responses_pending > 0:
            raise KeygloveError("Cannot use send_and_return() if there is already a pending response")
        self.send(packet)
        t0 = time.time()
        while self.connected and self.responses_pending > 0 and (timeout == 0 or time.time() - t0 < timeout):
            pass
        if self.responses_pending > 0:
            # timed out waiting for response
            self.responses_pending = 0
            self.on_api_timeout()
            self.on_api_idle()
            return None
        else:
            # got response back correctly
            if self.kgapi:
                return self.kgapi.get_last_response()
            else:
                # extremely unlikely but not impossible case where KGAPI object is gone before this finishes
                return None

    # handler for reading incoming raw HID packets via PyWinUSB (thread started inside PyWinUSB code)
    def pywinusb_read_handler(self, data):
        if ((data[0] == 0x00 and self.transport == 'usb') or (data[0] == 0x04 and self.transport == 'bluetooth')) and data[1] < len(data) - 1:
            for b in data[2:data[1] + 2]:
                if self.kgapi.parse(b) == 0xC0:
                    self.responses_pending = self.responses_pending - 1
                    if self.responses_pending == 0:
                        self.on_api_idle()

    # handler for reading incoming raw HID packets via PyUSB (thread started in local connect() method)
    def pyusb_read_handler(self):
        while self.pyusb_endpoint_in != None and self.connected:
            try:
                ret = self.devobj.read(self.pyusb_endpoint_in.bEndpointAddress, self.pyusb_endpoint_in.wMaxPacketSize)
                if len(ret) > 0 and ret[0] > 0:
                    for b in ret[1:ret[0] + 1]:
                        if self.kgapi.parse(b) == 0xC0:
                            self.responses_pending = self.responses_pending - 1
                            if self.responses_pending == 0:
                                self.on_api_idle()
            except usb.core.USBError as e:
                if e.errno == 110 or "timed out" in str(e) or "not detach" in str(e):
                    # PyUSB timeout, probably just no data
                    sys.exc_clear()
                elif e.errno == 5 or e.errno == 19 or "such device" in str(e):
                    # "Input/Output Error" or "No such device", this is serious
                    self.on_unplugged()
                    self.disconnect()
                else:
                    raise KeygloveHIDError("PyUSB read thread error: %s" % e)

    # handler for reading incoming raw serial data via PySerial (thread started in local connect() method)
    def pyserial_read_handler(self):
        while self.serial_port != None and self.serial_port.isOpen():
            try:
                ch = self.serial_port.read()
                if len(ch):
                    #print "%02X " % ord(ch)
                    if self.kgapi.parse(ord(ch)) == 0xC0:
                        self.responses_pending = self.responses_pending - 1
                        if self.responses_pending == 0:
                            self.on_api_idle()
            except serial.SerialException as e:
                # serial port cannot be read from, most likely unplugged/disconnected
                self.on_unplugged()
                self.disconnect()



class KGAPI(object):

    def kg_cmd_system_ping(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x01, 0x01)
    def kg_cmd_system_reset(self, mode):
        return struct.pack('<4BB', 0xC0, 0x01, 0x01, 0x02, mode)
    def kg_cmd_system_get_info(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x01, 0x03)
    def kg_cmd_system_get_capabilities(self, category):
        return struct.pack('<4BB', 0xC0, 0x01, 0x01, 0x04, category)
    def kg_cmd_system_get_memory(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x01, 0x05)
    def kg_cmd_system_get_battery_status(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x01, 0x06)
    def kg_cmd_system_set_timer(self, handle, interval, oneshot):
        return struct.pack('<4BBHB', 0xC0, 0x04, 0x01, 0x07, handle, interval, oneshot)
    
    def kg_cmd_bluetooth_get_mode(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x02, 0x01)
    def kg_cmd_bluetooth_set_mode(self, mode):
        return struct.pack('<4BB', 0xC0, 0x01, 0x02, 0x02, mode)
    def kg_cmd_bluetooth_reset(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x02, 0x03)
    def kg_cmd_bluetooth_get_mac(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x02, 0x04)
    def kg_cmd_bluetooth_get_pairings(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x02, 0x05)
    def kg_cmd_bluetooth_discover(self, duration):
        return struct.pack('<4BB', 0xC0, 0x01, 0x02, 0x06, duration)
    def kg_cmd_bluetooth_pair(self, address):
        return struct.pack('<4B6s', 0xC0, 0x06, 0x02, 0x07, b''.join(chr(i) for i in address))
    def kg_cmd_bluetooth_delete_pairing(self, pairing):
        return struct.pack('<4BB', 0xC0, 0x01, 0x02, 0x08, pairing)
    def kg_cmd_bluetooth_clear_pairings(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x02, 0x09)
    def kg_cmd_bluetooth_get_connections(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x02, 0x0A)
    def kg_cmd_bluetooth_connect(self, pairing, profile):
        return struct.pack('<4BBB', 0xC0, 0x02, 0x02, 0x0B, pairing, profile)
    def kg_cmd_bluetooth_disconnect(self, handle):
        return struct.pack('<4BB', 0xC0, 0x01, 0x02, 0x0C, handle)
    
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
    
    def kg_cmd_touch_get_mode(self):
        return struct.pack('<4B', 0xC0, 0x00, 0x04, 0x01)
    def kg_cmd_touch_set_mode(self, mode):
        return struct.pack('<4BB', 0xC0, 0x01, 0x04, 0x02, mode)
    
    def kg_cmd_motion_get_mode(self, index):
        return struct.pack('<4BB', 0xC0, 0x01, 0x05, 0x01, index)
    def kg_cmd_motion_set_mode(self, index, mode):
        return struct.pack('<4BBB', 0xC0, 0x02, 0x05, 0x02, index, mode)
    
    kg_rsp_system_ping = KeygloveEvent()
    kg_rsp_system_reset = KeygloveEvent()
    kg_rsp_system_get_info = KeygloveEvent()
    kg_rsp_system_get_capabilities = KeygloveEvent()
    kg_rsp_system_get_memory = KeygloveEvent()
    kg_rsp_system_get_battery_status = KeygloveEvent()
    kg_rsp_system_set_timer = KeygloveEvent()
    
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
    
    kg_rsp_feedback_get_blink_mode = KeygloveEvent()
    kg_rsp_feedback_set_blink_mode = KeygloveEvent()
    kg_rsp_feedback_get_piezo_mode = KeygloveEvent()
    kg_rsp_feedback_set_piezo_mode = KeygloveEvent()
    kg_rsp_feedback_get_vibrate_mode = KeygloveEvent()
    kg_rsp_feedback_set_vibrate_mode = KeygloveEvent()
    kg_rsp_feedback_get_rgb_mode = KeygloveEvent()
    kg_rsp_feedback_set_rgb_mode = KeygloveEvent()
    
    kg_rsp_touch_get_mode = KeygloveEvent()
    kg_rsp_touch_set_mode = KeygloveEvent()
    
    kg_rsp_motion_get_mode = KeygloveEvent()
    kg_rsp_motion_set_mode = KeygloveEvent()
    
    kg_evt_protocol_error = KeygloveEvent()
    
    kg_evt_system_boot = KeygloveEvent()
    kg_evt_system_ready = KeygloveEvent()
    kg_evt_system_error = KeygloveEvent()
    kg_evt_system_capability = KeygloveEvent()
    kg_evt_system_battery_status = KeygloveEvent()
    kg_evt_system_timer_tick = KeygloveEvent()
    
    kg_evt_bluetooth_mode = KeygloveEvent()
    kg_evt_bluetooth_ready = KeygloveEvent()
    kg_evt_bluetooth_inquiry_response = KeygloveEvent()
    kg_evt_bluetooth_inquiry_complete = KeygloveEvent()
    kg_evt_bluetooth_pairing_status = KeygloveEvent()
    kg_evt_bluetooth_pairing_failed = KeygloveEvent()
    kg_evt_bluetooth_pairings_cleared = KeygloveEvent()
    kg_evt_bluetooth_connection_status = KeygloveEvent()
    kg_evt_bluetooth_connection_closed = KeygloveEvent()
    
    kg_evt_feedback_blink_mode = KeygloveEvent()
    kg_evt_feedback_piezo_mode = KeygloveEvent()
    kg_evt_feedback_vibrate_mode = KeygloveEvent()
    kg_evt_feedback_rgb_mode = KeygloveEvent()
    
    kg_evt_touch_mode = KeygloveEvent()
    kg_evt_touch_status = KeygloveEvent()
    
    kg_evt_motion_mode = KeygloveEvent()
    kg_evt_motion_data = KeygloveEvent()
    kg_evt_motion_state = KeygloveEvent()
    
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
    debug = False

    last_response = None
    last_event = None

    def get_last_response(self):
        return self.last_response

    def get_last_event(self):
        return self.last_event

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
            Byte 0:     8 bits, Packet Type              0xC0 = command/response, 0x80 = event
            Byte 1:     8 bits, Length                   Payload length
            Byte 2:     8 bits, Class ID (CID)           Packet class
            Byte 3:     8 bits, Command ID (CMD)         Packet ID
            Bytes 4-n:  0 - 250 Bytes, Payload (PL)      Up to 250 bytes of payload
        """

        #print('%02X: %d, %d' % (b, len(self.kgapi_rx_buffer), self.kgapi_rx_expected_length))
        if self.kgapi_rx_expected_length > 0 and len(self.kgapi_rx_buffer) == self.kgapi_rx_expected_length:
            if self.debug:
                print('<=[ ' + ' '.join(['%02X' % b for b in self.kgapi_rx_buffer ]) + ' ]')
            packet_type, payload_length, packet_class, packet_command = self.kgapi_rx_buffer[:4]
            self.kgapi_last_rx_packet = self.kgapi_rx_buffer
            self.kgapi_rx_payload = b''.join(chr(i) for i in self.kgapi_rx_buffer[4:])
            self.kgapi_rx_buffer = []
            if packet_type & 0xC0 == 0xC0:
                # 0xC0 = response packet after a command that has just been sent
                # initialize last_response with unknown packet if we don't match
                self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { }, 'raw': self.kgapi_last_rx_packet }
                if packet_class == 1: # SYSTEM
                    if packet_command == 1: # kg_rsp_system_ping
                        uptime, = struct.unpack('<L', self.kgapi_rx_payload[:4])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'uptime': uptime }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_system_ping(self.last_response['payload'])
                    elif packet_command == 2: # kg_rsp_system_reset
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_system_reset(self.last_response['payload'])
                    elif packet_command == 3: # kg_rsp_system_get_info
                        major, minor, patch, protocol, timestamp, = struct.unpack('<HHHHL', self.kgapi_rx_payload[:12])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'major': major, 'minor': minor, 'patch': patch, 'protocol': protocol, 'timestamp': timestamp }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_system_get_info(self.last_response['payload'])
                    elif packet_command == 4: # kg_rsp_system_get_capabilities
                        count, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'count': count }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_system_get_capabilities(self.last_response['payload'])
                    elif packet_command == 5: # kg_rsp_system_get_memory
                        free_ram, total_ram, = struct.unpack('<LL', self.kgapi_rx_payload[:8])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'free_ram': free_ram, 'total_ram': total_ram }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_system_get_memory(self.last_response['payload'])
                    elif packet_command == 6: # kg_rsp_system_get_battery_status
                        status, level, = struct.unpack('<BB', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'status': status, 'level': level }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_system_get_battery_status(self.last_response['payload'])
                    elif packet_command == 7: # kg_rsp_system_set_timer
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_system_set_timer(self.last_response['payload'])
                elif packet_class == 2: # BLUETOOTH
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
                elif packet_class == 4: # TOUCH
                    if packet_command == 1: # kg_rsp_touch_get_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_touch_get_mode(self.last_response['payload'])
                    elif packet_command == 2: # kg_rsp_touch_set_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_touch_set_mode(self.last_response['payload'])
                elif packet_class == 5: # MOTION
                    if packet_command == 1: # kg_rsp_motion_get_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_motion_get_mode(self.last_response['payload'])
                    elif packet_command == 2: # kg_rsp_motion_set_mode
                        result, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_response = { 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': result }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_rsp_motion_set_mode(self.last_response['payload'])
                self.kg_response(self.last_response)
            elif packet_type & 0xC0 == 0x80:
                # 0x80 = event packet
                # initialize last_event with unknown packet if we don't match
                self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { }, 'raw': self.kgapi_last_rx_packet }
                if packet_class == 0: # PROTOCOL
                    if packet_command == 1: # kg_evt_protocol_error
                        code, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'code': code }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_protocol_error(self.last_event['payload'])
                elif packet_class == 1: # SYSTEM
                    if packet_command == 1: # kg_evt_system_boot
                        major, minor, patch, protocol, timestamp, = struct.unpack('<HHHHL', self.kgapi_rx_payload[:12])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'major': major, 'minor': minor, 'patch': patch, 'protocol': protocol, 'timestamp': timestamp }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_system_boot(self.last_event['payload'])
                    elif packet_command == 2: # kg_evt_system_ready
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': {  }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_system_ready(self.last_event['payload'])
                    elif packet_command == 3: # kg_evt_system_error
                        code, = struct.unpack('<H', self.kgapi_rx_payload[:2])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'code': code }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_system_error(self.last_event['payload'])
                    elif packet_command == 4: # kg_evt_system_capability
                        category, record_len, = struct.unpack('<HB', self.kgapi_rx_payload[:3])
                        record_data = [ord(b) for b in self.kgapi_rx_payload[3:]]
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'category': category, 'record': record_data }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_system_capability(self.last_event['payload'])
                    elif packet_command == 5: # kg_evt_system_battery_status
                        status, level, = struct.unpack('<BB', self.kgapi_rx_payload[:2])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'status': status, 'level': level }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_system_battery_status(self.last_event['payload'])
                    elif packet_command == 6: # kg_evt_system_timer_tick
                        handle, seconds, subticks, = struct.unpack('<BLB', self.kgapi_rx_payload[:6])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'handle': handle, 'seconds': seconds, 'subticks': subticks }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_system_timer_tick(self.last_event['payload'])
                elif packet_class == 2: # BLUETOOTH
                    if packet_command == 1: # kg_evt_bluetooth_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_mode(self.last_event['payload'])
                    elif packet_command == 2: # kg_evt_bluetooth_ready
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': {  }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_ready(self.last_event['payload'])
                    elif packet_command == 3: # kg_evt_bluetooth_inquiry_response
                        address, cod, rssi, status, pairing, name_len, = struct.unpack('<6s3sbBBB', self.kgapi_rx_payload[:13])
                        address = [ord(b) for b in self.kgapi_rx_payload[0:6]]
                        cod = [ord(b) for b in self.kgapi_rx_payload[6:9]]
                        name_data = [ord(b) for b in self.kgapi_rx_payload[13:]]
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'address': address, 'cod': cod, 'rssi': rssi, 'status': status, 'pairing': pairing, 'name': name_data }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_inquiry_response(self.last_event['payload'])
                    elif packet_command == 4: # kg_evt_bluetooth_inquiry_complete
                        count, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'count': count }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_inquiry_complete(self.last_event['payload'])
                    elif packet_command == 5: # kg_evt_bluetooth_pairing_status
                        pairing, address, priority, profiles_supported, profiles_active, handle_list_len, = struct.unpack('<B6sBBBB', self.kgapi_rx_payload[:11])
                        address = [ord(b) for b in self.kgapi_rx_payload[1:7]]
                        handle_list_data = [ord(b) for b in self.kgapi_rx_payload[11:]]
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'pairing': pairing, 'address': address, 'priority': priority, 'profiles_supported': profiles_supported, 'profiles_active': profiles_active, 'handle_list': handle_list_data }, 'raw': self.kgapi_last_rx_packet }
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
                        handle, address, pairing, profile, status, = struct.unpack('<B6sBBB', self.kgapi_rx_payload[:10])
                        address = [ord(b) for b in self.kgapi_rx_payload[1:7]]
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'handle': handle, 'address': address, 'pairing': pairing, 'profile': profile, 'status': status }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_connection_status(self.last_event['payload'])
                    elif packet_command == 9: # kg_evt_bluetooth_connection_closed
                        handle, reason, = struct.unpack('<BH', self.kgapi_rx_payload[:3])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'handle': handle, 'reason': reason }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_bluetooth_connection_closed(self.last_event['payload'])
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
                elif packet_class == 4: # TOUCH
                    if packet_command == 1: # kg_evt_touch_mode
                        mode, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_touch_mode(self.last_event['payload'])
                    elif packet_command == 2: # kg_evt_touch_status
                        status_len, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        status_data = [ord(b) for b in self.kgapi_rx_payload[1:]]
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'status': status_data }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_touch_status(self.last_event['payload'])
                elif packet_class == 5: # MOTION
                    if packet_command == 1: # kg_evt_motion_mode
                        index, mode, = struct.unpack('<BB', self.kgapi_rx_payload[:2])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': index, 'mode': mode }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_motion_mode(self.last_event['payload'])
                    elif packet_command == 2: # kg_evt_motion_data
                        index, flags, data_len, = struct.unpack('<BBB', self.kgapi_rx_payload[:3])
                        data_data = [ord(b) for b in self.kgapi_rx_payload[3:]]
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': index, 'flags': flags, 'data': data_data }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_motion_data(self.last_event['payload'])
                    elif packet_command == 3: # kg_evt_motion_state
                        index, state, = struct.unpack('<BB', self.kgapi_rx_payload[:2])
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': index, 'state': state }, 'raw': self.kgapi_last_rx_packet }
                        self.kg_evt_motion_state(self.last_event['payload'])
                elif packet_class == 0xFF: # LOG
                    if packet_command == 0xFF: # kg_log
                        level, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        message = self.kgapi_rx_payload[1:]
                        payload = { 'level': level, 'message': message }
                        self.last_event = { 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': payload, 'raw': self.kgapi_last_rx_packet }
                        self.kg_log(payload)
                self.kg_event(self.last_event)

            return packet_type & 0xC0

        else:
            return 0

    def friendly_packet(self, packet, incoming):
        if type(packet) == type(list()):
            packet = b''.join(chr(x) for x in packet)
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
                    mode, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_system_reset', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                elif packet_command == 3: # kg_cmd_system_get_info
                    return { 'type': 'command', 'name': 'kg_cmd_system_get_info', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 4: # kg_cmd_system_get_capabilities
                    category, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_system_get_capabilities', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'category': ('%02X' % category) }, 'payload_keys': [ 'category' ] }
                elif packet_command == 5: # kg_cmd_system_get_memory
                    return { 'type': 'command', 'name': 'kg_cmd_system_get_memory', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 6: # kg_cmd_system_get_battery_status
                    return { 'type': 'command', 'name': 'kg_cmd_system_get_battery_status', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 7: # kg_cmd_system_set_timer
                    handle, interval, oneshot, = struct.unpack('<BHB', payload[:4])
                    return { 'type': 'command', 'name': 'kg_cmd_system_set_timer', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'handle': ('%d' % (handle)), 'interval': ('%d' % (interval)), 'oneshot': ('%d' % (oneshot)) }, 'payload_keys': [ 'handle', 'interval', 'oneshot' ] }
            elif packet_class == 2: # BLUETOOTH
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
                    pairing, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_delete_pairing', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'pairing': ('%d' % (pairing)) }, 'payload_keys': [ 'pairing' ] }
                elif packet_command == 9: # kg_cmd_bluetooth_clear_pairings
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_clear_pairings', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 10: # kg_cmd_bluetooth_get_connections
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_get_connections', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 11: # kg_cmd_bluetooth_connect
                    pairing, profile, = struct.unpack('<BB', payload[:2])
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_connect', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'pairing': ('%d' % (pairing)), 'profile': ('%02X' % profile) }, 'payload_keys': [ 'pairing', 'profile' ] }
                elif packet_command == 12: # kg_cmd_bluetooth_disconnect
                    handle, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_bluetooth_disconnect', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'handle': ('%d' % (handle)) }, 'payload_keys': [ 'handle' ] }
            elif packet_class == 3: # FEEDBACK
                if packet_command == 1: # kg_cmd_feedback_get_blink_mode
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_get_blink_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 2: # kg_cmd_feedback_set_blink_mode
                    mode, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_set_blink_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                elif packet_command == 3: # kg_cmd_feedback_get_piezo_mode
                    index, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_get_piezo_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%d' % (index)) }, 'payload_keys': [ 'index' ] }
                elif packet_command == 4: # kg_cmd_feedback_set_piezo_mode
                    index, mode, duration, frequency, = struct.unpack('<BBBH', payload[:5])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_set_piezo_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%d' % (index)), 'mode': ('%02X' % mode), 'duration': ('%d %s' % (duration * 10, 'ms')), 'frequency': ('%d %s' % (frequency, 'Hz')) }, 'payload_keys': [ 'index', 'mode', 'duration', 'frequency' ] }
                elif packet_command == 5: # kg_cmd_feedback_get_vibrate_mode
                    index, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_get_vibrate_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%d' % (index)) }, 'payload_keys': [ 'index' ] }
                elif packet_command == 6: # kg_cmd_feedback_set_vibrate_mode
                    index, mode, duration, = struct.unpack('<BBB', payload[:3])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_set_vibrate_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%d' % (index)), 'mode': ('%02X' % mode), 'duration': ('%d %s' % (duration * 10, 'ms')) }, 'payload_keys': [ 'index', 'mode', 'duration' ] }
                elif packet_command == 7: # kg_cmd_feedback_get_rgb_mode
                    index, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_get_rgb_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%d' % (index)) }, 'payload_keys': [ 'index' ] }
                elif packet_command == 8: # kg_cmd_feedback_set_rgb_mode
                    index, mode_red, mode_green, mode_blue, = struct.unpack('<BBBB', payload[:4])
                    return { 'type': 'command', 'name': 'kg_cmd_feedback_set_rgb_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%d' % (index)), 'mode_red': ('%02X' % mode_red), 'mode_green': ('%02X' % mode_green), 'mode_blue': ('%02X' % mode_blue) }, 'payload_keys': [ 'index', 'mode_red', 'mode_green', 'mode_blue' ] }
            elif packet_class == 4: # TOUCH
                if packet_command == 1: # kg_cmd_touch_get_mode
                    return { 'type': 'command', 'name': 'kg_cmd_touch_get_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                elif packet_command == 2: # kg_cmd_touch_set_mode
                    mode, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_touch_set_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
            elif packet_class == 5: # MOTION
                if packet_command == 1: # kg_cmd_motion_get_mode
                    index, = struct.unpack('<B', payload[:1])
                    return { 'type': 'command', 'name': 'kg_cmd_motion_get_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%d' % (index)) }, 'payload_keys': [ 'index' ] }
                elif packet_command == 2: # kg_cmd_motion_set_mode
                    index, mode, = struct.unpack('<BB', payload[:2])
                    return { 'type': 'command', 'name': 'kg_cmd_motion_set_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'index': ('%d' % (index)), 'mode': ('%02X' % mode) }, 'payload_keys': [ 'index', 'mode' ] }
        else:
            if packet_type & 0xC0 == 0xC0: # response packet
                if packet_class == 1: # SYSTEM
                    if packet_command == 1: # kg_rsp_system_ping
                        uptime, = struct.unpack('<L', payload[:4])
                        return { 'type': 'response', 'name': 'kg_rsp_system_ping', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'uptime': ('%d %s' % (uptime, 'second' if (uptime == 1) else 'seconds')) }, 'payload_keys': [ 'uptime' ] }
                    elif packet_command == 2: # kg_rsp_system_reset
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_system_reset', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                    elif packet_command == 3: # kg_rsp_system_get_info
                        major, minor, patch, protocol, timestamp, = struct.unpack('<HHHHL', payload[:12])
                        return { 'type': 'response', 'name': 'kg_rsp_system_get_info', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'major': ('%d' % (major)), 'minor': ('%d' % (minor)), 'patch': ('%d' % (patch)), 'protocol': ('%d' % (protocol)), 'timestamp': ('%d' % (timestamp)) }, 'payload_keys': [ 'major', 'minor', 'patch', 'protocol', 'timestamp' ] }
                    elif packet_command == 4: # kg_rsp_system_get_capabilities
                        count, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_system_get_capabilities', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'count': ('%d' % (count)) }, 'payload_keys': [ 'count' ] }
                    elif packet_command == 5: # kg_rsp_system_get_memory
                        free_ram, total_ram, = struct.unpack('<LL', payload[:8])
                        return { 'type': 'response', 'name': 'kg_rsp_system_get_memory', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'free_ram': ('%d %s' % (free_ram, 'byte' if (free_ram == 1) else 'bytes')), 'total_ram': ('%d %s' % (total_ram, 'byte' if (total_ram == 1) else 'bytes')) }, 'payload_keys': [ 'free_ram', 'total_ram' ] }
                    elif packet_command == 6: # kg_rsp_system_get_battery_status
                        status, level, = struct.unpack('<BB', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_system_get_battery_status', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'status': ('%02X' % status), 'level': ('%02X' % level) }, 'payload_keys': [ 'status', 'level' ] }
                    elif packet_command == 7: # kg_rsp_system_set_timer
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_system_set_timer', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                elif packet_class == 2: # BLUETOOTH
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
                elif packet_class == 4: # TOUCH
                    if packet_command == 1: # kg_rsp_touch_get_mode
                        mode, = struct.unpack('<B', payload[:1])
                        return { 'type': 'response', 'name': 'kg_rsp_touch_get_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                    elif packet_command == 2: # kg_rsp_touch_set_mode
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_touch_set_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
                elif packet_class == 5: # MOTION
                    if packet_command == 1: # kg_rsp_motion_get_mode
                        mode, = struct.unpack('<B', payload[:1])
                        return { 'type': 'response', 'name': 'kg_rsp_motion_get_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                    elif packet_command == 2: # kg_rsp_motion_set_mode
                        result, = struct.unpack('<H', payload[:2])
                        return { 'type': 'response', 'name': 'kg_rsp_motion_set_mode', 'length': payload_length, 'class_id': packet_class, 'command_id': packet_command, 'payload': { 'result': ('%04X' % result) }, 'payload_keys': [ 'result' ] }
            if packet_type & 0xC0 == 0x80: # event packet
                if packet_class == 0: # PROTOCOL
                    if packet_command == 1: # kg_evt_protocol_error
                        code, = struct.unpack('<H', payload[:2])
                        return { 'type': 'event', 'name': 'kg_evt_protocol_error', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'code': ('%04X' % code) }, 'payload_keys': [ 'code' ] }
                elif packet_class == 1: # SYSTEM
                    if packet_command == 1: # kg_evt_system_boot
                        major, minor, patch, protocol, timestamp, = struct.unpack('<HHHHL', payload[:12])
                        return { 'type': 'event', 'name': 'kg_evt_system_boot', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'major': ('%d' % (major)), 'minor': ('%d' % (minor)), 'patch': ('%d' % (patch)), 'protocol': ('%d' % (protocol)), 'timestamp': ('%d' % (timestamp)) }, 'payload_keys': [ 'major', 'minor', 'patch', 'protocol', 'timestamp' ] }
                    elif packet_command == 2: # kg_evt_system_ready
                        return { 'type': 'event', 'name': 'kg_evt_system_ready', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                    elif packet_command == 3: # kg_evt_system_error
                        code, = struct.unpack('<H', payload[:2])
                        return { 'type': 'event', 'name': 'kg_evt_system_error', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'code': ('%04X' % code) }, 'payload_keys': [ 'code' ] }
                    elif packet_command == 4: # kg_evt_system_capability
                        category, record_len, = struct.unpack('<HB', payload[:3])
                        record_data = [ord(b) for b in payload[3:]]
                        return { 'type': 'event', 'name': 'kg_evt_system_capability', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'category': ('%04X' % category), 'record': ' '.join(['%02X' % b for b in record_data]) }, 'payload_keys': [ 'category', 'record' ] }
                    elif packet_command == 5: # kg_evt_system_battery_status
                        status, level, = struct.unpack('<BB', payload[:2])
                        return { 'type': 'event', 'name': 'kg_evt_system_battery_status', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'status': ('%02X' % status), 'level': ('%02X' % level) }, 'payload_keys': [ 'status', 'level' ] }
                    elif packet_command == 6: # kg_evt_system_timer_tick
                        handle, seconds, subticks, = struct.unpack('<BLB', payload[:6])
                        return { 'type': 'event', 'name': 'kg_evt_system_timer_tick', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'handle': ('%d' % (handle)), 'seconds': ('%d' % (seconds)), 'subticks': ('%d' % (subticks)) }, 'payload_keys': [ 'handle', 'seconds', 'subticks' ] }
                elif packet_class == 2: # BLUETOOTH
                    if packet_command == 1: # kg_evt_bluetooth_mode
                        mode, = struct.unpack('<B', payload[:1])
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                    elif packet_command == 2: # kg_evt_bluetooth_ready
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_ready', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                    elif packet_command == 3: # kg_evt_bluetooth_inquiry_response
                        address, cod, rssi, status, pairing, name_len, = struct.unpack('<6s3sbBBB', payload[:13])
                        address = [ord(b) for b in payload[0:6]]
                        cod = [ord(b) for b in payload[6:9]]
                        name_data = [ord(b) for b in payload[13:]]
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_inquiry_response', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'address': ':'.join(['%02X' % b for b in address][::-1]), 'cod': ''.join(['%02X' % b for b in cod][::-1]), 'rssi': ('%d' % (rssi)), 'status': ('%02X' % status), 'pairing': ('%d' % (pairing)), 'name': ''.join(['%c' % b for b in name_data]) }, 'payload_keys': [ 'address', 'cod', 'rssi', 'status', 'pairing', 'name' ] }
                    elif packet_command == 4: # kg_evt_bluetooth_inquiry_complete
                        count, = struct.unpack('<B', payload[:1])
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_inquiry_complete', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'count': ('%d' % (count)) }, 'payload_keys': [ 'count' ] }
                    elif packet_command == 5: # kg_evt_bluetooth_pairing_status
                        pairing, address, priority, profiles_supported, profiles_active, handle_list_len, = struct.unpack('<B6sBBBB', payload[:11])
                        address = [ord(b) for b in payload[1:7]]
                        handle_list_data = [ord(b) for b in payload[11:]]
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_pairing_status', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'pairing': ('%d' % (pairing)), 'address': ':'.join(['%02X' % b for b in address][::-1]), 'priority': ('%02X' % priority), 'profiles_supported': ('%02X' % profiles_supported), 'profiles_active': ('%02X' % profiles_active), 'handle_list': ' '.join(['%02X' % b for b in handle_list_data]) }, 'payload_keys': [ 'pairing', 'address', 'priority', 'profiles_supported', 'profiles_active', 'handle_list' ] }
                    elif packet_command == 6: # kg_evt_bluetooth_pairing_failed
                        address, = struct.unpack('<6s', payload[:6])
                        address = [ord(b) for b in payload[0:6]]
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_pairing_failed', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'address': ':'.join(['%02X' % b for b in address][::-1]) }, 'payload_keys': [ 'address' ] }
                    elif packet_command == 7: # kg_evt_bluetooth_pairings_cleared
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_pairings_cleared', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': {  }, 'payload_keys': [  ] }
                    elif packet_command == 8: # kg_evt_bluetooth_connection_status
                        handle, address, pairing, profile, status, = struct.unpack('<B6sBBB', payload[:10])
                        address = [ord(b) for b in payload[1:7]]
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_connection_status', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'handle': ('%d' % (handle)), 'address': ':'.join(['%02X' % b for b in address][::-1]), 'pairing': ('%d' % (pairing)), 'profile': ('%02X' % profile), 'status': ('%02X' % status) }, 'payload_keys': [ 'handle', 'address', 'pairing', 'profile', 'status' ] }
                    elif packet_command == 9: # kg_evt_bluetooth_connection_closed
                        handle, reason, = struct.unpack('<BH', payload[:3])
                        return { 'type': 'event', 'name': 'kg_evt_bluetooth_connection_closed', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'handle': ('%d' % (handle)), 'reason': ('%04X' % reason) }, 'payload_keys': [ 'handle', 'reason' ] }
                elif packet_class == 3: # FEEDBACK
                    if packet_command == 1: # kg_evt_feedback_blink_mode
                        mode, = struct.unpack('<B', payload[:1])
                        return { 'type': 'event', 'name': 'kg_evt_feedback_blink_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                    elif packet_command == 2: # kg_evt_feedback_piezo_mode
                        index, mode, duration, frequency, = struct.unpack('<BBBH', payload[:5])
                        return { 'type': 'event', 'name': 'kg_evt_feedback_piezo_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': ('%d' % (index)), 'mode': ('%02X' % mode), 'duration': ('%d' % (duration)), 'frequency': ('%d' % (frequency)) }, 'payload_keys': [ 'index', 'mode', 'duration', 'frequency' ] }
                    elif packet_command == 3: # kg_evt_feedback_vibrate_mode
                        index, mode, duration, = struct.unpack('<BBB', payload[:3])
                        return { 'type': 'event', 'name': 'kg_evt_feedback_vibrate_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': ('%d' % (index)), 'mode': ('%02X' % mode), 'duration': ('%d' % (duration)) }, 'payload_keys': [ 'index', 'mode', 'duration' ] }
                    elif packet_command == 4: # kg_evt_feedback_rgb_mode
                        index, mode_red, mode_green, mode_blue, = struct.unpack('<BBBB', payload[:4])
                        return { 'type': 'event', 'name': 'kg_evt_feedback_rgb_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': ('%d' % (index)), 'mode_red': ('%02X' % mode_red), 'mode_green': ('%02X' % mode_green), 'mode_blue': ('%02X' % mode_blue) }, 'payload_keys': [ 'index', 'mode_red', 'mode_green', 'mode_blue' ] }
                elif packet_class == 4: # TOUCH
                    if packet_command == 1: # kg_evt_touch_mode
                        mode, = struct.unpack('<B', payload[:1])
                        return { 'type': 'event', 'name': 'kg_evt_touch_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'mode': ('%02X' % mode) }, 'payload_keys': [ 'mode' ] }
                    elif packet_command == 2: # kg_evt_touch_status
                        status_len, = struct.unpack('<B', payload[:1])
                        status_data = [ord(b) for b in payload[1:]]
                        return { 'type': 'event', 'name': 'kg_evt_touch_status', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'status': ' '.join(['%02X' % b for b in status_data]) }, 'payload_keys': [ 'status' ] }
                elif packet_class == 5: # MOTION
                    if packet_command == 1: # kg_evt_motion_mode
                        index, mode, = struct.unpack('<BB', payload[:2])
                        return { 'type': 'event', 'name': 'kg_evt_motion_mode', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': ('%d' % (index)), 'mode': ('%02X' % mode) }, 'payload_keys': [ 'index', 'mode' ] }
                    elif packet_command == 2: # kg_evt_motion_data
                        index, flags, data_len, = struct.unpack('<BBB', payload[:3])
                        data_data = [ord(b) for b in payload[3:]]
                        return { 'type': 'event', 'name': 'kg_evt_motion_data', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': ('%d' % (index)), 'flags': ('%02X' % flags), 'data': ' '.join(['%02X' % b for b in data_data]) }, 'payload_keys': [ 'index', 'flags', 'data' ] }
                    elif packet_command == 3: # kg_evt_motion_state
                        index, state, = struct.unpack('<BB', payload[:2])
                        return { 'type': 'event', 'name': 'kg_evt_motion_state', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'index': ('%d' % (index)), 'state': ('%02X' % state) }, 'payload_keys': [ 'index', 'state' ] }
                elif packet_class == 0xFF: # LOG
                    if packet_command == 0xFF: # kg_log
                        level, = struct.unpack('<B', self.kgapi_rx_payload[:1])
                        message = self.kgapi_rx_payload[1:]
                        return { 'type': 'event', 'name': 'kg_log', 'length': payload_length, 'class_id': packet_class, 'event_id': packet_command, 'payload': { 'level': ('%d' % level), 'message': ''.join(['%c' % b for b in message]) }, 'payload_keys': [ 'level', 'message' ] }

# ======== ======== ======== ======== ======== ======== ======== ========
# ======== ======== ======== ======== ======== ======== ======== ========
# ======== ======== ======== ======== ======== ======== ======== ========

# initialize backend detection
usb_options = []
usb_selected = None
serial_options = []
serial_selected = None

# check for PyWinUSB
if platform.system() == "Windows":
    try:
        import pywinusb.hid
        usb_options.append("pywinusb")
        if usb_selected == None: usb_selected = "pywinusb"
    except ImportError as e:
        pass

# check for PyUSB
try:
    import usb.core
    import usb.util
    usb_options.append("pyusb")
    if usb_selected == None: usb_selected = "pyusb"
except ImportError as e:
    pass

# check for PySerial
try:
    import serial
    import serial.tools.list_ports
    serial_options.append("pyserial")
    if serial_selected == None: serial_selected = "pyserial"
except ImportError as e:
    pass

def get_devices(type=None, relaxed=False):
    if type == None or type == "hid":

        if usb_selected == "pywinusb":
            hidListTeensy = pywinusb.hid.HidDeviceFilter(vendor_id=0x16C0, product_id=0x0486).get_devices()
            hidListKeyglove = pywinusb.hid.HidDeviceFilter(vendor_id=0x1D50, product_id=0x6025).get_devices()
            hidNum = 0
            for index, device in enumerate(hidListTeensy):
                if ("&MI_00" in device.instance_id.upper()) or relaxed == True:
                    # Teensy in USB RawHID mode provides two entries, MI_00 is the one we want (MI_01 is serial emulation)
                    yield KeygloveDevice(devobj=device, type="hid", backend="pywinusb", transport="usb", hardware="teensy", port="HID%d" % hidNum, description="Teensy USB raw HID device", mac=None)
                    hidNum = hidNum + 1
            for index, device in enumerate(hidListKeyglove):
                if ("&0003" in device.instance_id.upper() and "{00001124-" in device.instance_id.upper()) or relaxed == True:
                    # Bluetooth HumanInterfaceDeviceServiceClass is 00001124-0000-1000-8000-00805F9B34FB
                    # Keyglove custom HID descriptor has four report types, index 3 is Raw HID
                    mac_address = None
                    mac_address = ':'.join([device.serial_number[i:i+2] for i in range(0, len(device.serial_number), 2)]).upper()
                    yield KeygloveDevice(devobj=device, type="hid", backend="pywinusb", transport="bluetooth", hardware="keyglove", port="HID%d" % hidNum, description="Keyglove Bluetooth raw HID device (%s)" % mac_address, mac=mac_address)
                    hidNum = hidNum + 1

        elif usb_selected == "pyusb":
            hidListTeensy = usb.core.find(idVendor=0x16C0, idProduct=0x0486, find_all=True)
            hidListKeyglove = usb.core.find(idVendor=0x1D50, idProduct=0x6025, find_all=True)
            hidNum = 0
            for device in hidListTeensy:
                yield KeygloveDevice(devobj=device, type="hid", backend="pyusb", transport="usb", hardware="teensy", port="HID%d" % hidNum, description="Teensy USB raw HID device")
                hidNum = hidNum + 1
            for device in hidListKeyglove:
                yield KeygloveDevice(devobj=device, type="hid", backend="pyusb", transport="bluetooth", hardware="keyglove", port="HID%d" % hidNum, description="Keyglove Bluetooth raw HID device")
                hidNum = hidNum + 1

    if type == None or type == "serial":

        if serial_selected == "pyserial":
            for port in serial.tools.list_ports.comports():
                devtransport = "generic"
                devhardware = "generic"
                mac_address = None
                if "16C0:048" in port[2].upper():
                    # Teensy USB
                    devtransport = "usb"
                    devhardware = "teensy"
                    description = "Teensy USB serial device"
                elif "1D50_PID&6025" in port[2].upper():
                    # Keyglove USB or Bluetooth
                    description = "Keyglove USB serial device"
                    devtransport = "usb"
                    if "BTHENUM" in port[2].upper():
                        devtransport = "bluetooth"
                        parts = port[2].split("&")
                        if len(parts) > 5:
                            mac_parts = parts[5].split("_")
                            mac_address = ':'.join([mac_parts[0][i:i+2] for i in range(0, len(mac_parts[0]), 2)]).upper()
                        description = "Keyglove Bluetooth serial device (%s)" % mac_address
                elif relaxed == True:
                    # unknown/generic/?
                    description = port[2]
                    if "BTHENUM" in port[2].upper():
                        devtransport = "bluetooth"
                        parts = port[2].split("&")
                        if len(parts) > 5:
                            mac_parts = parts[5].split("_")
                            mac_address = ':'.join([mac_parts[0][i:i+2] for i in range(0, len(mac_parts[0]), 2)]).upper()
                    if "16C0:048" in port[2].upper():
                        devhardware = "teensy"
                        description = "Teensy USB serial device"
                    if "1D50_PID&6025" in port[2].upper():
                        devhardware = "keyglove"
                        description = "Keyglove Bluetooth serial device (%s)" % mac_address
                else:
                    # move to the next object, no matches
                    continue

                yield KeygloveDevice(devobj=port[1], type="serial", backend="pyserial", transport=devtransport, hardware=devhardware, port=port[0], description=description, mac=mac_address)
