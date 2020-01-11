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

void CharacterControllerComponent::Init()
{
	m_Scale = GetParentEntity()->GetScale();
	m_Position = GetParentEntity()->GetTranslation();
	CHROMA_TRACE("CharacterController Component : {0} Initialized.", m_UID.data);
}

void CharacterControllerComponent::Update()
{
	CHROMA_TRACE("CharacterController Component : {0} Updating.", m_UID.data);
	ProcessInput();
	CalculateTransform();
	GetParentEntity()->SetTransform(m_Transform);
}

void CharacterControllerComponent::Destroy()
{
	CHROMA_TRACE("CharacterController Component : {0} Destroyed.", m_UID.data);
}

CharacterControllerComponent::CharacterControllerComponent()
{
}


CharacterControllerComponent::~CharacterControllerComponent()
{
	delete m_CameraController;
}
