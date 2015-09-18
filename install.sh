#!/bin/bash
sudo apt-get install moc libudev

sudo mkdir -p /music/{usb,mp3}

cd /usr/local/bin
sudo wget -O tnbmp http://git.io/vZb0O
sudo chmod +x tnbmp

cd /etc/init.d
sudo wget -O mocp http://git.io/vnkNo
sudo chmod +x mocp
sudo chmod 755 mocp
sudo update-rc.d mocp defaults

cd ~
reboot
