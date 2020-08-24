#include "Texture.h"
#include <resources/TextureLoader.h>
#include <resources/ResourceManager.h>

namespace Chroma
{
	void Texture::Init()
	{
		Chroma::TextureLoader::Initialize2DTexture(m_TextureData);
	}

	void Texture::Bind()
	{
		if (m_TextureData->isInitialized)
			glBindTexture(GL_TEXTURE_2D, m_TextureData->ID);
		else if (m_TextureData->isLoaded && m_TextureData->isInitialized == false)
			Init();
	}

	void Texture::Destroy()
	{
		CHROMA_TRACE("Removing Texture : {0}", m_TextureData->ID);
		glDeleteTextures(1, &m_TextureData->ID);
	}

	void Texture::ClearTexureMemory(unsigned int const& TextureID)
	{
		CHROMA_TRACE("Removing Texture : {0}", TextureID);
		glDeleteTextures(1, &TextureID);
	}

	Texture::Texture(unsigned int newID)
	{
		m_TextureData->ID = newID;
	}

	Texture::Texture(std::string sourcepath)
	{
		LoadFromFile(sourcepath);
	}

	Texture::Texture(std::string sourcePath, Chroma::Type::Texture type)
	{
		m_TextureData->type = type;

		LoadFromFile(sourcePath);
	}

	Texture::Texture(std::shared_ptr<TextureData> textData)
	{
		m_TextureData = textData;
	}


	bool Texture::operator<(const Texture& rhs) const
	{
		return true;
	}

	Texture::~Texture()
	{
		//Destroy();
	}

	void Texture::LoadFromFile(const std::string& sourcePath)
	{
		// Mark uninitialized
		m_TextureData->isInitialized = false;
		m_TextureData->isLoaded = false;
		m_TextureData->sourcePath = sourcePath;
		// Load
		Chroma::ResourceManager::Load2DTexture(sourcePath, m_TextureData);
	}

	unsigned int Texture::GetID()
	{
		if (m_TextureData->isInitialized)
			return m_TextureData->ID;
		else if (m_TextureData->isLoaded && m_TextureData->isInitialized != true)
			Init();
		else
			return 0;
	}

	void Texture::SetID(Texture& refTexture)
	{
		m_TextureData->ID = refTexture.GetID();
		m_TextureData->isLoaded = refTexture.GetTextureData()->isLoaded;
		m_TextureData->isInitialized = refTexture.GetTextureData()->isInitialized;
	}
}

