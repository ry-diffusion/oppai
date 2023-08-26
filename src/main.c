#include <oppai.h>

Oppai oppai = {.devices = {{0}},
	       .devicesFound = 0,
	       .enviroment = {.dynamicCPSDeadline = 100,
			      .isDynamicCPSEnabled = false,
			      .toggleKey = 0},
	       .threads = {0}};

void leave(void)
{
	for (int idx = 0; idx < oppai.devicesFound; ++idx)
	{
		libevdev_grab(oppai.devices[idx].hDevice, false);
		libevdev_free(oppai.devices[idx].hDevice);
		libevdev_uinput_destroy(oppai.devices[idx].uDevice);
	}
}

int main(const int argc, list(slice) argv)
{
	if (!parseCLI(&oppai.enviroment, argc, argv)) return 1;

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

	atexit(leave);
	if (!loop(&oppai))
	{
		LOG_ERR("Loop finished");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
