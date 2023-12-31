#include <oppai.h>

static struct option OPTIONS[] = {
    {"nodynamiccps", no_argument, 0, 'd'},
    {"dynamiccpsdeadline", required_argument, 0, 'l'},
    {"togglekey", required_argument, 0, 'k'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};

static const slice HELP_TEXT =
    "Oppai v" OPPAI_VERSION
    "\n"
    "USAGE: \n"
    " --nodynamiccps [-d]: Disables Dynamic CPS System\n"
    " --dynamiccpsdeadline [-l] {DEADLINE}: Sets DCPS Deadline\n"
    " --togglekey [-k] {KEY CODE}: Changes activation key of Oppai.";

bool parseCLI(unique(Enviroment) target, const u16 argc, list(slice) argv)
{
	target->isDynamicCPSEnabled = true;
	target->dynamicCPSDeadline = 300;
	target->toggleKey = KEY_GRAVE;

	int optionIndex = 0;
	char option;

	while ((option = getopt_long(argc, (char **)argv, "l:k:d", OPTIONS,
				     &optionIndex)) != -1)
	{
		switch (option)
		{
			case 'l':
				if (!optarg) goto error;

				target->dynamicCPSDeadline =
				    strtol(optarg, NULL, 10);
				break;
			case 'd':
				target->isDynamicCPSEnabled = false;
				break;
			case 'k':
				if (!optarg) goto error;

				target->toggleKey = strtol(optarg, NULL, 10);
				break;

			case 0:
			case 'h':
			case ':':
			case '?':
			default:
				goto error;
		}
	}

	return true;

error:
	puts(HELP_TEXT);
	return false;
}
