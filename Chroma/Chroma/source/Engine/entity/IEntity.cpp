#include "IEntity.h"

IEntity::IEntity()
{
	m_UID = UID::GenerateNewUID();
	std::cout << "New Entity UID" << m_UID << std::endl;
}
