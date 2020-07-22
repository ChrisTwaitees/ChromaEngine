#include "RigidBody.h"


namespace Chroma
{
	void RigidBody::SetConstructionData(const RBDConstructionData& constructionData)
	{
		m_ConstructionData = constructionData;
		
		m_RigidbodyRaw->setCollisionShape(GenerateColliderShape());
		
	}


	void RigidBody::SetShape(ColliderShape newShape)
	{
		m_Shape = newShape;

		m_RigidbodyRaw->setCollisionShape(GenerateColliderShape());
	}

	void RigidBody::SetState(ColliderState newState)
	{
		switch (newState)
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

	void RigidBody::SetTransform(const glm::mat4 newTransform)
	{
		m_RigidbodyRaw->getMotionState()->setWorldTransform(GLMToBullet(m_Transform));
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
		SetState(m_State);

		// add to physics world
		AddToWorld();
	}

	btCollisionShape* RigidBody::GenerateColliderShape()
	{
		switch (m_Shape)
		{
			case Box :
			{
				glm::vec3 boxSize(m_ConstructionData.m_MinBBox - m_ConstructionData.m_MaxBBox);
				boxSize *= glm::vec3(0.5f);
				return new btBoxShape(GLMToBullet(-boxSize));

				break;
			}
			case Sphere :
			{
				float sphereSize = glm::length(m_ConstructionData.m_MinBBox - m_ConstructionData.m_MaxBBox) / 2.0;
				return new btSphereShape(btScalar(sphereSize));

				break;
			}
			case Cylinder :
			{
				// Calculate height and width
				glm::vec3 boxSize(m_ConstructionData.m_MinBBox - m_ConstructionData.m_MaxBBox);
				boxSize *= glm::vec3(0.5f);
				// constructor : (radius, height)
				return new btCylinderShape(GLMToBullet(boxSize));

				break;
			}
			case Capsule :
			{
				// Get Entity Dimensions
				float capsuleHeight = glm::length(m_ConstructionData.m_MinBBox - m_ConstructionData.m_MaxBBox);
				float capsuleWidth = glm::length(glm::length(m_ConstructionData.m_MinBBox - m_ConstructionData.m_MaxBBox)) / 4.0; // divide by 2 for radius

				// constructor : (radius, height)
				return new btCapsuleShape(capsuleWidth, capsuleHeight);

				break;
			}
		}
	}
}