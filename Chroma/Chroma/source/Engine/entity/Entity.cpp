#include "Entity.h"
#include <scene/Scene.h>
#include <component/MeshComponent.h>
#include <component/PhysicsComponent.h>
#include <component/AnimationComponent.h>
#include <component/CharacterControllerComponent.h>
#include <component/animation/AnimConstraintComponent.h>
#include <physics/PhysicsEngine.h>

std::vector<ChromaVertex> Entity::GetVertices()
{
	// collecting all vertices within mesh components of entity
	std::vector<ChromaVertex> verts;
	for (UID const& meshUID : m_MeshComponentUIDs)
	{
		std::vector<ChromaVertex> m_vertices = (static_cast<MeshComponent*>(Chroma::Scene::GetComponent(meshUID))->GetVertices());
		for (ChromaVertex vert : m_vertices)
			verts.push_back(vert);
	}

	return verts;
}

std::pair<glm::vec3, glm::vec3> Entity::GetBBox()
{
	return std::make_pair(m_BBoxMin * m_Scale, m_BBoxMax * m_Scale);
}

glm::vec3& Entity::GetCentroid()
{
	CalculateCentroid();
	return m_Centroid;
}

float Entity::GetHeight()
{
	std::pair<glm::vec3, glm::vec3> bbox = GetBBox();
	return glm::abs(bbox.first.y) + glm::abs(bbox.second.y);
}

void Entity::Draw(Shader& shader)
{
	for (UID const& uid : Chroma::Scene::GetRenderableComponentUIDs())
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->Draw(shader);
}

void Entity::Draw(Shader& shader, Camera& RenderCamera)
{
	for (UID const& uid : Chroma::Scene::GetRenderableComponentUIDs())
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->Draw(shader, RenderCamera);
}

void Entity::Draw(Camera& RenderCamera)
{
	for (UID const& uid : Chroma::Scene::GetRenderableComponentUIDs())
		((MeshComponent*)Chroma::Scene::GetComponent(uid))->Draw(RenderCamera);
}

// ADDING/REMOVING COMPONENTS
void Entity::AddMeshComponent(MeshComponent*& newMeshComponent)
{
	// Prepare for Entity
	ProcessNewComponent(newMeshComponent);

	// add to scene 
	Chroma::Scene::AddMeshComponent(newMeshComponent);

	// add mesh component
	m_MeshComponentUIDs.push_back(newMeshComponent->GetUID());

	// Add to global components list
	m_ComponentUIDs.push_back(newMeshComponent->GetUID());
}

void Entity::AddPhysicsComponent(PhysicsComponent*& newPhysicsComponent)
{
	// Prepare for Entity
	ProcessNewComponent(newPhysicsComponent);

	// add to scene 
	Chroma::Scene::AddPhysicsComponent(newPhysicsComponent);

	// add physics component
	m_PhysicsComponentUIDs.push_back(newPhysicsComponent->GetUID());

	// Add to global components list
	m_ComponentUIDs.push_back(newPhysicsComponent->GetUID());

}

void Entity::AddAnimationComponent(AnimationComponent*& newAnimationComponent)
{
	// Prepare for Entity
	ProcessNewComponent(newAnimationComponent);

	// add animation component
	m_AnimationComponentUIDs.push_back(newAnimationComponent->GetUID());

	// add to updating components
	Chroma::Scene::AddAnimationComponent(newAnimationComponent);

	// add to self to animated entities
	Chroma::Scene::AddAnimatedEntity(this);

	// Add to global components list
	m_ComponentUIDs.push_back(newAnimationComponent->GetUID());
}

void Entity::AddCharacterControllerComponent(CharacterControllerComponent*& newCharacterControllerComponent)
{
	// Prepare for Entity
	ProcessNewComponent(newCharacterControllerComponent);

	// add animation component
	m_CharacterControllerComponentUIDs.push_back(newCharacterControllerComponent->GetUID());

	// add to updating components
	Chroma::Scene::AddCharacterControllerComponent(newCharacterControllerComponent);

	// Add to global components list
	m_ComponentUIDs.push_back(newCharacterControllerComponent->GetUID());
}

void Entity::AddStateMachineComponent(StateMachineComponent*& newStateMachineComponent)
{
	// Prepare for Entity
	ProcessNewComponent(newStateMachineComponent);

	// add state machines component
	m_StateMachineComponentUIDs.push_back(newStateMachineComponent->GetUID());

	// add to updating components
	Chroma::Scene::AddStateMachineComponent(newStateMachineComponent);

	// Add to global components list
	m_ComponentUIDs.push_back(newStateMachineComponent->GetUID());
}

void Entity::AddAnimConstraintComponent(AnimConstraintComponent*& newIKComponent)
{
	// Prepare for Entity
	ProcessNewComponent(newIKComponent);

	// add state machines component
	m_AnimConstraintComponentUIDs.push_back(newIKComponent->GetUID());

	// add to updating components
	Chroma::Scene::AddAnimConstraintComponent(newIKComponent);

	// Add to global components list
	m_ComponentUIDs.push_back(newIKComponent->GetUID());
}

void Entity::CalculateBBox()
{
	// Calculate new min and max bbox from mesh components associated with entity

	glm::vec3 newMinBBox(99999.00, 99999.00, 99999.00);
	glm::vec3 newMaxBBox(0.0, 0.0, 0.0);

	for (UID const& meshComponentUID : m_MeshComponentUIDs)
	{
		std::pair<glm::vec3, glm::vec3> meshBBox = static_cast<MeshComponent*>(Chroma::Scene::GetComponent(meshComponentUID))->GetBBox();
		newMinBBox = glm::min(newMinBBox, meshBBox.first);
		newMaxBBox = glm::max(newMaxBBox, meshBBox.second);
	}

	// re-establishing min and max bboxes
	m_BBoxMin = newMinBBox;
	m_BBoxMax = newMaxBBox;
}

void Entity::CalculateCentroid()
{
	m_Centroid = (m_BBoxMin + m_BBoxMax) / glm::vec3(2.0);
}

void Entity::AddComponent(IComponent*& newComponent)
{
	// Prepare for Entity
	ProcessNewComponent(newComponent);

	m_ComponentUIDs.push_back(newComponent->GetUID());
}


void Entity::UpdatePhysicsComponentsTransforms()
{
	for (UID const& uid : m_PhysicsComponentUIDs)
	{
		PhysicsComponent* physicsComp = static_cast<PhysicsComponent*>(Chroma::Scene::GetComponent(uid));
		if (physicsComp->GetColliderState() == Kinematic)
		{
			physicsComp->SetWorldTransform(m_Transform);
		}
	}
}

void Entity::ProcessNewComponent(IComponent* const& newComponent)
{
	// bind parent entity
	SetParentEntityUID(newComponent);
}


// TRANSFORMATIONS
void Entity::Scale(glm::vec3 scalefactor)
{
	m_Transform = glm::scale(m_Transform, scalefactor);
	UpdatePhysicsComponentsTransforms();
}

void Entity::Translate(glm::vec3 translatefactor)
{
	m_Transform = glm::translate(m_Transform, translatefactor);
	UpdatePhysicsComponentsTransforms();
}

void Entity::Rotate(float degrees, glm::vec3 rotationaxis)
{
	m_Transform = glm::rotate(m_Transform, glm::radians(degrees), rotationaxis);
	UpdatePhysicsComponentsTransforms();
}

void Entity::Update()
{
#ifdef EDITOR
	CHROMA_TRACE("{0} Updating. UID : {0} Updating.", GetTypeName(),  m_UID.m_Data);
	RebuildTransform();
#endif
}

void Entity::Destroy()
{
	ENTITY_DESTROYED
}

void Entity::Serialize(ISerializer*& serializer)
{
	ENTITY_SERIALIZE_BEGIN
	// Name
	serializer->AddProperty("m_Name", &m_Name);

	// Transforms
	EditorProperty editoryPrpty(Chroma::Type::EditorProperty::kTransformProperty);
	editoryPrpty.m_Vec3MinMax = std::make_pair(glm::vec3(-20.0), glm::vec3(20.0));
	serializer->AddProperty("m_Transform",   &m_Transform);
	serializer->AddProperty("m_Translation", &m_Translation, editoryPrpty);
	serializer->AddProperty("m_Rotation",    &m_Rotation, editoryPrpty);
	editoryPrpty.m_Vec3MinMax = std::make_pair(glm::vec3(0.0), glm::vec3(1.0));
	serializer->AddProperty("m_Scale",       &m_Scale, editoryPrpty);

}


void Entity::Init()
{
	// Set Type
	m_Type =  Chroma::Type::Entity::kEntity;

	CalculateBBox();
	CalculateCentroid();
	ENTITY_INITIALIZED
}

void Entity::SetTransform(glm::mat4 const& newTransformMat)
{
	m_Transform = newTransformMat;
	m_Translation = Chroma::Math::GetTranslation(newTransformMat);
	m_Scale = Chroma::Math::GetScale(newTransformMat);
	m_Rotation = Chroma::Math::GetQuatRotation(newTransformMat);
	UpdatePhysicsComponentsTransforms();
}

void Entity::SetScale(glm::vec3 const& newscale)
{
	m_Scale = newscale;
	RebuildTransform();
	UpdatePhysicsComponentsTransforms();
}

void Entity::SetTranslation(glm::vec3 const& newposition)
{
	m_Translation = newposition;
	RebuildTransform();
	UpdatePhysicsComponentsTransforms();
}

void Entity::SetRotation(glm::quat const& newRotation)
{
	m_Rotation = newRotation;
	RebuildTransform();
	UpdatePhysicsComponentsTransforms();
}


// VECTOR FUNCTIONS
int findIndexInVector(const std::vector<IComponent*>& componentsVector, IComponent*& component)
{
	// Find given element in vector
	auto it = std::find(componentsVector.begin(), componentsVector.end(), component);

	if (it != componentsVector.end())
		return (int)distance(componentsVector.begin(), it);
	else
	{
		CHROMA_WARN("ChromaComponent: {0} not found in Chroma Entity", component->GetName());
		return -1;
	}
}


Entity::Entity()
{
}


Entity::~Entity()
{
}