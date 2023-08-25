#include <oppai.h>

bool iAmRoot(void)
{
	return !getuid();
}

u64 now(void)
{
	struct timeval onceUponATime;
	gettimeofday(&onceUponATime, NULL);

	return ((onceUponATime.tv_sec * 1000) + onceUponATime.tv_usec / 1000);
}
