#author: De Vestel Gert-Jan
import PySide
from PySide.QtGui import QApplication, QIcon
from BluetoothWindow import *
from BluetoothThread import *
from Selector import *
import sys
import os
import serial
import serial.tools
from serial.tools import list_ports
import image_rc


sendBuffer=[]
receiveBuffer=[]

bt=None
rm=None


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
			
def ThreadCloser():
	bt.stop()
	pass

if __name__ == '__main__':

	app = QApplication(sys.argv)
	app.aboutToQuit.connect(ThreadCloser)
	
		
	bt=BluetoothThread(sendBuffer,receiveBuffer)
	rm=BluetoothWindow(receiveBuffer,sendBuffer)

	app.setApplicationName("QuadCopter Remote")
	app.setWindowIcon(QIcon(':logo.gif'))
	if (os.name == 'nt'):
	# This is needed to display the app icon on the taskbar on Windows 7
		import ctypes
		myappid = 'QuadCopter Remote' # arbitrary string
		ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)

	p=list(serial_ports())

	buttonBox=Selector(p)
	if buttonBox.exec_():
		a= buttonBox.getValue()
		if a==-1:
			pass
		else:
			bt.initialize(p[a])

			QObject.connect(bt,SIGNAL("Post()"), rm.receiveText)
			QObject.connect(rm,SIGNAL("Send()"), bt.send)

			rm.setWindowTitle("QuadCopter Remote")
			rm.showMaximized()

	sys.exit(app.exec_())

