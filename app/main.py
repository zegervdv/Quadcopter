#author: De Vestel Gert-Jan
import PyQt4
from PyQt4.QtGui import QApplication, QIcon
from PyQt4.QtCore import QSize
from BluetoothWindow import *
from BluetoothThread import *
from Selector import *
import sys
import os
import serial
import serial.tools
from serial.tools import list_ports


app = QApplication(sys.argv)
    
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
            

def startup():

    p=list(serial_ports())
    p.append("Rescan")
    p.append("Exit")

    buttonBox=Selector(p)
    if buttonBox.exec_():
        a= buttonBox.getValue()
        
        if a==len(p)-1:
            sys.exit()
        elif a==len(p)-2:
            startup()
            return                
        else:
            rm.reset()
            bt.initialize(p[a])
            rm.showMaximized()

    else:
        sys.exit()
        

if __name__ == '__main__':

    app.aboutToQuit.connect(ThreadCloser)    
    app.setApplicationName("QuadCopter Remote")
    app.setWindowIcon(QIcon('logo.png'))

    if (os.name == 'nt'):
    # This is needed to display the app icon on the taskbar on Windows 7
        import ctypes
        myappid = 'QuadCopter Remote' # arbitrary string
        ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)       


    bt=BluetoothThread(sendBuffer,receiveBuffer)
    rm=BluetoothWindow(receiveBuffer,sendBuffer)
    QObject.connect(bt,SIGNAL("Post()"), rm.receiveText)
    QObject.connect(rm,SIGNAL("Send()"), bt.send)
    rm.setWindowTitle("QuadCopter Remote")

    startup()
    app.connect(bt,SIGNAL("Closing"),startup)
    
    sys.exit(app.exec_())