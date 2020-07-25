#ifndef CHROMA_RIGIDBODY_H
#define CHROMA_RIGIDBODY_H

#include "Physics/PhysicsEngine.h"

namespace Chroma
{

	struct RigidBodyConstructionData
	{
		float m_Radius{ 1.0f };
		float m_Height{ 1.0f };

		glm::vec3 m_MinBBox{ 0.0f };
		glm::vec3 m_MaxBBox{ 1.0f };
		glm::vec3 m_HalfExtents{ 1.0f };

		ColliderShape m_ColliderShape{ColliderShape::Box};
		ColliderState m_ColliderState{ColliderState::Kinematic};

		glm::mat4 m_LocalTransform{ 1.0f };

		bool operator!=(const RigidBodyConstructionData& rhs)
		{
			if (m_Radius != rhs.m_Radius) return true;
			if (m_Height != rhs.m_Height) return true;
			if (m_MinBBox != rhs.m_MinBBox) return true;
			if (m_MaxBBox != rhs.m_MaxBBox) return true;
			if (m_HalfExtents != rhs.m_HalfExtents) return true;
			if (m_ColliderShape != rhs.m_ColliderShape) return true;
			if (m_ColliderState != rhs.m_ColliderState) return true;
			if (m_LocalTransform != rhs.m_LocalTransform) return true;
		}
	};


	class RigidBody
	{
	public:
		void SetConstructionData(const RigidBodyConstructionData& constructionData);
		RigidBodyConstructionData& GetConstructionData() { return m_ConstructionData; }
		void SetShape(ColliderShape newShape);
		void SetState(ColliderState newState);

		void SetTransform(const glm::mat4& newTransform);
		glm::mat4 GetTransform();

		btRigidBody* GetRawRigid() const { return m_RigidbodyRaw; }
		void SetRawRigid(btRigidBody* newRigid);

		void RemoveFromWorld();
		void AddToWorld();

		template<class T>
		void SetUserPointer(T userPointer)
		{
			m_RigidbodyRaw->setUserPointer(userPointer);
		}

	public:
		RigidBody() { BuildRigidBody(); }
		RigidBody(RigidBodyConstructionData& constructionData)
			: m_ConstructionData(constructionData) {
			BuildRigidBody();
		}

		~RigidBody() { BuildRigidBody(); }

	protected:
		void BuildRigidBody();

		glm::mat4 m_Transform{ 1.0f };

		btRigidBody* m_RigidbodyRaw{ nullptr };

		RigidBodyConstructionData m_ConstructionData{ RigidBodyConstructionData()};

		btCollisionShape* GenerateColliderShape();

	};
}

#endif //CHROMA_RIGIDBODY_H