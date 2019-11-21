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

class ChromaPhysicsComponent :	public IChromaComponent
{
	// attrs
	float m_mass{ 1.0f };
	// collision shape
	ColliderShape m_collisionShape = ColliderShape::AABB;

	// transforms
	btTransform m_transform;

	// physical settings
	float m_restitution{ 1.0f };
	float m_friction{ 0.5f };

	// members
	btRigidBody* m_body;
	btCollisionShape* m_shape;

	// functions
	void createBody();
	void createBodyWithMass();

public:
	// get/set
	float getMass() { return m_mass; };
	void setMass(float newMass) { m_mass = newMass; };

	float getRestitution() { return m_restitution; };
	void setRestitution(float newRestitution) { m_restitution = newRestitution; };

	float getFriction() { return m_friction; };
	void setFriction(float newFriction) { m_friction = newFriction; }

	void setLinearVelocity(glm::vec3 velocity);
	const glm::vec3 getLinearVelocity();

	void setCollisionShape(ColliderShape shape) { m_collisionShape = shape; };
	ColliderShape getColliderShape() { return m_collisionShape; };
	
	btRigidBody* getRigidBody() { return m_body; };

	// functions
	void buildRigidBody();
	void transformEntity(btTransform& transform);

	ChromaPhysicsComponent();
	~ChromaPhysicsComponent();
};

#endif