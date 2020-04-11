#ifndef _CHROMA_THIRDPERSON_CHARACTERCONTOLLER_H_
#define _CHROMA_THIRDPERSON_CHARACTERCONTOLLER_H_


// chroma
#include <component/CharacterControllerComponent.h>
#include <thirdperson/ThirdPersonCameraController.h>


class ThirdPersonCharacterController : public CharacterControllerComponent
{
public:
	void Update() override;

	float& GetSprintSpeed() override { return m_SprintSpeedMultiplier; };
	float& GetWalkSpeed() override { return m_WalkSpeed; };

	ThirdPersonCharacterController() {};
	~ThirdPersonCharacterController() {};

private:

	// Attrs
	float m_CamFacingAngle{ 0.0f };

	// Controls
	const float m_InputMin{ 0.1f };

	// Camera Attrs
	glm::vec3 m_CamLookAtPos{ 0.0f };
	const float m_CamDrag{ 0.5f };
	const float m_CamRotationSpeed{ 2.0f };
	const float m_CamSensitivity{ 1.0f };
	
	const float m_FollowDistMin{ 30.0f };
	const float m_FollowDistMax{ 35.0f };

	float m_CamRotX{ 0.0f }, m_CamRotY{0.0f};
	float const m_CamRotMax{ 75.0f };

	// Movement
	glm::vec3 m_PlayerHeading{ glm::vec3(1.0f, 0.0f, 0.0f) };
	float m_WalkSpeed{ 10.0f };
	float m_SprintSpeedMultiplier{ 2.5f };
	float m_maxSlopeAngle{ 30.0f };

	// Physics
	// Speed
	float m_CurrentSpeed{ 0.0f };
	glm::vec3 m_PreviousPosition{ 0.0f };

	// Forces
	glm::vec3 m_GravityForce{ 0.0f };
	glm::vec3 m_CollisionForce{ 0.0f };
	glm::vec3 m_ControllerForce{ 0.0f };

	// Jumping
	const float m_JumpHeight{ 12.0f};
	const float m_JumpHeadingBias{ 0.5f };
	glm::vec3 m_JumpVectorStationary{ 0.0f, 1.0f, 0.0f };
	glm::vec3 m_JumpVector{ 0.0f, 1.0, 0.0f };

	// Gravity
	float m_GravityStrength{ 1.0f };
	float m_GravityCurrent{ 0.0f };
	float m_GravityMax{ 9.81f };
	glm::vec3 m_GravityDirection{ 0.0f, -1.0f, 0.0f };

	// Collisions
	float m_CollisionCheckDist{0.2f };

	// Functions
	void ProcessPreviousFrame();
	void ProcessCollisions() override;
	void CalculateGravity();


	void ProcessInput() override;
	void ProcessCamera();
	void ProcessMovement();
	void ProcessIKs();
	void ProcessJump();
	void ProcessTransforms();
	

};


#endif