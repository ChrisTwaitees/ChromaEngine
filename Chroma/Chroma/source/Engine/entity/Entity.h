#ifndef _CHROMA_ENTITY_
#define _CHROMA_ENTITY_

//common
#include <common/PrecompiledHeader.h>

#include <entity/IEntity.h>

#include <model/Vertex.h>
#include <texture/Texture.h>
#include <shader/Shader.h>
#include <light/Light.h>
#include <camera/Camera.h>
#include <scene/Scene.h>



namespace Chroma
{
	int findIndexInVector(const std::vector<IComponent*>& vector, IComponent*& element);
	class Entity : public IEntity
	{
	public:
		// Functions
		virtual void Init() override;
		virtual void OnUpdate() override;
		virtual void Destroy() override;
		virtual void Serialize(ISerializer*& serializer) override;

		// Transformations
		// setting
		void SetTransform(glm::mat4 const& newTransformMat);
		void SetScale(glm::vec3 const& newscale);
		void SetTranslation(glm::vec3 const& newposition);
		void SetRotation(glm::quat const& newRotation);

		// additive
		void Scale(glm::vec3 scalefactor) override;
		void Translate(glm::vec3 translatefactor) override;
		void Rotate(float degrees, glm::vec3 rotationaxis) override;

		// attrs
		std::vector<ChromaVertex> GetVertices() override;
		std::pair<glm::vec3, glm::vec3> GetBBox() override;
		glm::vec3& GetCentroid() override;
		float GetHeight() override;

		// Draw
		virtual void Draw(Shader& shader);
		virtual void Draw(Shader& shader, Camera& RenderCamera);
		virtual void Draw(Camera& RenderCamera);

		Entity();
		~Entity();

	private:
		// calculate attrs
		void CalculateBBox() override;
		void CalculateCentroid() override;

		// Components
		void AddComponent(IComponent*& newComponent) override;
		void AddMeshComponent(MeshComponent*& newMeshComponent) override;
		void AddPhysicsComponent(PhysicsComponent*& newPhysicsComponent) override;
		void AddAnimationComponent(AnimationComponent*& newAnimationComponent) override;
		void AddCharacterControllerComponent(CharacterControllerComponent*& newCharacterControllerComponent) override;
		void AddStateMachineComponent(StateMachineComponent*& newStateMachineComponent) override;
		void AddAnimConstraintComponent(AnimConstraintComponent*& newIKComponent) override;

		// Components functions
		void UpdatePhysicsComponentsTransforms();
		void ProcessNewComponent(IComponent* const& newComponent);

		template<class ComponentClass>
		void SetParentEntityUID(ComponentClass component) {
			component->SetParentEntityUID(m_UID);
		}

	};
}

#endif