#include "PhysicsComponent.h"
#include <entity/IEntity.h>
#include <math/Math.h>


void PhysicsComponent::BuildRigidBody()
{
	// ran when component added to entity
	CreateCollisionShape();
	// create m_rigidbody object
	CreateRigidBody();
	// set whether object is dynamic, static or kinematic
	SetCollisionFlags();
}

void PhysicsComponent::SetLinearVelocity(glm::vec3 const& velocity)
{
	m_RigidBody->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

const glm::vec3 PhysicsComponent::GetLinearVelocity() const
{
	return  BulletToGLM(m_RigidBody->getLinearVelocity());
}

void PhysicsComponent::SetWorldTransform(glm::mat4 const& transform)
{
	m_MotionState->setWorldTransform(GLMToBullet(transform));
}

glm::mat4 PhysicsComponent::GetWorldTransform() const
{
	btTransform transform;
	m_MotionState->getWorldTransform(transform);
	return BulletToGLM(transform);

}

void PhysicsComponent::Transform(btTransform& transform)
{
	glm::mat4 transformMat = BulletToGLM(transform);
	GetParentEntity()->SetTransform(transformMat);
}

void PhysicsComponent::CreateCollisionShape()
{
	switch (m_ColliderShape)
	{
	case(AABB):
	{
		std::pair<glm::vec3, glm::vec3> bbox = GetParentEntity()->GetBBox();
		glm::vec3 boxSize = glm::abs(bbox.second);
		m_CollisionShape = new btBoxShape(GLMToBullet(boxSize));
		break;
	}
	case(Sphere) :
	{
		std::pair<glm::vec3, glm::vec3> bbox = GetParentEntity()->GetBBox();
		float boxSize = glm::length(bbox.first - bbox.second);
		m_CollisionShape = new btSphereShape(btScalar(boxSize) * 0.50);
		break;
	}
	case(Convex):
	{
		std::vector<ChromaVertex> m_vertices = m_ParentEntity->GetVertices();
		m_CollisionShape = new btConvexHullShape();
		for (ChromaVertex vert : m_vertices)
		{
			btVector3 btv = btVector3(vert.GetPosition().x,
				vert.GetPosition().y,
				vert.GetPosition().z);
			((btConvexHullShape*)m_CollisionShape)->addPoint(btv);
		}
		break;
	}
	case(Mesh):
	{
		std::vector<ChromaVertex> m_vertices = m_ParentEntity->GetVertices();
		btTriangleMesh* mesh = new btTriangleMesh();

		for (int i = 0; i < m_vertices.size(); i += 3)
		{
			ChromaVertex v1 = m_vertices[i];
			ChromaVertex v2 = m_vertices[i + 1];
			ChromaVertex v3 = m_vertices[i + 2];

			btVector3 btv1 = btVector3(v1.GetPosition().x,
				v1.GetPosition().y,
				v1.GetPosition().z);
			btVector3 btv2 = btVector3(v2.GetPosition().x,
				v2.GetPosition().y,
				v2.GetPosition().z);
			btVector3 btv3 = btVector3(v3.GetPosition().x,
				v3.GetPosition().y,
				v3.GetPosition().z);

			mesh->addTriangle(btv1, btv2, btv3);
		}
		m_CollisionShape = new btBvhTriangleMeshShape(mesh, true);
		break;
	}
	case(Capsule):
	{
		std::pair<glm::vec3, glm::vec3> bbox = GetParentEntity()->GetBBox();
		float boxheight = glm::length(bbox.first.y - bbox.second.y);
		bbox.first.y = 0;
		bbox.second.y = 0;
		float boxwidth = glm::length(bbox.first - bbox.second);
		// constructor : (radius, height)
		m_CollisionShape = new btCapsuleShape(boxwidth, boxheight);
		break;
	}
	case(Box):
	{
		std::pair<glm::vec3, glm::vec3> bbox = GetParentEntity()->GetBBox();
		glm::vec3 boxSize = glm::abs(bbox.second);
		m_CollisionShape = new btBoxShape(GLMToBullet(boxSize));
		break;
	}
	}

}

void PhysicsComponent::CreateRigidBody()
{
	// fetch entity position
	m_ColliderTransform = GLMToBullet(GetParentEntity()->GetTransform());

	// default motion state
	m_MotionState = new btDefaultMotionState(m_ColliderTransform);

	// mass
	btScalar bodyMass = m_Mass;
	btVector3 bodyIntertia;
	m_CollisionShape->calculateLocalInertia(bodyMass, bodyIntertia);

	// rigid body cconstruction info
	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(bodyMass, m_MotionState, m_CollisionShape, bodyIntertia);

	// setting friction and restitutino
	bodyCI.m_restitution = m_Restitution;
	bodyCI.m_friction = m_Friction;

	// build rigid
	m_RigidBody = new btRigidBody(bodyCI);

	// passing reference pointer to parent object
	m_RigidBody->setUserPointer(this);
}

void PhysicsComponent::SetCollisionFlags()
{
	// static objects have a mass of 0
	// kinematic objects have a mass 0 but can be moved by code
	// dynamic objects have a mass non-zero transforms are controlled by bullet
	if (m_Mass == 0 && m_CollisionState == Static)
		m_RigidBody->setCollisionFlags(m_RigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	else if (m_Mass == 0 && m_CollisionState == Kinematic)
	{
		m_RigidBody->setCollisionFlags(m_RigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		m_RigidBody->setActivationState(DISABLE_DEACTIVATION);
	}
	else if (m_Mass > 0)
	{
		SetCollisionState(Dynamic);
		//m_RigidBody->setMotionState();
	}
}


PhysicsComponent::PhysicsComponent()
{
}


PhysicsComponent::~PhysicsComponent()
{
	delete m_RigidBody->getMotionState();
	delete m_RigidBody;
	delete m_CollisionShape;

}
