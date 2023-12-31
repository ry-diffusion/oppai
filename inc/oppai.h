#pragma once
#define _GNU_SOURCE
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <libevdev/libevdev-uinput.h>
#include <libevdev/libevdev.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <types.h>
#include <unistd.h>

#include "oppai_const.h"

#define MAX_DEVICES 127

#define LOG_ERR(...) executeLog(ERROR, ##__VA_ARGS__)
#define LOG_INFO(...) executeLog(INFO, ##__VA_ARGS__)

#if defined(BUILD_DEBUG)
#define LOG_DEBUG(...) executeLog(DEBUG, ##__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

typedef struct Enviroment
{
	u8 toggleKey;
	bool isDynamicCPSEnabled;
	u64 dynamicCPSDeadline;
} Enviroment;

typedef struct Input
{
	struct libevdev* hDevice;
	struct libevdev_uinput* uDevice;
} Input;

typedef struct Context
{
	Enviroment enviroment;
	Input devices[MAX_DEVICES];
	pthread_t threads[MAX_DEVICES];

	byte devicesFound;
} Oppai;

struct WorkerContext
{
	Input input;
	Oppai oppai;
};

typedef enum LogLevel
{
	ERROR,
	INFO,
	DEBUG
} LogLevel;

__attribute_used__ static inline bool max(int a, int b)
{
	return a > b ? a : b;
}

void executeLog(LogLevel level, ...);
bool parseCLI(unique(Enviroment) target, const u16 argc, list(slice) argv);
bool setScheduler(void);
bool iAmRoot(void);
bool scanDevices(unique(Oppai) context);
bool startWorkers(unique(Oppai) oppai);
u64 now(void);
