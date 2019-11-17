#include "Physics.h"
#include <entity/ChromaEntity.h>

void ChromaPhysics::init()
{
	// create world
	initPhysics();

	createGround();
}

void ChromaPhysics::initPhysics()
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

void ChromaPhysics::createGround()
{
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -150, 0));

	btScalar mass(0.);
	btVector3 localInertia(0, 0, 0);


	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	//add the body to the dynamics world
	m_world->addRigidBody(body);
}


void ChromaPhysics::updateGravity()
{
	m_world->setGravity(btVector3(m_gravity.x, m_gravity.y, m_gravity.z));
}

void ChromaPhysics::addBodyToWorld(ChromaPhysicsComponent*& physicsComponent)
{
	m_world->addRigidBody(physicsComponent->getRigidBody());
}


void ChromaPhysics::update(ChromaTime& time)
{
	// step simulation
	m_world->stepSimulation(time.getDeltaTime());
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
			ChromaPhysicsComponent* physicsComponent = static_cast<ChromaPhysicsComponent*>(body->getUserPointer());
			btTransform trans;
			// collect transform
			if (body && body->getMotionState())
				body->getMotionState()->getWorldTransform(trans);
			else
				trans = obj->getWorldTransform();
			// apply transform
			physicsComponent->transformEntity(trans);
		}
	}

}

void ChromaPhysics::setGravity(glm::vec3 newGravity)
{
	m_world->setGravity(btVector3(newGravity.x, newGravity.y, newGravity.z));
}


ChromaPhysics::ChromaPhysics()
{
	init();
}

ChromaPhysics::~ChromaPhysics()
{
	delete m_world;
	delete m_solver;
	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_broadphase;
}
