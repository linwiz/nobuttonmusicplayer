#!/bin/bash
sudo apt-get install moc libudev

sudomkdir -p /music/{usb,mp3}

cd /usr/local/bin
sudo wget -O tnbmp http://git.io/vZb0O
sudo chmod +x tnbmp

cd ~
crontab -l > mycron
echo "@reboot /usr/local/bin/tnbmp &" >> mycron
crontab mycron
rm mycron

reboot
