#ifndef _CHROMA_THIRDPERSON_CHARACTERCONTOLLER_H_
#define _CHROMA_THIRDPERSON_CHARACTERCONTOLLER_H_


// chroma
#include <component/CharacterControllerComponent.h>
#include <thirdperson/ThirdPersonCameraController.h>




class ThirdPersonCharacterController : public CharacterControllerComponent
{
	// Controls
	const float m_ControllerMin{ 0.02f };

	// Camera Attrs
	const float m_CamDrag{ 0.5f };
	const float m_CamRotationSpeed{ 50.0f };
	const float m_CamSensitivity{ 1.0f };
	
	const float m_FollowDistMin{ 3.0f };
	const float m_FollowDistMax{ 10.0f };

	const float m_FacingAngleMax{ 90.0f };

	// Movement
	const float m_MovementSpeed{ 10.0f };
	const float m_JumpHeight = 3.0f;

	glm::vec3 m_CurrentVelocity{ 0.0f };
	glm::vec3 m_PreviousVelocity{ 0.0f };

	float m_CurrentGravity{ 0.0f };
	glm::vec3 m_Gravity{ 0.0f, -9.81f, 0.0f };

	// Functions
	void CalculateVelocity();
	float CalculateAimAngle();
	void ProcessCameraInput();
	void ProcessMovementInput();
	void ProcessInput() override;
	
public:

	void Update() override;

	ThirdPersonCharacterController() {};
	~ThirdPersonCharacterController() {};

};


#endif