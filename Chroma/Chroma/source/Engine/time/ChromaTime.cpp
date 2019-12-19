#include "ChromaTime.h"



void ChromaTime::sleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void ChromaTime::Process()
{
	// updating deltatime and lag
	current = GetGameTime();
	delta = current - previous;
	previous = current;

	lag += delta;
}
