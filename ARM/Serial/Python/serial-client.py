import time 
import serial
import os
import thread
# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=115200,
    timeout = 1)
#    parity=serial.PARITY_NONE,
#    stopbits=serial.STOPBITS_ONE,
#    bytesize=serial.EIGHTBITS )

#ser.open() 
ser.isOpen()

def log(logPoints):
	global ser
	f = open('log.csv','w')
	f.write('{')
	i = 0
	while i < logPoints:
		# Write "number\n"
#		char = format(ord(ser.read(1)), '#010b')
		char=ord(ser.read(1));
		f.write(str(char) + ',')
		f.flush()
		i=i+1
	f.seek(-1, os.SEEK_END)
	f.truncate()
	f.write('}')
	f.close()


#ser.write("\r")

print "1) start"
print "2) reset"
print "3) cod:<3 digits>:<3 digits>"
print "4) Get and log data to CSV"
print "5) Set motor right"
print "6) Set motor left"
print "7) Test 1 ) PWM switch direction"
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
#			msgToSend = 'start\r'
			msgToSend = '1'
		elif input == '2':
#			msgToSend = 'reset\r'
			msgToSend = '2'
		elif input == '3':
			print 'Enter first coordinate:'
			cod1 = raw_input(">> ")
			print 'Enter second coordinate:'
			cod2 = raw_input(">> ")

			msgToSend = 'cod:' + str(cod1) + ':' + str(cod2) + '\r'
			print 'message cod: ' + msgToSend


		elif input == '7':
			msgToSend = '7'


		elif input == '4':
			doLog = True
			msgToSend = '4\r'

		elif input == '6':
			print 'Enter motor left direction: 00 => off, 11 => STALL, 01 => CCW, 10 => CW'
			M_L_D = int(raw_input(">> "))

			print 'Enter motor left PWM: 0 => 255'
			M_L_PWM = int(raw_input(">> "))

#			outMotor = ((2 << 10) | (M_L_D << 8)) | M_L_PWM
#			print bin(outMotor)

			# get upper 4 bits
			char1 =(2 << 6) | (M_L_D << 4)
			print bin(char1)

			# get 4 lower bits
			char2 = M_L_PWM
#			print char2

			ser.write(chr(char1));
			time.sleep(0.1)
			msgToSend=chr(char2)
		else:
			print 'Number does not exist'
			continue

		ser.write(msgToSend)

		if doLog:
			print 'Logpoints: '
			logPoints = int(raw_input(">> "))
			thread.start_new_thread(log,(logPoints, ))
	
		else:
			# let's wait one second before reading output (let's give the device time to answer)
#			time.sleep(0.2)
			out = ser.read(1)
			print "output: " + out
			if out == '0':
				print 'Command executed'
			else:
				print 'ARM reported an error'

except KeyboardInterrupt:
#	if doLog:
#		f.seek(-1, os.SEEK_END)
#		f.truncate()
#		f.write('}')
#		f.close()
        ser.close()
