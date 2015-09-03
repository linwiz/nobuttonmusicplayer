thenobuttonmusicplayer
===========================

This Repository is a fork of https://github.com/exitnode/theonebuttonaudiobookplayer

This little Raspberry Pi based project is the base for my motorcycle radio project. I won't need any fancy display to distract me, or any buttons to press that will take my attention off of the car drivers who won't see me.

It basically consists of:  
  
* 1 Raspberry Pi and case
* 1 SD-Card
* 1 USB drive with the volume name "5C04-57E8" (can be changed in the py script)
* a pair of speakers
  
The following software has been used:  
  
* Raspbian minimal image (http://www.linuxsystems.it/2012/06/raspbian-wheezy-armhf-raspberry-pi-minimal-image)
* moc (music on console)
* mpd (music player daemon)
* mpc
* mpd-python
* pyudev (for USB access)
* a self-written python script
  
  The features are the following:
 
* always on: When you power on the raspberry, it will boot up and start the python script and start playing music
* remembers position: It will always remember the last played position
* easy music deployment: When you plug in a USB thumb drive with a special name/label, the Raspberry will stop playing, mount the thumb drive, copies the new music, rebuilds the playlist and – after unplugging the thumb drive – starts playing music automatically
* multi format: Since it uses mpd, the player supports  Ogg Vorbis, FLAC, OggFLAC, MP2, MP3, MP4/AAC, MOD, Musepack and wave


Instructions
=======
Install the following packages:
* sudo apt-get install moc mpd mpc python-mpd python-pyudev

Run the following commands: (below assumes using defaults for /etc/mpd.conf)
* sudo mkdir -p /music/usb
* sudo ln -s /var/lib/mpd /music/mpd
* sudo ln -s /var/lib/mpd/music /music/mp3

Copy the tobabp.py script to /home/pi
* cd /home/pi
* wget https://raw.githubusercontent.com/linwiz/thenobuttonmusicplayer/master/tobabp.py

Add the script to crontab:
* sudo crontab -e

Add following line to the crontab:
* @reboot python /home/pi/tobabp.py &

Reboot and test your new music player
* sudo reboot

You may also optinally enable random mode:
* mpc random on

To remove files from the Raspberry Pi:
* Rename the file on the usb drive, 'music1.mp3' will become 'music1.mp3.trash'
