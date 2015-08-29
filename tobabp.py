#!/usr/bin/env python
import os, fnmatch
import pyudev

from mpd import (MPDClient, CommandError)
from socket import error as SocketError
from time import sleep

# Configure MPD connection settings
HOST = 'localhost'
PORT = '6600'
CON_ID = {'host':HOST, 'port':PORT}
USBNAME = '5C04-57E8'

# Some functions
def find_files(directory, pattern):
        for root, dirs, files in os.walk(directory):
                for basename in files:
                        if fnmatch.fnmatch(basename, pattern):
                                filename = os.path.join(root, basename)
                                yield filename

def mpdConnect(client, con_id):
        try:
                client.connect(**con_id)
        except SocketError:
                return True

def loadMusic(client, con_id, device):
        os.system("mount "+device+" /music/usb")
        os.system("/etc/init.d/mpd stop")
        os.system("cp --no-clobber /music/usb/* /music/mp3/")
        os.system("umount /music/usb")
        for filename in find_files('/music/mp3', '*.trash'):
                os.remove(os.path.splitext(filename)[0])
                os.remove(filename)
        os.system("rm /music/mpd/tag_cache")
        os.system("/etc/init.d/mpd start")
        os.system("mpc clear")
        os.system("mpc ls | mpc add")
        os.system("/etc/init.d/mpd restart")

def checkForUSBDevice(name):
        res = ""
        context = pyudev.Context()
        for device in context.list_devices(subsystem='block', DEVTYPE='partition'):
                if device.get('ID_FS_LABEL') == name:
                        res = device.device_node
        return res

def main():
        # MPD object instance
        client = MPDClient()
        mpdConnect(client, CON_ID)

        status = client.status()
        print status

        while True:
                device = checkForUSBDevice(USBNAME)
                if device != "":
                        print "USB Inserted"
                        client.disconnect()
                        loadMusic(client, CON_ID, device)
                        mpdConnect(client, CON_ID)
                        print client.status()
                        while checkForUSBDevice(USBNAME) == device:
                                print "Waiting for USB to sleep"
                                sleep(1.0)
                        if client.status()["state"] == "stop":
                                client.play()
                                print "Playing"
                        else:
                                client.pause()
                                print "Paused"
                else:
                        client.play()
                sleep(0.1)

# Script starts here
if __name__ == "__main__":
        main()
