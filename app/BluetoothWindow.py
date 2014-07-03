import PySide
from PySide.QtCore import SIGNAL,QObject,QEvent,Qt
from PySide.QtGui import QPlainTextEdit,QWidget,QInputDialog,QSplitter,QMainWindow
from ControlPad import *
from Compass import *
from Attitude import *
import math

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

		self.cp=ControlPad()
		self.input=ChatInput()
		self.output=QPlainTextEdit()
		self.compass=Compass()
		self.ai=Attitude()

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
		self.ver.addWidget(self.cp)

		self.ver2.addWidget(self.output)
		self.ver2.addWidget(self.hor2)

		self.hor2.addWidget(self.compass)
		self.hor2.addWidget(self.ai)

		self.setCentralWidget(self.hor)

		QObject.connect(self.input,SIGNAL("EnterPressed()"), self.sendText)
		QObject.connect(self.input,SIGNAL("TabPressed()"), self.swapFocus)
		QObject.connect(self.cp,SIGNAL("TabPressed()"), self.swapFocus)
		QObject.connect(self.cp,SIGNAL("Key(int)"), self.sendKey)

		self.ver.setStretchFactor(0,7)
		self.ver.setStretchFactor(1,3)
		self.hor.setStretchFactor(0,7)
		self.hor.setStretchFactor(1,3)
		self.ver2.setStretchFactor(0,6)
		self.ver2.setStretchFactor(1,4)
		self.hor2.setStretchFactor(0,5)
		self.hor2.setStretchFactor(1,5)

    def receiveText(self):
		try:
			GYRO_SENSITIVITY_500 = 57.1429
			MAGN_SENSITIVITY_8_1 = [230, 230, 205]
			
			raw=[]
			for i in range(len(self.receivebuf)):
				raw.append(ord(self.receivebuf.pop(0)))
				
			merged = [((raw[2*i] << 8) + raw[2*i+1]) for i in range(9)]
			
			# Fix signedness
			for i in range(5):
				if merged[i] & 0x8000:
					merged[i] -= 0xFFFF

			# FS = 500 dps
			gyroscope = [merged[i] / GYRO_SENSITIVITY_500 for i in range(3)]
			# FS = 8100 mGauss
			magnetometer = [merged[i] * 1000 / MAGN_SENSITIVITY_8_1[i-3] for i in range(3,6)]
			# FS = 2000 mg
			accelerometer = [merged[i] >> 4 for i in range(6,9)]
			for i in xrange(len(accelerometer)):
				if accelerometer[i] & 0x800:
					accelerometer[i] -= 0xFFF

			#self.output.appendPlainText("Device said:\n"+" ".join(gyroscope)+"\n"+" ".join(magnetometer)+"\n"+" ".join(accelerometer)+"\n")

			# Calculate pitch and roll
			pitch = math.atan2(-accelerometer[0], math.sqrt(accelerometer[1]**2 + accelerometer[2]**2))
			roll = math.atan2(-accelerometer[1], math.sqrt(accelerometer[0]**2 + accelerometer[2]**2))
			
			self.ai.setPitch(math.degrees(pitch))
			self.ai.setRoll(math.degrees(roll))

			#print "Pitch: {0}".format(math.degrees(pitch))
			#print "Roll: {0}".format(math.degrees(roll))

			Xh = -1 * magnetometer[0] * math.cos(pitch) + magnetometer[2] * math.sin(pitch)
			Yh = -1 * magnetometer[0] * math.sin(roll) * math.sin(pitch) - magnetometer[1] * math.cos(roll) - magnetometer[2] * math.sin(roll) * math.cos(pitch)

			heading = math.atan2(Yh, Xh)

			if math.degrees(heading) < 0:
				heading += math.pi * 2

			#print "Heading: {0}".format(math.degrees(heading))

			self.compass.setOrientation(math.degrees(heading))
			self.output.appendPlainText("Device said:\n"+str(pitch)+" "+str(roll)+" "+str(heading))
			
		except:
			pass

    def sendText(self):
		text=self.input.toPlainText()
		self.output.appendPlainText("You said:\n"+text+"\n")
		self.sendbuf.append(str(text))
		self.emit(SIGNAL("Send()"))
		self.input.clear()

    def sendKey(self, text):
	
		self.output.appendPlainText("You said:\n"+str(int(text))+"\n")
		self.sendbuf.append(chr(text))
		self.emit(SIGNAL("Send()"))

    def swapFocus(self):
        if self.cp.hasFocus():
            self.input.setFocus()
        else:
            self.cp.setFocus()



if __name__ == '__main__':
    app = QApplication(sys.argv)

    sendBuffer=[]
    receiveBuffer=[]

    chatter=BluetoothWindow(receiveBuffer,sendBuffer)
    chatter.setWindowTitle("QuadCopterRemote")
    chatter.showMaximized()

    sys.exit(app.exec_())