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
#include <scene/ChromaSceneManager.h>


int findIndexInVector(const std::vector<IChromaComponent*>& vector, IChromaComponent*& element);

class ChromaEntity : public IChromaEntity
{
	// Parent Scene
	ChromaSceneManager* m_parentScene;


	// Components
	std::vector<IChromaComponent*> m_components;
	std::vector<IChromaComponent*> m_renderableComponents;
	std::vector<IChromaComponent*> m_litComponents;
	std::vector<IChromaComponent*> m_shadowCastingComponents;
	std::vector<IChromaComponent*> m_transparentComponents;
	std::vector<IChromaComponent*> m_unLitComponents;


	std::vector<IChromaComponent*> m_meshComponents;
	std::vector<IChromaComponent*> m_physicsComponents;

	// Components
	void addEmptyComponent(IChromaComponent*& newComponent) override;
	void addMeshComponent(ChromaMeshComponent*& newMeshComponent) override;
	void addPhysicsComponent(ChromaPhysicsComponent*& newPhysicsComponent) override;
	void removeEmptyComponent(IChromaComponent*& newComponent) override;
	void removeMeshComponent(ChromaMeshComponent*& newMeshComponent) override {};
	void removePhysicsComponent(ChromaPhysicsComponent*& newPhysicsComponent) override {};


	template<class ComponentClass>
	void bindParentEntity(ComponentClass component) {
		component->bindParentEntity(this);
	}

public:
	// Scene 
	ChromaSceneManager* getParentScene() override { return m_parentScene; };
	void bindParentScene(ChromaSceneManager* const& scene) override { m_parentScene = scene; };

	// components
	std::vector<IChromaComponent*> getComponents() { return m_components; };
	std::vector<IChromaComponent*> getRenderableComponents() { return m_renderableComponents; };
	std::vector<IChromaComponent*> getLitComponents() { return m_litComponents; };
	std::vector<IChromaComponent*> getShadowCastingComponents() { return m_shadowCastingComponents; };
	std::vector<IChromaComponent*> getTransparentComponents() { return m_transparentComponents; };
	std::vector<IChromaComponent*> getUnlitComponents() { return m_unLitComponents; };

	std::vector<IChromaComponent*> getMeshComponents() { return m_meshComponents; };
	std::vector<IChromaComponent*> getPhysicsComponents() { return m_physicsComponents; };


	// name
	ChromaUID getUID() { return m_uid; };
	std::string getName() { return m_name; };
	void setName(std::string newName) { m_name = newName; };


	// Transformations
	// additive
	virtual void scale(glm::vec3 scalefactor) override;
	virtual void translate(glm::vec3 translatefactor) override;
	virtual void rotate(float degrees, glm::vec3 rotationaxis) override;
	// set
	virtual void setScale(glm::vec3 newscale) override;
	virtual void setPosition(glm::vec3 newposition) override;

	// get
	glm::mat4 getTransformationMatrix() override { return m_transformMatrix; };
	virtual glm::vec3 getPosition() override { return glm::vec3(m_transformMatrix[3]); } ;

	// Attrs
	std::vector<ChromaVertex> getVertices() override;

	// Draw
	virtual void Draw(Shader& shader);
	virtual void Draw(Shader& shader, Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights);
	virtual void Draw(Camera& RenderCamera, std::vector < std::shared_ptr<Light>> Lights);

	ChromaEntity();
	~ChromaEntity();
};

#endif