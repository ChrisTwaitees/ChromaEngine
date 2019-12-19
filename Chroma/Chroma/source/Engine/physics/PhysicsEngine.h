#ifndef _CHROMA_PHYSICS_
#define _CHROMA_PHYSICS_

#include <glm/glm.hpp>
#include <iostream>

// bullet
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

// chroma
#include <ChromaConfig.h> 
#include <component/ChromaPhysicsComponent.h>
#include <physics/PhysicsDebug.h>
#include <buffer/DebugBuffer.h>
#include <time/ChromaTime.h>

class IChromaEntity;

class PhysicsEngine
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
	PhysicsDebug* m_debug{ new PhysicsDebug()};

	// functions
	void init();
	void initPhysics();
	void createGround();
	void updateGravity();

public:
	void addBodyToWorld(ChromaPhysicsComponent*& physicsComponent);
	
	void Update(ChromaTime& time);

	void setGravity(glm::vec3& newGravity) ;

	void BindDebugBuffer(DebugBuffer* const& DebugRenderer);
	void drawDebug();

	// Ray Queries
	IChromaEntity* GetEntityRayTest(glm::vec3& worldRay_origin, glm::vec3& worldRay_end);
	bool RayTest(glm::vec3& worldRay_origin, glm::vec3& worldRay_end);

	PhysicsEngine();
	~PhysicsEngine();
};

#endif
