#author: De Vestel Gert-Jan
import PyQt4
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import sys

class KeyPad(QWidget):
    def __init__(self, parent=None):
        super(KeyPad, self).__init__(parent)
        self.parent=parent
        self.keys=[False,False,False,False]
        self.x=[15,115,65,65]
        self.y=[75,75,25,125]
        self.im=None
        self.ypos=0
        self.xpos=0

    def reset(self):
        self.keys=[False,False,False,False]
        self.repaint()


    def paintEvent(self, event):

        side = min(self.width(), self.height())

        self.im=QImage(side,side,QImage.Format_ARGB32)
        self.im.fill(QColor(0,0,0,0));

        painter = QPainter(self.im)
        painter.setRenderHint(QPainter.Antialiasing)
        painter.scale(side / 210.0, side / 210.0)
        
        painter.setBrush(QColor(0,255,0))
        rectangle=QRectF(15,15, 150.0, 10.0)
        painter.drawRoundedRect(rectangle, 5.0, 5.0)
        
        rectangle=QRectF(185,45, 10.0, 150.0)
        painter.drawRoundedRect(rectangle, 5.0, 5.0)

        painter.setPen(Qt.NoPen)

        for i in range(len(self.x)):
            if self.keys[i]:
                painter.setBrush(QColor(0,255,0))
            else:
                painter.setBrush(QColor(200,200,200))

            painter.drawEllipse(self.x[i], self.y[i]+20, 50, 50)

        painter2=QPainter(self)
        painter2.drawImage(QPoint((self.width()-side) / 2,(self.height()-side) / 2),self.im)


    def sizeHint(self):
        return QSize(220,220)

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
            i=0
        if key == Qt.Key_Right:
            i=1
        if key == Qt.Key_Up:
            i=2
        if key == Qt.Key_Down:
            i=3
        if key == Qt.Key_Tab:
            self.emit(SIGNAL("TabPressed()"))


        if i!=-1:
            if self.keys[i]!=True:
                self.keys[i]=True
                self.keyChanged()
                self.repaint()
        event.accept()

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
            i=0
        if key == Qt.Key_Right:
            i=1
        if key == Qt.Key_Up:
            i=2
        if key == Qt.Key_Down:
            i=3

        if i!=-1:
            if self.keys[i]!=False:
                self.keys[i]=False
                self.keyChanged()
                self.repaint()
        event.accept()

    def keyChanged(self):
        self.emit(SIGNAL("Changed()"))

    def getValues(self):
        return self.xpos,self.ypos

if __name__ == '__main__':
    app = QApplication(sys.argv)
    w=KeyPad()
    w.show()

    sys.exit(app.exec_())
