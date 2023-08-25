
#include <oppai.h>

void loadEnviroment(unique(Enviroment) target)
{
	target->isDynamicCPSEnabled = true;
	target->dynamicCPSDeadline = 300;

	const slice toggleKey = getenv("OPPAI_TOGGLE_KEY");

	target->toggleKey = toggleKey ? atoi(toggleKey) : KEY_GRAVE;
}
