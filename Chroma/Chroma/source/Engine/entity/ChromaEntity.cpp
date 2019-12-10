#include "ChromaEntity.h"
#include <component/ChromaMeshComponent.h>
#include <component/ChromaPhysicsComponent.h>

std::vector<ChromaVertex> ChromaEntity::GetVertices()
{
	// collecting all vertices within mesh components of entity
	std::vector<ChromaVertex> verts;
	for (IChromaComponent* meshComponent : m_meshComponents)
	{
		std::vector<ChromaVertex> m_vertices = ((ChromaMeshComponent*)meshComponent)->GetVertices();
		for (ChromaVertex vert : m_vertices)
			verts.push_back(vert);
	}

	return verts;
}

std::pair<glm::vec3, glm::vec3> ChromaEntity::GetBBox()
{
	CalculateBBox();
	return std::make_pair(m_BBoxMin, m_BBoxMax);
}

glm::vec3 ChromaEntity::GetCentroid()
{
	CalculateCentroid();
	return m_Centroid;
}

void ChromaEntity::Draw(Shader& shader)
{
	for (IChromaComponent* component : m_renderableComponents)
		((ChromaMeshComponent*)component)->Draw(shader);
}

void ChromaEntity::Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*> Lights)
{
	for (IChromaComponent* component : m_renderableComponents)
		((ChromaMeshComponent*)component)->Draw(shader, RenderCamera, Lights, m_transformMatrix);
}

void ChromaEntity::Draw(Camera& RenderCamera, std::vector<Light*> Lights)
{
	for (IChromaComponent* component : m_renderableComponents)
		((ChromaMeshComponent*)component)->Draw(RenderCamera, Lights, m_transformMatrix);
}

ChromaEntity::ChromaEntity()
{
}


ChromaEntity::~ChromaEntity()
{
}

// ADDING/REMOVING COMPONENTS
void ChromaEntity::addMeshComponent(ChromaMeshComponent*& newMeshComponent)
{
	// bind parent entity
	SetParentEntity(newMeshComponent);

	// add mesh component
	m_meshComponents.push_back(newMeshComponent);

	// TODO: Consider shared_ptr to prevent memory duplication
	if (newMeshComponent->m_IsRenderable)
		m_renderableComponents.push_back(newMeshComponent);
	if (newMeshComponent->m_IsLit)
		m_litComponents.push_back(newMeshComponent);
	if (newMeshComponent->m_CastShadows)
		m_shadowCastingComponents.push_back(newMeshComponent);
	if (newMeshComponent->m_IsTransparent)
		m_transparentComponents.push_back(newMeshComponent);
	if (newMeshComponent->m_IsLit == false)
		m_unLitComponents.push_back(newMeshComponent);
	if (newMeshComponent->m_IsTransparent || newMeshComponent->m_IsLit == false)
		m_transparentComponents.push_back(newMeshComponent);
}

void ChromaEntity::addPhysicsComponent(ChromaPhysicsComponent*& newPhysicsComponent)
{
	// bind parent entity
	SetParentEntity(newPhysicsComponent);

	// add physics component
	m_physicsComponents.push_back(newPhysicsComponent);

	// build rigidBody
	newPhysicsComponent->BuildRigidBody();

	// add rigid body to physics world
	m_parentScene->getPhysics()->addBodyToWorld(newPhysicsComponent);
}

void ChromaEntity::CalculateBBox()
{
	// collecting all bboxes within mesh components of entity and returning overall
	std::vector<std::pair<glm::vec3, glm::vec3>> bboxes;
	for (IChromaComponent* meshComponent : m_meshComponents)
		bboxes.push_back(((ChromaMeshComponent*)meshComponent)->GetBBox());
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
	glm::vec3 scale = getScale(m_transformMatrix);
	m_BBoxMin = newMinBBox * scale;
	m_BBoxMax = newMaxBBox * scale;
}

void ChromaEntity::CalculateCentroid()
{
	m_Centroid = (m_BBoxMin - m_BBoxMax) * glm::vec3(0.5);
}

void ChromaEntity::addEmptyComponent(IChromaComponent*& newComponent)
{
	// bind parent entity
	SetParentEntity(newComponent);

	// TODO: Consider shared_ptr to prevent memory duplication
	m_components.push_back(newComponent);
}

void ChromaEntity::removeEmptyComponent(IChromaComponent*& removeMe)
{
	// all components 
	int componentIndex = findIndexInVector(m_components, removeMe);
	if (componentIndex > 0)
		m_components.erase(m_components.begin() + componentIndex);	
	// renderable components
	// TODO: Consider using shared_ptr to better manage memory
	componentIndex = findIndexInVector(m_renderableComponents, removeMe);
	if (componentIndex)
		m_renderableComponents.erase(m_renderableComponents.begin() + componentIndex);
}

void ChromaEntity::updatePhysicsComponentsTransforms()
{
	for (IChromaComponent* physicsComponent : m_physicsComponents)
	{
		if (((ChromaPhysicsComponent*)physicsComponent)->getColliderState() == Kinematic) // check if physics object is kinematic
			((ChromaPhysicsComponent*)physicsComponent)->SetWorldTransform(m_transformMatrix);
	}
}


// TRANSFORMATIONS
void ChromaEntity::scale(glm::vec3 scalefactor)
{
	m_transformMatrix = glm::scale(m_transformMatrix, scalefactor);
	updatePhysicsComponentsTransforms();
}

void ChromaEntity::translate(glm::vec3 translatefactor)
{
	m_transformMatrix = glm::translate(m_transformMatrix, translatefactor);
	updatePhysicsComponentsTransforms();
}

void ChromaEntity::rotate(float degrees, glm::vec3 rotationaxis)
{
	m_transformMatrix = glm::rotate(m_transformMatrix, glm::radians(degrees), rotationaxis);
	updatePhysicsComponentsTransforms();
}

void ChromaEntity::setScale(glm::vec3 newscale)
{
	m_transformMatrix = glm::scale(m_transformMatrix, newscale);
	updatePhysicsComponentsTransforms();
}

void ChromaEntity::setPosition(glm::vec3 newposition)
{
	m_transformMatrix[3] = glm::vec4(newposition, 1);
	updatePhysicsComponentsTransforms();
}

void ChromaEntity::setRotation(float degrees, glm::vec3 rotationaxis)
{
	glm::vec3 existingTranslation = GetPosition();
	m_transformMatrix = glm::translate(m_transformMatrix, existingTranslation);
	rotate(degrees, rotationaxis);
	updatePhysicsComponentsTransforms();
}


// VECTOR FUNCTIONS
int findIndexInVector(const std::vector<IChromaComponent*>& componentsVector, IChromaComponent*& component)
{
	// Find given element in vector
	auto it = std::find(componentsVector.begin(), componentsVector.end(), component);

	if (it != componentsVector.end())
		return distance(componentsVector.begin(), it);
	else
	{
		std::cout << "ChromaComponent: " << component->GetName() << "not found in Chroma Entity" << std::endl;
		return -1;
	}
}
