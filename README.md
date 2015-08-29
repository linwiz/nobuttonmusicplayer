WORK IN PROGRESS
thenobuttonaudiobookplayer
===========================

This little Raspberry Pi based project is a gift for my wife’s grandmother for her 90th birthday. Being visually impaired, she is hard to entertain but loves to listen to audiobooks. The problem is, that she isn’t able to handle a ghetto blaster or MP3 player.  
  
The solution to this problem was – tadaaaah – a no button audiobook player  
  
It basically consists of:  
  
* 1 Raspberry Pi
* 1 ModMyPi enclosure
* 1 (slow) 8GB SD-Card
* a pair of speakers
  
The following software has been used:  
  
* Raspbian minimal image (http://www.linuxsystems.it/2012/06/raspbian-wheezy-armhf-raspberry-pi-minimal-image)
* mpd (music player daemon)
* mpc
* mpd-python
* pyudev (for USB access)
* a self-written python script
  
  The features are the following:
 
* always on: When you power on the raspberry, it will boot up and start the python script with the audio book in autoplay
* remembers position: It will always remember the last played position
* easy audio book deployment: When you plug in a USB thumb drive with a special name/label, the Raspberry will stop playing, mount the thumb drive, deletes the old audio book, copies the new one, rebuilds the playlist and – after unplugging the thumb drive – starts the new audiobook in pause mode
* multi format: Since it uses mpd, the player supports  Ogg Vorbis, FLAC, OggFLAC, MP2, MP3, MP4/AAC, MOD, Musepack and wave
  
If you like to build your own one button audio book player, here are the super simple schematics:
http://blogs.fsfe.org/clemens/files/2012/10/obabp_schematics.jpg  
  
You can read more about it on http://blogs.fsfe.org/clemens/2012/10/30/the-one-button-audiobook-player/  
