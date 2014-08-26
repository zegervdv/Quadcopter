try:
    GYRO_SENSITIVITY_500 = 57.1429
    MAGN_SENSITIVITY_8_1 = [230, 230, 205]
    
    raw=[]
    for i in range(len(self.receivebuf)):
        raw.append(ord(self.receivebuf.pop(0)))
        
    merged = [((raw[2*i] << 8) + raw[2*i+1]) for i in range(9)]
    
    # Fix signedness
    for i in range(5):
        if merged[i] & 0x8000:
            merged[i] -= 0xFFFF

    # FS = 500 dps
    gyroscope = [merged[i] / GYRO_SENSITIVITY_500 for i in range(3)]
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

    #self.output.appendPlainText("Device said:\n"+" ".join(gyroscope)

    # Calculate pitch and roll
    pitch = math.atan2(accelerometer[0], math.sqrt(accelerometer[1]**2 + accelerometer[2]**2))
    roll = math.atan2(accelerometer[1], math.sqrt(accelerometer[0]**2 + accelerometer[2]**2))

    self.pitchbuffer.pop(0)
    self.rollbuffer.pop(0)
    self.pitchbuffer.append(pitch)
    self.rollbuffer.append(roll)
    
    pitch = sum(self.pitchbuffer) / len(self.pitchbuffer)
    roll = sum(self.rollbuffer) / len(self.rollbuffer)
    
    self.ai.setPitch(math.degrees(pitch))
    self.ai.setRoll(math.degrees(roll))

    #print "Pitch: {0}".format(math.degrees(pitch))
    #print "Roll: {0}".format(math.degrees(roll))

    Xh = magnetometer[0] * math.cos(pitch) + magnetometer[2] * math.sin(pitch)
    Yh = magnetometer[0] * math.sin(roll) * math.sin(pitch) + magnetometer[1] * math.cos(roll) - magnetometer[2] * math.sin(roll) * math.cos(pitch)

    heading = math.atan2(Yh, Xh)
    
    if math.degrees(heading) < 0:
        heading += math.pi * 2

    #print "Heading: {0}".format(math.degrees(heading))

    self.compass.setOrientation(math.degrees(heading))
    #self.output.appendPlainText("Device said:\n"+str(pitch)+" "+str(roll)+" "+str(heading))
    self.repaint()            
    
except:
    pass
