#include "PhysicsEngine.h"
#include <entity/IEntity.h>

void PhysicsEngine::init()
{
	// create world
	initPhysics();

	// ground 
	createGround();
}

void PhysicsEngine::initPhysics()
{
	//1
	m_broadphase = new btDbvtBroadphase();

	//2
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	//3
	m_solver = new btSequentialImpulseConstraintSolver();

	//4
	m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

	//5
	updateGravity();
}


void PhysicsEngine::createGround()
{
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(10.), btScalar(50.)));

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -10, 0));

	btScalar mass(0.);
	btVector3 localInertia(0, 0, 0);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	//add the body to the dynamics world
	m_world->addRigidBody(body);
}


void PhysicsEngine::updateGravity()
{
	m_world->setGravity(btVector3(m_gravity.x, m_gravity.y, m_gravity.z));
}

void PhysicsEngine::addBodyToWorld(PhysicsComponent*& physicsComponent)
{
	m_world->addRigidBody(physicsComponent->GetRigidBody());
}


void PhysicsEngine::Update(ChromaTime& time)
{
	// step simulation
	m_world->stepSimulation(time.GetDeltaTime());
	// update transforms of physics entities
	for (int i = 0; i < m_world->getNumCollisionObjects(); i++)
	{
		// collect rigid body
		btCollisionObject* obj = m_world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

		// check if connected to physics component
		if (body->getUserPointer() != NULL)
		{
			// collect physics component
			PhysicsComponent* physicsComponent = static_cast<PhysicsComponent*>(body->getUserPointer());
			btTransform trans;
			// collect transform
			if (body && body->getMotionState())
				body->getMotionState()->getWorldTransform(trans);
			else
				trans = obj->getWorldTransform();
			// apply transform
			if (physicsComponent->getColliderState() == Dynamic)
				physicsComponent->Transform(trans);
		}
	}

}

void PhysicsEngine::setGravity(glm::vec3& newGravity)
{
	m_world->setGravity(btVector3(newGravity.x, newGravity.y, newGravity.z));
}

void PhysicsEngine::BindDebugBuffer(DebugBuffer* const& DebugRenderer)
{
	m_debug->BindDebugBuffer(DebugRenderer);
	m_debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_world->setDebugDrawer(m_debug);
}

void PhysicsEngine::drawDebug()
{
	m_world->debugDrawWorld();
}

IEntity* PhysicsEngine::GetEntityRayTest(glm::vec3& worldRay_origin, glm::vec3& worldRay_end)
{
	btVector3 start(worldRay_origin.x, worldRay_origin.y, worldRay_origin.z);
	btVector3 end(worldRay_end.x, worldRay_end.y, worldRay_end.z);

	btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
	
	m_world->rayTest(start, end, RayCallback);

	if (RayCallback.hasHit())
	{
		void* rayObjectPointer = RayCallback.m_collisionObject->getUserPointer();
		if (rayObjectPointer != NULL)
		{
			std::cout << "Entity Hit : " << std::endl;
			std::cout << static_cast<IComponent*>(rayObjectPointer)->GetParentEntity()->GetName() << std::endl;
			return static_cast<IComponent*>(rayObjectPointer)->GetParentEntity();
		}
		else
		{
			std::cout << "Hit RigidBody, no Entity Associated" << std::endl;
			return NULL;
		}
	}
	else
		return NULL;
	
}

bool PhysicsEngine::RayTest(glm::vec3& worldRay_origin, glm::vec3& worldRay_end)
{
	btVector3 start(worldRay_origin.x, worldRay_origin.y, worldRay_origin.z);
	btVector3 end(worldRay_end.x, worldRay_end.y, worldRay_end.z);

	btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);

	m_world->rayTest(start, end, RayCallback);

	return RayCallback.hasHit();
}



PhysicsEngine::PhysicsEngine()
{
	init();
}

PhysicsEngine::~PhysicsEngine()
{
	delete m_world;
	delete m_solver;
	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_broadphase;

}
