#include "ChromaEntity.h"
#include <component/ChromaMeshComponent.h>
#include <component/ChromaPhysicsComponent.h>

std::vector<ChromaVertex> ChromaEntity::getVertices()
{
	// collecting all vertices within mesh components of entity
	std::vector<ChromaVertex> verts;
	for (IChromaComponent* meshComponent : m_meshComponents)
	{
		std::vector<ChromaVertex> vertices = ((ChromaMeshComponent*)meshComponent)->getVertices();
		for (ChromaVertex vert : vertices)
			verts.push_back(vert);
	}

	return verts;
}

void ChromaEntity::Draw(Shader& shader)
{
	for (IChromaComponent* component : m_renderableComponents)
		((ChromaMeshComponent*)component)->Draw(shader);
}

void ChromaEntity::Draw(Shader& shader, Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights)
{
	for (IChromaComponent* component : m_renderableComponents)
		((ChromaMeshComponent*)component)->Draw(shader, RenderCamera, Lights, m_transformMatrix);
}

void ChromaEntity::Draw(Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights)
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
	bindParentEntity(newMeshComponent);

	// add mesh component
	m_meshComponents.push_back(newMeshComponent);

	// TODO: Consider shared_ptr to prevent memory duplication
	if (newMeshComponent->isRenderable)
		m_renderableComponents.push_back(newMeshComponent);
	if (newMeshComponent->isLit)
		m_litComponents.push_back(newMeshComponent);
	if (newMeshComponent->castShadows)
		m_shadowCastingComponents.push_back(newMeshComponent);
	if (newMeshComponent->isTransparent)
		m_transparentComponents.push_back(newMeshComponent);
	if (newMeshComponent->isLit == false)
		m_unLitComponents.push_back(newMeshComponent);
	if (newMeshComponent->isTransparent || newMeshComponent->isLit == false)
		m_transparentComponents.push_back(newMeshComponent);
}

void ChromaEntity::addPhysicsComponent(ChromaPhysicsComponent*& newPhysicsComponent)
{
	// bind parent entity
	bindParentEntity(newPhysicsComponent);

	// add physics component
	m_physicsComponents.push_back(newPhysicsComponent);

	// build rigidBody
	newPhysicsComponent->buildRigidBody();

	// add rigid body to physics world
	m_parentScene->getPhysics()->addBodyToWorld(newPhysicsComponent);
}

void ChromaEntity::addEmptyComponent(IChromaComponent*& newComponent)
{
	// bind parent entity
	bindParentEntity(newComponent);

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


// TRANSFORMATIONS
void ChromaEntity::scale(glm::vec3 scalefactor)
{
	m_transformMatrix = glm::scale(m_transformMatrix, scalefactor);
}

void ChromaEntity::translate(glm::vec3 translatefactor)
{
	m_transformMatrix = glm::translate(m_transformMatrix, translatefactor);
}

void ChromaEntity::rotate(float degrees, glm::vec3 rotationaxis)
{
	m_transformMatrix = glm::rotate(m_transformMatrix, glm::radians(degrees), rotationaxis);
}

void ChromaEntity::setScale(glm::vec3 newscale)
{
	m_transformMatrix = glm::scale(m_identityMatrix, newscale);
}

void ChromaEntity::setPosition(glm::vec3 newposition)
{
	m_transformMatrix = glm::translate(m_identityMatrix, newposition);
}

void ChromaEntity::setRotation(float degrees, glm::vec3 rotationaxis)
{
	glm::vec3 existingTranslation = getPosition();
	m_transformMatrix = glm::translate(m_identityMatrix, existingTranslation);
	rotate(degrees, rotationaxis);
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
		std::cout << "ChromaComponent: " << component->getName() << "not found in Chroma Entity" << std::endl;
		return -1;
	}
}
