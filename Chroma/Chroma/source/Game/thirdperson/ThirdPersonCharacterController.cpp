#include "ThirdPersonCharacterController.h"
#include <input/Input.h>
#include <entity/Entity.h>

void ThirdPersonCharacterController::ProcessInput()
{
	if (Chroma::Input::IsPressed(Chroma::Input::SQUARE))
		m_Position.y += m_JumpStrength * Chroma::Time::GetDeltaTime();

}

void ThirdPersonCharacterController::Update()
{
	ProcessInput();
	CalculateTransform();
	m_ParentEntity->setTransformMatrix(m_Transform);
}
