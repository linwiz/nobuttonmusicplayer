nobuttonmusicplayer
=======

This little Raspberry Pi based project is the base for my motorcycle radio project. I won't need any fancy display to distract me, or any buttons to press that will take my attention off of the car drivers who won't see me.

It basically consists of:  
  
* 1 Raspberry Pi and case
* 1 SD-Card
* 1 USB drive with one vfat partition
* a pair of speakers
  
The following software has been used:  
  
* Raspbian Distro
* moc (music on console)
* libudev (for USB access)
* a self-written C program
  
Features:
====== 
* Always on: When you power on the raspberry, it will boot up and start the python script and start playing music
* Easy music deployment: When you plug in a USB thumb drive with a special name/label, the Raspberry will stop playing, removes all old music, mount the thumb drive, copies the new music, rebuilds the playlist and – after unplugging the thumb drive – starts playing music automatically


Instructions
=======
Via installation script:
* wget -O install.sh http://git.io/vZbEu
* chmod +x install.sh
* ./install.sh

Manually:
Install the following packages:
* sudo apt-get install moc libudev

Run the following commands:
* sudo mkdir -p /music/usb
* sudo mkdir -p /music/mp3

Copy the script to /usr/local/bin
* cd /usr/local/bin
* sudo wget -O tnbmp http://git.io/vZb0O
* sudo chmod +x tnbmp

Install the init.d script
* cd /etc/init.d
* sudo wget -O mocp http://git.io/vnkNo
* sudo chmod +x mocp
* sudo chmod 755 mocp
* sudo update-rc.d mocp defaults

Reboot and test your new music player
* sudo reboot
