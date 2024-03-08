import time
import serial

startMarker = ''
endMarker = '\n'
dataStarted = False
dataBuf = ""
messageComplete = False



def setupSerial(baudRate, serialPortName):
    
    global  serialPort
    
    serialPort = serial.Serial(port= serialPortName, baudrate = baudRate, timeout=0, rtscts=True)

    print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))

    time.sleep(3)


def sendCommand(stringToSend):
    
    # this adds the start- and end-markers before sending
    global startMarker, endMarker, serialPort
    
    stringWithMarkers = (startMarker)
    stringWithMarkers += stringToSend
    stringWithMarkers += (endMarker)

    serialPort.write(stringWithMarkers.encode('utf-8')) # encode needed for Python3


if __name__ == '__main__':

    setupSerial(115200, "COM3")

    while True:

        cmd = input("User command: ")

        sendCommand(cmd)

        time.sleep(0.5)

