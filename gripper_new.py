import finger1 #roller finger
import finger2 #direct-drive finger
import time

import msvcrt

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

def reorientTip(angle):
    finger1.sendCommand("S"+angle)

if __name__ == '__main__':

    finger1.setupSerial(115200, 'COM3')
    finger2.setupSerial(115200, 'COM18')

    roller_angle = 0
    servo_angle = 25 #initial servo angle

    while True:

        # cmd = input("0: open\n1: close\n2: twirl\n3: rollTip\n4: rollerH\n5: rollerV\n")
        print("0: open\n1: close\n2: twirl\n3: rollerH\n5: rollerV\n")

        cmd = msvcrt.getch()

        if cmd.upper() == b"0":
            open()

        elif cmd.upper() == b"1":
            close()

        elif cmd.upper() == b"2":
            twirl()        

        elif cmd.upper() == b"3":
            rollerH()

        elif cmd.upper() == b"4":
            rollerV()
            

        elif cmd == b'\xe0':  # Arrow key prefix
            arrow_key = msvcrt.getch()
            if arrow_key == b'H':
                roller_angle = roller_angle + 20
                rollTip(str(roller_angle))

            elif arrow_key == b'P':
                roller_angle = roller_angle - 20
                rollTip(str(roller_angle))

            elif arrow_key == b'M':
                servo_angle = servo_angle + 5
                reorientTip(str(servo_angle))

            elif arrow_key == b'K':
                servo_angle = servo_angle - 5
                reorientTip(str(servo_angle))
            else:
                print("Invalid command")  

        else:
            print("Invalid command")    

        time.sleep(0.1)
