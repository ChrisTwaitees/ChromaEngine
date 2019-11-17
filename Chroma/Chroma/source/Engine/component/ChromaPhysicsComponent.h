#ifndef _CHROMA_PHYSICS_COMPONENT_
#define _CHROMA_PHYSICS_COMPONENT_

// bullet
#include <bullet/btBulletDynamicsCommon.h>

// chroma
#include <component/IChromaComponent.h>
#include <time/ChromaTime.h>
#include <model/Vertex.h>

class ChromaPhysicsComponent :	public IChromaComponent
{
	// attrs
	float m_mass{ 1.0f };
	bool m_convex{ true };

	// transforms
	btQuaternion m_rotationQuat;
	glm::vec3 m_rotation;
	glm::vec3 m_position;

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

	btRigidBody* getRigidBody() { return m_body; };

	// functions
	void buildRigidBody();
	void setLinearVelocity(glm::vec3 velocity);
	void transformEntity(btTransform& transform);

	ChromaPhysicsComponent();
	~ChromaPhysicsComponent();
};

#endif