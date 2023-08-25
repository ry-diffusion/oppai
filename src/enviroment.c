
#include <oppai.h>

void loadEnviroment(unique(Enviroment) target)
{
	const slice toggleKey = getenv("OPPAI_TOGGLE_KEY");

	target->toggleKey = toggleKey ? atoi(toggleKey) : KEY_GRAVE;
}
