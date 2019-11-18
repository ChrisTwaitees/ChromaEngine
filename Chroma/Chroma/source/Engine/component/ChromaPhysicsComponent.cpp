#include "ChromaPhysicsComponent.h"
#include <entity/IChromaEntity.h>
#include <math/ChromaMath.h>


void ChromaPhysicsComponent::buildRigidBody()
{
	// ran when component added to entity
	createBody();
	createBodyWithMass();
}

void ChromaPhysicsComponent::setLinearVelocity(glm::vec3 velocity)
{
	m_body->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

const glm::vec3 ChromaPhysicsComponent::getLinearVelocity()
{
	return  BulletToGLM(m_body->getLinearVelocity());
}


void ChromaPhysicsComponent::transformEntity(btTransform& transform)
{
	glm::mat4 transformMat = BulletToGLM(transform);
	getParentEntity()->setTransformMatrix(transformMat);
}


void ChromaPhysicsComponent::createBody()
{
	std::vector<ChromaVertex> vertices = m_parentEntity->getVertices();

	switch (m_collisionShape)
	{
	case(AABB):
	{
		
		//m_shape = new btAA;
		break;
	}
	case(Sphere) :
	{
		// constructor : (radius)
		m_shape = new btSphereShape(btScalar(1.0f));
		break;
	}
	case(Convex):
	{
		std::cout << "adding convex shape to rigid body" << std::endl;
		m_shape = new btConvexHullShape();
		for (ChromaVertex vert : vertices)
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
		btTriangleMesh* mesh = new btTriangleMesh();

		for (int i = 0; i < vertices.size(); i += 3)
		{
			ChromaVertex v1 = vertices[i];
			ChromaVertex v2 = vertices[i + 1];
			ChromaVertex v3 = vertices[i + 2];

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
		// constructor : (radius, height)
		m_shape = new btCapsuleShape(btScalar(1.0f), btScalar(1.0f));
		break;
	}
	case(Box):
	{
		m_shape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
		break;
	}
	}

}

void ChromaPhysicsComponent::createBodyWithMass()
{
	// fetch entity position
	m_transform = GLMToBullet(getParentEntity()->getTransformationMatrix());

	// default motion state
	btDefaultMotionState* motionState = new btDefaultMotionState(m_transform);

	// mass
	btScalar bodyMass = m_mass;
	btVector3 bodyIntertia;
	m_shape->calculateLocalInertia(bodyMass, bodyIntertia);

	// rigid body cconstruction info
	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, m_shape, bodyIntertia);

	// setting friction and restitutino
	bodyCI.m_restitution = m_restitution;
	bodyCI.m_friction = m_friction;

	// build rigid
	m_body = new btRigidBody(bodyCI);

	// passing reference pointer to parent object
	m_body->setUserPointer(this);
}


ChromaPhysicsComponent::ChromaPhysicsComponent()
{
}


ChromaPhysicsComponent::~ChromaPhysicsComponent()
{
	delete m_body->getMotionState();
	delete m_body;
	delete m_shape;

}
