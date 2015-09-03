#!/usr/bin/env python
import os, fnmatch
import os.path
import pyudev
from time import sleep
USBNAME = '5C04-57E8'

# Some functions
def find_files(directory, pattern):
        for root, dirs, files in os.walk(directory):
                for basename in files:
                        if fnmatch.fnmatch(basename, pattern):
                                filename = os.path.join(root, basename)
                                yield filename

def loadMusic(device):
        os.system("mount "+device+" /music/usb")
        os.system("cp --no-clobber /music/usb/* /music/mp3/")
        for filename in find_files('/music/mp3', '*.trash'):
                if os.path.isfile(os.path.splitext(filename)[0]):
                        os.remove(os.path.splitext(filename)[0])
                os.remove(filename)
        for filename in find_files('/music/usb', '*.trash'):
                os.remove(filename)
        os.system("umount /music/usb")

def checkForUSBDevice(name):
        res = ""
        context = pyudev.Context()
        for device in context.list_devices(subsystem='block', DEVTYPE='partition'):
                if device.get('ID_FS_LABEL') == name:
                        res = device.device_node
        return res

def main():
        os.system("/bin/su -c /usr/local/bin/start-mocp.sh pi")
        os.system("/bin/su -c /usr/local/bin/music.sh pi")
        while True:
                device = checkForUSBDevice(USBNAME)
                if device != "":
                        os.system("/bin/su -c /usr/local/bin/stop-mocp.sh pi")
                        print "USB Inserted"
                        loadMusic(device)
                        while checkForUSBDevice(USBNAME) == device:
                                print "Waiting for USB removal"
                                sleep(1.0)
                        os.system("/bin/su -c /usr/local/bin/start-mocp.sh pi")
                        os.system("/bin/su -c /usr/local/bin/music.sh pi")
                sleep(0.1)

# Script starts here
if __name__ == "__main__":
        main()
