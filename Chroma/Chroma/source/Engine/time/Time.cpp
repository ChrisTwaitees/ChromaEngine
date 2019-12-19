#include "Time.h"



void Time::sleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void Time::Process()
{
	// updating deltatime and lag
	current = GetGameTime();
	delta = current - previous;
	previous = current;

	lag += delta;
}
