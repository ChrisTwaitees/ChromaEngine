#ifndef _CHROMA_TIME_
#define _CHROMA_TIME_

#include <chrono>
#include <thread>

#include <GLFW/glfw3.h>


class ChromaTime
{
	const int MAX_FRAME_RATE = 30;
	const int MAX_FRAMESKIP = 5;
	const int MS_PER_FRAME = 1000 / MAX_FRAME_RATE;

	void sleep(int milliseconds);

	double delta;
	double current;
	double previous;
	double lag;

public:
	double getLag() { return lag; };
	void decreaseLag(double decreaseAmount) { lag -= decreaseAmount; }
	double getMSPerFrame() { return MS_PER_FRAME; };
	double getGameTime() { return glfwGetTime(); }
	double getDeltaTime() { return  delta; };

	void process();
};

#endif