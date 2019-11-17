#ifndef _CHROMA_PHYSICS_
#define _CHROMA_PHYSICS_

#include <glm/glm.hpp>
#include <iostream>

// bullet
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

// chroma
#include "screenmanager/ChromaScreenManagerConfig.h" 
#include <component/ChromaPhysicsComponent.h>
#include <entity/ChromaEntity.h>
#include "scene/ChromaSceneManager.h"
#include "time/ChromaTime.h"

class ChromaPhysics
{
private:

	ChromaSceneManager* m_scene;
	glm::vec3 m_gravity{ 0.0, -9.8, 0.9 };

	// new variables
	btBroadphaseInterface*			      m_broadphase;
	btDefaultCollisionConfiguration*      m_collisionConfiguration;
	btCollisionDispatcher*				  m_dispatcher;
	btSequentialImpulseConstraintSolver*  m_solver;
	btDiscreteDynamicsWorld*			  m_world;

	void init();
	void initPhysics();
	void addRigidComponentsToWorld();
	void updateGravity();
	void addBodyToWorld(ChromaPhysicsComponent* const& physicsComponent);

public:
	
	void update(ChromaTime& time);
	
	void setGravity(glm::vec3 newGravity);

	ChromaPhysics(ChromaSceneManager* Scene);
	~ChromaPhysics();
};

#endif
