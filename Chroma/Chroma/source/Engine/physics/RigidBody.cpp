#include "RigidBody.h"


namespace Chroma
{
	void RigidBody::SetConstructionData(const RigidBodyConstructionData& constructionData)
	{
		if (m_ConstructionData != constructionData)
		{
			m_ConstructionData = constructionData;
			RemoveFromWorld();
			BuildRigidBody();
		}
	}


	void RigidBody::SetShape(ColliderShape newShape)
	{
		m_ConstructionData.m_ColliderShape = newShape;

		m_RigidbodyRaw->setCollisionShape(GenerateColliderShape());
	}

	void RigidBody::SetState(ColliderState newState)
	{
		m_ConstructionData.m_ColliderState = newState;

		switch (m_ConstructionData.m_ColliderState)
		{
			case Static:
			{
				m_RigidbodyRaw->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
				break;
			}
			case Kinematic:
			{
				m_RigidbodyRaw->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
				m_RigidbodyRaw->setActivationState(DISABLE_DEACTIVATION);
				break;
			}
			case Dynamic:
			{
				m_RigidbodyRaw->setActivationState(DISABLE_DEACTIVATION);
				break;
			}
		}
	}

	void RigidBody::SetTransform(const glm::mat4& newTransform)
	{
		m_Transform = newTransform;
		m_RigidbodyRaw->getMotionState()->setWorldTransform(GLMToBullet(m_Transform * m_ConstructionData.m_LocalTransform));
	}

	glm::mat4 RigidBody::GetTransform()
	{
		btTransform transform;
		m_RigidbodyRaw->getMotionState()->getWorldTransform(transform);
		return BulletToGLM(transform);
	}

	void RigidBody::SetRawRigid(btRigidBody* newRigid)
	{

		RemoveFromWorld();

		delete m_RigidbodyRaw;

		m_RigidbodyRaw = newRigid;

		AddToWorld();

	}

	void RigidBody::RemoveFromWorld()
	{
		Physics::RemoveBodyFromWorld(m_RigidbodyRaw);
	}

	void RigidBody::AddToWorld()
	{
		Physics::AddBodyToWorld(m_RigidbodyRaw);
	}


	void RigidBody::BuildRigidBody()
	{
		// construct rigid shape
		btCollisionShape* newColliderShape = GenerateColliderShape();

		// create bullet construction info
		btScalar mass = btScalar(1.0f);
		btVector3 bodyIntertia(0, 0, 0);
		btDefaultMotionState* newMotionState = new btDefaultMotionState(GLMToBullet(m_Transform));
		btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(mass, newMotionState, newColliderShape, bodyIntertia);

		// set friction and restitution
		bodyCI.m_restitution = 1.0f;
		bodyCI.m_friction = 1.0f;

		// create rigid
		m_RigidbodyRaw = new btRigidBody(bodyCI);

		// set state
		SetState(m_ConstructionData.m_ColliderState);

		// add to physics world
		AddToWorld();
	}

	btCollisionShape* RigidBody::GenerateColliderShape()
	{
		switch (m_ConstructionData.m_ColliderShape)
		{
			case Box :
			{
				return new btBoxShape(GLMToBullet(m_ConstructionData.m_HalfExtents));
				break;
			}
			case Sphere :
			{				
				return new btSphereShape(btScalar(m_ConstructionData.m_Radius));
				break;
			}
			case Cylinder :
			{			
				return new btCylinderShape(GLMToBullet(m_ConstructionData.m_HalfExtents));
				break;
			}
			case Capsule :
			{
				return new btCapsuleShape(m_ConstructionData.m_Radius, m_ConstructionData.m_Height);
				break;
			}
		}
	}
}