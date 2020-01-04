#include "CharacterControllerComponent.h"
#include <input/Input.h>
#include <entity/Entity.h>


void CharacterControllerComponent::ProcessInput()
{
	
}

void CharacterControllerComponent::CalculateTransform()
{
	m_Transform = Chroma::Math::BuildMat4(m_Position, m_Rotation, m_Scale);
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
