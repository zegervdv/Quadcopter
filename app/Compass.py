#author: De Vestel Gert-Jan
import PyQt4
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import sys
import time

size=100

class Compass(QWidget):

    north = QPolygon([
        QPoint(7, 0),
        QPoint(-7, 0),
        QPoint(0, -80)
    ])

    south = QPolygon([
        QPoint(7, 0),
        QPoint(-7, 0),
        QPoint(0, 80)
    ])

    def __init__(self, parent=None):
        super(Compass, self).__init__(parent)
        self.parent=parent
        self.orientation=0
        self.resize(size,size)
    
    def reset(self):
        self.orientation=0
        self.repaint()

    def paintEvent(self, event):

		side = min(self.width(), self.height())

		painter = QPainter(self)
		painter.setRenderHint(QPainter.Antialiasing)
		im=QImage("compass.png").scaled(self.size(), Qt.KeepAspectRatio)
		painter.drawImage(QPoint((self.width()-side) / 2,(self.height()-side) / 2),im)
		
		painter.translate(self.width() / 2, self.height() / 2)
		painter.scale(side / 200.0, side / 200.0)

		painter.setPen(Qt.NoPen)

		painter.save()
		painter.rotate(self.orientation)
		painter.setBrush(Qt.red)
		painter.drawConvexPolygon(Compass.north)
		painter.setBrush(Qt.blue)
		painter.drawConvexPolygon(Compass.south)
		painter.restore()

    def setOrientation(self, orientation):
        self.orientation=orientation
        self.repaint()
		
    def sizeHint(self):
        return QSize(size+10,size+10)


if __name__ == '__main__':
	app = QApplication(sys.argv)
	w=Compass()
	w.show()

	for i in range(361):
		w.setOrientation(i)
		time.sleep(0.050)
		app.processEvents()

	sys.exit(app.exec_())