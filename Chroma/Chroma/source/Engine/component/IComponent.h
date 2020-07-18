#ifndef _CHROMA_COMPONENT_
#define _CHROMA_COMPONENT_

//common
#include <common/PrecompiledHeader.h>

// chroma
#include <uid/UID.h>
#include <common/CoreCommon.h>
#include <serialization/ISerializer.h>

namespace Chroma
{
	class IEntity;
	class IComponent
	{
	public:
		// Functions
		virtual void Init() = 0;
		virtual void OnUpdate() = 0;
		virtual void Destroy() = 0;
		virtual void Serialize(ISerializer*& serializer) = 0;

		// Acessors
		// UID
		UID GetUID() const { return m_UID; };
		void SetUID(const UID& newUID) { m_UID = newUID; };

		// Type
		virtual Chroma::Type::Component GetType() { return m_Type; };
		virtual std::string GetTypeName() { return Chroma::Type::GetName(m_Type); };

		// Name
		std::string GetName() const { return m_Name; };
		void SetName(std::string newName) { m_Name = newName; };

		// Entity
		IEntity* GetParentEntity() const;
		UID GetParentEntityUID() const;
		std::string GetParentEntityName() const;
		virtual void SetParentEntityUID(UID const& newParentEntityUID) { m_ParentEntityUID = newParentEntityUID; }

		IComponent();
		virtual ~IComponent() {};

	protected:
		// UID
		UID m_UID;

		//Name
		std::string m_Name;

		// Serialization
		Chroma::Type::Component m_Type{ Chroma::Type::Component::kIComponent };

		// Parent
		UID m_ParentEntityUID;
	};
}



#ifdef DEBUG
#define CMPNT_SERIALIZE_BEGIN CHROMA_INFO("Serializing : {} , UID : {}", GetTypeName(), m_UID.m_Data); serializer->StartObject(GetType(), m_UID); serializer->AddProperty("m_ParentEntityUID", m_ParentEntityUID);
#define CMPNT_DESTROYED CHROMA_INFO("{} Destroyed. UID : {}", GetTypeName(), m_UID.m_Data );
#define CMPNT_INITIALIZED CHROMA_INFO("{} Intialized. UID : {}", GetTypeName(), m_UID.m_Data );
#else
#define CMPNT_SERIALIZE_BEGIN serializer->StartObject(GetType(), m_UID);
#define CMPNT_DESTROYED
#define CMPNT_INITIALIZED
#endif

#endif