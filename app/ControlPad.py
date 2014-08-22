#author: De Vestel Gert-Jan
import PyQt4
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import sys

class ControlPad(QWidget):
    def __init__(self, parent=None):
        super(ControlPad, self).__init__(parent)
        self.parent=parent
        self.keys=[False,False,False,False,False,False,False,False]
        self.x=[25,125,75,75,225,325,275,275]
        self.y=[75,75,25,125,75,75,25,125]
        
    def reset(self):
        self.keys=[False,False,False,False,False,False,False,False]
        self.repaint()


    def paintEvent(self, event):

        painter=QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)

        rectangle=QRectF(10.0, 10.0, 400.0, 200.0)
        painter.drawRoundedRect(rectangle, 50.0, 50.0)

        painter.translate(10,10)
        painter.setPen(Qt.NoPen)

        for i in range(len(self.x)):
            if self.keys[i]:
            	painter.setBrush(QColor(0,255,0))
            else:
                painter.setBrush(QColor(200,200,200))

            painter.drawEllipse(self.x[i], self.y[i], 50, 50)



    def sizeHint(self):
        return QSize(420,220)

    def keyPressEvent(self, event):
		if event.isAutoRepeat():
			return

		i=-1
		key = event.key()
		if key == Qt.Key_Q:
			i=0
		if key == Qt.Key_D:
			i=1
		if key == Qt.Key_Z:
			i=2
		if key == Qt.Key_W:
			i=3
		if key == Qt.Key_Left:
			i=4
		if key == Qt.Key_Right:
			i=5
		if key == Qt.Key_Up:
			i=6
		if key == Qt.Key_Down:
			i=7
		if key == Qt.Key_Tab:
			self.emit(SIGNAL("TabPressed()"))


		if i!=-1:
			if self.keys[i]!=True:
				self.keys[i]=True
				self.keyChanged()
				self.repaint()

    def keyReleaseEvent(self, event):
		if event.isAutoRepeat():
			return

		i=-1
		key = event.key()
		if key == Qt.Key_Q:
			i=0
		if key == Qt.Key_D:
			i=1
		if key == Qt.Key_Z:
			i=2
		if key == Qt.Key_W:
			i=3
		if key == Qt.Key_Left:
			i=4
		if key == Qt.Key_Right:
			i=5
		if key == Qt.Key_Up:
			i=6
		if key == Qt.Key_Down:
			i=7

		if i!=-1:
			if self.keys[i]!=False:
				self.keys[i]=False
				self.keyChanged()
				self.repaint()

    def keyChanged(self):
        a=0x00
        for i in range(len(self.keys)):
            if self.keys[i]:
                a=a | (0x01<<i)

        self.emit(SIGNAL("Key(int)"),a)
