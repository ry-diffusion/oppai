#pragma once
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <libevdev/libevdev-uinput.h>
#include <libevdev/libevdev.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <types.h>
#include <unistd.h>

#define MAX_DEVICES 128

#define __LOG__PREFIX(level, contents) level " " __FILE__ ": " contents "\n"

#define LOG_ERR(fmt, ...) \
	fprintf(stderr, __LOG__PREFIX("err  ", fmt), ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) \
	fprintf(stderr, __LOG__PREFIX("info ", fmt), ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) \
	fprintf(stderr, __LOG__PREFIX("debug", fmt), ##__VA_ARGS__)

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

bool parseCLI(unique(Enviroment) target, const u16 argc, list(slice) argv);

bool iAmRoot(void);
bool scanDevices(unique(Oppai) context);
bool loop(unique(Oppai) oppai);
u64 now(void);
