thenobuttonmusicplayer
=======

This Repository is a fork of https://github.com/exitnode/theonebuttonaudiobookplayer

This little Raspberry Pi based project is the base for my motorcycle radio project. I won't need any fancy display to distract me, or any buttons to press that will take my attention off of the car drivers who won't see me.

It basically consists of:  
  
* 1 Raspberry Pi and case
* 1 SD-Card
* 1 USB drive with the volume name "16GB" (can be changed in the py script)
* a pair of speakers
  
The following software has been used:  
  
* Raspbian Distro
* moc (music on console)
* pyudev (for USB access)
* a self-written python script
  
Features:
====== 
* Always on: When you power on the raspberry, it will boot up and start the python script and start playing music
* Easy music deployment: When you plug in a USB thumb drive with a special name/label, the Raspberry will stop playing, mount the thumb drive, copies the new music, rebuilds the playlist and – after unplugging the thumb drive – starts playing music automatically
* To remove music from the Raspberry Pi simply rename the file on the USB drive, 'music1.mp3' should become 'music1.mp3.trash'. The next time you plug the USB drive in to the Raspberry Pi the files music1.mp3 and music1.mp3.trash will be deleted.


Instructions
=======
Via installation script:
* wget https://raw.githubusercontent.com/linwiz/thenobuttonmusicplayer/master/install.sh
* chmod +x install.sh
* sudo ./install.sh

Manually:
Install the following packages:
* sudo apt-get install moc python-pyudev

Run the following commands:
* sudo mkdir -p /music/usb
* sudo mkdir -p /music/mp3

Copy the scripts to /usr/local/bin
* cd /usr/local/bin
* sudo wget https://raw.githubusercontent.com/linwiz/thenobuttonmusicplayer/master/usr/local/bin/tobabp.py
* sudo wget https://raw.githubusercontent.com/linwiz/thenobuttonmusicplayer/master/usr/local/bin/mocp-start.sh
* sudo wget https://raw.githubusercontent.com/linwiz/thenobuttonmusicplayer/master/usr/local/bin/mocp-stop.sh
* sudo wget https://raw.githubusercontent.com/linwiz/thenobuttonmusicplayer/master/usr/local/bin/mocp-play.sh
* sudo chmod +x mocp-* tobabp.py

 
Add the script to crontab:
* sudo crontab -e

Add following line to the crontab:
* @reboot python /usr/local/bin/tobabp.py &

Reboot and test your new music player
* sudo reboot
