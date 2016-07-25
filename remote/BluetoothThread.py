import struct
import copy

from PyQt4.QtCore import QThread
import serial
import time
# import crcmod


class BluetoothThread(QThread):
    def __init__(self, send_buf, receive_buf, parent=None):
        QThread.__init__(self, parent)

        self.send_buffer = send_buf
        self.receive_buffer = receive_buf
        self.serial_connection = serial.Serial()
        self.run_thread = True
        self.data_buffer = []
        self.timer = 0

    def send(self):
        if self.serial_connection.isOpen():
            while len(self.send_buffer) > 0:
                a = self.send_buffer.pop(0)

                data = []

                if a[0] == 0:
                    if len(a) != 5:
                        continue
                    data.append(chr(0x12))  # 18 in hex, 4 * 4 + 2
                    data.append(chr(0x00))
                    for i in range(1, 5):
                        data += struct.pack('f', a[i])
                else:
                    if len(a) != 10:
                        continue
                    data.append(chr(0x26))  # 38 in hex, 4 * 9 + 2
                    data.append(chr(0x10))
                    for i in range(1, 10):
                        data += struct.pack('f', a[i])

                # crc = crcmod.Crc(0x1D5, initCrc=0, rev=False)
                # crc.update(str(data))
                # crc_byte = crc.crcValue
                #
                # data += [chr(crc_byte), '\n']

                for d in data:
                    self.serial_connection.write(d)
                print time.time(), "sent ", data

    def initialize(self, port):
        self.run_thread = True
        ok = True
        while ok:
            try:
                self.serial_connection = serial.Serial(port)
                ok = False
            except:
                continue

        self.serial_connection.baudrate = 115200
        self.serial_connection.bytesize = serial.EIGHTBITS  # number of bits per bytes
        self.serial_connection.parity = serial.PARITY_NONE  # set parity check: no parity
        self.serial_connection.stopbits = serial.STOPBITS_ONE  # number of stop bits
        self.serial_connection.timeout = 0  # block read
        self.serial_connection.xonxoff = False  # disable software flow control
        self.serial_connection.rtscts = False  # disable hardware (RTS/CTS) flow control
        self.serial_connection.dsrdtr = False  # disable hardware (DSR/DTR) flow control
        self.serial_connection.writeTimeout = 0  # timeout for write

        self.serial_connection.close()
        while not self.serial_connection.isOpen():
            try:
                self.serial_connection.close()
                self.serial_connection.open()
                self.start()
            except Exception, e:
                print "error open serial port: " + str(e)

    def run(self):
        while self.run_thread:
            a = self.serial_connection.read()
            if not a:
                continue

            self.data_buffer.append(a)
            print time.time(), "received ", hex(ord(a))

            if len(self.data_buffer) < 22:  # 22 = 5 * 4 for regular stats + 2 * U
                continue

            if self.data_buffer[0] == 'U' and self.data_buffer[1] == 'U':
                self.receive_buffer.append(copy.deepcopy(self.data_buffer[2:]))
                print time.time(), "detected ", self.data_buffer
                del self.data_buffer[:]
            else:
                self.databuffer.pop(0)

        self.serial_connection.close()

    def stop(self):
        self.run_thread = False