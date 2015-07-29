from mpsse import *

flash = MPSSE(SPI0, TEN_MHZ, MSB)
flash.start()
flash.Write("\x03\x00\x00\x00")
data = flash.Read(256)
flash.Stop()
flash.Close()
