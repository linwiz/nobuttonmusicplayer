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
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <dirent.h>

// Run a command and log all output into syslog.
void logpopen (char *command, char *mode)
{
	FILE *in;
	extern FILE *popen();
	char buff[512];
	if (!(in = popen(command, mode)))
	{
		syslog (LOG_NOTICE, "Command failed: %s (%s :%d)", command, strerror(errno), errno);
	}
	while (fgets(buff, sizeof(buff), in) != NULL)
	{
		syslog (LOG_NOTICE, "%s", buff);
	}
	pclose(in);
}

// Check to see if a directory exists, make it if not.
void makedir(const char *directory)
{
	// Create directories if they are non existant.
	DIR* trgtdir = opendir(directory);
	// Directory exists.
	if (trgtdir)
	{
		closedir(trgtdir);
	}
	// Directory does not exist.
	else if (ENOENT == errno)
	{
		int mkstatus;
		mkstatus = mkdir(directory, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (mkstatus < 0)
		{
			syslog (LOG_NOTICE, "Unable to create directory %s (%s :%d)",
			        directory, strerror(errno), errno);
		}
		else {
			syslog (LOG_NOTICE, "Directory %s created", directory);
		}
	}
	else
	{
		syslog (LOG_NOTICE, "Unable to open directory %s (%s :%d)",
		        directory, strerror(errno), errno);
	}
}

int main (void)
{
	setlogmask (LOG_UPTO (LOG_NOTICE));
	openlog ("tnbmp", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	syslog (LOG_NOTICE, "Program started by User %d", getuid ());

	if (geteuid() != 0)
	{
		syslog (LOG_NOTICE, "Please run tnbmp as a superuser");
		closelog ();
		return 1;
	}

	// Create variables.
	const char* media_src = "/run/media/pi/Seagate Freeagent Go/Documents/Music/kyle/";
	const char* media_trgt = "/music/usb/";
	char commandRebuildPlaylist[100];
	char commandPlayMusic[100];
	char commandRemoveFiles[100];
	char commandCopyFiles[100];
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;
	char mysubs[] = "block";
	char mysubs2[] = "partition";
	struct udev_monitor *mon;
	int fd;
	const char* mount_trgt = "/music/usb";
	const char* mount_type = "vfat";
	const unsigned long mount_flags = 0;

	// Setup udev monitoring instance.
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
		dev = udev_device_get_parent_with_subsystem_devtype(dev,
									"usb", "usb_device");
		if (!dev)
		{
			syslog (LOG_NOTICE, "Unable to find Parent usb device");
		}
	}
	udev_enumerate_unref(enumerate);

	// Start monitoring udev for new usb storage devices.
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
				// Look for a device with a node matching /dev/sd[a-z][0-9].
				reti = regcomp(&regex, "^/dev/sd[a-z][0-9]", 0);
				if (reti)
				{
					syslog (LOG_NOTICE, "Could not compile regex");
				}
				reti = regexec(&regex, udev_device_get_devnode(dev), 0, NULL, 0);
				if (!reti)
				{
					// Check that the matching device is being added.
					if (strcmp(udev_device_get_action(dev), "add") == 0)
					{
						syslog (LOG_NOTICE, "Device node added: %s",
						        udev_device_get_devnode(dev));
						const char* mount_src  = udev_device_get_devnode(dev);

						// Mount the added usb device.
						int result = mount(mount_src, mount_trgt, mount_type, mount_flags, NULL);
						if (result == 0)
						{
							syslog (LOG_NOTICE, "%s", udev_device_get_sysname(dev));
							// Add new music from the usb device.
							//syslog (LOG_NOTICE, "Adding new music...\n");
							//sprintf(commandCopyFiles, "/bin/cp %s*.* %s 2>&1", media_src, media_trgt);
							//logpopen(commandCopyFiles, "r");

							// Remove the mount point.
							//syslog (LOG_NOTICE, "Mount removed from  %s...\n", mount_trgt);
							//umount(mount_trgt);
						}
						else
						{
							syslog (LOG_NOTICE, "Error : Failed to mount %s\n Reason: %s [%d]\n",
							        mount_src, strerror(errno), errno);
						}
						udev_device_unref(dev);
					}
				}
				regfree(&regex);
			}
		}
		usleep(250 * 1000);
		fflush(stdout);
	}
	udev_unref(udev);
	closelog ();
	return 0;
}
