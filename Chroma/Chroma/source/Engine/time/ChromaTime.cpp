#include "ChromaTime.h"



void ChromaTime::sleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void ChromaTime::process()
{
	// updating deltatime and lag
	current = getGameTime();
	delta = current - previous;
	previous = current;

	lag += delta;
}
