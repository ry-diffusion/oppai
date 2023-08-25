#include <oppai.h>
Oppai oppai;

void leave(void)
{
	for (int idx = 0; idx < oppai.devicesFound; ++idx)
	{
		libevdev_grab(oppai.devices[idx].hDevice, false);
		libevdev_free(oppai.devices[idx].hDevice);
		libevdev_uinput_destroy(oppai.devices[idx].uDevice);
	}
}
int main(void)
{
	loadEnviroment(&oppai.enviroment);
	atexit(leave);
	if (!iAmRoot())
	{
		LOG_ERR("I'm not running as root! Aborting :c");
		return EXIT_FAILURE;
	}

	if (!scanDevices(&oppai))
	{
		LOG_ERR("Unable to find any devices!");
		return EXIT_FAILURE;
	}

	if (!loop(&oppai))
	{
		LOG_ERR("Loop finished");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
