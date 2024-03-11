import finger1 #roller finger
import finger2 #direct-drive finger
import time


def open():
    finger2.sendCommand("T-0.3")

def close():
    finger2.sendCommand("T-0.8")

def twirl():
    finger2.sendCommand("T0.45")

def rollTip(angle):
    finger1.sendCommand("D"+angle)

def rollerH():
    finger1.sendCommand("S25")

def rollerV():
    finger1.sendCommand("S120")


if __name__ == '__main__':

    finger1.setupSerial(115200, 'COM3')
    finger2.setupSerial(115200, 'COM18')

    while True:

        cmd = input("0: open\n1: close\n2: twirl\n3: rollTip\n4: rollerH\n5: rollerV\n")

        if cmd == "0":
            open()

        elif cmd == "1":
            close()

        elif cmd == "2":
            twirl()        

        elif cmd == "3":
            angle = input("angle: ")
            rollTip(angle)

        elif cmd == "4":
            rollerH()
            print("You entered" + cmd)

        elif cmd == "5":
            rollerV()

        else:
            print("Invalid command")    

        time.sleep(0.5)
