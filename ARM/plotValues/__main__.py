#!/usr/bin/env python
'''
Created on 08/05/2014

@author: exchizz
'''
import serial
import numpy as np
import matplotlib.pyplot as plt
from pylab import plot, subplot, axis, grid, title, xlabel, ylabel, ylim, draw, ion
# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=115200)

y = [0]*500

ion()
subplot (111)
plt_p, = plot(y, 'red')
quit
ylim([0, 1000])
ylabel('Error')
grid (True)
title ("plotting dat' speed")
draw() # redraw plot

while True:
    if ser.read(1) == '|':
	    hundrede=ord(ser.read(1));
	    tiere=ord(ser.read(1));
	    y2 = hundrede*256+tiere
	    y.append (y2)
	    del y[0]
	    plt_p.set_ydata(y)           # update the plot data
	    draw()                      # redraw the canvas
