#
from threading import Thread
import serial
import math
import time
import sys
#

ser = serial.Serial('/dev/ttyUSB0',3000000) 
def readSerial():
	global ser
	while True:
		byte = ser.read(1)
#		print byte
		sys.stdout.write(byte)
		sys.stdout.flush()

if __name__ == "__main__":
	thread = Thread(target = readSerial)
	thread.start()
        i = 0
        while True:
		i = i % 31.4
		try:
	                x = int(round(50*math.cos(i)))
	                y = int(round(50*math.sin(i)))

			if y < 0:
				y = y+1080

			if x < 0:
				x = x+1080

	                ser.write('f')
	                ser.write('t')

	                tt = (x/1000)+48
	                tu = (x%1000)/100+48
	                hu = ((x%100)/10)+48
	                ti = (x%10)+48

	                ser.write(chr(tt))
	                ser.write(chr(tu))
	                ser.write(chr(hu))
	                ser.write(chr(ti))
#	                print ("x: %d: %c%c%c%c" % (x, tt, tu, hu, ti))




	                ser.write('f')
	                ser.write('p')

	                tt = (y/1000)+48
	                tu = (y%1000)/100+48
	                hu = ((y%100)/10)+48
	                ti = (y%10)+48

	                ser.write(chr(tt))

	                ser.write(chr(tu))

	                ser.write(chr(hu))
	                ser.write(chr(ti))

#	                print ("y: %d: %c%c%c%c" % (y, tt, tu, hu, ti))
	                time.sleep(0.02)
	                i +=0.006
		except KeyboardInterrupt:
			print "exitting"
			sys.exit()
