#include "MeshComponent.h"


MeshComponent::MeshComponent()
{
	m_IsRenderable = true;
	m_IsLit = true;
	m_CastShadows = true;
	m_IsTransparent = false;
}


MeshComponent::~MeshComponent()
{
}
