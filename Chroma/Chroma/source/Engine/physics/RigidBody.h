#ifndef CHROMA_RIGIDBODY_H
#define CHROMA_RIGIDBODY_H

#include "Physics/PhysicsEngine.h"

namespace Chroma
{

	struct RBDConstructionData
	{
		float m_Radius{ 1.0f };
		glm::vec3 m_MinBBox{ 0.0f };
		glm::vec3 m_MaxBBox{ 1.0f };
		float m_Height{ 1.0f };
	};


	class RigidBody
	{
	public:
		void SetConstructionData(const RBDConstructionData& constructionData);
		void SetShape(ColliderShape newShape);
		void SetState(ColliderState newState);

		void SetTransform(const glm::mat4 newTransform);
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
		RigidBody(RBDConstructionData& constructionData)
			: m_ConstructionData(constructionData) {
			BuildRigidBody();
		}

		~RigidBody() { BuildRigidBody(); }

	protected:
		void BuildRigidBody();

		ColliderShape m_Shape{ ColliderShape::Box };
		ColliderState m_State{ ColliderState::Kinematic };

		glm::mat4 m_Transform{ 1.0f };

		btRigidBody* m_RigidbodyRaw{ nullptr };

		RBDConstructionData m_ConstructionData{ RBDConstructionData()};

		btCollisionShape* GenerateColliderShape();

	};
}

#endif //CHROMA_RIGIDBODY_H