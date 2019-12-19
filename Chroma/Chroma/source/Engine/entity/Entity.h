#ifndef _CHROMA_ENTITY_
#define _CHROMA_ENTITY_

#include <vector>
#include <memory>

#include <entity/IEntity.h>

#include <model/Vertex.h>
#include <memory/ChromaUID.h>
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

	// Components
	void addEmptyComponent(IComponent*& newComponent) override;
	void addMeshComponent(MeshComponent*& newMeshComponent) override;
	void addPhysicsComponent(PhysicsComponent*& newPhysicsComponent) override;
	void addAnimationComponent(AnimationComponent*& newAnimationComponent) override;

	void removeEmptyComponent(IComponent*& newComponent) override;
	void removeMeshComponent(MeshComponent*& newMeshComponent) override {};
	void removePhysicsComponent(PhysicsComponent*& newPhysicsComponent) override {};
	void removeAnimationComponent(AnimationComponent*& oldAnimationComponent) override {};

	// Components transforms
	void updatePhysicsComponentsTransforms();

	template<class ComponentClass>
	void SetParentEntity(ComponentClass component) {
		component->SetParentEntity(this);
	}

public:
	// Scene 
	Scene* GetParentScene() override { return m_ParentScene; };
	void bindParentScene(Scene* const& scene) override { m_ParentScene = scene; };

	// components
	std::vector<IComponent*> getComponents() { return m_Components; };
	std::vector<IComponent*> getRenderableComponents() { return m_RenderableComponents; };
	std::vector<IComponent*> getLitComponents() { return m_LitComponents; };
	std::vector<IComponent*> getShadowCastingComponents() { return m_ShadowCastingComponents; };
	std::vector<IComponent*> getTransparentComponents() { return m_TransparentComponents; };
	std::vector<IComponent*> getUnlitComponents() { return m_UnLitComponents; };

	std::vector<IComponent*> getMeshComponents() { return m_MeshComponents; };
	std::vector<IComponent*> getPhysicsComponents() { return m_PhysicsComponents; };
	std::vector<IComponent*> getAnimationComponents() { return m_AnimationComponents; };

	// name
	ChromaUID GetUID() { return m_uid; };
	std::string GetName() { return m_Name; };
	void SetName(std::string newName) { m_Name = newName; };

	// Transformations
	// additive
	void scale(glm::vec3 scalefactor) override;
	void translate(glm::vec3 translatefactor) override;
	void rotate(float degrees, glm::vec3 rotationaxis) override;
	// set
	void setTransformMatrix(glm::mat4 newTransformMat) override { m_transformMatrix = newTransformMat; };
	void setScale(glm::vec3 newscale) override;
	void SetPosition(glm::vec3 newposition) override;
	void setRotation(float degrees, glm::vec3 rotationaxis) override;
	// get
	glm::mat4 GetTransformationMatrix() override { return m_transformMatrix; };
	virtual glm::vec3 GetPosition() override { return glm::vec3(m_transformMatrix[3]); } ;

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