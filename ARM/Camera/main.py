## Color Tracking v1.0
## Copyright (c) 2013-2014 Abid K and Jay Edry
## You may use, redistribute and/or modify this program it under the terms of the MIT license (https://github.com/abidrahmank/MyRoughWork/blob/master/license.txt).

''' file name : houghcircles.py

Description : This sample shows how to detect circles in image with Hough Transform

This is Python version of this tutorial : http://opencv.itseez.com/doc/tutorials/imgproc/imgtrans/hough_circle/hough_circle.html

Level : Beginner

Benefits : Learn to find circles in the image and draw them

Usage : python houghcircles.py 

Written by : Abid K. (abidrahman2@gmail.com) , Visit opencvpython.blogspot.com for more tutorials '''
import cv2
import numpy as np
import cv2.cv as cv
import serial

try:
    ser = serial.Serial('/dev/ttyUSB0',3000000)
except(serial.serialutil.SerialException):
    print "Unable to find serial-port"
    exit(0)
def sendPixel(pan, tilt):
    global ser
    ser.write('p')
    ser.write('p')
    
    pan = str(pan)
    pan  = pan.zfill(4)
    
    tt = pan[0]
    tu = pan[1]
    hu = pan[2]
    ti = pan[3]
    #print tt +  ',' + tu + ',' +hu +',' +ti 
   
    ser.write(tt)
    ser.write(tu)
    ser.write(hu)
    ser.write(ti)
    #       print ("x: %d: %c%c%c%c" % (x, tt, tu, hu, ti))
    
    ser.write('p')
    ser.write('t')
    
    tilt = str(tilt)
    tilt = tilt.zfill(4)
    
    tt = tilt[0]
    tu = tilt[1]
    hu = tilt[2]
    ti = tilt[3]
    
    ser.write(tt)
    ser.write(tu)
    
    ser.write(hu)
    ser.write(ti)

c = cv2.VideoCapture(1)
#while True:
#    ref, img = cam.read()
#    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
#    cv2.imshow('ok',gray)
delayCounter = 0
while True:
    _,f = c.read()
    img = cv2.flip(f,1)
    width  = c.get(cv2.cv.CV_CAP_PROP_FRAME_WIDTH)
    heigth = c.get(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT)
    
    #print "width: " + str(width) + " heigth: " + str(heigth)
    #cv2.cv.CV_CAP_PROP_
    wantedWidth = 1920
    wantedHeigth = 1080
    
    #img = cv2.resize(img, (0,0), fx=wantedWidth/width, fy=wantedHeigth/heigth) 
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    cimg = cv2.cvtColor(gray, cv2.COLOR_GRAY2BGR)

    gray = cv2.GaussianBlur(gray, (0,0), 2)
    '''
    10 = distnce between circle
    5 = min radius
    33 = max radius
    '''
    circles = cv2.HoughCircles(gray,cv.CV_HOUGH_GRADIENT, 2, 10, np.array([]), 40, 80, 5, 33)
    if delayCounter == 0:
        delayCounter = 1
        #print "run"
        if circles is not None:
            circles = np.uint16(np.around(circles))
            i = circles[0][:][0]
            cv2.circle(cimg,(i[0],i[1]),i[2],(0,255,0),2)
            # draw the center of the circle
            cv2.circle(cimg,(i[0],i[1]),2,(0,0,255),3)
            sendPixel(640-i[0], i[1])
            #print "x: " + str(i[0]) + " y: " + str(i[1])
    cv2.imshow('detected circles',cimg) 
    delayCounter-=1
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
#Window(winName)
#       break

print "Goodbye"


''' v 0.1 - It tracks two objects of blue and yellow color each '''
'''
import cv2
import numpy as np
import serial

def getthresholdedimg(hsv):
    #h:53-90 s:74-147 v: 160-255)
    yellow = cv2.inRange(hsv,np.array((53,74,100)),np.array((90,147,255)))
    #blue = cv2.inRange(hsv,np.array((100,100,50)),np.array((120,255,255)))
#    both = cv2.add(yellow,blue)
    return yellow

try:
    ser = serial.Serial('/dev/ttyUSB0',115200)
except(serial.serialutil.SerialException):
    print "Unable to find serial-port"
    exit(0)
c = cv2.VideoCapture(0)
width,height = c.get(3),c.get(4)
print "miniframe width and height : ", width, height
imgcounter = 0 
while(1):
    _,f = c.read()
    f = cv2.flip(f,1)
    blur = cv2.medianBlur(f,5)
    hsv = cv2.cvtColor(f,cv2.COLOR_BGR2HSV)
    both = getthresholdedimg(hsv)
    erode = cv2.erode(both,None,iterations = 3)
    dilate = cv2.dilate(erode,None,iterations = 10)

    contours,hierarchy = cv2.findContours(dilate,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)

    #for cnt in contours:
    if imgcounter > 15:
        imgcounter = 0
        if len(contours) > 0: 
            x,y,w,h = cv2.boundingRect(contours[0])
            cx,cy = x+w/2, y+h/2
        
            #if 20 < hsv.item(cy,cx,0) < 30:
            cv2.rectangle(f,(x,y),(x+w,y+h),[0,255,255],2)
            #print "box :", x,y,w,h
            #x = x+(w/2)
            #y = y+(h/2)
            #working : 
                #FixedToCamWidth = x*1.68
                #FixedToCamHeigth = y*2.2
            
            FixedToCamHeigth=y/2
            
            FixedToCamWidth=x/1.5
            if FixedToCamWidth < 219:
                FixedToCamWidth+=860
            else:
                FixedToCamWidth-=219
            
            ser.write('f')
            ser.write('p')
    
            tt = (FixedToCamWidth/1000)+48
            tu = (FixedToCamWidth%1000)/100+48
            hu = ((FixedToCamWidth%100)/10)+48
            ti = (FixedToCamWidth%10)+48
    
            ser.write(chr(int(tt)))
            ser.write(chr(int(tu)))
            ser.write(chr(int(hu)))
            ser.write(chr(int(ti)))
            #       print ("x: %d: %c%c%c%c" % (x, tt, tu, hu, ti))
    
    
            ser.write('f')
            ser.write('t')
    
            tt = (FixedToCamHeigth/1000)+48
            tu = (FixedToCamHeigth%1000)/100+48
            hu = ((FixedToCamHeigth%100)/10)+48
            ti = (FixedToCamHeigth%10)+48
    
            ser.write(chr(int(tt)))
    
            ser.write(chr(int(tu)))
    
            ser.write(chr(int(hu)))
            ser.write(chr(int(ti)))
            
            print "pan: " + str(FixedToCamWidth) + " tilt: " + str(FixedToCamHeigth)


    cv2.imshow('img',f)
    imgcounter +=1


    if cv2.waitKey(25) == 27:
        break

cv2.destroyAllWindows()
c.release()
'''