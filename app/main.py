#author: De Vestel Gert-Jan
import PySide
from PySide.QtGui import QApplication
from BluetoothWindow import *
from BluetoothThread import *
from Selector import *
import sys
import os
import serial
import serial.tools
from serial.tools import list_ports

def serial_ports():
    if os.name == 'nt':
        # windows
        for i in range(256):
            try:
                s = serial.Serial(i)
                s.close()
                yield 'COM' + str(i + 1)
            except serial.SerialException:
                pass
    else:
        # unix
        for port in list_ports.comports():
            yield port[0]

if __name__ == '__main__':

    app = QApplication(sys.argv)

    sendBuffer=[]
    receiveBuffer=[]

    bt=BluetoothThread(sendBuffer,receiveBuffer)
    rm=BluetoothWindow(receiveBuffer,sendBuffer)

    p=list(serial_ports())

    buttonBox=Selector(p)
    if buttonBox.exec_():
        a= buttonBox.getValue()
        if a==-1:
            sys.exit(0)
        else:
			bt.initialize(p[a])

			QObject.connect(bt,SIGNAL("Post()"), rm.receiveText)
			QObject.connect(rm,SIGNAL("Send()"), bt.send)

			rm.setWindowTitle("QuadCopterRemote")
			rm.showMaximized()

    sys.exit(app.exec_())

    bt.stop()

