#!/bin/bash
apt-get install moc python-pyudev

mkdir -p /music/{usb,mp3}

cd /usr/local/bin
wget http://git.io/vZINO
wget http://git.io/vZINC
wget http://git.io/vZINg
wget http://git.io/vZINr
chmod +x mocp-* tobabp.py

crontab -l > mycron
echo "@reboot python /usr/local/bin/tobabp.py &" >> mycron
crontab mycron
rm mycron

reboot
