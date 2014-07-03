import serial
import re
import math

GYRO_SENSITIVITY_500 = 57.1429
MAGN_SENSITIVITY_8_1 = [230, 230, 205]

while True:
    try:
        s = serial.Serial("/dev/tty.Quadcopter-1-RNI-SPP", 115200)
        break
    except OSError:
        print "Retrying"

data = s.read(40)

# Find full string of data bytes
raw = [ord(val) for val in (re.search('UU.{18}', data).group(0))]
print ":".join("{:02X}".format(c) for c in raw)
merged = [((raw[2*i] << 8) + raw[2*i+1]) for i in range(1,10)]

# Fix signedness
for i in range(1,6):
    if merged[i] & 0x8000:
        merged[i] -= 0xFFFF

# FS = 500 dps
gyroscope = [merged[i] / GYRO_SENSITIVITY_500 for i in range(0,3)]
# FS = 8100 mGauss
magnetometer = [merged[i] * 1000 / MAGN_SENSITIVITY_8_1[i-3] for i in range(3,6)]
# FS = 2000 mg
accelerometer = [merged[i] >> 4 for i in range(6,9)]
for i in xrange(len(accelerometer)):
    if accelerometer[i] & 0x800:
        accelerometer[i] -= 0xFFF

# Rotate axes
magnetometer = [-mag for mag in magnetometer]
accelerometer = [-acc for acc in accelerometer]

print gyroscope
print magnetometer
print accelerometer

# Calculate pitch and roll
pitch = math.atan2(accelerometer[0], math.sqrt(accelerometer[1]**2 + accelerometer[2]**2))
roll = math.atan2(accelerometer[1], math.sqrt(accelerometer[0]**2 + accelerometer[2]**2))

print "Pitch: {0}".format(math.degrees(pitch))
print "Roll: {0}".format(math.degrees(roll))

Xh = magnetometer[0] * math.cos(pitch) + magnetometer[2] * math.sin(pitch)
Yh = magnetometer[0] * math.sin(roll) * math.sin(pitch) + magnetometer[1] * math.cos(roll) - magnetometer[2] * math.sin(roll) * math.cos(pitch)

heading = math.atan2(Yh, Xh)

if math.degrees(heading) < 0:
    heading += math.pi * 2

print "Heading: {0}".format(math.degrees(heading))

s.close()

