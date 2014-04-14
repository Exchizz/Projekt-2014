import time 
import serial
import os
# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600)
#    parity=serial.PARITY_NONE,
#    stopbits=serial.STOPBITS_ONE,
#    bytesize=serial.EIGHTBITS )

#ser.open() 
ser.isOpen()

print "1) start"
print "2) reset"
print "3) cod:<3 digits>:<3 digits>"
print "4) Get and log data to CSV"
print "q) exits"
input=1
msgToSend = 0 
doLog = 0
try:
	while 1 :
		# get keyboard input
		input = raw_input(">> ")

		if input == 'q':
			ser.close()
			exit()
		elif input == '1':
			msgToSend = 'start\r'
		elif input == '2':
			msgToSend = 'reset\r'
		elif input == '3':
			print 'Enter first coordinate:'
			cod1 = raw_input(">> ")
			print 'Enter second coordinate:'
			cod2 = raw_input(">> ")

			msgToSend = 'cod:' + str(cod1) + ':' + str(cod2) + '\r'
			print 'message cod: ' + msgToSend

		elif input == '4':
			msgToSend = '4\r'
			doLog = True
		else:
			print 'Number does not exist'
			continue

		ser.write(msgToSend)

		if doLog:
			f = open('log.csv','w')
			f.write('{')
			ser.flushInput()
			while True:
				# Write "number\n"
				line=ser.readline();
				f.write(line.strip() + ',')
				f.flush()
	
		else:
			# let's wait one second before reading output (let's give the device time to answer)
			time.sleep(0.1)
			out = ser.read(1)
			print "output: " + out
			if out == '1':
				print 'Command executed'
			else:
				print 'ARM reported an error'

except KeyboardInterrupt:
	if doLog:
		f.seek(-1, os.SEEK_END)
		f.truncate()
		f.write('}')
		f.close()
        ser.close()
