#include "CharacterControllerComponent.h"
#include <input/Input.h>
#include <entity/Entity.h>
#include <component/AnimationComponent.h>
#include <component/PhysicsComponent.h>


namespace Chroma
{

	PhysicsComponent* CharacterControllerComponent::GetPhysicsComponent()
	{
		for (UID const& physUID : GetParentEntity()->GetPhysicsComponentUIDs())
		{
			return static_cast<PhysicsComponent*>(Scene::GetComponent(physUID));
		}
	}

	void CharacterControllerComponent::ProcessInput()
	{
	
	}

	void CharacterControllerComponent::ProcessCollisions()
	{
	}

	void CharacterControllerComponent::CalculateTransform()
	{
		m_Transform = Math::BuildMat4(m_Position, m_Rotation, m_Scale);
	}

	void CharacterControllerComponent::Init()
	{
		// Set Type
		m_Type = Type::Component::kCharacterControllerComponent;

		m_Scale = GetParentEntity()->GetScale();
		m_Position = GetParentEntity()->GetTranslation();

		CMPNT_INITIALIZED
	}

	void CharacterControllerComponent::Update()
	{
		ProcessInput();
		CalculateTransform();
		GetParentEntity()->SetTransform(m_Transform);
	}

	void CharacterControllerComponent::Destroy()
	{
		delete m_CameraController;
		CMPNT_DESTROYED
	}

	void CharacterControllerComponent::Serialize(ISerializer*& serializer)
	{
		CMPNT_SERIALIZE_BEGIN
	}

	CharacterControllerComponent::CharacterControllerComponent()
	{
	}


	CharacterControllerComponent::~CharacterControllerComponent()
	{
		delete m_CameraController;
	}
}
