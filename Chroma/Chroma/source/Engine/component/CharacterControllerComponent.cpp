#include "CharacterControllerComponent.h"
#include <input/Input.h>
#include <entity/Entity.h>


void CharacterControllerComponent::ProcessInput()
{
	
}

void CharacterControllerComponent::CalculateTransform()
{
	glm::mat4 ident(1.0);
	ident = glm::translate(ident, m_Position);
	ident = ident * glm::toMat4(m_Rotation);
	glm::scale(ident, m_Scale);
	m_Transform = ident;
}

void CharacterControllerComponent::Update()
{
	ProcessInput();
	CalculateTransform();
	m_ParentEntity->SetTransform(m_Transform);
}

CharacterControllerComponent::CharacterControllerComponent()
{
}


CharacterControllerComponent::~CharacterControllerComponent()
{
	delete m_CameraController;
}
