#include "ChromaPhysicsComponent.h"
#include <entity/IChromaEntity.h>


void ChromaPhysicsComponent::buildRigidBody()
{
	createBody();
	createBodyWithMass();
}


void ChromaPhysicsComponent::createBody()
{
	std::vector<ChromaVertex> vertices = m_parentEntity->getVertices();

	if (m_convex)
	{
		m_shape = new btConvexHullShape();
		for (ChromaVertex vert : vertices)
		{
			btVector3 btv = btVector3(vert.getPosition().x,
				vert.getPosition().y,
				vert.getPosition().z);
			((btConvexHullShape*)m_shape)->addPoint(btv);
		}
	}
	else
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
	}

}

void ChromaPhysicsComponent::createBodyWithMass()
{
	// set rotation
	m_rotationQuat.setEulerZYX(m_rotation.z, m_rotation.y, m_rotation.x);

	// set position
	btVector3 btposition = btVector3(m_position.x, m_position.y, m_position.z);

	// default motion state
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(m_rotationQuat, btposition));

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
