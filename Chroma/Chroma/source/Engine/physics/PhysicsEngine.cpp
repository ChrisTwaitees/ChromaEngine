#include "PhysicsEngine.h"
#include <entity/IEntity.h>

// callbacks
struct myContactResultCallback : public btCollisionWorld::ContactResultCallback
{
	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
		btVector3 ptA = cp.getPositionWorldOnA();
		btVector3 ptB = cp.getPositionWorldOnB();
		return 0.0;
	}

	bool hasHit()
	{
		CHROMA_ERROR("Physics :: MyContactCallBack not implemented");
		return false;
	}
};



namespace Chroma
{
	float Physics::m_TerrainFriction;
	glm::vec3 Physics::m_Gravity;
	btCollisionObject* Physics::m_CollisionObject;
	btSphereShape* Physics::m_SphereShape;

	// bullet objects
	btBroadphaseInterface* Physics::m_Broadphase;
	btDefaultCollisionConfiguration* Physics::m_CollisionConfiguration;
	btCollisionDispatcher* Physics::m_Dispatcher;
	btSequentialImpulseConstraintSolver* Physics::m_Solver;
	btDiscreteDynamicsWorld* Physics::m_World;

	// debug
	PhysicsDebug* Physics::m_Debug;

	void Physics::Init()
	{
		// new debugbuffer
		m_Debug = new PhysicsDebug();

		// create world
		InitPhysicsWorld();

		// ground 
		InitTerrain();
	}

	void Physics::InitPhysicsWorld()
	{
		// attrs
		m_CollisionObject = new btCollisionObject;
		m_SphereShape = new btSphereShape(1.0);
		m_Gravity = glm::vec3(0.0, -9.81, 0.0);

		//1
		m_Broadphase = new btDbvtBroadphase();

		//2
		m_CollisionConfiguration = new btDefaultCollisionConfiguration();
		m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);

		//3
		m_Solver = new btSequentialImpulseConstraintSolver();

		//4
		m_World = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfiguration);

		//5
		UpdateGravity();

		//6 add test shapes to collision world
		//m_World->addCollisionObject(m_CollisionObject); TODO : Needs shape and user pointer
	}


	void Physics::InitTerrain()
	{
		m_TerrainFriction = 10.0f;

		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(10.), btScalar(50.)));

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -10, 0));

		btScalar mass(0.);
		btVector3 localInertia(0, 0, 0);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		rbInfo.m_friction = m_TerrainFriction;
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		m_World->addRigidBody(body);
	}


	void Physics::UpdateGravity()
	{
		m_World->setGravity(btVector3(m_Gravity.x, m_Gravity.y, m_Gravity.z));
	}

	void Physics::AddBodyToWorld(PhysicsComponent*& physicsComponent)
	{
		m_World->addRigidBody(physicsComponent->GetRigidBody());
	}


	void Physics::Update()
	{
		// step simulation
		m_World->stepSimulation(Chroma::Time::GetDeltaTime());
		// update transforms of physics entities
		for (int i = 0; i < m_World->getNumCollisionObjects(); i++)
		{
			// collect rigid body
			btCollisionObject* obj = m_World->getCollisionObjectArray()[i];
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

	void Physics::SetGravity(glm::vec3 & newGravity)
	{
		m_World->setGravity(btVector3(newGravity.x, newGravity.y, newGravity.z));
	}

	void Physics::BindDebugBuffer(DebugBuffer* DebugRenderer)
	{
		m_Debug->BindDebugBuffer(DebugRenderer);
		m_Debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
		m_World->setDebugDrawer(m_Debug);
	}

	void Physics::DrawDebug()
	{
		m_World->debugDrawWorld();
	}

	IEntity* Physics::GetEntityRayTest(glm::vec3 & worldRay_origin, glm::vec3 & worldRay_end)
	{
		btVector3 start(worldRay_origin.x, worldRay_origin.y, worldRay_origin.z);
		btVector3 end(worldRay_end.x, worldRay_end.y, worldRay_end.z);

		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);

		m_World->rayTest(start, end, RayCallback);

		if (RayCallback.hasHit())
		{
			void* rayObjectPointer = RayCallback.m_collisionObject->getUserPointer();
			if (rayObjectPointer != NULL)
			{
				CHROMA_INFO("PHYSICS DEBUG :: Entity Hit : {0}", static_cast<IComponent*>(rayObjectPointer)->GetParentEntity()->GetName());
				return static_cast<IComponent*>(rayObjectPointer)->GetParentEntity();
			}
			else
			{
				CHROMA_INFO("PHYSICS DEBUG :: Hit RigidBody, no Entity Associated");
				return NULL;
			}
		}
		else
			return NULL;

	}

	bool Physics::RayTest(glm::vec3 & worldRay_origin, glm::vec3 & worldRay_end)
	{
		btVector3 start(worldRay_origin.x, worldRay_origin.y, worldRay_origin.z);
		btVector3 end(worldRay_end.x, worldRay_end.y, worldRay_end.z);

		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);

		m_World->rayTest(start, end, RayCallback);

		return RayCallback.hasHit();
	}

	bool Physics::SphereTest(glm::vec3 const& sphereCenter, float const& sphereRadius)
	{
		btVector3 center(sphereCenter.x, sphereCenter.y, sphereCenter.z);
		btScalar radius{ sphereRadius };
		
		// use local collision object to test
		m_CollisionObject->getWorldTransform().setOrigin(center);
		// convert to sphere
		m_SphereShape->setUnscaledRadius(radius);
		m_CollisionObject->setCollisionShape(m_SphereShape);

		// create custom callback and query world using it
		myContactResultCallback ContactSphereCallback;
		m_World->contactTest(m_CollisionObject, ContactSphereCallback);

		return ContactSphereCallback.hasHit();
	}


	float Physics::CalcInertiaToReachHeight(float const& targetHeight, float const& gravityMagnitude)
	{
		return glm::sqrt(targetHeight * (-2.0f * gravityMagnitude));
	}

	Physics::Physics()
	{
		Init();
	}

	Physics::~Physics()
	{
		delete m_World;
		delete m_Solver;
		delete m_CollisionConfiguration;
		delete m_Dispatcher;
		delete m_Broadphase;

	}

}


