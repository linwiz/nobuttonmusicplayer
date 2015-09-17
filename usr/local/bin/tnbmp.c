/*******************************************
    ORIGINAL AUTHOR-PROVIDED INFORMATION
********************************************

 libudev example.

 This example prints out properties of
 each of the hidraw devices. It then
 creates a monitor which will report when
 hidraw devices are connected or removed
 from the system.

 This code is meant to be a teaching
 resource. It can be used for anyone for
 any reason, including embedding into
 a commercial product.

 The document describing this file, and
 updated versions can be found at:
    http://www.signal11.us/oss/udev/

 Alan Ott
 Signal 11 Software
 2010-05-22 - Initial Revision
 2010-05-27 - Monitoring initializaion
              moved to before enumeration.
*******************************************

*******************************************



*******************************************/
#include <linux/limits.h>
#include <libudev.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>
#include <sys/mount.h>
#include <syslog.h>

#define COMMAND_LEN 8
#define DATA_SIZE 512

int mypopen (char *command, char *mode)
{
	FILE *in;
	extern FILE *popen();
	char buff[512];
	if(!(in = popen(command, mode))){
		syslog (LOG_NOTICE, "Command failed: %s", command);
	}
	while(fgets(buff, sizeof(buff), in)!=NULL){
		syslog (LOG_NOTICE, "%s", buff);
	}
	pclose(in);
	return 0;
}

int main (void)
{
	setlogmask (LOG_UPTO (LOG_NOTICE));
	openlog ("tnbmp", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	syslog (LOG_NOTICE, "Program started by User %d", getuid ());

	mypopen("su - pi -c \"/usr/bin/mocp -S\" 2>&1", "r");

        if (geteuid()!=0)
	{
		syslog (LOG_NOTICE, "Please run tnbmp as a superuser");
		closelog ();
                return 1;
        }
	const char* media_src = "/music/usb/";
	const char* media_trgt = "/music/mp3/";
	char command3[100];
	sprintf(command3, "su - pi -c \"/bin/ls -d %s*.* > /home/pi/.moc/playlist.m3u && /usr/bin/mocp -o s,r,n -p\" 2>&1",media_trgt);
	mypopen(command3,"r");

	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;
	char mysubs[] = "block";
	char mysubs2[] = "partition";
   	struct udev_monitor *mon;
	int fd;
	udev = udev_new();
	if (!udev)
	{
		syslog (LOG_NOTICE, "Can't create udev");
		closelog ();
		exit(1);
	}
	mon = udev_monitor_new_from_netlink(udev, "udev");
	udev_monitor_filter_add_match_subsystem_devtype(mon, mysubs, NULL);
	udev_monitor_enable_receiving(mon);
	fd = udev_monitor_get_fd(mon);
	enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, mysubs2);
	udev_enumerate_scan_devices(enumerate);
	devices = udev_enumerate_get_list_entry(enumerate);
	udev_list_entry_foreach(dev_list_entry, devices)
	{
		const char *path;
		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev, path);
		dev = udev_device_get_parent_with_subsystem_devtype(dev,"usb","usb_device");
		if (!dev)
		{
			syslog (LOG_NOTICE, "Unable to find Parent usb device");
		}
	}
	udev_enumerate_unref(enumerate);
	while (1)
	{
		fd_set fds;
		struct timeval tv;
		int ret;
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		ret = select(fd+1, &fds, NULL, NULL, &tv);
		if (ret > 0 && FD_ISSET(fd, &fds))
		{
			dev = udev_monitor_receive_device(mon);
			if (dev)
			{
				regex_t regex;
				int reti;
				reti = regcomp(&regex, "^/dev/sd[a-z][0-9]", 0);
				if (reti)
				{
					syslog (LOG_NOTICE, "Could not compile regex");
				}
				reti = regexec(&regex, udev_device_get_devnode(dev), 0, NULL, 0);
				if (!reti)
				{
					if (strcmp(udev_device_get_action(dev),"add")==0)
					{
						syslog (LOG_NOTICE, "Device node added: %s", udev_device_get_devnode(dev));
						const char* mount_src  = udev_device_get_devnode(dev);
						const char* mount_trgt = "/music/usb";
						const char* mount_type = "vfat";
						const unsigned long mount_flags = 0;
						if (mkdir(media_trgt,0777)<0)
						{
							syslog (LOG_NOTICE, "Unable to create directory %s (already exists?)",media_trgt);
						}
						if (mkdir(media_src,0777)<0)
						{
							syslog (LOG_NOTICE, "Unable to create directory %s (already exists?)",media_src);
						}
						int result = mount(mount_src,mount_trgt,mount_type,mount_flags,NULL);
						if (result==0)
						{
							mypopen("su - pi -c \"/usr/bin/mocp -s\" 2>&1", "r");
							syslog (LOG_NOTICE, "Mount created at %s...\n",mount_trgt);
							syslog (LOG_NOTICE, "Removing old music...\n");
							mypopen("ls -lah /music/mp3 2>&1", "r");
							char command1[100];
							sprintf(command1, "rm -v %s*.* 2>&1", media_trgt);
							mypopen(command1, "r");
							mypopen("ls -lah /music/mp3 2>&1", "r");
							syslog (LOG_NOTICE, "Adding new music...\n");
							char command2[100];
							sprintf(command2, "cp -v %s*.* %s 2>&1",media_src,media_trgt);
							mypopen(command2, "r");
							umount(mount_trgt);
							syslog (LOG_NOTICE, "Rebuilding playlist...\n");
							mypopen(command3, "r");
							syslog (LOG_NOTICE, "Done...\n");
						}
						else
						{
							syslog (LOG_NOTICE, "Error : Failed to mount %s\n Reason: %s [%d]\n",mount_src,strerror(errno),errno);
						}
						udev_device_unref(dev);
					}
				}
				regfree(&regex);
			}
		}
		usleep(250*1000);
		fflush(stdout);
	}
	udev_unref(udev);
	closelog ();
	return 0;
}
