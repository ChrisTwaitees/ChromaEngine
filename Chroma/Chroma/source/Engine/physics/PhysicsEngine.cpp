#include "PhysicsEngine.h"
#include <entity/IEntity.h>
#include <math/Math.h>

// callbacks
struct ChromaSimpleContactResultCallback : public btCollisionWorld::ContactResultCallback
{
	btVector3 ptA;
	btVector3 ptB;

	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
		ptA = cp.getPositionWorldOnA();
		ptB = cp.getPositionWorldOnB();
		return 0.0;
	}

	bool hasHit()
	{
		if (glm::length(BulletToGLM(ptA) - BulletToGLM(ptB)) < 0.05)
		{
			return true;
		}
		return false;
	}
};

struct ChromaContactResultCallback : public btCollisionWorld::ContactResultCallback
{
	//! Constructor, pass whatever context you want to have available when processing contacts
	/*! You may also want to set m_collisionFilterGroup and m_collisionFilterMask
	 * (supplied by the superclass) for needsCollision() */
	ChromaContactResultCallback(btRigidBody& tgtBody, std::vector<CollisionData>& collisionData /*, ... */)
		: btCollisionWorld::ContactResultCallback(), body(tgtBody), collisionData(collisionData){ }

	btRigidBody& body; //!< The body the sensor is monitoring
	std::vector<CollisionData>& collisionData;

	//! If you don't want to consider collisions where the bodies are joined by a constraint, override needsCollision:
	/*! However, if you use a btCollisionObject for #body instead of a btRigidBody,
	 * then this is unnecessary—checkCollideWithOverride isn't available */
	virtual bool needsCollision(btBroadphaseProxy* proxy) const {
		// superclass will check m_collisionFilterGroup and m_collisionFilterMask
		if (!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
			return false;
		// if passed filters, may also want to avoid contacts between constraints
		return body.checkCollideWithOverride(static_cast<btCollisionObject*>(proxy->m_clientObject));
	}

	//! Called with each contact for your own processing (e.g. test if contacts fall in within sensor parameters)
	virtual btScalar addSingleResult(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0, int partId0, int index0,
		const btCollisionObjectWrapper* colObj1, int partId1, int index1)
	{
		CollisionData newColData;

		// fetching contact points and collider normal
		newColData.m_ColliderAContactPoint = BulletToGLM(cp.getPositionWorldOnA());
		newColData.m_ColliderBContactPoint = BulletToGLM(cp.getPositionWorldOnB());
		newColData.m_ColliderBContactNormal = BulletToGLM(cp.m_normalWorldOnB);

		// fetching collision tags
		// colA
		if (colObj0->getCollisionObject()->getCollisionFlags() == btCollisionObject::CF_STATIC_OBJECT)
			newColData.m_ColliderAState = Static;
		else if (colObj0->getCollisionObject()->getCollisionFlags() == btCollisionObject::CF_KINEMATIC_OBJECT)
			newColData.m_ColliderAState = Kinematic;
		else
			newColData.m_ColliderAState = Dynamic;

		// colB
		if (colObj1->getCollisionObject()->getCollisionFlags() == btCollisionObject::CF_STATIC_OBJECT)
			newColData.m_ColliderBState = Static;

		else if (colObj1->getCollisionObject()->getCollisionFlags() == btCollisionObject::CF_KINEMATIC_OBJECT)
			newColData.m_ColliderBState = Kinematic;
		else
			newColData.m_ColliderBState = Dynamic;

		// push back new collision data
		collisionData.push_back(newColData);
		return 0.0;
	}
};

class ClosestNotMeRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
{
public:
	ClosestNotMeRayResultCallback(btCollisionObject* me);
	virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace);
protected:
	btCollisionObject* m_me;
};

ClosestNotMeRayResultCallback::ClosestNotMeRayResultCallback (btCollisionObject* me) 
	: ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
{
	m_me = me;
}

btScalar ClosestNotMeRayResultCallback::addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
{
	if (rayResult.m_collisionObject == m_me)
		return 1.0;

	return ClosestNotMeRayResultCallback::ClosestRayResultCallback::addSingleResult (rayResult, normalInWorldSpace);
}

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
	bool Physics::m_DrawDebug;
	PhysicsDebug* Physics::m_Debug;

	void Physics::Init()
	{
		// new debugbuffer
		m_Debug = new PhysicsDebug();
		m_DrawDebug = false;

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

	void Physics::AddBodyToWorld(btRigidBody*& rigidBody)
	{
		m_World->addRigidBody(rigidBody);
	}

	void Physics::RemoveBodyFromWorld(btRigidBody*& rigidBody)
	{
		m_World->removeRigidBody(rigidBody);
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
				btTransform currentWorldTransform;
				// collect transform
				if (body && body->getMotionState())
					body->getMotionState()->getWorldTransform(currentWorldTransform);
				else
					currentWorldTransform = obj->getWorldTransform();
				// apply transform
				if (physicsComponent->GetColliderState() == Dynamic)
					physicsComponent->Transform(currentWorldTransform);
			}
		}

		// debug
		if(m_DrawDebug)
			DrawDebug();

	}

	void Physics::ToggleDrawDebug()
	{
		m_DrawDebug = m_DrawDebug ? false : true;
	}

	void Physics::SetGravity(glm::vec3 & newGravity)
	{
		m_World->setGravity(btVector3(newGravity.x, newGravity.y, newGravity.z));
	}

	void Physics::BindDebugBuffer(DebugBuffer* DebugRenderer)
	{
		m_Debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
		m_World->setDebugDrawer(m_Debug);
	}

	void Physics::DrawDebug()
	{
		m_World->debugDrawWorld();
	}

	RayHitData Physics::GetRayHitData(glm::vec3 const& worldRay_origin, glm::vec3 const& worldRay_end)
	{
		RayHitData newRayHit;

		btVector3 start(worldRay_origin.x, worldRay_origin.y, worldRay_origin.z);
		btVector3 end(worldRay_end.x, worldRay_end.y, worldRay_end.z);

		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);

		m_World->rayTest(start, end, RayCallback);

		if (RayCallback.hasHit())
		{
			newRayHit.m_Hit = true;
			newRayHit.m_RayStart = worldRay_origin;
			newRayHit.m_RayHitPosition = BulletToGLM(RayCallback.m_hitPointWorld);
			newRayHit.m_RayHitNormal = BulletToGLM(RayCallback.m_hitNormalWorld);
		}

		return newRayHit;
	}

	RayHitData Physics::GetRayHitDataExcludeRigidbody(btRigidBody*& rigidBody, glm::vec3 const& rayStart, glm::vec3 const& rayEnd)
	{
		RayHitData newRayHit;

		btVector3 start(rayStart.x, rayStart.y, rayStart.z);
		btVector3 end(rayEnd.x, rayEnd.y, rayEnd.z);

		ClosestNotMeRayResultCallback rayCallBack(rigidBody);

		m_World->rayTest(start, end, rayCallBack);

		if (rayCallBack.hasHit())
		{
			newRayHit.m_Hit = true;
			newRayHit.m_RayStart = rayStart;
			newRayHit.m_RayHitPosition = BulletToGLM(rayCallBack.m_hitPointWorld);
			newRayHit.m_RayHitNormal = BulletToGLM(rayCallBack.m_hitNormalWorld);
		}

		return newRayHit;
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
		btScalar m_Radius{ sphereRadius };
		
		// use local collision object to test
		m_CollisionObject->getWorldTransform().setOrigin(center);
		// convert to sphere
		m_SphereShape->setUnscaledRadius(m_Radius);
		m_CollisionObject->setCollisionShape(m_SphereShape);

		// create custom callback and query world using it
		ChromaSimpleContactResultCallback ContactSphereCallback;
		m_World->contactTest(m_CollisionObject, ContactSphereCallback);

		return ContactSphereCallback.hasHit();
	}

	std::vector<CollisionData> Physics::GetRigidBodyCollisionData(btRigidBody*& rigidBody)
	{
		std::vector<CollisionData> colData;

		ChromaContactResultCallback collisionCallBack(*rigidBody, colData);
		m_World->contactTest(rigidBody, collisionCallBack);

		return colData;
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


