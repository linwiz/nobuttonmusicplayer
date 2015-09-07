#!/bin/bash
apt-get install moc python-pyudev

mkdir -p /music/{usb,mp3}

cd /usr/local/bin
GITURL='https://raw.githubusercontent.com/linwiz/thenobuttonmusicplayer/master/usr/local/bin/'
wget "$GITURL"tobabp.py
wget "$GITURL"mocp-start.sh
wget "$GITURL"mocp-stop.sh
wget "$GITURL"mocp-play.sh
chmod +x mocp-* tobabp.py

crontab -l > mycron
echo "@reboot python /usr/local/bin/tobabp.py &" >> mycron
crontab mycron
rm mycron

reboot
