import time 
import serial
import os
import thread
# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=115200,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE
    )

ser.isOpen()

f = open('log.csv','w')


while True:
	tens =  ord(ser.read(1))*256
#	print "recv byte: " + str(tens)
	ones = ord(ser.read(1))
	ticks = tens + ones

	pwm = ord(ser.read(1))


	print " ticks: " + str(ticks) + " pwm: " + str(pwm)

	f.write(str(ticks) + "," + str(pwm) + ",\n" )
	f.flush()
