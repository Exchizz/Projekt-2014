#!/usr/bin/env python
'''
Created on 08/05/2014

@author: exchizz
'''
#from __future__ import print_function
import serial
import numpy as np
import matplotlib.pyplot as plt
from pylab import plot, subplot, axis, grid, title, xlabel, ylabel, ylim, draw, ion
import time

# configure the serial connections (the parameters differs on the device you are connecting to)
positionPunkter = ["0900","0100","0600","0500"]

ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=3000000)

y = [0]*500

ion()
subplot (111)
plt_p, = plot(y, 'red')
quit
ylim([-1200, 1200])
ylabel('Error')
grid (True)
title ("plotting dat' speed")
draw() # redraw plot

positionHold = 0
positionCounterVar = 199
f = open('log.txt','w')
'''
plot position = 1
plot speed = 2
'''

state = 2


while True:
    if ser.read(1) == '|':

		if state == 1:
			positionCounterVar +=1
			if positionCounterVar == 200:
				positionCounterVar = 0
		
	                	ser.write('f')
	                	ser.write('p')

				ser.write(positionPunkter[positionHold][0])
				ser.write(positionPunkter[positionHold][1])
				ser.write(positionPunkter[positionHold][2])
				ser.write(positionPunkter[positionHold][3])
				print "new position tilt: " + str(positionPunkter[positionHold])

				#time.sleep(1)
	                	ser.write('f')
	                	ser.write('t')

				ser.write(positionPunkter[positionHold][0])
				ser.write(positionPunkter[positionHold][1])
				ser.write(positionPunkter[positionHold][2])
				ser.write(positionPunkter[positionHold][3])
				print "new position pan: " + str(positionPunkter[positionHold])

				positionHold+=1
				positionHold %= len(positionPunkter)
		hundrede=ord(ser.read(1));
		tiere=ord(ser.read(1));
		y2 = hundrede*256+tiere-32768
		f.write(str(y2) + "\n")
		f.flush()
		y.append (y2)
		del y[0]
		plt_p.set_ydata(y)           # update the plot data
		draw()                      # redraw the canvas
