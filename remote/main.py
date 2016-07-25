from BluetoothWindow import *
from BluetoothThread import *
from Selector import *
import sys
import os
import serial
import serial.tools
from serial.tools import list_ports
import pygame

app = QApplication(sys.argv)

send_buffer = []
receive_buffer = []

bt = None
rm = None


def single_iteration():

    r = p = t = y = 0
    if bt:
        if my_joystick is not None:
            # get the newest status
            pygame.event.get()

            y = my_joystick.get_axis(1) * math.pi
            t = - my_joystick.get_axis(0) * 100
            r = my_joystick.get_axis(2) * math.pi
            p = - my_joystick.get_axis(3) * math.pi

    send_buffer.append([0, r, p, t, y])
    bt.send()

    if len(receive_buffer):
        rm.receive_text()


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


def close_thread():
    bt.stop()
    pass


def startup():
    p = list(serial_ports())
    p.append("Rescan")
    p.append("Exit")

    buttonbox = Selector(p)
    if buttonbox.exec_():
        a = buttonbox.getValue()

        if a == len(p) - 1:
            sys.exit()
        elif a == len(p) - 2:
            startup()
            return
        else:
            rm.reset()
            bt.initialize(p[a])
            rm.showMaximized()
    else:
        sys.exit()


if __name__ == '__main__':

    app.aboutToQuit.connect(close_thread)
    app.setApplicationName("QuadCopter Remote")
    app.setWindowIcon(QIcon('logo.png'))

    if os.name == 'nt':
        # This is needed to display the app icon on the taskbar on Windows 7
        import ctypes

        app_id = 'QuadCopter Remote'  # arbitrary string
        ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID(app_id)

    bt = BluetoothThread(send_buffer, receive_buffer)
    rm = BluetoothWindow(receive_buffer, send_buffer)

    rm.setWindowTitle("QuadCopter Remote")

    startup()
    app.connect(bt, SIGNAL("Closing"), startup)

    pygame.init()
    print "Joysticks: ", pygame.joystick.get_count()
    if pygame.joystick.get_count() != 0:
        my_joystick = pygame.joystick.Joystick(0)
        my_joystick.init()

    timer = QTimer()
    QObject.connect(timer, SIGNAL("timeout()"), single_iteration)
    timer.start(500)

    sys.exit(app.exec_())