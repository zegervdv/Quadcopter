import serial
import re
import time

GYRO_SENSITIVITY_500 = 57.1429
MAGN_SENSITIVITY_8_1 = [230, 230, 205]

s = serial.Serial("/dev/tty.Quadcopter-1-RNI-SPP", 115200)

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

print gyroscope
print magnetometer
print accelerometer


s.close()
