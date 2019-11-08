#include "Physics.h"

void ChromaPhysics::init()
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
	m_world->setGravity(btVector3(0, -9.8, 0));
}


void ChromaPhysics::update(ChromaTime& time)
{
	std::cout << "current delta time : " << time.getDeltaTime() << std::endl;
}


ChromaPhysics::ChromaPhysics(const ChromaSceneManager* Scene)
{
	m_scene = Scene;
	init();
}
