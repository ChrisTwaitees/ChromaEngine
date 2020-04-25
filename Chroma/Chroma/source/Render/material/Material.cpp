#include "Material.h"

void Material::Destroy()
{
	// Textures
	for (Texture& texture : m_TextureSet)
	{
		texture.Destroy();
	}
	m_TextureSet.clear();
	// Shader
	m_Shader.Destroy();
}

void Material::SetTextureSet(std::vector<Texture> newTextureSet)
{
	for (unsigned int i = 0; m_TextureSet.size(); i++)
	{
		bool skip{ false };
		for (unsigned int j = 0; j < m_TextureSet.size(); j++)
		{
			if (std::strcmp(m_TextureSet[j].GetSourcePath().data(), newTextureSet[j].GetSourcePath().data()) == 0)
			{
				skip = true;
				break;
			}
		}
		if (!skip)
			m_TextureSet.push_back(m_TextureSet[i]);
	}
}

void Material::AddTexture(Texture& newTexture)
{
	bool skip{ false };
	for (unsigned int i = 0; i < m_TextureSet.size(); i++)
	{
		skip = false;
		if (std::strcmp(m_TextureSet[i].GetSourcePath().data(), newTexture.GetSourcePath().data()) == 0)
		{
			skip = true;
			break;
		}
	}
	if (!skip)
	{
		m_TextureSet.push_back(newTexture);
	}
}

Material::Material()
{
	m_Shader = Shader("resources/shaders/fragPBR.glsl", "resources/shaders/vertexLitShadowsNormals.glsl");
}

Material::~Material()
{
}
