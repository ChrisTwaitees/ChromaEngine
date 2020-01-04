#include "ThirdPersonCharacterController.h"
#include <input/Input.h>
#include <entity/Entity.h>


void ThirdPersonCharacterController::CalculateVelocity()
{
	m_CurrentVelocity = m_Position - m_PreviousVelocity;
	m_PreviousVelocity = m_CurrentVelocity;
}

float ThirdPersonCharacterController::CalculateAimAngle()
{
	// use atan to calc angle camera is aimed at character
	// pythag to get 2D dist without Y
	float xa_xb = m_Position.x - m_CamPosition.x;
	float ya_yb = m_Position.y - m_CamPosition.y;

	float x = glm::sqrt(glm::pow(xa_xb, 2) + glm::pow(ya_yb, 2));

	// y dist simple
	float y = m_Position.y - m_CamPosition.y;

	return Chroma::Math::CartesianToPolar(x , y);
}

void ThirdPersonCharacterController::ProcessCameraInput()
{
	glm::vec3 toCam = glm::normalize(m_CamPosition - m_Position);
	// camera position
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
		//if(glm::length(m_CurrentVelocity) > 1.0)
		//	m_CamPosition += m_CurrentVelocity * m_CamDrag;
	}
	// camera rotate around plauer
	// vertical rotation
	glm::vec3 verticalAxis = glm::cross(toCam, CHROMA_UP);
	glm::quat verticalRotation = glm::angleAxis(Chroma::Input::GetControllerRightVertical() * (float)(m_CamRotationSpeed * Chroma::Time::GetDeltaTime()), verticalAxis);
	m_CamPosition = Chroma::Math::RotateAroundPivot(m_CamPosition, m_Position, verticalRotation);
	// horizontal rotation
	glm::vec3 horizontalAxis = glm::cross(toCam, verticalAxis);
	glm::quat horizontalRotation = glm::angleAxis((float)(Chroma::Input::GetControllerRightHorizontal() * m_CamRotationSpeed * Chroma::Time::GetDeltaTime()) , horizontalAxis);
	m_CamPosition = Chroma::Math::RotateAroundPivot(m_CamPosition, m_Position, horizontalRotation);

	// camera aim
	m_CamDirection = glm::normalize(m_Position - m_CamPosition);
	glm::vec3 ref = glm::cross(m_CamDirection, CHROMA_UP);
	m_CamUp = glm::cross(ref, m_CamDirection);


	//CHROMA_INFO(glm::degrees(CalculateAimAngle()));
	//CHROMA_INFO("{0}, {1}, {2}", m_CamPosition.x, m_CamPosition.y, m_CamPosition.z);
	//CHROMA_INFO("{0}, {1}, {2}", m_Position.x, m_Position.y, m_Position.z);
}

void ThirdPersonCharacterController::ProcessMovementInput()
{
	glm::vec3 toPlayer = glm::normalize(m_Position - m_CamPosition);
	glm::vec3 sidePlayer = glm::cross(toPlayer, CHROMA_UP);

	// left and right
	m_Position += sidePlayer * Chroma::Input::GetControllerLeftHorizontal() * m_MovementSpeed * glm::vec3((float)Chroma::Time::GetDeltaTime());

	// forward and back
	m_Position -= toPlayer * Chroma::Input::GetControllerLeftVertical() * m_MovementSpeed * glm::vec3((float)Chroma::Time::GetDeltaTime());

}

void ThirdPersonCharacterController::ProcessInput()
{

	ProcessCameraInput();
	ProcessMovementInput();
}

void ThirdPersonCharacterController::Update()
{
	// Calculate from Previous Frame
	CalculateVelocity();
	// Input
	ProcessInput();

	// Calculate and apply transform to parent entity
	CalculateTransform();
	m_ParentEntity->SetTransform(m_Transform);
	// Apply transforms to camera controller
	m_CameraController->m_CamPos = m_CamPosition;
	m_CameraController->m_CamDir = m_CamDirection;
	m_CameraController->m_CamUp = m_CamUp;
}
