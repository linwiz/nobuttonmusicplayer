#!/bin/bash
/bin/ls -d /music/mp3/*.* > /home/pi/.moc/playlist.m3u && /usr/bin/mocp -o s,n -p
