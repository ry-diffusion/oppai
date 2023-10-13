#include <oppai.h>

void executeLog(LogLevel level, ...)
{
	va_list va;
	time_t stamp;
	struct tm* info;

	char *now, *logLevel;

	time(&stamp);
	info = localtime(&stamp);
	now = asctime(info);

	now[strlen(now) - 1] = '\0';
	va_start(va, level);

	switch (level)
	{
		case ERROR:
			logLevel = "error";
			break;
		case INFO:
			logLevel = "info";
			break;
		case DEBUG:
			logLevel = "debug";
			break;
	}

	const slice fmt = va_arg(va, const slice);

	printf("%6s %s ", logLevel, now);
	vprintf(fmt, va);
	puts("");
}
