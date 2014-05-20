import serial

# configure the serial connections (the parameters differs on the device you are connecting to)

ser = serial.Serial(

    port='COM27',

    baudrate=115200,

    stopbits=serial.STOPBITS_ONE,

    bytesize=serial.EIGHTBITS,

    parity=serial.PARITY_NONE

    )
ser.isOpen()
f = open('log.csv','w')

while True:
        thousands = ord(ser.read(1))*16777216
        hundreds = ord(ser.read(1))*65536
        tens =  ord(ser.read(1))*256
#       print "recv byte: " + str(tens)
        ones = ord(ser.read(1))
        ticks = thousands +  hundreds + tens + ones
        ticks = ticks - 2147483647
        pwm = ord(ser.read(1)) - 50

        print " ticks: " + str(ticks) + " pwm: " + str(pwm)

        f.write(str(ticks) + "," + str(pwm) + ",\n" )
        f.flush()


