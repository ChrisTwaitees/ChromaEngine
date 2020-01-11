#ifndef _CHROMA_THIRDPERSON_CHARACTERCONTOLLER_H_
#define _CHROMA_THIRDPERSON_CHARACTERCONTOLLER_H_


// chroma
#include <component/CharacterControllerComponent.h>
#include <thirdperson/ThirdPersonCameraController.h>


class ThirdPersonCharacterController : public CharacterControllerComponent
{
	// Attrs
	float m_CamFacingAngle{ 0.0f };

	// Controls
	const float m_ControllerMin{ 0.22f };

	// Camera Attrs
	const float m_CamDrag{ 0.5f };
	const float m_CamRotationSpeed{ 2.0f };
	const float m_CamSensitivity{ 1.0f };
	
	const float m_FollowDistMin{ 10.0f };
	const float m_FollowDistMax{ 20.0f };

	float m_CamRotX{ 0.0f }, m_CamRotY{0.0f};
	float const m_CamRotMax{ 75.0f };

	// Movement
	glm::vec3 m_PlayerHeading{ 0.0f };

	const float m_MovementSpeed{ 10.0f };
	const float m_SprintSpeedMultiplier{ 2.5f };
	float m_CurrentSpeed{ 0.0f };

	glm::vec3 m_Velocity{ 0.0f };
	glm::vec3 m_PreviousPosition{ 0.0f };

	// Physics
	glm::vec3 m_Force{ 0.0f };

	const float m_JumpHeight{ 5.0f};
	const float m_JumpHeadingBias{ 1.5f };
	glm::vec3 m_JumpVectorStationary{ 0.0f, 1.0f, 0.0f };
	glm::vec3 m_JumpVector{ 0.0f, 1.0, 0.0f };

	float m_GravityStrength{ 1.0f };
	float m_GravityCurrent{ 0.0f };
	float m_GravityMax{ 9.81f };
	glm::vec3 m_GravityDirection{ 0.0f, -1.0f, 0.0f };

	// Collisions
	bool m_HitGround{ true };
	float m_CollisionCheckDist{0.5f };

	// Functions
	void ProcessCurrentFrame();
	void GroundCollisionCheck();
	void CalculateGravity();


	void ProcessInput() override;
	void ProcessCamera();
	void ProcessMovement();
	
public:

	void Update() override;

	ThirdPersonCharacterController() {};
	~ThirdPersonCharacterController() {};

};


#endif