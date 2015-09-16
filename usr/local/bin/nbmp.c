#include <stdio.h>
#include <errno.h>

//http://www.linuxquestions.org/questions/programming-9/how-to-use-mount-function-from-c-920210/
int main() {
	if(geteuid()!=0) {
		printf("Please run nbmp as a superuser.\n");
		return 1;
	}
	system("su - pi -c \"/usr/bin/mocp -S\"");
	const char* media_src = "/music/usb/";
	const char* media_trgt = "/music/mp3/";
	const char* mount_src  = "/dev/sda1";
	const char* mount_trgt = "/music/usb";
	const char* mount_type = "vfat";
	const unsigned long mount_flags = 0;
	const char* opts = "mode=0700,uid=65534";
	if (mkdir(media_trgt,0777)<0) {
		printf("Unable to create directory %s (probably it already exists)\n",media_trgt);
	}
	if (mkdir(media_src,0777)<0) {
		printf("Unable to create directory %s (probably it already exists)\n",media_src);
	}
	int result = mount(mount_src,mount_trgt,mount_type,mount_flags,NULL);
	if (result==0) {
		system("su - pi -c \"/usr/bin/mocp -s\"");
		printf("Mount created at %s...\n",mount_trgt);
		printf("Removing old music...\n");
		char command1[100];
		sprintf(command1, "rm %s*", media_trgt);
		system(command1);
		printf("Adding new music...\n");
		char command2[100];
		sprintf(command2, "cp %s* %s",media_src,media_trgt);
		system(command2);
		umount(mount_trgt);
		printf("Rebuilding playlist...\n");
		char command3[100];
		sprintf(command3, "su - pi -c \"/bin/ls -d %s*.* > /home/pi/.moc/playlist.m3u && /usr/bin/mocp -o s,r,n -p\"",media_trgt);
		system(command3);
	}
	else {
		printf("Error : Failed to mount %s\n Reason: %s [%d]\n",mount_src,strerror(errno),errno);
		//return -1;
	}
	return 0;
}
