#ifndef _CHROMA_TIME_
#define _CHROMA_TIME_

#include <chrono>
#include <thread>

#include <GLFW/glfw3.h>


class ChromaTime
{
	const int MAX_FRAME_RATE = 60;
	double MS_PER_FRAME = 1.0 / MAX_FRAME_RATE;

	void sleep(int milliseconds);

	double delta{ 0.0f };
	double current{ 0.0f };
	double previous{ 0.0f };
	double lag{ 0.0f };

public:
	double GetLag() { return lag; };
	void DecreaseLag(double decreaseAmount) { lag -= decreaseAmount; }
	double GetMSPerFrame() { return MS_PER_FRAME; };
	double GetDeltaTime() { return  delta; };
	double GetGameTime() { return glfwGetTime(); }

	void Process();
};

#endif