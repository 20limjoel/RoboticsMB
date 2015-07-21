#
# Example python program to run on the computer to process commands from
# the Serial Port.  Use in conjunction with the ComputerController Sketch
# to control the computer with an IR remote.
#
# Make sure you set the serial port to match your Arduino configuration.
#
import serial
import os
from pymouse import PyMouse
from pykeyboard import PyKeyboard

ser=serial.Serial('/dev/ttyUSB0',19200,timeout=999)
key=PyKeyboard()

while (1 == 1):
    line=ser.readline()
    tokens=line.split()
    print(tokens)
    if (tokens[0] == 'Sense'):
        intro()
    elif (tokens[0] == "NoSense"):
        speak("Goodbye.  Thanks for visiting.")
    elif (tokens[0] == "Cmd"):
        if (tokens[1] == "Up"):
            key.tap_key(key.up_key);
        elif (tokens[1] == "Down"):
            key.tap_key(key.down_key);
        elif (tokens[1] == "Left"):
            key.tap_key(key.left_key)
        elif (tokens[1] == "Right"):
            key.tap_key(key.right_key)
    elif (tokens[0] == "ScrollDown"):
        for i in range(0,5):
            key.tap_key(key.down_key)
    elif (tokens[0] == "IRCode"):
        print(tokens[1]);

# clear the serial buffer   
    ser.flushInput()
