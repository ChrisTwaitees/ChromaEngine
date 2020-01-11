#include "IEntity.h"

IEntity::IEntity()
{
	m_UID = UID::GenerateNewUID();
}

void IEntity::RebuildTransform()
{
	m_Transform = Chroma::Math::BuildMat4(m_Translation, m_Rotation, m_Scale);
}


void IEntity::SetTransform(glm::mat4 const& newTransformMat)
{
	m_Transform = newTransformMat;
	m_Scale = Chroma::Math::GetScale(newTransformMat);
	m_Translation = Chroma::Math::GetTranslation(newTransformMat);
	m_Rotation = Chroma::Math::GetQuatRotation(newTransformMat);
}

void IEntity::Init()
{
	CalculateBBox();
	CHROMA_TRACE("Entity : {0} Initialized.", m_UID);
}
