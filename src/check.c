#include <oppai.h>

bool iAmRoot(void)
{
	return !getuid();
}
