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
		if ((now() - lastTick) > oppai->enviroment.dynamicCPSDeadline)
		{
			LOG_DEBUG("DynamicCPS: Clicking normally");

			shouldIgnoreClick = true;
		}
		else
		{
			LOG_DEBUG("DynamicCPS: Autoclicker enabled");

			shouldIgnoreClick = false;
		}

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

void* deviceLoop(void* threadContextPtr)
{
	struct WorkerContext context = *(struct WorkerContext*)threadContextPtr;
	Input device = context.input;
	Oppai oppai = context.oppai;
	struct input_event event;

	libevdev_grab(device.hDevice, LIBEVDEV_GRAB);

	while (true)
	{
		libevdev_next_event(device.hDevice, LIBEVDEV_READ_FLAG_NORMAL,
				    &event);

		if (!handleDevice(device, event, &oppai))
		{
			libevdev_uinput_write_event(device.uDevice, event.type,
						    event.code, event.value);
		}
	}

	return NULL;
}

bool loop(unique(Oppai) oppai)
{
	byte idx = 0;
	struct WorkerContext contexts[MAX_DEVICES] = {0};

	for (idx = 0; idx < oppai->devicesFound; ++idx)
	{
		Input device = oppai->devices[idx];

		contexts[idx].input = device;
		contexts[idx].oppai = *oppai;

		pthread_create(&oppai->threads[idx], NULL, deviceLoop,
			       &contexts[idx]);
	}

	for (idx = 0; idx < oppai->devicesFound; ++idx)
	{
		void* ret;

		pthread_join(oppai->threads[idx], &ret);
	}

	return true;
}
