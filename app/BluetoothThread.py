import PyQt4
from PyQt4.QtCore import QThread,QObject,SIGNAL
from PyQt4.QtGui import QErrorMessage
import serial, time, sys,copy


class BluetoothThread(QThread):
    
    timestamp=0

    def __init__(self, sbuf, rbuf, parent=None):
        QThread.__init__(self,parent)

        self.sendbuf=sbuf
        self.receivebuf=rbuf
        self.ser=serial.Serial()
        self.runThread=True
        self.databuffer=[]

    def send(self):
        if self.ser.isOpen():
            while len(self.sendbuf):
                a=self.sendbuf.pop(0)
                self.ser.write(a)
                time.sleep(0.05)

    def initialize(self,port):
        self.runThread=True
        ok=True
        while ok:
            try:
                self.ser = serial.Serial(port)
                ok=False
            except:
                continue
                
        self.ser.baudrate = 115200
        self.ser.bytesize = serial.EIGHTBITS #number of bits per bytes
        self.ser.parity = serial.PARITY_NONE #set parity check: no parity
        self.ser.stopbits = serial.STOPBITS_ONE #number of stop bits
        self.ser.timeout = 0          #block read
        self.ser.xonxoff = False     #disable software flow control
        self.ser.rtscts = False     #disable hardware (RTS/CTS) flow control
        self.ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
        self.ser.writeTimeout = 2     #timeout for write

        self.ser.close()
        while not self.ser.isOpen():
            try:
                self.ser.close()
                self.ser.open()
                self.timestamp=time.time()
                self.start()
            except Exception, e:
                print "error open serial port: " + str(e)


    def run(self):
        while self.runThread:
       
            a=self.ser.read()           
            t=time.time()
            
            if len(a)==0:
                if t-self.timestamp>5:
                    print "Connection lost"
                    self.emit(SIGNAL("Closing"))
                    break
                else:
                    continue
            
            self.timestamp=t
            self.databuffer.append(a)
            
            if len(self.databuffer)<20:
                continue
            
            if self.databuffer[0]=='U' and self.databuffer[1]=='U':
                
                self.databuffer=self.databuffer[2:]
                for i in range(len(self.databuffer)):
                    self.receivebuf.append(self.databuffer.pop(0))
                
                self.emit(SIGNAL("Post()"))
            else:
                self.databuffer.pop(0)
            
        self.ser.close()

    def stop(self):
        self.runThread=False