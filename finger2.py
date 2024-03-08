import time
import serial

startMarker = ''
endMarker = '\n'
dataStarted = False
dataBuf = ""
messageComplete = False



def setupSerial(baudRate, serialPortName):
    
    global  serialPort
    
    serialPort = serial.Serial(port= serialPortName, baudrate = baudRate, timeout=1)

    print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))

    time.sleep(3)


def sendToArduino(stringToSend):
    
        # this adds the start- and end-markers before sending
    global startMarker, endMarker, serialPort
    
    stringWithMarkers = (startMarker)
    stringWithMarkers += stringToSend
    stringWithMarkers += (endMarker)

    serialPort.write(stringWithMarkers.encode()) # encode needed for Python3



def recvLikeArduino():

    global startMarker, endMarker, serialPort, dataStarted, dataBuf, messageComplete

    if serialPort.inWaiting() > 0 and messageComplete == False:
        x = serialPort.read().decode("utf-8") # decode needed for Python3
        
        if dataStarted == True:
            if x != endMarker:
                dataBuf = dataBuf + x
            else:
                dataStarted = False
                messageComplete = True
        elif x == startMarker:
            dataBuf = ''
            dataStarted = True
    
    if (messageComplete == True):
        messageComplete = False
        return dataBuf
    else:
        return "XXX" 



if __name__ == '__main__':

    setupSerial(115200, 'COM18')

    while True:

        cmd = input("User command: ")

        sendToArduino(cmd)

        time.sleep(0.5)

