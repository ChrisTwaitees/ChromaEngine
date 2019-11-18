#ifndef _CHROMA_PHYSICS_
#define _CHROMA_PHYSICS_

#include <glm/glm.hpp>
#include <iostream>

// bullet
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

// chroma
#include <screenmanager/ChromaScreenManagerConfig.h> 
#include <component/ChromaPhysicsComponent.h>
#include <physics/PhysicsDebug.h>
#include <buffer/DebugBuffer.h>
#include <time/ChromaTime.h>

class IChromaEntity;

class ChromaPhysics
{
private:
	// attrs
	glm::vec3 m_gravity{ 0.0, -9.8, 0.0 };

	// bullet objects
	btBroadphaseInterface*			      m_broadphase;
	btDefaultCollisionConfiguration*      m_collisionConfiguration;
	btCollisionDispatcher*				  m_dispatcher;
	btSequentialImpulseConstraintSolver*  m_solver;
	btDiscreteDynamicsWorld*			  m_world;

	// debug
	ChromaPhysicsDebug* m_debug{ new ChromaPhysicsDebug()};

	// functions
	void init();
	void initPhysics();
	void createGround();
	void updateGravity();

public:
	void addBodyToWorld(ChromaPhysicsComponent*& physicsComponent);
	
	void update(ChromaTime& time);

	void setGravity(glm::vec3& newGravity) ;

	void bindDebugBuffer(DebugBuffer* const& DebugRenderer);
	void drawDebug();

	IChromaEntity* rayTest(glm::vec3& worldRay_origin, glm::vec3& worldRay_end);

	ChromaPhysics();
	~ChromaPhysics();
};

#endif
