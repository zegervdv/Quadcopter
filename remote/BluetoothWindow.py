import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas

from Compass import *
from Attitude import *
import math
import struct

pidvalues = {
    'PID_SAMPLE_TIME': 1000,
    'KP_PITCH': 45000.0 / math.pi,
    'KI_PITCH': 0,
    'KD_PITCH': 0,
    'KP_ROLL': 45000.0 / math.pi,
    'KI_ROLL': 0,
    'KD_ROLL': 0,
    'KP_YAW': 0,
    'KI_YAW': 0,
    'KD_YAW': 0
}

TIM_FREQ = 2e6


class BluetoothWindow(QMainWindow):
    def __init__(self, in_buf, out_buf, parent=None):

        super(BluetoothWindow, self).__init__(parent)

        self.receive_buffer = in_buf
        self.send_buffer = out_buf

        self.compass = Compass()
        self.ai = Attitude()

        self.hor = QHBoxLayout()
        self.hor.addWidget(self.compass)
        self.hor.addWidget(self.ai)

        self.figure = plt.figure()
        self.canvas = FigureCanvas(self.figure)
        self.lines = []
        self.xdata = range(100)
        self.buffers = [[0 for y in range(100)] for x in range(4)]
        self.prepare_plots()

        self.ver2 = QVBoxLayout()
        self.prepare_updaters()

        self.ver = QVBoxLayout()
        self.ver.addLayout(self.hor, 1)
        self.ver.addWidget(self.canvas, 1)

        self.main_hor = QHBoxLayout()
        self.main_hor.addLayout(self.ver, 3)
        self.main_hor.addLayout(self.ver2, 1)

        w = QWidget()
        w.setLayout(self.main_hor)
        self.setCentralWidget(w)

    def prepare_updaters(self):
        def update_value(key, value):
            pidvalues[key] = float(value)
            data = [1]
            for element in pidvalues:
                if element == "PID_SAMPLE_TIME":
                    continue
                data.append(pidvalues[element])
            self.send_buffer.append(data)
            print key, "updated to", pidvalues[key]

        from collections import OrderedDict

        helper = OrderedDict(sorted(pidvalues.items(), key=lambda t: t[0]))

        for k, v in helper.iteritems():
            label = QLabel(k)
            edit = QLineEdit(str(v))
            button = QPushButton("Update")
            l = QHBoxLayout()
            l.addWidget(label, 2)
            l.addWidget(edit, 5)
            l.addWidget(button, 2)
            self.ver2.addLayout(l)
            button.clicked.connect(lambda clicked, label=label, edit=edit: update_value(label.text(), edit.text()))

    def prepare_plots(self):
        # throttle
        ax = plt.subplot(2, 2, 1)
        ax.set_xlim(0, 100)
        ax.set_ylim(1500, 4000)
        plt.title("Throttle")
        line = Line2D([], [], color='black')
        ax.add_line(line)
        self.lines.append(line)
        line.set_data(self.xdata, self.buffers[0][0:100])
        # pitch
        ax = plt.subplot(2, 2, 2)
        ax.set_xlim(0, 100)
        ax.set_ylim(1500, 4000)
        plt.title("Pitch")
        line = Line2D([], [], color='black')
        ax.add_line(line)
        self.lines.append(line)
        line.set_data(self.xdata, self.buffers[1][0:100])
        # roll
        ax = plt.subplot(2, 2, 3)
        ax.set_xlim(0, 100)
        ax.set_ylim(1500, 4000)
        plt.title("Roll")
        line = Line2D([], [], color='black')
        ax.add_line(line)
        self.lines.append(line)
        line.set_data(self.xdata, self.buffers[2][0:100])
        # yaw
        ax = plt.subplot(2, 2, 4)
        ax.set_xlim(0, 100)
        ax.set_ylim(1500, 4000)
        plt.title("Yaw")
        line = Line2D([], [], color='black')
        ax.add_line(line)
        self.lines.append(line)
        line.set_data(self.xdata, self.buffers[3][0:100])

    def plot(self, throttle, pitch, roll, yaw):
        self.buffers[0].append(throttle)
        self.buffers[0].pop(0)
        self.buffers[1].append(pitch)
        self.buffers[1].pop(0)
        self.buffers[2].append(roll)
        self.buffers[2].pop(0)
        self.buffers[3].append(yaw)
        self.buffers[3].pop(0)

        for i in range(4):
            self.lines[i].set_ydata(self.buffers[i])

        self.canvas.draw()

    def receive_text(self):

        # | 2-5   | Roll    | [-pi, pi]   |
        # | 6-9   | Pitch   | [-pi, pi]   |
        # | 10-13 | Yaw     | [-pi, pi]   |
        # | 14-17 | Height  | [0, 10m]  |
        # | 18-21 | Battery | [0, 100%] |

        new_status = []
        while len(self.receive_buffer) != 0:
            new_status = self.receive_buffer.pop(0)

        roll, pitch, yaw, height, battery = struct.unpack('fffff', ''.join(new_status))

        self.ai.set_pitch(math.degrees(pitch))
        self.ai.set_roll(math.degrees(roll))
        self.compass.set_orientation(math.degrees(yaw))

        # TODO fill in with proper values when they are sent correctly
        self.plot(3000 + 100 * (pitch + roll + yaw), pitch, roll, yaw)

        self.repaint()

    def reset(self):
        self.compass.reset()
        self.ai.reset()
