#include "Physics.h"

void ChromaPhysics::init()
{
	// create world
	initPhysics();

	// add all physics components to world
	addRigidComponentsToWorld();
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

void ChromaPhysics::addRigidComponentsToWorld()
{
	for (IChromaEntity* entity : m_scene->getEntities())
		for (IChromaComponent* physicsComponent : ((ChromaEntity*)entity)->getPhysicsComponents())
			addBodyToWorld(((ChromaPhysicsComponent*)physicsComponent));
}

void ChromaPhysics::updateGravity()
{
	m_world->setGravity(btVector3(m_gravity.x, m_gravity.y, m_gravity.z));
}

void ChromaPhysics::addBodyToWorld(ChromaPhysicsComponent* const &physicsComponent)
{
	std::cout << "physics component added to world" << std::endl;
	m_world->addRigidBody(physicsComponent->getRigidBody());
}


void ChromaPhysics::update(ChromaTime& time)
{
	m_world->stepSimulation(time.getDeltaTime());
}


ChromaPhysics::ChromaPhysics(ChromaScene* Scene)
{
	m_scene = Scene;
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
