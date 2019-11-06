#ifndef _CHROMA_PHYSICS_
#define _CHROMA_PHYSICS_


#include <glm/glm.hpp>
#include <iostream>

#include "screenmanager/ChromaScreenManagerConfig.h" 
#include "scene/ChromaSceneManager.h"

class ChromaPhysics
{
private:
	void initialize();
	const ChromaSceneManager* mScene;

protected:

	void RayIntersect();

public:
	ChromaPhysics(const ChromaSceneManager* Scene);
	~ChromaPhysics() {};
};

#endif
