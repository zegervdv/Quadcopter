#author: De Vestel Gert-Jan
import PyQt4
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import sys

size=200
sticksize=0.2

class Joystick(QWidget):

    def __init__(self, parent=None):
        super(Joystick, self).__init__(parent)
        self.parent=parent
        self.resize(size,size)
        self.follow=False
        self.x=0.5
        self.y=0.5
        self.setMouseTracking(True)
        self.im=None
        
    def reset(self):
        self.follow=False
        self.x=0.5
        self.y=0.5
        self.repaint()

    def paintEvent(self, event):

        side = min(self.width(), self.height())

        self.im=QImage(side,side,QImage.Format_ARGB32)
        self.im.fill(QColor(0,0,0,0));
    
        painter = QPainter(self.im)
        painter.setPen(Qt.NoPen)
        painter.setRenderHint(QPainter.Antialiasing)
        painter.scale(side / 200.0, side / 200.0)
        
        painter.setBrush(Qt.black)
        painter.drawEllipse(0,0,200,200)
        
        if self.follow:
            painter.setBrush(Qt.green)
        else:
            painter.setBrush(Qt.red)
        painter.drawEllipse((self.x-sticksize/2)*200,(self.y-sticksize/2)*200,sticksize*200,sticksize*200)
       
       
       
        painter.setPen(Qt.yellow)
        painter.drawLine(0,100,200,100)  
        painter.drawLine(100,0,100,200) 
        
        for i in range(1,9):
            painter.drawLine(95,i*25,105,i*25)  
            painter.drawLine(i*25,95,i*25,105)  
        
        painter.setPen(Qt.NoPen) 
        
        painter2=QPainter(self)
        painter2.drawImage(QPoint((self.width()-side) / 2,(self.height()-side) / 2),self.im)

        
    def sizeHint(self):
        return QSize(size+10,size+10)
        
        
    def mouseMoveEvent(self, event):
        if not self.follow:
            return
        
        side = float(min(self.width(), self.height()))
        a=event.pos().x()-((self.width()-side)/2.0)
        b=event.pos().y()-((self.height()-side)/2.0)
        
        if a<0 or b<0:
            return
            
        if a>side or b>side:
            return
            
        c=side/2.0
        if (a-c)*(a-c)+(b-c)*(b-c)>c*c:
            return
            
        self.x=a/side
        self.y=b/side
        
        pos=int(self.x*4)<<4
        pos=pos|int(self.y*4)

        self.emit(SIGNAL("Moved(int)"),pos)        
        
        self.repaint()
        
    def mousePressEvent(self, event):
        self.follow=not self.follow
        self.repaint()
        
    def mouseReleaseEvent(self, event):
        pass

    def wheelEvent(self, event):
        pass

    def enterEvent(self, event):
        pass

    def leaveEvent(self, event):
        pass


if __name__ == '__main__':
    app = QApplication(sys.argv)
    w=Joystick()
    w.show()

    sys.exit(app.exec_())