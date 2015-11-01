import PyQt4
from PyQt4.QtCore import SIGNAL,QObject,QEvent,Qt
from PyQt4.QtGui import QPlainTextEdit,QWidget,QInputDialog,QSplitter,QMainWindow
import math, struct


PID_SAMPLE_TIME = 1000

PID_THROTTLE = 0
KP_THROTTLE = 0
KI_THROTTLE = 0
KD_THROTTLE = 0
MIN_THROTTLE = -100
MAX_THROTTLE = 100

PID_PITCH = 1
KP_PITCH = (450.0 / math.pi)
KI_PITCH = 0
KD_PITCH = 0
MIN_PITCH = (450.0 / (-math.pi/6))
MAX_PITCH = (450.0 / (math.pi/6))

PID_ROLL = 2
KP_ROLL = (450.0 / math.pi)
KI_ROLL = 0
KD_ROLL = 0
MIN_ROLL = (450.0 / (-math.pi/6))
MAX_ROLL = (450.0 / (math.pi/6))

PID_YAW = 3
KP_YAW = 0
KI_YAW = 0
KD_YAW = 0
MIN_YAW = -math.pi
MAX_YAW = math.pi


TIM_FREQ = 2e6
PWM_FREQ = 400
PWM_PERIOD = (TIM_FREQ / PWM_FREQ) - 1
PWM_PULSE = PWM_PERIOD / (1000.0/PWM_FREQ)

MOTOR_SPEED_MIN = 1.15 * PWM_PULSE
MOTOR_SPEED_HALF = 1.5 * PWM_PULSE
MOTOR_SPEED_MAX = 2 * PWM_PULSE
MOTOR_SPEED_OFF = 1 * PWM_PULSE

MOTOR_LEFT_FRONT = 0x1
MOTOR_RIGHT_FRONT = 0x2
MOTOR_LEFT_BACK = 0x4
MOTOR_RIGHT_BACK = 0x8
MOTOR_ALL = MOTOR_LEFT_FRONT | MOTOR_RIGHT_FRONT | MOTOR_LEFT_BACK | MOTOR_RIGHT_BACK


class pid_params_typedef(object):
    def __init__(self,**kwargs):
      self.iterm = 0;
      self.lastInput = 0;
      self.ki = 0;
      self.kp = 0;
      self.kd = 0;
      self.min = 0;
      self.max = 0;
    def __str__(self):
      return str(self.iterm)+" "+str(self.lastInput)+" "+str(self.ki)+" "+str(self.kp)+" "+str(self.kd)+" "+str(self.min)+" "+str(self.max)

class pid_output_typedef(object):
    def __init__(self):
      self.throttle = 0
      self.pitch = 0
      self.roll = 0
      self.yaw = 0
    def __str__(self):
      return str(self.throttle)+" "+str(self.pitch)+" "+str(self.roll)+" "+str(self.yaw)
          
# /*
# * Command structure
# * roll     - float between -30 and +30 (in radians)
# * pitch    - float between -30 and +30 (in radians)
# * throttle - float between -100 and 100
# * yaw      - float between -180 and +180 (in radians)
# */
class command_typedef(object):
    def __init__(self):
      self.roll = 0
      self.pitch = 0
      self.throttle = 0
      self.yaw = 0 


pid_params = []
pid_output = pid_output_typedef()
command = command_typedef()
command.throttle = 50
data = command_typedef()
lastThrottle = 0

class BluetoothWindow(QMainWindow):

    def __init__(self, inbuf, outbuf,parent=None):

        super(BluetoothWindow, self).__init__(parent)

        self.sendbuf=outbuf
        self.receivebuf=inbuf
        
        self.output=QPlainTextEdit()

        self.output.setReadOnly(True)
        self.output.setLineWrapMode(QPlainTextEdit.WidgetWidth)
       
        self.setCentralWidget(self.output)
        
        SampleTimeInSec = (PID_SAMPLE_TIME) / 1000.0
        
        for i in range(4):
          pid_params.append(pid_params_typedef())
        
        pid_params[PID_THROTTLE].kp = KP_THROTTLE;
        pid_params[PID_THROTTLE].ki = KI_THROTTLE * SampleTimeInSec;
        pid_params[PID_THROTTLE].kd = KD_THROTTLE / SampleTimeInSec;
        pid_params[PID_THROTTLE].min = MIN_THROTTLE;
        pid_params[PID_THROTTLE].max = MAX_THROTTLE;

        pid_params[PID_PITCH].kp = KP_PITCH;
        pid_params[PID_PITCH].ki = KI_PITCH * SampleTimeInSec; 
        pid_params[PID_PITCH].kd = KD_PITCH / SampleTimeInSec;
        pid_params[PID_PITCH].min = MIN_PITCH;
        pid_params[PID_PITCH].max = MAX_PITCH;

        pid_params[PID_ROLL].kp = KP_ROLL;
        pid_params[PID_ROLL].ki = KI_ROLL * SampleTimeInSec;
        pid_params[PID_ROLL].kd = KD_ROLL / SampleTimeInSec;
        pid_params[PID_ROLL].min = MIN_ROLL;
        pid_params[PID_ROLL].max = MAX_ROLL;
        
        pid_params[PID_YAW].kp = KP_YAW;
        pid_params[PID_YAW].ki = KI_YAW * SampleTimeInSec;
        pid_params[PID_YAW].kd = KD_YAW / SampleTimeInSec;
        pid_params[PID_YAW].min = MIN_YAW;
        pid_params[PID_YAW].max = MAX_YAW;

        for i in pid_params:
           print i              
 
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
        
        self.output.appendPlainText("p "+str(pitch[0])+", r "+str(roll[0])+", y "+str(yaw[0]))
        
        ## pids 
        data.pitch = pitch[0]
        data.roll = roll[0]
        data.yaw = yaw[0]
                
        global lastThrottle
                
        pid_output.throttle = self.pid_compute(PID_THROTTLE, lastThrottle, command.throttle);
        pid_output.pitch = self.pid_compute(PID_PITCH, data.pitch, command.pitch);
        pid_output.roll = self.pid_compute(PID_ROLL, data.roll, command.roll);
        pid_output.yaw = self.pid_compute(PID_YAW, data.yaw, command.yaw);
        
        #Save last throttle value
        lastThrottle = pid_output.throttle * 2 * ( MAX_THROTTLE / MOTOR_SPEED_MAX) - MIN_THROTTLE;
    

        #Add offset to throttle
        pid_output.throttle += MOTOR_SPEED_HALF;
        
        self.motors_pid_apply()
                
        ## update gui
        self.repaint()   

    def motor_name(self, motor):
        if (motor == MOTOR_RIGHT_BACK):
            return "RIGHT__BACK"
        elif (motor == MOTOR_LEFT_BACK):
            return "LEFT___BACK"
        elif (motor == MOTOR_RIGHT_FRONT):
            return "RIGHT_FRONT"
        elif (motor == MOTOR_LEFT_FRONT):
            return "LEFT__FRONT"
        else:
            return "UNKNOWN"

    def motors_set_speed(self, motor, value):
        self.output.appendPlainText("motor "+str(self.motor_name(motor))+", value "+str(int(value)))
        

    def motors_pid_apply(self):
      self.motors_set_speed(MOTOR_RIGHT_FRONT, pid_output.throttle - pid_output.pitch - pid_output.roll - pid_output.yaw);
      self.motors_set_speed(MOTOR_LEFT_FRONT, pid_output.throttle - pid_output.pitch + pid_output.roll + pid_output.yaw);
      self.motors_set_speed(MOTOR_RIGHT_BACK, pid_output.throttle + pid_output.pitch - pid_output.roll + pid_output.yaw);
      self.motors_set_speed(MOTOR_LEFT_BACK, pid_output.throttle + pid_output.pitch + pid_output.roll - pid_output.yaw);
                
                
    def pid_compute(self, index, input, setpoint):
     
      pid = pid_params[index]
      retVal = 0

      #Compute all the working error variables*/
      error = setpoint - input;
      pid.iterm += pid.ki * error;
      if (pid.iterm > pid.max) :
        pid.iterm = pid.max;
      else:
        if (pid.iterm < pid.min):
          pid.iterm = pid.min;
          
      dInput = (input - pid.lastInput);

      #Compute PID Output*/
      retVal = (pid.kp * error + pid.iterm - pid.kd * dInput);
      if (retVal > pid.max):
        retVal = pid.max;
      else:
         if (retVal < pid.min):
           retVal = pid.min;

      #Remember some variables for next time*/
      pid.lastInput = input;
      pid_params[index] = pid
      return retVal        
      
    def reset(self):
        self.output.clear()
        
        
if __name__ == '__main__':
    app = QApplication(sys.argv)

    sendBuffer=[]
    receiveBuffer=[]

    chatter=BluetoothWindow(receiveBuffer,sendBuffer)
    chatter.setWindowTitle("QuadCopterRemote")
    chatter.showMaximized()

    sys.exit(app.exec_())
