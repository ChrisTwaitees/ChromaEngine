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
	Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, m_Position + m_Velocity, glm::vec3(1.0, 0.0, 1.0));

	// AIM ANGLE
	m_CamFacingAngle = Chroma::Math::GetFacingAngleEuler(m_Position, m_CamPosition);

	// COLLISIONS
	GroundCollisionCheck();

	// GRAVITY
	CalculateGravity();
}

void ThirdPersonCharacterController::GroundCollisionCheck()
{
	m_CollisionCheckDist = glm::length(GetParentEntity()->GetBBox().first) * 0.8;
	glm::vec3 checkPos = m_Position + (glm::normalize(m_GravityDirection) * m_CollisionCheckDist);
	m_HitGround = Chroma::Physics::RayTest(m_Position, checkPos);
	// debug
if (m_HitGround)
Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, checkPos, glm::vec3(1.0, 0.0, 0.0));
else
Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, checkPos, glm::vec3(0.0, 1.0, 0.0));
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
	if (glm::abs(Chroma::Math::GetFacingAngleEuler(m_Position, nextVerticalPosition)) < m_FacingAngleMax)
		m_CamPosition = nextVerticalPosition;

	// DIRECTION - Aim At Player
	m_CamDirection = glm::normalize(m_Position - m_CamPosition);
	// UP
	glm::vec3 ref = glm::cross(m_CamDirection, CHROMA_UP);
	m_CamUp = glm::cross(ref, m_CamDirection);
	//m_CamUp = CHROMA_UP;

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
	if (m_HitGround && Chroma::Input::IsPressed(Chroma::Input::CROSS))
	{
		CHROMA_INFO("Jump triggered!");
		float jumpIntertia = Chroma::Math::InertiaForHeight(m_GravityMax, m_JumpHeight) * DELTATIME;
		m_Force += m_JumpVector * glm::vec3(jumpIntertia);
	}

	// JumpVector
	glm::vec3 checkPos = m_Position + (glm::normalize(m_GravityDirection) * m_CollisionCheckDist);
	Chroma::Render::GetDebugBuffer()->DrawOverlayLine(checkPos, checkPos + (m_JumpVector * glm::vec3(m_JumpHeight)) + glm::vec3(0.1, 0.0, 0.0), glm::vec3(1.0, 1.0, 0.0));
}

void ThirdPersonCharacterController::ProcessMovement()
{
	glm::vec3 toPlayer = glm::normalize(m_Position - m_CamPosition);
	glm::vec3 sidePlayer = glm::normalize(glm::cross(toPlayer, CHROMA_UP));

	// removing y and normalizing
	toPlayer.y = 0.0f;
	sidePlayer.y = 0.0f;
	toPlayer = normalize(toPlayer);
	sidePlayer = normalize(sidePlayer);

	// Set Heading
	if (glm::abs(Chroma::Input::GetControllerLeftHorizontal()) > m_ControllerMin || glm::abs(Chroma::Input::GetControllerLeftVertical()) > m_ControllerMin)
	{
		glm::vec3 playerHeadingUnClamped = (sidePlayer * Chroma::Input::GetControllerLeftHorizontal()) + (toPlayer * -Chroma::Input::GetControllerLeftVertical());
		m_PlayerHeading = glm::normalize(playerHeadingUnClamped); 
		m_Position += m_PlayerHeading * glm::min(glm::length(playerHeadingUnClamped), 0.75f) * m_MovementSpeed * glm::vec3(DELTATIME);
	}

	// add force
	m_Position += m_Force;

	// prevent falling into the abyss
	m_Position.y = glm::max(m_Position.y, 1.5f);

	// player rotation
	float degreesRotated = Chroma::Math::DegreesBetweenVectors2D(CHROMA_FORWARD, m_PlayerHeading);
	m_Rotation = glm::angleAxis(-degreesRotated, CHROMA_UP);

	// DEBUG
	// Force
	Chroma::Render::GetDebugBuffer()->DrawOverlayLine(m_Position, m_Position + m_Force, glm::vec3(1.0, 0.0, 1.0));
	// Player Heading
	glm::vec3 checkPos = m_Position + (glm::normalize(m_GravityDirection) * m_CollisionCheckDist);
	Chroma::Render::GetDebugBuffer()->DrawOverlayLine(checkPos, checkPos + m_PlayerHeading, glm::vec3(0.1, 0.9, 0.8));
}

void ThirdPersonCharacterController::ProcessInput()
{
	ProcessCamera();
	ProcessMovement();
}

void ThirdPersonCharacterController::Update()
{
	// Calculate from Previous Frame
	ProcessCurrentFrame();
	// Input
	ProcessInput();
	// Calculate and apply transform to parent entity
	CalculateTransform();
	Chroma::Render::GetDebugBuffer()->DrawOverlayCoordinates(m_Transform);
	m_ParentEntity->SetTransform(m_Transform);
	// Apply transforms to camera controller
	m_CameraController->m_CamPos = m_CamPosition;
	m_CameraController->m_CamDir = m_CamDirection;
	m_CameraController->m_CamUp = m_CamUp;
}
