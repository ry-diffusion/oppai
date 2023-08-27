#include <oppai.h>

static bool isEnabled = false;
static u64 lastTick = 0;
static bool shouldIgnoreClick = false;

static bool handleDevice(Input device, struct input_event event,
			 unique(Oppai) oppai)
{
	if (event.type != EV_KEY) return false;

	if (event.code == oppai->enviroment.toggleKey && event.value)
	{
		isEnabled = !isEnabled;
		LOG_INFO("Upgraded state to %d", isEnabled);
		return true;
	}

	if (!isEnabled) return false;
	if (event.code != BTN_LEFT && event.code != BTN_RIGHT) return false;
	if (oppai->enviroment.isDynamicCPSEnabled)
	{
		shouldIgnoreClick =
		    (now() - lastTick) > oppai->enviroment.dynamicCPSDeadline;

		LOG_DEBUG(shouldIgnoreClick
			      ? "DynamicCPS: Clicking normally"
			      : "DynamicCPS: Autoclicker enabled");

		lastTick = now();
	}

	if (shouldIgnoreClick) return false;

	for (int i = 0; i < 2; ++i)
	{
		libevdev_uinput_write_event(device.uDevice, EV_KEY, event.code,
					    1);

		libevdev_uinput_write_event(device.uDevice, EV_SYN, SYN_REPORT,
					    0);

		libevdev_uinput_write_event(device.uDevice, EV_KEY, event.code,
					    0);

		libevdev_uinput_write_event(device.uDevice, EV_SYN, SYN_REPORT,
					    0);
	}

	return true;
}

static void* deviceWorker(void* threadContextPtr)
{
	struct WorkerContext context = *(struct WorkerContext*)threadContextPtr;
	struct input_event event;

	char taskBuf[15] = {0};

	Input device = context.input;
	Oppai oppai = context.oppai;

	int error;

	strncpy(taskBuf, libevdev_get_name(device.hDevice), 15);

	pthread_setname_np(pthread_self(), taskBuf);

	libevdev_grab(device.hDevice, LIBEVDEV_GRAB);

	while (true)
	{
		if ((error = libevdev_next_event(
			 device.hDevice, LIBEVDEV_READ_FLAG_BLOCKING, &event)) <
		    0)
		{
			LOG_ERR(
			    "An error ocurried when waiting for a event: %s",
			    strerror(error));

			continue;
		}

		if (!handleDevice(device, event, &oppai))
			libevdev_uinput_write_event(device.uDevice, event.type,
						    event.code, event.value);
	}

	return NULL;
}

bool startWorkers(unique(Oppai) oppai)
{
	byte idx = 0;
	void* ret;

	struct WorkerContext contexts[MAX_DEVICES] = {0};

	for (idx = 0; idx < oppai->devicesFound; ++idx)
	{
		Input device = oppai->devices[idx];

		contexts[idx].input = device;
		contexts[idx].oppai = *oppai;

		pthread_create(&oppai->threads[idx], NULL, deviceWorker,
			       &contexts[idx]);
	}

	for (idx = 0; idx < oppai->devicesFound; ++idx)
		pthread_join(oppai->threads[idx], &ret);

	return true;
}
