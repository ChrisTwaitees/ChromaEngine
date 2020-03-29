#include "PhysicsComponent.h"
#include <physics/PhysicsEngine.h>
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

std::vector<CollisionData> PhysicsComponent::GetRigidBodyCollisionData()
{
	return Chroma::Physics::GetRigidBodyCollisionData(m_RigidBody);
}

const glm::vec3 PhysicsComponent::GetLinearVelocity() const
{
	return  BulletToGLM(m_RigidBody->getLinearVelocity());
}

void PhysicsComponent::SetWorldTransform(glm::mat4 const& transform)
{
	m_MotionState->setWorldTransform(GLMToBullet(m_LocalTransform * transform));
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

		// transform
		break;
	}
	case(Sphere) :
	{
		std::pair<glm::vec3, glm::vec3> bbox = GetParentEntity()->GetBBox();
		float sphereSize = glm::length(bbox.first - bbox.second) / 2.0;
		m_CollisionShape = new btSphereShape(btScalar(sphereSize));

		// transform
		break;
	}
	case(Convex):
	{
		std::vector<ChromaVertex> m_vertices = GetParentEntity()->GetVertices();
		m_CollisionShape = new btConvexHullShape();
		for (ChromaVertex vert : m_vertices)
		{
			btVector3 btv = btVector3(vert.m_position.x,
				vert.m_position.y,
				vert.m_position.z);
			((btConvexHullShape*)m_CollisionShape)->addPoint(btv);
		}

		// transform
		break;
	}
	case(Mesh):
	{
		std::vector<ChromaVertex> m_vertices = GetParentEntity()->GetVertices();
		btTriangleMesh* mesh = new btTriangleMesh();

		for (int i = 0; i < m_vertices.size(); i += 3)
		{
			ChromaVertex v1 = m_vertices[i];
			ChromaVertex v2 = m_vertices[i + 1];
			ChromaVertex v3 = m_vertices[i + 2];

			btVector3 btv1 = btVector3(v1.m_position.x,
				v1.m_position.y,
				v1.m_position.z);
			btVector3 btv2 = btVector3(v2.m_position.x,
				v2.m_position.y,
				v2.m_position.z);
			btVector3 btv3 = btVector3(v3.m_position.x,
				v3.m_position.y,
				v3.m_position.z);

			mesh->addTriangle(btv1, btv2, btv3);
		}
		m_CollisionShape = new btBvhTriangleMeshShape(mesh, true);

		// transform
		break;
	}
	case(Capsule):
	{
		// Get Entity Dimensions
		std::pair<glm::vec3, glm::vec3> bbox = GetParentEntity()->GetBBox();
		float entityHeight = GetParentEntity()->GetHeight();

		// Calculate height and width
		float capsuleHeight = entityHeight / 2.0; // divide by 2 for radius
		bbox.first.y = 0;
		bbox.second.y = 0;
		float capsuleWidth = glm::length(bbox.first - bbox.second) / 4.0; // divide by 2 for radius

		// constructor : (radius, height)
		m_CollisionShape = new btCapsuleShape(capsuleWidth, capsuleHeight);

		// transform
		m_LocalTransform = glm::translate(m_LocalTransform, glm::vec3(0 , entityHeight / 2.0, 0));
		break;
	}
	case(Box):
	{
		std::pair<glm::vec3, glm::vec3> bbox = GetParentEntity()->GetBBox();
		glm::vec3 boxSize = glm::abs(bbox.second);
		m_CollisionShape = new btBoxShape(GLMToBullet(boxSize));

		// transform
		break;
	}
	}

}

void PhysicsComponent::CreateRigidBody()
{
	// create btTransform
	m_InitTransform = m_LocalTransform * GetParentEntity()->GetTransform();
	m_ColliderTransform = GLMToBullet(m_InitTransform);

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

void PhysicsComponent::Init()
{
	// build rigidBody
	BuildRigidBody();

   // add rigid body to physics world
	Chroma::Physics::AddBodyToWorld(m_RigidBody);
	CHROMA_TRACE("PhysicsComponent : {0} Initialized.", m_UID.data);
}

void PhysicsComponent::Update()
{
}

void PhysicsComponent::Destroy()
{
	CHROMA_TRACE("PhysicsComponent : {0} Destroyed.", m_UID.data);
	Chroma::Physics::RemoveBodyFromWorld(m_RigidBody);
	delete m_RigidBody;
	delete m_CollisionShape;
	delete m_MotionState;
}

void PhysicsComponent::Serialize(ISerializer*& serializer)
{
	CHROMA_INFO("Serializing Physics Component : {0}", m_UID.data);
	serializer->StartObject("PhysicsComponent", m_UID);
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
