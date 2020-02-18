#include "Icon.h"
#include <resources/ResourceManager.h>


void Icon::SetIconTexture(std::string const& iconPath)
{
	m_IconTexture.Destroy();
	m_IconDirectory = iconPath;
	m_IconTexture = Chroma::ResourceManager::LoadTexture(m_IconDirectory);
}

void Icon::SetIconTexture(Texture& newTexture)
{
	m_IconTexture.Destroy();
	m_IconDirectory = newTexture.GetSourcePath();
	m_IconTexture = Chroma::ResourceManager::LoadTexture(m_IconDirectory);
}

void Icon::Draw()
{
	CHROMA_INFO("Drawing Icon : {0}", m_UID.data);
	BindDrawVAO();
}

Icon::Icon(std::string const& iconPath)
{
	m_IconDirectory = iconPath;
	m_IconTexture = Chroma::ResourceManager::LoadTexture(m_IconDirectory);

	Initialize();
}

Icon::Icon()
{
	Initialize();
}


Icon::~Icon()
{
}

void Icon::Initialize()
{
	SetupQuad();
	UpdateTransform();
}

void Icon::UpdateTransform()
{
	m_ModelMatrix = glm::mat4();
	m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale));
	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
}
