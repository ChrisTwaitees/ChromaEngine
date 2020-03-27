#include "ThirdPersonCharacterController.h"
#include <input/Input.h>
#include <entity/Entity.h>
#include <render/Render.h>


void ThirdPersonCharacterController::ProcessCurrentFrame()
{
	// VELOCITY
	m_Velocity = m_Position - m_PreviousPosition;
	m_PreviousPosition = m_Position;
	m_CurrentSpeed = glm::length(m_Velocity);

	// debug
	//Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, m_Position + m_Velocity, glm::vec3(1.0, 0.0, 1.0));

	// AIM ANGLE
	m_CamFacingAngle = Chroma::Math::GetFacingAngleEuler(m_Position, m_CamPosition);

	// COLLISIONS
	GroundCollisionCheck();

	// GRAVITY
	CalculateGravity();
}

void ThirdPersonCharacterController::GroundCollisionCheck()
{
	//m_CollisionCheckDist = glm::length(GetParentEntity()->GetBBox().first) * 0.8;
	glm::vec3 rayStart = m_Position + ( - glm::normalize(m_GravityDirection) * 0.1f);
	glm::vec3 rayEnd = m_Position + (glm::normalize(m_GravityDirection) * m_CollisionCheckDist);
	m_HitGround = Chroma::Physics::RayTest(rayStart, rayEnd);
	// debug
	if (m_HitGround)
		Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, rayEnd, glm::vec3(1.0, 0.0, 0.0));
	else
		Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, rayEnd, glm::vec3(0.0, 1.0, 0.0));
}



void ThirdPersonCharacterController::ProcessCamera()
{
	glm::vec3 toCam = glm::normalize(m_CamPosition - m_Position);
	// POSITION
	// check if camera within radius 
	if (glm::length(m_Position - m_CamPosition) > m_FollowDistMax)
	{
		// pull back within follow radius
		m_CamPosition = m_Position + (toCam * m_FollowDistMax);
	}
	else if (glm::length(m_Position - m_CamPosition) < m_FollowDistMin)
	{
		// push back within follow radius
		m_CamPosition = m_Position + (toCam * m_FollowDistMin);
	}
	else // follow movement
	{
		m_CamPosition += m_Velocity * m_CamDrag;
	}
	// ROTATION
	// vertical rotation
	glm::vec3 horizontalAxis = CHROMA_UP;
	glm::quat horizontalRotation = glm::angleAxis((float)(Chroma::Input::GetControllerRightHorizontal() * m_CamRotationSpeed * Chroma::Time::GetDeltaTime()), horizontalAxis);
	m_CamPosition = Chroma::Math::RotateAroundPivot(m_CamPosition, m_Position, horizontalRotation);
	// horizontal rotation
	glm::vec3 verticalAxis = glm::normalize(glm::cross(toCam, horizontalAxis));
	glm::quat verticalRotation = glm::angleAxis((float)(Chroma::Input::GetControllerRightVertical() * m_CamRotationSpeed * Chroma::Time::GetDeltaTime()), verticalAxis);
	glm::vec3 nextVerticalPosition = Chroma::Math::RotateAroundPivot(m_CamPosition, m_Position, verticalRotation);
	if (glm::abs(Chroma::Math::GetFacingAngleEuler(m_Position, nextVerticalPosition)) < m_CamRotMax)
		m_CamPosition = nextVerticalPosition;

	// DIRECTION - Aim At Player
	m_CamLookAtPos = (m_Position + (GetParentEntity()->GetHeight() * 0.8f * m_GravityDirection));

	m_CamDirection = glm::normalize(m_Position - m_CamPosition);
	// UP
	glm::vec3 ref = glm::cross(m_CamDirection, CHROMA_UP);
	m_CamUp = glm::cross(ref, m_CamDirection);


}

void ThirdPersonCharacterController::CalculateGravity()
{
	// Apply Gravity
	if (!m_HitGround) // if in air, apply gravity
	{
		// using dot product check if gravity has reached max
		float gravitySpeed = (float)glm::max(glm::dot(m_GravityDirection, glm::normalize(m_Force)), 0.0f);
		gravitySpeed = gravitySpeed > 0.0f ? gravitySpeed * glm::length(m_Force) : 0.0f; // weird hack since float kept returning nan(-ind)		
		if (gravitySpeed < m_GravityMax)
		{
			m_GravityCurrent += 0.5f * m_GravityMax * glm::pow(DELTATIME, 2.0) * m_GravityStrength;
			m_Force += m_GravityDirection * glm::vec3(m_GravityCurrent);
		}
	}
	else
	{
		m_GravityCurrent = 0.0;
		m_Force = glm::vec3(0.0f);
	}

	// JUMPING
	// calculate jump vector
	glm::vec3 rotateJumpVectorAxis = glm::normalize(glm::cross(CHROMA_UP, m_PlayerHeading));
	glm::quat rotateJumpVector = glm::angleAxis(m_CurrentSpeed * m_JumpHeadingBias, rotateJumpVectorAxis);
	m_JumpVector = rotateJumpVector * m_JumpVectorStationary;

	// Jump : if on ground and button pressed
	if (m_HitGround && (Chroma::Input::IsPressed(Chroma::Input::CROSS) || Chroma::Input::IsPressed(Chroma::Input::SPACEBAR)))
	{
		CHROMA_INFO("Jump triggered!");
		float jumpIntertia = Chroma::Math::InertiaForHeight(m_GravityMax, m_JumpHeight) * DELTATIME;
		m_Force += m_JumpVector * glm::vec3(jumpIntertia);
	}

	// JumpVector
	Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, m_Position + (m_JumpVector * glm::vec3(m_JumpHeight)) + glm::vec3(0.1, 0.0, 0.0), glm::vec3(1.0, 1.0, 0.0));
}

void ThirdPersonCharacterController::ProcessMovement()
{
	glm::vec3 toPlayer = glm::normalize(m_Position - m_CamPosition);
	glm::vec3 sidePlayer = glm::normalize(glm::cross(toPlayer, CHROMA_UP));

	// removing y and normalizing
	toPlayer.y = 0.0f;
	sidePlayer.y = 0.0f;
	normalize(toPlayer);
	normalize(sidePlayer);

	// Set Heading
	if (glm::abs(Chroma::Input::GetAxis("Horizontal")) > m_ControllerMin || glm::abs(Chroma::Input::GetAxis("Vertical")) > m_ControllerMin)
	{
		glm::vec3 playerHeadingUnClamped = (sidePlayer * Chroma::Input::GetAxis("Horizontal")) + (toPlayer * -Chroma::Input::GetAxis("Vertical"));
		m_PlayerHeading = glm::normalize(playerHeadingUnClamped); 
		if (Chroma::Input::IsPressed(Chroma::Input::L3))
			m_Position += m_PlayerHeading * glm::min(glm::length(playerHeadingUnClamped), 0.75f) * m_WalkSpeed * m_SprintSpeedMultiplier * glm::vec3(DELTATIME);
		else
			m_Position += m_PlayerHeading * glm::min(glm::length(playerHeadingUnClamped), 0.75f) * m_WalkSpeed * glm::vec3(DELTATIME);
	}

	// add force
	m_Position += m_Force;

	// prevent falling into the abyss
	m_Position.y = glm::max(m_Position.y, .0f);
	//m_Position.y -= 1.05f;

	// player rotation
	float degreesRotated = Chroma::Math::DegreesBetweenVectors2D(CHROMA_FORWARD, m_PlayerHeading);
	m_Rotation = glm::angleAxis(-degreesRotated, CHROMA_UP);

	// DEBUG
	// Force
	Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, m_Position + m_Force, glm::vec3(1.0, 0.0, 1.0));
	// Player Heading
	Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, m_Position + m_PlayerHeading, glm::vec3(0.1, 0.9, 0.8));
}

void ThirdPersonCharacterController::ProcessTransforms()
{
	// Calculate and apply transform to parent entity
	CalculateTransform();
	GetParentEntity()->SetTransform(m_Transform);
	// Apply transforms to camera controller
	m_CameraController->m_CamPos = m_CamPosition;
	m_CameraController->m_CamDir = m_CamDirection;
	m_CameraController->m_CamUp = m_CamUp;
	// Debug
	Chroma::Render::GetDebugBuffer()->DrawOverlayCoordinates(m_Transform, 5.0f);
}

void ThirdPersonCharacterController::ProcessInput()
{
	// camera context switching
	// switch between editor cam and player cam
	if (Chroma::Input::IsPressed(Chroma::Input::R1) && Chroma::Input::IsPressed(Chroma::Input::L1)) 
	{
		CHROMA_WARN("Switching Camera Context...");
		Chroma::Input::ToggleCursorEnabledState();
		Chroma::Scene::GetRenderCamera()->SetCameraMode(CameraMode::Custom);
	}

	if (Chroma::Input::IsPressed(Chroma::Input::R3) && Chroma::Input::IsPressed(Chroma::Input::L3))
	{
		CHROMA_WARN("Toggling Physics Rendering");
		Chroma::Physics::ToggleDrawDebug();
	}
}

void ThirdPersonCharacterController::Update()
{
	// Calculate from Previous Frame
	ProcessCurrentFrame();

	// Input
	ProcessInput();

	// Movement
	ProcessMovement();

	// Camera
	ProcessCamera();

	// Transfroms
	ProcessTransforms();
}
