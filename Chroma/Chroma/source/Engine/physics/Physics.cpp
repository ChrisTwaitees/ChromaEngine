#include "Physics.h"

void ChromaPhysics::initialize()
{
}

void ChromaPhysics::RayIntersect()
{
	std::cout << "attempting ray intersect" << std::endl;
}

ChromaPhysics::ChromaPhysics(const ChromaSceneManager* Scene)
{
	mScene = Scene;
	initialize();
}
