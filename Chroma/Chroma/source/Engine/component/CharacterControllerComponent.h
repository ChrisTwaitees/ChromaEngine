#ifndef _CHROMA_CHARACTERCONTROLLER_COMPONENT_H_
#define _CHROMA_CHARACTERCONTROLLER_COMPONENT_H_

// thirdparty
#include <glm/glm.hpp>

// chroma
#include <common/CoreCommon.h>
#include <component/IComponent.h>
#include <camera/ICameraController.h>
#include <physics/PhysicsEngine.h>


class CharacterControllerComponent : public IComponent
{
protected:
	// camera controller
	ICameraController* m_CameraController{ nullptr };

	// common attrs
	glm::vec3 m_Gravity{ 0.0, -9.81, 0.0 };
	float m_Speed{ 10.0 };
	float m_JumpHeight{ 2.0 };

	// transform
	glm::vec3 m_Position{ 0.0 };
	glm::quat m_Rotation{ glm::quat() };
	glm::vec4 m_Scale{ 1.0 };

	glm::mat4 m_Transform{ 1.0 };

	// functions
	virtual void ProcessInput();
	void CalculateTransform();

public:

	inline void SetCustomCameraController(ICameraController*& newCameraController) { m_CameraController = newCameraController; }
	virtual void Update() override;

	CharacterControllerComponent();
	virtual ~CharacterControllerComponent();
};

#endif