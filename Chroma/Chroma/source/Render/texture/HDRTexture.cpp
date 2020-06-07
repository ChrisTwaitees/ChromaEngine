#include "HDRTexture.h"
#include <resources/ResourceManager.h>


#ifndef STB_IMAGE_IMPLEMENTATION
#include <texture/stb_image.h>
#endif

HDRTexture::HDRTexture(TextureData const& textData)
{
	m_TextureData = textData;
}

HDRTexture::HDRTexture(const std::string& sourcepath)
{
	m_TextureData.type = Chroma::Type::Texture::kHDR;
	LoadFromFile(sourcepath);
}


HDRTexture::~HDRTexture()
{
	//Destroy();
}

void HDRTexture::LoadFromFile(const std::string& sourcePath)
{
	// Mark uninitialized
	m_TextureData.isInitialized = false;
	m_TextureData.isLoaded = false;
	// Load
	Chroma::ResourceManager::LoadHDRTexture(sourcePath, &m_TextureData);
}

void HDRTexture::SetupTexture()
{
	// Mark uninitialized
	m_TextureData.isInitialized = false;

	// Check for Data
	if (m_TextureData.imageData)
	{
		// ID
		glGenTextures(1, &m_TextureData.ID);

		glBindTexture(GL_TEXTURE_2D, m_TextureData.ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_TextureData.width, m_TextureData.height, 0, GL_RGB, GL_FLOAT, &m_TextureData.imageData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Mark initialized
		m_TextureData.isInitialized = true;

		// Cleanup
		stbi_image_free(&m_TextureData.imageData);

		// Log
		CHROMA_INFO("HDR Texture Initialized : {0}", m_TextureData.sourcePath);
	}
	else
	{
		CHROMA_ERROR("ImageData not Loaded from : {0}", m_TextureData.sourcePath);
	}
}
