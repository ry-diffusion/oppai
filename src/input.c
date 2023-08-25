#include <oppai.h>

static bool openDevice(const slice fileName, unique(Input) inputDevice,
		       unique(Oppai) oppai)
{
	int fd;

	fd = open(fileName, O_RDWR | O_NDELAY);

	if (fd < 0) return false;

	if (libevdev_new_from_fd(fd, &inputDevice->hDevice) < 0)
	{
		close(fd);
		return false;
	}

	if (!inputDevice->hDevice)
	{
		close(fd);
		return false;
	}

	if (!(libevdev_has_event_code(inputDevice->hDevice, EV_KEY,
				      oppai->enviroment.toggleKey) ||
	      libevdev_has_event_code(inputDevice->hDevice, EV_KEY, BTN_LEFT)))
		return false;

	if (libevdev_uinput_create_from_device(inputDevice->hDevice,
					       LIBEVDEV_UINPUT_OPEN_MANAGED,
					       &inputDevice->uDevice) < 0)
	{
		libevdev_free(inputDevice->hDevice);
		close(fd);
		return false;
	}

	return true;
}

bool scanDevices(unique(Oppai) context)
{
	DIR* dInput;
	byte idx = 0;
	struct dirent* entry;
	dInput = opendir("/dev/input");

	if (!dInput)
	{
		LOG_ERR("unable to open /dev/input(dir) cuz %s",
			strerror(errno));
		return false;
	}

	while ((entry = readdir(dInput)))
	{
		char path[256] = {0};
		strcpy(path, "/dev/input/");
		strcat(path, entry->d_name);

		if (entry->d_name[0] != 'e') continue;
		if (!openDevice(path, &context->devices[idx], context))
		{
			/*LOG_DEBUG("Unable to open device %s cuz %s", path,
				  strerror(errno));
				  */

			continue;
		}

		LOG_DEBUG("Opened device %s (%s)", path,
			  libevdev_get_name(context->devices[idx].hDevice));

		++idx;
	}

	context->devicesFound = idx;
	closedir(dInput);

	return idx;
}
