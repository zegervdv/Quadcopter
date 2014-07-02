#Gert-Jan De Vestel
import PySide
from PySide.QtGui import QDialog, QPushButton,QVBoxLayout
from PySide.QtCore import QObject,SIGNAL


class Selector(QDialog):

    def __init__(self, items, parent=None):
        super(Selector, self).__init__(parent)

        self.layout=QVBoxLayout()
        self.setModal(True)
        self.setWindowTitle("Make a selection:")

        self.parent=parent

        self.selectorLayout=QVBoxLayout()
        self.buttons=[]

        for a in items:
            b=QPushButton(a)
            self.buttons.append(b)
            self.selectorLayout.addWidget(b)
            self.connect(b, SIGNAL("clicked()"), self.buttonClicked)

        self.value=-1
        self.setLayout(self.selectorLayout)


    def buttonClicked(self):

        s=self.sender()
        for b in self.buttons:
            if s == b:
                self.value=self.buttons.index(b)
                self.accept()
                break


    def getValue(self):
        return self.value
