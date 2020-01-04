#ifndef _CHROMA_ENTITY_
#define _CHROMA_ENTITY_

#include <vector>
#include <memory>

#include <entity/IEntity.h>

#include <model/Vertex.h>
#include <texture/Texture.h>
#include <shader/Shader.h>
#include <light/Light.h>
#include <camera/Camera.h>
#include <scene/Scene.h>


int findIndexInVector(const std::vector<IComponent*>& vector, IComponent*& element);

class Entity : public IEntity
{
	// calculate attrs
	void CalculateBBox();
	void CalculateCentroid();

	// Components
	std::vector<IComponent*> m_Components;
	std::vector<IComponent*> m_RenderableComponents;
	std::vector<IComponent*> m_LitComponents;
	std::vector<IComponent*> m_ShadowCastingComponents;
	std::vector<IComponent*> m_TransparentComponents;
	std::vector<IComponent*> m_UnLitComponents;

	std::vector<IComponent*> m_MeshComponents;
	std::vector<IComponent*> m_PhysicsComponents;
	std::vector<IComponent*> m_AnimationComponents;
	std::vector<IComponent*> m_CharacterControllerComponents;

	// Components
	void addEmptyComponent(IComponent*& newComponent) override;
	void addMeshComponent(MeshComponent*& newMeshComponent) override;
	void addPhysicsComponent(PhysicsComponent*& newPhysicsComponent) override;
	void addAnimationComponent(AnimationComponent*& newAnimationComponent) override;
	void addCharacterControllerComponent(CharacterControllerComponent*& newCharacterControllerComponent) override;

	void removeEmptyComponent(IComponent*& newComponent) override;
	void removeMeshComponent(MeshComponent*& newMeshComponent) override {};
	void removePhysicsComponent(PhysicsComponent*& newPhysicsComponent) override {};
	void removeAnimationComponent(AnimationComponent*& oldAnimationComponent) override {};
	void removeCharacterControllerComponent(CharacterControllerComponent*& newCharacterControllerComponent) override {};

	// Components functions
	void UpdatePhysicsComponentsTransforms();
	void ProcessNewComponent(IComponent* const& newComponent);

	template<class ComponentClass>
	void SetParentEntity(ComponentClass component) {
		component->SetParentEntity(this);
	}

public:
	// Scene 
	Scene* GetParentScene() override { return m_ParentScene; };
	void SetParentScene(Scene* const& scene) override { m_ParentScene = scene; };

	// components
	std::vector<IComponent*> getComponents() { return m_Components; };
	std::vector<IComponent*> getRenderableComponents() { return m_RenderableComponents; };
	std::vector<IComponent*> getLitComponents() { return m_LitComponents; };
	std::vector<IComponent*> getShadowCastingComponents() { return m_ShadowCastingComponents; };
	std::vector<IComponent*> GetTransparentComponents() { return m_TransparentComponents; };
	std::vector<IComponent*> getUnlitComponents() { return m_UnLitComponents; };

	std::vector<IComponent*> getMeshComponents() { return m_MeshComponents; };
	std::vector<IComponent*> getPhysicsComponents() { return m_PhysicsComponents; };
	std::vector<IComponent*> getAnimationComponents() { return m_AnimationComponents; };
	std::vector<IComponent*> getCharacterControllerComponents() { return m_CharacterControllerComponents; };

	// Transformations
	// additive
	void Scale(glm::vec3 scalefactor) override;
	void Translate(glm::vec3 translatefactor) override;
	void Rotate(float degrees, glm::vec3 rotationaxis) override;
	// set
	void SetTransform(glm::mat4 const& newTransformMat) override;
	void setScale(glm::vec3  const&  newscale) override;
	void SetPosition(glm::vec3 const&  newposition) override;
	void SetRotation(float const& degrees, glm::vec3 const& rotationaxis) override;

	// get
	glm::mat4 GetTransform() override { return m_Transform; };
	virtual glm::vec3 GetPosition() override { return glm::vec3(m_Transform[3]); } ;

	// attrs
	std::vector<ChromaVertex> GetVertices() override;
	std::pair<glm::vec3, glm::vec3> GetBBox() override;
	glm::vec3 GetCentroid() override;

	// Draw
	virtual void Draw(Shader& shader);
	virtual void Draw(Shader& shader, Camera& RenderCamera, std::vector<Light*> Lights);
	virtual void Draw(Camera& RenderCamera, std::vector<Light*> Lights);

	Entity();
	~Entity();
};

#endif