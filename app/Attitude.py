#author: De Vestel Gert-Jan
import PyQt4
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import sys
import time

size=100

class Attitude(QWidget):

    sky=QColor(102,255,255)
    earth=QColor(204,102,0)

    def __init__(self, parent=None):
    	super(Attitude, self).__init__(parent)
    	self.parent=parent
    	self.roll=0
    	self.pitch=0
    	self.resize(size,size)
    	self.im=QImage()

    def paintEvent(self, event):
        side = min(self.width(), self.height())

        self.im=QImage(side,side,QImage.Format_RGB32)
        painter = QPainter(self.im)
        painter.setRenderHint(QPainter.Antialiasing)
        painter.scale(side / 200.0, side / 200.0)
        painter.setPen(Qt.NoPen)
        painter.translate(100,100)
        painter.save()

        if self.pitch>=90 and self.pitch<270:
        	painter.setBrush(Attitude.sky)
        	painter.drawRect(-100,-100,200,200)
        	painter.setBrush(Attitude.earth)
        	painter.rotate(self.roll+180)
        	painter.drawRect(-200,-100+(60-(self.pitch-150))*20.0/6.0,400,400)

        if self.pitch<90 or self.pitch>=270:
        	p=0
        	if self.pitch<90:
        		p=self.pitch+30
        	else:
        		p=self.pitch-330

        	painter.setBrush(Attitude.earth)
        	painter.drawRect(-100,-100,200,200)
        	painter.setBrush(Attitude.sky)
        	painter.rotate(self.roll+180)
        	painter.drawRect(-200,-100+(60-p)*20.0/6.0,400,400)

        painter.restore()
        painter.setBrush(Qt.black)
        painter.drawRect(-20,-4,40,8)

        painter2=QPainter(self)
        painter2.drawImage(QPoint((self.width()-side) / 2,(self.height()-side) / 2),self.im)

    def setRoll(self, roll):
    	self.roll=roll
    	self.repaint()

    def setPitch(self, pitch):
    	self.pitch=pitch
    	self.repaint()

    def sizeHint(self):
    	return QSize(size+10,size+10)


if __name__ == '__main__':
    app = QApplication(sys.argv)

    w=Attitude()
    w.show()

#	for i in range(361):
#		w.setRoll(i)
#		print i
#		time.sleep(0.050)
#		app.processEvents()

#	for i in range(361):
#		w.setPitch(i)
#		print i
#		time.sleep(0.050)
#		app.processEvents()

    for i in range(361):
		w.setPitch(i)
		w.setRoll(i)
		print i
		time.sleep(0.050)
		app.processEvents()

    sys.exit(app.exec_())