#!/usr/bin/env python
#
# Example python program to run on the computer to process commands from
# the Serial Port.  Use in conjunction with the ComputerController Sketch
# to control the computer with an IR remote.
#
# Make sure you set the serial port to match your Arduino configuration.
#
import serial
import os
import sys, getopt

from pymouse import PyMouse
from pykeyboard import PyKeyboard

serialPort='/dev/ttyUSB0'
#
# Python way of defining functions
#
def processCmds(tokens):
    "This processes commands from the serial port"
    if (tokens[0] == "Cmd"):
        if (tokens[1] == "Up"):
            key.tap_key(key.up_key);
        elif (tokens[1] == "Down"):
            key.tap_key(key.down_key);
        elif (tokens[1] == "Left"):
            key.tap_key(key.left_key)
        elif (tokens[1] == "Right"):
            key.tap_key(key.right_key)
        else:
            print "unprocessed command:  %s " % tokens[1]
    elif (tokens[0] == "IRCode"):
        # process IR Codes directly
        print "IR Code: %d" % tokens[1]
    else:
        print "Unrecognized input:  %s" % tokens


def usage():
    global serialPort
    print 'Controller.py -s {serial port}'
    print
    print 'Receive commands via the serial port'
    print
    print 'Options:'
    print '   -s {serial port} (default %s)' % serialPort
    print
    sys.exit(2)
    
def main(argv):
    global serialPort
    try:
        opts, args=getopt.getopt(argv, "h:s:", ["serial="])
    except getopt.GetoptError:
        usage()
    for opt, arg in opts:
        if opt == '-h':
            usage
        elif opt in ("-s", "--serial"):
            serialPort=arg

    try:
        ser=serial.Serial('/dev/ttyUSB0',19200,timeout=999)
        key=PyKeyboard()
        while (1 == 1):
            line=ser.readline()
            tokens=line.split()
            print(tokens)
            if (len(tokens) == 2):
                processCmds(tokens)
                
                # clear the serial buffer   
                ser.flushInput()
    except (OSError, serial.SerialException):
        print "Error opening serial port %s" % serialPort

if __name__ == "__main__":
    main(sys.argv[1:])
