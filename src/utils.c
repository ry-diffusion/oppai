#include <oppai.h>

bool iAmRoot(void)
{
	return !getuid();
}

bool setScheduler(void)
{
	struct sched_param param = {.sched_priority =
					sched_get_priority_max(SCHED_RR)};

	return sched_setscheduler(getpid(), SCHED_RR, &param) == 0;
}
u64 now(void)
{
	struct timeval onceUponATime;
	gettimeofday(&onceUponATime, NULL);

	return (onceUponATime.tv_sec * 1000 + onceUponATime.tv_usec / 1000);
}
