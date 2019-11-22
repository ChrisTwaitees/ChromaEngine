#ifndef _CHROMA_PHYSICS_COMPONENT_
#define _CHROMA_PHYSICS_COMPONENT_

// bullet
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

// chroma
#include <component/IChromaComponent.h>
#include <time/ChromaTime.h>
#include <model/Vertex.h>


enum ColliderShape { Box, Convex, Mesh, Capsule, Sphere, AABB };
enum ColliderState {Static, Kinematic, Dynamic};

class ChromaPhysicsComponent :	public IChromaComponent
{
	// attrs
	float m_mass{ 0.0f };
	// collision shape
	ColliderShape m_collisionShape = ColliderShape::AABB;
	ColliderState m_collisionState = ColliderState::Kinematic;

	// physical settings
	float m_restitution{ 1.0f };
	float m_friction{ 0.5f };

	// members
	btRigidBody* m_rigidBody;
	btCollisionShape* m_shape;
	btMotionState* m_motionState;
	btTransform m_transform;

	// functions
	void createCollisionShape();
	void createRigidBody();
	void setCollisionFlags();

public:
	// get/set
	float getMass() { return m_mass; };
	void setMass(float const& newMass) { m_mass = newMass; };

	float getRestitution() { return m_restitution; };
	void setRestitution(float const& newRestitution) { m_restitution = newRestitution; };

	float getFriction() { return m_friction; };
	void setFriction(float const& newFriction) { m_friction = newFriction; };
	// collisions
	void setCollisionShape(ColliderShape const& shape) { m_collisionShape = shape; };
	ColliderShape getColliderShape() { return m_collisionShape; };

	void setCollisionState(ColliderState const& state) { m_collisionState = state; };
	ColliderState getColliderState() { return m_collisionState; };
	
	btRigidBody* getRigidBody() { return m_rigidBody; };

	btMotionState* getMotionState() { return m_motionState; };
	// transforms
	void setLinearVelocity(glm::vec3 const& velocity);
	const glm::vec3 getLinearVelocity();

	void setWorldTransform(glm::mat4 const& transform);
	glm::mat4 getWorldTransform() ;

	// functions
	void buildRigidBody();
	void transformParentEntity(btTransform& transform);

	ChromaPhysicsComponent();
	~ChromaPhysicsComponent();
};

#endif