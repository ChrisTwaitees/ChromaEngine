#ifndef _CHROMA_PHYSICS_
#define _CHROMA_PHYSICS_

#include <glm/glm.hpp>
#include <iostream>

// bullet
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

// chroma
#include "screenmanager/ChromaScreenManagerConfig.h" 
#include "scene/ChromaSceneManager.h"
#include "time/ChromaTime.h"

class ChromaPhysics
{
private:
	void init();
	const ChromaSceneManager* m_scene;

	// new variables
	btBroadphaseInterface*			      m_broadphase;
	btDefaultCollisionConfiguration*      m_collisionConfiguration;
	btCollisionDispatcher*				  m_dispatcher;
	btSequentialImpulseConstraintSolver*  m_solver;
	btDiscreteDynamicsWorld*			  m_world;

public:
	
	void update(ChromaTime& time);
	
	ChromaPhysics(const ChromaSceneManager* Scene);
	~ChromaPhysics() {};
};

#endif
