thenobuttonaudiobookplayer
===========================

This little Raspberry Pi based project is the base for my motorcycle radio project. I won't need any fancy display to distract me, or any buttons to press that will take my attention off of the car drivers who won't see me.

It basically consists of:  
  
* 1 Raspberry Pi and case
* 1 SD-Card
* 1 USB drive
* a pair of speakers
  
The following software has been used:  
  
* Raspbian minimal image (http://www.linuxsystems.it/2012/06/raspbian-wheezy-armhf-raspberry-pi-minimal-image)
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
