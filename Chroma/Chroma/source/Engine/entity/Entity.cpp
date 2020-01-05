#include "Entity.h"
#include <component/MeshComponent.h>
#include <component/PhysicsComponent.h>
#include <component/AnimationComponent.h>
#include <component/CharacterControllerComponent.h>
#include <physics/PhysicsEngine.h>

std::vector<ChromaVertex> Entity::GetVertices()
{
	// collecting all vertices within mesh components of entity
	std::vector<ChromaVertex> verts;
	for (IComponent* meshComponent : m_MeshComponents)
	{
		std::vector<ChromaVertex> m_vertices = ((MeshComponent*)meshComponent)->GetVertices();
		for (ChromaVertex vert : m_vertices)
			verts.push_back(vert);
	}

	return verts;
}

std::pair<glm::vec3, glm::vec3> Entity::GetBBox()
{
	CalculateBBox();
	return std::make_pair(m_BBoxMin, m_BBoxMax);
}

glm::vec3 Entity::GetCentroid()
{
	CalculateCentroid();
	return m_Centroid;
}

void Entity::Draw(Shader& shader)
{
	for (IComponent* component : m_RenderableComponents)
		((MeshComponent*)component)->Draw(shader);
}

void Entity::Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*> Lights)
{
	for (IComponent* component : m_RenderableComponents)
		((MeshComponent*)component)->Draw(shader, RenderCamera, Lights, m_Transform);
}

void Entity::Draw(Camera& RenderCamera, std::vector<Light*> Lights)
{
	for (IComponent* component : m_RenderableComponents)
		((MeshComponent*)component)->Draw(RenderCamera, Lights, m_Transform);
}

// ADDING/REMOVING COMPONENTS
void Entity::addMeshComponent(MeshComponent*& newMeshComponent)
{
	// Prepare for Entity
	ProcessNewComponent(newMeshComponent);

	// add mesh component
	m_MeshComponents.push_back(newMeshComponent);

	// TODO: Consider std::map<UID, Component> for look up instead of duplication
	if (newMeshComponent->m_IsRenderable)
		m_RenderableComponents.push_back(newMeshComponent);
	if (newMeshComponent->m_IsLit && newMeshComponent->m_IsTransparent == false)
		m_LitComponents.push_back(newMeshComponent);
	if (newMeshComponent->m_CastShadows)
		m_ShadowCastingComponents.push_back(newMeshComponent);
	if (newMeshComponent->m_IsTransparent)
	{
		m_TransparentComponents.push_back(newMeshComponent);
		Chroma::Scene::AddTransparentEntity(this);
	}
	if (newMeshComponent->m_IsLit == false && newMeshComponent->m_IsTransparent == false)
		m_UnLitComponents.push_back(newMeshComponent);
	if (newMeshComponent->m_IsTransparent)
		m_TransparentComponents.push_back(newMeshComponent);
}

void Entity::addPhysicsComponent(PhysicsComponent*& newPhysicsComponent)
{
	// Prepare for Entity
	ProcessNewComponent(newPhysicsComponent);

	// add physics component
	m_PhysicsComponents.push_back(newPhysicsComponent);

	// build rigidBody
	newPhysicsComponent->BuildRigidBody();

	// add rigid body to physics world
	Chroma::Physics::AddBodyToWorld(newPhysicsComponent);
}

void Entity::addAnimationComponent(AnimationComponent*& newAnimationComponent)
{
	// Prepare for Entity
	ProcessNewComponent(newAnimationComponent);

	// add animation component
	m_AnimationComponents.push_back(newAnimationComponent);

	// add to updating components
	Chroma::Scene::AddUpdatingComponent(newAnimationComponent);
}

void Entity::addCharacterControllerComponent(CharacterControllerComponent*& newCharacterControllerComponent)
{
	// Prepare for Entity
	ProcessNewComponent(newCharacterControllerComponent);

	// add animation component
	m_CharacterControllerComponents.push_back(newCharacterControllerComponent);

	// add to updating components
	Chroma::Scene::AddUpdatingComponent(newCharacterControllerComponent);
}

void Entity::CalculateBBox()
{
	// collecting all bboxes within mesh components of entity and returning overall
	std::vector<std::pair<glm::vec3, glm::vec3>> bboxes;
	for (IComponent* meshComponent : m_MeshComponents)
		bboxes.push_back(((MeshComponent*)meshComponent)->GetBBox());
	// once collected, calculate new min and max bbox
	glm::vec3 newMinBBox(99999.00, 99999.00, 99999.00);
	glm::vec3 newMaxBBox(0.0, 0.0, 0.0);
	for (std::pair<glm::vec3, glm::vec3> MinMaxBBoxes : bboxes)
	{
		newMinBBox = glm::min(newMinBBox, MinMaxBBoxes.first);
		newMaxBBox = glm::max(newMaxBBox, MinMaxBBoxes.second);
	}
	// re-establishing min and max bboxes
	// scale by entity's current size
	glm::vec3 scale = getScale(m_Transform);
	m_BBoxMin = newMinBBox * scale;
	m_BBoxMax = newMaxBBox * scale;
}

void Entity::CalculateCentroid()
{
	m_Centroid = (m_BBoxMin - m_BBoxMax) * glm::vec3(0.5);
}

void Entity::addEmptyComponent(IComponent*& newComponent)
{
	// Prepare for Entity
	ProcessNewComponent(newComponent);

	// TODO: Consider shared_ptr to prevent memory duplication
	m_Components.push_back(newComponent);
}

void Entity::removeEmptyComponent(IComponent*& removeMe)
{
	// all components 
	int componentIndex = findIndexInVector(m_Components, removeMe);
	if (componentIndex > 0)
		m_Components.erase(m_Components.begin() + componentIndex);	
	// renderable components
	// TODO: Consider using shared_ptr to better manage memory
	componentIndex = findIndexInVector(m_RenderableComponents, removeMe);
	if (componentIndex)
		m_RenderableComponents.erase(m_RenderableComponents.begin() + componentIndex);
}

void Entity::UpdatePhysicsComponentsTransforms()
{
	for (IComponent* physicsComponent : m_PhysicsComponents)
	{
		if (((PhysicsComponent*)physicsComponent)->getColliderState() == Kinematic) // check if physics object is kinematic
			((PhysicsComponent*)physicsComponent)->SetWorldTransform(m_Transform);
	}
}

void Entity::ProcessNewComponent(IComponent* const& newComponent)
{
	// bind parent entity
	SetParentEntity(newComponent);
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

void Entity::SetTransform(glm::mat4 const& newTransformMat)
{
	m_Transform = newTransformMat;
	UpdatePhysicsComponentsTransforms();
}

void Entity::setScale(glm::vec3 const& newscale)
{
	m_Transform = glm::scale(m_Transform, newscale);
	UpdatePhysicsComponentsTransforms();
}

void Entity::SetPosition(glm::vec3 const& newposition)
{
	m_Transform[3] = glm::vec4(newposition, 1);
	UpdatePhysicsComponentsTransforms();
}

void Entity::SetRotation(float const& degrees, glm::vec3 const& rotationaxis)
{
	glm::vec3 existingTranslation = GetPosition();
	m_Transform = glm::translate(m_Transform, existingTranslation);
	Rotate(degrees, rotationaxis);
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