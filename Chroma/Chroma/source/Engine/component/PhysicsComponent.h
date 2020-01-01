#ifndef _CHROMA_PHYSICS_COMPONENT_
#define _CHROMA_PHYSICS_COMPONENT_

// bullet
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

// chroma
#include <component/IComponent.h>
#include <time/Time.h>
#include <model/Vertex.h>


enum ColliderShape { Box, Convex, Mesh, Capsule, Sphere, AABB };
enum ColliderState {Static, Kinematic, Dynamic};

class PhysicsComponent :	public IComponent
{
	// attrs
	float m_Mass{ 0.0f };
	// collision shape
	ColliderShape m_ColliderShape = ColliderShape::Box;
	ColliderState m_CollisionState = ColliderState::Kinematic;

	// physical settings
	float m_Restitution{ 1.0f };
	float m_Friction{ 0.5f };

	// members
	btRigidBody* m_RigidBody;
	btCollisionShape* m_CollisionShape;
	btMotionState* m_MotionState;
	btTransform m_ColliderTransform;

	// functions
	void CreateCollisionShape();
	void CreateRigidBody();
	void SetCollisionFlags();

public:

	void BuildRigidBody();
	void Transform(btTransform& transform);

	// Accessors
	float GetMass() const { return m_Mass; };
	void setMass(float const& newMass) { m_Mass = newMass; };

	float GetRestitution() const { return m_Restitution; };
	void SetRestitution(float const& newRestitution) { m_Restitution = newRestitution; };

	float GetFriction() const { return m_Friction; };
	void SetFriction(float const& newFriction) { m_Friction = newFriction; };
	// collisions
	ColliderShape GetColliderShape() const { return m_ColliderShape; };
	void SetColliderShape(ColliderShape const& shape) { m_ColliderShape = shape; };

	ColliderState getColliderState() const { return m_CollisionState; };
	void SetCollisionState(ColliderState const& state) { m_CollisionState = state; };
	
	btRigidBody* GetRigidBody() const { return m_RigidBody; };

	btMotionState* GetMotionState() const { return m_MotionState; };
	// transforms
	const glm::vec3 GetLinearVelocity() const;
	void SetLinearVelocity(glm::vec3 const& velocity);

	glm::mat4 GetWorldTransform() const;
	void SetWorldTransform(glm::mat4 const& transform);

	PhysicsComponent();
	~PhysicsComponent();
};

#endif