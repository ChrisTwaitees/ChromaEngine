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
	std::vector<IChromaComponent*> Components;
	std::vector<IChromaComponent*> RenderableComponents;
	std::vector<IChromaComponent*> LitComponents;
	std::vector<IChromaComponent*> ShadowCastingComponents;
	std::vector<IChromaComponent*> TransparentComponents;
	std::vector<IChromaComponent*> UnLitComponents;

	std::vector<IChromaComponent*> m_meshComponents;
	std::vector<IChromaComponent*> m_physicsComponents;

	void addEmptyComponent(IChromaComponent*& newComponent);
	void addMeshComponent(ChromaMeshComponent*& newMeshComponent);
	void addPhysicsComponent(ChromaPhysicsComponent*& newPhysicsComponent);
	void removeEmptyComponent(IChromaComponent*& newComponent) {};
	void removeMeshComponent(ChromaMeshComponent*& newMeshComponent) {};
	void removePhysicsComponent(ChromaPhysicsComponent*& newPhysicsComponent) {};

	template<class ComponentClass>
	void bindParentEntity(ComponentClass component) {
		component->bindParentEntity(this);
	}

public:
	// Scene 
	ChromaSceneManager* getParentScene() override { return m_parentScene; };
	void bindParentScene(ChromaSceneManager* const& scene) override { m_parentScene = scene; };

	// components
	std::vector<IChromaComponent*> getComponents() { return Components; };
	std::vector<IChromaComponent*> getRenderableComponents() { return RenderableComponents; };
	std::vector<IChromaComponent*> getLitComponents() { return LitComponents; };
	std::vector<IChromaComponent*> getShadowCastingComponents() { return ShadowCastingComponents; };
	std::vector<IChromaComponent*> getTransparentComponents() { return TransparentComponents; };
	std::vector<IChromaComponent*> getUnlitComponents() { return UnLitComponents; };

	std::vector<IChromaComponent*> getMeshComponents() { return m_meshComponents; };
	std::vector<IChromaComponent*> getPhysicsComponents() { return m_physicsComponents; };


	// name
	std::string getUID() { return m_uid.UID; };
	std::string getName() { return m_name; };
	void setName(std::string newName) { m_name = newName; };

	// Components
	// add

	void addComponent(IChromaComponent* newComponent) override {};

	template<typename component>
	void addComponent(component newComponent)  {
		std::cout << "ComponentType not supported" << std::endl;
	};

	template<>
	void addComponent<IChromaComponent*>(IChromaComponent* newComponent )  {
		addEmptyComponent(newComponent);
	};

	template<>
	void addComponent<ChromaMeshComponent*>(ChromaMeshComponent* newComponent)  {
		addMeshComponent(newComponent);
	};

	template<>
	void addComponent<ChromaPhysicsComponent*>(ChromaPhysicsComponent* newComponent) {
		addPhysicsComponent(newComponent);
	};

	// remove
	void removeComponent(IChromaComponent*& removeMe);

	template<typename component>
	void removeComponent(component newComponent) {
		std::cout << "ComponentType not supported" << std::endl;
	};

	template<>
	void removeComponent<IChromaComponent*>(IChromaComponent* newComponent) {
		removeEmptyComponent(newComponent);
	};

	template<>
	void removeComponent<ChromaMeshComponent*>(ChromaMeshComponent* newComponent) {
		removeMeshComponent(newComponent);
	};

	template<>
	void removeComponent<ChromaPhysicsComponent*>(ChromaPhysicsComponent* newComponent) {
		removePhysicsComponent(newComponent);
	};


	// Transformations
	// set
	virtual void scale(glm::vec3 scalefactor) override;
	virtual void translate(glm::vec3 translatefactor) override;
	virtual void rotate(float degrees, glm::vec3 rotationaxis) override;

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