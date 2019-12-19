#ifndef _CHROMA_ENTITY_
#define _CHROMA_ENTITY_

#include <vector>
#include <memory>

#include <entity/IChromaEntity.h>

#include <model/Vertex.h>
#include <memory/ChromaUID.h>
#include <texture/Texture.h>
#include <shader/Shader.h>
#include <light/Light.h>
#include <camera/Camera.h>
#include <scene/ChromaScene.h>


int findIndexInVector(const std::vector<IChromaComponent*>& vector, IChromaComponent*& element);

class ChromaEntity : public IChromaEntity
{
	// calculate attrs
	void CalculateBBox();
	void CalculateCentroid();

	// Components
	std::vector<IChromaComponent*> m_Components;
	std::vector<IChromaComponent*> m_RenderableComponents;
	std::vector<IChromaComponent*> m_LitComponents;
	std::vector<IChromaComponent*> m_ShadowCastingComponents;
	std::vector<IChromaComponent*> m_TransparentComponents;
	std::vector<IChromaComponent*> m_UnLitComponents;

	std::vector<IChromaComponent*> m_MeshComponents;
	std::vector<IChromaComponent*> m_PhysicsComponents;
	std::vector<IChromaComponent*> m_AnimationComponents;

	// Components
	void addEmptyComponent(IChromaComponent*& newComponent) override;
	void addMeshComponent(ChromaMeshComponent*& newMeshComponent) override;
	void addPhysicsComponent(ChromaPhysicsComponent*& newPhysicsComponent) override;
	void addAnimationComponent(ChromaAnimationComponent*& newAnimationComponent) override;

	void removeEmptyComponent(IChromaComponent*& newComponent) override;
	void removeMeshComponent(ChromaMeshComponent*& newMeshComponent) override {};
	void removePhysicsComponent(ChromaPhysicsComponent*& newPhysicsComponent) override {};
	void removeAnimationComponent(ChromaAnimationComponent*& oldAnimationComponent) override {};

	// Components transforms
	void updatePhysicsComponentsTransforms();

	template<class ComponentClass>
	void SetParentEntity(ComponentClass component) {
		component->SetParentEntity(this);
	}

public:
	// Scene 
	ChromaScene* GetParentScene() override { return m_ParentScene; };
	void bindParentScene(ChromaScene* const& scene) override { m_ParentScene = scene; };

	// components
	std::vector<IChromaComponent*> getComponents() { return m_Components; };
	std::vector<IChromaComponent*> getRenderableComponents() { return m_RenderableComponents; };
	std::vector<IChromaComponent*> getLitComponents() { return m_LitComponents; };
	std::vector<IChromaComponent*> getShadowCastingComponents() { return m_ShadowCastingComponents; };
	std::vector<IChromaComponent*> getTransparentComponents() { return m_TransparentComponents; };
	std::vector<IChromaComponent*> getUnlitComponents() { return m_UnLitComponents; };

	std::vector<IChromaComponent*> getMeshComponents() { return m_MeshComponents; };
	std::vector<IChromaComponent*> getPhysicsComponents() { return m_PhysicsComponents; };
	std::vector<IChromaComponent*> getAnimationComponents() { return m_AnimationComponents; };

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

	ChromaEntity();
	~ChromaEntity();
};

#endif