import finger1 #roller finger
import finger2 #direct-drive finger
import time


if __name__ == '__main__':

    finger1.setupSerial(115200, 'COM3')
    finger2.setupSerial(115200, 'COM18')

    while True:

        cmd = input("User command: ")

        if cmd[0] == 'S' or cmd[0] == 'D':
            finger1.sendCommand(cmd)

        elif cmd[0] == 'T':
            finger2.sendCommand(cmd)

        else:
            print("Invalid command")

        time.sleep(0.5)
