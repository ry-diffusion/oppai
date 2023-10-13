#include <oppai.h>

Oppai oppai = {.devices = {{0}},
	       .devicesFound = 0,
	       .enviroment = {.dynamicCPSDeadline = 100,
			      .isDynamicCPSEnabled = false,
			      .toggleKey = 0},
	       .threads = {0}};

void leave(void)
{
	for (byte idx = 0; idx < oppai.devicesFound; ++idx)
	{
		libevdev_grab(oppai.devices[idx].hDevice, LIBEVDEV_UNGRAB);
		libevdev_free(oppai.devices[idx].hDevice);
		libevdev_uinput_destroy(oppai.devices[idx].uDevice);
	}
}

int main(const int argc, char** argv)
{
	close(STDIN_FILENO);
	if (!parseCLI(&oppai.enviroment, argc, argv)) return 1;

	if (!iAmRoot())
	{
		LOG_ERR("I'm not running as root! Aborting :c");
		return EXIT_FAILURE;
	}

	LOG_INFO("Toggle key: %d", oppai.enviroment.toggleKey);

	if (!scanDevices(&oppai))
	{
		LOG_ERR("Unable to find any devices!");
		return EXIT_FAILURE;
	}

	if (!setScheduler())
		LOG_ERR(
		    "Unable to set scheduler! Your friend input-lag appears!");

	atexit(leave);

	if (!startWorkers(&oppai))
	{
		LOG_ERR("An unknown error occuried in works. Bye!");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
