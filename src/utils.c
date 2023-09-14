#include <oppai.h>

bool iAmRoot(void)
{
	return !getuid();
}

void setScheduler(void)
{
	struct sched_param param = {.sched_priority =
					sched_get_priority_max(SCHED_RR)};
	if (sched_setscheduler(getpid(), SCHED_RR, &param) < 0)
	{
		LOG_ERR("Unable to set scheduler! Your input may be wrong!");
	}
}

u64 now(void)
{
	struct timeval onceUponATime;
	gettimeofday(&onceUponATime, NULL);

	return (onceUponATime.tv_sec * 1000 + onceUponATime.tv_usec / 1000);
}
