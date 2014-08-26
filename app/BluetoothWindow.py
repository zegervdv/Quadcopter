import PyQt4
from PyQt4.QtCore import SIGNAL,QObject,QEvent,Qt
from PyQt4.QtGui import QPlainTextEdit,QWidget,QInputDialog,QSplitter,QMainWindow
from KeyPad import *
from Compass import *
from Attitude import *
from Joystick import *
import math, struct

class ChatInput(QPlainTextEdit):

    def __init__(self, parent=None):
        super(ChatInput, self).__init__(parent)

        self.setLineWrapMode(QPlainTextEdit.NoWrap)

    def keyPressEvent (self, event):

        if event.key() == Qt.Key_Return:
            self.emit(SIGNAL("EnterPressed()"))
            event.accept()
        elif event.key() == Qt.Key_Tab:
            self.emit(SIGNAL("TabPressed()"))
            event.accept()
        else:
            QPlainTextEdit.keyPressEvent(self,event)


class BluetoothWindow(QMainWindow):

    def __init__(self, inbuf, outbuf,parent=None):

        super(BluetoothWindow, self).__init__(parent)

        self.sendbuf=outbuf
        self.receivebuf=inbuf

        self.pitchbuffer = [0] * 10
        self.rollbuffer = [0] * 10

        self.kp=KeyPad()
        self.input=ChatInput()
        self.output=QPlainTextEdit()
        self.compass=Compass()
        self.ai=Attitude()
        self.joystick=Joystick()

        self.output.setReadOnly(True)
        self.output.setLineWrapMode(QPlainTextEdit.WidgetWidth)
        self.input.setLineWrapMode(QPlainTextEdit.WidgetWidth)

        self.ver=QSplitter(Qt.Horizontal)
        self.ver2=QSplitter(Qt.Horizontal)
        self.hor=QSplitter(Qt.Vertical)
        self.hor2=QSplitter(Qt.Vertical)

        self.hor.addWidget(self.ver2)
        self.hor.addWidget(self.ver)

        self.ver.addWidget(self.input)
        self.ver.addWidget(self.joystick)
        self.ver.addWidget(self.kp)

        self.ver2.addWidget(self.output)
        self.ver2.addWidget(self.hor2)

        self.hor2.addWidget(self.compass)
        self.hor2.addWidget(self.ai)

        self.setCentralWidget(self.hor)

        QObject.connect(self.input,SIGNAL("EnterPressed()"), self.sendCommand)
        QObject.connect(self.input,SIGNAL("TabPressed()"), self.swapFocus)
        QObject.connect(self.kp,SIGNAL("TabPressed()"), self.swapFocus)
        QObject.connect(self.kp,SIGNAL("Changed()"), self.sendData)
        QObject.connect(self.joystick,SIGNAL("Changed()"), self.sendData)

        self.ver.setStretchFactor(0,7)
        self.ver.setStretchFactor(1,3)
        self.hor.setStretchFactor(0,7)
        self.hor.setStretchFactor(1,3)
        self.ver2.setStretchFactor(0,6)
        self.ver2.setStretchFactor(1,4)
        self.hor2.setStretchFactor(0,5)
        self.hor2.setStretchFactor(1,5)

    def receiveText(self):
          
#        | 2-5   | Roll    | [-pi, pi]   |
#        | 6-9   | Pitch   | [-pi, pi]   |
#        | 10-13 | Yaw     | [-pi, pi]   |
#        | 14-17 | Height  | [0, 10m]  |
#        | 18-21 | Battery | [0, 100%] |
        
        rolldata=""
        pitchdata=""
        yawdata=""
        heightdata=""
        batterydata=""
        
        for i in range(4):
            rolldata+=self.receivebuf.pop(0)
        
        for i in range(4):
            pitchdata+=self.receivebuf.pop(0)
        
        for i in range(4):
            yawdata+=self.receivebuf.pop(0)
        
        for i in range(4):
            heightdata+=self.receivebuf.pop(0)
        
        for i in range(4):
            batterydata+=self.receivebuf.pop(0)
        
        roll=struct.unpack('f', rolldata)
        pitch=struct.unpack('f', pitchdata)
        yaw=struct.unpack('f', yawdata)
        #height=struct.unpack('f', heightdata)
        #battery=struct.unpack('f', batterydata)
        
        print math.degrees(pitch[0]),math.degrees(roll[0]),math.degrees(yaw[0])
           
        self.ai.setPitch(math.degrees(pitch[0]))
        self.ai.setRoll(math.degrees(roll[0]))
        self.compass.setOrientation(math.degrees(yaw[0]))
        self.repaint()            


    def sendCommand(self):
        text=self.input.toPlainText()
        self.output.appendPlainText("You said:\n"+text+"\n")
        t=list(str(text))   
        l=float(16)
        t+=['0']*int(l*math.ceil(len(t)/l)-len(t))   
        
        for i in range(int(math.ceil(len(t)/l))): 
            n=[1]
            for j in range(int(l)):
                n.append(t[l*i+j])
                
            self.sendbuf.append(n)
            
        self.emit(SIGNAL("Send()"))
        self.input.clear()

    def sendData(self):
        
        t,y=self.kp.getValues()
        r,p=self.joystick.getValues()
        
        r=-math.pi/6.0+(math.pi*r/3.0)
        p=-math.pi/6.0+(math.pi*p/3.0)
        y=-math.pi+(2*math.pi*y)
        
        self.sendbuf.append((0,r,t,p,y))
        self.output.appendPlainText("New status:\n"+str(r)+" "+str(t)+" "+str(p)+" "+str(y)+"\n")
        
        self.emit(SIGNAL("Send()"))

    def swapFocus(self):
        if self.kp.hasFocus():
            self.input.setFocus()
        else:
            self.kp.setFocus()
            
    def reset(self):
        self.output.clear()
        self.input.clear()
        self.kp.reset()
        self.compass.reset()
        self.ai.reset()
        self.joystick.reset()


if __name__ == '__main__':
    app = QApplication(sys.argv)

    sendBuffer=[]
    receiveBuffer=[]

    chatter=BluetoothWindow(receiveBuffer,sendBuffer)
    chatter.setWindowTitle("QuadCopterRemote")
    chatter.showMaximized()

    sys.exit(app.exec_())
