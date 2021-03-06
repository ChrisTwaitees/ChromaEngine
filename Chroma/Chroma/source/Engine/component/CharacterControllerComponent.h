#ifndef _CHROMA_CHARACTERCONTROLLER_COMPONENT_H_
#define _CHROMA_CHARACTERCONTROLLER_COMPONENT_H_

//common
#include <common/PrecompiledHeader.h>

// chroma
#include <component/IComponent.h>
#include <camera/ICameraController.h>
#include <physics/PhysicsEngine.h>
#include <math/Math.h>


namespace Chroma
{
	class PhysicsComponent;

	class CharacterControllerComponent : public IComponent
	{
	public:
		// Functions
		void Init() override;
		void OnUpdate() override;
		void Destroy() override;
		void Serialize(ISerializer*& serializer) override;

		virtual inline void SetCustomCameraController(std::shared_ptr<ICameraController> newCameraController) { m_CameraController = newCameraController; }

		inline virtual glm::vec3& GetVelocity() { return m_Velocity; }
		inline virtual glm::vec3& GetPlayerPosition() { return m_Position; };
		inline virtual glm::vec3& GetCamPosition() { return m_CamPosition; };
		virtual float& GetSprintSpeed() { return m_SprintSpeed; };
		virtual float& GetWalkSpeed() { return m_WalkSpeed; };
		virtual bool& GetIsOnGround() { return m_IsOnGround; };
	
		CharacterControllerComponent();
		virtual ~CharacterControllerComponent();

	protected:
		// camera controller
		std::shared_ptr<ICameraController> m_CameraController{ nullptr };
		glm::vec3 m_CamPosition{ glm::vec3(10.0) };
		glm::vec3 m_CamDirection{ CHROMA_RIGHT };
		glm::vec3 m_CamUp{ CHROMA_UP };

		// transform
		glm::vec3 m_Position{ 0.0f };
		glm::quat m_Rotation{ glm::quat() };
		glm::vec3 m_Scale{ 1.0 };

		glm::mat4 m_Transform{ 1.0 };

		// physics
		PhysicsComponent* GetPhysicsComponent();
		bool m_IsOnGround{ true };
		float m_WalkSpeed;
		float m_SprintSpeed;
		bool m_HasDoubleJumped{ false };
		glm::vec3 m_Velocity{ 0.0f };

		// functions
		virtual void ProcessInput();
		virtual void ProcessCollisions();
		void CalculateTransform();
	};
}

#endif