#include "CharacterControllerComponent.h"
#include <input/Input.h>
#include <entity/Entity.h>
#include <component/AnimationComponent.h>


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
	//// attach to any animators found
	//for (UID const& animUID : GetParentEntity()->getAnimationComponentUIDs())
	//{
	//	((AnimationComponent*)Chroma::Scene::GetComponent(animUID))->SetCharacterControllerComponentUID(m_UID);
	//}
}

void CharacterControllerComponent::Update()
{
	ProcessInput();
	CalculateTransform();
	GetParentEntity()->SetTransform(m_Transform);
}

void CharacterControllerComponent::Destroy()
{
	CHROMA_TRACE("CharacterController Component : {0} Destroyed.", m_UID.data);
}

void CharacterControllerComponent::Serialize(ISerializer*& serializer)
{
	CHROMA_INFO("Serializing Character Controller Component : {0}", m_UID.data);
	serializer->StartObject("CharacterControllerComponent", m_UID.data.c_str());
}

CharacterControllerComponent::CharacterControllerComponent()
{
}


CharacterControllerComponent::~CharacterControllerComponent()
{
	delete m_CameraController;
}
