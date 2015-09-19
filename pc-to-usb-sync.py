#!/usr/bin/env python
#  Original Copyright (C) 2012 Michael Clemens
#  nobuttonmusicplayer Copyright (C) 2015 Matthew Motz
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
import os, os.path, fnmatch, pyudev
from time import sleep
USBNAME = 'KYLEMUSIC'

def find_files(directory, pattern):
        for root, dirs, files in os.walk(directory):
                for basename in files:
                        if fnmatch.fnmatch(basename, pattern):
                                filename = os.path.join(root, basename)
                                yield filename

def loadMusic(device):
        os.system('notify-send "Music Sync" "Syncing music to Kyle\'s USB drive."')
        os.system("sudo mount "+device+" /music/usb")
        os.system("sudo rm /music/usb/*.*")
        os.system("sudo cp /run/media/pi/Seagate\ Freeagent\ Go/Documents/Music/kyle/*.* /music/usb")
        os.system("sudo umount /music/usb")
        os.system('notify-send "Music Sync" "Its is safe to remove Kyle\'s USB drive."')

def checkForUSBDevice(name):
        res = ""
        context = pyudev.Context()
        for device in context.list_devices(subsystem='block', DEVTYPE='partition'):
                if device.get('ID_FS_LABEL') == name:
                        res = device.device_node
        return res

def main():
        while True:
                device = checkForUSBDevice(USBNAME)
                if device != "":
                        loadMusic(device)
                        while checkForUSBDevice(USBNAME) == device:
                                sleep(1.0)
                sleep(0.1)

if __name__ == "__main__":
        main()
