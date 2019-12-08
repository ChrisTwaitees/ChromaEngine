#include "ChromaPhysicsComponent.h"
#include <entity/IChromaEntity.h>
#include <math/ChromaMath.h>


void ChromaPhysicsComponent::buildRigidBody()
{
	// ran when component added to entity
	createCollisionShape();
	// create m_rigidbody object
	createRigidBody();
	// set whether object is dynamic, static or kinematic
	setCollisionFlags();
}

void ChromaPhysicsComponent::setLinearVelocity(glm::vec3 const& velocity)
{
	m_rigidBody->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

const glm::vec3 ChromaPhysicsComponent::getLinearVelocity()
{
	return  BulletToGLM(m_rigidBody->getLinearVelocity());
}

void ChromaPhysicsComponent::setWorldTransform(glm::mat4 const& transform)
{
	m_motionState->setWorldTransform(GLMToBullet(transform));
}

glm::mat4 ChromaPhysicsComponent::getWorldTransform()
{
	btTransform transform;
	m_motionState->getWorldTransform(transform);
	return BulletToGLM(transform);

}


void ChromaPhysicsComponent::transformParentEntity(btTransform& transform)
{
	glm::mat4 transformMat = BulletToGLM(transform);
	getParentEntity()->setTransformMatrix(transformMat);
}


void ChromaPhysicsComponent::createCollisionShape()
{
	switch (m_collisionShape)
	{
	case(AABB):
	{
		std::pair<glm::vec3, glm::vec3> bbox = getParentEntity()->getBBox();
		glm::vec3 boxSize = glm::abs(bbox.second);
		m_shape = new btBoxShape(GLMToBullet(boxSize));
		break;
	}
	case(Sphere) :
	{
		std::pair<glm::vec3, glm::vec3> bbox = getParentEntity()->getBBox();
		float boxSize = glm::length(bbox.first - bbox.second);
		m_shape = new btSphereShape(btScalar(boxSize) * 0.50);
		break;
	}
	case(Convex):
	{
		std::vector<ChromaVertex> m_vertices = m_parentEntity->getVertices();
		m_shape = new btConvexHullShape();
		for (ChromaVertex vert : m_vertices)
		{
			btVector3 btv = btVector3(vert.getPosition().x,
				vert.getPosition().y,
				vert.getPosition().z);
			((btConvexHullShape*)m_shape)->addPoint(btv);
		}
		break;
	}
	case(Mesh):
	{
		std::vector<ChromaVertex> m_vertices = m_parentEntity->getVertices();
		btTriangleMesh* mesh = new btTriangleMesh();

		for (int i = 0; i < m_vertices.size(); i += 3)
		{
			ChromaVertex v1 = m_vertices[i];
			ChromaVertex v2 = m_vertices[i + 1];
			ChromaVertex v3 = m_vertices[i + 2];

			btVector3 btv1 = btVector3(v1.getPosition().x,
				v1.getPosition().y,
				v1.getPosition().z);
			btVector3 btv2 = btVector3(v2.getPosition().x,
				v2.getPosition().y,
				v2.getPosition().z);
			btVector3 btv3 = btVector3(v3.getPosition().x,
				v3.getPosition().y,
				v3.getPosition().z);

			mesh->addTriangle(btv1, btv2, btv3);
		}
		m_shape = new btBvhTriangleMeshShape(mesh, true);
		break;
	}
	case(Capsule):
	{
		std::pair<glm::vec3, glm::vec3> bbox = getParentEntity()->getBBox();
		float boxheight = glm::length(bbox.first.y - bbox.second.y);
		bbox.first.y = 0;
		bbox.second.y = 0;
		float boxwidth = glm::length(bbox.first - bbox.second);
		// constructor : (radius, height)
		m_shape = new btCapsuleShape(boxwidth, boxheight);
		break;
	}
	case(Box):
	{
		std::pair<glm::vec3, glm::vec3> bbox = getParentEntity()->getBBox();
		glm::vec3 boxSize = glm::abs(bbox.second);
		m_shape = new btBoxShape(GLMToBullet(boxSize));
		break;
	}
	}

}

void ChromaPhysicsComponent::createRigidBody()
{
	// fetch entity position
	m_transform = GLMToBullet(getParentEntity()->getTransformationMatrix());

	// default motion state
	m_motionState = new btDefaultMotionState(m_transform);

	// mass
	btScalar bodyMass = m_mass;
	btVector3 bodyIntertia;
	m_shape->calculateLocalInertia(bodyMass, bodyIntertia);

	// rigid body cconstruction info
	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(bodyMass, m_motionState, m_shape, bodyIntertia);

	// setting friction and restitutino
	bodyCI.m_restitution = m_restitution;
	bodyCI.m_friction = m_friction;

	// build rigid
	m_rigidBody = new btRigidBody(bodyCI);

	// passing reference pointer to parent object
	m_rigidBody->setUserPointer(this);
}

void ChromaPhysicsComponent::setCollisionFlags()
{
	// static objects have a mass of 0
	// kinematic objects have a mass 0 but can be moved by code
	// dynamic objects have a mass non-zero transforms are controlled by bullet
	if (m_mass == 0 && m_collisionState == Static)
		m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	else if (m_mass == 0 && m_collisionState == Kinematic)
	{
		m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
	}
	else if (m_mass > 0)
		setCollisionState(Dynamic);
}


ChromaPhysicsComponent::ChromaPhysicsComponent()
{
}


ChromaPhysicsComponent::~ChromaPhysicsComponent()
{
	delete m_rigidBody->getMotionState();
	delete m_rigidBody;
	delete m_shape;

}
